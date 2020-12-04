#ifndef TREE_H
#define TREE_H
#include <iostream>
#include <string>

using std::cerr;
using std::cout;
using std::endl;
using std::string;

#define MAXCHILDREN 3



enum NodeType
{
    NODE_CONST,
    NODE_BOOL,
    NODE_VAR,
    NODE_EXPR,
    NODE_TYPE,
    NODE_STMT,
    NODE_PROG,
    NODE_OP,
    NODE_FUNC,
    NODE_PARAM,
    NODE_PARAMS,
    NODE_DEFLIST,
    NODE_UMINUS,
    NODE_INC,
    NODE_DEC,
    NODE_STR
};

enum StmtType
{
    STMT_IF,
    STMT_ELSE,
    STMT_WHILE,
    STMT_DECL,
    STMT_ASSIGN,
    STMT_PRINTF,
    STMT_SCANF,
    STMT_ADDASSIGN,
    STMT_SUBASSIGN,
    STMT_FOR,
    STMT_FOR1,
    STMT_FOR2,
    STMT_FOR3,
    STMT_INC,
    STMT_DEC,
};

enum OpType
{
    OP_EQUAL,
    OP_NOT,
    OP_ADD,
    OP_SUB,
    OP_MUL,
    OP_QUO,
    OP_REM,
    OP_LSS,
    OP_LEQ,
    OP_GTR,
    OP_GEQ,
    OP_AND,
    OP_OR
};

enum VarType
{
    VAR_INTEGER,
    VAR_VOID,
    VAR_CHAR
};

struct TreeNode
{
    int nodeID;
    NodeType nodeType;
    int line;

    TreeNode *child = nullptr;
    TreeNode *sibling = nullptr;

    void addSibling(TreeNode *);
    void addChild(TreeNode *);

    void genNodeId(); //从根节点开始逐个赋Id 实现方式同学们可以自行修改

    void printNodeInfo();
    void printNodeConnection();
    void printAST(); //打印语法树结点
    /***
     * 以下的几个函数皆为在printAST过程中辅助输出使用
     * 同学们可以根据需要自己使用其他方法
    ***/

    string nodeTypeInfo();

    int int_val;
    bool bool_val;
    StmtType stmtType;
    OpType opType;
    VarType varType;
    string var_name;

    TreeNode(NodeType type,int num);
};

#endif