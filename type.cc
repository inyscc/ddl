#include "type.hpp"
#include <cstddef>

const int DEFAULT_LEN = 1024; // 默认长度

DDL_String::DDL_String(int cap) { this->init(); }

void DDL_String::append(char ch) {
  if (this->_len == this->_cap) {
    // 字符串的长度等于容量时，说明已经满了
    // 需要扩容  容量加倍
    if (this->_cap == 0)
      this->init();
    else {
      // 重新分配
      this->_cap *= 2;
      char *s = new char[this->_cap];
      for (int i = 0; i < this->_len; ++i)
        s[i] = this->_str[i];
      delete[] this->_str;
      this->_str = s;
    }
  }
  this->_str[this->_len++] = ch;
}

void DDL_String::to_upper() {
  for (int i = 0; i < this->_len; i++) {
    if (this->_str[i] >= 'a' && this->_str[i] <= 'z')
      this->_str[i] -= ('a' - 'A');
  }
}

void DDL_String::reset() { this->_len = 0; }

void DDL_String::init() {
  this->_len = 0;
  this->_cap = DEFAULT_LEN;
  this->_str = new char[DEFAULT_LEN];
}

char *DDL_String::String() { return this->_str; }

int DDL_String::len() { return this->_len; }

void DDL_String::print() {
  for (int i = 0; i < this->_len; ++i)
    printf("%c", this->_str[i]);
}

void DDL_String::split(std::vector<DDL_String *> &vstr) {
  DDL_String *dstr = nullptr;
  for (int i = 0; i < this->_len; ++i) {
    char ch = this->_str[i];
    if (ch != ' ' && ch != '\n' && ch != '\t') {
      if (dstr == nullptr) {
        dstr = new DDL_String(256);
        vstr.push_back(dstr);
      }
      dstr->append(ch);
    } else {
      dstr = nullptr;
    }
  }
}

bool DDL_String::operator==(const DDL_String *&dstr) {
  if (this->_len != dstr->_len)
    return false;

  for (int i = 0; i < this->_len; ++i)
    if (this->_str[i] != dstr->_str[i])
      return false;

  return true;
}

bool DDL_String::equal(const char *str) {
  for (int i = 0; str[i]; ++i)
    if (i >= this->_len || this->_str[i] != str[i])
      return false;
  return true;
}