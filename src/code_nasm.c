#include "code_nasm.h"

void init_asm_file(FILE *file){

	fprintf(file, "section .text\n");

	/*pour les variables non-initalisees*/
	fprintf(file, "section .bss\n");
	/*pour les variables initalisees*/
	fprintf(file, "section .data\n\n");
	
	fprintf(file, "extern scanf\n");
	fprintf(file, "extern printf\n\n");

	fprintf(file, "global _start\n\n");
	fprintf(file, "_start:\n");
	fprintf(file, "\tpush rbp\n");
	MOV("rbp", "rsp");
	fprintf(file, "\n");
	

}

void end_asm_file(FILE *file){
	fprintf(file, "\n");
	MOV("rax", "0");
	fprintf(file, "\tpop rbp\n");
	fprintf(file, "\tret\n");
}


void parcours_asm(Node *t, FILE *file){
	
	Node* type;
	Node* id;
	Node* parametres;
	Node* VarDeclF;
	Node* tmp;
	
	if(t->kind == VariablesDecl){

		if(t->firstChild != NULL){
			
			type = t->firstChild;

			while(type != NULL){

				if(type->kind == Structure){

					tmp = type->firstChild;

					while(tmp != NULL){
						
						id = tmp->firstChild;

						while(id != NULL){

							char pre[150];

							snprintf(pre, sizeof(pre), "QWORD [%s.%s]", type->u.identifier, id->u.identifier);

							MOV( pre, "rax");
							id = id->nextSibling;
						}

						tmp = tmp->nextSibling;
					}
				}
				else{

					id = type->firstChild;

					while(id != NULL){

						MOV(" RAX", id->u.identifier);
						id = id->nextSibling;
						
					}
				}

				type = type->nextSibling;
			}
		}
	}

	if(t->kind == Function){  /*Ajout des variables de fonctions*/

		if(t->firstChild != NULL){
		
			VarDeclF = t->firstChild->nextSibling;
		
			parametres = t->firstChild->firstChild;
		
			if(strcmp(parametres->u.identifier, "None") != 0){
		
				parametres = parametres->firstChild->firstChild;
		
				while(parametres != NULL){
		
					int i;
					int j;
		
					for(i = 0; i < MAXNAME; i++){
		
						if(parametres->u.identifier[i] == ' '){
		
							i++;
							break;
		
						}
		
					}
		
		
					for(j = 0; j < MAXNAME; j++){
		
						if(parametres->u.identifier[i] == '\0')
		
							break;
		
						i++;
					}
		
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
				
							tmp = tmp->nextSibling;
						}
				
						VarDeclF = VarDeclF->nextSibling;
					}
				}
			}
		}
	}

	if(t->nextSibling != NULL)
		parcours_asm(t->nextSibling, file);

	if(t->firstChild != NULL)
		parcours_asm(t->firstChild, file);
}