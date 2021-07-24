#include <stdio.h>
#include <stdlib.h>    /* exit, malloc, calloc, etc. */
#include <string.h>
#include <getopt.h>    /* getopt */
#include "matriz.h"

#ifdef LIKWID_PERFMON
#include <likwid.h>
#else
#define LIKWID_MARKER_INIT
#define LIKWID_MARKER_THREADINIT
#define LIKWID_MARKER_SWITCH
#define LIKWID_MARKER_REGISTER(regionTag)
#define LIKWID_MARKER_START(regionTag)
#define LIKWID_MARKER_STOP(regionTag)
#define LIKWID_MARKER_CLOSE
#define LIKWID_MARKER_GET(regionTag, nevents, events, time, count)
#endif

/**
 * Exibe mensagem de erro indicando forma de uso do programa e termina
 * o programa.
 */

static void usage(char *progname)
{
  fprintf(stderr, "Forma de uso: %s [ -n <ordem> ] \n", progname);
  exit(1);
}



/**
 * Programa principal
 * Forma de uso: matmult [ -n <ordem> ]
 * -n <ordem>: ordem da matriz quadrada e dos vetores
 *
 */

int main (int argc, char *argv[]) 
{
  int c, n=DEF_SIZE;
  
  MatPtr mPtr_1, mPtr_2, resMatPtr;
  MatRow mRow_1, mRow_2, resMatRow;
  Vetor vet, resPtr, resRow;
  

  /* =============== TRATAMENTO DE LINHA DE COMANDO =============== */

  char *opts = "n:";
  c = getopt (argc, argv, opts);
  
  while ( c != -1 ) {
    switch (c) {
    case 'n':  n = atoi(optarg);              break;
    default:   usage(argv[0]);
    }
      
    c = getopt (argc, argv, opts);
  }
  
  /* ================ FIM DO TRATAMENTO DE LINHA DE COMANDO ========= */
 
  srand(20202);
      
  resPtr = (double *) calloc (n, sizeof(double));
  resRow = (double *) calloc (n, sizeof(double));
  resMatPtr = geraMatPtr(n, n, 1);
  resMatRow = geraMatRow(n, n, 1);
    
  mPtr_1 = geraMatPtr (n, n, 0);
  mPtr_2 = geraMatPtr (n, n, 0);

  mRow_1 = geraMatRow (n, n, 0);
  mRow_2 = geraMatRow (n, n, 0);

  vet = geraVetor (n, 0);
    
#ifdef DEBUG
    prnMatPtr (mPtr_1, n, n);
    prnMatPtr (mPtr_2, n, n);
    prnMatRow (mRow_1, n, n);
    prnMatRow (mRow_2, n, n);
    prnVetor (vet, n);
    printf ("=================================\n\n");
#endif /* DEBUG */


  LIKWID_MARKER_INIT;
  

  multMatPtrVet (mPtr_1, vet, n, n, resPtr);

  LIKWID_MARKER_START("MAT*VET");
  multMatRowVet (mRow_1, vet, n, n, resRow);
  LIKWID_MARKER_STOP("MAT*VET");

  for(int i=0; i<n; ++i)
    resRow[i] = 0;//resetando vetor resultante

  LIKWID_MARKER_START("MAT*VETotimizado_m=4");
  multMatRowVet_otimiz(mRow_1, vet, n, n, resRow);
  LIKWID_MARKER_STOP("MAT*VETotimizado_m=4");  

  multMatMatPtr (mPtr_1, mPtr_2, n, resMatPtr);

  LIKWID_MARKER_START("MAT*MAT");
  multMatMatRow (mRow_1, mRow_2, n, resMatRow);
  LIKWID_MARKER_STOP("MAT*MAT");

  for(int i=0; i<n; ++i)
    for(int j=0; j<n; ++j)
      resMatRow[i*n+j] = 0;//resetando matriz resultante

  LIKWID_MARKER_START("MAT*MATotimizado");
  multMatMatRow_otimiz(mRow_1, mRow_2, n, resMatRow);
  LIKWID_MARKER_STOP("MAT*MATotimizado");



#ifdef DEBUG
    prnVetor (resPtr, n);
    prnVetor (resRow, n);
    prnMatRow (resMatRow, n, n);
    prnMatPtr (resMatPtr, n, n);
#endif /* DEBUG */

  liberaMatPtr (mPtr_1, n);
  liberaMatPtr (mPtr_2, n);
  liberaMatPtr (resMatPtr, n);
  liberaVetor ((void*)mRow_1);
  liberaVetor ((void*)mRow_2);
  liberaVetor ((void*)resMatRow);
  liberaVetor ((void*)vet);
    
  free(resRow);
  free(resPtr);

  LIKWID_MARKER_CLOSE;
  return 0;
}

