/*
 * Copyright (C) 2019 Elisa Rojas(1), Hedayat Hosseini(2), and David Carrascal(1);
 *                    (1) GIST, University of Alcala, Spain.
 *                    (2) CEIT, Amirkabir University of Technology (Tehran
 *                        Polytechnic), Iran.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define CONDITION_MAX 8
#define MAX_LINE_LEN 500

int main(int argc, char *argv[])
{
    FILE *fp;

    char destfile[100];
    int sim_parser(FILE *, char *);

    printf("\nSim changer 2019\n\n");

    if (argc==1)
    {
        fprintf(stderr,"\nNo simulation file introduced\n");
    }
    else{
            if ((fp= fopen(*++argv,"r"))==NULL){
                fprintf(stderr,"\nCan't open %s\n",*argv);
                return 0;
            }else{
                system("mkdir new-sim");
                strcat(strcpy(destfile, "new-sim/"), *argv);
                if (sim_parser(fp, destfile))
                    printf("\nNew simulation files were generated successfully.\n");
                else
                    printf("\nNew file is not generated successfully! please try again.\n");
                fclose(fp);
            }

    }

    return 1;
}
/*---------------------------------------------------------------------------*/
int sim_parser(FILE *fp, char *destfile){

    char line[MAX_LINE_LEN];
    int i,common_check=0,check_condition[CONDITION_MAX];

    FILE *destfp;
    if((destfp=fopen(destfile,"w"))==NULL){
       fprintf(stderr,"\nCan't open destination file (%s)\n",destfile);
       return 0;

       }else{
           while (fgets(line,MAX_LINE_LEN,fp)){
                for(i=0;i<CONDITION_MAX;i++)
                    check_condition[i]=0;
                common_check = 1;
                check_condition[0] = common_check && (strstr(line,"<?xml"));
                check_condition[1] = common_check && (strstr(line,"<motetype>"));
                check_condition[2] = common_check && (strstr(line,"<mote>"));
                check_condition[3] = common_check && (strstr(line,"</simulation>"));

                if (check_condition[0]){
                  do{
                   fputs(line, destfp);
                 }while (!strstr(fgets(line,MAX_LINE_LEN,fp), "<motetype>"));
                 check_condition[1] = common_check && (strstr(line,"<motetype>"));
                }

                if (check_condition[1]){
                  while (!strstr(fgets(line,MAX_LINE_LEN,fp), "<source")); //To find node path
                  char node_path[500];
                  sscanf(strstr(line, "<source EXPORT=\"discard\">") + strlen("<source EXPORT=\"discard\">"), "%s", node_path);
                  if(strstr(line, "root"))
                    fprintf(destfp, "\t<motetype>\n\t\torg.contikios.cooja.contikimote.ContikiMoteType\n\t\t<identifier>cooja1</identifier>\n\t\t<description>Cooja Sink</description>\n\t\t<source>%s\n\t\t<commands>make iotorii-root.cooja TARGET=cooja</commands>\n\t\t<moteinterface>org.contikios.cooja.interfaces.Position</moteinterface>\n\t\t<moteinterface>org.contikios.cooja.interfaces.Battery</moteinterface>\n\t\t<moteinterface>org.contikios.cooja.contikimote.interfaces.ContikiVib</moteinterface>\n\t\t<moteinterface>org.contikios.cooja.contikimote.interfaces.ContikiMoteID</moteinterface>\n\t\t<moteinterface>org.contikios.cooja.contikimote.interfaces.ContikiRS232</moteinterface>\n\t\t<moteinterface>org.contikios.cooja.contikimote.interfaces.ContikiBeeper</moteinterface>\n\t\t<moteinterface>org.contikios.cooja.interfaces.RimeAddress</moteinterface>\n\t\t<moteinterface>org.contikios.cooja.contikimote.interfaces.ContikiIPAddress</moteinterface>\n\t\t<moteinterface>org.contikios.cooja.contikimote.interfaces.ContikiRadio</moteinterface>\n\t\t<moteinterface>org.contikios.cooja.contikimote.interfaces.ContikiButton</moteinterface>\n\t\t<moteinterface>org.contikios.cooja.contikimote.interfaces.ContikiPIR</moteinterface>\n\t\t<moteinterface>org.contikios.cooja.contikimote.interfaces.ContikiClock</moteinterface>\n\t\t<moteinterface>org.contikios.cooja.contikimote.interfaces.ContikiLED</moteinterface>\n\t\t<moteinterface>org.contikios.cooja.contikimote.interfaces.ContikiCFS</moteinterface>\n\t\t<moteinterface>org.contikios.cooja.contikimote.interfaces.ContikiEEPROM</moteinterface>\n\t\t<moteinterface>org.contikios.cooja.interfaces.Mote2MoteRelations</moteinterface>\n\t\t<moteinterface>org.contikios.cooja.interfaces.MoteAttributes</moteinterface>\n\t\t<symbols>false</symbols>\n\t</motetype>\n", node_path);
                  else
                    fprintf(destfp, "\t<motetype>\n\t\torg.contikios.cooja.contikimote.ContikiMoteType\n\t\t<identifier>cooja2</identifier>\n\t\t<description>Cooja Mote</description>\n\t\t<source>%s\n\t\t<commands>make iotorii-common-node.cooja TARGET=cooja</commands>\n\t\t<moteinterface>org.contikios.cooja.interfaces.Position</moteinterface>\n\t\t<moteinterface>org.contikios.cooja.interfaces.Battery</moteinterface>\n\t\t<moteinterface>org.contikios.cooja.contikimote.interfaces.ContikiVib</moteinterface>\n\t\t<moteinterface>org.contikios.cooja.contikimote.interfaces.ContikiMoteID</moteinterface>\n\t\t<moteinterface>org.contikios.cooja.contikimote.interfaces.ContikiRS232</moteinterface>\n\t\t<moteinterface>org.contikios.cooja.interfaces.RimeAddress</moteinterface>\n\t\t<moteinterface>org.contikios.cooja.contikimote.interfaces.ContikiIPAddress</moteinterface>\n\t\t<moteinterface>org.contikios.cooja.contikimote.interfaces.ContikiRadio</moteinterface>\n\t\t<moteinterface>org.contikios.cooja.contikimote.interfaces.ContikiButton</moteinterface>\n\t\t<moteinterface>org.contikios.cooja.contikimote.interfaces.ContikiPIR</moteinterface>\n\t\t<moteinterface>org.contikios.cooja.contikimote.interfaces.ContikiClock</moteinterface>\n\t\t<moteinterface>org.contikios.cooja.contikimote.interfaces.ContikiLED</moteinterface>\n\t\t<moteinterface>org.contikios.cooja.contikimote.interfaces.ContikiCFS</moteinterface>\n\t\t<moteinterface>org.contikios.cooja.contikimote.interfaces.ContikiEEPROM</moteinterface>\n\t\t<moteinterface>org.contikios.cooja.interfaces.Mote2MoteRelations</moteinterface>\n\t\t<moteinterface>org.contikios.cooja.interfaces.MoteAttributes</moteinterface>\n\t\t<symbols>false</symbols>\n\t</motetype>\n", node_path);
                }

                if (check_condition[2]){
                  char node_x[50], node_y[50], node_z[50];
                  int node_id, is_root;
                  while (!strstr(fgets(line,MAX_LINE_LEN,fp), "</mote>")){
                    if (strstr(line,"<x>"))
                      sscanf(strstr(line,"<x>") + strlen("<x>"), "%s", node_x);
                    if (strstr(line,"<y>"))
                      sscanf(strstr(line,"<y>") + strlen("<y>"), "%s", node_y);
                    if (strstr(line,"<z>"))
                      sscanf(strstr(line,"<z>") + strlen("<z>"), "%s", node_z);
                    if (strstr(line,"<id>"))
                      sscanf(strstr(line,"<id>") + strlen("<id>"), "%d", &node_id);
                    if (strstr(line,"sky1"))
                      is_root = 1;
                    if (strstr(line,"sky2"))
                      is_root = 0;


                  }

                  fprintf(destfp,"\t<mote>\n\t\t<interface_config>\n\t\torg.contikios.cooja.interfaces.Position\n\t\t<x>%s\n\t\t<y>%s\n\t\t<z>%s\n\t\t</interface_config>\n\t\t<interface_config>\n\t\torg.contikios.cooja.contikimote.interfaces.ContikiMoteID\n\t\t<id>%d</id>\n\t\t</interface_config>\n\t\t<interface_config>\n\t\torg.contikios.cooja.contikimote.interfaces.ContikiRadio\n\t\t<bitrate>250.0</bitrate>\n\t\t</interface_config>\n\t\t<interface_config>\n\t\torg.contikios.cooja.contikimote.interfaces.ContikiEEPROM\n\t\t<eeprom>AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA==</eeprom>\n\t\t</interface_config>\n\t\t<motetype_identifier>%s</motetype_identifier>\n\t</mote>\n", node_x, node_y, node_z, node_id, is_root==1?"cooja1":"cooja2");

                }

                if (check_condition[3]){
                  do{
                   fputs(line, destfp);
                 }while (fgets(line,MAX_LINE_LEN,fp));
                }


           }//END while
           fclose(destfp);
       }
       return 1;
}
