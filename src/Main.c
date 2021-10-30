#include "code_nasm.h"
#include "parser.h"

static void display_help(){
	printf("--------------------------------------------------------------------------------\n");
	printf("Help required ? Do not worry, we got you !\n");
	printf("This is a compiler for the TPC language. In order to use it, please check that your file is in TPC format\n");
	printf("To use this compiler, go to the correct folder. From there, you have the choice :\n");
	printf("1-You can launch a script which will be executed on all the files of the different folders provided for this purpose. You just have to execute the command 'bash script.sh'\n");
	printf("2-You can manually use the compiler on the file of your choice, as well as choose whether you want to diplay your program's abstract tree and its symbol table.\nTo do this, run the make command, then run the tpcc executable (located in 'bin' directory) with the -t or --tree options to diplay the tree, -s or --symtabs to diplay the table, followed by your file name\n");
	printf("--------------------------------------------------------------------------------\n");
}

int main(int argc, char *argv[]){
	int i;

	if(yyparse() == 1)
		return 1;

	ST st = NULL;
	char file_name[] =  "_anonymous.asm";

	parcours(t, &st);

	if(parcoursSemantique(t, st) == 2)
		return 2;

	for(i = 1; i < argc; i++){
		if(strcmp(argv[i], "-t") == 0 || strcmp(argv[i], "--tree") == 0)
			printTree(t);
		else if(strcmp(argv[i], "-s") == 0 || strcmp(argv[i], "--symtabs") == 0)
			printSymbolTable(st);
		else if(strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0){
			display_help();
			return 0;
		}
		else if( strcmp(argv[i] + strlen(argv[i]) - 4, ".tpc") == 0 ) {
			strcpy(file_name, argv[i]);
			file_name[strlen(file_name) - 3] = 'a';
			file_name[strlen(file_name) - 2] = 's';
			file_name[strlen(file_name) - 1] = 'm';

		}
		else{
			printf("Wrong argument '%s'\n", argv[i]);
			return 3;
		}
	}

	if ((file = fopen( file_name, "w" )) == NULL){
		perror("fopen ");
		exit(1);
  	}

	init_asm_file(file);
	parcours_asm(t, file);
	end_asm_file(file);

	free(t);
	free(st);

	return 0;
}