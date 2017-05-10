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

class Node{

    public:
        //Node(const int& i);
        Node(const int& i, const int& x0, const int& y0, const int& x1, const int& y1);
        void addEdge(Edge *e);
        void sortEdge();

        int id;
        int x[2]; // x0 is the smaller one
	int y[2];
        int color;
        bool traveled;
        vector<Edge *> edge;

        int d;
        Node *prev;
        int heap_idx;
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

        map<int, Node *> nodesMap;
        vector<Node *> nodes;
        vector<Edge *> edges;
        int alpha;
        int beta;
        int omega;
};

#endif
