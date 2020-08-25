#include "paralleling.h"

void *paralelFunction(void *input) {
    Lenght init = *(Lenght*)input;
    Element *soluciones = NULL;

    soluciones = (Element*) malloc(sizeof(Element) * OPERATIONS_PER_THREAD);
    if (soluciones != NULL) {
        for (Lenght x = 0; x < OPERATIONS_PER_THREAD; x++) {
            soluciones[x] = getOperation(init + x);
        }
    }

    return (void*)soluciones;
}

bool paralel(Lenght *current) {
    int thread;
    void *p;
    Lenght x;
    bool found = false;

    pthread_t threads[THREADS];
    Lenght input[THREADS];
    Element *retorno = NULL;

    // create all threads
    for (thread = 0; thread < THREADS; thread++) {
        // init input
        input[thread] = *current;
        *current += OPERATIONS_PER_THREAD;

        if(pthread_create(&threads[thread], NULL, &paralelFunction, &input[thread])) {
            printf("[e] On thread create\n");
            exit(EXIT_FAILURE);
        }
    }

    // merge
    for (thread = 0; thread < THREADS; thread++) {
        if (pthread_join(threads[thread], &p)) {
            printf("[e] On thread join\n");
            exit(EXIT_FAILURE);
        }

        retorno = (Element*) p;
        if (retorno == NULL) exit(EXIT_FAILURE);

        for (x = 0; x < OPERATIONS_PER_THREAD; x++) {
            if (!found) found = checkValid(retorno[x]);
            // si ya se ha encontrado solo mira (pero no alteres el valor)
            else checkValid(retorno[x]);

            if (retorno[x].operation == NULL) exit(EXIT_FAILURE);
            free(retorno[x].operation);
        }
        free(retorno);
        retorno = NULL;
    }

    return found;
}