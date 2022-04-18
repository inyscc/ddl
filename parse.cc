#include "parse.hpp"
#include "type.hpp"

#include <cstdio>
#include <iostream>
#include <map>
#include <regex>
#include <string>

const char *CMD_CREATE = "CREATE";
const char *CMD_ALTER = "ALTER";
const char *CMD_DROP = "DROP";
const char *CMD_SHOW = "SHOW";

const std::basic_regex<char> REG_CREATE_TABLE =
    std::regex("^CREATE TABLE [IF NOT EXISTS ]?([A-Z_]+) \\( ([A-Z_]+) "
               "(INT|CHAR|FLOAT) \\) ;$");
const std::basic_regex<char> REG_MODIFY_COLUMN =
    std::regex("^ALTER TABLE ([A-Z_]+) MODIFY COLUMN ([A-Z_]+) ([A-Z_]+) "
               "(INT|CAHR|FLOAT) ; $");
const std::basic_regex<char> REG_ADD_COLUMN =
    std::regex("^ALTER TABLE ([A-Z_]+) ADD COLUMN ([A-Z_]+) (INT|CAHR|FLOAT) "
               "(BEFORE|AFTER) ([A-Z_]+) ;$");
const std::basic_regex<char> REG_DROP_COLUMN =
    std::regex("^ALTER TABLE ([A-Z_]+) DROP COLUMN ([A-Z_]+) ;$");
const std::basic_regex<char> REG_MODIFY_TABLE_NAME =
    std::regex("^ALTER TABLE ([A-Z_]+) RENAME TO ([A-Z_]+) ;$");
const std::basic_regex<char> REG_DROP_TABLE =
    std::regex("^DROP TABLE [IF EXISTS ]?([A-Z_]?) ;$");
//  const std::basic_regex<char> REG_SHOW_TABLES = std::regex("");
const std::basic_regex<char> REG_SHOW_CREATE_TABLE =
    std::regex("^SHOW CREATE TABLE ([A-Z_]+) ;$");

std::map<std::string, Table *> mp;

// create table IF NOT EXISTS a_A_ ( aBb int ) ;

// 处理用户输入
void process_input(DDL_String *dstr) {
  std::vector<DDL_String *> vstr;
  dstr->split(vstr);

  std::string str;
  for (auto it : vstr) {
    char *s = it->String();
    for (int i = 0; i < it->len(); ++i)
      str.push_back(s[i]);
    str.push_back(' ');
  }
  str.pop_back(); // 减除最后一个空格

  if (vstr.size() <= 1) {
    printf("不知是何的输入[%s]\n", str.c_str());
    return;
  }

  auto it = vstr[0];
  if (it->equal(CMD_CREATE))
    create_table(str);
  else if (it->equal(CMD_ALTER))
    modify_table(str);
  else if (it->equal(CMD_DROP))
    drop_table(str);
  else if (it->equal(CMD_SHOW))
    show_table(str);
  else {
    dstr->print();
    printf("\n不知是何的输入\n");
  }
}

// ----- 创建表 -----
// create table name ( ) ;
void create_table(std::string &str) {
  std::smatch res;
  bool isMatch = std::regex_match(str, res, REG_CREATE_TABLE);
  if (!isMatch) {
    printf("can not regex match[%s]\n", str.c_str());
    return;
  }

  // 暂不支持约束 - 只支持 `字段名 类型`
  // 一个字段两个属性，加上字符串本身和表名，正则匹配结果必然是偶数
  if (res.size() % 2 != 0 || res.size() < 2) {
    printf("unpredictable parsing results[%s]\n", str.c_str());
    return;
  }

  Table *table = new Table();
  table->name = res[1].str();

  if (mp.find(table->name) != mp.end()) {
    if (str.find("IF NOT EXISTS") != str.npos) {
      printf("existing table[%s]\n", table->name.c_str());
      return;
    } else {
      printf("can not create an existing table[%s]\n", table->name.c_str());
      mp[table->name]->print();
      return;
    }
  }

  for (int i = 2; i < res.size(); i += 2) {
    Column *column = new Column(res[i].str(), res[i + 1].str());
    if (column->is_known()) {
      printf("unsupported data type[%s]\n", res[i + 1].str().c_str());
      return;
    }
    table->column.push_back(column);
  }
  mp[table->name] = table;
  table->print();
}

// ----- 修改表 -----
void modify_table(std::string &vstr) {
  for (auto it : vstr) {
    printf("-");
  }
  printf("\n");
}

// 修改列信息
void modify_column(std::string &str) {
  std::smatch res;
  bool isMatch = std::regex_match(str, res, REG_MODIFY_COLUMN);
  if (!isMatch) {
    printf("can not regex match[%s]\n", str.c_str());
    return;
  }

  if (res.size() != 5) {
    printf("unpredictable parsing results[%s]\n", str.c_str());
    return;
  }

  if (mp.find(res[1]) != mp.end()) {
    for (auto &it : mp[res[1]]->column)
      if (it->is_name(res[2])) {
        if (!it->chg_info(res[3], res[4])) {
          printf("can not change the column[%s] of the table[%s]\n",
                 res[3].str().c_str(), res[1].str().c_str());
          return;
        }
      }
    printf("can not change the column[%s] of the table[%s]\n",
           res[3].str().c_str(), res[1].str().c_str());
  } else {
    printf("can not alter a nonexistent table[%s]\n", res[1].str().c_str());
  }
}

// 添加新列
void add_column(std::string &str) {}

// 删除列
void drop_column(std::string &str) {
  std::smatch res;
  bool isMatch = std::regex_match(str, res, REG_DROP_COLUMN);
  if (!isMatch) {
    printf("can not regex match[%s]\n", str.c_str());
    return;
  }

  if (res.size() != 2) {
    printf("unpredictable parsing results[%s]\n", str.c_str());
    return;
  }

  if (mp.find(res[1]) != mp.end()) {
    mp[res[1]]->name = res[2].str();
    mp[res[1]]->print();
  } else {
    printf("can not alter a nonexistent table[%s]\n", res[1].str().c_str());
  }
}

// 修改表名
void modify_table_name(std::string &str) {
  std::smatch res;
  bool isMatch = std::regex_match(str, res, REG_MODIFY_TABLE_NAME);
  if (!isMatch) {
    printf("can not regex match[%s]\n", str.c_str());
    return;
  }

  if (res.size() != 3) {
    printf("unpredictable parsing results[%s]\n", str.c_str());
    return;
  }

  if (mp.find(res[1]) != mp.end()) {
    mp[res[1]]->name = res[2].str();
    mp[res[1]]->print();
  } else {
    printf("can not alter a nonexistent table[%s]\n", res[1].str().c_str());
  }
}

// 删除表
void drop_table(std::string &str) {
  std::smatch res;
  bool isMatch = std::regex_match(str, res, REG_DROP_TABLE);
  if (!isMatch) {
    printf("can not regex match[%s]\n", str.c_str());
    return;
  }
  if (res.size() != 2) {
    printf("unpredictable parsing results[%s]\n", str.c_str());
    return;
  }
  if (mp.find(res[1]) != mp.end()) {
    printf("would drop the table[%s]\n", res[1].str().c_str());
    mp.erase(res[1]);
    printf("dropped\n");
  } else {
    if (str.find("IF EXISTS") != str.npos) {
      printf("undiscovered table[%s]\n", res[1].str().c_str());
    } else {
      printf("can not drop a nonexistent table[%s]\n", res[1].str().c_str());
    }
  }
}

// ----- 查询 -----
void show_table(std::string &str) {
  if (str == "SHOW TABLES ;")
    show_tables(str);
  else if (str.find("SHOW CREATE TABLE"))
    show_create_table(str);
  else
    printf("incomprehensible statement[%s]\n", str.c_str());
}

// 查看全部表名
void show_tables(std::string &str) {
  if (mp.empty())
    printf("<null>\n");
  else
    for (auto &it : mp)
      printf("%s\n", it.first.c_str());
}

// 查询表信息
void show_create_table(std::string &str) {
  std::smatch res;
  bool isMatch = std::regex_match(str, res, REG_SHOW_CREATE_TABLE);
  if (!isMatch) {
    printf("can not regex match[%s]\n", str.c_str());
    return;
  }
  if (res.size() != 2) {
    printf("unpredictable parsing results[%s]\n", str.c_str());
    return;
  }
  if (mp.find(res[1]) != mp.end()) {
    mp[res[1]]->print();
  } else {
    printf("can not find the table[%s]\n", res[1].str().c_str());
  }
}