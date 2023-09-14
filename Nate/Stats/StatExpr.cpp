#include "StatExpr.h"
#include "ICodeVisitor.h"

StatExpr::StatExpr(const Location& aLocation, const Expr& aExpr)
  : StatWithExpr(aLocation, aExpr)
{}
