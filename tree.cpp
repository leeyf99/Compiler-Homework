#include <iostream>
#include <iomanip>
#include "tree.h"
#include "symbol.h"
using namespace std;

extern int lineno;
extern symbol_table symtbl;
Str str_list;

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
    "NODE_STR",
    "NODE_CHAR"};

string oStmtType[]{
    "STMT_IF",
    "STMT_ELSE",
    "STMT_WHILE",
    "STMT_DECL",
    "STMT_CONDECL",
    "STMT_ASSIGN",
    "STMT_PRINTF",
    "STMT_SCANF",
    "STMT_ADDASSIGN",
    "STMT_SUBASSIGN",
    "STMT_FOR",
    "STMT_FOR1",
    "STMT_FOR2",
    "STMT_FOR3",
    "STMT_INC",
    "STMT_DEC",
    "STMT_BODY",
    "STMT_COND",
    "STMT_RETURN",
    "STMT_ELIF"};

string oOpType[]{
    "OP_EQUAL",
    "OP_NOTEQ",
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
    "OP_OR"};

string oVarType[]{
    "VAR_VOID",
    "VAR_INTEGER",
    "VAR_CHAR",
    "VAR_BOOL"};

string oType[]{
    "notype",
    "integer",
    "char",
    "bool"};

int Str::lookup(string t)
{
    for(int i=0;i<size;i++)
    {
        if(str[i]==t)
            return i;
    }
    return -1;
}

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
void TreeNode::type_check()
{

    TreeNode *temp = this->child;
    while (temp != nullptr)
    {
        temp->type_check();
        temp = temp->sibling;
    }
    if (this->nodeType == NODE_CONST)
    {
        this_type = Integer;
        return;
    }
    else if (this->nodeType == NODE_BOOL)
    {
        this_type = Boolean;
        return;
    }
    else if (this->nodeType == NODE_VAR)
    {
        int pos;
        pos = symtbl.lookup(this->var_name);
        if (pos != -1)
        {
            this_type = (Type)symtbl.get_type(pos);
            return;
        }
        else
        {
            this_type = Notype;
            return;
        }
    }
    else if (this->nodeType == NODE_TYPE)
    {
        this_type = (Type)this->varType;
    }
    else if (this->nodeType == NODE_STMT)
    {
        if (this->stmtType == STMT_COND)
        {
            if (this->child->this_type != Boolean)
            {
                cerr << "Bad boolean type at line: " << this->line << endl;
                exit(1);
            }
            else
            {
                this_type = Notype;
                return;
            }
        }
        else if (this->stmtType == STMT_DECL)
        {
            TreeNode *temp_node = this->child->sibling;
            VarType temp_type = this->child->varType;
            if (temp_node->nodeType == NODE_VAR)
            {
                if (symtbl.lookup(temp_node->var_name) != -1)
                {
                    cerr << "变量重定义! line:" << this->line << endl;
                    exit(1);
                }
                else
                {
                    if ((Type)temp_type != temp_node->sibling->this_type)
                    {
                        cerr << "error declare assign not right on line: " << line << endl;
                        exit(1);
                    }
                    this_type = Notype;
                    cerr<<temp_node->var_name<<endl;
                    temp_node->this_type = (Type)temp_type;
                    int pos = symtbl.insert(temp_node->var_name, 0);
                    symtbl.set_type(pos, (int)temp_type);
                    return;
                }
            }
            else if (temp_node->nodeType == NODE_DEFLIST)
            {

                temp_node = temp_node->child;
                this_type = Notype;
                while (temp_node != nullptr)
                {
                    if (symtbl.lookup(this->child->sibling->var_name) != -1)
                    {
                        cerr << "变量重定义! line:" << this->line << endl;
                        exit(1);
                    }
                    else
                    {
                        temp_node->this_type = (Type)temp_type;
                        int pos = symtbl.insert(temp_node->var_name, 0);
                        symtbl.set_type(pos, (int)temp_type);
                    }
                    temp_node = temp_node->sibling;
                }
                return;
            }
        }
        else if (this->stmtType == STMT_CONDECL)
        {
            TreeNode *temp_node = this->child->sibling;
            VarType temp_type = this->child->varType;
            if (temp_node->nodeType == NODE_VAR)
            {
                if (symtbl.lookup(temp_node->var_name) != -1)
                {
                    cerr << "变量重定义! line:" << this->line << endl;
                    exit(1);
                }
                else
                {
                    if ((Type)temp_type != temp_node->sibling->this_type)
                    {
                        cerr << "error declare assign not right on line: " << line << endl;
                        exit(1);
                    }
                    this_type = Notype;
                    temp_node->this_type = (Type)temp_type;
                    int pos = symtbl.insert(temp_node->var_name, 0);
                    symtbl.set_type(pos, (int)temp_type);
                    symtbl.table[pos].Isconst=true;
                    return;
                }
            }
            else if (temp_node->nodeType == NODE_DEFLIST)
            {

                temp_node = temp_node->child;
                this_type = Notype;
                while (temp_node != nullptr)
                {
                    if (symtbl.lookup(this->child->sibling->var_name) != -1)
                    {
                        cerr << "变量重定义! line:" << this->line << endl;
                        exit(1);
                    }
                    else
                    {
                        temp_node->this_type = (Type)temp_type;
                        int pos = symtbl.insert(temp_node->var_name, 0);
                        symtbl.set_type(pos, (int)temp_type);
                    }
                    temp_node = temp_node->sibling;
                }
                return;
            }
        }
        else if (this->stmtType == STMT_ASSIGN)
        {
            TreeNode *n1, *n3;
            n1 = this->child;
            n3 = this->child->sibling;
            if (n1->this_type != n3->this_type)
            {
                if (n1->this_type == Notype)
                {
                    cerr << "error var is not defined on line: " << line << endl;
                    exit(1);
                }
                cerr << "assign error at line: " << line << ", 1 is " << oType[n1->this_type]
                     << " and 2 is " << oType[n3->this_type] << endl;
                exit(1);
            }
            else if (symtbl.table[symtbl.lookup(n1->var_name)].Isconst)
            {
                cerr<<"change const error"<<endl;
                exit(1);
            }
            
            else
            {
                this_type = n1->this_type;
                return;
            }
        }
        else if (this->stmtType == STMT_ADDASSIGN || this->stmtType == STMT_SUBASSIGN)
        {
            TreeNode *n1, *n3;
            n1 = this->child;
            n3 = n1->sibling;
            if (n1->this_type != Integer || n3->this_type != Integer)
            {
                cerr << "Not integer error on line: " << line << endl;
                exit(1);
            }
            else if (symtbl.table[symtbl.lookup(n1->var_name)].Isconst)
            {
                cerr<<"change const error"<<endl;
                exit(1);
            }
            else
            {
                this->this_type = Integer;
                return;
            }
        }
        else if (this->stmtType == STMT_INC || this->stmtType == STMT_DEC)
        {
            if (this->child->this_type != Integer)
            {
                cerr << "not integer error on line: " << line << endl;
            }
            else if (symtbl.table[symtbl.lookup(this->child->var_name)].Isconst)
            {
                cerr<<"change const error"<<endl;
                exit(1);
            }
        }
        else
        {
            this_type = Notype;
            return;
        }
    }
    else if (this->nodeType == NODE_PROG)
    {
        this_type = Notype;
        return;
    }
    else if (this->nodeType == NODE_OP)
    {
        if (this->opType == OP_ADD || this->opType == OP_SUB ||
            this->opType == OP_MUL || this->opType == OP_QUO || this->opType == OP_REM)
        {
            TreeNode *n1, *n3;
            n1 = this->child;
            n3 = n1->sibling;
            if (n1->this_type != Integer || n3->this_type != Integer)
            {
                Type temp = n3->this_type;
                if (n1->this_type != Integer)
                    temp = n1->this_type;
                cerr << "error need integer but " << oType[temp] << " on line: " << this->line << endl;
                exit(1);
            }
            else
            {
                this_type = Integer;
                return;
            }
        }
        else if (this->opType == OP_EQUAL||this->opType == OP_NOTEQ)
        {
            TreeNode *n1, *n3;
            n1 = this->child;
            n3 = n1->sibling;
            if (n1->this_type == n3->this_type && n1->this_type != Notype)
            {
                this_type = Boolean;
                return;
            }
            else
            {
                cerr << "error types not same on line: " << line << endl;
                exit(0);
            }
        }
        else if (this->opType == OP_LSS || this->opType == OP_LEQ || this->opType == OP_GTR || this->opType == OP_GEQ)
        {
            TreeNode *n1, *n3;
            n1 = this->child;
            n3 = n1->sibling;
            if (n1->this_type == Integer && n3->this_type == Integer)
            {
                this_type = Boolean;
                return;
            }
            else
            {
                cerr << "error compare not integer on line: " << line << endl;
                cerr << oOpType[opType] << " " << oType[n1->this_type] << " " << oType[n3->this_type]
                     << endl;
                exit(1);
            }
        }
        else if (this->opType == OP_AND || this->opType == OP_OR)
        {
            TreeNode *n1, *n3;
            n1 = this->child;
            n3 = n1->sibling;
            if (n1->this_type == Boolean && n3->this_type == Boolean)
            {
                this_type = Boolean;
                return;
            }
            else
            {
                cerr << "error not boolean on line: " << line << endl;
                exit(1);
            }
        }
        else if (this->opType == OP_NOT)
        {
            TreeNode *t = this->child;
            if (t->this_type == Boolean)
            {
                this_type = Boolean;
                return;
            }
            else
            {
                cerr << "error not boolean on line: " << line << endl;
                exit(1);
            }
        }
    }
    else if (this->nodeType == NODE_DEFLIST)
    {
        TreeNode *t = this->child;
        while (t->sibling != nullptr)
        {
            t = t->sibling;
        }
        if (t->nodeType == NODE_VAR && symtbl.lookup(t->var_name) == -1)
        {
            this_type = Notype;
            return;
        }
        else
        {
            cerr << "error redefine on line: " << line << endl;
            exit(1);
        }
    }
    else if (this->nodeType == NODE_UMINUS)
    {
        TreeNode *t = this->child;
        if (t->this_type == Integer)
        {
            this_type = Integer;
            return;
        }
        else
        {
            cerr << "error minus not integer on line: " << line << endl;
        }
        return;
    }
    else if(this->nodeType==NODE_STR)
    {
        if(str_list.lookup(this->var_name)!=-1)
            return;
        str_list.str[str_list.size]=this->var_name;
        str_list.size++;
        this_type=Notype;
        return;
    }
    else if(this->nodeType==NODE_CHAR)
    {
        this_type=Character;
        return;
    }
    else
    {
        this_type = Notype;
        return;
    }
}

void TreeNode::printNodeInfo()
{
    cout << setw(15) << oNodeType[nodeType];
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
    else if (nodeType == NODE_TYPE)
        cout << setw(15) << oVarType[varType];
    else if (nodeType == NODE_STR)
    {
        cout << setw(9) << "string:" << var_name;
    }
    else if(nodeType==NODE_CHAR)
    {
        cout<<setw(9)<<"char:"<<int_val;
    }
    else
    {

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

int temp_seq=0;

void TreeNode::get_temp_var()
{
    if(this->nodeType==NODE_OP||this->nodeType==NODE_UMINUS)
    {
        temp_seq++;
    }
    TreeNode *temp=this->child;
    while(temp)
    {
        temp->get_temp_var();
        temp=temp->sibling;
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
    int flag = 0;
    if (temp)
        flag = 1;
    if (flag == 1)
        cout << "   child:[";
    while (temp)
    {
        cout << "@" << temp->nodeID << " ";
        temp = temp->sibling;
    }
    if (flag == 1)
        cout << "]";
    cout << endl;
    temp = this->child;
    while (temp)
    {
        temp->printAST();
        temp = temp->sibling;
    }
}

int label_seq=0;


string TreeNode::new_label()
{
    char tmp[20];

    sprintf(tmp, "L%d", label_seq);
    label_seq++;
    // cout<<tmp<<endl;
    return tmp;
}

void TreeNode::stmt_get_label(TreeNode *n)
{
    switch (n->stmtType)
    {
    case STMT_WHILE:
    {
        TreeNode *e = n->child; // 循环条件
        TreeNode *s = e->sibling; // 循环体
        if (n->label.begin_label == "")
            n->label.begin_label = new_label();
        s->label.next_label = n->label.begin_label;
        s->label.begin_label = e->label.true_label = new_label();
        if (n->label.next_label == "")
            n->label.next_label = new_label();
        e->label.false_label = n->label.next_label;
        if (n->sibling)
            n->sibling->label.begin_label = n->label.next_label;
        recursive_get_label(e);
        recursive_get_label(s);
        break;
    }
    case STMT_FOR:
    {
        // cout<<"gen for label"<<endl;
        TreeNode *cond = n->child->sibling;
        TreeNode *for3 = cond->sibling;
        TreeNode *body = for3->sibling;
        if(cond->label.begin_label=="")
            cond->label.begin_label=new_label();
        cond->label.true_label=body->label.begin_label=new_label();
        body->label.next_label=cond->label.begin_label;
        if(n->label.next_label=="")
            n->label.next_label = new_label();
        cond->label.false_label=n->label.next_label;
        if (n->sibling)
            n->sibling->label.begin_label = n->label.next_label;
        recursive_get_label(cond);
        if(for3->child)
            recursive_get_label(for3);
        if(body->child)
            recursive_get_label(body);
        break;
    }
    case STMT_IF:
    {
        // cout<<"get_label_if"<<endl;
        TreeNode *cond=n->child;
        TreeNode *body=cond->sibling;
        TreeNode *else0=body->sibling;
        cond->label.true_label=body->label.begin_label=new_label();
                // cout<<"else"<<endl;
        if(else0)
            {
                cond->label.false_label=else0->label.begin_label=new_label();
                TreeNode *end=else0;
                while(end->sibling)
                {
                    end=end->sibling;
                }
                if(end->label.next_label=="")
                    end->label.next_label=new_label();
                body->label.next_label=end->label.next_label;
                while (true)
                {
                    if(!else0)
                        break;
                    if (!else0->sibling)
                    {
                        break;
                    }
                    else0->label.true_label = else0->sibling->label.begin_label = new_label();
                    else0->sibling->label.next_label=end->label.next_label;
                    if (else0->sibling->sibling)
                        else0->label.false_label = else0->sibling->sibling->label.begin_label = new_label();
                    else
                        else0->label.false_label=end->label.next_label;
                    else0=else0->sibling->sibling;
                }
            }
        else
        {
            if(body->label.next_label=="")
                body->label.next_label=new_label();
            cond->label.false_label=body->label.next_label;
        }
        TreeNode *temp=n->child;
        while(temp)
            {
                recursive_get_label(temp);
                temp=temp->sibling;
            }
        break;
    }
    case STMT_FOR2:
        if(n->child==nullptr)
            break;
        else
        {
        if(n->child->nodeType==NODE_OP)
        {
            TreeNode* chil=n->child;
            chil->label.true_label=n->label.true_label;
            chil->label.false_label=n->label.false_label;
            recursive_get_label(chil);
        }
        break;
        }
        
    case STMT_COND:
    {
        if(n->child->nodeType==NODE_OP)
        {
            TreeNode* chil=n->child;
            chil->label.true_label=n->label.true_label;
            chil->label.false_label=n->label.false_label;
            recursive_get_label(chil);
        }
        break;
    }
    case STMT_BODY:
    {
        TreeNode *temp=n->child;
        while(temp)
        {
            recursive_get_label(temp);
            temp=temp->sibling;
        }
        break;
    }
    default:
        break;
    }
}

void TreeNode::expr_get_label(TreeNode *n)
{
    switch (n->opType)
    {
    case OP_AND:
    {
        TreeNode *left=n->child;
        TreeNode *right=left->sibling;
        left->label.true_label=right->label.begin_label=new_label();
        left->label.false_label=n->label.false_label;
        right->label.true_label=n->label.true_label;
        right->label.false_label=n->label.false_label;
        recursive_get_label(left);
        recursive_get_label(right);
        break;
    }
    case OP_OR:
    {
        // cout<<"or start"<<endl;
        TreeNode *left=n->child;
        TreeNode *right=left->sibling;
        left->label.true_label=n->label.true_label;
        left->label.false_label=right->label.begin_label=new_label();
        right->label.true_label=n->label.true_label;
        right->label.false_label=n->label.false_label;
        // cout<<"or finished"<<endl;
        recursive_get_label(left);
        // cout<<"or left finished"<<endl;
        recursive_get_label(right);

        break;
    }
    case OP_NOT:
    {
        TreeNode *temp=n->child;
        temp->label.true_label=n->label.false_label;
        temp->label.false_label=n->label.true_label;
        recursive_get_label(temp);
        break;
    }
    default:
        break;
    }
}

void TreeNode::recursive_get_label(TreeNode *n)
{
    if (n->nodeType == NODE_STMT)
        stmt_get_label(n);
    else if (n->nodeType == NODE_OP)
        expr_get_label(n);
    else if(n->nodeType==NODE_FUNC)
        {
            n->label.begin_label="main";
            TreeNode *temp=n->child;;
            while(temp)
            {
                recursive_get_label(temp);
                temp=temp->sibling;
            }
        }
    else
    {
        TreeNode *temp=n->child;
        while(temp)
        {
            recursive_get_label(temp);
            temp=temp->sibling;
        }
    }
    
}

void TreeNode::get_label()
{
    recursive_get_label(this);
}

void TreeNode::gen_header()
{
    cout<<"\t.text"<<endl<<"\t.globl main"<<endl<<"\t.type main, @function"<<endl;
    cout<<"main:"<<endl;
}

void TreeNode::gen_decl()
{
    cout<<"\t.text"<<endl<<"\t.section\t.data"<<endl;
    for(int i=0;i<symtbl.size;i++)
    {

        cout<<"_"<<symtbl.getname(i)<<":"<<endl;
        cout<<"\t.zero\t4"<<endl;
        if(symtbl.get_type(i)==(int)Integer)
            cout<<"\t.align\t4"<<endl;
        if(symtbl.get_type(i)==(int)Character)
            cout<<"\t.align\t1"<<endl;
    }
    for (int i = 0; i < temp_seq; i++)
	{
		cout << "t" <<  i << ":" << endl;
        cout << "\t.zero\t4" << endl;
        cout << "\t.align\t4" << endl;
	}
    for(int i=0;i<str_list.size;i++)
    {
        cout<<".STR"<<i<<":"<<endl;
        cout<<"\t.string\t\""<<str_list.str[i]<<"\""<<endl;
    }
}

int use_temp=-1;
string temp_str="";

string last_label="";

int rec_print(TreeNode *n, int cal)
{
    if (n->sibling)
        cal = rec_print(n->sibling, cal);
    int t=symtbl.lookup(n->var_name);
    if(symtbl.get_type(t)==(int)Integer)
    {cout << "\tmovl\t_" << n->var_name << ", %eax" << endl;
    cout << "\tpushl\t%eax" << endl;}
    else if (symtbl.get_type(t)==(int)Character)
    {
        cout << "\tmovl\t$0, %eax" << endl;
        cout << "\tmovb\t_" << n->var_name << ", %al" << endl;
    cout << "\tpushl\t%eax" << endl;
    }
    cal++;
    return cal;
}

int rec_scanf(TreeNode *n, int cal)
{
    if (n->sibling)
        cal = rec_scanf(n->sibling, cal);
    cout << "\tpushl\t$_"<<n->var_name << endl;
    cal++;
    return cal;
}

void TreeNode::stmt_gen_code()
{
    if(this->stmtType==STMT_WHILE)
    {
        TreeNode *cond=this->child;
        TreeNode *body=cond->sibling;
        cond->recursive_gen_code();
        body->recursive_gen_code();
        cout<<"\tjmp\t"<<this->label.begin_label<<endl;
        // cout<<this->label.next_label<<":"<<endl;
    }
    else if(this->stmtType==STMT_FOR)
    {
        TreeNode *for1=this->child;
        TreeNode *for2=for1->sibling;
        TreeNode *for3=for2->sibling;
        TreeNode *body=for3->sibling;
        for1->recursive_gen_code();
        for2->recursive_gen_code();
        cout<<body->label.begin_label<<":"<<endl;
        if(body->child)
            body->child->recursive_gen_code();
        if(for3->child)
            for3->child->recursive_gen_code();
        cout<<"\tjmp\t"<<for2->label.begin_label<<endl;
    }
    else if(this->stmtType==STMT_ASSIGN)
    {
        string left_name = this->child->var_name;
        this->child->sibling->recursive_gen_code();
        if (symtbl.get_type(symtbl.lookup(left_name)) == (int)Integer)
        {

            if (this->child->sibling->nodeType == NODE_CONST)
                cout << "\tmovl\t$" << this->child->sibling->int_val << ", %eax" << endl;
            else if (temp_str == "")
                cout << "\tmovl\tt" << use_temp << ", %eax" << endl;
            else
                cout << "\tmovl\t_" << temp_str << ", %eax" << endl;
            cout << "\tmovl\t%eax, _" << left_name << endl;
        }
        else if (symtbl.get_type(symtbl.lookup(left_name)) == (int)Character)
        {
            if (this->child->sibling->nodeType == NODE_CHAR)
                cout << "\tmovb\t$" << this->child->sibling->int_val << ", %al" << endl;
            cout << "\tmovb\t%al, _" << left_name << endl;
        }
                temp_str=left_name;
    }
    else if(this->stmtType==STMT_ADDASSIGN)
    {
        string left_name=this->child->var_name;
        cout<<"\tmovl\t_"<<left_name<<", %eax"<<endl;
        this->child->sibling->recursive_gen_code();
        if(this->child->sibling->nodeType==NODE_CONST)
            cout<<"\taddl\t$"<<this->child->sibling->int_val<<", %eax"<<endl;
        else if(temp_str=="")
            cout<<"\taddl\tt"<<use_temp<<", %eax"<<endl;
        else
            cout<<"\taddl\t_"<<temp_str<<", %eax"<<endl;
        cout<<"\tmovl\t%eax, _"<<left_name<<endl;
    }
    else if(this->stmtType==STMT_SUBASSIGN)
    {
        string left_name=this->child->var_name;
        cout<<"\tmovl\t_"<<left_name<<", %eax"<<endl;
        this->child->sibling->recursive_gen_code();
        if(this->child->sibling->nodeType==NODE_CONST)
            cout<<"\tsubl\t$"<<this->child->sibling->int_val<<", %eax"<<endl;
        else if(temp_str=="")
            cout<<"\tsubl\tt"<<use_temp<<", %eax"<<endl;
        else
            cout<<"\tsubl\t_"<<temp_str<<", %eax"<<endl;
        cout<<"\tmovl\t%eax, _"<<left_name<<endl;
    }
    else if(this->stmtType==STMT_INC)
    {
        string left_name=this->child->var_name;
        cout<<"\tmovl\t_"<<left_name<<", %eax"<<endl;
        cout<<"\taddl\t$1, %eax"<<endl;
        cout<<"\tmovl\t%eax, _"<<left_name<<endl;
    }
    else if(this->stmtType==STMT_DEC)
    {
        string left_name=this->child->var_name;
        cout<<"\tmovl\t_"<<left_name<<", %eax"<<endl;
        cout<<"\tsubl\t$1, %eax"<<endl;
        cout<<"\tmovl\t%eax, _"<<left_name<<endl;
    }
    else if(this->stmtType==STMT_DECL||this->stmtType==STMT_CONDECL)
    {
        TreeNode *left=this->child->sibling;
        if(left->nodeType==NODE_DEFLIST)
            return;
        TreeNode *right=left->sibling;
        string left_name=left->var_name;
        if(right->nodeType==NODE_CONST)
            cout<<"\tmovl\t$"<<right->int_val<<", %eax"<<endl;
        else
        {
            right->recursive_gen_code();
            if(temp_str=="")
                cout<<"\tmovl\tt"<<use_temp<<", %eax"<<endl;
            else
            {
                cout<<"\tmovl\t_"<<temp_str<<", %eax"<<endl;
            }
        }
        cout<<"\tmovl\t%eax, _"<<left_name<<endl;
    }
    else if(this->stmtType==STMT_PRINTF)
    {
        int ntemp=str_list.lookup(this->child->var_name);
        TreeNode *temp=this->child->sibling;
        int cal=0;
        if(this->child->sibling)
            cal=rec_print(this->child->sibling,cal);
        cout<<"\tmovl\t$0, %eax"<<endl;
        cout<<"\tpushl\t$.STR"<<ntemp<<endl;
        cout<<"\tcall\tprintf"<<endl;
        cout<<"\taddl\t$"<<(cal+1)*4<<", %esp"<<endl;
    }
    else if(this->stmtType==STMT_SCANF)
    {
        int ntemp=str_list.lookup(this->child->var_name);
        TreeNode *temp=this->child->sibling;
        int cal=0;
        if(this->child->sibling)
            cal=rec_scanf(this->child->sibling,cal);
        cout<<"\tmovl\t$0, %eax"<<endl;
        cout<<"\tpushl\t$.STR"<<ntemp<<endl;
        cout<<"\tcall\tscanf"<<endl;
        cout<<"\taddl\t$"<<(cal+1)*4<<", %esp"<<endl;
    }
    else if(this->stmtType==STMT_IF)
    {
        TreeNode *temp=this->child;
        int else_flag=0;
        while(temp)
        {
            temp->recursive_gen_code();
            if(temp->stmtType==STMT_BODY && temp->sibling)
            {
                cout<<"\tjmp\t"<<temp->label.next_label<<endl;
            }
            temp=temp->sibling;
        }
        temp=this->child;
        while(temp->sibling)
        {
            temp=temp->sibling;
        }
        if(temp->label.next_label!=""&&temp->label.next_label!=last_label)
        {
            cout<<temp->label.next_label<<":"<<endl;
            last_label=temp->label.next_label;
        }
        return;
    }
    else
    {
        TreeNode *temp=this->child;
        while(temp)
        {
            temp->recursive_gen_code();
            temp=temp->sibling;
        }
        return;
    }
    
}

void TreeNode::expr_gen_code()
{
    if(this->opType==OP_ADD||this->opType==OP_SUB||this->opType==OP_MUL||
    this->opType==OP_QUO||this->opType==OP_REM)
    {
        string left,right;
        TreeNode *l=this->child;
        TreeNode *r=l->sibling;
        if(l->nodeType==NODE_CONST)
        {
            cout<<"\tmovl\t$"<<l->int_val<<", %eax"<<endl;
        }
        else
        {
            l->recursive_gen_code();
            left=temp_str;
            if (left != "")
            {
                cout << "\tmovl\t_" << left << ", %eax" << endl;

            }
            else
            {
                cout << "\tmovl\tt" << use_temp << ", %eax" << endl;
            }
        }
        cout << "\tpushl\t%eax" << endl;
        if(r->nodeType==NODE_CONST)
        {
            cout<<"\tmovl\t$"<<r->int_val<<", %ebx"<<endl;
        }
        else
        {
            r->recursive_gen_code();
            right=temp_str;
            if (right != "")
            {
                cout << "\tmovl\t_" << right << ", %ebx" << endl;
            }
            else
            {
                cout << "\tmovl\tt" << use_temp << ", %ebx" << endl;
            }
        }
                        cout << "\tpopl\t%eax" << endl;
        if(this->opType==OP_ADD)
            cout<<"\taddl\t%ebx, %eax"<<endl;
        else if(this->opType==OP_SUB)
            cout<<"\tsubl\t%ebx, %eax"<<endl;
        else if(this->opType==OP_MUL)
            cout<<"\timull\t%ebx"<<endl;
        else if(this->opType==OP_QUO)
            {
                cout<<"\tpushl\t%ebx"<<endl;
                cout<<"\tpushl\t%eax"<<endl;
                cout<<"\tmovl\t%eax, %edx"<<endl;
                cout<<"\tsar\t\t$31, %edx"<<endl;
                cout<<"\tpopl\t%eax"<<endl;
                cout<<"\tpopl\t%ebx"<<endl;
                cout<<"\tidiv\t%ebx"<<endl;
                
            }
        else if(this->opType==OP_REM)
        {
            cout<<"\tpushl\t%ebx"<<endl;
            cout<<"\tpushl\t%eax"<<endl;
            cout<<"\tmovl\t%eax, %edx"<<endl;
            cout<<"\tsar\t\t$31, %edx"<<endl;
            cout<<"\tpopl\t%eax"<<endl;
            cout<<"\tpopl\t%ebx"<<endl;
            cout<<"\tidiv\t%ebx"<<endl;
            cout<<"\tmovl\t%edx, %eax"<<endl;
        }
        temp_str="";
        use_temp++;
        cout<<"\tmovl\t%eax, t"<<use_temp<<endl;
    }
    else if(this->opType==OP_LSS||this->opType==OP_LEQ||this->opType==OP_GTR||
    this->opType==OP_GEQ||this->opType==OP_EQUAL||this->opType==OP_NOTEQ)
    {
        string left,right;
        TreeNode *l=this->child;
        TreeNode *r=l->sibling;
        if(l->nodeType==NODE_CONST)
        {
            cout<<"\tmovl\t$"<<l->int_val<<", %eax"<<endl;
        }
        else
        {
            l->recursive_gen_code();
            left=temp_str;
            if (left != "")
            {
                cout << "\tmovl\t_" << left << ", %eax" << endl;
            }
            else
            {
                cout << "\tmovl\tt" << use_temp << ", %eax" << endl;
            }
        }
        if(r->nodeType==NODE_CONST)
        {
            cout<<"\tmovl\t$"<<r->int_val<<", %ebx"<<endl;
        }
        else
        {
            r->recursive_gen_code();
            right=temp_str;
            if (right != "")
            {
                cout << "\tmovl\t_" << right << ", %ebx" << endl;
            }
            else
            {
                cout << "\tmovl\tt" << use_temp << ", %ebx" << endl;
            }
        }
        cout<<"\tcmpl\t %ebx, %eax"<<endl;
        switch (this->opType)
        {
        case OP_LSS:
        {
            cout<<"\tjl\t"<<this->label.true_label<<endl;
            break;
        }
        case OP_LEQ:
        {
            cout<<"\tjle\t"<<this->label.true_label<<endl;
            break;
        }
        case OP_GTR:
        {
            cout<<"\tjg\t"<<this->label.true_label<<endl;
            break;
        }
        case OP_GEQ:
        {
            cout<<"\tjge\t"<<this->label.true_label<<endl;
            break;
        }
        case OP_EQUAL:
        {
            cout<<"\tje\t"<<this->label.true_label<<endl;
            break;
        }
        case OP_NOTEQ:
        {
            cout<<"\tjne\t"<<this->label.true_label<<endl;
            break;
        }
        default:
            break;
        }
        cout<<"\tjmp\t"<<this->label.false_label<<endl;
    }
    else if(this->opType==OP_NOT)
    {
        TreeNode *temp=this->child;
        temp->recursive_gen_code();
    }
    else
    {
        TreeNode *left=this->child;
        TreeNode *right=left->sibling;
        left->recursive_gen_code();
        right->recursive_gen_code();
    }
    
}

void TreeNode::recursive_gen_code()
{
    if(this->label.begin_label!=""&&this->label.begin_label!="main"&&this->label.begin_label!=last_label)
        {
            cout<<label.begin_label<<":"<<endl;
            last_label=label.begin_label;
        }
    if(nodeType==NODE_VAR)
    {
        temp_str=var_name;
        return;
    }
    else if(nodeType==NODE_PROG)
    {
        TreeNode *temp=this->child;
        while(temp)
        {
            temp->recursive_gen_code();
            temp=temp->sibling;
        }
        return;
    }
    else if(nodeType==NODE_FUNC)
    {
        TreeNode *temp=this->child;
        while(temp)
        {
            temp->recursive_gen_code();
            temp=temp->sibling;
        }
        return;
    }
    else if (nodeType == NODE_UMINUS)
    {

        this->child->recursive_gen_code();
        if (temp_str != "")
            cout << "\tmovl\t_" << temp_str << ", %eax" << endl;
        else
        {
            cout << "\tmovl\tt" << use_temp << ", %eax" << endl;
        }
        cout << "\tnegl\t%eax" << endl;
        temp_str = "";
        use_temp++;
        cout << "\tmovl\t%eax, t" << use_temp << endl;
        return;
    }
    else if(nodeType==NODE_OP)
    {
        this->expr_gen_code();
        return;
    }
    else if (nodeType == NODE_STMT)
    {
        this->stmt_gen_code();
        return;
    }
    else
    {
        return;
    }
}

void TreeNode::gen_code()
{
    gen_decl();
    gen_header();
    this->recursive_gen_code();
    cout<<"\tret"<<endl;
}