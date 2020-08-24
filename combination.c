#include "combination.h"
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include <math.h>

// retorna el número de elementos (en total) dado un ciclo
Lenght cycles[RECOMENDED_CYCLES];
Lenght getCycleLenght(int cycle) {
    Lenght l = 0, last, tmp;

    if (cycle == 0) l = 1;
    else if (cycle > 0) {
        tmp = cycle-1;
        if (tmp >= RECOMENDED_CYCLES || cycles[tmp] == 0) {
            printf("[d] Cycle %d not found, generating...\n", cycle);
            last = getCycleLenght(tmp);
            // last * this * OPERATIONS + this*SINGLE_OPERATIONS + last = this*(last*OPERATIONS + SINGLE_OPERATIONS) + last
            l = (last - getCycleLenght(cycle - 2)) * (last * OPERATIONS + SINGLE_OPERATIONS) + last;
            if (tmp < RECOMENDED_CYCLES) cycles[tmp] = l;
        }
        else l = cycles[cycle-1];
    }

    return l;
}

// dado un número, retorna el ciclo al cual pertenece
int getCycle(Lenght element) {
    int cycle = 0;

    while (!(element >= getCycleLenght(cycle) && element < getCycleLenght(cycle+1))) cycle++;

    cycle++;

    return cycle;
}

char indexToOperation(int index) {
    char op;

    switch (index) {
        case 0:
            op = SUMA;
            break;
        case 1:
            op = RESTA;
            break;
        case 2:
            op = MULTIPLICACION;
            break;
        case 3:
            op = DIVISION;
            break;
        case 4:
            op = FACTORIAL;
            break;
        case 5:
            op = RESTA;
            break;
        default:
            op = NOT_DEFINED;
    }

    return op;
}

Element operar(float e1, float e2, char op) {
    Element result = getDefaultElement();

    switch (op) {
        case RESTA:
            e2 *= -1;
        case SUMA:
            result.valor = e1 + e2;
            if ((e1 > 0 && e2 > 0 && result.valor < 0) || (e1 < 0 && e2 < 0 && result.valor > 0)) result.is_valid = false; // overflow?
            break;
        case MULTIPLICACION:
            result.valor = e1 * e2;
            break;
        case DIVISION:
            if (e2 == 0) result.is_valid = false;
            else result.valor = e1 / e2;

            break;
        default:
            result.is_valid = false;
    }

    return result;
}

Valor factorial(float val) {
    Valor result = 1.0f;
    Valor last = result;

    for (Valor x = 2.0f; x <= val && result != ERROR; x++) {
        result *= x;
        if (result/x != last) result = ERROR;
        last = result;
    }

    return result;
}

Element operarSelf(float e1, char op) {
    Element result = getDefaultElement();

    switch (op) {
        case RESTA:
            result.valor = -e1;
            break;
        case FACTORIAL:
            if (ceilf(e1) != e1 || e1 < 0) result.is_valid = false;
            else {
                result.valor = factorial(e1);
                if (result.valor == ERROR) result.is_valid = false;
            }
            break;
        default:
            result.is_valid = false;
    }

    return result;
}

Element getDefaultElement() {
    Element element;

    element.is_valid = true;
    element.valor = NUMBER;
    element.numbers = 1;

    element.operation = NULL;
    element.operation = (char*) malloc(sizeof(char) * (LEN_NUMBER + 1));
    if (element.operation != NULL) strcpy(element.operation, STR_NUMBER);

    return element;
}

void notValidElement(Element *r) {
    r->is_valid = false;

    // text
    if (r->operation != NULL) {
        free(r->operation);
        r->operation = NULL;
    }

    r->operation = (char*) malloc(sizeof(char) * 4); // "NaN\0"
    if (r->operation != NULL) strcpy(r->operation, "NaN");
    else exit(EXIT_FAILURE);

    return;
}

void removeLast(char **a, char **b) {
    if (*a != NULL) {
        free(*a);
        *a = NULL;
    }

    if (*b != NULL) {
        free(*b);
        *b = NULL;
    }
}

void mergeOperations(char **to, char **comb1, char **comb2, char op) {
    unsigned int x;

    if (*to != NULL) {
        free(*to);
        *to = NULL;
    }

    x = strlen(*comb1);
    *to = (char*) malloc(sizeof(char) * (x + strlen(*comb2) + 6)); // size of first comb; size of second comb; operation (1) + parenthesis (4) + \0 (1)

    if (*to != NULL) {
        strcpy(*to, "(");
        strcat(*to, *comb1);

        x++;
        (*to)[x] = ')';
        (*to)[x+1] = op;
        (*to)[x+2] = '(';
        (*to)[x+3] = '\0';

        strcat(*to, *comb2);
        strcat(*to, ")");
    }
    else exit(EXIT_FAILURE);

    removeLast(comb1, comb2);

    return;
}

void mergeSelfOperations(char **to, char **comb1, char op) {
    unsigned int x;

    if (*to != NULL) {
        free(*to);
        *to = NULL;
    }

    x = strlen(*comb1);
    *to = (char*) malloc(sizeof(char) * (x + 4)); // size of first comb; size of second comb; operation (1) + parenthesis (2) + \0 (1)

    if (*to != NULL) {
        if (op == FACTORIAL) {
            strcpy(*to, "(");
            strcat(*to, *comb1);
            x++;
            (*to)[x] = ')';
            (*to)[x+1] = op;
            (*to)[x+2] = '\0';
        }
        else if (op == RESTA) {
            (*to)[0] = op;
            (*to)[1] = '(';
            (*to)[2] = '\0';
            strcat(*to, *comb1);
            strcat(*to, ")");
        }
        else exit(EXIT_FAILURE);
    }
    else exit(EXIT_FAILURE);

    if (*comb1 != NULL) {
        free(*comb1);
        *comb1 = NULL;
    }

    return;
}

// dado un número, retorna la operación correspondiente
Element getOperation(Lenght element) {
    int current_cycle;
    Element result;
    Lenght cycle_number, first_element, element_number, operation_number;
    Lenght all, tmp;

    Element e1, e2;
    char operation;

    if (element>0) {
        current_cycle = getCycle(element);
        all = getCycleLenght(current_cycle - 1);
        cycle_number = element - all; // actual cycle number
        tmp = all*OPERATIONS + SINGLE_OPERATIONS;
        first_element = (Lenght)(cycle_number / tmp); // index of the first element
        element_number = cycle_number - first_element*tmp; // idk what to say, just see it, it's more easy that explain it
        if (element_number < all*OPERATIONS) operation_number = (Lenght)(element_number / all);
        else operation_number = (element_number - all*OPERATIONS) + OPERATIONS;

        operation = indexToOperation(operation_number);
        if (current_cycle>1) {
            e1 = getOperation(getCycleLenght(current_cycle - 2) + first_element);
            if (operation_number < OPERATIONS) e2 = getOperation(element_number - operation_number * all);
        }
        else {
            e1 = getDefaultElement();
            if (operation_number < OPERATIONS) e2 = getDefaultElement();
        }

        if (operation_number >= OPERATIONS) {
            if (e1.is_valid) {
                result = operarSelf(e1.valor, operation);

                // numbers
                result.numbers = e1.numbers;
                if (USE_NUMBER && result.numbers > N_NUMBER) result.is_valid = false;

                if (result.is_valid) mergeSelfOperations(&result.operation, &e1.operation, operation);
                else notValidElement(&result);
            }
            else {
                // remove e1
                if (e1.operation != NULL) {
                    free(e1.operation);
                    e1.operation = NULL;
                }
                notValidElement(&result);
            }
        }
        else {
            if (e1.is_valid && e2.is_valid) {
                result = operar(e1.valor, e2.valor, operation);

                // numbers
                result.numbers = e1.numbers + e2.numbers;
                if (USE_NUMBER && result.numbers > N_NUMBER) result.is_valid = false;

                if (result.is_valid) mergeOperations(&result.operation, &e1.operation, &e2.operation, operation);
                else notValidElement(&result);
            } else {
                removeLast(&e1.operation, &e2.operation);
                notValidElement(&result);
            }
        }
    }
    else result = getDefaultElement();

    return result;
}