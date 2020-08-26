#ifndef _COMBINATION_H_
#define _COMBINATION_H_

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>

#define RECOMENDED_CYCLES 6

#define NUMBER 3
#define STR_NUMBER "3"
#define LEN_NUMBER 1 // NUMBER's characters

#define SOLUTION 2019
#define USE_NUMBER 1
#define N_NUMBER 7 // using N_NUMBER number of NUMBER, get SOLUTION
#define STOP_ON_FOUND 1

typedef float Valor;
typedef unsigned long long Lenght;
typedef struct {
    bool is_valid; // indica si la operación es o no es válida (ej: 5/0; is_valid=false)
    Valor valor; // valor actual de la operación
    char *operation; // equación
    int numbers;
} Element;

Lenght getCycleLenght(int cycle);
int getCycle(Lenght element);
Element getDefaultElement();
void notValidElement(Element *r);
void mergeOperations(char **to, char **comb1, char **comb2, char op);
Element getOperation(Lenght element);
void freeAndNull(char **p);
bool checkValid(Element solution);

#endif
