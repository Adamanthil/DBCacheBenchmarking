#ifndef COLUMN_H_
#define COLUMN_H_

#include <vector>
#include <string>

struct Column
{
public:
  std::string m_name;
  std::string m_table;
  std::string m_qualified_name;
};

class Columns : private std::vector<Column *>
{
 public:
  ~Columns();
  void add(const std::string & name, const std::string table);
  const Column * at(int idx) const;
  const Column * operator[](int idx) const;
  int count() const;
  bool contains(const std::string & column) const;
};

#endif
