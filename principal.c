/**************************************************************
		Pontificia Universidad Javeriana
	Autor: Jose Andres Jaramillo
	Fecha: 7 Mayo 2024
	Materia: Sistemas Operativos
	Tema: Taller de Evaluación de Rendimiento
	Fichero: Fuente_Evaluacion.c es un archivo que implementa las funciones y logica para la multiplicacion y impresion de matrices.
	Objetivo: Evaluar el tiempo de ejecución del
		      algoritmo clásico de multiplicación de matrices.
			  Se implementa con la Biblioteca POSIX Pthreads.
****************************************************************/
// Importando las librerias requeridas para el desarrollo del programa//
#include "biblioteca.c"
// Implementacion de funcion para multiplicar matrices //
int main(int argc, char *argv[]){
    // Condicional para verificar los argumentos
    if (argc < 2){
        printf("Ingreso de argumentos \n $./ejecutable tamMatriz numHilos\n");
        return -1;
    }
    // Obteniendo los argumentos
    int SZ = atoi(argv[1]);
    int n_threads = atoi(argv[2]);
    // Declarando las variables
    pthread_t p[n_threads];
    pthread_attr_t atrMM;
    mA = MEM_CHUNK;
    mB = mA + SZ*SZ;
    mC = mB + SZ*SZ;
    // Llenando las matrices
    llenar_matriz(SZ);
    // Imprimiendo las matrices
    print_matrix(SZ, mA);
    print_matrix(SZ, mB);
    // Inicializando el tiempo
    inicial_tiempo();
    // Inicializando el mutex
    pthread_mutex_init(&MM_mutex, NULL);
    pthread_attr_init(&atrMM);
    pthread_attr_setdetachstate(&atrMM, PTHREAD_CREATE_JOINABLE);
    // Creando los hilos y ejecutando la multiplicacion dependiendo de los hilos
    for (int j=0; j<n_threads; j++){
        struct parametros *datos = (struct parametros *) malloc(sizeof(struct parametros));
        datos->idH = j;
        datos->nH  = n_threads;
        datos->N   = SZ;
        pthread_create(&p[j],&atrMM,mult_thread,(void *)datos);
    }
    // Esperando a que los hilos terminen
    for (int j=0; j<n_threads; j++)
        pthread_join(p[j],NULL);
    // Finalizando el tiempo
    final_tiempo();
    // Imprimiendo la matriz resultante
    print_matrix(SZ, mC);
    // Finalizando el mutex
    pthread_attr_destroy(&atrMM);
    pthread_mutex_destroy(&MM_mutex);
    // Finalizando los hilos
    pthread_exit (NULL);
}