#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "combination.h"
#include "paralleling.h"
#include <time.h>

#define VERBOSE 0
#define INIT_NUMBER 0 // starting number (~7987000000)

int main() {
    bool found;
    Element solution;
    Lenght x = INIT_NUMBER;
    clock_t begin = clock();

    for (int cycle = 0; cycle <= RECOMENDED_CYCLES; cycle++) printf("Ciclo %d: %llu\n", cycle, getCycleLenght(cycle));

    do {
        #if (THREADS > 1)
            // paralleling
            found = paralel(&x);
            printf("[v] %llu\n", x);
        #else
            solution = getOperation(x);

            #if (VERBOSE)
                if (solution.operation != NULL && solution.is_valid) printf("Resultado del elemento %llu: %s   [%.2f]\n", x, solution.operation, solution.valor);
                else printf("Elemento %llu no válido.\n", x);
            #endif
            if (x % 250000 == 0) printf("[v] %llu\n", x);

            found = checkValid(solution);

            if (solution.operation != NULL) {
                free(solution.operation);
                solution.operation = NULL;
            }

            x++;
        #endif
    } while (!found);

    printf("[*] Tiempo: %fs\n", (double)(clock() - begin) / CLOCKS_PER_SEC);

    return 0;
}
