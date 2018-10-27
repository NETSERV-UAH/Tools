#include "pch.h"
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>

/*Version VS17 anterior a la 15.8.7 Windows 10*/

#define LENGTH_AUX 100       /* Depending on the accuracy of the latency and the node numbers */
#define PATH_MAX 1024	
#define PATH_NODE "\\RyuFileNodes.txt"          /*  Topologias_Brite\Nombre_De_La_Topología\RyuFileNodes.txt       */
#define PATH_EDGE "\\RyuFileEdges.txt"           /*Se espera encontrar esta ruta, de no ser así cambiar el valor de define*/  
#define X_MAP_ESCENARIO 10000             //Maximo establecido para situar nodos
#define Y_MAP_ESCENARIO 10000

void NameDir(char *);
void GenOmnetppIni(char *, char *);
void GenSimPackage(char *);
void GenConfigNet(char *, char *, FILE *, FILE *);
void Header_Ned(FILE *, int, int, char *);
int Nodes_Ned(FILE *, FILE *);
int Edges_Ned(FILE *, FILE *);
int NumTopos(char *);
void ListDir(char *, char *);
int contenString(char *);
int DirValid(char *);
int Parser(char *, char *);
int NumLines(FILE *);
void GenFileSim(char *);

