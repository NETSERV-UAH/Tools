#include "pch.h"
#include "parser_funciones.h"


void NameDir(char * name_dir) {
	char aux_sn = '\0';

	//Name of root directory

	while (aux_sn != 's' && aux_sn != 'n') {
		system("cls");
		printf("Is your root directory: Topologias_Brite ? [s/n] : ");
		scanf_s("%c", &aux_sn);
		while (getchar() != '\n');
	}
	if (aux_sn == 'n') {
		system("cls");
		printf("Indicate the name of the root directory: ");
		scanf_s("%s", name_dir);
		aux_sn = '\0';
		while (aux_sn != 's' && aux_sn != 'n') {

			printf("\n\nConfirm, %s is your root directory? [s/n] : ", name_dir);
			scanf_s("%c", &aux_sn);
			while (getchar() != '\n');
		}
		if (aux_sn == 'n') {
			system("cls");
			printf("Indicate for the last time the name of the root directory:");
			scanf_s("%s", name_dir);
			aux_sn = '\0';
			while (aux_sn != 's' && aux_sn != 'n') {

				printf("\n\nConfirm, %s is your root directory? [s/n] : ", name_dir);
				scanf_s("%c", &aux_sn);
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

		strcpy_s(name_dir, 50, "Topologias_Brite");
	}
}

void GenOmnetppIni(char * directory_root_name, char * name_topo) {
	FILE * omnetpp = NULL;
	char aux_str[100];

	//Generates the omnetpp.ini file

	strcpy_s(aux_str, "C:.\\Omnetpp_workspace\\");
	strcat_s(aux_str, name_topo);
	strcat_s(aux_str, "\\simulations\\omnetpp.ini");

	fopen_s(&omnetpp, aux_str, "wt+");
	if (omnetpp == NULL) {
		printf("ERROR it wasnt possible to create the file omnetpp.ini ");
		system("pause");
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
	char path[PATH_MAX];
	char comando_string[50];

	//Count the number of topologies to parse
	strcpy_s(comando_string, "cd ");
	strcat_s(comando_string, name_dir);
	strcat_s(comando_string, " && dir");
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


void GenConfigNet(char *directory_root_name, char * name_topo, FILE * Edges_File, FILE * Nodes_File) {
	FILE * Ned_File = NULL;
	int z, x_netmap = X_MAP_ESCENARIO , y_netmap = Y_MAP_ESCENARIO;
	char aux_str[100];

	//Generates *.ned file
	strcpy_s(aux_str, "C:.\\Omnetpp_workspace\\");
	strcat_s(aux_str, name_topo);
	strcat_s(aux_str, "\\simulations\\configTopo.ned");

	fopen_s(&Ned_File, aux_str, "wt+");
	if (Ned_File == NULL) {
		printf("ERROR it wasnt possible to create the file configTopo.ned ");
		system("pause");
		exit(-1);
	}
	else {

		Header_Ned(Ned_File, x_netmap, y_netmap, name_topo);     
		z = Nodes_Ned(Ned_File, Nodes_File);         
		z = Edges_Ned(Ned_File, Edges_File);        

		if (z < 0) {
			printf("Error..");
			fclose(Ned_File);
			exit(-1);
		}

		fprintf(Ned_File, "\n\n}");
		fclose(Ned_File);
	}
}

void Header_Ned(FILE * ned, int x_netmap, int y_netmap, char * name_topo) {

	fprintf(ned, "package simulations;\nimport src.node;\nimport src.pktDropper;\nimport src.pktgen;\nimport src.sdnController;\n\nnetwork Net {\n\nparameters:\n\t@display(%cbgb=%d,%d%c);\n\n", 0x22, x_netmap, y_netmap, 0x22);
	fprintf(ned, "submodules:\n\n");
}


int Nodes_Ned(FILE * ned, FILE * Nodes_File) {
	//Var aux
	char str_aux[LENGTH_AUX];
	char *pt_aux = NULL;
	char *next_pt_aux = NULL;
	const char * delim = ";";
	int counter_aux = 0, z = 1, nodo = 0, num_lines = 0;
	float x_map = 0, y_map;
	//rsize_t strmax; since C(11)

	num_lines = NumLines(Nodes_File);
	while (num_lines != 0) {
		if (fgets(str_aux, LENGTH_AUX, Nodes_File) != NULL) {  //Read RyuFileNodes.txt
			//strmax = sizeof str_aux; since C(11) 
			pt_aux = strtok_s(str_aux, delim, &next_pt_aux);
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
				pt_aux = strtok_s(NULL, delim, &next_pt_aux);
			}

			fprintf(ned, "\tnodo%d: node { @display(%cp=%f,%f%c); }\n", nodo, 0x22, x_map, y_map, 0x22);
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
	char *next_pt_aux = NULL;
	const char * delim = ";";
	int counter_aux = 0, z = 1, nodo_A = 0, nodo_B = 0, num_lines = 0;
	float BW = 0, delay = 0;
	//rsize_t strmax; since C(11)

	fprintf(Ned_File, "\n\nconnections:\n");
	num_lines = NumLines(Edges_File);
	while (num_lines != 0) {
		if (fgets(str_aux, LENGTH_AUX, Edges_File) != NULL) {  //Read RyuFileEdges.txt
			//strmax = sizeof str_aux; since C(11) 
			pt_aux = strtok_s(str_aux, delim, &next_pt_aux);
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
				pt_aux = strtok_s(NULL, delim, &next_pt_aux);
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


void ListDir(char * path, char * name_dir) {
	char *pt_aux = NULL;
	char *next_pt_aux = NULL;
	const char * delim = " ";
	int counter_aux = 1;

	pt_aux = strtok_s(path, delim, &next_pt_aux);  /*Split del string */
	while (counter_aux < 5) {
		switch (counter_aux)
		{
		case 4:
			if (DirValid(pt_aux) == 1) {         //Discriminates:  <DIR> .\n  and  <DIR> ..\n
				strcpy_s(name_dir, 50, pt_aux);
				break;
			}
			break;
		}
		counter_aux++;
		pt_aux = strtok_s(NULL, delim, &next_pt_aux);
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
	int z = 1, num_enlaces = 0, enlace = 1;
	int nodo_A = 0, pref_nodo_A = 1;
	int nodo_B = 0, pref_nodo_B = 1;
	FILE *Edges_File = NULL, *Nodes_File = NULL;
	char path_fileEdges_toParse[100];
	char path_fileNode_toParse[100];
	char dir_omnetpp[200];

	//Genrates Omnetpp_workspace directory 
	name_topo[strlen(name_topo) - 1] = '\0';

	strcpy_s(dir_omnetpp, sizeof dir_omnetpp, "mkdir Omnetpp_workspace");
	system(dir_omnetpp);

	strcpy_s(dir_omnetpp, sizeof dir_omnetpp, "cd Omnetpp_workspace && mkdir ");
	strcat_s(dir_omnetpp, name_topo);
	system(dir_omnetpp);

	strcpy_s(dir_omnetpp, sizeof dir_omnetpp, "cd .//Omnetpp_workspace//");
	strcat_s(dir_omnetpp, name_topo);
	strcat_s(dir_omnetpp, " && mkdir simulations");
	system(dir_omnetpp);

	strcpy_s(dir_omnetpp, sizeof dir_omnetpp, "cd .//Omnetpp_workspace//");
	strcat_s(dir_omnetpp, name_topo);
	strcat_s(dir_omnetpp, " && mkdir src");
	system(dir_omnetpp);

	strcpy_s(dir_omnetpp, sizeof dir_omnetpp, "xcopy C:.\\src C:.\\Omnetpp_workspace\\");
	strcat_s(dir_omnetpp, name_topo);
	strcat_s(dir_omnetpp, "\\src /s/c/y");
	system(dir_omnetpp);

	strcpy_s(path_fileEdges_toParse, sizeof path_fileEdges_toParse, "C:.\\");
	strcat_s(path_fileEdges_toParse, directory_root_name);
	strcat_s(path_fileEdges_toParse, "\\");
	strcat_s(path_fileEdges_toParse, name_topo);
	strcat_s(path_fileEdges_toParse, PATH_EDGE);

	strcpy_s(path_fileNode_toParse, sizeof path_fileNode_toParse, "C:.\\");
	strcat_s(path_fileNode_toParse, directory_root_name);
	strcat_s(path_fileNode_toParse, "\\");
	strcat_s(path_fileNode_toParse, name_topo);
	strcat_s(path_fileNode_toParse, PATH_NODE);


	//Open nodes and edges files.
	fopen_s(&Edges_File, path_fileEdges_toParse, "r+");
	if (Edges_File == NULL) {
		printf("ERROR has not been possible to open the file");
		exit(-1);
	}

	//Open nodes and edges files.
	fopen_s(&Nodes_File, path_fileNode_toParse, "r+");
	if (Nodes_File == NULL) {
		printf("ERROR has not been possible to open the file..");
		exit(-1);
	}

	GenOmnetppIni(directory_root_name, name_topo);

	GenSimPackage(name_topo);

	GenConfigNet(directory_root_name, name_topo, Edges_File, Nodes_File);

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
	strcpy_s(aux_str, sizeof aux_str, "copy C:.\\.cproject C:.\\Omnetpp_workspace\\");
	strcat_s(aux_str, name_topo);
	system(aux_str);

	strcpy_s(aux_str, sizeof aux_str, "copy C:.\\.oppbuildspec C:.\\Omnetpp_workspace\\");
	strcat_s(aux_str, name_topo);
	system(aux_str);

	strcpy_s(aux_str, sizeof aux_str, "C:.\\Omnetpp_workspace\\");
	strcat_s(aux_str, name_topo);
	strcat_s(aux_str, "\\.project");

	fopen_s(&sim_pryc, aux_str, "wt+");
	if (sim_pryc == NULL) {
		printf("ERROR it wasnt possible to create the file .project ");
		system("pause");
		exit(-1);
	}
	else {  //Generates a .project file based on each topology

		fprintf(sim_pryc, "<?xml version=%c1.0%c encoding=%cUTF-8%c?>\n", 0x22, 0x22, 0x22, 0x22);
		fprintf(sim_pryc, "<projectDescription>\n\t<name>%s</name>\n\t<comment></comment>\n\t<projects>\n\t</projects>\n\t<buildSpec>\n\t\t<buildCommand>\n", name_topo);
		fprintf(sim_pryc, "\t\t\t<name>org.omnetpp.cdt.MakefileBuilder</name>\n\t\t\t<arguments>\n\t\t\t</arguments>\n");
		fprintf(sim_pryc, "\t\t</buildCommand>\n\t\t<buildCommand>\n\t\t\t<name>org.omnetpp.scave.builder.vectorfileindexer</name>\n\t\t\t<arguments>\n\t\t\t</arguments>\n\t\t</buildCommand>\n\t\t<buildCommand>\n\t\t\t<name>org.eclipse.cdt.managedbuilder.core.genmakebuilder</name>\n");
		fprintf(sim_pryc, "\t\t\t<triggers>clean,full,incremental,</triggers>\n\t\t\t<arguments>\n\t\t\t</arguments>\n\t\t</buildCommand>\n\t\t<buildCommand>\n\t\t\t<name>org.eclipse.cdt.managedbuilder.core.ScannerConfigBuilder</name>\n\t\t\t<triggers>full,incremental,</triggers>\n");
		fprintf(sim_pryc, "\t\t\t<arguments>\n\t\t\t</arguments>\n\t\t</buildCommand>\n\t</buildSpec>\n\t<natures>\n\t\t<nature>org.eclipse.cdt.core.cnature</nature>\n\t\t<nature>org.eclipse.cdt.core.ccnature</nature>\n\t\t<nature>org.eclipse.cdt.managedbuilder.core.managedBuildNature</nature>\n");
		fprintf(sim_pryc, "\t\t<nature>org.eclipse.cdt.managedbuilder.core.ScannerConfigNature</nature>\n\t\t<nature>org.omnetpp.main.omnetppnature</nature>\n\t</natures>\n</projectDescription>");

		fclose(sim_pryc);
	}

}

void GenSimPackage(char * name_topo) {

	FILE * package = NULL;
	char aux_str[100];

	//Generates the package.ned file

	strcpy_s(aux_str, sizeof aux_str, "C:.\\Omnetpp_workspace\\");
	strcat_s(aux_str, name_topo);
	strcat_s(aux_str, "\\simulations\\package.ned");

	fopen_s(&package, aux_str, "wt+");
	if (package == NULL) {
		printf("ERROR it wasnt possible to create the file package.ned ");
		system("pause");
		exit(-1);
	}
	else {
		fprintf(package, "package simulations;\n");
		fclose(package);
	}

}