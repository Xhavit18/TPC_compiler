#include "SymbolTable.h"

ST allocateSymbol(char name[]){
	ST tmp;
	int i;
	tmp = (Sym *)malloc(sizeof(Sym));
	if(tmp != NULL){
		strcpy(tmp->stname, name);
		tmp->size = 0;
		for(i = 0; i < MAXSYMB; i++){
			tmp->symbol[i].struct_size = 0;
		}
	}
	return tmp;
}

ST addSymbolTable(ST *st, char name[]){
	ST tmp;
	tmp = allocateSymbol(name);
	if(tmp != NULL){
		tmp->next = *st;
		*st = tmp;
	}
	return tmp;
}

int searchSymbolTable(ST st, char name[]){
	ST tmp = st;
	while(tmp != NULL){
		if(strcmp(tmp->stname, name) == 0)
			return 1;
		tmp = tmp->next;
	}
	return 0;
}

int searchVarInST(ST st, char nameST[], char nameStruct[], char var[]){
	ST tmp = st;
	if(searchSymbolTable(st, nameST) == 0)
		return 0;
	if(nameStruct == NULL){
		while(tmp != NULL){
			if(strcmp(tmp->stname, nameST) == 0){
				int i;
				for(i = 0; i < tmp->size; i++){
					if(strcmp(var, tmp->symbol[i].name) == 0){
						return 1;
					}
				}
			}
			tmp = tmp->next;
		}
	}
	else{
		while(tmp != NULL){
			if(strcmp(tmp->stname, nameST) == 0){
				int i;
				int j;
				int k;
				int cpt;
				char name_var[30];
				for(i = 0; i < tmp->size; i++){
					if(strcmp(nameStruct, tmp->symbol[i].name) == 0){
						for(j = 0; j < tmp->symbol[i].struct_size; j++){
							for(k = 0; tmp->symbol[i].champs[j][k-1] != ' '; k++)
							cpt = 0;
							while(tmp->symbol[i].champs[j][k] != '\0'){
								name_var[cpt] = tmp->symbol[i].champs[j][k];
								k++;
								cpt++;
							}
							name_var[cpt] = '\0';
							if(strcmp(var, name_var) == 0)
								return 1;
							memset(name_var, 0, sizeof(name_var));
						}
					}
				}
			}
			tmp = tmp->next;
		}
	}
	return 0;
}

int searchTypeOfVar(ST st, char nameST[], char nameStruct[], char var[]){
	ST tmp = st;
	if(nameStruct == NULL){
		while(tmp != NULL){
			if(strcmp(tmp->stname, nameST) == 0){
				int i;
				for(i = 0; i < tmp->size; i++){
					if(strcmp(var, tmp->symbol[i].name) == 0){
						if(strcmp(tmp->symbol[i].type, "int") == 0)
							return 0;
						return 1;
					}
				}
			}
			tmp = tmp->next;
		}
	}
	else{
		while(tmp != NULL){
			if(strcmp(tmp->stname, nameST) == 0){
				int i;
				int j;
				int k;
				int cpt;
				char name_var[30];
				char type[4];

				for(i = 0; i < tmp->size; i++){
					if(strcmp(nameStruct, tmp->symbol[i].name) == 0){
						for(j = 0; j < tmp->symbol[i].struct_size; j++){
							for(k = 0; tmp->symbol[i].champs[j][k-1] != ' '; k++)
							cpt = 0;
							while(tmp->symbol[i].champs[j][k] != '\0'){
								name_var[cpt] = tmp->symbol[i].champs[j][k];
								k++;
								cpt++;
							}
							name_var[cpt] = '\0';
							if(strcmp(var, name_var) == 0){
								memset(name_var, 0, sizeof(name_var));
								k = 0;
								while(tmp->symbol[i].champs[j][k] != ' '){
									type[k] = tmp->symbol[i].champs[j][k];
									k++;
								}
								type[k] = '\0';
								if(strcmp(type, "int") == 0)
									return 0;
								return 1;
							}
						}
					}
				}
			}
			tmp = tmp->next;
		}
	}
	return -1;
}

int searchTypeOfFunc(ST st, char func_name[]){
	ST tmp = st;
	char name[30];
	while(tmp != NULL){
		memset(name, 0, sizeof(name));
		strcat(name, "int");
		strcat(name, " ");
		strcat(name, func_name);
		if(strcmp(tmp->stname, name) == 0)
			return 0;
		tmp = tmp->next;
	}
	return 1;
}

int searchFunction(ST st, char func_name[]){
	ST tmp = st;
	int i;
	int j;
	int k;
	char name[30];
	while(tmp != NULL){
		for(i=0; tmp->stname[i-1] != ' '; i++)
			continue;
		k=0;
		for(j=i; tmp->stname[j] != '\0'; j++){
			name[k] = tmp->stname[j];
			k++;
		}
		name[k] = '\0';
		if(strcmp(name, func_name) == 0)
			return 1;
		tmp = tmp->next;
		memset(name, 0, sizeof(name));
	}
	return 0;
}

int searchNumArg(ST st, char func_name[]){
	ST tmp = st;
	char name[30];
	int i;
	int k;
	while(tmp != NULL){
		for(i = 0; tmp->stname[i-1] != ' '; i++)
		k=0;
		for(i = i; tmp->stname[i] != '\0'; i++){
			name[k] = tmp->stname[i];
			k++;
		}
		name[k] = '\0';
		if(strcmp(name, func_name) == 0){
			return tmp->size;
		}
		tmp = tmp->next;
	}
	return -1;
}

int isNotaChamp(ST st, char func_name[], char var[]){
	ST tmp = st;
	int i;
	int j;
	while(tmp != NULL){
		if(strcmp(tmp->stname, func_name) == 0){
			for(i = 0; i < tmp->size; i++){
				for(j = 0; j < tmp->symbol[i].struct_size; j++){
					char name[20];
					int k, l;
					for(k = 0; tmp->symbol[i].champs[j][k-1] != ' '; k++)
					l = 0;
					while(tmp->symbol[i].champs[j][k] != '\0'){
						name[l] = tmp->symbol[i].champs[j][k];
						l++;
						k++;
					}
					name[l] = '\0';
					if(strcmp(name, var) == 0)
						return 1;
				}
			}
		}
		tmp = tmp->next;
	}
	return 0;
}

int is_Struct(ST st, char func_name[], char var_struct[]){
	ST tmp = st;
	int i;
	while(tmp != NULL){
		if(strcmp(tmp->stname, func_name) == 0){
			for(i = 0; i < tmp->size; i++){
				if(strcmp(tmp->symbol[i].name, var_struct) == 0 && strcmp(tmp->symbol[i].type, "Structure") == 0)
					return 1;
			}
		}
		tmp = tmp->next;
	}
	return 0;
}


void printSymbolTable(ST st){
	int i;
	int j;
	if(st == NULL)
		return;
	while(st != NULL){
		printf("Table : %s\n", st->stname);
		printf("Symbol(s) :\n");
		for(i = 0; i < st->size; i++)
			if(strcmp(st->symbol[i].type, "Structure")==0){
				printf("%s %s\n", st->symbol[i].type, st->symbol[i].name);
				for(j = 0; j < st->symbol[i].struct_size; j++){
					printf("\tChamp de %s : %s\n", st->symbol[i].name, st->symbol[i].champs[j]);
				}
			}
			else{
				printf("%s %s\n", st->symbol[i].type, st->symbol[i].name);
			}
			
		st = st->next;
		printf("\n");
	}
}

void addChamps(ST *st, char name[], char struct_name[], char champ_type[], char champ_id[]){
	if(searchSymbolTable(*st, name) == 0){
		printf("La table n'existe pas\n");
		return;
	}
	ST tmp = *st;
	while(strcmp(tmp->stname, name) != 0)
		tmp = tmp->next;
	for(int i = 0; i < (*st)->size; i++){
		if(strcmp(tmp->symbol[i].name, struct_name) == 0){
			strcpy(tmp->symbol[i].champs[tmp->symbol[i].struct_size], champ_type);
			strcat(tmp->symbol[i].champs[tmp->symbol[i].struct_size], " ");
			strcat(tmp->symbol[i].champs[tmp->symbol[i].struct_size], champ_id);
			tmp->symbol[i].struct_size += 1;
		}
	}
}

void addVars(ST *st, char name[], char* type, char* id){
	if(searchSymbolTable(*st, name) == 0){
		printf("La table n'existe pas\n");
		return;
	}
	ST tmp = *st;
	while(strcmp(tmp->stname, name) != 0)
		tmp = tmp->next;
	strcpy(tmp->symbol[tmp->size].name, id);
	strcpy(tmp->symbol[tmp->size].type, type);
	tmp->size += 1;
}



