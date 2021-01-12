#pragma once

#include "Expr.h"
#include "NateParser.tab.h"

#include <memory>

struct Location
{
	Location()
	{}

	Location(const yy::parser::location_type& aLocation, const std::string& aCurFile)
		: filename(aLocation.begin.filename != nullptr ? *aLocation.begin.filename : aCurFile),
		  beginLine(aLocation.begin.line),
		  beginColumn(aLocation.end.column)
	{
	}

	std::string filename;
	int beginLine = -1;
	int beginColumn = -1;
};

enum class ByteCode
{
	Code,
	Program,
	Expr,
	Block,
	StdOutput,
	OutputSepComma,
	OutputSepConcat,
	LocalVar,
	Assign,
	If,
	Else,
	ElseIf,
	EndIf,
	IfIs,
	CaseIsList,
	CaseIs,
	ElseIs,
	LoopStart,
	LoopStartForStep,
	LoopStartForRange,
	While
};

struct TreeNode
{
	TreeNode(ByteCode code) 
		: code(code)
	{}

	TreeNode(ByteCode code, const Location& aLocation, TreeNode* back = nullptr) 
		: code(code),
		  location(aLocation),
		  back(back)
	{}

	TreeNode* add(ByteCode code, const Location& aLocation, TreeNode* back = nullptr)
	{
		//std::cout << this << " add " << (int) code << std::endl;
		auto node = std::make_shared<TreeNode>(code, aLocation, back);
		nested.push_back(node);
		return node.get();
	}
	
	TreeNode* add(ByteCode code, const Expr& expr, const Location& aLocation, TreeNode* back = nullptr)
	{
		//std::cout << this << " add " << (int) code << std::endl;
		auto node = std::make_shared<TreeNode>(code, aLocation, back);
		node->expr = expr;
		nested.push_back(node);
		return node.get();
	}

	ByteCode code;
	std::vector<std::shared_ptr<TreeNode>> nested;
	TreeNode* back = nullptr;
	Expr expr;
	Expr expr2;
	Expr expr3;
	std::vector<Expr> exprList;
	Location location;
	IdentifierPtr id;
	bool bool1 = false;
};

using TreeNodePtr = std::shared_ptr<TreeNode>;