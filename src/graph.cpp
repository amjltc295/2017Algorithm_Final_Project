#include "graph.h"
#include <iostream>
using namespace std;


Edge::Edge(Node *a, Node *b, const int& w)
{
    if ( a->id <= b->id ) { node[0] = a; node[1] = b; }
    else { node[0] = b; node[1] = a; }
    weight = w;
}

bool Edge::operator < (const Edge& rhs) const{

    int id1a = node[0]->id;
    int id1b = node[1]->id;

    int id2a = rhs.node[0]->id;
    int id2b = rhs.node[1]->id;


    if ( id1a == id2a ) return id1b < id2b;
    if ( id1a == id2b ) return id1b < id2a;
    if ( id1b == id2a ) return id1a < id2b;
    if ( id1b == id2b ) return id1a < id2a;

    return weight < rhs.weight;
}
Node * Edge::getNeighbor(Node *n)
{
    if ( node[0] == n ) return node[1];
    if ( node[1] == n ) return node[0];

    return 0;   
}


Node::Node(const int& i, const int& x0, const int& y0, const int& x1, const int& y1)
{
    id = i;
    x[0] = x0; //x0 is the smaller one
    y[0] = y0;
    x[1] = x1;
    y[1] = y1;
    paintConflict = false; // valid = 1 means this subgraph is colorable
    color = WHITE;
    paintColor = NON_PAINTED;
    colored = false; // colored == 1 --> be colored yet
    d = DIS_INF;
    prev = 0;
}

void Node::addEdge(Edge *e)
{
    edge.push_back(e);
}

bool edgeComp( const Edge* A, const Edge* B ){
    if ( (*A) < (*B) ) return true;
    return false;
}


void Node::sortEdge()
{
    sort(edge.begin(), edge.end(), edgeComp);
}

Graph::Graph(const int& a, const int& b, const int& o)
{
    alpha = a;
    beta = b;
    omega = o;
}

Graph::~Graph()
{
    vector<Edge *>::iterator itE;
    for ( itE = edges.begin() ; itE != edges.end() ; itE++ )
    {
        delete (*itE);
        (*itE) = 0;
    }
    
    map<int, Node *>::iterator itN;
    for ( itN = nodesMap.begin() ; itN != nodesMap.end() ; itN++ )
    {
        delete (*itN).second;
        (*itN).second = 0;
    }

    /*vector<Node *>::iterator itN2;
    for ( itN2 = nodes.begin() ; itN2 != nodes.end() ; itN2++ )
    {
        (*itN2) = 0;
    }*/
}

void Graph::addNode(const int& i, const int& x0, const int& y0, const int& x1, const int& y1)
{
    Node *node;
    node = new Node(i, x0, y0, x1, y1);
    nodesMap[i] = node;
    nodes.push_back(node);

}

void Graph::addEdge(const int& v1, const int& v2, const int& w)
{
    Node *a, *b;
    map<int, Node *>::iterator it = nodesMap.find(v1);
    if ( it != nodesMap.end() )
        a = (*it).second;
    else
    {
        fprintf(stderr, "**ERROR Graph::addEdge(): ");
        fprintf(stderr, "No node with such id\n");
    }

    it = nodesMap.find(v2);
    if ( it != nodesMap.end() )
        b = (*it).second;
    else
    {
        fprintf(stderr, "**ERROR Graph::addEdge(): ");
        fprintf(stderr, "No node with such id\n");
    }

    Edge *e = new Edge(a, b, w);
    edges.push_back(e);

    a->edge.push_back(e);
    b->edge.push_back(e);

}

void Graph::printGraph()
{
    
    cout << "alpha:" << alpha << " / beta:" << beta << " / omega:" << omega << endl;
    map<int, Node *>::iterator it;
    for ( it = nodesMap.begin() ; it != nodesMap.end() ; it++ )
    {
        Node *node = (*it).second;
    	cout << "x1=" << node->x[0] << ',' << "y1=" << node->y[0] << ',' << "x2=" << node->x[1] << ',' <<"y2=" << node->y[1] << endl;
        cout << "paintConflict = " << node->paintConflict << endl << endl;
    }

}
    
void Graph::sortEdgesOfNode()
{
    map<int, Node *>::iterator it;
    for ( it = nodesMap.begin() ; it != nodesMap.end() ; it++ )
    {
        Node *node = (*it).second;
        node->sortEdge();
    }

}

bool edgeCompByW( const Edge* A, const Edge* B ){
    if ( A->weight < B->weight ) return true;
    return false;
}

void Graph::sortEdgesByWeight()
{
    sort(edges.begin(), edges.end(), edgeCompByW);
}


bool NodeCompByD( const Node* A, const Node* B ){
    if ( A->edge.size() > B->edge.size() ) 
        return true;
    else if (A->edge.size() == B->edge.size()) {
        if (A->id < B->id) 
            return true;
        else 
            return false;
    }
    else
        return false;
}

void Graph::sortNodesByDegree()
{
    sort(nodes.begin(), nodes.end(), NodeCompByD);
}


bool NodeCompByID( const Node* A, const Node* B ){
    if ( A->id < B->id ) return true;
    else return false;
}

void Graph::sortNodesByID()
{
    sort(nodes.begin(), nodes.end(), NodeCompByID);
}
void Graph::init()
{
    map<int, Node *>::iterator itN;
    for ( itN = nodesMap.begin() ; itN != nodesMap.end() ; itN++ )
    {
        Node *node = (*itN).second;
        node->d = DIS_INF;
        node->prev = 0;
        node->color = WHITE;
        node->colored = false;
        node->paintColor = NON_PAINTED;
        node->paintConflict = false;
    }
    
}

Node * Graph::getNodeById(const int& id)
{
    return nodesMap[id];
}

void Build_Color_Graph(Graph *graph)
{
    graph->sortNodesByID();
    for(int i=0;i<graph->nodes.size();i++)
    {
        for(int j=i+1;j<graph->nodes.size();j++)
        {
            int ax1=graph->nodes[i]->x[0];
            int ax2=graph->nodes[i]->x[1];
            int ay1=graph->nodes[i]->y[0];
            int ay2=graph->nodes[i]->y[1];
            int bx1=graph->nodes[j]->x[0];
            int bx2=graph->nodes[j]->x[1];
            int by1=graph->nodes[j]->y[0];
            int by2=graph->nodes[j]->y[1];
            int alpha=graph->alpha;
            int beta=graph->beta;
            if( ( bx1<=ax1 && bx2>ax1 ) || ( bx2>=ax2 && bx1<ax2) || ( bx1>=ax1 && bx2<=ax2 ))
            {
                if( ( by2 < ay1 && by2 > (ay1-beta) ) || ( by1 > ay2 && by1 < (ay2+beta) ) )
                {
                    graph->addEdge(i,j,1);
                }
            }
            if( ( by1<=ay1 && by2>ay1 ) || ( by2>=ay2 && by1<ay2) || ( by1>=ay1 && by2<=ay2 ))
            {
                if( ( bx2 < ax1 && bx2 > (ax1-alpha) ) || ( bx1 > ax2 && bx1 < (ax2+alpha) ) )
                {
                    graph->addEdge(i,j,1);
                }
            }
        }
    }
    cout<<"there are"<<graph->edges.size()<<"edges\n";
}

void Output_Graph(Graph * graph,char * filepath)
{
    fstream fout;
    fout.open(filepath,ios::out);//open output file
    fout<<"// SCC from specific input"<<endl;
    fout<<"graph {"<<endl;
    vector<int> outputed;
    for(int i=0;i<graph->nodes.size();i++)
    {
        outputed.push_back(0);
    }
    for(int i=0;i<graph->edges.size();i++)
    {
        fout<<"v"<<graph->edges[i]->node[0]->id<<" -- v"<<graph->edges[i]->node[1]->id<<";"<<endl;
        outputed[graph->edges[i]->node[0]->id]=1;
        outputed[graph->edges[i]->node[1]->id]=1;
    }
    for(int i=0;i<graph->nodes.size();i++)
    {
        if(outputed[i]==0)
        {
            fout<<"v"<<i<<";"<<endl;
        }
    }
    fout<<"}";
}

//////////////////////////////////////////////////////////////
//// user defined ////////////////////////////////////////////
void Graph::DFS()
{
    // test only
    cout << "Graph::DFS()..........." << endl;
    // test only

    Graph::init();
    // int time = 0;
    map<int, Node *>::iterator itN;
    for (itN = nodesMap.begin(); itN != nodesMap.end(); ++itN)
    {
        Node *node = (*itN).second;
        // test only
        cout << "Graph::DFS(): node = " << node->id << endl;
        cout << "Graph::DFS(): node.color = " << node->color << endl;
        // test only
        if (node->color == WHITE)
        {
            // test only
            cout << "Graph::DFS(): node = " << node->id << endl;
            // test only

            // DFS_visit(graph, node, time);
            node->paintColor = RED;
            if (~DFS_visit(node, GREEN))
                node->paintConflict = true;
        }
    }
}

// bool Graph::DFS_visit(Node *u, int &time)
// return false if paint conflict
// we pass the color for next node in DFS using paintThisWith
bool Graph::DFS_visit(Node *u, PaintColor paintThisWith)
{
    // test only
    cout << "Graph::DFS_visit........" << endl;
    // test only

    // time = time + 1;
    // u->d = time;
    u->color = GRAY;
    u->paintColor = paintThisWith;
    PaintColor adjColor = NON_PAINTED;
    vector<Edge *>::iterator itE;
    for (itE = u->edge.begin(); itE != u->edge.end(); ++itE)
    {
        Node *v = (*itE)->getNeighbor(u);

        // test only
        cout << "Graph::DFS_visit: adj v = " << v->id << endl;
        // test only

        adjColor = NON_PAINTED;
        if (v->color == WHITE)
        {
            v->prev = u;
            PaintColor colorForNext = (paintThisWith == RED)? GREEN : RED; // pass diff color to next node
            if (~DFS_visit(v, colorForNext)) {
                u->paintConflict = true;
                return false;
            }
        } else {
            // test only
            cout << "adjColor = " << adjColor << endl;
            // test only
            if (adjColor == NON_PAINTED) {
                adjColor = v->paintColor;
            } else {
                if (adjColor != v->paintColor) {
                    u->paintConflict = true;
                    return false;
                }
            }
        }
    }

    u->color = BLACK;
    // u->paintColor = (adjColor == NON_PAINTED || adjColor == RED)? GREEN : RED;
    // time = time + 1;
    // u.f = time;
    return true;
}