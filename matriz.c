#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <likwid.h>
#include "matriz.h"

/**
 * Função que gera valores para para ser usado em uma matriz
 * @param i,j coordenadas do elemento a ser calculado (0<=i,j<n)
*  @return valor gerado para a posição i,j
  */
static inline double generateRandomA( unsigned int i, unsigned int j)
{
  static double invRandMax = 1.0 / (double)RAND_MAX;
  return ( (i==j) ? (double)(BASE<<1) : 1.0 )  * (double)rand() * invRandMax;
}

/**
 * Função que gera valores aleatórios para ser usado em um vetor
 * @return valor gerado
 *
 */
static inline double generateRandomB( )
{
  static double invRandMax = 1.0 / (double)RAND_MAX;
  return (double)(BASE<<2) * (double)rand() * invRandMax;
}



/* ----------- FUNÇÕES ---------------- */

/**
 *  Funcao geraMatPtr: gera matriz como vetor de ponteiros para as suas linhas
 * 
 *  @param m  número de linhas da matriz
 *  @param n  número de colunas da matriz
 *  @param zerar se 0, matriz  tem valores aleatórios, caso contrário,
 *               matriz tem valores todos nulos
 *  @return  ponteiro para a matriz gerada
 *
 */

MatPtr geraMatPtr (int m, int n, int zerar)
{
  MatPtr matriz = (double **) malloc(m * sizeof(double *));

  if (matriz) {
    for (int i=0; i < m; ++i) {
      if (matriz[i] = (double *) malloc(n * sizeof(double)))
        for (int j=0; matriz[i] && j < n; ++j) {
          if (zerar) matriz[i][j] = 0.0;
	  else       matriz[i][j] = generateRandomA(i, j);
	}
      else
          return NULL;
    }
  }
  
  return (matriz);
}

/**
 *  \brief: libera matriz alocada como vetor de ponteiros para as suas linhas
 * 
 *  @param  ponteiro para matriz
 *
 */
void liberaMatPtr (MatPtr matriz, int m)
{
  if (matriz) {
    for (int i=0; i < m; ++i) {
      free (matriz[i]);
    }
    free (matriz);
  }
}



/**
 *  Funcao geraMatRow: gera matriz como vetor único, 'row-oriented'
 * 
 *  @param m     número de linhas da matriz
 *  @param n     número de colunas da matriz
 *  @param zerar se 0, matriz  tem valores aleatórios, caso contrário,
 *               matriz tem valores todos nulos
 *  @return  ponteiro para a matriz gerada
 *
 */

MatRow geraMatRow (int m, int n, int zerar)
{
  MatRow matriz = (double *) malloc(m*n*sizeof(double));

  if (matriz) {
    for (int i=0; i < m; ++i) {
      for (int j=0; j < n; ++j) {
	      if (zerar) matriz[i*m + j] = 0.0;
          else       matriz[i*m + j] = generateRandomA(i, j);
      }
    }
  }
  
  return (matriz);
}


/**
 *  Funcao geraVetor: gera vetor de tamanho 'n'
 * 
 *  @param n  número de elementos do vetor
 *  @param zerar se 0, vetor  tem valores aleatórios, caso contrário,
 *               vetor tem valores todos nulos
 *  @return  ponteiro para vetor gerado
 *
 */

Vetor geraVetor (int n, int zerar)
{
  Vetor vetor = (double *) malloc(n*sizeof(double));

  if (vetor)
    for (int i=0; i < n; ++i) {
      if (zerar) vetor[i] = 0.0;
      else       vetor[i] = generateRandomB();
    }
  
  return (vetor);
}

/**
 *  \brief: libera vetor
 * 
 *  @param  ponteiro para vetor
 *
 */
void liberaVetor (void *vet)
{
	free(vet);
}


/**
 *  Funcao multMatPtrVet:  Efetua multiplicacao entre matriz 'mxn' por vetor
 *                       de 'n' elementos
 *  @param mat matriz 'mxn'
 *  @param m número de linhas da matriz
 *  @param n número de colunas da matriz
 *  @param  res vetor  que guarda  o resultado.  Deve estar  previamente
 *              alocado e com seus elementos inicializados em 0.0 (zero)
 *  @return vetor de 'm' elementos
 *
 */

void multMatPtrVet (MatPtr mat, Vetor v, int m, int n, Vetor res)
{
    
  /* Efetua a multiplicação */
  if (res) {
    for (int i=0; i < m; ++i)
      for (int j=0; j < n; ++j)
        res[i] += mat[i][j] * v[j];
  }
}


/**
 *  Funcao multMatRowVet:  Efetua multiplicacao entre matriz 'mxn' por vetor
 *                       de 'n' elementos
 *  @param mat matriz 'mxn'
 *  @param m número de linhas da matriz
 *  @param n número de colunas da matriz
 *  @param res vetor que guarda o resultado. Deve estar previamente alocado e com
 *             seus elementos inicializados em 0.0 (zero)
 *  @return vetor de 'm' elementos
 *
 */

void multMatRowVet (MatRow mat, Vetor v, int m, int n, Vetor res)
{
    
  /* Efetua a multiplicação */
  if (res) {
    for (int i=0; i < m; ++i)
      for (int j=0; j < n; ++j)
          res[i] += mat[m*i + j] * v[j];
  }


}

void multMatRowVet_otimiz (MatRow mat, Vetor v, int m, int n, Vetor res){
  //usando loop unroll & jam com m=4
  //usando loop blocking com b=4
  for(int i=0; i<m; ++i)
    res[i] = 0;//zerando pois foi alterado em multMatRowVet
  
  for(int ii=0; ii<m/4; ++ii){
    int istart=ii*4;
    int iend = istart + 4;
    for(int jj=0; jj<n/4; ++jj){
      int jstart = jj*4;
      int jend = jstart + 4;
      for(int i=istart; i<iend; i+=4)
        for(int j=jstart; j<jend; ++j){
          res[i] = res[i] + mat[m*i + j] * v[j];
          res[i+1] = res[i+1] + mat[m*(i+1) + j] * v[j];
          res[i+2] = res[i+2] + mat[m*(i+2) + j] * v[j];
          res[i+3] = res[i+3] + mat[m*(i+3) + j] * v[j];
        }
    }
  }


}


/**
 *  Funcao multMatMatPtr: Efetua multiplicacao de duas matrizes 'n x n' 
 *  @param A matriz 'n x n'
 *  @param B matriz 'n x n'
 *  @param n ordem da matriz quadrada
 *  @param C   matriz que guarda o resultado. Deve ser previamente gerada com 'geraMatPtr()'
 *             e com seus elementos inicializados em 0.0 (zero)
 *
 */

void multMatMatPtr (MatPtr A, MatPtr B, int n, MatPtr C)
{

  /* Efetua a multiplicação */
  for (int i=0; i < n; ++i)
    for (int j=0; j < n; ++j)
      for (int k=0; k < n; ++k)
	C[i][j] += A[i][k] * B[k][j];
}




/**
 *  Funcao multMatMatPtr: Efetua multiplicacao de duas matrizes 'n x n' 
 *  @param A matriz 'n x n'
 *  @param B matriz 'n x n'
 *  @param n ordem da matriz quadrada
 *  @param C   matriz que guarda o resultado. Deve ser previamente gerada com 'geraMatPtr()'
 *             e com seus elementos inicializados em 0.0 (zero)
 *
 */

void multMatMatRow (MatRow A, MatRow B, int n, MatRow C)
{

  /* Efetua a multiplicação */
  for (int i=0; i < n; ++i)
    for (int j=0; j < n; ++j)
      for (int k=0; k < n; ++k)
	C[i*n+j] += A[i*n+k] * B[k*n+j];
}


void multMatMatRow_otimiz(MatRow A, MatRow B, int n, MatRow C){
  
  for(int i=0; i<n; ++i)
    for(int j=0; j<n; ++j)
      C[i*n+j] = 0;


  for(int ii=0; ii<n/4; ++ii){
    int istart=ii*4;
    int iend = istart + 4;
    for(int jj=0; jj<n/4; ++jj){
       int jstart = jj*4;
       int jend = jstart + 4;
       for(int kk=0; kk<n/4; ++kk){
         int kstart = kk*4;
         int kend = kstart + 4;
         for(int i=istart; i<iend; ++i)
            for(int j=jstart; j<jend; j+=4){
              for(int k=kstart; k<kend; ++k){
                C[i*n+j] += A[i*n+k] * B[k*n+j];
                C[i*n+j+1] += A[i*n+k] * B[k*n+j+1];
                C[i*n+j+2] += A[i*n+k] * B[k*n+j+2];
                C[i*n+j+3] += A[i*n+k] * B[k*n+j+3];
              }              
            }
       }
    }
  }

}


/**
 *  Funcao prnMatPtr:  Imprime o conteudo de uma matriz em stdout
 *  @param mat matriz
 *  @param m número de linhas da matriz
 *  @param n número de colunas da matriz
 *
 */

void prnMatPtr (MatPtr mat, int m, int n)
{
  for (int i=0; i < m; ++i) {
    for (int j=0; j < n; ++j)
      printf(DBL_FIELD, mat[i][j]);
    printf("\n");
  }
  printf(SEP_RES);
}

/**
 *  Funcao prnMatRow:  Imprime o conteudo de uma matriz em stdout
 *  @param mat matriz
 *  @param m número de linhas da matriz
 *  @param n número de colunas da matriz
 *
 */

void prnMatRow (MatRow mat, int m, int n)
{
  for (int i=0; i < m; ++i) {
    for (int j=0; j < n; ++j)
      printf(DBL_FIELD, mat[m*i + j]);
    printf("\n");
  }
  printf(SEP_RES);
}

/**
 *  Funcao prnVetor:  Imprime o conteudo de vetor em stdout
 *  @param vet vetor com 'n' elementos
 *  @param n número de elementos do vetor
 *
 */

void prnVetor (Vetor vet, int n)
{
  for (int i=0; i < n; ++i)
    printf(DBL_FIELD, vet[i]);
  printf(SEP_RES);
}

