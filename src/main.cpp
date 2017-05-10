// **************************************************************************
//  File       [main.cpp]
//  Author     [Yu-Hao Ho]
//  Synopsis   [The main program of 2015 Spring Algorithm PA2]
//  Modify     [2015/03/20 Yu-Hao Ho]
// **************************************************************************

#include <cstdlib>
#include <cstring>
#include <iostream>
#include <fstream>
#include "../lib/tm_usage.h"
#include "graph.h"

using namespace std;

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
    WorkerAnt workerAntSolver;

    char buffer[200];
    fstream fin(argv[2]);
    fstream fout;
    fout.open(argv[3],ios::out);
    //fin.getline(buffer,200);
    //fin.getline(buffer,200);
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

    //////////// find the solution to pick up the food ////
    tmusg.periodStart();

    //do color balancing

    tmusg.getPeriodUsage(stat);

    //////////// write the output file ///////////
    vector<int> foodIDList = workerAntSolver.getFoodIDList();
    for (int i=0; i<foodIDList.size(); i++) {
        fout << foodIDList[i] << endl;
    }
    fout << workerAntSolver.getTotalDistance() << endl;
    fin.close();
    fout.close();

    cout <<"# run time = " << (stat.uTime + stat.sTime) / 1000000.0 << "sec" << endl;
    cout <<"# memory =" << stat.vmPeak / 1000.0 << "MB" << endl;

    return 0;
}

