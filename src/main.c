#include "app.h"
#include <stdlib.h>
#include <stdio.h>

int main(void) {
    App app = {0};

    if (!app_run(&app)) {
        fprintf(stderr, "Errore fatale durante l'esecuzione dell'applicazione!\n");
        app_cleanup(&app);
        return EXIT_FAILURE;
    }

    app_cleanup(&app);
    return EXIT_SUCCESS;
}
