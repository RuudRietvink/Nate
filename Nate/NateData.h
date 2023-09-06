#pragma once

#include "Expr.h"
#include "Define.h"
#include "Location.h"

#include <memory>

enum class ByteCode
{
	None,
	Code,
	Program,
	Expr,
	ExprStat,
	Block,
	StdOutput,
	StdError,
	StdInput,
	Data,
	Write,
	Read,
	SepComma,
	SepConcat,
	End,
	LocalVar,
	Assign,
	IfThen,
	If,
	Else,
	ElseIf,
	IfIs,
	CaseIsList,
	CaseIs,
	ElseIs,
	LoopStart,
	LoopStartForStep,
	LoopStartForRange,
	While,
	Scope,
	CodeInclude,
	Record,
	Define,
	Return,
	DeclObject,
	DeclObjectDefine,
	ImplObject,
	Prop,
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
	TypePtr type;
	ObjectPtr object;
	DefinePtr defyne;
	bool bool1 = false;
	std::string string;
	InputType inputType = InputType::Normal;
};

using TreeNodePtr = std::shared_ptr<TreeNode>;