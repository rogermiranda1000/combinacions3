#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "combination.h"

#define VERBOSE 0

int main() {
    bool found;
    Element solution;
    Lenght x = 0;
    //for (int x = 0; x <= 5; x++) printf("Ciclo %d: %llu\n", x, getCycleLenght(x));

    x = 82500000;
    do {
        solution = getOperation(x);

        if (VERBOSE) {
            if (solution.operation != NULL && solution.is_valid) printf("Resultado del elemento %llu: %s   [%.2f]\n", x, solution.operation, solution.valor);
            else printf("Elemento %llu no válido.\n", x);
        }
        if (x % 250000 == 0) printf("[v] %llu\n", x);

        if (solution.is_valid) {
            found = (solution.valor == SOLUTION && (solution.numbers == N_NUMBER || !USE_NUMBER));
            if (found) printf("[*] FOUND: %s\n", solution.operation);
        }
        else found = false;

        if (solution.operation != NULL) free(solution.operation);

        x++;
    } while (!found);

    return 0;
}
