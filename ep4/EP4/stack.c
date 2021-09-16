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

  Nome:

  stack.c
  Pitao II

  Referências: Com exceção das rotinas fornecidas no esqueleto e em sala
  de aula, caso você tenha utilizado alguma refência, liste-as abaixo
  para que o seu programa não seja considerada plágio.

  Exemplo:
  - função mallocc retirada de: 
 
  http://www.ime.usp.br/~pf/algoritmos/aulas/aloca.html

  \__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__
*/

/* interface para o uso de uma pilha */
#include <stdlib.h>
#include "stack.h" 
#include "util.h"

/* 
 * 
 * STACK.c: IMPLEMENTACAO DE UMA PILHA 
 *
 * TAREFA EP3
 *
 * Faca aqui a implementacao de uma pilha atraves de uma 
 * __lista encadeada com cabeca__. 
 * 
 * A implementacao deve respeitar a interface que voce declarar em 
 * stack.h. 
 *
 * TAREFA EP4: copiar a implementacao feira para o EP3.
 *     E possivel que sua implementacao para o EP3 deve ser 
 *     adaptada, ja que agora a pilha de execucao pode conter
 *     nomes de variaveis e nao apenas valores double.
 *
 */

Stack stackInit(int n){
    Stack ini = mallocSafe(sizeof *ini);
    
    ini->tp = NULL;

    return ini;
}   

int stackEmpty(Stack s)
{
    return s->tp == NULL;
}

void stackPush(Stack s, CelObjeto item)
{ 
    Link novo;
    novo = mallocSafe(sizeof *novo);

    novo->celObj = item;
    novo->prox = s->tp;

    s->tp = novo;
}

CelObjeto stackPop(Stack s)
{
    Link p = s->tp;
    CelObjeto conteudo = p->celObj;
    s->tp = p->prox;
    free(p);
    return conteudo;
}

CelObjeto stackTop(Stack s)
{
    return s->tp->celObj;
}

void stackFree(Stack s)
{
    while (s->tp != NULL)
    {
        Link p = s->tp;
        s->tp = p->prox;
        free(p);
    }
    
    free(s);
}

