#include "combination.h"
#include "operaciones.h"
#include "paralleling.h"

// retorna el número de elementos (en total) dado un ciclo
Lenght cycles[RECOMENDED_CYCLES];
// 'cycles' al ser una variable global hay que vigilar su acceso
Lenght getCycleLenght(int cycle) {
    Lenght l = 0, last, tmp;

    if (cycle == 0) l = 1;
    else if (cycle > 0) {
        tmp = cycle-1;
        if (tmp >= RECOMENDED_CYCLES || cycles[tmp] == 0) {
            //printf("[d] Cycle %d not found, generating...\n", cycle);
            last = getCycleLenght(tmp);
            // last * this * OPERATIONS + this*SINGLE_OPERATIONS + last = this*(last*OPERATIONS + SINGLE_OPERATIONS) + last
            l = (last - getCycleLenght(cycle - 2)) * (last * (OPERATIONS+NON_COMMUTATIVE_OPERATIONS*2) + SINGLE_OPERATIONS) + last;
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

void freeAndNull(char **p) {
    if (*p != NULL) {
        free(*p);
        *p = NULL;
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

    freeAndNull(comb1);
    freeAndNull(comb2);

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

    freeAndNull(comb1);

    return;
}

int getOperationNumber(bool *self, bool *inverse, Lenght element_number, Lenght all) {
    Lenght operation_number;

    *self = false;
    *inverse = false;

    if (element_number < all*OPERATIONS) operation_number = (Lenght)(element_number / all);
    else if (element_number < all*(OPERATIONS+NON_COMMUTATIVE_OPERATIONS*2)) {
        operation_number = (element_number - all*OPERATIONS) / all;
        if (operation_number % 2 != 0) *inverse = true;
        //operation_number /= 2;
        operation_number += OPERATIONS;
    }
    else {
        // element_number < all*(OPERATIONS+NON_COMMUTATIVE_OPERATIONS*2) + SINGLE_OPERATIONS
        *self = true;
        operation_number = element_number - (all*(OPERATIONS+NON_COMMUTATIVE_OPERATIONS*2));
        operation_number += OPERATIONS+NON_COMMUTATIVE_OPERATIONS*2;
    }

    return operation_number;
}

// dado un número, retorna la operación correspondiente
Element getOperation(Lenght element) {
    int current_cycle;
    Element result = getDefaultElement();
    Lenght n_index, first_element, element_number, second_element;
    int operation_number;
    Lenght all, tmp;

    Element e1, e2, aux;
    char operation;
    bool self, inverse;

    if (element>0) {
        current_cycle = getCycle(element);
        all = getCycleLenght(current_cycle - 1); // all previous elements
        //prev = element - all; // previous cycle elements
        //prev = all - getCycleLenght(current_cycle - 2); // previous cycle elements (just previous)
        n_index = element - all; // index of just this cycle
        tmp = all*(OPERATIONS+NON_COMMUTATIVE_OPERATIONS*2) + SINGLE_OPERATIONS; // number of operations for each previous elements
        first_element = n_index / tmp; // first element (0 as first cycle element; use '+ getCycleLenght(current_cycle - 2)' to get "global" position)
        element_number = n_index - first_element*tmp; // n (<n><op><j>) as first element

        operation_number = getOperationNumber(&self, &inverse, element_number, all);
        operation = indexToOperation(operation_number);

        // getting each element(s)
        if (current_cycle>1) {
            e1 = getOperation(getCycleLenght(current_cycle - 2) + first_element);
            if (!self) {
                second_element = element_number % all;
                //e2 = getOperation(element_number - operation_number * all);
                e2 = getOperation(second_element);
            }
        }
        else {
            e1 = getDefaultElement();
            if (!self) e2 = getDefaultElement();
        }

        if (self) {
            if (e1.is_valid) {
                operarSelf(&result, e1.valor, operation);

                // numbers
                result.numbers = e1.numbers;
                if (USE_NUMBER && result.numbers > N_NUMBER) result.is_valid = false;

                if (result.is_valid) mergeSelfOperations(&result.operation, &e1.operation, operation);
            }
            else result.is_valid = false;
        }
        else {
            if (e1.is_valid && e2.is_valid) {
                if (inverse) {
                    // equal combinations are already calculated on inverse=false
                    if (strcmp(e1.operation, e2.operation) == 0) result.is_valid = false;
                    else {
                        aux = e1;
                        e1 = e2;
                        e2 = aux;
                    }
                }

                if (result.is_valid) {
                    operar(&result, e1.valor, e2.valor, operation);

                    // numbers
                    result.numbers = e1.numbers + e2.numbers;
                    if (USE_NUMBER && result.numbers > N_NUMBER) result.is_valid = false;

                    if (result.is_valid) mergeOperations(&result.operation, &e1.operation, &e2.operation, operation);
                }
            }
            else result.is_valid = false;
        }

        if (!result.is_valid) {
            freeAndNull(&e1.operation);
            if (!self) freeAndNull(&e2.operation);
            notValidElement(&result);
        }
    }

    return result;
}

// in: solucion
// out: valid?
bool checkValid(Element solution) {
    bool found;

    if (solution.is_valid) {
        found = (solution.valor == SOLUTION && (solution.numbers == N_NUMBER || !USE_NUMBER));
        if (found) printf("[*] FOUND: %s\n", solution.operation);
        if (!STOP_ON_FOUND) found = false;
    } else found = false;

    return found;
}