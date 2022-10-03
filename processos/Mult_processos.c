#include <stdio.h>
#include <stdlib.h> // necessário p/ as funções rand() e srand()
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h> //necessário p/ função time()
#include <unistd.h>

int main(int argc, char *argv[]) {
  // argv[1] = M1.txt | argv[2] = M2.txt | argv[3] = p processos

  FILE *file;

  // lendo M1
  file = fopen(argv[1], "r");
  if (file == NULL) {
    printf("Problemas na abertura do arquivo\n");
    exit(-1);
  }

  int m1d[2]; // m1d [0] = linha | m1d [1] = coluna
  for (int i = 0; i < 2; i++) {
    m1d[i] = 0;
    fscanf(file, "%d", &m1d[i]);
  }

  int **M1; // alocando memoria para M1
  M1 = malloc(m1d[0] * sizeof(int *));
  for (int i = 0; i < m1d[0]; i++) {
    M1[i] = malloc(m1d[1] * sizeof(int));
  }

  // alimentando M1
  for (int i = 0; i < m1d[0]; i++) {
    for (int j = 0; j < m1d[1]; j++) {
      M1[i][j] = 0;
      fscanf(file, "%d", &M1[i][j]);
    }
  }

  fclose(file);

  ///////////////////////////////////////////////////////////////

  // lendo M2
  file = fopen(argv[2], "r");
  if (file == NULL) {
    printf("Problemas na abertura do arquivo\n");
    exit(-1);
  }

  int m2d[2]; // m2d [0] = linha | m2d [1] = coluna
  for (int i = 0; i < 2; i++) {
    m2d[i] = 0;
    fscanf(file, "%d", &m2d[i]);
  }

  int **M2; // alocando memoria para M2
  M2 = malloc(m2d[0] * sizeof(int *));
  for (int i = 0; i < m2d[0]; i++) {
    M2[i] = malloc(m2d[1] * sizeof(int));
  }

  // alimentando M2
  for (int i = 0; i < m2d[0]; i++) {
    for (int j = 0; j < m2d[1]; j++) {
      M2[i][j] = 0;
      fscanf(file, "%d", &M2[i][j]);
    }
  }

  fclose(file);

  ///////////////////////////////////////////////////////////////

  int MTX_D[2]; // MTX_D [0] = linha | MTX_D [1] = coluna
  MTX_D[0] = m1d[0], MTX_D[1] = m2d[1];

  int **MTX_result; // alocando memoria para MTX_result
  MTX_result = malloc(MTX_D[0] * sizeof(int *));
  for (int i = 0; i < MTX_D[0]; i++) {
    MTX_result[i] = malloc(MTX_D[1] * sizeof(int));
  }

  int p = atoi(argv[3]);
  int n_cell = (m1d[0] * m2d[1]) / p;

  pid_t pid[p];
  int pid_p_idx;

  int pid_aux[p];
  for (int i = 0; i < p; i++){
    pid_aux[i] = i;
    char filename[256];
    sprintf (filename, "%dx%d/%dx%d-P%d.txt", MTX_D[0], MTX_D[1],MTX_D[0], MTX_D[1], pid_aux[i]);
    file = fopen(filename, "w");
    
    if (file == NULL) {
      printf("Problemas na abertura do arquivo\n");
      exit(-1);
    }
    
    fclose(file);
  }

  for (pid_p_idx = 0; pid_p_idx < p; pid_p_idx++) {
    
    pid[pid_p_idx] = fork();
    
    if (pid[pid_p_idx] < 0) {
      fprintf(stderr, "criacao falhou");
      exit(-1);
    }

    else if (pid[pid_p_idx] == 0) {
      double total_timer = 0;
      if(pid_aux[pid_p_idx] == pid_p_idx){
        
        char filename[256];
        sprintf (filename, "%dx%d/%dx%d-P%d.txt", MTX_D[0], MTX_D[1],MTX_D[0], MTX_D[1], pid_aux[pid_p_idx]);
        file = fopen(filename, "w");
        fprintf(file, "%d %d\n", m1d[0],m2d[1]); // salvando no arquivo as linhas e colunas

        for (int i = 0; i < MTX_D[0]; i++) {
          for (int j = 0; j < MTX_D[1]; j++) {
            
            double mult_cell_time = 0;
            MTX_result[i][j] = 0;
  
            time_t begin = time(NULL);
            for (int k = 0; k < MTX_D[0]; k++) {
  
              MTX_result[i][j] = MTX_result[i][j] + M1[i][k] * M2[k][j];
             
            }
            time_t end = time(NULL);
  
            mult_cell_time = end - begin;
            total_timer += mult_cell_time;
            fprintf(file, "c(%i,%i) %d %lf\n", i, j, MTX_result[i][j],mult_cell_time);
             
          }
        }
        
      }
       
      fprintf(file, "Total_timer = %lf\n", total_timer);

      FILE *file2;
      char filename2[256];
      sprintf (filename2, "%dx%d/Tempos_finais.txt", MTX_D[0], MTX_D[1]);
      file2 = fopen(filename2, "a");
      fprintf(file2, "%lf\n", total_timer);
      fclose(file2);
      
      fclose(file); 
    }
      
    else {
      for (pid_p_idx = 0; pid_p_idx < p; pid_p_idx++) {
      wait(NULL);
      }
    
      // desalocando matrizes
      for (int i = 0; i < m1d[0]; i++) {
        free(M1[i]);
        free(M2[i]);
        free(MTX_result[i]);
      }
      free(M1);
      free(M2);
      free(MTX_result);
    }
  }

  return 0;
}