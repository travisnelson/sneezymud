// tool.cc

#include "stdsneezy.h"
#include "obj_tool.h"

TTool::TTool() :
  TObj(),
  tool_type(MIN_TOOL_TYPE),
  tool_uses(0),
  max_tool_uses(0)
{
}

TTool::TTool(const TTool &a) :
  TObj(a),
  tool_type(a.tool_type),
  tool_uses(a.tool_uses),
  max_tool_uses(a.tool_uses)
{
}

TTool & TTool::operator=(const TTool &a)
{
  if (this == &a) return *this;
  TObj::operator=(a);
  tool_type = a.tool_type;
  tool_uses = a.tool_uses;
  max_tool_uses = a.max_tool_uses;
  return *this;
}

TTool::~TTool()
{
}

toolTypeT TTool::getToolType() const
{
  return tool_type;
}

void TTool::setToolType(toolTypeT r)
{
  tool_type = r;
}

int TTool::getToolUses() const
{
  return tool_uses;
}

void TTool::setToolUses(int r)
{
  tool_uses = r;
}

void TTool::addToToolUses(int r)
{
  tool_uses += r;
}

int TTool::getToolMaxUses() const
{
  return max_tool_uses;
}

void TTool::setToolMaxUses(int r)
{
  max_tool_uses = r;
}

void TTool::addToToolMaxUses(int r)
{
  max_tool_uses += r;
}

void TTool::assignFourValues(int x1, int x2, int x3, int)
{
  if (x1 < MIN_TOOL_TYPE || x1 >= MAX_TOOL_TYPE) {
    vlogf(LOG_LOW, "Bad toolTypeT value %d on %s!", x1, getName());
    x1 = MIN_TOOL_TYPE;
  }

  setToolType(toolTypeT(x1));
  setToolUses(x2);
  setToolMaxUses(x3);
}

void TTool::getFourValues(int *x1, int *x2, int *x3, int *x4) const
{
  *x1 = getToolType();
  *x2 = getToolUses();
  *x3 = getToolMaxUses();
  *x4 = 0;
}

string TTool::statObjInfo() const
{
  char buf[256];

  sprintf(buf, "Tool type: %d, Strength: %d, Max: %d",
                getToolType(),
                getToolUses(), getToolMaxUses());

  string a(buf);
  return a;
}

int TTool::objectSell(TBeing *ch, TMonster *keeper)
{
  char buf[256];

  if ((getToolUses() != getToolMaxUses())) {
    sprintf(buf, "%s I'm sorry, I don't buy back used tools.", ch->getName());
    keeper->doTell(buf);
    return TRUE;
  }
  return FALSE;
}

