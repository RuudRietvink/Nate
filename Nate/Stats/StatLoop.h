#pragma once

#include "Stat.h"
#include "StatExpr.h"


namespace nate
{
class StatLoop : public Stat
{
public:
  class While : public StatExpr
  {
  public:
    While(const Location& aLocation, const Expr& aExpr);

    void accept(ICodeVisitor* aVisitor) const override;
  };
  
  class ForStep : public Stat
  {
  public:
    ForStep(const Location& aLocation, const IdentifierPtr& aId, 
						bool aDownTo,
						const Expr& aStart, const Expr& aEnd, const Expr& aStep);

    void accept(ICodeVisitor* aVisitor) const override;
    
    const IdentifierPtr& getId() const;
    bool                 getDownTo() const;
		const Expr&          getStart() const;
    const Expr&          getEnd() const;
    const Expr&          getStep() const;

  private:
    IdentifierPtr mId; 
    bool mDownTo;
    Expr mStart;
    Expr mEnd;
    Expr mStep;
  };
  
  class ForRange : public Stat
  {
  public:
    ForRange(const Location& aLocation, const IdentifierPtr& aId, const Expr& aRange);

    void accept(ICodeVisitor* aVisitor) const override;
    
    const IdentifierPtr& getId() const;
		const Expr&          getRange() const;

  private:
    IdentifierPtr mId; 
    Expr mRange;
  };

  StatLoop(const Location& aLocation);

  void accept(ICodeVisitor* aVisitor) const override;
};

}
