#include <stdio.h>
#include <math.h>
#include <ctype.h>

typedef struct
{
	int nome_carga;
	int quant_tarefas;
	int nome_tarefa;
	int temp_execu;
	int periodo ;
	int	deadline;

} cargas_de_trabalho;

int mdc(int a, int b);
int mmc(int a, int b);
int mmcArray(int arr[], int n);
int ciclo_menor(int array_periodo[], int array_deadline[], int contador, int resultado_mmc, int maior_tempo);

int main()
{
	int  cont, resultado_mmc, resultado_menor, maior_tempo;
	int tarefas;
	int contador = 0;
	float div_t_p = 0;
	float div_t_d = 0;
	int tempo_t = 0;
	float periodo_t = 0;
	float deadline_t = 0;
	float testeRM, auxiliar, eleva, teste_EDF, div;

	FILE *arq;
	char Linha[50];
	int carg;
	char tarefa[10] = {0};
	int total_cargas = 0, total_tarefas = 0;
	int i, j, k;
	int count, cargaz;
	int numero;

	arq = fopen("arquivo_com_cargas.txt", "r");

	if (arq == NULL)  // verifica se houve erro na abertura
	{
		printf("HOUVE UM ERRO NA ABERTURA DO ARQUIVO\n\n");
		return 0;
	}

	while (fgets(Linha, sizeof(Linha), arq) != NULL)//percorre o arquivo até o fim
	{
		if ( Linha[0] != 'T')//quando o primeiro elemento da linha não for o nome de uma TAREFA será o nome de uma CARGA
		{
			total_cargas++;//contador para CARGAS
			count = 0;//contador não incrementa se a linha representa uma CARGA.
		}
		else
			count++;//para cada carga, conta-se a quantidade de tarefas
		if (total_tarefas < count)
			total_tarefas = count;//guarda o maior numero de tarefas dentre as cargas
	}

	fclose(arq);
	
	cargas_de_trabalho carga[total_cargas][total_tarefas];

	//array responsavel por guardar os periodos
	int array_periodo[total_tarefas];
	int array_deadline[total_tarefas];

	for(i = 0; i < total_cargas; i++)
	{
		for(j = 0; j < total_tarefas; j++)
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
	}

	fclose(arq);
  
//-------------------------------------------------------------------------------------------
	/*DESTA PARTE DO CODIGO EM DIANTE - TRATA-SE DE CALCULOS DOS TRES TIPOS DE ESCALONAMENTO*/

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
//teste para executivo cíclico
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
//mínimo multiplo comum dos periodos para o ciclo maior
			resultado_mmc = mmcArray(array_periodo, contador);
			printf("\n  Ciclo maior: %d ", resultado_mmc);
//Aqui seria o máximo divisor comum dos periodos para o ciclo menor
			resultado_menor = ciclo_menor(array_periodo, array_deadline, contador, resultado_mmc, maior_tempo);
			printf("\n  Ciclo menor: %d ", resultado_menor);
//testes do EDF, são 3 diferentes, uma para cada tipo de valor de deadline
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
//teste do RM
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
	}
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



// Função para calcular o MMC de um array de números
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


// Função para o ciclo menor
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
