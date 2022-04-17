#include "parse.hpp"
#include "type.hpp"
#include <cstdio>
#include <iostream>
#include <map>
#include <regex>

const char *CMD_CREATE = "CREATE";
const char *CMD_ALTER = "ALTER";
const char *CMD_DROP = "DROP";
const char *CMD_SHOW = "SHOW";

const std::basic_regex<char> REG_CREATE_TABLE =
    std::regex("^CREATE TABLE (A-Z_)+ (IF NOT EXISTS )?\\( [A-Z]+ "
               "(INT|CHAR|FLOAT) \\) ;$");
const std::basic_regex<char> REG_MODIFY_COLUMN = std::regex("");
const std::basic_regex<char> REG_ADD_COLUMN = std::regex("");
const std::basic_regex<char> REG_DROP_COLUMN = std::regex("");
const std::basic_regex<char> REG_MODIFY_TABLE = std::regex("");
const std::basic_regex<char> REG_SHOW_TABLES = std::regex("");
const std::basic_regex<char> REG_SHOW_CREATE_TABLE = std::regex("");

std::map<std::string, Table *> mp;

// 处理用户输入
void process_input(DDL_String *dstr) {
  std::vector<DDL_String *> vstr;
  dstr->split(vstr);
  int idx = 0;

  if (vstr.size() <= 1)
    printf("不知是何的输入\n");

  auto it = vstr[0];
  if (it->equal(CMD_CREATE))
    create_table(vstr);
  else if (it->equal(CMD_ALTER))
    modify_table(vstr);
  else if (it->equal(CMD_DROP))
    drop_table(vstr);
  else if (it->equal(CMD_SHOW))
    show_table(vstr);
  else {
    dstr->print();
    printf("\n不知是何的输入\n");
  }
}

// ----- 创建表 -----
// create table name ( ) ;
void create_table(std::vector<DDL_String *> &vstr) {
  int tot = vstr.size();
  if (tot < 6) {
    printf("unparseable table creation statement\n");
    return;
  }

  std::string str;
  for (auto it : vstr) {
    char *s = it->String();
    for (int i = 0; i < it->len(); ++i)
      str.push_back(s[i]);
    str.push_back(' ');
  }
  str.pop_back(); // 减除最后一个空格

  printf("SQL: |%s|\n", str.c_str());

  std::smatch res;
  bool isMatch = std::regex_match(str, res, REG_CREATE_TABLE);
  if (!isMatch) {
    printf("can not regex match[%s]\n", str.c_str());
    printf("can not regex match[");
    std::cout << "^CREATE TABLE [A-Z_]+ \\( [A-Z]+ "
               "(INT|CHAR|FLOAT) \\) ;$" << "]\n";
    return;
  }
  for (int i = 0; i < res.size(); ++i)
    printf("res[%d]:  |%s|\n", i, res[i].str().c_str());

  printf("\n");
}

// ----- 修改表 -----
void modify_table(std::vector<DDL_String *> &vstr) {
  for (auto it : vstr) {
    it->print();
    printf("-");
  }
  printf("\n");
}

// 修改列信息
void modify_column(std::vector<DDL_String *> &vstr) {}

// 添加新列
void add_column(std::vector<DDL_String *> &) {}

// 删除列
void drop_column(std::vector<DDL_String *> &) {}

// 修改表名
void modify_table_name(std::vector<DDL_String *> &) {}

// 删除表
void drop_table(std::vector<DDL_String *> &) {}

// ----- 查询 -----
void show_table(std::vector<DDL_String *> &vstr) {
  for (auto it : vstr) {
    it->print();
    printf("-");
  }
  printf("\n");
}

// 查看全部表名
void show_tables(std::vector<DDL_String *> &) {}

// 查询表信息
void show_create(std::vector<DDL_String *> &) {}
