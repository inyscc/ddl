#ifndef __DDL__TYPE__HPP__
#define __DDL__TYPE__HPP__

#include <stdio.h>
#include <string>
#include <vector>

struct DDL_String {
private:
  char *_str;
  int _cap;
  int _len;

public:
  DDL_String(int);

  bool equal(const char *);
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

enum DBType { UNKNOWN, INT, CHAR, FLOAT };

struct Column {
private:
  std::string _name;
  DBType _type;

public:
  Column(const std::string &name, const std::string &type) {
    this->_name = name;
    if (type == "INT")
      this->_type = INT;
    else if (type == "CHAR")
      this->_type = CHAR;
    else if (type == "FLOAT")
      this->_type = FLOAT;
    else
      this->_type = UNKNOWN;
  }
  bool is_known() { return this->_type == UNKNOWN; }
  bool is_known(const std::string &type) {
    if (type != "INT" && type != "CHAR" && type != "FLOAT")
      return true;
    return false;
  }
  void chg_name(const std::string &name) { this->_name = name; }
  bool is_name(const std::string &name) { return name == this->_name; }
  void chg_type(const std::string &type) {
    if (type == "INT")
      this->_type = INT;
    else if (type == "CHAR")
      this->_type = CHAR;
    else if (type == "FLOAT")
      this->_type = FLOAT;
    else
      this->_type = UNKNOWN;
  }
  bool chg_info(const std::string &name, const std::string &type) {
    if (this->is_known(type))
      return false;
    this->chg_name(name);
    this->chg_type(type);
    return true;
  }
};

struct Table {
  std::string name;
  std::vector<Column *> column;

  void print() {}
};

#endif // __DDL__TYPE__HPP__