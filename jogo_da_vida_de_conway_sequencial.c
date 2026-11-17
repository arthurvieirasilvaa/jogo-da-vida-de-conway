#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

void print_world(bool **world, int l, int c) {
    for(int i = 0; i < l; i++) {
        for(int j = 0; j < c; j++) {
            printf("%d ", world[i][j]);
        }
        printf("\n");
    }
}

void update_world(bool **world, bool **next_gen, int l, int c) {
    for(int i = 0; i < l; i++) {
        for(int j = 0; j < c; j++) {
            world[i][j] = next_gen[i][j];
        }
    }
}

void check_neighbors(bool **world, bool **next_gen, int i, int j, int l, int c) {
    int num = 0;

    // Verifica se a célula atual está na primeira linha:
    if(i == 0) {
        if(world[i+1][j]) {
            num++;
        }

        // Verifica se a célula atual está na primeira coluna:
        if(j == 0) {    
            if (world[i][j+1]) {
                num++;
            }

            if (world[i+1][j+1]) {
                num++;
            }
        }   

        // Verifica se a célula atual está na última coluna:
        if (j == c-1) {    
            if(world[i][j-1]) {
                num++;
            }

            if (world[i+1][j-1]) {
                num++;
            }
        }
        
        else {
            if (world[i][j+1]) {
                num++;
            }
            
            if(world[i][j-1]) {
                num++;
            }

            if (world[i+1][j+1]) {
                num++;
            }

            if (world[i+1][j-1]) {
                num++;
            }
        }
    }

    // Verifica se a célula atual está na primeira coluna:   
    else if(j == 0) {
        if (world[i][j+1]) {
            num++;
        }
        
        // Verifica se a célula atual está na última linha:
        if (i == l-1) {
            if(world[i-1][j]) {
                num++;
            }

            if(world[i-1][j+1]) {
                num++;
            }
        }   

        else {
            if(world[i+1][j]) {
                num++;
            }

            if(world[i-1][j]) {
                num++;
            }

            if(world[i+1][j+1]) {
                num++;
            }

            if(world[i-1][j+1]) {
                num++;
            }
        }
    }

    // Verifica se a célula atual está na última linha:
    else if(i == l-1) {
        if(world[i-1][j]) {
            num++;
        }

        // Verifica se a célula atual está na última coluna:
        if(j == c-1) {
            if(world[i][j-1]) {
                num++;
            }

            if(world[i][j-1]) {
                num++;
            }

            if(world[i-1][j-1]) {
                num++;
            }
        }

        else {
            if (world[i][j+1]) {
                num++;
            }

            if(world[i][j-1]) {
                num++;
            }

            if(world[i-1][j+1]) {
                num++;
            }

            if(world[i-1][j-1]) {
                num++;
            }
        }
    }

    // Verifica se a célula atual está na última coluna:
    else if(j == c-1) {
        if(world[i][j-1]) {
            num++;
        }

        if(world[i+1][j]) {
            num++;
        }

        if(world[i-1][j]) {
            num++;
        }

        if(world[i+1][j-1]) {
            num++;
        }

        if(world[i-1][j-1]) {
            num++;
        }
    }

    else {
        if(world[i+1][j]) {
            num++;
        }

        if(world[i-1][j]) {
            num++;
        }

        if (world[i][j+1]) {
            num++;
        }

        if(world[i][j-1]) {
            num++;
        }

        if(world[i+1][j+1]) {
            num++;
        }

        if (world[i+1][j-1]) {
            num++;
        }

        if(world[i-1][j+1]) {
            num++;
        }

        if(world[i-1][j-1]) {
            num++;
        }
    }

    // Verificando as regras:

    if (world[i][j]){
        next_gen[i][j] = num == 2 || num == 3;
    }

    else {
        next_gen[i][j] = num == 3;
    }
}

void conways_game_of_life(bool **world, bool **next_gen, int l, int c, int n_gen) {
    int gen = 0;

    while (gen < n_gen) {
        for(int i = 0; i < l; i++) {
            for(int j = 0; j < c; j++) {
                check_neighbors(world, next_gen, i, j, l, c);
            }
        }
        update_world(world, next_gen, l, c);

        printf("GEN = %d:\n", gen+1);
        print_world(world, l, c);
        printf("============================================\n");

        gen++;
    } 
}

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
    }

    // Inicializando as matrizes com valores false:
    for(int i = 0; i < l; i++) {
        for(int j = 0; j < c; j++) {
            world[i][j] = false;
            next_gen[i][j] = false;
        }
    }

    for(int i = 0; i < n; i++) {

        // Verifica se as coordenadas são válidas:
        if(fscanf(input, "%d %d", &x, &y) != 2) {
            printf("Coordenadas inválidas!\n");
            exit(1);
            fclose(input);
        }
        printf("(%d, %d)\n", x, y);
        
        world[x-1][y-1] = true;
    }
    
    printf("Mundo inicial:\n");
    print_world(world, l, c);
    printf("============================================\n");

    conways_game_of_life(world, next_gen, l, c, n_gen);

    for(int i = 0; i < l; i++) {
        free(world[i]);
        free(next_gen[i]);
    }
    free(world);
    free(next_gen);

    fclose(input);
}