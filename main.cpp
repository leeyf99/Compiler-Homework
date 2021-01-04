#include "main.tab.hh"
#include "common.h"
#include <iostream>
using std::cout;
using std::endl;
TreeNode *root = nullptr;
int main()
{
  yyparse();
  if (root)
  { //若存在语法树结点
    root->genNodeId(); //将整棵语法树赋予id
    // cerr<<"typecheck"<<endl;
    root->type_check();
    // cerr<<"AST"<<endl;
    // root->printAST(); //打印相关信息
    // cerr<<"label"<<endl;
    root->get_label();
    // cerr<<"get_t"<<endl;
    root->get_temp_var();
    // cerr<<"gen_code"<<endl;
    root->gen_code();
  }
}
int yyerror(char const *message)
{
  cout << message << endl;
  return -1;
}