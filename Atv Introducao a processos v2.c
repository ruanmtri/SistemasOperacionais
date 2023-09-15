#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>

#define MAX_INPUT_SIZE 1024

char *amsg;
int contador = 0;

void handler(int signum) {
    write(1, amsg, strlen(amsg));
}

void handlerOut(int signum) {
    write(1, "Você apertou CTRL-Z ", 21);
    contador++;
    int aux = contador + '0';
    write(1, &aux, sizeof(int));
    write(1, " vezes\n", 8);
    if (contador >= 3) {
        exit(0);
    }
}

void printsinc(char *msg, int segundos) {
    amsg = malloc(strlen(msg) * sizeof(char));
    strcpy(amsg, msg);
    signal(SIGALRM, handler);
    signal(SIGTSTP, handlerOut);
    while (1) {
        alarm(segundos);
        pause();
    }
}

void execute_ls() {
    system("ls");
}

void execute_ps() {
    system("ps");
}

void execute_cd(char *dir) {
    if (chdir(dir) == 0) {
        printf("Diretório alterado para: %s\n", dir);
    } else {
        perror("cd");
    }
}

void execute_mkdir(char *dir) {
    if (mkdir(dir, 0777) == 0) {
        printf("Diretório criado: %s\n", dir);
    } else {
        perror("mkdir");
    }
}

void execute_rm(char *path) {
    if (remove(path) == 0) {
        printf("Arquivo/diretório removido: %s\n", path);
    } else {
        perror("rm");
    }
}

void execute_cat(char *file) {
    printf("Conteúdo do arquivo %s:\n", file);
    char command[MAX_INPUT_SIZE];
    snprintf(command, sizeof(command), "cat %s", file);
    system(command);
}

void execute_ifconfig() {
    printf("Informações do dispositivo de rede e IP:\n");
    system("ifconfig");
}

int main() {
    char input[MAX_INPUT_SIZE];
    
    while (1) {
        printf("Digite um comando ou 'socorro' para exibir os comandos: ");
        if (fgets(input, sizeof(input), stdin) == NULL) {
            break;
        }
        
        if (strcmp(input, "socorro\n") == 0) {
            printf("Comandos disponíveis: ls, ps, cd [diretório], mkdir [diretório], rm [arquivo/diretório], cat [arquivo], ifconfig\n");
        } else if (strcmp(input, "ls\n") == 0) {
            execute_ls();
        } else if (strcmp(input, "ps\n") == 0) {
            execute_ps();
        } else if (strncmp(input, "cd ", 3) == 0) {
            char *dir = input + 3;
            dir[strlen(dir) - 1] = '\0'; // Remover o caractere de nova linha
            execute_cd(dir);
        } else if (strncmp(input, "mkdir ", 6) == 0) {
            char *dir = input + 6;
            dir[strlen(dir) - 1] = '\0'; // Remover o caractere de nova linha
            execute_mkdir(dir);
        } else if (strncmp(input, "rm ", 3) == 0) {
            char *path = input + 3;
            path[strlen(path) - 1] = '\0'; // Remover o caractere de nova linha
            execute_rm(path);
        } else if (strncmp(input, "cat ", 4) == 0) {
            char *file = input + 4;
            file[strlen(file) - 1] = '\0'; // Remover o caractere de nova linha
            execute_cat(file);
        } else if (strcmp(input, "ifconfig\n") == 0) {
            execute_ifconfig();
        } else {
            printf("Comando desconhecido. Digite 'socorro' para ver os comandos disponíveis.\n");
        }
    }
    
    return 0;
}