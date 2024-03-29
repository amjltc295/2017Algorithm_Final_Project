#ifndef GRAPH_H
#define GRAPH_H

#include <algorithm>
#include <vector>
#include <map>
#include <string>
#include <cstring>
#include <fstream>

#define DIS_INF 2147483647

using namespace std;

class Node;
class Graph;

class Edge{

    public:
        Edge(Node *a, Node *b, const int& w);
        Node* node[2];
        int weight;

        Node *getNeighbor(Node *n);

        bool operator < (const Edge& rhs) const;

};

// enumeration declartion
enum Color { WHITE, GRAY, BLACK };
enum PaintColor { NON_PAINTED, RED, GREEN};

class Node{

    public:
        //Node(const int& i);
        Node(const int& i, const int& x0, const int& y0, const int& x1, const int& y1);
        void addEdge(Edge *e);
        void sortEdge();

        int id;
        int x[2]; // x0 is the smaller one
        int y[2];

        Color color; // for DFS usage, check out enum Color above
        PaintColor paintColor; // for color balancing, check out enum PaintColor above

        bool colored; // if colored == true, skip balancing step.
        bool paintConflict; // paintConflict == true --> the subgraph cannot be paint with two color.
        bool inColorDensityWindow;
        vector<Edge *> edge;

        int d;
        Node *prev;
        int heap_idx;
        int area;
};


class SubGraph
{
public:
    SubGraph();
    ~SubGraph();
    
    void IsColflict(); // when doing DFS and find out there are color conflict, set paintConflict=True for all nodes in this subgraph.
    map<int, int> colorDiff; // this means that the color diffetence for different windows. < window->index , colorDiff >. and colordiff is RED-GREEN.
    map<int, int> areaInEachWindow; // <window->index, area in that window>
    bool colored; // if colored == true , we should filp the subgraph in the next window
    int totalArea;
    pair<int, int> maxAreaWindowIdAndArea; 
    vector<Node *> subGraphNodes;
    bool flipColor();

};

// as the sliding window. The whole ckt layout would be divided into many small windows.
// in this project, we balancing only the vertices in each window.
// i.e. the balance btw Window is not concerned.
class Window
{
public:
    Window(const int& i, const int& x0, const int& x1, const int& y0, const int& y1, Graph* g);
    ~Window();
    void addSubGraph(Graph *graph);
    int AreaInTheWindow(Node *u); //return the area of node u in this window.
    void calculateColorDensity();
//    bool subGraphCompByColorDiff(const SubGraph *A, const SubGraph *B);
    int greedyForColorBalancing();

    int index;
    int leftX, rightX;
    int upY, downY;
    int recursiveTime; // For cross-window optimization
    int color_diff_sum;
    float color_A_density; //A is Red
    float color_B_density; //B is Green
    Graph * gPtr; // To trace other window for cross-window optimization
    map<int, SubGraph *> subGraphSet; // contain all the subgraph in one window.
};


class Graph{

    public:
        Graph(const int& a, const int& b, const int& o);
        ~Graph();
        
        void addNode(const int& i, const int& x0, const int& y0, const int& x1, const int& y1);
        void addEdge(const int& v1, const int& v2, const int& w);
        void printGraph();
        void output_unbalanced_color_graph(char * filepath);
        void sortEdgesOfNode();
        void sortEdgesByWeight();
        void sortNodesByDegree();
        void sortNodesByID();
        void sortNodesByX1(); // sort nodes by x1 and store the order in X1_SortedNodeMap
        void sortNodesByY1(); // sort nodes by y1 and store the order in Y1_SortedNodeList
        void init();
        Node * getNodeById(const int& id);

        void Build_Color_Graph(); // run for a graph and add edge bewteen nodes if they are too close
        void DFS();    // graph travel
        bool DFS_visit(Node *u, PaintColor paintThisWith, SubGraph *tempsubgraph);
        void Find_Coloring_Bounding_Box(); // after find whether the nodes are 2 colorable, update the colorBoundBox. 
        void Build_Color_Density_Windows(); // after build the coloring bounding box, add the color density windows.
        void Balance_Color();  //Balance the color of each window using greedy algorithm
        void Output_Result(char * filename);

        // colorBoundBox[0] = x_left, [1] = x_right, [2] = y_up, [3] = y_down;
        int colorBoundBox[4];
        map<int, Node *> nodesMap;
        map<int, SubGraph *> wholeSubGraphMap;
        map<int, Window *> windowsMap;
        vector<Node *> nodes;
        vector<Edge *> edges;
        vector<Window *> windows;
        map<int, vector<Node *> > X1_SortedNodeMap; // map contain the ids of nodes sorted by x1 (right x)
        map<int, vector<Node *> > Y1_SortedNodeMap; // map contain the ids of nodes sorted by y1 (up y)
        vector<SubGraph *> wholeSubGraph;
        vector<SubGraph *> non2ColorableSubGraph;
        int alpha;
        int beta;
        int omega;
};


//void Build_Color_Graph(Graph *); // run for a graph and add edge bewteen nodes if they are too close
void Output_Graph(Graph *, char * filepath); // output a .dot file to see if the Build_Color_Graph function runs properly. The dot file contain all nodes and edges between them if they are too close.

#endif
