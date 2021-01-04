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

HEX 0x[0-9a-fA-F]+
OCT 0[0-7]+
INTEGER [1-9][0-9]*|0


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

chbegin \'
chelement [^\']|\n|\t|\r
chend \'
%x CHA

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

"return" return RETURN;
"int" return INT;
"void" return VOID;
"bool" return BOOL;
"char" return CHAR;
"const" return CONST;

"else if" return ELIF;
"if" return IF;
"while" return WHILE;
"else" return ELSE;

"for" return FOR;

"printf" return PRINTF;
"scanf" return SCANF;

"=" return ASSIGN;

"++" {
    return INC;
}
"--" {
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
"!=" return NOTEQ;

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



{HEX} {
    TreeNode *node = new TreeNode(NODE_CONST,yylineno);
    int result=0;
    int base=1;
    string s=yytext;
    for(int i=s.length()-1;i>1;i--)
    {
        if(s[i]>='0' && s[i]<='9')
            result+=(int)(s[i]-'0')*base;
        else if(s[i]>='a'&&s[i]<='f')
        {
            result+=((int)(s[i]-'a')+10)*base;
        }
        else if(s[i]>='A'&&s[i]<='F')
        {
            result+=((int)(s[i]-'A')+10)*base;
        }
        base*=16;
    }
    node->int_val=result;
    yylval = node;
    return INTEGER;
}

{OCT} {
    TreeNode *node = new TreeNode(NODE_CONST,yylineno);
    int result=0;
    int base=1;
    string s=yytext;
    for(int i=s.length()-1;i>0;i--)
    {
        if(s[i]>='0' && s[i]<='7')
            result+=(int)(s[i]-'0')*base;
        base*=8;
    }
    node->int_val = result;
    yylval = node;
    return INTEGER;
}
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

{chbegin} {BEGIN CHA;}
<CHA>{chelement} {s=s+yytext;}
<CHA>{chend} {    
    TreeNode *node = new TreeNode(NODE_CHAR,yylineno);
    if(s[1]=='n')
        node->int_val=(int)'\n';
    else if(s[1]=='t')
        node->int_val=(int)'\t';
   else
    node->int_val = (int)s[0];
    yylval = node;
    BEGIN INITIAL;
    s="";

    return CHARACTER;}
%%