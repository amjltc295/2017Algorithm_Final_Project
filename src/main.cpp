// **************************************************************************
//  File       [main.cpp]
//  Author     [Yu-Hao Ho]
//  Synopsis   [The main program of 2015 Spring Algorithm PA2]
//  Modify     [2015/03/20 Yu-Hao Ho]
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
    //WorkerAnt workerAntSolver;

    char buffer[200];
    fstream fin(argv[1]);
    // fstream fout;
    //fout.open(argv[3],ios::out);
    
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


    //Read coordinates starting from the fourth line.
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
    //cout << x0 << ',' << y0 << ',' << x1 << ',' << y1 << endl;
    //graphPtr->printGraph();
    Build_Color_Graph(graphPtr);
    Output_Graph(graphPtr,argv[2]);
        

    /*
    int capacity, numOfFood;
    fin >> capacity >> numOfFood;
    workerAntSolver.setCapacity(capacity);
    workerAntSolver.setNumOfFood(numOfFood);
    for (int i=0; i<numOfFood; i++) {
        int x, y, w;
        vector<int> food;
        fin >> x >> y >> w;
        food.push_back(x); food.push_back(y); food.push_back(w);
        workerAntSolver.appendFood(food);
    }
    //workerAntSolver.printData();
    */

    //////////// find the solution to pick up the food ////
    tmusg.periodStart();

    // test only
    cout << "Painting conflict detection with DFS....." << endl;
    // test only
    graphPtr->DFS();
    graphPtr->printGraph();
    graphPtr->Find_Coloring_Bounding_Box();
    graphPtr->Build_Color_Dsnsity_Windows();

    //do color balancing

    tmusg.getPeriodUsage(stat);

    //////////// write the output file ///////////
    /*
    vector<int> foodIDList = workerAntSolver.getFoodIDList();
    for (int i=0; i<foodIDList.size(); i++) {
        fout << foodIDList[i] << endl;
    }
    fout << workerAntSolver.getTotalDistance() << endl;
    */
    fin.close();
    //fout.close();

    cout <<"# run time = " << (stat.uTime + stat.sTime) / 1000000.0 << "sec" << endl;
    cout <<"# memory =" << stat.vmPeak / 1000.0 << "MB" << endl;
    return 0;
}

