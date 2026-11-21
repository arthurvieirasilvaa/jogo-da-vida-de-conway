#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// Função utilizada para liberar a memória das matrizes alocada dinamicamente:
void free_matrix(bool **matrix, int l) {
    // Verifica se a matriz foi alocada:
    if(matrix == NULL) {
        return;
    }

    for(int i = 0; i < l; i++) {
        free(matrix[i]);
    }
    free(matrix);
}

// Função utilizada para verificar a vizinhança da célula atual:
void check_neighbors(bool **world, bool **next_gen, int i, int j, int l, int c) {
    int num = 0;

    // Verifica as vizinhas da célula atual:
    for(int row = i-1; row <= i+1; row++) {
        for(int column = j-1; column <= j+1; column++) {
            // Verifica se a coordenada está dentro do mundo:
            if(row >= 0 && row <= l-1 && column >= 0 && column <= c-1) {
                // Verifica se a célula vizinha não é a célula atual:
                if(row != i || column != j) {
                    if(world[row][column]) {
                        num++;
                    }
                }
            }
        }
    }

    // Regras:
    if(world[i][j]) {
        next_gen[i][j] = num == 2 || num == 3;
    }

    else {
        next_gen[i][j] = num == 3;
    }
}

// Função utilizada para simular o Jogo da Vida de Conway:
void conways_game_of_life(bool ***world, bool ***next_gen, int l, int c, int n_gen) {
    int gen = 0;
    bool **temp;

    while (gen < n_gen) {
        for(int i = 0; i < l; i++) {
            for(int j = 0; j < c; j++) {
                check_neighbors(*world, *next_gen, i, j, l, c);
            }
        }
        // Atualiza o mundo:
        temp = *world;
        *world = *next_gen;
        *next_gen = temp;

        gen++;
    } 
}

// Função utilizada para imprimir a saída:
void print_output(int n_gen, int l, int c, bool **world) {
    printf("%d\n", n_gen);
    printf("%d %d\n", l, c);

    int n = 0;
    for(int i = 0; i < l; i++) {
        for(int j = 0; j < c; j++) {
            if(world[i][j]) {
                n++;
            }
        }
    }

    printf("%d\n", n);

    for(int i = 0; i < l; i++) {
        for(int j = 0; j < c; j++) {
            if(world[i][j]) {
                printf("%d %d\n", i+1, j+1);
            }
        }
    }
}

int main(int argc, char *argv[]) {
    // Verifica se os argumentos são válidos:
    if(argc < 2) {
        printf("Argumentos inválidos!\n");
        printf("Formato correto: ./jogo_da_vida_de_conway_sequencial <nome_do_arquivo_de_entrada>\n");
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
    if(fscanf(input, "%d", &n_gen) != 1 || fscanf(input, "%d %d", &l, &c) != 2 || fscanf(input, "%d", &n) != 1) {
        printf("Parâmetros da simulação inválidos!");
        fclose(input);
        exit(1);
    }

    bool **world = (bool**) malloc(l * sizeof(bool*));
    bool **next_gen = (bool**) malloc(l * sizeof(bool*));

    // Verifica se a memória foi alocada corretamente:
    if(world == NULL || next_gen == NULL) {
        printf("A memória não foi alocada corretamente!\n");
        exit(1);
    }

    for(int i = 0; i < l; i++) {
        world[i] = (bool*) malloc(c * sizeof(bool));
        next_gen[i] = (bool*) malloc(c * sizeof(bool));
   
        // Verifica se a memória foi alocada corretamente:
        if(world[i] == NULL || next_gen[i] == NULL) {
            printf("A memória não foi alocada corretamente!\n");
            free_matrix(world, i+1);
            free_matrix(next_gen, i+1);
            fclose(input);
            exit(1);
        }
    }

    // Inicializando as matrizes com valores false:
    for(int i = 0; i < l; i++) {
        for(int j = 0; j < c; j++) {
            world[i][j] = false;// Verifica se a memória foi alocada corretamente:
            next_gen[i][j] = false;
        }
    }

    for(int i = 0; i < n; i++) {
        // Verifica se as coordenadas são válidas:
        if(fscanf(input, "%d %d", &x, &y) != 2 || x < 1 || x > l || y < 1 || y > c) {
            printf("Coordenadas inválidas!\n");
            free_matrix(world, l);
            free_matrix(next_gen, l);
            fclose(input);
            exit(1);
        }
        world[x-1][y-1] = true;
    }
    
    conways_game_of_life(&world, &next_gen, l, c, n_gen);
    print_output(n_gen, l, c, world);

    free_matrix(world, l);
    free_matrix(next_gen, l);

    fclose(input);
}
