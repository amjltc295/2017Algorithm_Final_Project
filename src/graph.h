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
enum PaintColor { NON_PAINTED, RED, GREEN };

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
        vector<Edge *> edge;

        int d;
        Node *prev;
        int heap_idx;
};


class SubGraph
{
public:
    SubGraph();
    ~SubGraph();
    
    int colorDiff;
    bool colored;
    std::vector<Node *> subGraph;
    void flipColor();

};

// as the sliding window. The whole ckt layout would be divided into many small windows.
// in this project, we balancing only the vertices in each window.
// i.e. the balance btw Window is not concerned.
class Window
{
public:
    Window();
    ~Window();

    int index;
    int leftX, rightX;
    int leftY, rightY;
    std::map<int, SubGraph *> subGraphSet; // contain all the subgraph in one window.
};


class Graph{

    public:
        Graph(const int& a, const int& b, const int& o);
        ~Graph();
        
        void addNode(const int& i, const int& x0, const int& y0, const int& x1, const int& y1);
        void addEdge(const int& v1, const int& v2, const int& w);
        void printGraph();
        void sortEdgesOfNode();
        void sortEdgesByWeight();
        void sortNodesByDegree();
        void sortNodesByID();
        void init();
        Node * getNodeById(const int& id);

        // graph travel
        void DFS();
        bool DFS_visit(Node *u, PaintColor paintThisWith);
        void Find_Coloring_Bounding_Box(); // after find whether the nodes are 2 colorable, update the colorBoundBox. 

        // colorBoundBox[0] = x_left, [1] = x_right, [2] = y_up, [3] = y_down;
        int colorBoundBox[4];
        map<int, Node *> nodesMap;
        std::map<int, SubGraph *> wholeSubGraphMap;
        map<int, Window *> windowsMap;
        vector<Node *> nodes;
        vector<Edge *> edges;
        int alpha;
        int beta;
        int omega;
};


void Build_Color_Graph(Graph *); // run for a graph and add edge bewteen nodes if they are too close
void Output_Graph(Graph *,char * filepath); // output a .dot file to see if the Build_Color_Graph function runs properly. The dot file contain all nodes and edges between them if they are too close.

#endif
