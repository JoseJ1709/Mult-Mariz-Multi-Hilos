#!/usr/bin/perl

#/***********************************
#Autores:
#- Jose Jaramillo
#Fecha: 7 de mayo de 2024
#Materia: Sistemas Operativos
#Pontificia Universidad Javeriana
#Tema: Taller rendimiento
#*************************************/
#Este script se encarga de ejecutar los programas ->
# -> MM_clasico y MM_transpuesta con diferentes configuraciones
#Se obtiene la ruta actual
$Path = `pwd`;
chomp($Path);

#Se ejecutan los dos programas con las diferentes configuraciones
@Nombre_Ejecutable = ("MM_clasico","MM_transpuesta");
#Se definen los tamaños de las matrices
@Size_Matriz = ("200","400","800","1000","1200", "1400");
#Se definen los hilos
@Num_Hilos = (1,2,4,6,8);
#Se definen las repeticiones
$Repeticiones = 30;
foreach $nombre (@Nombre_Ejecutable){
	foreach $size (@Size_Matriz){
		foreach $hilo (@Num_Hilos) {
			#Se define el archivo de salida
			$file = "$Path/$nombre-".$size."-Hilos-".$hilo.".dat";
			#Se abre el archivo
			for ($i=0; $i<$Repeticiones; $i++) {
			system("\"$Path/$nombre\" $size $hilo  >> \"$file\"");
			printf("$Path/$nombre $size $hilo \n");
			}
			close($file);
		$p=$p+1;
		}
	}
}
