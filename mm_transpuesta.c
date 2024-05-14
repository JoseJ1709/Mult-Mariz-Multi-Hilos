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
// Importa las bibliotecas necesarias
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
// Define el tamaño de los datos
#define DATA_SIZE (1024*1024*64*3) 
// Define el mutex para la exclusión mutua
pthread_mutex_t MM_mutex;
static double MEM_CHUNK[DATA_SIZE];
double *mA, *mB, *mC;
// Define la estructura de los parámetros
struct parametros{
	int nH;
	int idH;
	int N;
};
// Define las variables de tiempo
struct timeval start, stop;
// Llena la matriz con valores aleatorios
void llenar_matriz(int SZ){
    // Inicializa la semilla para los números aleatorios
	srand48(time(NULL));
    // Llena las matrices con valores aleatorios
	for(int i = 0; i < SZ*SZ; i++){
			mA[i] = 1.1*i; //drand48(); 
			mB[i] = 2.2*i; //drand48();
			mC[i] = 0; 
		}	
}
// Imprime la matriz
void print_matrix(int sz, double *matriz){
    // Verifica si el tamaño de la matriz es menor a 12
	if(sz < 12){
            // Imprime la matriz
    		for(int i = 0; i < sz*sz; i++){
     				if(i%sz==0) printf("\n");
            		printf(" %.3f ", matriz[i]);
			}	
    	printf("\n>-------------------->\n");
	}
}
// Inicializa el tiempo
void inicial_tiempo(){
    // Obtiene el tiempo actual
	gettimeofday(&start, NULL);
}
// Finaliza el tiempo
void final_tiempo(){
    // Obtiene el tiempo actual
	gettimeofday(&stop, NULL);
    // Calcula el tiempo transcurrido
	stop.tv_sec -= start.tv_sec;
    // Imprime el tiempo transcurrido
	printf("\n:-> %9.0f µs\n", (double) (stop.tv_sec*1000000 + stop.tv_usec));
}
// Función de multiplicación de matrices
void *mult_thread(void *variables){
    // Obtiene los parámetros de la función
	struct parametros *data = (struct parametros *)variables;
    // Obtiene los parámetros de la función
	int idH = data->idH;
	int nH  = data->nH;
	int N   = data->N;
	int ini = (N/nH)*idH;
	int fin = (N/nH)*(idH+1);
    // Realiza la multiplicación de matrices
    for (int i = ini; i < fin; i++){
        for (int j = 0; j < N; j++){
			double *pA, *pB, sumaTemp = 0.0;
			pA = mA + (i*N); 
			pB = mB + (j*N);
            for (int k = 0; k < N; k++, pA++, pB++){
				sumaTemp += (*pA * *pB);
			}
			mC[i*N+j] = sumaTemp;
		}
	}
    // Finaliza el hilo
	pthread_mutex_lock (&MM_mutex);
	pthread_mutex_unlock (&MM_mutex);
	pthread_exit(NULL);
}
// Función principal
int main(int argc, char *argv[]){
    // Verifica si se proporcionan suficientes argumentos de línea de comandos
	if (argc < 2){
		printf("Ingreso de argumentos \n $./ejecutable tamMatriz numHilos\n");
		return -1;	
	}
    // Analiza los argumentos de línea de comandos para el tamaño de la matriz y el número de hilos
    int SZ = atoi(argv[1]); 
    int n_threads = atoi(argv[2]); 
    // Arreglo de identificadores de hilo
    pthread_t p[n_threads];
    pthread_attr_t atrMM;
    // Asigna memoria para las matrices
	mA = MEM_CHUNK;
	mB = mA + SZ*SZ;
	mC = mB + SZ*SZ;
    // Llena las matrices con valores aleatorios
	llenar_matriz(SZ);
	print_matrix(SZ, mA);
	print_matrix(SZ, mB);
    // Inicializa el tiempo de ejecución
	inicial_tiempo();
    // Inicializa el mutex para la exclusión mutua
	pthread_mutex_init(&MM_mutex, NULL);
	pthread_attr_init(&atrMM);
	pthread_attr_setdetachstate(&atrMM, PTHREAD_CREATE_JOINABLE);
    // Crea los hilos para la multiplicación de matrices
    for (int j=0; j<n_threads; j++){
        // Asigna memoria para los datos del hilo
		struct parametros *datos = (struct parametros *) malloc(sizeof(struct parametros)); 
		datos->idH = j;
		datos->nH  = n_threads;
		datos->N   = SZ;
        // Crea el hilo y envia los datos
        pthread_create(&p[j],&atrMM,mult_thread,(void *)datos);
	}
    // Espera a que todos los hilos terminen
    for (int j=0; j<n_threads; j++)
        pthread_join(p[j],NULL);
	final_tiempo();
    // Imprime la matriz resultante
	print_matrix(SZ, mC);
    // Limpia los atributos del hilo y destruye el mutex
	pthread_attr_destroy(&atrMM);
	pthread_mutex_destroy(&MM_mutex);
	pthread_exit (NULL);
}
