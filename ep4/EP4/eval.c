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

  Nome:Miguel Pereira Ostrowski

  eval.c
  Pitao II

  Referências: Com exceção das rotinas fornecidas no esqueleto e em sala
  de aula, caso você tenha utilizado alguma refência, liste-as abaixo
  para que o seu programa não seja considerada plágio.
  Exemplo:
  - função mallocc retirada de: http://www.ime.usp.br/~pf/algoritmos/aulas/aloca.html

  \__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__
*/

/*
  NAO EDITE OU MODIFIQUE NADA QUE ESTA ESCRITO NESTE ESQUELETO
*/

/*------------------------------------------------------------*/
/* interface para as funcoes deste modulo */
#include "eval.h" 

/*------------------------------------------------------------*/
#include <stdlib.h>  /* atoi(), atof(), strtol() */
#include <string.h>  /* strncmp(), strlen(), strncpy(), strcpy(), strcmp() */
#include <math.h>    /* pow() */

/*------------------------------------------------------------*/
#include "categorias.h" /* Categoria, MAX_OPERADORES */
#include "util.h"       /* ERRO(), AVISO(), mallocSafe() */
#include "objetos.h"    /* CelObjeto, mostreObjeto(), freeObjeto()
                           mostreListaObjetos(), 
                           freeListaObjetos() */
#include "stack.h"      /* funcoes que manipulam uma pilha */ 
                        /* essa pilha sera usada para implementar 
                           a pilha de execucao */
#include "st.h"         /* getValorST(), setValorST() */    
    
#define MAX_TAMANHO 128 
/*------------------------------------------------------------*/
/* Protipos de funcoes auxiliares */

/*------------------------------------------------------------*/
/* Tabela com uma representacao da precedencia dos operadores
 * atraves de valores inteiros. 
 * Quanto maior o valor, maior o valor, maior a precedencia.
 *
 *  http://www.ime.usp.br/~pf/algoritmos/apend/precedence.html
 */
static const int precedencia[MAX_OPERADORES] =
{
    /* 4 operadores relacionais com 2 simbolos  */
    4   /* "==" */ /* da esquerda para a direita */
    ,4   /* "!=" */ /* da esquerda para a direita */
    ,5   /* ">=" */ /* da esquerda para a direita */
    ,5   /* "<=" */ /* da esquerda para a direita */
         
    /* 2 operadores aritmeticos com 2 simbolos */
    ,8 /* ,"**" */ /* da direita para a esquerda */
    ,7 /* ,"//" */ /* da esquerda para a direita */
 
    /* 2 operadores relacionais com 1 simbolo */
    ,5  /* ">"  */ /* da esquerda para a direita */
    ,5  /* "<"  */ /* da esquerda para a direita */ 
    
    /* 6 operadores aritmeticos */
    ,7  /* "%" */ /* da esquerda para a direita */
    ,7  /* "*" */ /* da esquerda para a direita */
    ,7  /* "/" */ /* da esquerda para a direita */
    ,6  /* "+" */ /* da esquerda para a direita */
    ,6  /* "-" */ /* da esquerda para a direita */
    ,8  /* "_" */ /* da direita para a esquerda */
    
    /* 3 operadores logicos  */
    ,3  /* "and" */ /* da esquerda para a direita */ 
    ,2  /* "or" */ /* da esquerda para a direita */
    ,8  /* "not"  */ /* da direita para a esquerda */
    
    /* operador de indexacao */
    ,9  /* "$"  */ /* da esquerda para a direita (IGNORAR)*/

    /* atribuicao */ 
    ,1  /* "=" */ /* da direita para a esquerda EP4 */ 
}; 

/*-------------------------------------------------------------
 *  itensParaValores
 *  
 *  RECEBE uma lista ligada com cabeca INIFILAITENS representando uma
 *  fila de itens lexicos. Inicialmente, o campo VALOR de cada celula
 *  contem um string com o item lexico.
 *
 *  A funca CONVERTE o campo VALOR de cada celula para um inteiro 
 *  ou double, como descrito a seguir. 
 *
 *  IMPLEMENTACAO
 *
 *  O campo VALOR de uma CelObjeto tem 3 subcampos:
 *
 *      - valor.vInt   (int)
 *      - valor.vFloat (float)
 *      - valor.pStr   (string)
 *
 *  Nessa conversao, o campo VALOR de cada celula recebe um valor 
 *  da seguinte maneira.
 *
 *     - Se o campo CATEGORIA da celula indica que o item e um
 *       string representando um int (INT_STR) entao o campo 
 *       valor.vFloat deve receber esse numero inteiro. 
 *
 *       Nesse caso a CATEGORIA do item deve ser alterada para 
 *       FLOAT.
 *
 *     - se o campo CATEGORIA da celula indica que o item e um
 *       string representando um float (FLOAT_STR) entao o campo 
 *       valor.vFloat deve receber esse float;
 *
 *       Nesse caso a CATEGORIA do item deve ser alterada para 
 *       FLOAT.
 *
 *     - se o campo CATEGORIA da celula indica que o item e um
 *       string representando um Bool (BOOL_STR) entao o campo 
 *       valor.vFloat deve receber o inteiro correspondente 
 *       (False = 0, True = 1);
 *
 *       Nesse caso a CATEGORIA do item deve ser alterada para 
 *       FLOAT.
 *
 *     - se o campo CATEGORIA da celula indica que o item e um
 *       string representando um operador (OPER_*) entao o campo 
 *       valor.vInt deve receber o inteiro correspondente 
 *       a precedencia desse operador. 
 *       
 *       Para isto utilize o vetor PRECEDENCIA declarado antes 
 *       desta funcao. 
 *
 * Nesta funcao (e nas outras) voce pode utilizar qualquer funcao da
 * biblioteca string do C, como, por exemplo, atoi(), atof().
 *
 * Esta funcao (e todas as outras) devem 'dar' free nas estruturas que
 * deixarem de ser necessarias.
 *
 * Esse e o caso das dos strings dos itens das categorias 
 * FLOAT_STR e INT_STR.  
 *
 * Os campos strings de objetos OPER_* e BOOL_STR apontam para
 * strings em tabelas definidas no modulo lexer.h. Nesse
 * caso, tentar liberar essa memoria e' um erro.
 *
 */

void
itensParaValores(CelObjeto *iniFilaItens)
{
    CelObjeto *p, *q;

    p = iniFilaItens;
    q = p->prox;

    while (q != NULL)
    {
        /* Este if e else partem do principio que as categorias estão certas */
        if (q->categoria == FLOAT_STR || q->categoria == INT_STR)
        {
            q->valor.vFloat = atof(q->valor.pStr);
            q->categoria = FLOAT;
        }
        else 
        {
            q->valor.vInt = precedencia[q->categoria];
        }

        p = q;
        q = p->prox;
    }
}

/* calcula a expressão entre dois numeros a partir da categoria */
/* recebe o valor de cada variavel em float */
/* no caso no OPER_LOGICO_NOT apenas a é ultilizado e b é ignorado*/
static double calcula(Categoria categoria, double a, double b)
{
    /* OPERADORES RELACIONAIS */
    if (categoria == OPER_IGUAL)
    {
        if (a == b)
        {
            return 1;
        }
        return 0;
    }
    /*else if (categoria == OPER_ATRIBUICAO)
    {
        return b;
    }*/
    else if (categoria == OPER_DIFERENTE)
    {
        if (a != b)
        {
            return 1;
        }
        return 0;
    }
    else if (categoria == OPER_MAIOR_IGUAL)
    {
        if (a >= b)
        {
            return 1;
        }
        return 0;
    }
    else if (categoria == OPER_MENOR_IGUAL)
    {
        if (a <= b)
        {
            return 1;
        }
        return 0;
    }
    /* OPERADORES ARITIMETICOS */
    else if (categoria == OPER_EXPONENCIACAO)
    {
        return pow(b,a);
    }
    else if (categoria == OPER_DIVISAO_INT)
    {
        int res = a/b;
        return res;
    }
    else if (categoria == OPER_MAIOR)
    {
        if (a > b)
        {
            return 1;
        }
        return 0;
    }
    else if (categoria == OPER_MENOR)
    {
        if (a < b)
        {
            return 1;
        }
        return 0;
    }
    else if (categoria == OPER_RESTO_DIVISAO)
    {
        while (b-a >= 0)
        {
            b = b-a;
        }
        return b;
    }
    else if (categoria == OPER_MULTIPLICACAO)
    {
        return a*b;
    }
    else if (categoria == OPER_DIVISAO)
    {
        return b/a;
    }
    else if (categoria == OPER_ADICAO)
    {
        return a+b;
    }
    else if (categoria == OPER_SUBTRACAO)
    {
        return b-a;
    }
    else if (categoria == OPER_MENOS_UNARIO)
    {
        return (-1)*a;
    }
    else if (categoria == OPER_LOGICO_AND)
    {
        if (a != 0 && b != 0)
        {
            return 1;
        }
        return 0;
    }
    else if (categoria == OPER_LOGICO_OR)
    {
        if (a != 0 || b != 0)
        {
            return 1;
        }
        return 0;
    }
    else if (categoria == OPER_LOGICO_NOT)
    {
        if (a == 0)
        {
            return 1;
        }
        return 0;
    }
}

static void mostraPilha(double v[MAX_TAMANHO], int j)
{
    int i;
    j--;

    printf("==========================\n");
    printf("  Pilha de execucao\n");
    printf("  valor\n");
    printf(". . . . . . . . . . . . . . .\n");

    for (i = j; i >= 0; --i)
    {
        printf("  %f\n", v[i]);
    }
    printf("\n\n");
}

/*-------------------------------------------------------------
 * eval
 * 
 * RECEBE uma lista ligada como cabeca INIPOSFIXA que representa
 * uma expressao em notacao posfixa. 
 *
 * RETORNA o endereco de uma celula com o valor da expressao.
 *
 * PRE-CONDICAO: a funcao supoe que a expressao esta sintaticamente
 *               correta.
 *
 * IMPLEMENTACAO
 *
 * Para o calculo do valor da expressao deve ser usada uma 
 * pilha. O endereco retornado sera o da celula no topo no
 * topo dessa  "pilha de execucao".
 * 
 * A funcao percorre a expressao calculando os valores resultantes.
 * Para isto e utilizada uma pilha de execucao. 
 * 
 * A implementacao das funcoes que manipulam a pilham devem ser
 * escritas no modulo stack.c.
 * 
 * O arquivo stack.h contem a interface para essas funcoes. 
 * A pilha de execucao so deve ser acessada atraves dessa interface
 * (em caso contrario nao chamariamos stack.h de interface).
 * 
 * O parametro mostrePilhaExecucao indica se os valores
 * na pilha de execucao devem ser exibidos depois de qualquer 
 * alteracao.
 * 
 * Esta funcao deve "dar free" nas estruturas que deixarem de ser 
 * necessarias.
 *
 * EP4: funcao deve ser adaptada para tratar do operador de atribuicao
 *      '=' e variaveis. A maneira de proceder esta descrita no 
 *      enunciado na secao "Interpretacao da expressao posfixa".
 */
CelObjeto *
eval (CelObjeto *iniPosfixa, Bool mostrePilhaExecucao)
{
    int j = 0;
    /*vetor que guarda as informações da pilha de execução */
    /*utilizado apenas se mostrePilhaExecucao == TRUE*/
    double v[MAX_TAMANHO];
    CelObjeto *p, *q, aux1, aux2;

    Stack *iniPilha1, pilha1;

    p = iniPosfixa;
    q = p->prox;

    /* pilha pricipal */
    iniPilha1 = stackInit(1);

    pilha1 = iniPilha1;

    while (q != NULL)
    {
        /* se for um numero */
        if (q->categoria == FLOAT)
        {
            stackPush(pilha1, *q);
            v[j++] = q->valor.vFloat;

            if (mostrePilhaExecucao)
            {
                mostraPilha(v, j);
            }
            p = q;
            q = p->prox;
        }
        else{            
            /* calcula os com os operadores que vão da esquerda para a direita */
            while (q != NULL && q->categoria != FLOAT)
            {
                if (q->categoria == OPER_LOGICO_NOT || q->categoria == OPER_MENOS_UNARIO)
                {
                    /* numero real da pilha 1 */
                    aux2 = stackPop(pilha1);
                    j--;


                    aux2.valor.vFloat = calcula(q->categoria, aux2.valor.vFloat, 0);

                    stackPush(pilha1, aux2);
                    v[j++] = aux2.valor.vFloat;

                    p = q;
                    q = p->prox;

                }
                else
                {
                    /* numero tipo float (double) */
                    aux1 = stackPop(pilha1);
                    aux2 = stackPop(pilha1);
                    j--;
                    j--;
                    
                    aux2.valor.vFloat = calcula(q->categoria, aux1.valor.vFloat, aux2.valor.vFloat);

                    stackPush(pilha1, aux2);
                    v[j++] = aux2.valor.vFloat;
                    
                    p = q;
                    q = p->prox;
                }
                
                if (mostrePilhaExecucao)
                {
                    mostraPilha(v, j);
                }
            }
        }
    }  
    return pilha1->tp;
}
