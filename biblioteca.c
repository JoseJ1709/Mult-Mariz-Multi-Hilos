/**************************************************************
		Pontificia Universidad Javeriana
	Autor: Jose Andres Jaramillo
	Fecha: 7 Mayo 2024
	Materia: Sistemas Operativos
	Tema: Taller de Evaluación de Rendimiento
	Fichero: funciones.c es un archivo que implementa las funciones y
             la logica dentro de ellas para las funciones que se utilizaran en el programa principal.
	Objetivo: Evaluar el tiempo de ejecución del
		      algoritmo clásico de multiplicación de matrices.
			  Se implementa con la Biblioteca POSIX Pthreads.
****************************************************************/
// Importando las librerias requeridas para el desarrollo del programa//
#include "biblioteca.h"
// Implementacion de funcion para llenar matrices //
void llenar_matriz(int SZ){
    // Inicializando la semilla para el generador de numeros aleatorios
    srand48(time(NULL));
    // Llenando las matrices
    for(int i = 0; i < SZ*SZ; i++){
        mA[i] = 1.1*i;
        mB[i] = 2.2*i;
        mC[i] = 0;
    }
}
// Implementacion de funcion para imprimir matrices //
void print_matrix(int sz, double *matriz){
    // Condicional para imprimir la matriz
    if(sz < 12){
        // Ciclo para imprimir la matriz
        for(int i = 0; i < sz*sz; i++){
            if(i%sz==0) printf("\n");
            printf(" %.3f ", matriz[i]);
        }
    }
    printf("\n>-------------------->\n");
}
// Implementacion de funcion para inicializar el tiempo //
void inicial_tiempo(){
    // Inicializando el tiempo
    gettimeofday(&start, NULL);
}
// Implementacion de funcion para finalizar el tiempo //
void final_tiempo(){
    // Finalizando el tiempo
    gettimeofday(&stop, NULL);
    // Calculando el tiempo transcurrido
    stop.tv_sec -= start.tv_sec;
    // Imprimiendo el tiempo transcurrido
    printf("\n:-> %9.0f µs\n", (double) (stop.tv_sec*1000000 + stop.tv_usec));
}

// Implementacion de funcion para multiplicar matrices //
void *mult_thread(void *variables){
    // Casting de la estructura de parametros
    struct parametros *data = (struct parametros *)variables;
    // Obteniendo los parametros de la estructura
    int idH = data->idH;
    int nH  = data->nH;
    int N   = data->N;
    int ini = (N/nH)*idH;
    int fin = (N/nH)*(idH+1);
    // Ciclo para multiplicar las matrices
    for (int i = ini; i < fin; i++){
        for (int j = 0; j < N; j++){
            double *pA, *pB, sumaTemp = 0.0;
            pA = mA + (i*N);
            pB = mB + j;
            for (int k = 0; k < N; k++, pA++, pB+=N){
                sumaTemp += (*pA * *pB);
            }
            mC[i*N+j] = sumaTemp;
        }
    }
    // Bloqueo de la seccion critica
    pthread_mutex_lock (&MM_mutex);
    pthread_mutex_unlock (&MM_mutex);
    // Salida del hilo
    pthread_exit(NULL);
}