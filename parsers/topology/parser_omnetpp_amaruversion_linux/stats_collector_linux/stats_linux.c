#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LENGTH_AUX 100       
#define PATH_MAX 1024	

void NameDir(char *);
int NumTopos(char *);
void ListarDir(char *, char *);
int contenidoString(char *);
int DirValid(char *);
int Parser(char *, char *);
int NumLines(FILE *);
int NumNodes(char *);
void  ReadStats(FILE *, int, char *, char *);
int IsAstats(char *);

int main() {

	FILE *fp;
	int status,z,i, num_topo, aux_parser;
	char name_dir[50]; //name of the root directory
	char path[PATH_MAX];  //var aux to get info from pipe
	char **name_dir_dinamica = NULL; // pointer for dynamic reservation
	char comando_string[50];      //To open the pipe with the name of the root directory that the user writes, if not, Topologia_Brite will be used

	//Name root directory
	NameDir(name_dir);

	//Number of topologies
	num_topo = NumTopos(name_dir);
	
	printf("\n\n %d Topologies detected..\n\n", num_topo);
	printf("\n\nPress enter to continue...");
	while(getchar() != '\n');

	//Dynamic reservation of the two-dimensional array for the names of each topology
	name_dir_dinamica = (char **)malloc(num_topo * sizeof(char *));
	if (name_dir_dinamica == NULL) {
		system("clear");
		printf("Error there is not enough space in the memory...");
		printf("\n\nPress enter to continue..."); //  Equivalent to system ("pause"); in windows
		while(getchar() != '\n');                       //
		exit(-1);
	}
	
	for (i = 0; i < num_topo; i++) {
		name_dir_dinamica[i] = (char *)malloc(50);

	}
	
	strcpy(comando_string, "cd ");
	strcat(comando_string, name_dir);
	strcat(comando_string, " && ls -l | grep ^d");
	
	fp = popen(comando_string, "r");                  //Open pipe to get as input and output of the prompt to the command: cd name_dir 
	if (fp == NULL)                                    //   && ls -l | grep ^d   inside the root directory.
		exit(-1);

	i = 0 ;

	while (fgets(path, PATH_MAX, fp) != NULL) {
		
		z=contenidoString(path);                      //Check if it is a directory to each line that reads
		
		if (z == 1) {
			
			ListarDir(path, name_dir_dinamica[i]);      
			i++;
		}
	}

	system("mkdir Omnetpp_workspace");
	//Starts parser
	for (i = 0; i < num_topo; i++) {
		aux_parser = Parser(name_dir_dinamica[i], name_dir);

		if (aux_parser == 0) {
			printf("\nError in the parsing of the topology%s \n", name_dir_dinamica[i]);
		}
	}

	//Free pipe
	status = pclose(fp);
	if (status == -1) {
		exit(-1);
	}
	
	

	//Free dynamic reserve
	for (i = 0; i < num_topo; i++) {

		free(name_dir_dinamica[i]);
	}
	free(name_dir_dinamica);

	printf("\n\n\n ---- Work finished ----\n\n\n");
	printf("\n\npress enter to continue...");
	while(getchar() != '\n');

	return 0;
}


void NameDir(char * name_dir) {
	char aux_sn = '\0';

	//Name of root directory

	while (aux_sn != 's' && aux_sn != 'n') {
		system("clear");
		printf("Is your root directory: Data ? [s/n] : ");
		scanf("%c", &aux_sn);
		while (getchar() != '\n');
	}
	if (aux_sn == 'n') {
		system("clear");
		printf("Indicate the name of the root directory: ");
		gets(name_dir);
		aux_sn = '\0';
		while (aux_sn != 's' && aux_sn != 'n') {

			printf("\n\nConfirm, %s is your root directory? [s/n] : ", name_dir);
			scanf("%c", &aux_sn);
			while (getchar() != '\n');
		}
		if (aux_sn == 'n') {
			system("clear");
			printf("Indicate for the last time the name of the root directory:");
			gets(name_dir);
			aux_sn = '\0';
			while (aux_sn != 's' && aux_sn != 'n') {

				printf("\n\nConfirm, %s is your root directory? [s/n] : ", name_dir);
				scanf("%c", &aux_sn);
				while (getchar() != '\n');
			}
			if (aux_sn == 'n') {
				system("clear");
				printf("\nThe number of attempts to enter the name of the directory has been exceeded, closing the program ..\n\n\n");
				printf("\n\npress enter to continue...");
				while(getchar() != '\n');
				exit(1);
			}

		}
	}
	else {
		strcpy(name_dir, "Data");
	}
}


int NumTopos(char * name_dir) {
	FILE *fp;
	int status, z, num_topo = 0;
	char path[PATH_MAX];  //var aux to get info from pipe
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


int contenidoString(char * path) {      //Just check if the first character of each string in the list if it is a directory
	int z = 0;
	
	if(path[0] == 'd'){
		z = 1;
	}
	
	return z;
}


void ListarDir(char * path, char *name_dir) {
	char *pt_aux = NULL;
	int counter_aux = 1;

	pt_aux = strtok(path, " ");  /*Split  string */
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


int Parser(char * name_topo, char *directory_root_name) {
	int z = 1, num_nodes_topo = 0;
	FILE *Stats_File = NULL;
	char path_statsfile_toParse[100];
	char name_topo_aux[30];

	name_topo[strlen(name_topo) - 1] = '\0';

	strcpy(path_statsfile_toParse, "./");
	strcat(path_statsfile_toParse, directory_root_name);
	strcat(path_statsfile_toParse, "/");
	strcat(path_statsfile_toParse, name_topo);
	strcat(path_statsfile_toParse, "/");
	strcat(path_statsfile_toParse, name_topo);
	strcat(path_statsfile_toParse, "_stats.txt");

	strcpy(name_topo_aux, name_topo);

	num_nodes_topo = NumNodes(name_topo_aux);
	Stats_File = fopen(path_statsfile_toParse, "wt+");
	if (Stats_File == NULL) {
		printf("ERROR, cant create stats file. . .\n");
		printf("\n\npress enter to continue...");
		while(getchar() != '\n');
		exit(-1);
	}
	while (num_nodes_topo != 0) {

		ReadStats(Stats_File, num_nodes_topo, directory_root_name, name_topo);
		num_nodes_topo--;
	}

	////


	fclose(Stats_File);


	return z;
}

int NumLines(FILE * File_toRead) {
	int num_lines = 0;
	char str_aux[LENGTH_AUX];

	//Number of lines reading

	while (!feof(File_toRead)) {

		num_lines++;
		fgets(str_aux, LENGTH_AUX, File_toRead);


	}

	rewind(File_toRead);
	return num_lines - 1; //"-1" due to "\n" at the end          

}

int NumNodes(char * NameTopo) {
	char * pt_aux = NULL;
	int counter_aux = 0, nodes = 0;


	pt_aux = strtok(NameTopo, "-");
	while (counter_aux != 3) {
		switch (counter_aux)
		{

		case 2:
			nodes = atoi(pt_aux);
			break;

		}
		counter_aux++;
		pt_aux = strtok(NULL, "-");
	}

	return nodes;
}

void ReadStats(FILE * Stats_File, int num_node, char * directory_root_name, char * name_topo) {
	FILE *Data_File = NULL;
	char path_statsfile_toParse[100];
	char aux_num_node[12];
	char stats_line_topo[200];
	int z = 0;

	strcpy(path_statsfile_toParse, "./");
	strcat(path_statsfile_toParse, directory_root_name);
	strcat(path_statsfile_toParse, "/");
	strcat(path_statsfile_toParse, name_topo);
	strcat(path_statsfile_toParse, "/");
	strcat(path_statsfile_toParse, name_topo);
	strcat(path_statsfile_toParse, "[nodo");
	sprintf(aux_num_node, "%d", num_node);
	strcat(path_statsfile_toParse, aux_num_node);
	strcat(path_statsfile_toParse, "].txt");


	Data_File = fopen(path_statsfile_toParse, "r");
	if (Data_File == NULL) {
		printf("ERROR, cant create stats file. . .\n");
		printf("\n\nPress enter to continue...");
		while(getchar() != '\n');
		exit(-1);
	}
	while (fgets(stats_line_topo, PATH_MAX, Data_File) != NULL) {

		z = IsAstats(stats_line_topo);

		if (z == 1) {
			fprintf(Stats_File, stats_line_topo);
			fprintf(Stats_File, "\n");
		}
	}

	fclose(Data_File);
}


int IsAstats(char * str) {
	int aux = 0;

	if (str[0] == '-') {
		aux = 1;
	}

	return aux;
}
