%{
  /* lexer.l */
  /* Compatible avec parser.y */
  #define MAXNAME 30
  #include "Projet.tab.h"
  int lineno=1;
  int charno=0;
  int number;
  char* line = NULL;
  char id[64];
  char type[4];
  char addsub;
  char divstar;
  char order[2];
  char equal[2];
%}

%option noinput nounput noyywrap
%x SHORT_COMMENT LONG_COMMENT
%%
[ \t\r]+		{charno += yyleng;}
\n			{charno=0; lineno++;}
"/*"			{ charno += yyleng;BEGIN LONG_COMMENT; }
"//"			{ charno += yyleng;BEGIN SHORT_COMMENT; }
&&			{ charno += yyleng;return AND; }
"||"			{ charno += yyleng;return OR; }
"+"|-			{addsub = yytext[0]; charno += yyleng; return ADDSUB; }
"*"|"/"|"%"			{ divstar = yytext[0]; charno += yyleng;return DIVSTAR; }
"<"|"<="|">"|>=		{ memset(order, 0, sizeof(order)); strcpy(order, yytext); charno += yyleng;return ORDER; }
==|!=			{ memset(equal, 0, sizeof(equal)); strcpy(equal, yytext); charno += yyleng;return EQ; }
int			{memset(type, 0, sizeof(type)); strcpy(type, yytext); charno += yyleng;return SIMPLETYPE;}
char		{memset(type, 0, sizeof(type)); strcpy(type, yytext); charno += yyleng;return SIMPLETYPE;}
print			{ charno += yyleng;return PRINT; }
readc			{ charno += yyleng;return READC; }
reade			{ charno += yyleng;return READE; }
void			{ charno += yyleng;return VOID; }
struct		{ charno += yyleng;return STRUCT; }
if			{ charno += yyleng;return IF; }
else			{ charno += yyleng;return ELSE; }
while			{ charno += yyleng;return WHILE; }
return			{ charno += yyleng;return RETURN; }
[a-zA-Z_][a-zA-Z0-9_]* { memset(id, 0, sizeof(id)); strcpy(id, yytext); charno += yyleng; return IDENT; }
[0-9]+			{ charno += yyleng; number = atoi(yytext); return NUM;}
'\\?.'			{ charno += yyleng; memset(id, 0, sizeof(id)); strcpy(id, yytext); return CHARACTER; }
^.*[\n]			{free(line); line = (char *)malloc(sizeof(char) * 1024); strcpy(line, yytext); REJECT;}
.			{ charno += yyleng; return yytext[0];}
<LONG_COMMENT>"*/"		{ BEGIN INITIAL; charno += yyleng; }
<LONG_COMMENT,SHORT_COMMENT>.		{charno += yyleng;}
<LONG_COMMENT>\n		{lineno++;charno=0;}
<SHORT_COMMENT>\n		{BEGIN INITIAL; lineno++;charno=0;}
%%



