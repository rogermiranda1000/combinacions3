#ifndef _OPERACIONES_H_
#define _OPERACIONES_H_

#include "combination.h"

#define OPERATIONS 2
#define SUMA '+'
#define MULTIPLICACION '*'
#define NON_COMMUTATIVE_OPERATIONS 3
#define RESTA '-'
#define DIVISION '/'
#define EXPONENT '^'
#define SINGLE_OPERATIONS 2
// RESTA
#define FACTORIAL '!'

#define NOT_DEFINED '?'

#define FACTORIAL_MAX 15
// base^exponent
#define BASE_MAX 2000
#define EXPONENT_MAX 12

char indexToOperation(int index);
Valor elevar(bool *valid, Valor b, Valor e);
void operar(Element *result, float e1, float e2, char op);
Valor factorial(bool *valid, Valor val);
void operarSelf(Element *result, float e1, char op);

#endif
