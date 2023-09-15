#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_INPUT_SIZE 1024

void execute_command(const char *command) {
    pid_t child_pid = fork();
    
    if (child_pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    } else if (child_pid == 0) {
        // Este é o processo filho
        execl("/bin/sh", "sh", "-c", command, NULL);
        // Isso só é chamado se execl falhar
        perror("execl"); 
        exit(EXIT_FAILURE);
    } else {
        // Este é o processo pai
        int status;
        waitpid(child_pid, &status, 0);
        
        if (WIFEXITED(status)) {
            int exit_status = WEXITSTATUS(status);
            printf("Comando '%s' executado com status de saída: %d\n", command, exit_status);
        } else {
            perror("wait");
            exit(EXIT_FAILURE);
        }
    }
}

int main() {
    char input[MAX_INPUT_SIZE];
    
    while (1) {
        printf("Digite um comando ou 'socorro' para exibir os comandos: ");
        if (fgets(input, sizeof(input), stdin) == NULL) {
            break;
        }
        
        if (strcmp(input, "socorro\n") == 0) {
            printf("Comandos disponíveis: listar, info processos, cd [diretório], mkdir [diretório], rm [arquivo/diretório], cat [arquivo], ifconfig\n");
        } else {
            // Remova o caractere de nova linha do comando
            input[strlen(input) - 1] = '\0';
            if (strcmp(input, "listar") == 0) {
                execute_command("ls"); // Se o comando for "listar", execute "ls"
            } else if (strcmp(input, "info processos") == 0) {
                execute_command("ps");// Se o comando for "info processos", execute "lps"
            } else {
                execute_command(input);
            }
        }
    }
    
    return 0;
}