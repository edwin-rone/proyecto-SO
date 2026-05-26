#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Uso: %s <container_id>\n", argv[0]);
        return 1;
    }

    char *container_id = argv[1];

    printf("Ejecutando comando seguro sin shell...\n");
    
    // Parche: Arreglo de argumentos tratado como texto literal, no como comandos
    char *args[] = {"echo", "Inspeccionando Docker ID (Seguro):", container_id, NULL};
    execvp(args[0], args);

    return 0;
}