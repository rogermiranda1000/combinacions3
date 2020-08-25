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

// dado un número, retorna la operación correspondiente
Element getOperation(Lenght element) {
    int current_cycle;
    Element result = getDefaultElement();
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
                operarSelf(&result, e1.valor, operation);

                // numbers
                result.numbers = e1.numbers;
                if (USE_NUMBER && result.numbers > N_NUMBER) result.is_valid = false;

                if (result.is_valid) mergeSelfOperations(&result.operation, &e1.operation, operation);
                else {
                    freeAndNull(&e1.operation);
                    notValidElement(&result);
                }
            }
            else {
                freeAndNull(&e1.operation);
                notValidElement(&result);
            }
        }
        else {
            if (e1.is_valid && e2.is_valid) {
                operar(&result, e1.valor, e2.valor, operation);

                // numbers
                result.numbers = e1.numbers + e2.numbers;
                if (USE_NUMBER && result.numbers > N_NUMBER) result.is_valid = false;

                if (result.is_valid) mergeOperations(&result.operation, &e1.operation, &e2.operation, operation);
                else {
                    freeAndNull(&e1.operation);
                    freeAndNull(&e2.operation);
                    notValidElement(&result);
                }
            } else {
                freeAndNull(&e1.operation);
                freeAndNull(&e2.operation);
                notValidElement(&result);
            }
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