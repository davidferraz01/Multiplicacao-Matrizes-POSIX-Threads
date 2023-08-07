# Multiplicação de Matrizes POSIX Threads

Este é um programa em C que realiza a multiplicação de matrizes utilizando a biblioteca POSIX Threads para paralelizar o processo. O código lê os dados de entrada de um arquivo, executa a multiplicação de matrizes de forma paralela e grava o resultado em outro arquivo de saída. Esse código fonte foi desenvolvido como parte do estudo realizado na disciplina de Interface/Hardware Software.

## Descrição
O código consiste em uma implementação da multiplicação de matrizes utilizando threads POSIX para paralelizar o processo. A função `multiplicar_matrizes` é responsável por realizar a multiplicação das matrizes, dividindo o trabalho entre as threads. O struct `Dados_thread` é usado para armazenar os dados necessários para cada thread executar a multiplicação parcial das matrizes. A função `task` é a tarefa executada por cada thread. Cada thread recebe uma porção das linhas da primeira matriz para multiplicar pela segunda matriz. As matrizes são lidas do arquivo de entrada utilizando a função `input_matriz`. O resultado da multiplicação é gravado no arquivo de saída usando a função `imprimir_output`. É possível Modificar o valor de `num_threads` no código-fonte para controlar o número de threads a serem utilizadas na multiplicação.

## Formato do Arquivo de Entrada (pthread.input)

O arquivo `pthread.input` deve conter o seguinte formato, no qual cada operação de multiplicação é definida por um conjunto de duas matrizes:

```
<num_operacoes>
<linhas_matriz1> <colunas_matriz1>
<elementos da matriz_1>

<linhas_matriz2> <colunas_matriz2>
<elementos da matriz_2>
...
```

## Compilação e Execução

Para compilar o programa, utilize o seguinte comando:

```bash
gcc -g -Wall main.c -o main.bin -pthread
```

Após compilar, você pode executar o programa da seguinte forma:

```bash
./main.bin pthread.input pthread.output
```

Onde:
- `pthread.input` é o arquivo de entrada contendo as matrizes a serem multiplicadas. 
- `pthread.output` é o arquivo de saída onde o resultado da multiplicação será gravado.
