#include "operaciones.h"
#include <math.h>

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

void operar(Element *result, float e1, float e2, char op) {
    switch (op) {
        case RESTA:
            e2 *= -1;
        case SUMA:
            result->valor = e1 + e2;
            if ((e1 > 0 && e2 > 0 && result->valor < 0) || (e1 < 0 && e2 < 0 && result->valor > 0)) result->is_valid = false; // overflow?
            break;
        case MULTIPLICACION:
            result->valor = e1 * e2;
            break;
        case DIVISION:
            if (e2 == 0) result->is_valid = false;
            else result->valor = e1 / e2;

            break;
        default:
            result->is_valid = false;
    }

    return;
}

void operarSelf(Element *result, float e1, char op) {
    switch (op) {
        case RESTA:
            result->valor = -e1;
            break;
        case FACTORIAL:
            if (ceilf(e1) != e1 || e1 < 0) result->is_valid = false;
            else {
                result->valor = factorial(e1);
                if (result->valor == ERROR) result->is_valid = false;
            }
            break;
        default:
            result->is_valid = false;
    }

    return;
}