#include "StatProgram.h"
#include "ICodeVisitor.h"

StatProgram::StatProgram(const Location& aLocation)
  : Stat(aLocation)
{}

void StatProgram::accept(ICodeVisitor* aVisitor) const
{
  aVisitor->visit(*this);
}