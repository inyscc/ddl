#ifndef __DDL__PARSE__HPP__
#define __DDL__PARSE__HPP__

#include "type.hpp"
#include <string>
#include <map>
#include <regex>

// 处理用户输入
void process_input(DDL_String *);

// ----- 创建表 -----
void create_table(std::vector<DDL_String *>&);


// ----- 修改表 -----
void modify_table(std::vector<DDL_String *> &);

// 修改列信息
void modify_column(std::vector<DDL_String *>&);

// 添加新列
void add_column(std::vector<DDL_String *>&);

// 删除列
void drop_column(std::vector<DDL_String *>&);

// 修改表名
void modify_table_name(std::vector<DDL_String *>&);

// 删除表
void drop_table(std::vector<DDL_String *>&);


// ----- 查询 -----
void show_table(std::vector<DDL_String *>&);

// 查看全部表名
void show_tables(std::vector<DDL_String *>&);

// 查询表信息
void show_create(std::vector<DDL_String *>&);

#endif // __DDL__PARSE__HPP__