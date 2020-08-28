#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "combination.h"
#include "paralleling.h"
#include <time.h>
#include <unistd.h> // sleep

#define DELAY 0
#define SAVE 0

void saveCurrent(Lenght c) {
    FILE *f = NULL;

    f = fopen("last.txt", "w");

    if (f != NULL) {
        fprintf(f, "%llu", c);

        fclose(f);
    }

    return;
}

int main() {
    bool found;
    Element solution;
    Lenght x = INIT_NUMBER;
    clock_t begin = clock();

    // pre-generate cycles (do it if multithreading)
    for (int cycle = 0; cycle <= RECOMENDED_CYCLES; cycle++) printf("Ciclo %d: %llu\n", cycle, getCycleLenght(cycle));

    // finding 2019:
    //printf("[*] %llu\n", getCycleLenght(getCycle(425139)) + (425139-getCycleLenght(getCycle(425139)-1))*(getCycleLenght(getCycle(425139)))*(2+3*2)+3);

    // calculations
    do {
        #if (THREADS > 1)
            // paralleling
            found = paralel(&x);
            printf("[v] %llu\n", x);
            #if SAVE
                saveCurrent(x);
            #endif
        #else
            solution = getOperation(x);

            #if (VERBOSE)
                if (DELAY) sleep(1);
                if (solution.operation != NULL && solution.is_valid) printf("Resultado del elemento %llu: %s   [%.2f]\n", x, solution.operation, solution.valor);
                //else printf("Elemento %llu no válido.\n", x);
            #endif
            if (x % 250000 == 0) {
                printf("[v] %llu\n", x);
                #if SAVE
                    saveCurrent(x);
                #endif
            }

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
