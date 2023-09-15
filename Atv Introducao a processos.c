#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h> 

#define MAX_INPUT_SIZE 1024
#define MAX_TOKENS 100

// Função para dividir a entrada em tokens
int tokenize_input(char *input, char **tokens) {
    int token_count = 0;
    char *token = strtok(input, " \t\n");
    
    while (token != NULL) {
        tokens[token_count++] = token;
        token = strtok(NULL, " \t\n");
    }
    
    return token_count;
}

// Função para o comando "ls"
void execute_ls() {
    printf("Listando arquivos do diretório:\n");
    system("ls");
}

// Função para o comando "ps"
void execute_ps() {
    printf("Informações sobre processos:\n");
    system("ps aux");
}

// Função para o comando "cd"
void execute_cd(char *dir) {
    if (chdir(dir) == 0) {
        printf("Diretório alterado para: %s\n", dir);
    } else {
        perror("cd");
    }
}

// Função para o comando "mkdir"
void execute_mkdir(char *dir) {
    if (mkdir(dir, 0777) == 0) {
        printf("Diretório criado: %s\n", dir);
    } else {
        perror("mkdir");
    }
}

// Função para o comando "rm"
void execute_rm(char *path) {
    if (remove(path) == 0) {
        printf("Arquivo/diretório removido: %s\n", path);
    } else {
        perror("rm");
    }
}

// Função para o comando "cat"
void execute_cat(char *file) {
    printf("Conteúdo do arquivo %s:\n", file);
    char command[MAX_INPUT_SIZE];
    snprintf(command, sizeof(command), "cat %s", file);
    system(command);
}

// Função para o comando "ifconfig"
void execute_ifconfig() {
    printf("Informações do dispositivo de rede e IP:\n");
    system("ifconfig");
}

int main() {
    char input[MAX_INPUT_SIZE];
    char *tokens[MAX_TOKENS];
    
    while (1) {
        printf("Digite um comando ou 'socorro' para exibir os comandos: ");
        if (fgets(input, sizeof(input), stdin) == NULL) {
            break;
        }
        
        int token_count = tokenize_input(input, tokens);
        if (token_count <= 0) {
            continue;
        }
        
        if (strcmp(tokens[0], "socorro") == 0) {
            printf("Comandos disponíveis: ls, ps, cd [diretório], mkdir [diretório], rm [arquivo/diretório], cat [arquivo], ifconfig\n");
        } else if (strcmp(tokens[0], "ls") == 0) {
            execute_ls();
        } else if (strcmp(tokens[0], "ps") == 0) {
            execute_ps();
        } else if (strcmp(tokens[0], "cd") == 0 && token_count == 2) {
            execute_cd(tokens[1]);
        } else if (strcmp(tokens[0], "mkdir") == 0 && token_count == 2) {
            execute_mkdir(tokens[1]);
        } else if (strcmp(tokens[0], "rm") == 0 && token_count == 2) {
            execute_rm(tokens[1]);
        } else if (strcmp(tokens[0], "cat") == 0 && token_count == 2) {
            execute_cat(tokens[1]);
        } else if (strcmp(tokens[0], "ifconfig") == 0) {
            execute_ifconfig();
        } else {
            printf("Comando desconhecido. Digite 'socorro' para ver os comandos disponíveis.\n");
        }
    }
    
    return 0;
}