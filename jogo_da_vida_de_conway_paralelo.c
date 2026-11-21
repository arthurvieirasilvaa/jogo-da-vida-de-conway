#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>

// Struct utilizada como argumento para a função que as threads iniciam:
typedef struct ThreadArgs {
    int id;
    int n_gen;
    int l;
    int c;
    bool ***world;
    bool ***next_gen;
    int start;
    int end;
    pthread_barrier_t *barrier;
}ThreadArgs;

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
void check_neighbors(ThreadArgs thread_args, int i, int j) {
    int num = 0;
    
    // Verifica as vizinhas da célula atual:
    for(int row = i-1; row <= i+1; row++) {
        for(int column = j-1; column <= j+1; column++) {
            // Verifica se a coordenada está dentro do mundo:
            if(row >= 0 && row <= thread_args.l-1 && column >= 0 && column <= thread_args.c-1) {
                // Verifica se a célula vizinha não é a célula atual:
                if(row != i || column != j) {
                    if((*thread_args.world)[row][column]) {
                        num++;
                    }
                }
            }
        }
    }

    // Regras:
    if((*thread_args.world)[i][j]) {
        (*thread_args.next_gen)[i][j] = num == 2 || num == 3;
    }

    else {
        (*thread_args.next_gen)[i][j] = num == 3;
    }
}

// Função utilizada para simular o Jogo da Vida de Conway:
void *conways_game_of_life(void *args) {
    ThreadArgs *thread_args = (ThreadArgs*) args;
    int gen = 0;
    bool **temp;

    while (gen < thread_args->n_gen) {
        for(int i = thread_args->start; i < thread_args->end; i++) {
            for(int j = 0; j < thread_args->c; j++) {
                check_neighbors(*thread_args, i, j);
            }
        }
        // Mecanismo de sincronização (barreiras):
        pthread_barrier_wait(thread_args->barrier);

        // Apenas a thread 0 atualiza o mundo:
        if(thread_args->id == 0) {
            temp = *thread_args->world;
            *thread_args->world = *thread_args->next_gen;
            *thread_args->next_gen = temp;
        }

        // Mecanismo de sincronização (barreiras):
        pthread_barrier_wait(thread_args->barrier);

        gen++;
    }

    return NULL;
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
    if(argc < 3) {
        printf("Argumentos inválidos!\n");
        printf("Formato correto: ./jogo_da_vida_de_conway_sequencial num_threads <nome_do_arquivo_de_entrada>\n");
        exit(1);
    }

    int num_threads = atoi(argv[1]);
    char *input_name = argv[2];
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
            world[i][j] = false;
            next_gen[i][j] = false;
        }
    }

    for(int i = 0; i < n; i++) {
        // Verifica se as coordenadas são válidas:
        if(fscanf(input, "%d %d", &x, &y) != 2 || x < 1 || x > l || y < 1 || y > c) {
            printf("Coordenadas inválidas!\n");
            fclose(input);
            free_matrix(world, l);
            free_matrix(next_gen, l);
            exit(1);
        }
        world[x-1][y-1] = true;
    }

    pthread_t threads[num_threads];
    ThreadArgs thread_args[num_threads];

    pthread_barrier_t barrier;
    
    // Verifica se a barreira de sincronização foi inicializada corretamente:
    if(pthread_barrier_init(&barrier, NULL, num_threads) != 0) {
        printf("A barreira de sincronização não foi inicializada corretamente!\n");
        fclose(input);
        free_matrix(world, l);
        free_matrix(next_gen, l);
        exit(1);
    }
    
    int rows_by_thread = l / num_threads; 
    int rows;
    int remainder = l % num_threads;
    int start = 0;

    // Dividindo quais linhas cada thread vai trabalhar:
    for(int i = 0; i < num_threads; i++) {
        rows = rows_by_thread;

        // Adicionando o resto do trabalho utilizando round robin:
        if(i < remainder) {
            rows++;
        }

        // Inicializando o struct com os argumentos das threads:
        thread_args[i].id = i;
        thread_args[i].n_gen = n_gen;
        thread_args[i].l = l;
        thread_args[i].c = c;
        thread_args[i].world = &world;
        thread_args[i].next_gen = &next_gen;
        thread_args[i].start = start;
        thread_args[i].end = start + rows;
        thread_args[i].barrier = &barrier;

        start = thread_args[i].end;
    }

    for(int i = 0; i < num_threads; i++) {
        pthread_create(&threads[i], NULL, conways_game_of_life, (void *)&thread_args[i]);
    }

    for(int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }

    print_output(n_gen, l, c, world);

    // Verifica se a barreira de sincronização foi destruída corretamente: 
    if(pthread_barrier_destroy(&barrier) != 0) {
        printf("A barreira de sincronização não foi destruída corretamente!\n");
        fclose(input);
        free_matrix(world, l);
        free_matrix(next_gen, l);
        exit(1);
    }

    free_matrix(world, l);
    free_matrix(next_gen, l);

    fclose(input);
}
