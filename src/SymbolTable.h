#ifndef __SymbolTable__
#define __SymbolTable__


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXNAME 32
#define MAXSYMB 256


typedef struct STable{
	char name[MAXNAME];
	char type[MAXNAME];
	char champs[MAXSYMB][MAXNAME];
	int struct_size;
}STable;


typedef struct SymbolTable {
	char stname[MAXNAME];
	STable symbol[MAXSYMB];
	int size;
	struct SymbolTable *next;
} Sym, *ST;

ST allocateSymbol(char name[]);
ST addSymbolTable(ST *st, char name[]);
int searchSymbolTable(ST st, char name[]);
int searchVarInST(ST st, char nameST[], char nameStruct[], char var[]);
int searchTypeOfVar(ST st, char nameST[], char nameStruct[], char var[]);
int searchFunction(ST st, char func_name[]);
int searchTypeOfFunc(ST st, char func_name[]);
int searchNumArg(ST st, char func_name[]);
int isNotaChamp(ST st, char func_name[], char var[]);
int is_Struct(ST st, char func_name[], char var_struct[]);
void printSymbolTable(ST st);
void addChamps(ST *st, char name[], char struct_name[], char champ_type[], char champ_id[]);
void addVars(ST *st, char name[], char type[], char id[]);

#endif