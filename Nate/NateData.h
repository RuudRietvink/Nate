#pragma once

#include "Expr.h"
#include "NateParser.tab.h"

#include <memory>

struct Location
{
	Location(const yy::parser::location_type& aLocation, const std::string& aCurFile)
		: filename(aLocation.begin.filename != nullptr ? *aLocation.begin.filename : aCurFile),
		  beginLine(aLocation.begin.line),
		  beginColumn(aLocation.end.column)
	{
	}

	std::string filename;
	int beginLine;
	int beginColumn;
};

enum class ByteCode
{
	Program,
	Expr,
	Block,
	StdOutput,
	OutputSepComma,
	OutputSepConcat,
};

struct TreeNode
{
	TreeNode(ByteCode code, const Location& aLocation) 
		: code(code),
		  location(aLocation)
	{}

	TreeNode* add(ByteCode code, const Location& aLocation, TreeNode* back = nullptr)
	{
		//std::cout << this << " add " << (int) code << std::endl;
		auto node = std::make_shared<TreeNode>(code, aLocation);
		nested.push_back(node);
		node->back = back;
		return node.get();
	}

	TreeNode* addExpr(const Expr& expr, const Location& aLocation)
	{
		//std::cout << this << " addExpr " << expr << std::endl;
		auto node = std::make_shared<TreeNode>(ByteCode::Expr, aLocation);
		nested.push_back(node);
		node->expr = expr;
		return node.get();
	}


	ByteCode code;
	std::vector<std::shared_ptr<TreeNode>> nested;
	TreeNode* back = nullptr;
	Expr expr;
	Location location;
};