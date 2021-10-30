%{
/* parser.y */
/* Syntaxe du TPC pour le projet d'analyse syntaxique de 2020-2021 */
  #include <stdio.h>
  #include <string.h>
  extern int lineno;
  extern int charno;
  extern char id[64];
  extern char type[4];
  extern int number;
  extern char* line;
  extern char addsub;
  extern char divstar;
  extern char order[2];
  extern char equal[2];
  char name_struct[20];
  char name_func[20];
  char type_func[4];
  int yylex();
  void yyerror(const char *);
%}

%code requires{
  #include "abstract-tree.h"
  Node *t;
}

%union{
    Node *tree;
}


%token CHARACTER
%token NUM
%token IDENT
%token SIMPLETYPE
%token ORDER EQ
%token ADDSUB
%token DIVSTAR
%token OR AND STRUCT IF WHILE RETURN VOID PRINT READC READE
%precedence ')'
%precedence ELSE

%type <tree> Prog TypesVars DeclFoncts Type Declarateurs DeclChamps DeclFonct EnTeteFonct Corps Parametres ListTypVar DeclVars SuiteInstr Instr LValue Exp TB FB M E T F Arguments ListExp

%%

Prog: TypesVars DeclFoncts {$$ = makeNode(Program); $$->lineno = lineno; addChild($$, $1); addChild($$, $2); t = $$;}
    ;
TypesVars:
       TypesVars Type Declarateurs ';' {addChild($$, $2); addChild($2, $3);}
    |  TypesVars STRUCT IDENT {memset(name_func, 0, sizeof(name_func)); strcpy(name_func, id);} '{' DeclChamps '}' ';' {Node *tmp = makeNode(Structure); tmp->lineno = lineno ;strcpy(tmp->u.identifier, name_func); addChild(tmp, $6); addChild($$, tmp);}
    |  %empty {$$ = makeNode(VariablesDecl); $$->lineno=lineno;}
    ;
Type:
       SIMPLETYPE {$$ = makeNode(VariablesList); strcpy($$->u.identifier, type); $$->lineno = lineno;}
    |  STRUCT IDENT {$$ = makeNode(Structure); $$->lineno = lineno; strcpy($$->u.identifier, id);}
    ;
Declarateurs:
       Declarateurs ',' IDENT {Node *tmp = makeNode(Identifier); tmp->lineno = lineno; strcpy(tmp->u.identifier, id); addSibling($$, tmp);}
    |  IDENT {$$ = makeNode(Identifier); $$->lineno = lineno; strcpy($$->u.identifier, id);}
    ;
DeclChamps :
       DeclChamps SIMPLETYPE Declarateurs ';' {if(strcmp($$->u.identifier, type) == 0) addChild($$, $3); else{Node *tmp = makeNode(Champ); tmp->lineno = lineno; strcpy(tmp->u.identifier, type); addChild(tmp, $3); addSibling($$, tmp);}}
    |  SIMPLETYPE Declarateurs ';' {$$ = makeNode(Champ); $$->lineno = lineno; strcpy($$->u.identifier, type); addChild($$, $2);}
    ;
DeclFoncts:
       DeclFoncts DeclFonct {addChild($$, $2);}
    |  DeclFonct {$$ = makeNode(FuncsDecl);$$->lineno = lineno; addChild($$, $1);}
    ;
DeclFonct:
       EnTeteFonct Corps {$$ = makeNode(Function);$$->lineno = lineno; addChild($$, $1); addChild($$, $2);}
    ;
EnTeteFonct:
       Type IDENT {memset(name_func, 0, sizeof(name_func)); memset(type_func, 0, sizeof(type_func)); strcpy(name_func, id); strcpy(type_func, type);} '(' Parametres ')' {$$ = makeNode(Prototype); $$->lineno = lineno; strcpy($$->u.identifier, type_func); strcat($$->u.identifier, " "); strcat($$->u.identifier, name_func); addChild($$, $5);}
    |  VOID IDENT {memset(name_func, 0, sizeof(name_func));  strcpy(name_func, id);} '(' Parametres ')' {$$ = makeNode(Prototype); $$->lineno = lineno; strcpy($$->u.identifier, "void"); strcat($$->u.identifier, " "); strcat($$->u.identifier, name_func); addChild($$, $5);}
    ;
Parametres:
       VOID {$$ = makeNode(Parametres); $$->lineno = lineno; strcpy($$->u.identifier, "None");}
    |  ListTypVar {$$ = makeNode(Parametres); $$->lineno = lineno; addChild($$, $1);}
    ;
ListTypVar:
       ListTypVar ',' Type IDENT {Node *tmp = makeNode(Identifier); tmp->lineno = lineno;strcpy(tmp->u.identifier, type); strcat(tmp->u.identifier, " "); strcat(tmp->u.identifier, id); addChild($$, tmp);}
    |  Type IDENT {$$ = makeNode(VariablesList); $$->lineno = lineno; Node *tmp = makeNode(Identifier); tmp->lineno = lineno; strcpy(tmp->u.identifier, type); strcat(tmp->u.identifier, " "); strcat(tmp->u.identifier, id); addChild($$, tmp);}
    ;
Corps: '{' DeclVars SuiteInstr '}' {$$ = makeNode(Body); $$->lineno = lineno; addChild($$, $2); addChild($$, $3);}
    ;
DeclVars:
       DeclVars Type Declarateurs ';' {addChild($$, $2); addChild($2, $3);}
    |  %empty {$$ = makeNode(DeclVarFunc); $$->lineno = lineno;}
    ;
SuiteInstr:
       SuiteInstr Instr {addChild($$, $2);}
    |  %empty {$$ = makeNode(Instructions); $$->lineno = lineno;}
    ;
Instr:
       LValue '=' Exp ';' {$$ = makeNode(Assignement); $$->lineno = lineno; addChild($$, $1); addChild($$, $3);}
    |  READE '(' LValue ')' ';' {$$ = makeNode(FunctionCall); $$->lineno = lineno; strcpy($$->u.identifier, "reade"); addChild($$, $3);}
    |  READC '(' LValue ')' ';' {$$ = makeNode(FunctionCall); $$->lineno = lineno; strcpy($$->u.identifier, "readc"); addChild($$, $3);}
    |  PRINT '(' Exp ')' ';' {$$ = makeNode(FunctionCall); $$->lineno = lineno; strcpy($$->u.identifier, "print"); addChild($$, $3);}
    |  IF '(' Exp ')' Instr {$$ = makeNode(Condition); $$->lineno = lineno; Node* tmp = makeNode(If); tmp->lineno = lineno; addChild(tmp, $3); Node* tmp2 = makeNode(Then); tmp2->lineno = lineno; addSibling(tmp, tmp2); addChild($$, tmp);}
    |  IF '(' Exp ')' Instr ELSE Instr {$$ = makeNode(Condition); $$->lineno = lineno; Node* tmp = makeNode(If); tmp->lineno = lineno; addChild(tmp, $3); Node* tmp2 = makeNode(Then); tmp2->lineno = lineno; addChild(tmp2, $5); addSibling(tmp, tmp2); Node* tmp3 = makeNode(Else); tmp3->lineno = lineno; addChild(tmp3, $7); addSibling(tmp, tmp3); addChild($$, tmp);}
    |  WHILE '(' Exp ')' Instr {$$ = makeNode(Loop); $$->lineno = lineno; Node* tmp = makeNode(While); tmp->lineno = lineno; addChild(tmp, $3); Node *tmp2 = makeNode(Then); tmp2->lineno = lineno; addChild(tmp2, $5); addSibling(tmp, tmp2); addChild($$, tmp);}
    |  Exp ';' {$$ = $1;}
    |  RETURN Exp ';' {$$ = makeNode(Return); $$->lineno = lineno; addChild($$, $2);}
    |  RETURN ';' {$$ = makeNode(Return); $$->lineno = lineno;}
    |  '{' SuiteInstr '}' {$$ = $2;}
    |  ';' {}
    ;
Exp :  Exp OR TB {$$ = makeNode(Or); $$->lineno = lineno; addChild($$, $1); addChild($$, $3);}
    |  TB
    ;
TB  :  TB AND FB {$$ = makeNode(And); $$->lineno = lineno; addChild($$, $1); addChild($$, $3);}
    |  FB
    ;
FB  :  FB EQ M {$$ = makeNode(Equals); $$->lineno = lineno; strcpy($$->u.identifier, equal); addChild($$, $1); addChild($$, $3);}
    |  M
    ;
M   :  M ORDER E {$$ = makeNode(Order); $$->lineno = lineno; strcpy($$->u.identifier, order); addChild($$, $1); addChild($$, $3);}
    |  E
    ;
E   :  E ADDSUB T {$$ = makeNode(AddSub); $$->lineno = lineno; $$->u.character = addsub; addChild($$, $1); addChild($$, $3);}
    |  T
    ;
T   :  T DIVSTAR F {$$ = makeNode(DivStar); $$->lineno = lineno; $$->u.character = divstar; addChild($$, $1), addChild($$, $3);}
    |  F
    ;
F   :  ADDSUB F {$$ = makeNode(AddSub); $$->lineno = lineno; $$->u.character = addsub; addChild($$, $2);}
    |  '!' F {$$ = makeNode(Not); $$->lineno = lineno; addChild($$, $2);}
    |  '(' Exp ')' {$$ = $2;}
    |  NUM {$$ = makeNode(IntLiteral); $$->lineno = lineno; $$->u.integer = number;}
    |  CHARACTER {$$ = makeNode(CharLiteral); $$->lineno = lineno; $$->u.character = id[1];}
    |  LValue {$$ = $1;}
    |  IDENT {memset(name_func, 0, sizeof(name_func)); strcpy(name_func, id);} '(' Arguments ')' {$$ = makeNode(FunctionCall); $$->lineno = lineno; strcpy($$->u.identifier, name_func); addChild($$, $4);}
    ;
LValue:
       IDENT {$$ = makeNode(Identifier); $$->lineno = lineno; strcpy($$->u.identifier, id);}
    |  IDENT '.' {memset(name_struct, 0, sizeof(name_struct)); strcpy(name_struct, id);} IDENT {$$ = makeNode(ChampCall); strcpy($$->u.identifier, name_struct); $$->lineno = lineno; Node *tmp = makeNode(Identifier); tmp->lineno = lineno; strcpy(tmp->u.identifier, id); addChild($$, tmp);}
    ;
Arguments:
       ListExp {$$ = makeNode(Arguments); $$->lineno = lineno; addChild($$, $1);}
    |  %empty {$$ = makeNode(Arguments); $$->lineno = lineno;}
    ;
ListExp:
       ListExp ',' Exp {addSibling($$, $3);}
    |  Exp {$$ = $1;}
    ;

%%


void yyerror(const char *s){int i;
    printf("%s near character %d line %d\n", s, charno, lineno);
    printf("%s", line);
    for(i = 0; i < charno - 1; i++){
        printf(" ");
    }
    printf("^\n");
    free(line);
    return;
}