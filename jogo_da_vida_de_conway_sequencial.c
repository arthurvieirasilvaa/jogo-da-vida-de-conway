#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    // Verifica se os argumentos são válidos:
    if(argc < 2) {
        printf("Argumentos inválidos!\n");
        printf("Formato correto: ./jogo_da_vida_de_conway_sequencial.c <nome_do_arquivo_de_entrada>");
        exit(1);
    }

    char *input_name = argv[1];

    FILE *input = fopen(input_name, "r");

    // Verifica se o arquivo de entrada foi aberto corretamente:
    if(input == NULL) {
        printf("O arquivo de entrada %s não foi aberto corretamente!\n", input_name);
        exit(1);
    }

    // Lendo os dados do arquivo de entrada:
    int n_gen, l, c, n, x, y;
    
    // Verifica se os parâmetros da simulação são válidos:
    if(fscanf(input, "%d", &n_gen) != 1 | fscanf(input, "%d %d", &l, &c) != 2 | fscanf(input, "%d", &n) != 1) {
        printf("Parâmetros da simulação inválidos!");
        fclose(input);
        exit(1);
    }

    printf("N_GEN = %d\n", n_gen);
    printf("L = %d e C = %d\n", l, c);
    printf("N = %d\n", n);

    for(int i = 0; i < n; i++) {

        // Verifica se as coordenadas são válidas:
        if(fscanf(input, "%d %d", &x, &y) != 2) {
            printf("Coordenadas inválidas!\n");
            exit(1);
            fclose(input);
        }
        printf("(%d, %d)\n", x, y);
    }

    fclose(input);
}