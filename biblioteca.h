/**************************************************************
		Pontificia Universidad Javeriana
	Autor: Jose Andres Jaramillo
	Fecha: 7 Mayo 2024
	Materia: Sistemas Operativos
	Tema: Taller de Evaluación de Rendimiento
	Fichero: funciones.h es un archivo que contiene las funciones que se van a utilizar en el programa principal.
	Objetivo: Evaluar el tiempo de ejecución del
		      algoritmo clásico de multiplicación de matrices.
			  Se implementa con la Biblioteca POSIX Pthreads.
****************************************************************/
// Importando las librerias requeridas para el desarrollo del programa//
#ifndef FUNCIONES_H
#define FUNCIONES_H
// Definiendo las librerias requeridas para el desarrollo del programa//
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
// Reserva de memoria para la matriz//
#define DATA_SIZE (1024*1024*64*3)
// Definiendo las variables globales a utilizar//
pthread_mutex_t MM_mutex;
static double MEM_CHUNK[DATA_SIZE];
double *mA, *mB, *mC;
// Estructura para los parametros de los hilos //
struct parametros{
    int nH;
    int idH;
    int N;
};
// Variables para el tiempo //
struct timeval start, stop;
// Funcion para llenar matrices //
void llenar_matriz(int SZ);
// Funcion para imprimir matrices //
void print_matrix(int sz, double *matriz);
// Funcion para inicializar el tiempo //
void inicial_tiempo();
// Funcion para finalizar el tiempo //
void final_tiempo();
// Funcion para multiplicar matrices //
void *mult_thread(void *variables);

#endif