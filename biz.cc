#include "biz.hpp"
#include "parse.hpp"
#include "type.hpp"
#include <stdio.h>

void version() {
  printf("---------------------------------------------------------------------"
         "--------------\n");
  printf("--------------------------------Welcom To Simple "
         "DDL-------------------------------\n");
  printf("-----------------------------------------------------------------"
         "Create By yxw@2022\n");
  printf("------------------------------------------------------------------"
         "...+'\\n' for quit \n");
  printf("---------------------------------------------------------------------"
         "--------------\n");
}

void read_input() {
  printf("DDL >>> ");

  DDL_String *dstr = new DDL_String(1024); // 存放用户输入
  char ch;                                 // 存放用户输入的单个字符

  int tot(0);        // 计算连续输入多个点
  bool iSime(false); // 标记上一个字符是否是分号

  while (true) {
    scanf("%c", &ch);

    switch (ch) {
    case ';':
      tot = 0;
      iSime = true;
      break;
    case '.':
      tot++;
      break;
    case '\n': // 上一个字符是结束符分号 `;` 开始处理
      if (tot >= 3) {
        printf("Bye ~ ");
        return;
      }
      if (iSime) {
        dstr->to_upper();
        process_input(dstr);

        dstr->reset();
        iSime = false;
        printf("DDL >>> ");
        continue;
      }
      break;
    }
    dstr->append(ch);
  }
}
