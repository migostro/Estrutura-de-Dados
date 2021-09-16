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

  posfixa.c
  Pitao II

  Referências: Com exceção das rotinas fornecidas no esqueleto e em sala
  de aula, caso você tenha utilizado alguma refência, liste-as abaixo
  para que o seu programa não seja considerada plágio.
  Exemplo:

  - função mallocc retirada de: 

  http://www.ime.usp.br/~pf/algoritmos/aulas/aloca.html

  \__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__
*/

/*
  NAO EDITE OU MODIFIQUE NADA QUE ESTA ESCRITO NESTE ESQUELETO
*/

/*------------------------------------------------------------*/
/* iterface para o uso da funcao deste módulo */
#include "posfixa.h"

/*------------------------------------------------------------*/
#include "categorias.h" /* Categoria, MAX_OPERADORES, INDEFINIDA, 
                           ABRE_PARENTESES, ... */
#include "objetos.h" /* tipo CelObjeto, freeObjeto(), freeListaObjetos() */
#include "stack.h"   /* stackInit(), stackFree(), stackPop() 
                        stackPush(), stackTop() */
#include "util.h"

/* Cria uma nova celulaObjeto e adiciona na lista posfixa */
static void *criaCelulaObjeto(CelObjeto celObj, CelObjeto *ultimo)
{

    CelObjeto *novo;
    novo = mallocSafe(sizeof *novo);

    novo->categoria = celObj.categoria;

    if (celObj.categoria == FLOAT)
    {
       novo->valor.vFloat = celObj.valor.vFloat;
    }
    else if (celObj.categoria == ID)
    {
        novo->valor.vInt = celObj.valor.vInt;
    } 
    else
    {
        novo->valor.pStr = celObj.valor.pStr;
    }
    
    novo->prox = NULL;

    ultimo->prox = novo;

    return novo;
}

/*-------------------------------------------------------------
 *  infixaParaPosfixa
 * 
 *  Recebe uma lista ligada com cabeca INIINFIXA representando uma
 *  fila de itens de uma expressao infixa e RETORNA uma lista ligada
 *  com cabeca contendo a fila com a representacao da correspondente
 *  expressao em notacao posfixa.
 */
 /*  As celulas da notacao infixa que nao forem utilizadas na notacao 
  *  posfixa (abre e fecha parenteses) devem ser liberadas 
  *  (freeObjeto()).
  */
CelObjeto *
infixaParaPosfixa(CelObjeto *iniInfixa)
{
    /*p e q percorre a lista infixa*/
    /*a e b percorrem e guardam na for posfixa*/
    CelObjeto *p, *iniPosfixa, *a;
    /*pilha é apenas para armazenar os categorias == FLOAT*/
    /*pilhaOper é serve apenas para armazenar operadores*/
    /*pilhaOperAux armazena os operadores que atuam da direita para a esquerda*/
    Stack *inipilha, pilha, pilhaOper;

    iniPosfixa = mallocSafe(sizeof *iniPosfixa);

    a = iniPosfixa;

    /*percorre a lista da expressão infixa*/
    p = iniInfixa->prox;

    inipilha = stackInit(0);
    pilha = inipilha;
    pilhaOper = stackInit(1);

    while (p != NULL)
    {
        if (p->categoria == FLOAT)
        {
            a = criaCelulaObjeto(*p, a);
        }
        /*else if (p->categoria == ABRE_PARENTESES)
        {
            stackPush(pilha, *p);
        }
        /*operadores que vão da direita para a esquerda com precedência 8 que precisa de 2 floats*/
        /*operadores que vão da direita para a esquerda com precedência 8 que precisa de 1 float*/
        /*operador que vai da direita para a esquerda com precedência 1*/
        else if (p->categoria == OPER_EXPONENCIACAO || p->categoria == OPER_MENOS_UNARIO || p->categoria == OPER_LOGICO_NOT || p->categoria == OPER_ATRIBUICAO)
        {
            if (stackEmpty(pilha) || stackTop(pilha).valor.vInt <= p->valor.vInt)
            {
                stackPush(pilhaOper, *p);
            }
            else
            {
                while (!stackEmpty(pilha))
                {
                    a = criaCelulaObjeto(stackPop(pilha), a);
                }

                while (!stackEmpty(pilhaOper))
                {
                    a = criaCelulaObjeto(stackPop(pilhaOper), a);

                }
                /*empilha depois de desempilhar as fita tudo*/
                stackPush(pilhaOper, *p);
            }
        }
        /*parenteses*/
        /*else if (p->categoria == FECHA_PARENTESES)
        {
            while (stackTop(pilha).categoria != ABRE_PARENTESES)
            {
                a = criaCelulaObjeto(stackPop(pilha), a);
            }
        }
        /*restante dos operadores que vao da esquerda para a direita*/
        else
        {
            if (stackEmpty(pilha) || stackTop(pilha).valor.vInt <= p->valor.vInt)
            {
                stackPush(pilha, *p);
            }
            else
            {
                while (!stackEmpty(pilha))
                {
                    a = criaCelulaObjeto(stackPop(pilha), a);
                }

                while (!stackEmpty(pilhaOper))
                {
                    a = criaCelulaObjeto(stackPop(pilhaOper), a);

                }
                /*empilha depois de desempilhar as fita tudo*/
                stackPush(pilha, *p);
            }
        }
        p = p->prox;
    }
    while (!stackEmpty(pilha))
    {
        a = criaCelulaObjeto(stackPop(pilha), a);
    }

    while (!stackEmpty(pilhaOper))
    {
        a = criaCelulaObjeto(stackPop(pilhaOper), a);
    }

    return iniPosfixa; 
}
