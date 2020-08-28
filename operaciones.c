#include "operaciones.h"
#include <math.h>

char indexToOperation(int index) {
    char op;

    switch (index) {
        case 0:
            op = SUMA;
            break;
        case 1:
            op = MULTIPLICACION;
            break;
        case OPERATIONS:
        case OPERATIONS+1:
            op = RESTA;
            break;
        case OPERATIONS+2:
        case OPERATIONS+3:
            op = DIVISION;
            break;
        case OPERATIONS+4:
        case OPERATIONS+5:
            op = EXPONENT;
            break;
        case OPERATIONS + NON_COMMUTATIVE_OPERATIONS*2:
            op = RESTA;
            break;
        case OPERATIONS + NON_COMMUTATIVE_OPERATIONS*2 + 1:
            op = FACTORIAL;
            break;
        default:
            op = NOT_DEFINED;
    }

    return op;
}

Valor factorial(bool *valid, Valor val) {
    Valor result, last;

    if (val == 0) result = 1;
    else result = val;
    last = result;

    // pls don't burn my CPU
    if (result > FACTORIAL_MAX) *valid = false;
    else {
        for (Valor x = val - 1; x > 1.0f && *valid; x--) {
            result *= x;
            if (result / x != last) *valid = false;
            last = result;
        }
    }

    return result;
}

Valor elevar(bool *valid, Valor b, Valor e) {
    Valor retorno, last;

    if (e == 0) {
        if (b == 0) *valid = false; // indeterminacion
        else retorno = 1;
    }
    else retorno = b;
    last = retorno;

    if (ceilf(e) != e) *valid = false;
    else {
        // exponente negativo?
        if (e < 0) {
            if (b == 0) *valid = false;
            else {
                e *= -1;
                b = 1/b;
            }
        }

        if (b == 1); // retorno = 1; but it's already done
        // pls don't burn my CPU
        else if (e > EXPONENT_MAX || b > BASE_MAX) *valid = false;
        else {
            for (Valor x = 2; x <= e && *valid; x++) {
                retorno *= b;
                if (retorno / b != last) *valid = false;
                last = retorno;
            }
        }
    }

    return retorno;
}

void operar(Element *result, Valor e1, Valor e2, char op) {
    switch (op) {
        case RESTA:
            e2 *= -1;
        case SUMA:
            result->valor = e1 + e2;
            if ((e1 > 0 && e2 > 0 && result->valor < 0) || (e1 < 0 && e2 < 0 && result->valor > 0)) result->is_valid = false; // overflow?
            break;
        case DIVISION:
            if (e2 == 0) result->is_valid = false;
            else e2 = 1/e2;
        case MULTIPLICACION:
            if (result->is_valid) {
                result->valor = e1 * e2;
                if (result->valor / e1 != e2) result->is_valid = false;
            }
            break;
        case EXPONENT:
            result->valor = elevar(&result->is_valid, e1, e2);
            //result->valor = (Valor)pow((double) e1, (double) e2);
            break;
        default:
            result->is_valid = false;
    }

    return;
}

void operarSelf(Element *result, Valor e1, char op) {
    switch (op) {
        case RESTA:
            result->valor = -e1;
            break;
        case FACTORIAL:
            if (ceilf(e1) != e1 || e1 < 0) result->is_valid = false;
            else result->valor = factorial(&result->is_valid, e1);
            break;
        default:
            result->is_valid = false;
    }

    return;
}