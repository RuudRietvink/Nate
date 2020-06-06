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

void NateParser::codeStartProgram(const yy::parser::location_type& aLocation)
{
	*mOut << in() << "#define NOMINMAX" << std::endl;
	*mOut << in() << "#include <windows.h>" << std::endl;

	printLineNr(aLocation);

	*mOut << in() << "int main(int argc, char** argv)\n" << in() << "{" << std::endl;
	pushScope(std::make_shared<Scope>("main", IIdentifiersHolder::ScopeFlag::Local));
	*mOut << in() << "output = std::shared_ptr<std::ostream>(&std::cout, [](void*) {});" << std::endl;
	*mOut << in() << "error = std::shared_ptr<std::ostream>(&std::cerr, [](void*) {});" << std::endl;
	*mOut << in() << "SetConsoleOutputCP(65001);" << std::endl;
	//*mOut << "std::locale::global(std::locale(\"en_US.UTF8\"));" << std::endl;
    
}

void NateParser::codeEndProgram(const yy::parser::location_type& aLocation)
{
	popScope();
	printLineNr(aLocation);
	*mOut << in() << "}\n" << std::endl;
}

void NateParser::codeStartScope()
{
	*mOut << in() << "{" << std::endl;
	pushScope(std::make_shared<Scope>("scope", IIdentifiersHolder::ScopeFlag::Local));
}

void NateParser::codeEndScope()
{
	popScope();
	*mOut << in() << "}\n" << std::endl;
}

void NateParser::codeCodeInclude()
{
	*mOut << in() << mCodes.back()->code() << std::endl;
	mCodes.pop_back();
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

	std::ostream* savedOut = mOut;
	if (data.inObjectImpl)
	{
		mOut = &curObject()->getImplOut();
	}

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
		*mOut << " = " << codeExpr(*aIdentifier->initValue());
	}
	
	*mOut << ";" << std::endl;

	mOut = savedOut;
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

		if (!id->initValue()->is(ExprNode::Default))
		{
			*mOut << codeExpr(*id->initValue());
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
	mSavedOut = mOut;
	mOut = &curObject()->getImplOut();
	
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

		mOut = &curObject()->getNormalOut();
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
	mOut = &curObject()->getNormalOut();

	for (const auto& propMethod : curObject()->propertyMethods())
	{
		codeDefaultPropertyImpl(propMethod.first);
	}

	mOut = mSavedOut;

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

void NateParser::codeAssign(const std::vector<Expr>& aExpressions,
														Expr& aValue,
														const yy::parser::location_type& aLocation)
{
	std::string endPars;

	printLineNr(aLocation);
	for (auto const& expr : aExpressions)
	{
		if (expr.is(ExprNode::ConstExpr))
		{
			error("Cannot assign to a constant or readonly");
		}
		else if (!expr.is(ExprNode::Output))
		{
			error("Cannot assign to a non-variable");
		}

		const TypePtr& exprType = expr.type();
		bool ok = aValue.node().castToType(exprType);
		if (!ok)
		{
			error("cannot cast '" + aValue.text() + "' of type " + aValue.type()->name() + " to type " + exprType->name());
		}
				
		std::string code = codeExpr(expr);
		size_t size = code.size();
		if (size > 6 && code.substr(size - 6, 6) == "_get()")
		{
			code[size - 5] = 's';
			code[size - 1] = '\0';
			endPars += ")";
			*mOut << in() << code;
		}
		else
		{
			*mOut << in() << code << " = ";
		}
	}

	*mOut << codeExpr(aValue) << endPars << ";" << std::endl;
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
	mCachedOutput.clear();
}

void NateParser::codeDataStart(const std::string& aId, const yy::parser::location_type& aLocation)
{
	mDataOutput = true;
	printLineNr(aLocation);
	mDataOutput = true;
	IdentifierPtr id = std::make_shared<Identifier>(curIdentifiersHolder(), aId, determineType("text"));
	*mOut << "std::ostringstream " << id->codeName() << "_temp;";
	*mOut << id->codeName() << "_temp ";
}

void NateParser::codeDataEnd(const std::string& aId, const yy::parser::location_type& aLocation)
{
	printLineNr(aLocation);
	IdentifierPtr id = std::make_shared<Identifier>(curIdentifiersHolder(), aId, determineType("text"));
	addIdentifier(id);
	*mOut << "; " << id->type()->codeType() << " " << id->codeName() << "= " << id->codeName() << "_temp" << ".str();" << std::endl;
}

void NateParser::codeDataOutputEnd(bool aAddEnd)
{
	if (aAddEnd)
	{
		codeOutput("std::endl");
	}
	else
	{
		codeOutput("");
	}
}

void NateParser::codeOutputStart(const std::string& aStream, const yy::parser::location_type& aLocation)
{
	mDataOutput = false;
	mStream = aStream;
	printLineNr(aLocation);
	mFirstOutput = true;
	mCachedOutput.clear();
}

void NateParser::codeOutputNew()
{
	if (mFirstOutput && !mDataOutput)
	{
		*mOut << in() << mStream;
		mFirstOutput = false;
	}
}

void NateParser::codeOutput(const std::string& aString)
{
	if ((!mCachedOutput.empty()) && aString[0] != '"')
	{
		codeOutputNew();
		*mOut << " << \"" << mCachedOutput << "\"";
		mCachedOutput.clear();
		if (!aString.empty())
		{
			codeOutputNew();
			*mOut << " << " << aString << (mDataOutput ? "" : ";");
			mFirstOutput = true;
		}
		else
		{
			*mOut << (mDataOutput ? "" : ";");
		}
	}
	else if (!mCachedOutput.empty())
	{
		mCachedOutput += unquote(aString);
	}
	else if (aString[0] == '"')
	{
		mCachedOutput = unquote(aString);
	}
	else
	{
		if (!aString.empty())
		{
			codeOutputNew();
			*mOut << " << " << aString << (mDataOutput ? "" : ";");
			mFirstOutput = true;
		}
	}
}

void NateParser::codeOutput(const Expr& aValue)
{
	if (aValue.type() && aValue.type()->is(Type::Boolean))
	{
		codeOutput("std::boolalpha ");
	}
	
	if (aValue.is(ExprNode::Literal))
	{
		codeOutput(codeExpr(aValue));
	}
	else
	{
		if (aValue.type() && aValue.type()->name() == "int-8")
		{
			codeOutput("static_cast<int>(" + codeExpr(aValue) + ")");
		}
		else
		{
			Expr outExpr(Expr("stream-out"), aValue);
			Expr resExpr = evaluate(outExpr);
			if (!resExpr.isEmpty())
			{
				codeOutput("(" + codeExpr(resExpr) + ")");
			}
			else
			{
				codeOutput("(" + codeExpr(aValue) + ")");
			}
		}
	}
}

void NateParser::codeOutputEnd(bool aAddEnd)
{
	if (aAddEnd)
	{
		codeOutputNew();
		codeOutput("std::endl");
	}
	else
	{
		codeOutput("");
	}

	*mOut << std::endl;
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
		*mOut << mStream << ".ignore(std::numeric_limits<std::streamsize>::max(), '\\n');";
	}
	*mOut << std::endl;
}

void NateParser::NateParser::codeIf(const Expr& aValue, const yy::parser::location_type& aLocation)
{
	printLineNr(aLocation);
	if (!aValue.type()->is(Type::Boolean))
	{
		error("Expected boolean expression for IF statement");
	}

	*mOut << in() << "if (" << codeExpr(aValue) << ")\n" << in() << "{" << std::endl;
	pushScope(std::make_shared<Scope>("if", IIdentifiersHolder::ScopeFlag::Local));
}

void NateParser::codeElseIf()
{
	*mOut << in() << "else " << std::endl;
}

void NateParser::codeElse(const yy::parser::location_type& aLocation)
{
	printLineNr(aLocation);
	*mOut << in() << "else\n" << in() << "{" << std::endl;
	pushScope(std::make_shared<Scope>("else", IIdentifiersHolder::ScopeFlag::Local));
}

void NateParser::codeEndIf()
{
	popScope();
	*mOut << in() << "}" << std::endl;
}

void NateParser::codeIfIs(const Expr& aValue, const std::string& idName, const yy::parser::location_type& aLocation)
{
	printLineNr(aLocation);
	IfIs info;
	info.idName = idName;
	info.isSwitch = aValue.type()->is(Type::Scalar);
	info.out = std::make_shared<std::ostringstream>();
	info.savedOut = mOut;
	mIfIs.push(info);
	
	*mOut << in() << "auto const " << idName << " = " << codeExpr(aValue) << ";" << std::endl;
}

void NateParser::codeIs(const Expr& aValue, const Expr& aIfExpr, const yy::parser::location_type& aLocation)
{
	auto ifIs = mIfIs.top();
	if (!(aValue.type()->isOfType(aIfExpr.type()->name()) ||
				aValue.type()->is(Type::Real) == aIfExpr.type()->is(Type::Real)))
	{
		error("Expected expression with same type as in IF");
	}

	auto prevCase = ifIs.nextCase;
	ifIs.nextCase = (ifIs.isSwitch && aValue.is(ExprNode::ConstExpr) && ifIs.isFirstTest);

	if (ifIs.isFirstTest && prevCase && !ifIs.nextCase)
	{
		error("Non-constant expression needs to be first in multi-IS");
	}

	if (ifIs.nextCase)
	{
		mOut = ifIs.out.get();
		printLineNr(aLocation);
		*mOut << in() << "case " << codeExpr(aValue) << ":" << std::endl;
	}
	else
	{
		bool firstExpr = ifIs.isFirstTest;
		
		if (!ifIs.isFirst && firstExpr)
		{
			printLineNr(aLocation);
			*mOut << in() << "else ";
		}

		if (firstExpr)
		{
			if (ifIs.isFirst)
			{
				printLineNr(aLocation);
			}

			*mOut << in() << "if (";
		}
		else
		{
			*mOut << std::endl;			
			printLineNr(aLocation);
			*mOut << in(2) << " || ";
		}
		
		if (ifIs.isFirst || firstExpr)
		{
			ifIs.isFirst = false;
			ifIs.isFirstTest = false;
		}

		*mOut << "(" << mIfIs.top().idName << " == " << codeExpr(aValue) << ")";
	}

	mIfIs.pop();
	mIfIs.push(ifIs);
}

void NateParser::codeElseIs(const yy::parser::location_type& aLocation)
{
	printLineNr(aLocation);
	auto ifIs = mIfIs.top();
	if (!ifIs.isFirst)
	{
		*mOut << in() << "else" << std::endl;
	}

	ifIs.nextCase = ifIs.isSwitch && !ifIs.out->str().empty();
	if (ifIs.nextCase)
	{
		*mOut << in() << "switch (" << ifIs.idName << ")\n" << in() << "{" << std::endl;
		*mOut << in() << ifIs.out->str();
		*mOut << in() << "default:" << std::endl;
		mOut = ifIs.savedOut;
	}
	else
	{
		ifIs.nextElse = true;
	}

	mIfIs.pop();
	mIfIs.push(ifIs);
}

void NateParser::codeBeginIs()
{
	auto ifIs = mIfIs.top();

	if (!ifIs.nextCase && !ifIs.nextElse)
	{
		*mOut << ")" << std::endl;
	}
	
	*mOut << in() << "{" << std::endl;
	pushScope(std::make_shared<Scope>("is", IIdentifiersHolder::ScopeFlag::Local));
}

void NateParser::codeEndIs(const yy::parser::location_type& aLocation)
{
	auto ifIs = mIfIs.top();
	printLineNr(aLocation);

	if (ifIs.nextCase)
	{
		*mOut << in() << "break;" << std::endl;
	}

	popScope();
	*mOut << in() << "}" << std::endl;

	if (ifIs.nextCase)
	{
		mOut = ifIs.savedOut;
	}
	
	mIfIs.pop();
	ifIs.isFirstTest = true;
	ifIs.nextCase = false;
	mIfIs.push(ifIs);
}

void NateParser::codeEndIfIs(const yy::parser::location_type& aLocation)
{
	auto ifIs = mIfIs.top();
	printLineNr(aLocation);
	if (mIfIs.top().isSwitch && !ifIs.out->str().empty())
	{
		*mOut << in() << "}" << std::endl;
	}

	mIfIs.pop();
}

void NateParser::codeInitLoop(const yy::parser::location_type& aLocation)
{
	printLineNr(aLocation);
	mLoopWhileCounts.push_back(0);
	pushScope(std::make_shared<Scope>("while", IIdentifiersHolder::ScopeFlag::Local));
}

void NateParser::codeStartLoop()
{
	*mOut << in(-1) << "while (true)\n" << in(-1) << "{" << std::endl;
}

void NateParser::codeStartForStepLoop(const std::string& aId,
																			const TypePtr& aType,   
																			bool aDownTo,
																			const Expr& aStart,
																			const Expr& aEnd,
																			const Expr& aStep)
{
	TypePtr type = aType->empty()
							   ? aStart.type()
							   : aType;

	IdentifierPtr id = std::make_shared<Identifier>(curIdentifiersHolder(), aId, type);
	addIdentifier(id);

	*mOut << in(-1) << "for (" << id->type()->codeType() << " " 
			 << id->codeName() << "= " << codeExpr(aStart) << ";" 
			 << id->name() << (aDownTo ? " >= " : "<=") << codeExpr(aEnd) << ";"
			 << id->name() << (aDownTo ? " -= " : "+=") << codeExpr(aStep) << ")\n" << in(-1) << "{" << std::endl;
}

void NateParser::codeStartForRangeLoop(const std::string& aId, 
																			 const Expr& aRange)
{
	TypePtr rangeType = aRange.type();
	if (rangeType->isOfType("container"))
	{
		TypePtr type = rangeType->typenameType();
		IdentifierPtr id = std::make_shared<Identifier>(curIdentifiersHolder(), aId, type);
		addIdentifier(id);

		auto range = uniqueName();
		auto iter = uniqueName();
		auto next = uniqueName();

		std::string ref = aRange.is(ExprNode::Output) ? "&" : "";
		std::string increment;

		if (rangeType->isOfType("text"))
		{
			increment = iter + "=" + next;
			*mOut << in(-1) << "const std::string" << ref << " " << range << " = " << codeExpr(aRange) << ";" << std::endl; 
			*mOut << in(-1) << "auto " << next << " = " << range << ".cbegin();" << std::endl;
		}
		else
		{
			increment = "++" + iter;
			*mOut << in(-1) << "auto const" << ref << " " << range << " = " << codeExpr(aRange) << ";" << std::endl; 
		}

		*mOut << in(-1) << "for (auto " << iter << " = " << range << ".cbegin(); "
				 << iter << " != " << range << ".cend(); "
			   << increment << ")\n" << in(-1) << "{" << std::endl;
		if (rangeType->isOfType("text"))
		{
			*mOut << in() << "uint32_t " << id->codeName() << " = utf8::next(" + next + "," + range + ".cend());" << std::endl;
		}
		else
		{
			*mOut << in() << "auto const& " << id->codeName() << " = *" << iter << ";" << std::endl;
		}
	}
	else
	{
		error("Range must be a container, got: " + rangeType->name());
	}
}

void NateParser::codeEndLoop()
{
	mLoopWhileCounts.pop_back();
	popScope();
	*mOut << in() << "}" << std::endl;
}

void NateParser::codeLoopWhile(const Expr& aExpr, const yy::parser::location_type& aLocation)
{
	if (mLoopWhileCounts.back() > 0)
	{
		error("Only one while allowed in a loop.");
	}

	++mLoopWhileCounts.back();

	if (!aExpr.type()->is(Type::Boolean))
	{
		error("Expected boolean condition in while.");
	}
		
	printLineNr(aLocation);
	*mOut << in() << "if (!(" << codeExpr(aExpr) << ")) break;" << std::endl;
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