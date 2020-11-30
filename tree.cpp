#include <iostream>
#include <iomanip>
#include <string>
#include "tree.h"
using namespace std;

string oNodeType[] = {
    "NODE_CONST",
    "NODE_BOOL",
    "NODE_VAR",
    "NODE_EXPR",
    "NODE_TYPE",
    "NODE_STMT",
    "NODE_PROG",
    "NODE_OP",
    "NODE_FUNC",
    "NODE_PARAM",
    "NODE_PARAMS",
    "NODE_DEFLIST",
    "NODE_UMINUS",
    "NODE_INC",
    "NODE_DEC",
    "NODE_STR"};

string oStmtType[]{
    "STMT_IF",
    "STMT_WHILE",
    "STMT_DECL",
    "STMT_ASSIGN",
    "STMT_PRINTF",
    "STMT_SCANF",
    "STMT_ADDASSIGN",
    "STMT_SUBASSIGN",
    "STMT_FOR",
    "STMT_FOR1",
    "STMT_FOR2",
    "STMT_FOR3"};

string oOpType[]{
    "OP_EQUAL",
    "OP_NOT",
    "OP_ADD",
    "OP_SUB",
    "OP_MUL",
    "OP_QUO",
    "OP_REM",
    "OP_LSS",
    "OP_LEQ",
    "OP_GTR",
    "OP_GEQ",
    "OP_AND",
    "OP_OR"
};

string oVarType[3]{
    "VAR_INTEGER",
    "VAR_VOID",
    "VAR_CHAR"};

TreeNode::TreeNode(NodeType type, int num)
{
    nodeType = type;
    line = num;
}

void TreeNode::addSibling(TreeNode *node)
{
    if (sibling != nullptr)
    {
        TreeNode *temp;
        temp = sibling;
        while (temp->sibling)
            temp = temp->sibling;
        temp->sibling = node;
    }
    else
    {
        sibling = node;
    }
}

void TreeNode::addChild(TreeNode *node)
{
    if (child != nullptr)
    {
        TreeNode *temp;
        temp = child;
        temp->addSibling(node);
    }
    else
    {
        child = node;
    }
}

int num = 0;

void TreeNode::genNodeId()
{
    this->nodeID = num++;
    if (this->child != nullptr)
    {
        TreeNode *temp = this->child;
        temp->genNodeId();
        while (temp->sibling != nullptr)
        {
            temp->sibling->genNodeId();
            temp = temp->sibling;
        }
    }
}

void TreeNode::printNodeInfo()
{
    if (nodeType == NODE_STMT)
    {
        cout << setw(15) << oStmtType[stmtType];
    }
    else if (nodeType == NODE_VAR)
    {
        cout << setw(15) << oVarType[varType];
        cout << setw(9) << "name:" << setw(6) << var_name;
    }

    else if (nodeType == NODE_OP)
        cout << setw(15) << oOpType[opType];
    else if(nodeType==NODE_TYPE)
        cout<<setw(15)<<oVarType[varType];
    else if(nodeType==NODE_STR)
    {
        cout<<setw(15)<<oNodeType[NODE_STR];
        cout<<setw(9)<<"string:"<<var_name;
    }
    else
    {
        cout << setw(15) << oNodeType[nodeType];
        if (nodeType == NODE_CONST)
        {
            cout << setw(9) << "value:" << setw(6) << int_val;
        }
        else if (nodeType == NODE_BOOL)
        {
            cout << setw(9) << "value:" << setw(6) << bool_val;
        }
    }
}

void TreeNode::printAST()
{
    // cout<<"???"<<endl;
    cout << "lno:" << setw(2) << line << "   ";
    cout << "@" << setw(2) << nodeID;
    printNodeInfo();
    TreeNode *temp;
    temp = this->child;
    int flag=0;
    if(temp)
        flag=1;
    if(flag==1)
        cout<<"   child:[";
    while (temp)
    {
        cout << "@" << temp->nodeID<<" ";
        temp = temp->sibling;
    }
    if(flag==1)
        cout<<"]";
    cout << endl;
    temp = this->child;
    while (temp)
    {
        temp->printAST();
        temp = temp->sibling;
    }
}