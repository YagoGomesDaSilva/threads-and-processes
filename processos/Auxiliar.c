#include <stdio.h>
#include <string.h>
#include <stdlib.h> // necessário p/ as funções rand() e srand()
#include <time.h>   //necessário p/ função time()


void convert_to_int(int argc, char *argv[], int *arg) {
  for (int i = 1; i < argc; i++) {
    char str[5];
    strcpy(str, argv[i]);
    arg[i - 1] = atoi(str);
  }
}

int main(int argc, char *argv[]) {

  int arg[argc - 1];

  convert_to_int(argc, argv, arg);

  if (arg[0] != arg[3]) {
    for (int i = 0; i < argc - 1; i++) {
      printf("%i\n", arg[i]);
    }
    printf("Para multiplicar as mastrizes o numero de linhas da matriz A tem "
           "que ser igual ao numero de colunas da matriz B!\n");
    exit(1);
  }

  int m1l = arg[0], m1c = arg[1], m2l = arg[2], m2c = arg[3];
  
  //int M1[m1l][m1c], M2[m2l][m2c];
  
  int ** M1;//alocando memoria para M1
  M1 = malloc( m1l * sizeof(int*));
  for (int i = 0; i < m1l; i++){
     M1[i] = malloc( m1c * sizeof(int));
  }
  int ** M2;//alocando memoria para M2
  M2 = malloc( m2l * sizeof(int*));
  for (int i = 0; i < m2l; i++){
     M2[i] = malloc( m2c * sizeof(int));
  }

  //add numeros aleatorios
  srand(time(NULL));
  for (int i = 0; i < m1l; i++) {
    for (int j = 0; j < m1c; j++) {
      M1[i][j] = rand() % 100;
    }
  }
  for (int i = 0; i < m2l; i++) {
    for (int j = 0; j < m2c; j++) {
      M2[i][j] = rand() % 100;
    }
  }
  
//add no arquivo 
  FILE* file;
  file = fopen("M1.txt","w");
  fprintf(file, "%d\n%d\n", m1l, m1c);
  
  for (int i = 0; i < m1l; i++) {
    for (int j = 0; j < m1c; j++) {
      int num = M1[i][j];
      fprintf(file, "%d\n", num);
    }
}
  fclose(file);
  
  file = fopen("M2.txt","w");
  fprintf(file, "%d\n%d\n", m2l, m2c);
  for (int i = 0; i < m2l; i++) {
    for (int j = 0; j < m2c; j++) {
      int num = M2[i][j];
      fprintf(file, "%d\n", num);
    }
}
  fclose(file);

  for(int i = 0; i < m1l ; i++ ){
    free(M1[i]);
    free(M2[i]);
  }
  free(M1);
  free(M2);
 
  return 0;
}