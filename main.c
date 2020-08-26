#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "combination.h"
#include "paralleling.h"
#include <time.h>

#define VERBOSE 0
#define INIT_NUMBER 0 // starting number (~7987000000 sin exponente; ~X con exponente)

int main() {
    bool found;
    Element solution;
    Lenght x = INIT_NUMBER;
    clock_t begin = clock();

    // pre-generate cycles (do it if multithreading)
    for (int cycle = 0; cycle <= RECOMENDED_CYCLES; cycle++) printf("Ciclo %d: %llu\n", cycle, getCycleLenght(cycle));

    // calculations
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

    // time
    double tiempo = (double)(clock() - begin) / CLOCKS_PER_SEC;
    if (tiempo < 120) printf("[*] Tiempo: %fs\n", tiempo);
    else if (tiempo < 3600) printf("[*] Tiempo: %fmin", tiempo/60.0f);
    else printf("[*] Tiempo: %fh", tiempo/3600.0f);

    return 0;
}
