%{
    #include"common.h"
    extern TreeNode * root;
    extern int yylineno;
    int yylex();
    int yyerror( char const * );
%}
%defines

%start program

%token STRING
%token ID INTEGER
%token INC DEC
%token IF ELSE WHILE FOR
%token INT VOID CHAR
%token LPAREN RPAREN LBRACE RBRACE SEMICOLON COMMA
%token TRUE FALSE
%token AND OR
%token NOT
%token EQUAL LEQ GEQ LSS GTR
%token ASSIGN ADD_ASSIGN SUB_ASSIGN
%token ADD SUB
%token MUL QUO REM
%token PRINTF SCANF
%token QUOTE

%right UMINUS
%left AND OR
%right NOT
%left EQUAL LEQ GEQ LSS GTR
%right ASSIGN ADD_ASSIGN SUB_ASSIGN
%left ADD SUB
%left MUL QUO REM
%nonassoc LOWER_THEN_ELSE
%nonassoc ELSE 
%%

program
    : unit {root=new TreeNode(NODE_PROG,0);root->addChild($1);}
    ;

unit
    : unit func{$$=$1;$1->addSibling($2);}
    | func{$$=$1;}
    ;

func
    : type ID LPAREN func_params RPAREN statement {
        TreeNode *node=new TreeNode(NODE_FUNC,$4->line);
        $$=node;
        node->addChild($4);
        node->addChild($6);}
    | statement {$$=$1;}
    ;
func_params
    : func_param {
        TreeNode *node=new TreeNode(NODE_PARAMS,$1->line);
        node->addChild($1);
        $$=node;
    }
    | func_params COMMA func_param {
        $$=$1;
        $$->addChild($3);
    }
    | {
        TreeNode *node=new TreeNode(NODE_PARAMS,yylineno);
        $$=node;
    } 
    ;

func_param
    : type ID {
        TreeNode *node=new TreeNode(NODE_PARAM,$1->line);
        node->addChild($1);
        node->addChild($2);
        $$=node;
    }
    | type ID ASSIGN expr {
        TreeNode *node=new TreeNode(NODE_PARAM,$1->line);
        node->addChild($1);
        node->addChild($2);
        node->addChild($4);
        $$=node;
    }
    ;

statements
    : statement {$$=$1;}
    | statements statement{$$=$1;$$->addSibling($2);}
    ;
statement
    : instruction {$$=$1;}
    | if_else {$$=$1;}
    | while {$$=$1;}
    | LBRACE statements RBRACE {$$=$2;}
    | var_def {$$=$1;}
    | for {$$=$1;}
    ;
var_def
    : type def_list SEMICOLON {
        TreeNode *node=new TreeNode(NODE_STMT,$1->line);
        node->stmtType=STMT_DECL;
        $$=node;
        $$->addChild($1);
        $$->addChild($2);
     }
    ;
def_list
    : ID {
        TreeNode *node=new TreeNode(NODE_DEFLIST,$1->line);
        node->addChild($1);
        $$=node;
    }
    | def_list COMMA ID {
        $1->addChild($3);
        $$=$1;
    }
    ;
if_else
    : IF bool_statment statement %prec LOWER_THEN_ELSE {
        TreeNode *node=new TreeNode(NODE_STMT,$2->line);
        node->stmtType=STMT_IF;
        node->addChild($2);
        node->addChild($3);
        $$=node;
    }
    | IF bool_statment statement ELSE statement {
        TreeNode *node=new TreeNode(NODE_STMT,$2->line);
        node->stmtType=STMT_IF;
        node->addChild($2);
        node->addChild($3);
        TreeNode *node0=new TreeNode(NODE_STMT,$5->line);
        node0->stmtType=STMT_ELSE;
        node0->addChild($5);
        node->addChild(node0);
        $$=node;
    }
    ;
while
    : WHILE bool_statment statement {
        TreeNode *node=new TreeNode(NODE_STMT,$2->line);
        node->stmtType=STMT_WHILE;
        node->addChild($2);
        node->addChild($3);
        $$=node;
    }
    ;
for
    : FOR LPAREN for1 SEMICOLON for2 SEMICOLON for3 RPAREN statement
    {
        TreeNode *node=new TreeNode(NODE_STMT,$3->line);
        node->stmtType=STMT_FOR;
        node->addChild($3);
        node->addChild($5);
        node->addChild($7);
        node->addChild($9);
        $$=node;
    }
for1
    : ID{
        TreeNode *node=new TreeNode(NODE_STMT,$1->line);
        node->stmtType=STMT_FOR1;
        node->addChild($1);
        $$=node;
    }
    | instruction0{
        TreeNode *node=new TreeNode(NODE_STMT,$1->line);
        node->stmtType=STMT_FOR1;
        node->addChild($1);
        $$=node;
    }
    | {
        TreeNode *node=new TreeNode(NODE_STMT,yylineno);
        node->stmtType=STMT_FOR1;
        $$=node;
    }
    ;
for2
    : bool_expr {
        TreeNode *node=new TreeNode(NODE_STMT,$1->line);
        node->stmtType=STMT_FOR2;
        node->addChild($1);
        $$=node;
    }
    | {
        TreeNode *node=new TreeNode(NODE_STMT,yylineno);
        node->stmtType=STMT_FOR2;
        $$=node;
    }
    ;
for3
    : instruction0{
        TreeNode *node=new TreeNode(NODE_STMT,$1->line);
        node->stmtType=STMT_FOR3;
        node->addChild($1);
        $$=node;
    }
bool_statment
    : LPAREN bool_expr RPAREN {$$=$2;}
    ;
instruction0
    : type ID ASSIGN expr  {
        TreeNode *node=new TreeNode(NODE_STMT,$1->line);
        node->stmtType=STMT_DECL;
        node->addChild($1);
        node->addChild($2);
        node->addChild($4);
        $$=node;
    }
    | ID ASSIGN expr  {
        TreeNode *node=new TreeNode(NODE_STMT,$1->line);
        node->stmtType=STMT_ASSIGN;
        node->addChild($1);
        node->addChild($3);
        $$=node;  
    }
    | ID ADD_ASSIGN expr 
    {
        TreeNode *node=new TreeNode(NODE_STMT,$1->line);
        node->stmtType=STMT_ADDASSIGN;
        node->addChild($1);
        node->addChild($3);
        $$=node;  
    }
    | ID SUB_ASSIGN expr 
    {
        TreeNode *node=new TreeNode(NODE_STMT,$1->line);
        node->stmtType=STMT_SUBASSIGN;
        node->addChild($1);
        node->addChild($3);
        $$=node;  
    }
    |ID INC {
        TreeNode *node=new TreeNode(NODE_STMT,$1->line);
        node->stmtType=STMT_INC;
        node->addChild($1);
        $$=node;
    }
    |ID DEC {
        TreeNode *node=new TreeNode(NODE_STMT,$1->line);
        node->stmtType=STMT_DEC;
        node->addChild($1);
        $$=node;
    }
    | printf  {$$=$1;}
    | scanf  {$$=$1;}
    ;
instruction
    : type ID ASSIGN expr SEMICOLON {
        TreeNode *node=new TreeNode(NODE_STMT,$1->line);
        node->stmtType=STMT_DECL;
        node->addChild($1);
        node->addChild($2);
        node->addChild($4);
        $$=node;
    }
    | ID ASSIGN expr SEMICOLON {
        TreeNode *node=new TreeNode(NODE_STMT,$1->line);
        node->stmtType=STMT_ASSIGN;
        node->addChild($1);
        node->addChild($3);
        $$=node;  
    }
    | ID ADD_ASSIGN expr SEMICOLON
    {
        TreeNode *node=new TreeNode(NODE_STMT,$1->line);
        node->stmtType=STMT_ADDASSIGN;
        node->addChild($1);
        node->addChild($3);
        $$=node;  
    }
    | ID SUB_ASSIGN expr SEMICOLON
    {
        TreeNode *node=new TreeNode(NODE_STMT,$1->line);
        node->stmtType=STMT_SUBASSIGN;
        node->addChild($1);
        node->addChild($3);
        $$=node;  
    }
    |ID INC SEMICOLON{
        TreeNode *node=new TreeNode(NODE_STMT,$1->line);
        node->addChild($1);
        node->addChild($2);
        $$=node;
    }
    |ID DEC SEMICOLON{
        TreeNode *node=new TreeNode(NODE_STMT,$1->line);
        node->addChild($1);
        node->addChild($2);
        $$=node;
    }
    | printf SEMICOLON {$$=$1;}
    | scanf SEMICOLON {$$=$1;}
    ;
printf
    : PRINTF LPAREN expr RPAREN {
        TreeNode *node=new TreeNode(NODE_STMT,yylineno);
        node->stmtType=STMT_PRINTF;
        node->addChild($3);
        $$=node;
    }
    | PRINTF LPAREN STRING COMMA expr RPAREN {
        TreeNode *node=new TreeNode(NODE_STMT,yylineno);
        node->stmtType=STMT_PRINTF;
        node->addChild($3);
        node->addChild($5);
        $$=node;}
    ;
scanf
    : SCANF LPAREN expr RPAREN {
        TreeNode *node=new TreeNode(NODE_STMT,yylineno);
        node->stmtType=STMT_SCANF;
        node->addChild($3);
        $$=node;
    }
    | SCANF LPAREN STRING COMMA QUOTE expr RPAREN {
        TreeNode *node=new TreeNode(NODE_STMT,yylineno);
        node->stmtType=STMT_SCANF;
        node->addChild($3);
        node->addChild($6);
        $$=node;
    }
    ;
bool_expr
    : TRUE {$$=$1;}
    | FALSE {$$=$1;}
    | expr EQUAL expr {
        TreeNode *node=new TreeNode(NODE_OP,$1->line);
        node->opType=OP_EQUAL;
        node->addChild($1);
        node->addChild($3);
        $$=node;
    }
    | expr LEQ expr {
        TreeNode *node=new TreeNode(NODE_OP,$1->line);
        node->opType=OP_LEQ;
        node->addChild($1);
        node->addChild($3);
        $$=node;
    }
    | expr GEQ expr {
        TreeNode *node=new TreeNode(NODE_OP,$1->line);
        node->opType=OP_GEQ;
        node->addChild($1);
        node->addChild($3);
        $$=node;
    }
    | expr LSS expr {
        TreeNode *node=new TreeNode(NODE_OP,$1->line);
        node->opType=OP_LSS;
        node->addChild($1);
        node->addChild($3);
        $$=node;
    }
    | expr GTR expr {
        TreeNode *node=new TreeNode(NODE_OP,$1->line);
        node->opType=OP_GTR;
        node->addChild($1);
        node->addChild($3);
        $$=node;
    }
    | bool_expr AND bool_expr {
        TreeNode *node=new TreeNode(NODE_OP,$1->line);
        node->opType=OP_AND;
        node->addChild($1);
        node->addChild($3);
        $$=node;
    }
    | bool_expr OR bool_expr {
        TreeNode *node=new TreeNode(NODE_OP,$1->line);
        node->opType=OP_OR;
        node->addChild($1);
        node->addChild($3);
        $$=node;
    }
    | NOT bool_expr {
        TreeNode *node=new TreeNode(NODE_OP,$1->line);
        node->opType=OP_NOT;
        node->addChild($2);
        $$=node;        
    }
    ;
expr
    : ID {$$=$1;}
    | SUB ID %prec UMINUS {
        TreeNode *node=new TreeNode(NODE_UMINUS,$1->line);
        node->addChild($2);
        $$=node;
    }
    | INTEGER {$$=$1;}
    | SUB INTEGER %prec UMINUS {$$=$1; $$->int_val=-$$->int_val;}
    | expr ADD expr {
        TreeNode *node=new TreeNode(NODE_OP,$1->line);
        node->opType=OP_ADD;
        node->addChild($1);
        node->addChild($3);
        $$=node;   
    }
    | expr SUB expr {
        TreeNode *node=new TreeNode(NODE_OP,$1->line);
        node->opType=OP_SUB;
        node->addChild($1);
        node->addChild($3);
        $$=node;   
    }
    | expr MUL expr {
        TreeNode *node=new TreeNode(NODE_OP,$1->line);
        node->opType=OP_MUL;
        node->addChild($1);
        node->addChild($3);
        $$=node;   
    }
    | expr QUO expr {
        TreeNode *node=new TreeNode(NODE_OP,$1->line);
        node->opType=OP_QUO;
        node->addChild($1);
        node->addChild($3);
        $$=node;   
    }
    | expr REM expr {
        TreeNode *node=new TreeNode(NODE_OP,$1->line);
        node->opType=OP_REM;
        node->addChild($1);
        node->addChild($3);
        $$=node;   
    }
    ;
type
    : INT {
        TreeNode *node=new TreeNode(NODE_TYPE,yylineno);
        node->varType=VAR_INTEGER;
        $$=node; 
    }
    | VOID {
        TreeNode *node=new TreeNode(NODE_TYPE,yylineno);
        node->varType=VAR_VOID;
        $$=node;         
    }
    | CHAR {
        TreeNode *node=new TreeNode(NODE_TYPE,yylineno);
        node->varType=VAR_CHAR;
        $$=node;  
    }
    ;

%%

void yyerror( char * message )
{
    cout<<yylineno<<endl;
    return;
}