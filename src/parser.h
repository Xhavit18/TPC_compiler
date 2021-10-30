#ifndef __parser__
#define __parser__

#include <string.h>
#include "abstract-tree.h"
#include "SymbolTable.h"

extern int yyparse();
extern Node* t;
extern int lineno;
extern int charno;

void parcours(Node *t, ST *st);
int searchNumArgInTree(Node *t, char func_name[]);
int checkAssignement(ST st, Node* tmp, Node* funcName);
int checkReturn(ST st, Node* tmp, Node* funcName);
int checkIdentifier(ST st, Node *tmp, Node* funcName);
int checkChampCall(ST st, Node *tmp, Node* funcName);
int checkFunctionCall(ST st, Node* tmp, Node* funcName);
int parcoursSemantique(Node *t, ST st);

#endif