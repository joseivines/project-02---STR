int conta_carga_tarefa( int * cargas, int * tarefas)
{
  FILE *arq;
  char Linha[50];
  int totalLinhas = 0, maxcolunas = 0;
  int count;



  arq = fopen("arquivo_com_cargas.txt", "r");

  if (arq == NULL)  // verifica se houve erro na abertura
  {
    printf("HOUVE UM ERRO NA ABERTURA DO ARQUIVO\n\n");
    return 1;
  }

  while (fgets(Linha, sizeof(Linha), arq) != NULL)//percorre o arquivo atÃ© o fim
  {
    if ( Linha[0] != 'T')//quando o primeiro elemento da linha nÃ£o for o nome de uma TAREFA serÃ¡ o nome de uma CARGA
    {
      totalLinhas++;//contador para CARGAS
      count = 0;//contador nÃ£o incrementa se a linha representa uma CARGA.
    }
    else
      count++;//para cada carga, conta-se a quantidade de tarefas
    if (maxcolunas < count)
      maxcolunas = count;//guarda o maior numero de tarefas dentre as cargas
  }

  *cargas = totalLinhas;
  *tarefas	 = maxcolunas;
  fclose(arq);

  return 0;
}
