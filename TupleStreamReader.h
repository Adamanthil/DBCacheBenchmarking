#ifndef TUPLE_STREAM_READER_H_
#define TUPLE_STREAM_READER_H_

#include "Attribute.h"
#include "MemoryBlock.h"
#include "MaterializationLayout.h"
#include "Tuple.h"
#include "globals.h"

class TupleStreamReader
{
 private:
  int m_nRecs;
  int m_pos;
  const MaterializationLayout * m_layout;
  MemoryBlock & m_block;
 public:
  TupleStreamReader(MemoryBlock & block);
  ~TupleStreamReader();
  void layout(const MaterializationLayout * layout);

  void read(Tuple & t);
  bool isEndOfStream();

  void reset();
};

#endif
