#include "NateParser.h"
#include "core/Core.h"
#include "lex.yy.h"
#include <algorithm>
#include <inttypes.h>
#include <cctype>
#include <tuple>

extern int gDebug;

void NateParser::printLineNr(const yy::parser::location_type& aLocation)
{
	static int prevLine = 0;
	static std::string prevFile;
	std::string curFile = aLocation.begin.filename != nullptr ? *aLocation.begin.filename : mLexer->currentFile();

	if (mLexer->has_matcher())
	{	 
		if (aLocation.begin.line != prevLine + 1 || 
				curFile != prevFile)
		{
			*mOut << "#line " << aLocation.begin.line;
			if (prevFile != curFile)
			{
			  *mOut << " \"" << curFile << "\"";
			}
			
			*mOut << std::endl;

			prevLine = aLocation.begin.line;
			prevFile = curFile;
		}
	}
}

std::string NateParser::codeExpr(const Expr& aValue)
{
	std::string result = aValue.code();
	if (aValue.is(ExprNode::Identifier) && !aValue.is(ExprNode::Property))
	{
		IdentifierPtr id = aValue.id();
		if (id)
		{
			if (id->isObjectMe())
			{
				result = "std::dynamic_pointer_cast<" + toCodeName(id->type()->name()) + ">(shared_from_this())";
			}
			else
			{
				IIdentifiersHolderPtr holder = id->identifiersHolder().lock();
				if (holder)
				{
					if (holder->scopeFlag() == IIdentifiersHolder::ScopeFlag::ObjectImpl)
					{
						if (curDefine() && !curDefine()->is(Define::Undeclared))
						{
							result = "_impl->" + result;
						}
					}
				}
			}
		}
	}

	return result;
}

void NateParser::codeDeclareLocalIdentifier(bool aExtern,
																						const IdentifierPtr& aIdentifier,
																						bool initializeVariables,
																						const yy::parser::location_type& aLocation)
{
	addIdentifier(aIdentifier);
	if (aIdentifier->type()->is(Type::Abstract))
	{
		error("Abstract type: " + aIdentifier->type()->name());
		return;
	}

	if (aIdentifier->type()->is(Type::Unknown))
	{
		error("Unknown type: " + aIdentifier->type()->name());
		return;
	}
	
	//if (aIdentifier->is(Identifier::Const) && !aIdentifier->initValue().is(ExprNode::ConstExpr))
	//{
	//	error("Expected constant expression.");
	//}

	//std::ostream* savedOut = mOut;
	//if (data.inObjectImpl)
	//{
	//	mOut = &curObject()->getImplOut();
	//}

	printLineNr(aLocation);
	if (aExtern)
	{
	  *mOut << in() << "extern ";	
	}

	if (aIdentifier->is(Identifier::Const))
	{
		*mOut << in() << "const ";		
	}

	*mOut << in() << aIdentifier->type()->codeType() << " " << aIdentifier->codeName();

	if (initializeVariables)
	{
		*mOut << " = " << codeExpr(aIdentifier->initValue());
	}
	
	*mOut << ";" << std::endl;

	//mOut = savedOut;
}


void NateParser::codeStartRecord(const RecordPtr& aRecord,
																 const yy::parser::location_type& aLocation)
{
	printLineNr(aLocation);
	*mOut << in() << "struct " << aRecord->codeType() << "\n" << in() << "{" << std::endl;
  data.curRecord.push(aRecord);
	addType(aRecord, aRecord->name());
	pushIdentifiersHolder(aRecord);
}
void NateParser::codeEndRecord(const yy::parser::location_type& aLocation)
{
	RecordPtr record = data.curRecord.top();

	*mOut << in() << record->codeType() << "()" << std::endl;
	bool first = true;

	for (auto& id : record->identifiers().get())
	{
		if (first)
		{
			*mOut << in(1) << ": ";
			first = false;
		}
		else
		{
			*mOut << in(1) << ", ";
		}

		*mOut << id->codeName() << "(";

		if (!id->initValue().is(ExprNode::Default))
		{
			*mOut << codeExpr(id->initValue());
		}
		*mOut << ")" << std::endl;
	}

	*mOut << in(1) << "{}" << std::endl;
	popIdentifiersHolder();
	printLineNr(aLocation);
	*mOut << in() << "};\n" << std::endl;
}

void NateParser::codeObjectBases(const ObjectPtr& aObject)
{
	*mOut << "class " << toCodeName(aObject->name());

	if (aObject->getBases().empty() && !aObject->isRole())
	{
		*mOut << ": public std::enable_shared_from_this<" << toCodeName(aObject->name()) << ">";
	}
	else
	{
		bool first = true;
		for (auto const& base : aObject->getBases())
		{
			*mOut << (first ? ": public " : ", public ") << 
						   (base->isRole() ? "virtual " : "") <<
				       toCodeName(base->name());
			first = false;
		}
	}
}

void NateParser::codeStartDeclObject(const yy::parser::location_type& aLocation)
{	
	printLineNr(aLocation);
	codeObjectBases(curObject());

	*mOut << std::endl;
	*mOut << "{" << std::endl;
	auto name = toCodeName(curObject()->name());
	
	*mOut << "public:" << std::endl;
	
	if (curObject()->isRole())
	{
		*mOut << "  virtual ~" << name << "() = default;" << std::endl;
	}
	else
	{
		*mOut << "  virtual ~" << name << "();" << std::endl;
		*mOut << "  " << name << "();" << std::endl;
		*mOut << "private:" << std::endl;
		*mOut << "  class __impl;"  << std::endl;
		*mOut << "  __impl* _impl;"  << std::endl;
		*mOut << "  friend class __impl;"  << std::endl;
	  *mOut << "public:" << std::endl;
	}
}

void NateParser::codeEndDeclObject()
{
	*mOut << "};\n" << std::endl;
}

void NateParser::codeStartImplObject(const yy::parser::location_type& aLocation)
{	
	//mSavedOut = mOut;
	//mOut = &curObject()->getImplOut();
	
	printLineNr(aLocation);

	if (curObject()->is(Type::ObjectImpl))
	{
		codeObjectBases(curObject());
		*mOut << std::endl;
		*mOut << "{" << std::endl;
		*mOut << "public:" << std::endl;
	}
	else
	{
		auto name = toCodeName(curObject()->name());
		*mOut << "\n" << "class " << name << "::__impl" << std::endl;
		*mOut << "{" << std::endl;
		*mOut << "private:" << std::endl;
		*mOut << "  " << name << "* me;" << std::endl;
		*mOut << "public:" << std::endl;
		*mOut << "  __impl(" << name << "* aMe) : me(aMe) {}" << std::endl;

		//mOut = &curObject()->getNormalOut();
		*mOut << name << "::" << name << "()" << std::endl;
		*mOut << "  : _impl(new __impl(this)) {}\n" << std::endl;
		*mOut << name << "::~" << name << "() { delete _impl; }\n" << std::endl;
	}
}

void NateParser::codeObjectMethodHeaderDecl(const ObjectPtr& aObject, const DefinePtr& aDefine)
{
		const char* startKeys = aDefine->is(Method::Final) || aDefine->is(Method::Overriden)
															? "" : "virtual ";
		const char* endKeys = aDefine->is(Method::Overriden)
															? " override" : "";
		const char* abstract = aObject->isRole()
															? " = 0" : "";
		*mOut << in(-1) << (aDefine->isStatic() ? "static " : startKeys) << 
											  aDefine->createCodeDecl() << endKeys << abstract << ";" << std::endl;
}

void NateParser::codeDeclareProperty(const IdentifierPtr& aId,
																		 const yy::parser::location_type& aLocation)
{
	TypePtr idType = aId->type();
	std::string propType = (idType->is(Type::NeedsRef))
											    ? idType->codeType() + "&"
			                    : idType->codeType();
	std::string declType = idType->codeType();
				
	if (!curObject()->isRole())
	{
		*mOut << in(-1) << "private:" << std::endl;
		printLineNr(aLocation);
		*mOut << in() << declType << " " << aId->codeName() << " = {};" << std::endl;
	}
		
	const char* permisKey = "public";
	*mOut << in(-1) << permisKey << ":" << std::endl;

	bool overriden = curObject()->basesIsPropDeclared(aId, Object::PropType::Get);

	const char* startKeys = aId->is(Identifier::Final) || overriden
			                    ? "" : "virtual ";
	const char* abstractKey = curObject()->isRole()
			                      ? " = 0" : "";
	const char* endKeys = overriden
			                  ? " override" : "";
	printLineNr(aLocation);
	*mOut << in() << startKeys << codePropHeader(false, aId, Object::PropType::Get) << endKeys << abstractKey << ";" << std::endl;

	if (!aId->is(Identifier::ReadOnly))
	{
		printLineNr(aLocation);
		overriden = curObject()->basesIsPropDeclared(aId, Object::PropType::Set);
	  startKeys = aId->is(Identifier::Final) || overriden
			          ? "" : "virtual ";
		endKeys = overriden
			        ? " override" : "";
		*mOut << in() << startKeys << codePropHeader(false, aId, Object::PropType::Set) << endKeys << abstractKey << ";" << std::endl;
	}
}

std::string NateParser::codePropHeader(bool aAddObjectName,
																			 const IdentifierPtr& aId, 
																			 Object::PropType aPropType)
{
	std::stringstream buf;
	
	auto scopeName = aId->type()->typeScopeName();
	std::string propType = (aId->type()->is(Type::NeedsRef))
											    ? scopeName + aId->type()->codeType()
			                    : aId->type()->codeType();
	std::string refType = (aId->type()->is(Type::NeedsRef))
											    ? "&"
			                    : "";
	std::string objectPrefix = aAddObjectName
													   ? toCodeName(curObject()->name()) + "::"
														 : "";

	if (aPropType == Object::PropType::Get)
	{
	  buf << propType << " " << objectPrefix << aId->codeName();
		buf << "_get() const";
	}
	else
	{
	  buf << "const " <<  propType << refType << " " << objectPrefix << aId->codeName();
		buf << "_set(const " << propType << refType << " value)";
	}

	return buf.str();
}

void NateParser::codeDefaultPropertyImpl(const IdentifierPtr& propId)
{
		if (curObject()->getPropState(propId, Object::PropType::Get).state == Object::PropState::State::Declared)
		{
			*mOut << in() << codePropHeader(true, propId, Object::PropType::Get) 
				    << " { return " << propId->codeName() << "; }" << std::endl;
		}

		if (curObject()->getPropState(propId, Object::PropType::Set).state == Object::PropState::State::Declared)
		{
			*mOut << in() << codePropHeader(true, propId, Object::PropType::Set) 
				    << " { return " << propId->codeName() << " = value; }" << std::endl;
		}
}

void NateParser::codeEndImplObject()
{
	//mOut = &curObject()->getNormalOut();

	for (const auto& propMethod : curObject()->propertyMethods())
	{
		codeDefaultPropertyImpl(propMethod.first);
	}

	//mOut = mSavedOut;

	if (curObject()->is(Type::ObjectImpl))
	{		
		*mOut << curObject()->getImplOut().str();
		*mOut << curObject()->getNormalOut().str();
		*mOut << "};\n" << std::endl;
	}
	else
	{
		*mOut << curObject()->getImplOut().str();
		*mOut << "};\n" << std::endl;
		*mOut << curObject()->getNormalOut().str();
	}
}

std::string NateParser::typeScopeName() const
{
	std::string result;

	for (auto const& holder : mTypesHolders)
	{
		auto name = holder->typeScopeName();
		if (!name.empty())
		{
			result += toCodeName(name);
			result += "::";
		}
	}

	return result;
}


std::string NateParser::codeId(const std::string& aName, Scope* aScope)
{
	return getOrFakeIdentifier(aName, aScope)->codeName();
}

void NateParser::codeWriteStart(const Expr& aValue, const yy::parser::location_type& aLocation)
{
	mDataOutput = false;

	if (aValue.isEmpty())
	{
		if (!mLastWriteStream.empty())
		{
			mStream = mLastWriteStream;
		}
		else
		{
			error("Need to specify where to write to");
		}
	}
	else if (aValue.type()->isOfType("output"))
	{
		mStream = "*" + codeExpr(aValue);
		mLastWriteStream = mStream;
	}
	else
	{
		error("Cannot write to type: " + aValue.type()->name());
	}

	printLineNr(aLocation);
	mFirstOutput = true;
	mStartOutput = true;
	mCachedOutput.clear();
}

void NateParser::codeReadStart(const Expr& aValue, const yy::parser::location_type& aLocation)
{
	if (aValue.isEmpty())
	{
		if (!mLastReadStream.empty())
		{
			mStream = mLastReadStream;
		}
		else
		{
			error("Need to specify where to read from");
		}
	}
	else if (aValue.type()->isOfType("input"))
	{
		mStream = "*" + codeExpr(aValue);
		mLastReadStream = mStream;
	}
	else
	{
		error("Cannot read from type: " + aValue.type()->name());
	}

	printLineNr(aLocation);
	*mOut << in() << mStream;
}


void NateParser::codeInputStart(const std::string& aStream, const yy::parser::location_type& aLocation)
{
	mStream = aStream;
	printLineNr(aLocation);
	*mOut << in() << aStream;
}

void NateParser::codeInputSpace()
{
}

void NateParser::codeInputNoSpace()
{
}

void NateParser::codeInput(const Expr& aValue)
{
	if (aValue.is(ExprNode::Output) && !aValue.is(ExprNode::ConstExpr))
	{
		if (aValue.type() && aValue.type()->is(Type::Boolean))
		{
			*mOut << ">> std::boolalpha ";
		}
	
		*mOut << ">> " << codeExpr(aValue);
	}
	else
	{
		error("Expected non-constant variable for input");
	}
}

void NateParser::codeInputEnd(bool aAddEnd)
{
	*mOut << ";";
	if (aAddEnd)
	{
		*mOut << "(" << mStream << ").ignore(std::numeric_limits<std::streamsize>::max(), '\\n');";
	}
	*mOut << std::endl;
}

 void NateParser::codeReturn(const Expr& aValue, const yy::parser::location_type& aLocation)
 {
	 printLineNr(aLocation);
	 *mOut << in() << "return " << codeExpr(aValue) << ";" << std::endl;
 }

 void NateParser::codeExpressionStatement(const Expr& aValue, const yy::parser::location_type& aLocation)
 {
	 printLineNr(aLocation);
	 *mOut << in() << codeExpr(aValue) << ";" << std::endl;
 }