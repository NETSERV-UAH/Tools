
# BRITE to OMNeT++ Amaru_version


Parser of BRITE topologies to OMNeT++ projects(written in **C**, **not compatible for linux**) 

I modified the node.cc file from [@a-s-m-asadujjaman](https://github.com/a-s-m-asadujjaman) to generate results for each time that simulation is launched, moreover correcting the memory leaks that other file had.You've to change the constant string of node_DC.cc(line 614) for each topology, I haven't had enough time to automate that aspect, it remains as a future improvement :smile:

## How to import parsed topologies to OMNeT++

**First of all, start OMNeT ++ and select as workspace the directory called "*Omnetpp_workspace*" created by the parser.**


![Alt Text](https://github.com/gistnetserv-uah/Tools/blob/master/parsers/topology/parser_omnetpp_amaruversion/parser_omnetpp/Images/1.JPG)


**We will see something like this, skip tutorials and overview.**


![Alt Text](https://github.com/gistnetserv-uah/Tools/blob/master/parsers/topology/parser_omnetpp_amaruversion/parser_omnetpp/Images/2.JPG)<!-- .element height="50%" width="50%" -->


**Now, we go to:** `File->Open Projects from File System... `


![Alt Text](https://github.com/gistnetserv-uah/Tools/blob/master/parsers/topology/parser_omnetpp_amaruversion/parser_omnetpp/Images/3.jpg)<!-- .element height="50%" width="50%" -->


**Select**: `Directory...` **we seek for our workspace *Omnetpp_workspace* and import it** 


![Alt Text](https://github.com/gistnetserv-uah/Tools/blob/master/parsers/topology/parser_omnetpp_amaruversion/parser_omnetpp/Images/4.JPG)<!-- .element height="50%" width="50%" -->


**We'll wait for OMNeT ++ to finish importing topologies**


![Alt Text](https://github.com/gistnetserv-uah/Tools/blob/master/parsers/topology/parser_omnetpp_amaruversion/parser_omnetpp/Images/5.jpg)<!-- .element height="50%" width="50%" -->


**It would be done :turtle:**


![Alt Text](https://github.com/gistnetserv-uah/Tools/blob/master/parsers/topology/parser_omnetpp_amaruversion/parser_omnetpp/Images/6.JPG)<!-- .element height="50%" width="50%" -->
