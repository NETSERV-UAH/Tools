#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LENGTH_AUX 100 /*En función de la precisión de la latencia y los números de nodos*/
#define PATH_MAX 1024 

int contenidoString(char *);
void ListarDir(char *, char * );
int NumTopos(char *);
void NameDir(char *);
int Parser(char *, char *);
int read(FILE *, int *, int *);
int NumEnlaces(FILE *);

int main() {

	FILE *fp;
	int status,z,i, num_topo, aux_parser;
	char name_dir[50]; //Nombre del dir raiz 
	char path[PATH_MAX];  //var aux para obtener info de la pipe
	char **name_dir_dinamica = NULL; // puntero para reserva dinamica
	char comando_string[50];      //Para abrir la pipe con el nombre de directorio raiz que el user escriba, si no se utilizará Topologia_Brite

	//Nombre del Dir raiz
	NameDir(name_dir);

	//Numero de Topos.
	num_topo = NumTopos(name_dir);
	
	printf("\n\nSe han detectado %d topologias..\n\n", num_topo);
	printf("\n\nPresione Enter para continuar...");
	while(getchar() != '\n');
	//Reserva dinamica del array bidimensional para los nombre de cada topologia
	name_dir_dinamica = (char **)malloc(num_topo * sizeof(char *));
	if (name_dir_dinamica == NULL) {
		system("clear");
		printf("Error no hay suficiente espacio en la memoria..");
		printf("\n\nPresione Enter para continuar..."); //  Equivalente a system("pause"); en windows
		while(getchar() != '\n');                       //
		exit(-1);
	}
	
	for (i = 0; i < num_topo; i++) {
		name_dir_dinamica[i] = (char *)malloc(50);

	}
	
	strcpy(comando_string, "cd ");
	strcat(comando_string, name_dir);
	strcat(comando_string, " && ls -l | grep ^d");
	
	fp = popen(comando_string, "r");                  //Abre pipe para obetener como input el output del prompt al comando cd name_dir 
	if (fp == NULL)                                    //   && ls -l | grep ^d   dentro deldirectorio raiz.
		exit(-1);

	i = 0 ;
	while (fgets(path, PATH_MAX, fp) != NULL) {
		
		z=contenidoString(path);                      //Comprueba si se trata de un directorio  a cada linea que lee
		
		if (z == 1) {
			
			ListarDir(path, name_dir_dinamica[i]);      
			i++;
		}
	}
	
	//Empieza parseado
	for (i = 0; i < num_topo; i++) {
		aux_parser = Parser(name_dir_dinamica[i], name_dir);

		if (aux_parser == 0) {
			printf("\nError en el parseado de la topologia %s \n", name_dir_dinamica[i]);
		}
	}

	//Free pipe
	status = pclose(fp);
	if (status == -1) {
		exit(-1);
	}

	//Free de la reserva dinamica
	for (i = 0; i < num_topo; i++) {

		free(name_dir_dinamica[i]);
	}
	free(name_dir_dinamica);

	printf("\n\n\n ---- Tarea finalizada ----\n\n\n");
	printf("\n\nPresione Enter para continuar...");
	while(getchar() != '\n');

	return 0;
}


void NameDir(char * name_dir) {
	char aux_sn = '\0';

	//Nombre del Dir raiz
	//Tiene la posibilidad de cambiar el nombre del directorio raiz.
	while (aux_sn != 's' && aux_sn != 'n') {
		system("clear");
		printf("Es su directorio raiz: Topologias_Brite ? [s/n] : ");
		scanf("%c", &aux_sn);
		while (getchar() != '\n');
	}
	if (aux_sn == 'n') {
		system("clear");
		printf("Indique el nombre del directorio raiz: ");
		gets(name_dir);
		aux_sn = '\0';
		while (aux_sn != 's' && aux_sn != 'n') {
		
			printf("\n\nConfirme,  %s  es su directorio raiz? [s/n] : ", name_dir);
			scanf("%c", &aux_sn);
			while (getchar() != '\n');
		}
		if (aux_sn == 'n') {
			system("clear");
			printf("Indique por ultima vez el nombre del directorio raiz: ");
			gets(name_dir);
			aux_sn = '\0';
			while (aux_sn != 's' && aux_sn != 'n') {

				printf("\n\nConfirme,  %s  es su directorio raiz? [s/n] : ", name_dir);
				scanf("%c", &aux_sn);
				while (getchar() != '\n');
			}
			if (aux_sn == 'n') {
				system("clear");
				printf("\nSe ha superado el numero de intentos para introducir el nombre del directorio, cerrando programa..\n\n\n");
				printf("\n\nPresione Enter para continuar...");
				while(getchar() != '\n');
				exit(1);
			}
			
		}
	}
	else {
		strcpy(name_dir,"Topologias_Brite");
	}
}

int NumTopos(char * name_dir) {

	FILE *fp;
	int status, z, num_topo = 0;
	char path[PATH_MAX];  //var aux para obtener info de la pipe
	char comando_string[50];
	
	strcpy(comando_string,"cd ");
	strcat(comando_string, name_dir);
	strcat(comando_string, " && ls -l | grep ^d");
	fp = popen(comando_string, "r");
	if (fp == NULL)
		exit(-1);


	while (fgets(path, PATH_MAX, fp) != NULL) {

		z = contenidoString(path);

		if (z == 1) {

			num_topo++;

		}
	}

	status = pclose(fp);
	if (status == -1) {
		exit(-1);
	}

	return num_topo;          

}

int contenidoString(char * path) {      //Solo chequea si el primer caracter de cada string del listado se trata de un directorio 
	int z = 0;
	
	if(path[0] == 'd'){
		z = 1;
	}
	
	return z;
}


void ListarDir(char * path, char *name_dir) {
	char *pt_aux = NULL;
	int counter_aux = 1;

	pt_aux = strtok(path, " ");  /*Split del string */
	while (counter_aux < 10) {
		switch (counter_aux)
		{
		case 9:
			
			strcpy(name_dir, pt_aux);
				
			break;
		}
		counter_aux++;
		pt_aux = strtok(NULL, " ");
	}
}

int Parser(char * nombre_topo, char * nombre_directorio_raiz) {

	int z = 1, num_enlaces = 0, enlace = 1;
	int nodo_A = 0, pref_nodo_A = 1;
	int nodo_B = 0, pref_nodo_B = 1;
	FILE *topologia_parseada = NULL, *Topologia_para_parsear = NULL;
	char ruta_archivo_aparsear[100];
	char ruta_archivo_parseado[100];

	nombre_topo[strlen(nombre_topo) - 1] = '\0';                           //Rutas adecuadas para esta versión de linux
	strcpy(ruta_archivo_aparsear,"./");
	strcat(ruta_archivo_aparsear,nombre_directorio_raiz);
	strcat(ruta_archivo_aparsear, "/");
	strcat(ruta_archivo_aparsear,nombre_topo);
	strcat(ruta_archivo_aparsear, "/");
	strcat(ruta_archivo_aparsear, "RyuFileEdges.txt");

	Topologia_para_parsear = fopen(ruta_archivo_aparsear, "r+");
	if (Topologia_para_parsear == NULL) {
		printf("ERROR no ha sido posible abrir el archivo");
		exit(-1);
	}

	strcpy(ruta_archivo_parseado, "./");
	strcat(ruta_archivo_parseado, nombre_directorio_raiz);
	strcat(ruta_archivo_parseado, "/");
	strcat(ruta_archivo_parseado, nombre_topo);
	strcat(ruta_archivo_parseado, "/");
	strcat(ruta_archivo_parseado, nombre_topo);
	strcat(ruta_archivo_parseado, ".top");

	topologia_parseada = fopen(ruta_archivo_parseado, "wt+");                    //Crea el archivo sobre el cual se plasmará la información parseada de los enlaces
	if (topologia_parseada == NULL) {
		printf("ERROR no ha sido posible crear el archivo");
		exit(-1);
	}
	else {
		num_enlaces = NumEnlaces(Topologia_para_parsear);
		while (num_enlaces != 0) {
			printf("---- %s ----\n\n",nombre_topo);	
			printf("---- Enlace %d ----\n", enlace);
			z = read(Topologia_para_parsear, &nodo_A, &nodo_B);

			if (z == -1) {
				printf("Lectura del archivo RyuFileEdges.txt fallida ");
				exit(-1);
			}
			else {
				if (nodo_A == 1) {                //Si, alguno de los nodos se trata del nodo 1, se le asignara el prefijo de "0"
					pref_nodo_A = 0;
				}
				else {
					pref_nodo_A = 1;
				}
				if (nodo_B == 1) {
					pref_nodo_B = 0;
				}
				else {
					pref_nodo_B = 1;
				}                                /*Si es el nodo 1 ----> $n(0s(x-1)) , el resto --->  $n(1s(x-1))   */
				fprintf(topologia_parseada, "$topJAC duplex-link $n(%ds%d)		$n(%ds%d)\n", pref_nodo_A, nodo_A - 1, pref_nodo_B, nodo_B - 1);
				enlace++;
				num_enlaces--;
				system("clear");
			}
		}
		fclose(Topologia_para_parsear);
		fclose(topologia_parseada);
		
	}
	return z;
}

int read(FILE * fichero, int * a, int *b) {

	//Variables aux
	char str_aux[LENGTH_AUX];
	char *pt_aux = NULL;
	int counter_aux = 0, z = 1;


	if (fgets(str_aux, LENGTH_AUX, fichero) != NULL) {  //Lectura de RyuFileEdges.txt

		pt_aux = strtok(str_aux, ";");  /*Split del string  discrimina en base a los ';' ya que vienen así separados los datos del fichero "RyuFileEdges.txt" */
		while (counter_aux != 2) {
			switch (counter_aux)
			{
			case 0:
				*a = atoi(pt_aux);
				break;

			case 1:
				*b = atoi(pt_aux);
				break;
			}
			counter_aux++;
			pt_aux = strtok(NULL, ";");
		}
		
	}
	else {
		fclose(fichero);
		z = -1;
	}

	return z;
}


int NumEnlaces(FILE * Topologia_a_parsear) {
	int num_enalces = 0;
	char str_aux[LENGTH_AUX];

	while (!feof(Topologia_a_parsear)) {

		num_enalces++;
		fgets(str_aux, LENGTH_AUX, Topologia_a_parsear);
		

	}

	rewind(Topologia_a_parsear);
	return num_enalces-1;          //Salto de linea final que tiene todos los ficheros RyuFileEdges.txt

}
