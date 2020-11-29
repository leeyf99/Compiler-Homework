#include <iostream>
#include <iomanip>
#include <string>
#include "tree.h"
using namespace std;

string oNodeType[8]={    
    "NODE_CONST",
    "NODE_BOOL",
    "NODE_VAR",
    "NODE_EXPR",
    "NODE_TYPE",
    "NODE_STMT",
    "NODE_PROG",
    "NODE_OP"};

string oStmtType[6]
{
    "STMT_IF",
    "STMT_WHILE",
    "STMT_DECL",
    "STMT_ASSIGN",
    "STMT_PRINTF",
    "STMT_SCANF"
};

string oOpType[3]
{
    "OP_EQUAL",
    "OP_NOT",
    "OP_ADD",
};

string oVarType[2]
{
    "VAR_INTEGER",
    "VAR_VOID"
};

TreeNode::TreeNode(NodeType type)
{
    nodeType = type;
}

void TreeNode::addSibling(TreeNode *node)
{
    if (sibling!=nullptr)
    {
        TreeNode *temp;
        temp = sibling;
        while (temp->sibling)
            temp = temp->sibling;
        temp->sibling = node;
    }
    else
    {
        sibling=node;
    }
}

void TreeNode::addChild(TreeNode *node)
{
    if (child!=nullptr)
    {
        TreeNode *temp;
        temp = child;
        temp->addSibling(node);
    }
    else
    {
        child=node;
    }
}

int num=0;

void TreeNode::genNodeId()
{
    this->nodeID = num++;
    if (this->child != nullptr)
    {
        TreeNode *temp = this->child;
        temp->genNodeId();
        while(temp->sibling!=nullptr)
        {
            temp->sibling->genNodeId();
            temp=temp->sibling;
        }
    }
}

void TreeNode::printNodeInfo()
{
    if(nodeType==NODE_STMT)
    {
        cout<<setw(15)<<oStmtType[stmtType];
    }
    else if(nodeType==NODE_VAR)
        cout<<setw(15)<<oVarType[varType];
    else if(nodeType==NODE_OP)
        cout<<setw(15)<<oOpType[opType];
    else
    {
        cout<<setw(15)<<oNodeType[nodeType];
    }
    
}

void TreeNode::printAST()
{
    // cout<<"???"<<endl;
    cout<<setw(6)<<nodeID;
    printNodeInfo();
    TreeNode*temp;
    temp=this->child;
    while(temp)
    {
        temp->printNodeInfo();
        cout<<setw(6)<<temp->nodeID;
        temp=temp->sibling;
    }
    cout<<endl;
    temp=this->child;
    while(temp)
    {
        temp->printAST();
        temp=temp->sibling;
    }
}