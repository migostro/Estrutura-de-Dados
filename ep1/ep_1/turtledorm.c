/*    
 \__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__

  AO PREENCHER ESSE CABEÇALHO COM O MEU NOME E O MEU NÚMERO USP, 
  DECLARO QUE SOU O ÚNICO AUTOR E RESPONSÁVEL POR ESSE PROGRAMA. 
  TODAS AS PARTES ORIGINAIS DESSE EXERCÍCIO-PROGRAMA (EP) FORAM 
  DESENVOLVIDAS E IMPLEMENTADAS POR MIM SEGUINDO AS INSTRUÇÕES DESSE EP
  E QUE PORTANTO NÃO CONSTITUEM PLÁGIO. DECLARO TAMBÉM QUE SOU RESPONSÁVEL
  POR TODAS AS CÓPIAS DESSE PROGRAMA E QUE EU NÃO DISTRIBUI OU FACILITEI A
  SUA DISTRIBUIÇÃO. ESTOU CIENTE QUE OS CASOS DE PLÁGIO SÃO PUNIDOS COM 
  REPROVAÇÃO DIRETA NA DISCIPLINA.

  Nome: Miguel Pereira Ostrowski
  NUSP: 10723610

  main.c

  Referências: Com exceção das rotinas fornecidas no esqueleto e em sala
  de aula, caso você tenha utilizado alguma referência, liste-as abaixo
  para que o seu programa não seja considerada plágio.
  Exemplo:

  - função mallocSafe copiada de: 

       http://www.ime.usp.br/~pf/algoritmos/aulas/aloca.html
       
 \__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__
*/
  

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/* tamanho máximo de um tdorm */
#define MAX 128

#define ACORDADO '#'
#define DORMINDO ' '
#define TAPINHA 'T'

#define ESPACO '	'

#define TRUE 1
#define FALSE 0

// Recebe *nLin, *nCol e tDorm
// Pede para o usuario digitar o nome do arquivo que esta o jogo
// Grava o tabuleiro em tDorm, a linha a coluna em nLin e nCol
void leiaTurtledorm(int *nLin, int *nCol, int tDorm[][MAX])
{
	int elemento;
	char nome_arquivo[10], linha_arquivo[3];

	FILE *arquivo_de_entrada;

	printf("Digite o nome do arquivo\n");
	scanf("%s", nome_arquivo);

	arquivo_de_entrada = fopen(nome_arquivo, "r"); /* abre o arquivo para leitura */

	int i = 0, j = 0, div, aux, numCol = 2;

	while (!feof(arquivo_de_entrada)){
		if (fscanf(arquivo_de_entrada, "%d", &elemento) != 1){
			continue;
		}
		// 0 e 1 foram reservados de tal forma para guardar nLin e nCol
		if (i == 0){
			*nLin = elemento;
			i++;
		} else if (i == 1){
			*nCol = elemento;
			i++;
		} else if (elemento == 0 || elemento == 1){
			aux = j/(*nCol); // Representa a lihha
			div = j%(*nCol); // Representa a coluna

			tDorm[aux][div] = elemento;
			j++;
		} 
	}
}

// Esta função recebe: as dimensões do tabuleiro, sendo nLin o numero de linhas
// e nCol o numero de colunas; tDorm sendo o tabuleiro em si, podendo ser a matriz binaria
// ou a matriz resolução; e um parametro c que para 0 será para printar a matriz binária
// e 1 para printar a resolução com os 'T's
void mostreTurtledorm(int nLin, int nCol, int tDorm[][MAX], char c)
{
	// parametro c, se igual a 0 mostra os acordados, e caso seja igual a 1 printa os tapinhas
	if (c == 0)
	{
		c = ACORDADO;
	} else {
		c = TAPINHA;
	}

	printf("\n\nÉ uma mariz %dx%d\n", nLin,nCol);
	printf("%c%c%c%c", DORMINDO, ESPACO, DORMINDO,DORMINDO);
	for (int i = 0; i < nCol; ++i)
	{
		if (i+1<=9)
		{
			printf("%d%c%c%c", i+1, DORMINDO, DORMINDO,DORMINDO);
		} else if (i+1 <= 99)
		{
			printf("%d%c%c", i+1, DORMINDO, DORMINDO);
		} else 
		{
			printf("%d%c", i+1, DORMINDO);
		}
		
	}
	printf("\n");

	for (int i = 0; i < nLin; ++i)
	{
		printf("%c%c", DORMINDO, ESPACO);
		for (int k = 0; k < nCol; ++k)
		{
			printf("+---");
		}
		printf("+\n%c%c%c%c%c%d%c|",DORMINDO,DORMINDO,DORMINDO,DORMINDO,DORMINDO, i+1, ESPACO);

		for (int j = 0; j < nCol; ++j)
		{
			if (tDorm[i][j] == 0)
			{
				printf("%c%c%c|",DORMINDO, DORMINDO, DORMINDO);
			} else 
			{
				printf("%c%c%c|",DORMINDO, c, DORMINDO);
			}
			
		}
		printf("\n");
	}

	printf("%c%c", DORMINDO, ESPACO);
	for (int k = 0; k < nCol; ++k)
	{
		printf("+---");
	}
	printf("+\n");
}

// Recebe nLin, nCol, tDorm, e lin e col como posição que será dado o tapinha
// Muda a matriz tDorm no lugar onde foi dado o "tapinha"
void tapinhaTurtle(int nLin, int nCol, int tDorm[][MAX], int lin, int col)
{
	if (tDorm[lin][col] == 1)
	{
		tDorm[lin][col] = 0;
	} else 
	{
		tDorm[lin][col] = 1;
	}

	for (int i = -1; i <= 1; i = i+2)
	{
		// Linha e casa[lin][col]
		// Acorda
		if (tDorm[lin+i][col] == 0)
		{

			if (lin+i < nLin && lin+i >= 0)
				tDorm[lin+i][col] = 1;
			
		} 
		else // faz dormir
		{
			if (lin+i < nLin && lin+i >= 0)
				tDorm[lin+i][col] = 0;
			
		}

		// Coluna
		// Acorda
		if (tDorm[lin][col+i] == 0)
		{
			if (col+i < nCol && col+i >= 0)
				tDorm[lin][col+i] = 1;	
		} 
		else // Faz dormir
		{
			if (col+i < nCol && col+i >= 0)
				tDorm[lin][col+i] = 0;
			
		}
	}
}

// Recebe nLin, nCol e tDorm.
// Retorna TRUE se a matriz tDorm for nula (Todos estão dormindo)
// e FALSE caso contrario
int todosDormindo(int nLin, int nCol, int tDorm[][MAX]) //retorna TRUE ou FALSE
{
	for (int i = 0; i < nLin; ++i)
	{
		for (int j = 0; j < nCol; ++j)
		{
			if (tDorm[i][j] == 1)
			{
				return FALSE;
			}
		}
	}

	return TRUE;
}

// Recebe nLin, nCol e tDorm e salva o tabuleiro atual e o numero de linhas e colunas no arquivo .txt
int graveTurtledorm(int nLin, int nCol, int tDorm[][MAX]) // FAZER ESSA FITA
{
	char nome_arquivo[30];

	printf("Digite o nome do arquivo txt que salvará o jogo\n");
	scanf("%s", nome_arquivo);
	FILE *arquivo; // cria variável ponteiro para o arquivo

	arquivo = fopen(nome_arquivo, "w");

	fprintf(arquivo, "%d %d\n", nLin, nCol);

	for (int i = 0; i < nLin; ++i)
	{
		for (int j = 0; j < nCol; ++j)
		{
			fprintf(arquivo, "%d ", tDorm[i][j]);
		}

		fprintf(arquivo, "\n");
	}

	fclose(arquivo);

	printf("\nSeu jogo foi gravado com sucesso!\n\n");

	return 0;
}

// Recebe dois inteiros low e high, e devolve um numero
// inteiro no intervalo fechado entre os dois numeros [low;high].
// Não funciona como o esperado caso low > high.
int randomInteger(int low, int high)
{
	double d;
    d = (double) rand () / ((double) RAND_MAX + 1);
    int k = d * (high - low + 1);
    return low + k;
}

// Recebe ponteiro de nLin e nCol, e a matriz binaria tDorm.
// Sorteia a matriz com base nos parametros digitados pelo usuario
void sorteieTurtledorm(int *nLin, int *nCol, int tDorm[][MAX])
{
	// nCasas é o numero total de casas da matriz
	// lin e col sao variaveis 0 <= lin < nLin, 0 <= col < nCol
	int semente, low, high, nTapinhas, nCasas, casa, lin, col, nTurtlesDespertos = 0;

	char dificuldade;

	printf("Qual a dimensão do turtledorm?\n");
 	scanf("%d %d", nLin, nCol);

 	printf("Qual a semente do sorteio?\n");
 	scanf("%d", &semente);

 	printf("Qual o nivel de dificuldade [f/m/d]\n");
 	scanf("%s",	 &dificuldade);



	srand(semente);

	if (dificuldade == 'f')
	{
		low = (*nLin)*(*nCol)*0.05;
		high = (*nLin)*(*nCol)*0.2;

		nTapinhas = randomInteger(low, high);
	} 
	else if (dificuldade == 'm')
	{
		low = (*nLin)*(*nCol)*0.25;
		high = (*nLin)*(*nCol)*0.5;

		nTapinhas = randomInteger(low, high);
	} 
	else if (dificuldade == 'd')
	{
		low = (*nLin)*(*nCol)*0.55;
		high = (*nLin)*(*nCol)*0.85;

		nTapinhas = randomInteger(low, high);
	}

	nCasas = (*nLin)*(*nCol);

	for (int i = 0; i < nTapinhas; ++i)
	{
		casa = randomInteger(0, nCasas-1);

		lin = casa/(*nCol);
		col = casa%(*nCol);

		tapinhaTurtle(*nLin, *nCol, tDorm, lin, col);
	}

	// Conta os turtles despertos
	for (int i = 0; i < *nLin; ++i)
	{
		for (int j = 0; j < *nCol; ++j)
		{
			if (tDorm[i][j] == 1)
			{
				nTurtlesDespertos++;
			}
		}
	}

	printf("Numero de tapinhas dados: %d\nNumero de turtles despertos: %d\n\n", nTapinhas, nTurtlesDespertos);
}

// Recebe os parametros *v, n, x
// Procura se existe o valor x no vetor *v
int busca(int *v, int n, int x){

	//printf("Dentro 1\n");
	for(int i = 0; i < n; i++)
	{
		//printf("%d\n",v[i]);
		if (v[i] == x)
		{
			return TRUE;
		}
	}

	return FALSE;
}

// Recebe nLin, nCol e tDorm
// Printa a resolução sem alterações em tDorm
void resolvaTurtledorm(int nLin, int nCol, int tDorm[][MAX])
{
	// matriz é uma copia de tDorm para não alterar a original
	// matriz_resolucao é a matriz de onde devera ser dado os tapinhas
	// v é um vetor onde v[i] pertence a [0; nCasas[, que guarda onde será dado cada tapinha (cada elemento de v representa um tapinha diferente)
	// 
	int matriz[MAX][MAX], matriz_resolucao[MAX][MAX], nCasas, *v, k, aux, lin, col;

	nCasas = nLin*nCol;

	v = malloc(nCasas*sizeof(int*));

	for (int i = 0; i < nLin; ++i)
	{
		for (int j = 0; j < nCol; ++j)
		{
			matriz[i][j] = tDorm[i][j];
			matriz_resolucao[i][j] = 0;
		}
	}

	// Define o numero máximo de tapinhas que pode ser dado no tabuleiro
	for (int nTapinhas = 1; nTapinhas < nCasas; ++nTapinhas)
	{
		// Posiciona os tapinhas em casas diferentes
		// Otimiza ligeiramente
		for (int aux = 0; aux < nTapinhas; ++aux)
		{
			v[aux] = aux;
		}

		// Itera v[0] até o numero maximo de casas
		// Quando algum v[i] == nCasas, i pertencendo ao intervalo [0 ;nCasas-1], v[i+1]++
		while (v[nTapinhas-1] != nCasas-1)
		{
			v[0]++;
			
			if (v[0]%nCasas == 0 && nTapinhas > 1)
			{
				for (int m = 0; m < nTapinhas; m++)
				{
					if (v[m]%nCasas == 0)
					{
						// Escolhe uma casa para a posicão m do vetor v que seja unico nesse momento
						aux = 0;
						while (busca(v, nTapinhas, aux)){
							aux++;
						}

						v[m] = aux;
						v[m+1]++;
					}
				}
			}

			// Da os tapinhas usando os elementos de v
			for (int m = 0; m < nTapinhas; ++m)
			{
				//printf("c\n");
				lin = v[m]/nCol;
				col = v[m]%nCol;
				tapinhaTurtle(nLin, nCol, matriz, lin, col);

				// Se todos turtles estiverem dormindo
				// Chama metodo para printar a resolução
				if (todosDormindo(nLin, nCol, matriz))
				{
					for (int l = 0; l < nTapinhas; ++l)
					{
						lin = v[l]/nCol;
						col = v[l]%nCol;
						matriz_resolucao[lin][col] = 1;
					}
					mostreTurtledorm(nLin, nCol, matriz_resolucao, 1);
					
					free(v);

					return;
				} 
			}
			// Deixa a matriz binária como antes dos tapinhas dados
			for (int m = 0; m < nTapinhas; ++m)
			{
				//printf("c\n");
				lin = v[m]/nCol;
				col = v[m]%nCol;
				tapinhaTurtle(nLin, nCol, matriz, lin, col);
			}
		}
	}

	printf("NÃO HÁ RESOLUÇÃO\n");

	free(v);
}

int main(int argc, char *argv[]){

	// mododejogo é a variavel onde esta guardado se o jogo será sorteado ou se será sorteado
	char mododejogo, *comando;
	int nLin, nCol, lin, col, tDorm[MAX][MAX], desistiu = 0;

	comando = calloc(3,sizeof(char));

	printf("(s)ortear ou (l)er arquivo?\n");
	scanf("%c", &mododejogo);

	if (mododejogo == 's')
	{
		sorteieTurtledorm(&nLin, &nCol, tDorm);
	} else if (mododejogo == 'l')
	{
		leiaTurtledorm(&nLin, &nCol, tDorm);
	}
	
 	while(!todosDormindo(nLin, nCol, tDorm))
 	{
 		mostreTurtledorm(nLin, nCol, tDorm, 0);

 		printf("\nColoque uma posição [lin][col]; ou\num caractere 'd' para (d)esistir da partida;\num caractere 'a' para (a)juda, que mostra uma solução ótima da partida;\num caractere 'g' para (g)ravar o turtledorm atual em um arquivo.\n");
 		scanf("%s", comando);
 		
 		if (comando[0] == 'd') // Desistir
 		{
 			desistiu = 1;
 			break;
 		} else if (comando[0] == 'a'){ // solucão
 			resolvaTurtledorm(nLin, nCol, tDorm);
 		} else if (comando[0] == 'g'){ // gravar
 			graveTurtledorm(nLin, nCol, tDorm);

 		} else { // jogar
 			scanf("%d", &col);

 			lin = atoi(comando);
 			lin = lin-1;
 			col = col-1;

 			tapinhaTurtle(nLin, nCol, tDorm, lin, col);
 		}
 	}

 	if (desistiu == 1)
 	{
 		printf("Você dessistiu da partida!\n");
 	} 
 	else if (desistiu == 0)
 	{
 		printf("Todos os turtles estão dormindo, VOCÊ GANHOU!\n");
 	}

 	free(comando);

	return 0;
}