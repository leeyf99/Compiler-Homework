#ifndef TREE_H
#define TREE_H
#include <iostream>
#include <string>

using std::cerr;
using std::cout;
using std::endl;
using std::string;
using namespace std;
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
    NODE_STR,
    NODE_CHAR
};

enum StmtType
{
    STMT_IF,
    STMT_ELSE,
    STMT_WHILE,
    STMT_DECL,
    STMT_CONDECL,
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
    STMT_BODY,
    STMT_COND,
    STMT_RETURN,
    STMR_ELIF
};

enum OpType
{
    OP_EQUAL,
    OP_NOTEQ,
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
    VAR_VOID,
    VAR_INTEGER,
    VAR_CHAR,
    VAR_BOOL
};

enum Type
{
    Notype,
    Integer,
    Character,
    Boolean
};

struct Str{
    string str[1024];
    int size=0;
    int lookup(string t);
};

struct Label {
	string true_label;
	string false_label;
	string begin_label;
	string next_label;
};


struct TreeNode
{
    int nodeID;
    NodeType nodeType;
    int line;
    Label label;
    int area;

    TreeNode *child = nullptr;
    TreeNode *sibling = nullptr;

    void addSibling(TreeNode *);
    void addChild(TreeNode *);

    void type_check();
    void get_temp_var();

    void genNodeId(); //从根节点开始逐个赋Id 实现方式同学们可以自行修改

private:
    void printNodeInfo();
    void printNodeConnection();

    string nodeTypeInfo();
    void gen_header();
    void gen_decl();
    void stmt_gen_code();
    void expr_gen_code();
    void recursive_gen_code();
    string new_label();
    void stmt_get_label(TreeNode *n);
    void expr_get_label(TreeNode *n);
    void recursive_get_label(TreeNode *n);


public:
    int int_val;
    bool bool_val;
    StmtType stmtType;
    OpType opType;
    VarType varType;
    string var_name;
    Type this_type;

    void get_label();
    void gen_code();
    void printAST(); //打印语法树结点
    TreeNode(NodeType type,int num);
};

#endif