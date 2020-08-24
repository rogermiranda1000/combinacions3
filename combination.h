#ifndef _COMBINATION_H_
#define _COMBINATION_H_

#include <stdbool.h>

#define OPERATIONS 4 // + - * /
#define SINGLE_OPERATIONS 2 // ! -self
#define SUMA '+'
#define RESTA '-'
#define MULTIPLICACION '*'
#define DIVISION '/'
#define FACTORIAL '!'
#define NOT_DEFINED '?'
#define ERROR -1

#define RECOMENDED_CYCLES 7

#define NUMBER 3
#define STR_NUMBER "3"
#define LEN_NUMBER 1 // NUMBER's characters

#define SOLUTION 2019
#define USE_NUMBER 1
#define N_NUMBER 7 // using N_NUMBER number of NUMBER, get SOLUTION

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
Element operar(float e1, float e2, char op);
char indexToOperation(int index);
Element getDefaultElement();
void notValidElement(Element *r);
void mergeOperations(char **to, char **comb1, char **comb2, char op);
Element getOperation(Lenght element);
void removeLast(char **a, char **b);

#endif
