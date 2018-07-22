#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LENGTH_AUX 100       /* Depending on the accuracy of the latency and the node numbers */
#define PATH_MAX 1024	

void NameDir(char *);
void GenOmnetppIni(char *,char *);
void GenSimPackage(char *);
void GenConfigNet(char *, char *,FILE *,FILE *);
void Header_Ned(FILE *, int , int, char *);
int Nodes_Ned(FILE *, FILE *);
int Edges_Ned(FILE *, FILE *);
int NumTopos(char *);
void ListarDir(char *, char *);
int contenidoString(char *);
int Parser(char *, char *);
int NumLines(FILE *);
void GenFileSim(char *);


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
	
	printf("\n\n %d Topologies detected..\n\n", num_topo);
	printf("\n\nPress enter to continue...");
	while(getchar() != '\n');

	//Reserva dinamica del array bidimensional para los nombre de cada topologia
	name_dir_dinamica = (char **)malloc(num_topo * sizeof(char *));
	if (name_dir_dinamica == NULL) {
		system("clear");
		printf("Error there is not enough space in the memory...");
		printf("\n\nPress enter to continue..."); //  Equivalente a system("pause"); en windows
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

	system("mkdir Omnetpp_workspace");
	//Empieza parseado
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
	
	

	//Free de la reserva dinamica
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
		printf("Is your root directory: Topologias_Brite ? [s/n] : ");
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
		strcpy(name_dir, "Topologias_Brite");
	}
}

void GenOmnetppIni(char * directory_root_name, char * name_topo) {
	FILE * omnetpp = NULL;
	char aux_str[100];
	
	//Generates the omnetpp.ini file

	strcpy(aux_str, "./Omnetpp_workspace/");
	strcat(aux_str, name_topo);
	strcat(aux_str, "/simulations/omnetpp.ini");

	omnetpp = fopen(aux_str, "wt+");                    
	if (omnetpp == NULL) {
		printf("ERROR it wasnt possible to create the file omnetpp.ini ");
		printf("\n\npress enter to continue...");
		while(getchar() != '\n');
		exit(-1);
	}
	else {
		fprintf(omnetpp, "[General]\nnetwork = Net\n\n**.N = 50\n**.P = 10\n**.L = 5\n**.isARoot = true");
		fclose(omnetpp);
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


void GenConfigNet(char *directory_root_name, char * name_topo, FILE * Edges_File, FILE * Nodes_File) {
	FILE * Ned_File = NULL;
	int z,x_netmap= 10000, y_netmap = 10000;
	char aux_str[100];

	//Generates *.ned file
	strcpy(aux_str, "./Omnetpp_workspace/");
	strcat(aux_str, name_topo);
	strcat(aux_str,"/simulations/configTopo.ned");

	Ned_File = fopen(aux_str, "wt+");
	if (Ned_File == NULL) {
		printf("ERROR it wasnt possible to create the file configTopo.ned ");
		printf("\n\npress enter to continue...");
		while(getchar() != '\n');
		exit(-1);
	}
	else {

		Header_Ned(Ned_File, x_netmap,y_netmap, name_topo);     //
		z = Nodes_Ned(Ned_File,Nodes_File);         //
		z = Edges_Ned(Ned_File, Edges_File);       // 

		if (z<0) {
			printf("Error..");
			fclose(Ned_File);
			exit(-1);
		}

		fprintf(Ned_File,"\n\n}");
		fclose(Ned_File);
	}
}

void Header_Ned(FILE * ned, int x_netmap, int y_netmap, char * name_topo) {

	fprintf(ned, "package simulations;\nimport src.node;\nimport src.pktDropper;\nimport src.pktgen;\nimport src.sdnController;\n\nnetwork Net {\n\nparameters:\n\t@display(%cbgb=%d,%d%c);\n\n", 0x22, x_netmap,y_netmap,0x22);
	fprintf(ned,"submodules:\n\n");
}


int Nodes_Ned(FILE * ned, FILE * Nodes_File) {
	//Var aux
	char str_aux[LENGTH_AUX];
	char *pt_aux = NULL;
	int counter_aux = 0, z = 1, nodo = 0, num_lines = 0;
	float x_map = 0, y_map;

	num_lines = NumLines(Nodes_File);
	while(num_lines != 0){
		if (fgets(str_aux, LENGTH_AUX, Nodes_File) != NULL) {  //Read RyuFileNodes.txt

			pt_aux = strtok(str_aux, ";");  
			while (counter_aux != 3) {
				switch (counter_aux)
				{
				case 0:
					nodo = atoi(pt_aux);
					break;

				case 1:
					x_map = atof(pt_aux);
					break;
				case 2:
					y_map = atof(pt_aux);
					break;
				}
				counter_aux++;
				pt_aux = strtok(NULL, ";");
			}

			fprintf(ned,"\tnodo%d: node { @display(%cp=%f,%f%c); }\n",nodo, 0x22,x_map,y_map,0x22);
			counter_aux = 0; 
			num_lines--;
		}
		else {
			fclose(Nodes_File);
			z = -1;
		}
	}
	fprintf(ned, "\tsdnController: sdnController{ @display(%cp=20,20%c); }\n", 0x22, 0x22);
	return z;
}

int Edges_Ned(FILE * Ned_File, FILE * Edges_File) {
	//Var aux
	char str_aux[LENGTH_AUX];
	char *pt_aux = NULL;
	int counter_aux = 0, z = 1, nodo_A = 0, nodo_B= 0, num_lines = 0;
	float BW = 0, delay = 0 ;

	fprintf(Ned_File,"\n\nconnections:\n");
	num_lines = NumLines(Edges_File);
	while (num_lines != 0) {
		if (fgets(str_aux, LENGTH_AUX, Edges_File) != NULL) {  //Read RyuFileEdges.txt

			pt_aux = strtok(str_aux, ";");
			while (counter_aux != 4) {
				switch (counter_aux)
				{
				case 0:
					nodo_A = atoi(pt_aux);
					break;

				case 1:
					nodo_B = atoi(pt_aux);
					break;
				case 2:
					BW = atof(pt_aux);
					break;
				case 3:
					delay = atof(pt_aux);
					break;
				}
				counter_aux++;
				pt_aux = strtok(NULL, ";");
			}
			counter_aux = 0;
			fprintf(Ned_File, "\tnodo%d.port++ <--> {datarate = %f Mbps; delay = %f ms;} <--> nodo%d.port++;\n", nodo_A, BW, delay, nodo_B);
			num_lines--;
		}
		else {
			fclose(Edges_File);
			z = -1;
		}
	}
	fprintf(Ned_File, "\t sdnController.port++ <--> nodo1.port++;\n");
	return z;
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



int Parser(char * name_topo, char *directory_root_name) {
	int z = 1;
	FILE *Edges_File = NULL, *Nodes_File = NULL;
	char path_fileEdges_toParse[100];
	char path_fileNode_toParse[100];
	char dir_omnetpp[200];

	//Genrates Omnetpp_workspace directory 
	name_topo[strlen(name_topo) - 1] = '\0';
	
	
	strcpy(dir_omnetpp,"cd Omnetpp_workspace && mkdir ");
	strcat(dir_omnetpp,name_topo);
	system(dir_omnetpp);

	strcpy(dir_omnetpp,"cd ./Omnetpp_workspace/");
	strcat(dir_omnetpp, name_topo);
	strcat(dir_omnetpp," && mkdir simulations");
	system(dir_omnetpp);

	strcpy(dir_omnetpp, "cd ./Omnetpp_workspace/");
	strcat(dir_omnetpp, name_topo);
	strcat(dir_omnetpp, " && mkdir src");
	system(dir_omnetpp);

	strcpy(dir_omnetpp, "cp -r src ./Omnetpp_workspace/");
	strcat(dir_omnetpp, name_topo);
	
	system(dir_omnetpp);

	strcpy(path_fileEdges_toParse, "./");
	strcat(path_fileEdges_toParse, directory_root_name);
	strcat(path_fileEdges_toParse, "/");
	strcat(path_fileEdges_toParse, name_topo);
	strcat(path_fileEdges_toParse, "/");
	strcat(path_fileEdges_toParse, "RyuFileEdges.txt");

	strcpy(path_fileNode_toParse, "./");
	strcat(path_fileNode_toParse, directory_root_name);
	strcat(path_fileNode_toParse, "/");
	strcat(path_fileNode_toParse, name_topo);
	strcat(path_fileNode_toParse, "/");
	strcat(path_fileNode_toParse, "RyuFileNodes.txt");
	
	//Open nodes and edges files.
	Edges_File = fopen(path_fileEdges_toParse, "r+");
	if (Edges_File == NULL) {
		printf("ERROR has not been possible to open the file");
		exit(-1);
	}
	Nodes_File = fopen(path_fileNode_toParse, "r+");                    
	if (Nodes_File == NULL) {
		printf("ERROR has not been possible to open the file..");
		exit(-1);
	}
	
	GenOmnetppIni(directory_root_name, name_topo);

	GenSimPackage(name_topo);
		
	GenConfigNet(directory_root_name, name_topo,Edges_File, Nodes_File);

	GenFileSim(name_topo);

	//Close Nodes and edges files
	fclose(Edges_File);
	fclose(Nodes_File);

	
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

void GenFileSim(char * name_topo) {
	char aux_str[100];
	FILE * sim_pryc = NULL;
	
	//Generates and copies the necessary files to import the project for a future simulation
	strcpy(aux_str,"cp ./.cproject ./Omnetpp_workspace/");
	strcat(aux_str,name_topo);
	system(aux_str);

	strcpy(aux_str, "cp ./.oppbuildspec ./Omnetpp_workspace/");
	strcat(aux_str, name_topo);
	system(aux_str);

	strcpy(aux_str,"./Omnetpp_workspace/");
	strcat(aux_str, name_topo);
	strcat(aux_str,"/.project");

	sim_pryc = fopen(aux_str, "wt+");
	if (sim_pryc == NULL) {
		printf("ERROR it wasnt possible to create the file .project ");
		printf("\n\npress enter to continue...");
		while(getchar() != '\n');
		exit(-1);
	}
	else {  //Generates a .project file based on each topology

		fprintf(sim_pryc,"<?xml version=%c1.0%c encoding=%cUTF-8%c?>\n",0x22,0x22,0x22,0x22);
		fprintf(sim_pryc,"<projectDescription>\n\t<name>%s</name>\n\t<comment></comment>\n\t<projects>\n\t</projects>\n\t<buildSpec>\n\t\t<buildCommand>\n",name_topo);
		fprintf(sim_pryc,"\t\t\t<name>org.omnetpp.cdt.MakefileBuilder</name>\n\t\t\t<arguments>\n\t\t\t</arguments>\n");
		fprintf(sim_pryc,"\t\t</buildCommand>\n\t\t<buildCommand>\n\t\t\t<name>org.omnetpp.scave.builder.vectorfileindexer</name>\n\t\t\t<arguments>\n\t\t\t</arguments>\n\t\t</buildCommand>\n\t\t<buildCommand>\n\t\t\t<name>org.eclipse.cdt.managedbuilder.core.genmakebuilder</name>\n");
		fprintf(sim_pryc,"\t\t\t<triggers>clean,full,incremental,</triggers>\n\t\t\t<arguments>\n\t\t\t</arguments>\n\t\t</buildCommand>\n\t\t<buildCommand>\n\t\t\t<name>org.eclipse.cdt.managedbuilder.core.ScannerConfigBuilder</name>\n\t\t\t<triggers>full,incremental,</triggers>\n");
		fprintf(sim_pryc,"\t\t\t<arguments>\n\t\t\t</arguments>\n\t\t</buildCommand>\n\t</buildSpec>\n\t<natures>\n\t\t<nature>org.eclipse.cdt.core.cnature</nature>\n\t\t<nature>org.eclipse.cdt.core.ccnature</nature>\n\t\t<nature>org.eclipse.cdt.managedbuilder.core.managedBuildNature</nature>\n");
		fprintf(sim_pryc,"\t\t<nature>org.eclipse.cdt.managedbuilder.core.ScannerConfigNature</nature>\n\t\t<nature>org.omnetpp.main.omnetppnature</nature>\n\t</natures>\n</projectDescription>");

		fclose(sim_pryc);
	}

}

void GenSimPackage( char * name_topo) {

	FILE * package = NULL;
	char aux_str[100];

	//Generates the package.ned file

	strcpy(aux_str, "./Omnetpp_workspace/");
	strcat(aux_str, name_topo);
	strcat(aux_str, "/simulations/package.ned");

	package = fopen(aux_str, "wt+");
	if (package == NULL) {
		printf("ERROR it wasnt possible to create the file package.ned ");
		printf("\n\npress enter to continue...");
		while(getchar() != '\n');
		exit(-1);
	}
	else {
		fprintf(package, "package simulations;\n");
		fclose(package);
	}

}
