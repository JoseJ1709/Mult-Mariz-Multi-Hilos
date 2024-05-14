/**************************************************************
		Pontificia Universidad Javeriana
	Autor: Jose Andres Jaramillo
	Fecha: 7 Mayo 2024
	Materia: Sistemas Operativos
	Tema: Taller de Evaluación de Rendimiento
	Fichero: fuente de multiplicación de matrices NxN por hilos.
	Objetivo: Evaluar el tiempo de ejecución del
		      algoritmo clásico de multiplicación de matrices.
			  Se implementa con la Biblioteca POSIX Pthreads.
****************************************************************/
// Importando las librerias requeridas para el desarrollo del programa//
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
// Definiendo el tamaño de la matriz a utilizar o reseva de memoria//
#define DATA_SIZE (1024*1024*64*3)
// --- Definiendo las variables globales a utilizar ---//
// Variable para el mutex
pthread_mutex_t MM_mutex;
// Variable para la reserva de memoria
static double MEM_CHUNK[DATA_SIZE];
// Variables para las matrices
double *mA, *mB, *mC;
// Estructura para los parametros de los hilos //
struct parametros{
    int nH;
    int idH;
    int N;
};
// Variables para el tiempo //
struct timeval start, stop;

// funcion para llenar matrices //
void llenar_matriz(int SZ){
    srand48(time(NULL));
    for(int i = 0; i < SZ*SZ; i++){
        mA[i] = 1.1*i; //drand48();
        mB[i] = 2.2*i; //drand48();
        mC[i] = 0;
    }
}
// Funcion para imprimir matrices //
void print_matrix(int sz, double *matriz){
    // Condicional para imprimir la matriz
    if(sz < 12){
        for(int i = 0; i < sz*sz; i++){
            if(i%sz==0) printf("\n");
            printf(" %.3f ", matriz[i]);
        }
    }
    printf("\n>-------------------->\n");
}
// Funcion para inicializar el tiempo //
void inicial_tiempo(){
    gettimeofday(&start, NULL);
}
// Funcion para finalizar el tiempo //
void final_tiempo(){
    gettimeofday(&stop, NULL);
    stop.tv_sec -= start.tv_sec;
    // Imprimir el tiempo de ejecucion
    printf("\n:-> %9.0f µs\n", (double) (stop.tv_sec*1000000 + stop.tv_usec));
}
// Funcion para multiplicar matrices //
void *mult_thread(void *variables){
    // Casting de la estructura de parametros
    struct parametros *data = (struct parametros *)variables;
    // Variables de la estructura
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
    // Salida del hilo
    pthread_mutex_lock (&MM_mutex);
    pthread_mutex_unlock (&MM_mutex);
    pthread_exit(NULL);
}
// Funcion principal //
int main(int argc, char *argv[]){
    // Condicional para verificar los argumentos
    if (argc < 2){
        printf("Ingreso de argumentos \n $./ejecutable tamMatriz numHilos\n");
        return -1;
    }
    // Variables para el tamaño de la matriz y el numero de hilos
    int SZ = atoi(argv[1]);
    int n_threads = atoi(argv[2]);
    // Variables para los hilos
    pthread_t p[n_threads];
    pthread_attr_t atrMM;
    // Reserva de memoria para las matrices
    mA = MEM_CHUNK;
    mB = mA + SZ*SZ;
    mC = mB + SZ*SZ;
    // Llenar las matrices
    llenar_matriz(SZ);
    print_matrix(SZ, mA);
    print_matrix(SZ, mB);
    // Inicializar el tiempo
    inicial_tiempo();
    pthread_mutex_init(&MM_mutex, NULL);
    pthread_attr_init(&atrMM);
    pthread_attr_setdetachstate(&atrMM, PTHREAD_CREATE_JOINABLE);
    // Crear los hilos y enviar los parametros con la funcion
    for (int j=0; j<n_threads; j++){
        struct parametros *datos = (struct parametros *) malloc(sizeof(struct parametros));
        datos->idH = j;
        datos->nH  = n_threads;
        datos->N   = SZ;
        pthread_create(&p[j],&atrMM,mult_thread,(void *)datos);
    }
    // Esperar a que los hilos terminen
    for (int j=0; j<n_threads; j++)
        pthread_join(p[j],NULL);
    final_tiempo();
    // Imprimir las matrices
    print_matrix(SZ, mC);
    // Finalizar los hilos
    pthread_attr_destroy(&atrMM);
    pthread_mutex_destroy(&MM_mutex);
    pthread_exit (NULL);
}