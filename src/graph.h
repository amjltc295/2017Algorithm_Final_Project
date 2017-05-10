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
		Node(const int& i);
		void addEdge(Edge *e);
		void sortEdge();

		int id;
        int color;
		bool traveled;
		vector<Edge *> edge;

		int d;
		Node *prev;
		int heap_idx;
};

class Graph{

	public:
		Graph(const string& n);
		~Graph();
		
		void addEdge(const int& v1, const int& v2, const int& w);
		void sortEdgesOfNode();
        void sortEdgesByWeight();
        void sortNodesByDegree();
        void sortNodesByID();
		void init();
		Node * getNodeById(const int& id);

		map<int, Node *> nodesMap;
        vector<Node *> nodes;
		vector<Edge *> edges;
		string name;
};

#endif
