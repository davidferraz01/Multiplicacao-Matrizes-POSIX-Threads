#include <stdio.h>
#include <stdint.h>
#include <pthread.h>
#include <stdlib.h>

//Struct para armazenar os dados da thread
typedef struct
{
  double** matriz_1;
  int linhas_1;
  int colunas_1;
  double** matriz_2;
  int linhas_2;
  int colunas_2;
  double** matriz_resultante;
  int linhas_resultante;
  int indice_inicial;
  int linhas_responsaveis;
} Dados_thread;

//Funcao tarefa da thread
void* task (void* dados)
{  
  Dados_thread* dados_thread = (Dados_thread*)dados;
  int fim = dados_thread->indice_inicial + dados_thread->linhas_responsaveis;

  for (int i = dados_thread->indice_inicial; i < fim; i++)
  {
    for (int j = 0; j < dados_thread->colunas_2; j++)
    {
      dados_thread->matriz_resultante[i][j] = 0.0;
      for (int k = 0; k < dados_thread->colunas_1; k++)
      {
        dados_thread->matriz_resultante[i][j] += dados_thread->matriz_1[i][k] * dados_thread->matriz_2[k][j];
      }
    }
  }
  
  return NULL;
}

void multiplicar_matrizes(int num_threads, double** matriz_1, int linhas_1, int colunas_1, double** matriz_2, int linhas_2, int colunas_2, double** matriz_resultante)
{
  pthread_t threads[num_threads];
  Dados_thread dados_thread[num_threads];

  int quant_linhas_thread = linhas_1 / num_threads;
  int linhas_restantes = linhas_1 % num_threads;
  int quant_linhas_ult_thread = 0;
  if(linhas_restantes > 0)
  {
    if(linhas_1 < num_threads)
    {
      quant_linhas_thread = 1;
      num_threads = linhas_1;
    }
    else //linhas_1 > num_threads
    {
      //quant_linhas_thread = linhas_1 / (num_threads - 1);
      quant_linhas_ult_thread = quant_linhas_thread + 1;
    }
  }
  
  int linha_inicial = 0;
  int linhas_responsaveis = 0;
  for (int i = 0; i < num_threads; i++)
  {
    dados_thread[i].matriz_1 = matriz_1;
    dados_thread[i].matriz_2 = matriz_2;
    dados_thread[i].matriz_resultante = matriz_resultante;
    dados_thread[i].linhas_1 = linhas_1;
    dados_thread[i].colunas_1 = colunas_1;
    dados_thread[i].linhas_2 = linhas_2;
    dados_thread[i].colunas_2 = colunas_2;

    if(i == num_threads - 1 && quant_linhas_ult_thread != 0)
    {
      dados_thread[i].linhas_responsaveis = quant_linhas_ult_thread;
    }
    else
    {
      dados_thread[i].linhas_responsaveis = quant_linhas_thread;
      linhas_responsaveis = quant_linhas_thread;
    }
    
    dados_thread[i].linhas_resultante = linhas_1;
    dados_thread[i].indice_inicial = linha_inicial;

    linha_inicial += linhas_responsaveis;
  }  

  //criacao das threads
  for (int i = 0; i < num_threads; i++)
  {
    pthread_create(&threads[i], NULL, task, &dados_thread[i]);
  }

  //garantir que as threads terminaram
  for (int i = 0; i < num_threads; i++)
  {
    pthread_join(threads[i], NULL);
  }
}

double** input_matriz(FILE* input, int linhas, int colunas)
{ 
  //alocando espaco para matriz
  double **matriz = (double**)malloc(linhas * sizeof(double*));
  for (int i = 0; i < linhas; i++)
  {
    matriz[i] = (double*)malloc(colunas * sizeof(double));
  }
  
  //Lendo os elementos
  for (int i = 0; i < linhas; i++) 
  {
    for (int j = 0; j < colunas; j++)
    {
      fscanf(input, "%lf", &matriz[i][j]);
    }
  }
  return matriz;
}

void imprimir_output(FILE *output, double** matriz, int linhas, int colunas, int contador)
{  
  fprintf(output, "M%d:\n", contador);
  for (int i = 0; i < linhas; i++)
  {
    for (int j = 0; j < colunas; j++)
    {
      fprintf(output, "%.2f ", matriz[i][j]);
    }
  }
  fprintf(output, "\n");
}

void free_matriz(double** matriz, int linhas)
{
  for (int i = 0; i < linhas; i++)
  {
    free(matriz[i]);
  }
  free(matriz);
}


int main(int argc, char *argv[]) 
{
  //abrindo arquivos
  FILE *input = fopen(argv[1], "r");
  FILE *output = fopen(argv[2], "w");

  //definindo numero maximo de threads
  int num_threads = 4;

  int num_operacoes;
  //Lendo o numero de operacoes
  fscanf(input, "%d", &num_operacoes);

  for (int k = 0; k < num_operacoes; k++) 
  {
    //Matriz 1
    int linhas_1, colunas_1;
    fscanf(input, "%d %d", &linhas_1, &colunas_1);
    double** matriz_1 = input_matriz(input, linhas_1, colunas_1);
    
    //Matriz 2
    int linhas_2, colunas_2;
    fscanf(input, "%d %d", &linhas_2, &colunas_2);
    double** matriz_2 = input_matriz(input, linhas_2, colunas_2);

    //Matriz Resultante
    double **matriz_resultante = (double**)malloc(linhas_1 * sizeof(double*));
    for (int i = 0; i < linhas_1; i++)
    {
      matriz_resultante[i] = (double*)malloc(colunas_2 * sizeof(double));
    }

    //Processar
    multiplicar_matrizes(num_threads, matriz_1, linhas_1, colunas_1, matriz_2, linhas_2, colunas_2, matriz_resultante);

    imprimir_output(output, matriz_resultante, linhas_1, colunas_2, k);
    
    //liberando espaço alocado das matrizes
    free_matriz(matriz_1, linhas_1);
    free_matriz(matriz_2, linhas_2);
    free_matriz(matriz_resultante, linhas_1);
  }

  //fechando arquivos
  fclose(input);
  fclose(output);

  return 0;
}