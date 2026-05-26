#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Uso: %s <container_id>\n", argv[0]);
        return 1;
    }

    char *container_id = argv[1];
    char cmd[512];

    // Simulacion de la vulnerabilidad de ABRT: construccion de comando inseguro
    snprintf(cmd, sizeof(cmd), "echo 'Inspeccionando Docker ID:' && echo %s", container_id);
    printf("Ejecutando en shell: %s\n", cmd);
    
    // Ejecucion insegura equivalente a libreport_run_in_shell_and_save_output
    system(cmd);

    return 0;
}