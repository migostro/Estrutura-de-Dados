/*
  \__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__

  AO PREENCHER ESSE CABEÇALHO COM O MEU NOME E O MEU NÚMERO USP, 
  DECLARO QUE SOU O ÚNICO AUTOR E RESPONSÁVEL POR ESSE PROGRAMA. 
  TODAS AS PARTES ORIGINAIS DESSE EXERCÍCIO PROGRAMA (EP) FORAM 
  DESENVOLVIDAS E IMPLEMENTADAS POR MIM SEGUINDO AS INSTRUÇÕES DESSE EP
  E QUE PORTANTO NÃO CONSTITUEM PLÁGIO. DECLARO TAMBÉM QUE SOU RESPONSÁVEL
  POR TODAS AS CÓPIAS DESSE PROGRAMA E QUE EU NÃO DISTRIBUI OU FACILITEI A
  SUA DISTRIBUIÇÃO. ESTOU CIENTE QUE OS CASOS DE PLÁGIO SÃO PUNIDOS COM 
  REPROVAÇÃO DIRETA NA DISCIPLINA.

  Nome: Miguel Pereira Ostrowski
  NUSP: 10723610

  IMDB: filmes.c


  Referências: Com exceção das rotinas fornecidas no esqueleto e em sala
  de aula, caso você tenha utilizado alguma refência, liste-as abaixo
  para que o seu programa não seja considerada plágio.
  Exemplo:
  - função mallocc retirada de: http://www.ime.usp.br/~pf/algoritmos/aulas/aloca.html

  \__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__
*/


/*----------------------------------------------------------*/
/* filmes.h e a interface para as funcoes neste modulo      */
#include "filmes.h" 

/*----------------------------------------------------------*/
#include <stdlib.h>  /* NULL, free() */
#include <stdio.h>   /* printf(), scanf() */ 
#include <string.h>  /* strlen(), strncpy(), strcmp(), strtok() */

#include "util.h"    /* Bool, mallocSafe() */
#include "iofilmes.h"
#include "st.h"      /* freeST(), initST(), putFilmeST(), getFilmeST(),
                        showST(), freeST() */

/*----------------------------------------------------------------------
 *  crieFilme
 *
 *  Recebe informacoes dobre um filme 
 *
 *      - DIST  : distribuicao de notas
 *      - VOTOS : numero de votos
 *      - NOTA  : nota do filme 
 *      - ANO   : ano de producao do filme
 *
 *  e cria uma celula do tipo Filme para armazenar essa informacao. 
 *  A funcao retorna o endereco da celula criada.
 */
Filme *
crieFilme (char dist[], int votos, float nota, char *nome, int ano)
{
    Filme *flm;
    int    len = strlen(nome) + 1; /* +1 para o '\0' */
    
    flm = mallocSafe(sizeof *flm);
    
    strncpy(flm->dist, dist, TAM_DIST+1); /* +1 para o '\0' */
    
    flm->votos = votos;
    flm->nota  = nota;
    
    flm->nome = mallocSafe(len*sizeof(char));
    strncpy(flm->nome, nome, len);
    
    flm->ano  = ano;

    flm->prox = flm->ant = NULL; /* paranoia */
    
    return flm;
}

/*----------------------------------------------------------------------
 *  crieListaFilmes
 * 
 *  Cria uma estrutura que representa lista de filmes vazia.
 *  A funcao retorna o endereco dessa estrutura.
 *
 *  Um lista de filmes e representada por uma lista encadeada 
 *  duplamente ligada com cabeca. 
 */
ListaFilmes *
crieListaFilmes()
{
    printf("AAAAAAAAAAAAA\n");
    ListaFilmes *lst;

    lst = mallocSafe(sizeof(ListaFilmes*));
    lst->cab = mallocSafe(sizeof(Filme*));

    printf("BBBBBBBBBBBBBBBb\n");
    lst->nFilmes = 0;
    /*lst->cab = cabeca;*/
    lst->cab->prox = NULL/*lst->cab*/;
    lst->cab->ant = NULL/*lst->cab*/;
    

    AVISO(crieListaFilmes em filmes.c: Vixe! Ainda nao fiz essa funcao ...); 
    return lst;
}

/*----------------------------------------------------------------------
 *  libereListaFilmes(lst)
 *
 *  Recebe um ponteiro lst para uma estrutura que representa uma lista 
 *  de filmes e libera toda a memoria alocada para a lista.
 *
 *  Esta funcao utiliza a funcao libereFilme().
 */

void
libereListaFilmes(ListaFilmes *lst)
{
    int i;
    Filme *p;
    
    for (i = 0; i < lst->nFilmes; ++i)
    {
        p = lst->cab->prox;
        lst->cab->prox = lst->cab->prox->prox;

        libereFilme(lst->cab->prox);
    }
    lst->nFilmes = 0;

    lst->cab->prox = NULL;
    lst->cab->ant = NULL;
}

/*----------------------------------------------------------------------
 *  libereFilme
 *
 *  Recebe um ponteiro FLM para uma estrutura que representa um 
 *  filme e libera a area alocada.
 *
 */
void 
libereFilme(Filme *flm)
{
    Filme *anterior, *proximo, *aux;

    aux = flm;

    anterior = flm->ant;
    proximo = flm->prox;

    anterior->prox = proximo;
    proximo->ant = anterior;

    free(aux);
}

/*----------------------------------------------------------------------
 *  insiraFilme
 *
 *  Recebe um ponteito LST para uma estrutura que representa
 *  uma lista de filmes e um ponteiro FLM para uma estrutura
 *  que representa uma filme.
 *
 *  A funcao insere o filme na lista.
 *  
 */
void 
insiraFilme(ListaFilmes *lst, Filme *flm)
{
    Filme *ultimo, *primeiro;

    primeiro = lst->cab->prox;
    ultimo = lst->cab->ant;

    if (lst->cab->prox != NULL && lst->cab->ant != NULL)
    {
        flm->ant = ultimo;
        ultimo->prox = flm;

        flm->prox = primeiro;
        primeiro->ant = flm;

    }
    else
    {
        lst->cab->prox = flm;
        lst->cab->ant = flm;

        flm->prox = flm;
        flm->ant = flm;
    }

    lst->cab->ant = flm;

    lst->nFilmes += 1;
}

void procuraFilme(ListaFilmes *lst, char *filme, int m, int n)
{
    Filme *flm;
    char verifica;

    for (flm = lst->cab->prox; flm != lst->cab; flm = flm->prox)
    {
        if (achePalavra(filme, m, flm->nome, strlen(flm->nome)))
        {
            mostreFilme(flm);

            printf("Esse e' o filme procurado? [s/n/x] (x para sair):");
            scanf("%c", &verifica);
            if (verifica == 's')
            {
                return;
            }
        }
    }
}




/*---------------------------------------------------------------------
 *  contemFilme
 *
 *  Recebe um ponteito LST para uma estrutura que representa
 *  uma lista de filmes e um ponteiro FLM para uma estrutura
 *  que representa uma filme.
 *
 *  A funcao retorna  TRUE se o filme esta na lista e 
 *  FALSE em caso contrario. 
 *
 *  Consideramos que dois filmes f e g sao iguais se
 *
 *       - f->nome e' igual a g->nome 
 *       - f->nota e' igual a g->nota
 *       - f->ano  e' igual a g->ano 
 *
 *  Para comparar dois nomes voce pode usar alguma funcao da 
 *  bibliteca do c  como strcmp, strncmp (string,h) 
 *  ou a funcao strCmp (util.h).
 *
 */
Bool 
contemFilme(ListaFilmes *lst, Filme *flm)
{
    int i;
    Filme *p;
    p = lst->cab->prox;

    for (i = 0; i < lst->nFilmes; ++i)
    {
        if (p->nota == flm->nota && p->ano == flm->ano && strcmp(p->nome,flm->nome) == 0)
        {
            return TRUE;
        }
    }
    AVISO(contemFilme em filme.c: Vixe! Ainda nao fiz essa funcao...);
    return FALSE;
}

/*----------------------------------------------------------------------
 *  removaFilme
 *
 *  Remove da lista de filmes LST o filme apontado por FLM.
 *    
 *  Pre-condicao: a funcao supoe que o filme FLM esta 
 *                na lista LST.
 */
void 
removaFilme(ListaFilmes *lst, Filme *flm)
{
    Filme *p;
    p = lst->cab->prox;

    if (lst->nFilmes == 1)
    {
        lst->cab->prox = NULL;
        lst->cab->ant = NULL;

        free(flm);
    }
    else
    {
        if (flm == lst->cab->prox)
        {
            lst->cab->prox = flm->prox;
            flm->prox->ant = lst->cab->ant;

            lst->cab->ant->prox = flm->prox;

            free(flm);
        }
        else if (flm == lst->cab->ant)
        {
            lst->cab->ant = flm->ant;
            flm->ant->prox = lst->cab->prox;

            lst->cab->prox->ant = flm->ant;

            free(flm);
        }
        else
        {
            libereFilme(flm);
        }

    }

    lst->nFilmes = lst->nFilmes - 1;

    AVISO(removaFilme em filmes.c: Vixe! Ainda nao fiz essa funcao...);
}

/* função que arredonda pra cima a divisão por 2*/
static int teto(int a)
{
    return (a+1)/2;
}

static void intercalaNome(ListaFilmes *lst, Filme *flmMeio, int meio)
{
    /*min e max são ponteiros que guardam o maximo e minimo do trecho verificado*
    /*vão ser usados para mudar a babeça ant e prox*/
    Filme *p, *q, *anterior, *proximo, *aux, *min, *max;
    ListaFilmes *lst1, *lst2;
    int i;

    p = lst->cab->prox;
    q = flmMeio;

    min = lst->cab->prox;
    max = lst->cab->ant;

    /* foi utilizado a->ant para passar pelo ultimo */
    i = 0;
    while (p != q && q != lst->cab->prox && i < lst->nFilmes)
    {
        if (strcmp( p->nome, q->nome) < 0)
        {
            if (strcmp(p->nome, min->nome) < 0)
            {
                min = p;
            }
            p = p->prox;
        }
        else
        {
            if (strcmp(p->nome, max->nome) > 0)
            {
                max = p;
            }

            /* aux é auxiliar para p poder seguir para o proximo da lista */
            aux = q;
            q = q->prox;

            anterior = aux->ant;
            proximo = aux->prox;

            /* PASSO 1 conecta o anterior e o proximo de q */
            aux->ant->prox = aux->prox;
            aux->prox->ant = aux->ant;

            /* PASSO 2 conecta q ao anterior de p */
            p->ant->prox = aux;
            aux->ant = p->ant;
            
            /* PASSO 3 conecta q à p */
            p->ant = aux;
            aux->prox = p;
        }
        i++;
    }

    lst->cab->prox = min;
    lst->cab->ant = max;
}

/* funcao que intercala as celulas para o mergesort para as notas*/
static void intercalaNota(ListaFilmes *lst, Filme *flmMeio, int meio)
{
    /*min e max são ponteiros que guardam o maximo e minimo do trecho verificado*
    /*vão ser usados para mudar a babeça ant e prox*/
    Filme *p, *q, *anterior, *proximo, *aux, *min, *max;
    ListaFilmes *lst1, *lst2;
    int i;

    p = lst->cab->prox;
    q = flmMeio;

    min = lst->cab->prox;
    max = lst->cab->ant;

    /* foi utilizado a->ant para passar pelo ultimo */
    i = 0;
    while (p != q && q != lst->cab->prox && i < lst->nFilmes)
    {
        if (p->nota < q->nota)
        {
            if (p->nota > min->nota)
            {
                min = p;
            }
            p = p->prox;
        }
        else
        {
            if (q->nota < max->nota)
            {
                min = q;
            }

            /* aux é auxiliar para p poder seguir para o proximo da lista */
            aux = q;
            q = q->prox;

            anterior = aux->ant;
            proximo = aux->prox;

            /* PASSO 1 conecta o anterior e o proximo de q */
            aux->ant->prox = aux->prox;
            aux->prox->ant = aux->ant;

            /**********************************/
            anterior = p->ant;

            /* PASSO 2 conecta q ao anterior de p */
            p->ant->prox = aux;
            aux->ant = p->ant;
            
            /* PASSO 3 conecta q à p */
            p->ant = aux;
            aux->prox = p;
        }
        i++;
    }

    lst->cab->prox = min;
    lst->cab->ant = max;
}



/*----------------------------------------------------------------------
 *  mergeSortFilmes
 *
 *  Recebe uma lista de filmes LST e ordena a lista utilizando o
 *  algoritmo mergeSort recursivo adaptado para listas encadeadas
 *  duplamente ligadas com cabeca.
 *
 *  A funcao recebe ainda um parametro CRITERIO tal que:
 *
 *  Se CRITERIO == NOTA, entao a lista deve ser ordenada
 *      em ordem decrescente de nota.
 *
 *  Se CRITERIO == NOME, entao a lista deve ser ordenada
 *      em ordem crescente de nome (ordem alfabetica).
 *
 *  ------------------------------------------------------------------
 *  OBSERVACAO IMPORTANTE:
 *
 *  A ordenacao deve ser feita 'in-place', ou seja o conteudo das
 *  celulas _nao deve_ ser copiado, apenas os ponteiros devem ser
 *  alterados.
 *
 *  A funcao so deve utilizar espaco extra O(1).  
 *
 *  Hmmm, ok, sem levar em consideracao o espaco O(lg n) utilizado
 *  pela pilha da recursao.  Em outras palavras, a funcao pode conter
 *  apenas declaracoes de umas poucas variaveis (um vetor v[0..n]
 *  conta como n variaveis e nao vale).
 *
 *  ------------------------------------------------------------------
 *  Para ordenar por nome, veja a funcao strCmp em util.[h|c].
 */
void 
mergeSortFilmes(ListaFilmes *lst, Criterio criterio)
{
    if (lst->nFilmes > 1)
    {
        int i, meio = 0, n = lst->nFilmes, anda = 0;

        ListaFilmes *lst1, *lst2;

        /* variavel para achar o filme que esta no meio da lista */
        Filme *aux, *cabecaEsquerda, *cabecaDireita, *filmeMeio;

        cabecaEsquerda = lst->cab->ant;

        aux = lst->cab->prox;
        filmeMeio = lst->cab->prox;
        /* "encontra" o filme do meio */
        while (aux != lst->cab->ant->ant)
        {
            aux = aux->prox;
            anda = (anda+1)%2;
            if (anda)
            {
                meio++;
                filmeMeio = filmeMeio->prox;
            }
        }

        /*cabecaEsquerda = lst->cab->ant;*/
        /* marca o final desta lista (intervalo que abrenje [ini;meio[ ) */
        /*lst->cab->prox = lst->cab->prox;*/
        lst->cab->ant = filmeMeio;
        lst->nFilmes = meio;
        mergeSortFilmes(lst, criterio);

        cabecaEsquerda = lst->cab->prox;
        /* marca o final desta lista (intervalo que abrenje [meio;fim] ) */
        lst->cab->prox = filmeMeio->prox;
        lst->cab->ant = cabecaEsquerda/*lst->cab->ant*/;
        lst->nFilmes = teto(lst->nFilmes);
        mergeSortFilmes(lst, criterio);

        lst->cab->prox = cabecaEsquerda/*lst1->cab->prox*/;
        /*lst->cab->ant = /*lst2->cab->ant;*/
        lst->nFilmes = n;
        /* faz a magia e ordena cada pedaço */
        if (criterio == NOTA)
        {
            intercalaNota(lst, filmeMeio, meio);
        }
        else if (criterio == NOME)
        {
            intercalaNome(lst, filmeMeio, meio);
        }
    }
}

/*static Filme ordena(ListaFilmes *lst)
{
    Filme *pivo, *p, *q, *r;
    pivo = lst->cab->ant;

    p = lst->cab->prox;
    q = p->prox;

    while (p != q)
    {
        if (p->nota < q->nota)
        {
            
        }
    }
}*/

/*----------------------------------------------------------------------
 *  quickSortFilmes [opcional]
 *
 *  Recebe uma lista de filmes LST e ordena a lista utilizando o
 *  algoritmo quickSort adaptado para listas encadeadas duplamente
 *  ligadas com cabeca.
 *
 *  A funcao recebe ainda um parametro CRITERIO tal que:
 * 
 *  Se CRITERIO == NOTA, entao a lista deve ser ordenada
 *      em ordem decrescente de nota.
 *
 *  Se CRITERIO == NOME, entao a lista deve ser ordenada
 *      em ordem crescente de nome (ordem alfabetica).
 *
 *  ------------------------------------------------------------------
 *  OBSERVACAO IMPORTANTE:
 *
 *  A ordenacao deve ser feita 'in-place', ou seja o conteudo das
 *  celulas _nao deve_ ser copiado, apenas os ponteiros devem ser
 *  alterados.
 *
 *  A funcao so deve utilizar espaco extra O(1).  
 *
 *  Hmmm, ok, sem levar em consideracao o espaco O(lg n) utilizado
 *  pela pilha da recursao.  Em outras palavras, a funcao pode conter
 *  apenas declaracoes de umas poucas variaveis (um vetor v[0..n]
 *  conta como n variaveis e nao vale).
 *
 *  ------------------------------------------------------------------
 *  Para ordenar por nome, veja a funcao strCmp em util.[h|c].
 */
void 
quickSortFilmes(ListaFilmes *lst, Criterio criterio)
{
    /*if (lst->cab->prox != lst->cab->ant)
    {
        Filme *flmMeio, *aux, *ini, *fim;
        int meio = flm->nFilmes/2;

        ini = lst->cab->prox;
        fim = lst->cab->ant;

        flmMeio = lst->cab->prox;
        for (int i = 0; i < meio; ++i)
        {
            flmMeio  = flmMeio>prox;
        }

        ordena(lst);

        lst->cab->ant = flmMeio;
        lst->nFilmes = meio;
        quickSortFilmes(lst, criterio);

        lst->cab->prox = flmMeio->prox;
        lst->cab->ant = fim;
        lst->nFilmes = teto(lst->nFilmes);
        quickSortFilmes(lst, criterio);

        lst->cab->prox = ini;
        lst->cab->ant = fim;
    }*/
    AVISO(quickSortFilmes em filmes.c: Vixe ainda nao fiz essa funcao...);
}

/*----------------------------------------------------------------------
 *  hashFilmes [opcional]
 *
 *  Recebe uma lista de filmes LST e distribui as palavras que
 *  ocorrem nos nomes do filmes em uma tabela de dispersao 
 *  (hash table):
 *
 *     http://www.ime.usp.br/~pf/mac0122-2002/aulas/hashing.html
 *     http://www.ime.usp.br/~pf/mac0122-2003/aulas/symb-table.html
 *
 *  Antes de inserir uma palavra na tabela de dispersao todas a
 *  letras da palavra devem ser convertidas para minusculo. Com
 *  isto faremos que a busca de filmes que possuam uma dada
 *  palavra em seu nome nao seja 'case insensitive'. Para essa
 *  tarefa a funcao tolower() pode ser utilizada.
 *
 *  Esta funcao utiliza as funcoes freeST(), initST() e putFilmeST()
 *
 *  Para obter as palavras podemos escrever uma funcao peguePalavra()
 *  inspirada na funcao pegueNome do modulo lexer.c do EP3/EP4 ou
 *  ainda utilizar a funcao strtok() da biblioteca string.h:
 *  
 *      http://linux.die.net/man/3/strtok    (man page)
 *
 */
void
hashFilmes(ListaFilmes *lst)
{
    AVISO(hashFilmes em filmes.c: Vixe ainda nao fiz essa funcao...);
}