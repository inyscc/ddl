#ifndef __DDL__PARSE__HPP__
#define __DDL__PARSE__HPP__

#include "type.hpp"
#include <map>
#include <regex>
#include <string>

// 处理用户输入
void process_input(DDL_String *);

// ----- 创建表 -----
void create_table(std::string &);

// ----- 修改表 -----
void modify_table(std::string &);

// 修改列信息
void modify_column(std::string &);

// 添加新列
void add_column(std::string &);

// 删除列
void drop_column(std::string &);

// 修改表名
void modify_table_name(std::string &);

// 删除表
void drop_table(std::string &);

// ----- 查询 -----
void show_table(std::string &);

// 查看全部表名
void show_tables(std::string &);

// 查询表信息
void show_create_table(std::string &);

#endif // __DDL__PARSE__HPP__