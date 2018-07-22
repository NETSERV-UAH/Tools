#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LENGTH_AUX 100       
#define PATH_MAX 1024	

void NameDir(char *);
int NumTopos(char *);
void ListDir(char *, char *);
int contenString(char *);
int DirValid(char *);
int Parser(char *, char *);
int NumLines(FILE *);
int NumNodes(char *);
void  ReadStats(FILE *, int, char *, char *);
int IsAstats(char *);

int main() {


	int z = 1, i, num_topo, status, aux_parser; //var aux
	char name_dir[50]; //Name of root Dir.
	char **name_dir_dynamic = NULL; // pointer to make a dynamic reserve
	char command_string[50]; //var aux
	char path[PATH_MAX];     // path 
	FILE *fp = NULL;      //pointer pipe

						  //Ask for the name of root dir.
	NameDir(name_dir);

	//Num. Topos.
	num_topo = NumTopos(name_dir);

	printf("\n\n %d Topologies detected..\n\n", num_topo);
	system("pause");

	//Dynamic reservation of the two - dimensional array for the names of each topology
	name_dir_dynamic = (char **)malloc(num_topo * sizeof(char *));
	if (name_dir_dynamic == NULL) {
		system("cls");
		printf("Error there is not enough space in the memory...");
		system("pause");
		exit(-1);
	}
	for (i = 0; i < num_topo; i++) {
		name_dir_dynamic[i] = (char *)malloc(50);

	}

	//We build the command to list the directories of our root directory
	strcpy(command_string, "cd ");
	strcat(command_string, name_dir);
	strcat(command_string, " && dir");


	fp = _popen(command_string, "r");                  //Open pipe to get as input the output of the prompt to the DIR command within the
	if (fp == NULL)                                    //   root directory.
		exit(-1);

	i = -2;
	while (fgets(path, PATH_MAX, fp) != NULL) {

		z = contenString(path);                      //Checks each line to verify if is a directory. 

		if (z == 1) {

			ListDir(path, name_dir_dynamic[i]);      //Discriminates:  <DIR> .\n  and  <DIR> ..\n
			i++;
		}
	}


	//Parser starts
	for (i = 0; i < num_topo; i++) {
		aux_parser = Parser(name_dir_dynamic[i], name_dir);

		if (aux_parser == 0) {
			printf("\nError in the parsing of the topology %s \n", name_dir_dynamic[i]);
		}
	}

	//Free pipe
	status = _pclose(fp);
	if (status == -1) {
		exit(-1);
	}

	//Free dynamic reserve 
	for (i = 0; i < num_topo; i++) {

		free(name_dir_dynamic[i]);
	}
	free(name_dir_dynamic);

	printf("\n\n\n ---- Work finished ----\n\n\n");
	system("pause");

	return 0;
}


void NameDir(char * name_dir) {
	char aux_sn = '\0';

	//Name of root directory

	while (aux_sn != 's' && aux_sn != 'n') {
		system("cls");
		printf("Is your root directory: Data ? [s/n] : ");
		scanf("%c", &aux_sn);
		while (getchar() != '\n');
	}
	if (aux_sn == 'n') {
		system("cls");
		printf("Indicate the name of the root directory: ");
		gets(name_dir);
		aux_sn = '\0';
		while (aux_sn != 's' && aux_sn != 'n') {

			printf("\n\nConfirm, %s is your root directory? [s/n] : ", name_dir);
			scanf("%c", &aux_sn);
			while (getchar() != '\n');
		}
		if (aux_sn == 'n') {
			system("cls");
			printf("Indicate for the last time the name of the root directory:");
			gets(name_dir);
			aux_sn = '\0';
			while (aux_sn != 's' && aux_sn != 'n') {

				printf("\n\nConfirm, %s is your root directory? [s/n] : ", name_dir);
				scanf("%c", &aux_sn);
				while (getchar() != '\n');
			}
			if (aux_sn == 'n') {
				system("cls");
				printf("\nThe number of attempts to enter the name of the directory has been exceeded, closing the program ..\n\n\n");
				system("pause");
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
	char path[PATH_MAX];
	char comando_string[50];

	//Count the number of topologies to parse
	strcpy(comando_string, "cd ");
	strcat(comando_string, name_dir);
	strcat(comando_string, " && dir");
	fp = _popen(comando_string, "r");
	if (fp == NULL)
		exit(-1);


	while (fgets(path, PATH_MAX, fp) != NULL) {

		z = contenString(path);

		if (z == 1) {

			num_topo++;

		}
	}

	status = _pclose(fp);
	if (status == -1) {
		exit(-1);
	}

	return num_topo - 2;          // "-2" due to  <DIR> .\n  and  <DIR> ..\n

}


void ListDir(char * path, char * name_dir) {
	char *pt_aux = NULL;
	int counter_aux = 1;

	pt_aux = strtok(path, " ");  /*Split del string */
	while (counter_aux < 5) {
		switch (counter_aux)
		{
		case 4:
			if (DirValid(pt_aux) == 1) {         //Discriminates:  <DIR> .\n  and  <DIR> ..\n
				strcpy(name_dir, pt_aux);
				break;
			}
			break;
		}
		counter_aux++;
		pt_aux = strtok(NULL, " ");
	}
}

int contenString(char * path) {
	int i, z = 0;

	for (i = 0; i < strlen(path); i++) {
		if ('<' == path[i] && 'D' == path[i + 1] && 'I' == path[i + 2] && 'R' == path[i + 3] && '>' == path[i + 4]) {
			z = 1;
			break;
		}
		if ((i + 4) > strlen(path)) {
			z = 0;
			break;
		}
	}

	return z;
}


int DirValid(char * pt_aux) {   //Discriminates:  <DIR> .\n  and  <DIR> ..\n			

	int z = 0;

	if (strcmp(".\n", pt_aux) == 0) {
		z = 0;
	}
	else if (strcmp("..\n", pt_aux) == 0) {
		z = 0;
	}
	else {
		z = 1;
	}
	return z;

}


int Parser(char * name_topo, char *directory_root_name) {
	int z = 1, num_nodes_topo = 0;
	FILE *Stats_File = NULL;
	char path_statsfile_toParse[100];
	char name_topo_aux[30];

	name_topo[strlen(name_topo) - 1] = '\0';

	strcpy(path_statsfile_toParse, "C:.\\");
	strcat(path_statsfile_toParse, directory_root_name);
	strcat(path_statsfile_toParse, "\\");
	strcat(path_statsfile_toParse, name_topo);
	strcat(path_statsfile_toParse, "\\");
	strcat(path_statsfile_toParse, name_topo);
	strcat(path_statsfile_toParse, "_stats.txt");

	strcpy(name_topo_aux, name_topo);

	num_nodes_topo = NumNodes(name_topo_aux);
	Stats_File = fopen(path_statsfile_toParse, "wt+");
	if (Stats_File == NULL) {
		printf("ERROR, cant create stats file. . .\n");
		system("pause");
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

	strcpy(path_statsfile_toParse, "C:.\\");
	strcat(path_statsfile_toParse, directory_root_name);
	strcat(path_statsfile_toParse, "\\");
	strcat(path_statsfile_toParse, name_topo);
	strcat(path_statsfile_toParse, "\\");
	strcat(path_statsfile_toParse, name_topo);
	strcat(path_statsfile_toParse, "(nodo");
	sprintf(aux_num_node, "%d", num_node);
	strcat(path_statsfile_toParse, aux_num_node);
	strcat(path_statsfile_toParse, ").txt");


	Data_File = fopen(path_statsfile_toParse, "r");
	if (Data_File == NULL) {
		printf("ERROR, cant create stats file. . .\n");
		system("pause");
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
