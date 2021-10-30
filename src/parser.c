#include "parser.h"

void parcours(Node *t, ST *st){
	Node* type;
	Node* id;
	Node* parametres;
	Node* VarDeclF;
	Node* tmp;
	if(t->kind == VariablesDecl){ 		/*Ajout des variables globales dans la tables des symboles*/
		if(t->firstChild != NULL){
			addSymbolTable(st, "Variables Globales");
			type = t->firstChild;
			while(type != NULL){
				if(type->kind == Structure){
					addVars(st, "Variables Globales", "Structure", type->u.identifier);
					tmp = type->firstChild;
					while(tmp != NULL){
						id = tmp->firstChild;
						while(id != NULL){
							addChamps(st, "Variables Globales", type->u.identifier, tmp->u.identifier, id->u.identifier);
							id = id->nextSibling;
						}
						tmp = tmp->nextSibling;
					}
				}
				else{
					id = type->firstChild;
					while(id != NULL){
						addVars(st, "Variables Globales", type->u.identifier, id->u.identifier);
						id = id->nextSibling;
					}
				}
				type = type->nextSibling;
			}
		}
	}

	if(t->kind == Function){  /*Ajout des variables de fonctions*/
		if(t->firstChild != NULL){
			addSymbolTable(st, t->firstChild->u.identifier);
			VarDeclF = t->firstChild->nextSibling;
			parametres = t->firstChild->firstChild;
			if(strcmp(parametres->u.identifier, "None") != 0){
				parametres = parametres->firstChild->firstChild;
				while(parametres != NULL){
					char typ[MAXNAME];
					char ide[MAXNAME];
					int i;
					int j;
					for(i = 0; i < MAXNAME; i++){
						if(parametres->u.identifier[i] == ' '){
							i++;
							break;
						}
						typ[i] = parametres->u.identifier[i];
					}
					typ[i] = '\0';
					for(j = 0; j < MAXNAME; j++){
						if(parametres->u.identifier[i] == '\0')
							break;
						ide[j] = parametres->u.identifier[i];
						i++;
					}
					ide[j] = '\0';
					addVars(st, t->firstChild->u.identifier, typ, ide);
					parametres = parametres->nextSibling;
				}
			}
			if(VarDeclF->kind == Body){
				VarDeclF = VarDeclF->firstChild;
				if(VarDeclF->firstChild != NULL){
					VarDeclF = VarDeclF->firstChild;
					while(VarDeclF != NULL){
						tmp = VarDeclF->firstChild;
						while(tmp != NULL){
							addVars(&(*st), t->firstChild->u.identifier, VarDeclF->u.identifier, tmp->u.identifier);
							tmp = tmp->nextSibling;
						}
						VarDeclF = VarDeclF->nextSibling;
					}
				}
			}

		}
	}
	if(t->nextSibling != NULL)
		parcours(t->nextSibling, st);
	if(t->firstChild != NULL)
		parcours(t->firstChild, st);
}

int searchNumArgInTree(Node *t, char func_name[]){
	Node *tmp;
	if(t->kind == FunctionCall){
		tmp = t;
		if(strcmp(func_name, tmp->u.identifier) == 0){
			if(tmp->firstChild->firstChild == NULL)
				return 0;
			else{
				int count = 0;
				tmp = tmp->firstChild->firstChild;
				while(tmp != NULL){
					count++;
					tmp = tmp->nextSibling;
				}
				return count;
			}
		}
	}
	if(t->nextSibling != NULL)
		return searchNumArgInTree(t->nextSibling, func_name);
	if(t->firstChild != NULL)
		return searchNumArgInTree(t->firstChild, func_name);
	return -1;
}

int checkAssignement(ST st, Node* tmp, Node* funcName){
	if(tmp->kind == Assignement){
		if(tmp->firstChild->kind == Identifier && (searchTypeOfVar(st, funcName->u.identifier, NULL, tmp->firstChild->u.identifier) == 1 || searchTypeOfVar(st, "Variables Globales", NULL, tmp->firstChild->u.identifier) == 1)){
			if(tmp->firstChild->nextSibling->kind != CharLiteral && tmp->firstChild->nextSibling->kind != FunctionCall && tmp->firstChild->nextSibling->kind != Identifier && tmp->firstChild->nextSibling->kind != ChampCall)
				printf("warning : line %d affecting to '%s' (char value) an int expression\n", tmp->lineno, tmp->firstChild->u.identifier);
			else{
				if(tmp->firstChild->nextSibling->kind == Identifier){
					if(searchTypeOfVar(st, funcName->u.identifier, NULL, tmp->firstChild->nextSibling->u.identifier) == 0 || searchTypeOfVar(st, "Variables Globales", NULL, tmp->firstChild->nextSibling->u.identifier) == 0){
						printf("warning : line %d affecting to '%s' (char value) an int expression\n", tmp->lineno, tmp->firstChild->u.identifier);
					}
				}
				if(tmp->firstChild->nextSibling->kind == ChampCall){
					if(searchTypeOfVar(st, funcName->u.identifier, tmp->firstChild->nextSibling->u.identifier, tmp->firstChild->nextSibling->firstChild->u.identifier) == 0 || searchTypeOfVar(st, "Variables Globales", tmp->firstChild->nextSibling->u.identifier, tmp->firstChild->nextSibling->firstChild->u.identifier) == 0){
						printf("warning : line %d affecting to '%s' (char value) an int expression\n", tmp->lineno, tmp->firstChild->u.identifier);
					}
				}
				if(tmp->firstChild->nextSibling->kind == FunctionCall){
					if(searchTypeOfFunc(st, tmp->firstChild->nextSibling->u.identifier) == 1){
						printf("warning : line %d affecting to '%s' (char value) an int expression\n", tmp->lineno, tmp->firstChild->u.identifier);
					}
				}
			}
		}
		else if(tmp->firstChild->kind == ChampCall && (searchTypeOfVar(st, funcName->u.identifier, tmp->firstChild->u.identifier, tmp->firstChild->firstChild->u.identifier) == 1 || searchTypeOfVar(st, "Variables Globales", tmp->firstChild->u.identifier, tmp->firstChild->firstChild->u.identifier) == 1)){
			if(tmp->firstChild->nextSibling->kind != CharLiteral && tmp->firstChild->nextSibling->kind != FunctionCall && tmp->firstChild->nextSibling->kind != Identifier && tmp->firstChild->nextSibling->kind != ChampCall)
				printf("warning : line %d affecting to '%s' from struct '%s' (char value) an int expression\n", tmp->lineno, tmp->firstChild->firstChild->u.identifier, tmp->firstChild->u.identifier);
			else{
				if(tmp->firstChild->nextSibling->kind == Identifier){
					if(searchTypeOfVar(st, funcName->u.identifier, NULL, tmp->firstChild->nextSibling->u.identifier) == 0 || searchTypeOfVar(st, "Variables Globales", NULL, tmp->firstChild->nextSibling->u.identifier) == 0){
						printf("warning : line %d affecting to '%s' from struct '%s' (char value) an int expression\n", tmp->lineno, tmp->firstChild->firstChild->u.identifier, tmp->firstChild->u.identifier);
					}
				}
				if(tmp->firstChild->nextSibling->kind == ChampCall){
					if(searchTypeOfVar(st, funcName->u.identifier, tmp->firstChild->nextSibling->u.identifier, tmp->firstChild->nextSibling->firstChild->u.identifier) == 0 || searchTypeOfVar(st, "Variables Globales", tmp->firstChild->nextSibling->u.identifier, tmp->firstChild->nextSibling->firstChild->u.identifier) == 0){
						printf("warning : line %d affecting to '%s' from struct '%s' (char value) an int expression\n", tmp->lineno, tmp->firstChild->firstChild->u.identifier, tmp->firstChild->u.identifier);
					}
				}
				if(tmp->firstChild->nextSibling->kind == FunctionCall){
					if(searchTypeOfFunc(st, tmp->firstChild->nextSibling->u.identifier) == 0){
						printf("warning : line %d affecting to '%s' from struct '%s' (char value) an int expression\n", tmp->lineno, tmp->firstChild->firstChild->u.identifier, tmp->firstChild->u.identifier);
					}
				}
			}
		}
	}
	if(tmp->firstChild != NULL)
		return checkAssignement(st, tmp->firstChild, funcName);
	if(tmp->nextSibling != NULL)	
		return checkAssignement(st, tmp->nextSibling, funcName);
	return 0;
}

int checkReturn(ST st, Node* tmp, Node* funcName){
	if(tmp->kind ==  Return){
		char type[5];
		int i;
		for(i=0; i < 4; i++){
			if(funcName->u.identifier[i] == ' ')
				break;
			type[i] = funcName->u.identifier[i];
		}
		type[i] = '\0';
		if(strcmp("void", type) == 0 && tmp->firstChild != NULL){
			if(tmp->firstChild->kind == IntLiteral)
				printf("semantic error : line %d void function returning an integer\n", tmp->lineno);
			else
				printf("semantic error : line %d void function returning a character\n", tmp->lineno);
			return 2;
		}
		if(tmp->firstChild != NULL){
			if(strcmp("char", type) == 0 && tmp->firstChild->kind == IntLiteral){
				printf("warning : line %d char function returning an integer\n", tmp->lineno);
			}
		}
		
		if(strcmp("void", type) != 0 && tmp->firstChild == NULL){
			printf("warning : line %d %s function returning nothing\n", tmp->lineno, type);
		}
	}

	if(tmp->firstChild != NULL)
		return checkReturn(st, tmp->firstChild, funcName);
	if(tmp->nextSibling != NULL)	
		return checkReturn(st, tmp->nextSibling, funcName);
	return 0;
}




int checkIdentifier(ST st, Node *tmp, Node* funcName){
	if(tmp->kind == Identifier){
		if(searchVarInST(st, funcName->u.identifier, NULL, tmp->u.identifier) == 0 && searchVarInST(st, "Variables Globales", NULL, tmp->u.identifier) == 0 && isNotaChamp(st, funcName->u.identifier, tmp->u.identifier) == 0 && isNotaChamp(st, "Variables Globales", tmp->u.identifier) == 0){
			printf("semantic error line %d undefined variable '%s'\n", tmp->lineno, tmp->u.identifier);
			return 2;
		}
	}
	if(tmp->firstChild != NULL)
		return checkIdentifier(st, tmp->firstChild, funcName);
	if(tmp->nextSibling != NULL)	
		return checkIdentifier(st, tmp->nextSibling, funcName);
	return 0; 
}


int checkChampCall(ST st, Node *tmp, Node* funcName){
	if(tmp->kind == ChampCall){
		if(is_Struct(st, funcName->u.identifier, tmp->u.identifier) == 0 && is_Struct(st, "Variables Globales", tmp->u.identifier) == 0){
			printf("%s\n", tmp->u.identifier);
			printf("semantic error line %d struct '%s' undefined\n", tmp->lineno, tmp->u.identifier);
			return 2;
		}
		if(searchVarInST(st, funcName->u.identifier, tmp->u.identifier, tmp->firstChild->u.identifier) == 0 && searchVarInST(st, "Variables Globales", tmp->u.identifier, tmp->firstChild->u.identifier) == 0){
			printf("semantic error line %d variable '%s' not defined in struct '%s'\n", tmp->firstChild->lineno, tmp->firstChild->u.identifier, tmp->u.identifier);
			return 2;
		}
	}
	if(tmp->firstChild != NULL)
		return checkChampCall(st, tmp->firstChild, funcName);
	if(tmp->nextSibling != NULL)	
		return checkChampCall(st, tmp->nextSibling, funcName);
	return 0;
}

int checkFunctionCall(ST st, Node* tmp, Node* funcName){
	if(tmp->kind == FunctionCall){
		if(strcmp(tmp->u.identifier, "reade") == 0 || strcmp(tmp->u.identifier, "readc") == 0 || strcmp(tmp->u.identifier, "print") == 0){
			if(tmp->firstChild->kind == ChampCall){
				if(strcmp(tmp->u.identifier, "reade") == 0){
					if(searchTypeOfVar(st, funcName->u.identifier, tmp->firstChild->u.identifier, tmp->firstChild->firstChild->u.identifier) == 1 || searchTypeOfVar(st, "Variables Globales", tmp->firstChild->u.identifier, tmp->firstChild->firstChild->u.identifier) == 1){
						printf("warning : line %d argument '%s' from struct %s passed to reade is of type char\n", tmp->lineno, tmp->firstChild->firstChild->u.identifier, tmp->firstChild->u.identifier);
					}
				}
			}
			else{
				if(strcmp(tmp->u.identifier, "reade") == 0){
					if(searchTypeOfVar(st, funcName->u.identifier, NULL, tmp->firstChild->u.identifier) == 1 || searchTypeOfVar(st, "Variables Globales", NULL, tmp->firstChild->u.identifier) == 1)
						printf("warning : line %d argument '%s' passed to reade is of type char\n", tmp->lineno, tmp->firstChild->u.identifier);
				}
			}
		}
		else{
			if(searchFunction(st, tmp->u.identifier) == 0){
				printf("semantic error : line %d function '%s' not defined\n", tmp->lineno, tmp->u.identifier);
				return 2;
			}
			
			if(searchNumArgInTree(tmp, tmp->u.identifier) != searchNumArg(st, tmp->u.identifier)){
				if(searchNumArgInTree(tmp, tmp->u.identifier) > searchNumArg(st, tmp->u.identifier))
					printf("semantic error : too much arguments in function '%s', %d were expected and %d got given\n", tmp->u.identifier, searchNumArg(st, tmp->u.identifier), searchNumArgInTree(tmp, tmp->u.identifier));
				else
					printf("semantic error : not enough arguments in function '%s', %d were expected and %d got given\n", tmp->u.identifier, searchNumArg(st, tmp->u.identifier), searchNumArgInTree(tmp, tmp->u.identifier));
				return 2;
			}
		}	
	}
	if(tmp->firstChild != NULL)
		return checkFunctionCall(st, tmp->firstChild, funcName);
	if(tmp->nextSibling != NULL)	
		return checkFunctionCall(st, tmp->nextSibling, funcName);
	return 0;
}



int parcoursSemantique(Node *t, ST st){
	Node* funcName;
	Node *instr;
	Node *tmp;
	if(t->kind ==  Function){
		if(t->firstChild->nextSibling->kind == Body){
			funcName = t->firstChild;
			if(t->firstChild->nextSibling->firstChild->nextSibling->kind == Instructions){
				instr = t->firstChild->nextSibling->firstChild->nextSibling;
				if(instr->firstChild != NULL){
					tmp = instr->firstChild;
					if(checkChampCall(st, tmp, funcName) == 2)
						return 2;
					if(checkIdentifier(st, tmp, funcName) == 2)
						return 2;
					if(checkFunctionCall(st, tmp, funcName) == 2)
						return 2;
					if(checkReturn(st, tmp, funcName) == 2)
						return 2;
					if(checkAssignement(st, tmp, funcName) == 2)
						return 2;
				}
			}
		}
	}
	if(t->nextSibling != NULL)
		return parcoursSemantique(t->nextSibling, st);
	if(t->firstChild != NULL)
		return parcoursSemantique(t->firstChild, st);
	return 0;
}