#include <stdio.h>
#include <math.h>
#include <ctype.h>
#include <stdlib.h>
#include "conta_carga_tarefa.h"

struct cargas_de_trabalho
{
  int nome_carga;
  int quant_tarefas;
  int nome_tarefa;
  int temp_execu;
  int periodo ;
  int	deadline;
};

void EDF(float , float , float , float , float );
void RM(int , float );
int mdc(int , int );
int mmc(int , int );
int mmcArray(int *, int n);
int ciclo_menor(int *, int *, int contador, int resultado_mmc, int maior_tempo);
int conta_carga_tarefa( int *, int *);
int preenche_array(struct cargas_de_trabalho **carga, int total_cargas, int total_tarefas);

int main()
{
//valores que usarei nos testes, sendo os que virão do txt, a carga, a quantidade de tarefas, o tempo, o periodo e as deadlines

  int  cont, resultado_mmc, resultado_menor, maior_tempo;
  int tarefas;
  int contador = 0;
  float div_t_p = 0;
  float div_t_d = 0;
  int tempo_t = 0;
  float periodo_t = 0;
  float deadline_t = 0;
  float  teste_EDF, div;

  int total_cargas, total_tarefas;
  int i,k;



  //printf( "Tem [%d] cargas no arquivo\n", total_cargas);
  //printf( "O numero maximo de tarefas por carga e: [%d]\n ", total_tarefas);
  conta_carga_tarefa( &total_cargas, &total_tarefas);
  total_cargas = 9;
  total_tarefas = 8;

  struct cargas_de_trabalho **carga = (struct cargas_de_trabalho **)malloc(total_cargas * sizeof(struct cargas_de_trabalho *));
  for (i = 0; i < total_cargas; i++)
    carga[i] = (struct cargas_de_trabalho *)malloc(total_tarefas * sizeof(struct cargas_de_trabalho));

  //array responsavel por guardar os periodos
  int array_periodo[total_tarefas];
  int array_deadline[total_tarefas];

  preenche_array( carga,  total_cargas,  total_tarefas);



//-------------------------------------------------------------------------------------------
  /*DESTA PARTE DO CODIGO EM DIANTE - TRATA-SE DE CALCULOS DOS TRES TIPOS DE ESCALONAMENTO*/

  //for que vai rodar até todas as cargas do arquivo txt serem testadas se escalonam
  for(cont = 1; cont <= total_cargas; cont++)
  {

    contador = 0;
    maior_tempo = carga[cont - 1][0].temp_execu;
    div_t_p = 0;
    div_t_d = 0;
    tempo_t = 0;
    periodo_t = 0;
    deadline_t = 0;
    k = 0;


    while(carga[cont - 1][k].nome_carga != 0 && k < total_tarefas)
      k++;

    tarefas = k;

    for(contador = 0; contador < tarefas; contador++)
    {
      if(maior_tempo < carga[cont - 1][contador].temp_execu)
      {
        maior_tempo = carga[cont - 1][contador].temp_execu;
      }
//Aqui são feitos os cálculos de valores que serão utilizados nos códigos para calcular os escalonamentos
      array_periodo[contador] = carga[cont - 1][contador].periodo;
      array_deadline[contador] = carga[cont - 1][contador].deadline;
      div_t_p = div_t_p + (((float)carga[cont - 1][contador].temp_execu) / ( carga[cont - 1][contador].periodo));
      div_t_d = div_t_d + (carga[cont - 1][contador].temp_execu / carga[cont - 1][contador].deadline);
      tempo_t = tempo_t + (carga[cont - 1][contador].temp_execu);
      periodo_t = periodo_t + ( carga[cont - 1][contador].periodo);
      deadline_t = deadline_t + carga[cont - 1][contador].deadline;

      if(( carga[cont - 1][contador].periodo) <= carga[cont - 1][contador].deadline)
      {
        div = (carga[cont - 1][contador].temp_execu) / ( carga[cont - 1][contador].periodo);
        teste_EDF = teste_EDF + div;
      }
      else
      {
        div = (carga[cont - 1][contador].temp_execu) / carga[cont - 1][contador].deadline;
        teste_EDF = teste_EDF + div;
      }
    }
//teste para ver se o executivo cíclico é escalonável ou não para essa carga
    if(div_t_p > 1)
    {

      printf("\n\nCarga %d\n", cont);
      printf("Executivo: NAO\n");
      printf("Ciclo maior: 0\n");
      printf("Ciclo menor: 0\n");
      printf("EDF: NAO\n");
      printf("RM: NAO");
    }

    else
    {
      printf("\n\nCarga %d ", cont);
      printf("\nExecutivo: SIM");


//Chama a função que calcula o mmc dos periodos da carga, tal valor resulta no ciclo maior do escalonamento cíclico, que é então escrita por printf quando o códig roda
      resultado_mmc = mmcArray(array_periodo, contador);
      printf("\n  Ciclo maior: %d ", resultado_mmc);

//Chama a função que calcula o ciclo menor do escalonamento cíclico, se utilizando de vários daos obtidos anteriormente no código,e esse valor do ciclo menor é então escrito por printf quando o códig roda
      resultado_menor = ciclo_menor(array_periodo, array_deadline, contador, resultado_mmc, maior_tempo);
      printf("\n  Ciclo menor: %d ", resultado_menor);

//Chama a função EDF que faz os testes de escalonamento para EDF
      EDF( deadline_t, periodo_t, div_t_p,  div_t_d, teste_EDF);

//Chama a função EDF que faz os testes de escalonamento para RM
      RM(tarefas, div_t_p);
    }
  }


//destruindo o array bid de cargasXtarefas
  for ( i = 0; i < total_cargas; i++)
  {
    free(carga[i]);
  }
  free(carga);

  return 0;
}


// Função para calcular o máximo divisor comum (MDC) de dois números
int mdc(int a, int b)
{
  if (b == 0)
  {
    return a;
  }
  return mdc(b, a % b);
}

// Função para calcular o MMC de dois números
int mmc(int a, int b)
{
  return (a * b) / mdc(a, b);
}

// Função para calcular o MMC de um array de números, tal função será usada para encontrar o valor do ciclo maior do escalonamento cíclico caso ele seja possível
int mmcArray(int arr[], int n)
{
  int resultado = arr[0];
  int i;
  for ( i = 1; i < n; i++)
  {
    resultado = mmc(resultado, arr[i]);
  }
  return resultado;
}


// Função para o ciclo menor usando os valores recebidos abaixo, ela em resumo irá testar todos os divisores inteiros do ciclo maior que não deixam restos, verificando quais deles passam nos testes que determinam o valor do ciclo menor, e caso eles passem a função retorna então esse valor, desse modo se obtendo o ciclo menor do escalonamento cíclico caso ele seja possível
int ciclo_menor(int array_periodo[], int array_deadline[], int contador, int resultado_mmc, int maior_tempo)
{

  int resultado = 0;
  int cont = 3;
  int compara = 0;
  int resultado_mdc;
  int maior_dead = array_deadline[0];

  int i, z;

  for ( i = 2; i <= resultado_mmc; i++)
  {

    compara = 0;
    if (resultado_mmc % i == 0)
    {
      resultado = i;
      if(maior_tempo <= i)
      {
        for( cont = 0; cont < contador; cont++)
        {
          if(compara == contador)
          {
            return resultado;
          }
          if(maior_dead < array_deadline[cont])
          {
            maior_dead = array_deadline[cont];
          }
          resultado_mdc = mdc(array_periodo[cont], i);
          if(((2 * i) - resultado_mdc) <= array_deadline[cont])
          {
            compara++;
            if(compara == contador)
            {
              return resultado;
            }
          }
        }
        if(compara == contador)
        {
          return resultado;
        }
      }
    }

    if(i > maior_dead)
    {
      for ( z = maior_dead; z >= 0; z--)
      {

        int compara = 0;
        if (resultado_mmc % z == 0)
        {
          resultado = z;
          for( cont = 0; cont < contador; cont++)
          {
            if(compara == contador)
            {
              return resultado;
            }
            if(maior_dead < array_deadline[cont])
            {
              maior_dead = array_deadline[cont];
            }
            resultado_mdc = mdc(array_periodo[cont], z);
            if(((2 * z) - resultado_mdc) <= array_deadline[cont])
            {
              compara++;
              if(compara == contador)
              {
                return resultado;
              }
            }
          }
          if(compara == contador)
          {
            return resultado;
          }
        }
      }
      break;
    }
  }
  return 0;
}

//Função que recebe os de deadline_t, periodo_t, de div_t_p, div_t_d, teste_EDF usando tais valores para calcular se a carga em questão é escalonavel em EDF
void EDF(float deadline_t, float periodo_t, float div_t_p, float div_t_d, float teste_EDF)
{
  if(deadline_t == periodo_t)
  {
    if(div_t_p <= 1)
    {
      printf("\nEDF:SIM");
    }
    else
    {
      printf("\nEDF:NAO");
    }
  }

  else if(deadline_t < periodo_t)
  {
    if(div_t_d <= 1)
    {
      printf("\nEDF:SIM");
    }
    else
    {
      printf("\nEDF: INCONCLUSIVO");
    }
  }

  else
  {
    if(teste_EDF <= 1)
    {
      printf("\nEDF: SIM\n");
    }
    else
    {
      printf("\nEDF: INCONCLUSIVO\n");
    }
  }
}

//Função que recebe os valores de tarefas e de div_t_p, usando tais valores para calcular se a carga em questão é escalonavel em RM
void RM(int tarefas, float div_t_p)
{
  float eleva, auxiliar, testeRM;
  eleva = 1.0 / tarefas;
  auxiliar = pow(2.0, eleva);

  testeRM = tarefas * (auxiliar - 1);
  if(div_t_p <= testeRM)
  {

    printf("\nRM: SIM\n");
  }
  else
  {
    printf("\nRM: INCONCLUSIVO\n");
  }
}



int preenche_array(struct cargas_de_trabalho **carga, int linhas, int colunas)
{
  FILE *arq;
  int i, j;
  int cargaz, carg;
  char tarefa[20] = {0};
  int numero;


  for(i = 0; i < linhas; i++)
  {
    for(j = 0; j < colunas; j++)
    {
      carga[i][j].nome_carga = 0;
      carga[i][j].nome_tarefa = 0;
      carga[i][j].temp_execu = 0;
      carga[i][j].periodo = 0;
      carga[i][j].deadline = 0;
    }
  }




  arq = fopen("arquivo_com_cargas.txt", "r");
  if (arq == NULL)
  {
    perror("Erro ao abrir o arquivo");
    return 1;
  }

  carg = 0;
  i = 0;
  j = 0;



  for(i = 0; ( (fscanf(arq, "%d", &cargaz) == 1) + 2) && (feof(arq) == 0) ; i++)
  {
    carg++;

    for(j = 0 ; (fscanf(arq, "%s %d %d %d", tarefa, &carga[i][j].temp_execu, &carga[i][j].periodo, &carga[i][j].deadline) == 4) && feof(arq) == 0  ; j++)
    {
      numero = tarefa[1] - '0';
      carga[i][j].nome_carga = carg;
      carga[i][j].nome_tarefa = numero;
    }
    //printf("\n");
  }
  fclose(arq);

  return 0;
}
