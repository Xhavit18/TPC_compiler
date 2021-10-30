#ifndef __code_nasm__
#define __code_nasm__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "abstract-tree.h"
#include "SymbolTable.h"

FILE *file;

#define MOV(r1, r2) fprintf(file, "\tmov %s, %s\n", r1, r2)
#define PUSH(r) fprintf(file, "\tpush %s\n", r)
#define MUL(r) fprintf(file, "\tmul %s\n", r)
#define DIV(r) fprintf(file, "\tdiv %s\n", r)
#define ADD(r1, r2) fprintf(file, "\tadd %s, %s\n", r1, r2)
#define SUB(r1, r2) fprintf(file, "\tsub %s, %s\n", r1, r2)
#define IDIV(r) fprintf(file, "\tidiv %s\n", r)
#define IMUL(r) fprintf(file, "\timul %s\n", r)
#define IMUL2(r1, r2) fprintf(file, "\timul %s, %s\n", r1, r2)

void init_asm_file(FILE *file);
void end_asm_file(FILE *file);
void parcours_asm(Node *t, FILE *file);

#endif