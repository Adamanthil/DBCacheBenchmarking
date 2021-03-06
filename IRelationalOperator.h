#ifndef I_RELATIONAL_OPERATOR_H_
#define I_RELATIONAL_OPERATOR_H_

#include <vector>
#include <fstream>

#include "MemoryBlock.h"
#include "Schema.h"
#include "MaterializationLayout.h"

typedef std::vector<const Attribute *> ColumnList;

// Interface for Relational Operators (eg SequentialScan, Joins, Projection, etc)
// Responsible for deleting any children IRelationalOperators
class IRelationalOperator
{
 public:
  virtual const Schema * schema() const = 0;
  virtual bool moveNext() = 0;
  virtual void next(MemoryBlock & buffer) = 0;

  virtual void reset() = 0;
  virtual void dump(std::ostream & strm,
		    char fs = '|', char rs = '\n') {}

  virtual void layout(const MaterializationLayout * layout) = 0;
  virtual ~IRelationalOperator() = 0;

  /*
  virtual void open() = 0;
  virtual void close() = 0;
  */
};

#endif
