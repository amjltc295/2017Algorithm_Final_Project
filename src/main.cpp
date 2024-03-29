// **************************************************************************
//  File       [main.cpp]
//  Author     [Ya-Liang Chang, Albert Tong, Benjamin Kao]
//  Synopsis   [The main program of 2017 Spring Algorithm Group 3 Final Project]
//  Modify     [2017/06/19 Benjamin Kao]
// **************************************************************************

#include <stdlib.h>
#include <cstring>
#include <iostream>
#include <sstream>
#include <fstream>
#include "../lib/tm_usage.h"
#include "graph.h"


using namespace std;
using namespace CommonNs;

void help_message() {
    cout << "ICCAD 2015 Contest - Color Balancing for Double Patterning" << endl;
    cout << "usage: ./DPT_balance_color ($input_file_name) ($input_file_name).out" << endl;
}

int main(int argc, char* argv[])
{
    if(argc != 3) {
       help_message();
       return 0;
    }
    CommonNs::TmUsage tmusg;
    CommonNs::TmStat stat;

    //////////// read the input file /////////////

    char buffer[200];
    fstream fin(argv[1]);
    fstream fout;
    fout.open(argv[2],ios::out);
    
    //Read alpha, beta, omega from the first three lines
    //For example: 
    //ALPHA=50
    //BETA=100
    //OMEGA=900
    //
    int alpha, beta, omega;
    fin.getline(buffer, 6, '=');
    fin.getline(buffer, 200);
    alpha = atoi(buffer);

    fin.getline(buffer, 5, '=');
    fin.getline(buffer, 200);
    beta = atoi(buffer);

    fin.getline(buffer, 7, '=');
    fin.getline(buffer, 200);
    omega = atoi(buffer);


    ///////// Read coordinates starting from the fourth line. //////////////
    //x0,y0,x1,y1, where x0 and y0 are the smaller ones.
    //For example:
    //0,200,185,260
    Graph* graphPtr = new Graph(alpha, beta, omega);
    int x0, y0, x1, y1, i=0;
    string textline, t;
    while ( getline(fin, textline)) { //fin.getline(buffer, 200) ) {
        stringstream ss(textline);
        getline(ss, t, ',');
        x0 = atoi(t.c_str());
        getline(ss, t, ',');
        y0 = atoi(t.c_str());
        getline(ss, t, ',');
        x1 = atoi(t.c_str());
        getline(ss, t);
        y1 = atoi(t.c_str());
        graphPtr->addNode(i, x0, y0, x1, y1);
        i++;
    }
    ////////// End of read file /////////


    ////////// do color balancing /////////
    tmusg.periodStart();
    graphPtr->Build_Color_Graph();
    graphPtr->DFS();
    // #ifdef DEBUG
    // Output_Graph(graphPtr,argv[2]); // this line output the .dot file for checking the edges
    // #endif
    #ifdef DEBUG
    graphPtr->printGraph();
    #endif
    graphPtr->Find_Coloring_Bounding_Box();
    graphPtr->Build_Color_Density_Windows();
    graphPtr->Balance_Color();
    #ifndef DEBUG
    graphPtr->Output_Result(argv[2]);
    #else    
    graphPtr->output_unbalanced_color_graph(argv[2]);
    #endif


    tmusg.getPeriodUsage(stat);

    fin.close();

    cout << endl;
    cout <<"# run time = " << (stat.uTime + stat.sTime) / 1000000.0 << "sec" << endl;
    cout <<"# memory =" << stat.vmPeak / 1000.0 << "MB" << endl;
    return 0;
}

