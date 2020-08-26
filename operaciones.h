#ifndef _OPERACIONES_H_
#define _OPERACIONES_H_

#include "combination.h"

#define OPERATIONS 5 // + - * /
#define SINGLE_OPERATIONS 2 // ! -self
#define SUMA '+'
#define RESTA '-'
#define MULTIPLICACION '*'
#define DIVISION '/'
#define EXPONENT '^'
#define FACTORIAL '!'
#define NOT_DEFINED '?'
#define ERROR -1

char indexToOperation(int index);
void operar(Element *result, float e1, float e2, char op);
Valor factorial(float val);
void operarSelf(Element *result, float e1, char op);

#endif
