#ifndef __DDL__TYPE__HPP__
#define __DDL__TYPE__HPP__

#include <stdio.h>
#include <vector>

struct DDL_String {
private:
  char *_str;
  int _cap;
  int _len;

public:
  DDL_String(int);

  bool equal(const char*);
  void append(char);
  void to_upper();
  char *String();
  void reset();
  void print();
  void init();
  int len();
  void split(std::vector<DDL_String *> &);

  // 重载运算符
  bool operator==(const DDL_String *&);
};

enum DBType { INT, CHAR, FLOAT };

struct Column {
private:
  char *name;
  DBType type;
};

struct Table {
  char *name;
  std::vector<Column> column;
};

#endif // __DDL__TYPE__HPP__