%option noyywrap
%option yylineno
%{
    #include"common.h"
    #include"main.tab.hh"
    #include<string>
%}

%{
    string s="";
%}

INTEGER -?[0-9]+
ID [[:alpha:]_][[:alpha:][:digit:]_]*

EOL (\r\n|\n)
WHITE [\t ]

commentbegin "/*"
commentelement .|\n
commentend "*/"
%x COMMENT

commentbegin0 "//"
commentelement0 .
commentend0 \n
%x COMMENT0

strbegin "\""
strelement [^\"]
strend "\""
%x STR

%%

"true" {
    TreeNode *node = new TreeNode(NODE_BOOL,yylineno);
    node->bool_val = true;
    yylval = node;
    return TRUE;
}
"false" {
    TreeNode *node = new TreeNode(NODE_BOOL,yylineno);
    node->bool_val = false;
    yylval = node;
    return FALSE;
}

"int" return INT;
"void" return VOID;
"char" return CHAR;

"if" return IF;
"while" return WHILE;
"else" return ELSE;
"for" return FOR;

"printf" return PRINTF;
"scanf" return SCANF;

"=" return ASSIGN;

"++" {
    TreeNode *node = new TreeNode(NODE_INC,yylineno);
    yylval = node;
    return INC;
}
"--" {
    TreeNode *node = new TreeNode(NODE_DEC,yylineno);
    yylval = node;
    return DEC;
}

"+=" return ADD_ASSIGN;
"-=" return SUB_ASSIGN;

"+" return ADD;
"-" return SUB;
"*" return MUL;
"/" return QUO;
"%" return REM;

"!" return NOT;
"==" return EQUAL;

"<=" return LEQ;
">=" return GEQ;
"<" return LSS;
">" return GTR;

"&&" return AND;
"||" return OR;

"&" return QUOTE;
";" return SEMICOLON;
"," return COMMA;
"(" return LPAREN;
")" return RPAREN;
"{" return LBRACE;
"}" return RBRACE;

{INTEGER} {
    TreeNode *node = new TreeNode(NODE_CONST,yylineno);
    node->int_val = atoi(yytext);
    yylval = node;
    return INTEGER;
}
{ID} {
    TreeNode *node = new TreeNode(NODE_VAR,yylineno);
    node->var_name = string(yytext);
    yylval = node;
    return ID;
}

{EOL}
{WHITE}

{commentbegin} {BEGIN COMMENT;}
<COMMENT>{commentelement} {}
<COMMENT>{commentend} {BEGIN INITIAL;}

{commentbegin0} {BEGIN COMMENT0;}
<COMMENT0>{commentelement0} {}
<COMMENT0>{commentend0} {BEGIN INITIAL;}

{strbegin} {BEGIN STR;}
<STR>{strelement} {s=s+yytext;}
<STR>{strend} {    
    TreeNode *node = new TreeNode(NODE_STR,yylineno);
    node->var_name = s;
    yylval = node;
    BEGIN INITIAL;
    s="";
    return STRING;}
%%