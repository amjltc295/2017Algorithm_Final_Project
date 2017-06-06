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
    	cout << "Node v" << node->id << ": x1=" << node->x[0] << ',' << "y1=" << node->y[0] << ',' << "x2=" << node->x[1] << ',' <<"y2=" << node->y[1] << endl;
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

bool NodeCompByX0( const Node* A, const Node* B )
{
    if ( A->x[0] < B->x[0] )
    {
        return true;
    }
    else
    {
        return false;
    }
}

void Graph::sortNodesByX0()
{
    sort(nodes.begin(), nodes.end(), NodeCompByX0);
    for ( int i=0 ; i < nodes.size() ; i++ )
    {
        X_SortedNodeList.push_back(nodes[i]->id);
    }
}

bool NodeCompByY0( const Node* A, const Node* B )
{
    if ( A->y[0] < B->y[0] )
    {
        return true;
    }
    else
    {
        return false;
    }
}

void Graph::sortNodesByY0()
{
    sort(nodes.begin(), nodes.end(), NodeCompByY0);
    for ( int i=0 ; i < nodes.size() ; i++ )
    {
        Y_SortedNodeList.push_back(nodes[i]->id);
    }
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
    cout<<"Add edges between nodes.\nthere are "<<graph->edges.size()<<" edges\n";
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
    int i=0; // this just a index of wholeSubGraphMap.
    Graph::init();
    map<int, Node *>::iterator itN;
    for (itN = nodesMap.begin(); itN != nodesMap.end(); ++itN)
    {
        Node *node = (*itN).second;
        SubGraph *tempsubgraph; // this is the temporary subgraph to store DFSed nodes, and it will be store in wholeSubGraph and wholeSubGraphMap after check that these nodes are 2-colorable.
        tempsubgraph = new SubGraph();
        if (node->color == WHITE) {
            if (DFS_visit(node, RED, tempsubgraph) == false) {
                node->paintConflict = true;
            }
            else
            {
                // ensured thar there is no conflict in this subgraph, add it into the wholeSubGraph
                wholeSubGraphMap[i] = tempsubgraph;
                wholeSubGraph.push_back(tempsubgraph);
                i++;
            }
        }
    }
}

// bool Graph::DFS_visit(Node *u, int &time)
// return false if paint conflict
// we pass the color for next node in DFS using paintThisWith
bool Graph::DFS_visit(Node *u, PaintColor paintThisWith, SubGraph *tempsubgraph)
{
    // time = time + 1;
    // u->d = time;
    u->color = GRAY;
    u->paintColor = paintThisWith;
    vector<Edge *>::iterator itE;
    for (itE = u->edge.begin(); itE != u->edge.end(); ++itE)
    {
        Node *v = (*itE)->getNeighbor(u);
        if (v->color == WHITE)
        {
            v->prev = u;
            PaintColor colorForNext = (paintThisWith == RED)? GREEN : RED; // pass diff color to next node
            if (DFS_visit(v, colorForNext, tempsubgraph) == false) {
                u->paintConflict = true;
                return false;
            }
        } else {
            if (u->paintColor == v->paintColor) {
                u->paintConflict = true;
                return false;
            }
        }
    }

    u->color = BLACK;
    tempsubgraph->subGraphNodes.push_back(u); // after u goes to black, that means the DFS has been done on u, and we shold put u into the tempsubgraph.
    return true;
}

void Graph::Find_Coloring_Bounding_Box()
{
    // x0 is the smaller one
    // colorBoundBox[0] = x_left, [1] = x_right, [2] = y_up, [3] = y_down;
    colorBoundBox[0]=colorBoundBox[3]=DIS_INF;
    colorBoundBox[1]=colorBoundBox[2]=-DIS_INF;
    map<int, Node *>::iterator itN;
    for (itN = nodesMap.begin(); itN != nodesMap.end(); ++itN)
    {
        Node *node = (*itN).second;
        if ( node->paintConflict == false )
        {
            if ( node->x[0] < colorBoundBox[0] )
            {
                colorBoundBox[0]=node->x[0];
            }
            if ( node->x[1] > colorBoundBox[1] )
            {
                colorBoundBox[1]=node->x[1];
            }
            if ( node->y[0] < colorBoundBox[3] )
            {
                colorBoundBox[3]=node->y[0];
            }
            if ( node->y[1] > colorBoundBox[2] )
            {
                colorBoundBox[2]=node->y[1];
            }
        }
    }
    cout << "Find coloring bounding box:\nx0=" << colorBoundBox[0] << " ,x1=" << colorBoundBox[1] << " ,y0=" << colorBoundBox[3] << " ,y1=" << colorBoundBox[2] <<endl;
}

Window::Window(const int& i, const int& x0, const int& x1, const int& y0, const int& y1)
{
    index = i;
    leftX = x0;
    rightX = x1;
    downY = y0;
    upY = y1;
}

void Graph::Build_Color_Dsnsity_Windows()
{
    int start_Y=colorBoundBox[3]; // mean the down y of window.
    int start_X=colorBoundBox[0]; // mean the left x of window.
    int i=0; // mean the counter of window.
    while ( start_Y<colorBoundBox[2] )
    {
        if ( start_Y+omega > colorBoundBox[2] )
        {
            start_Y=colorBoundBox[2]-omega;
        }
        while ( start_X<colorBoundBox[1] )
        {
            if ( start_X+omega>colorBoundBox[1] )
            {
                start_X=colorBoundBox[1]-omega;
            }
            Window *window;
            window = new Window(i,start_X,start_X+omega,start_Y,start_Y+omega);
            windowsMap[i] = window;
            windows.push_back(window);

            window->addSubGraph(this); // add subgraphes to the window.

            i++;
            start_X+=omega;
        }
        start_X=colorBoundBox[0];
        start_Y+=omega;
    }
    cout << "\nThere are " << i << " color density windows.\n\n";
    map<int, Window *>::iterator itN;
    for (itN = windowsMap.begin(); itN != windowsMap.end(); ++itN)
    {
        Window *window=(*itN).second;
        cout << "Window " << window->index << " leftX=" << window->leftX << " rightX=" << window->rightX << " downY=" << window->downY << " upY=" << window->upY << endl;
      	cout << "There are "<< window->subGraphSet.size() << " subgraph in this tindow.\n\n";
      	map<int, SubGraph *>::iterator itN2;
	    for (itN2 = window->subGraphSet.begin(); itN2 != window->subGraphSet.end(); ++itN2)
	    {
	    	SubGraph *subgraphinwindow=(*itN2).second;
	    	cout<<"In subgraph "<<(*itN2).first<<" ";
	    	for ( int l=0 ; l< subgraphinwindow->subGraphNodes.size() ; l++ )
	    	{
	    		cout << " Node " << subgraphinwindow->subGraphNodes[l]->id;
	    	}
	    	cout << " , color diff=" << subgraphinwindow->colorDiff[window->index]<<endl;
	    }
	    cout<<endl;
    }
}

void Window::addSubGraph(Graph *graph)
{
    int k=0; // just a index of subGraphSet.
    for ( int i=0 ; i < graph->wholeSubGraph.size() ; i++ )
    {
        for ( int j=0 ; j < graph->wholeSubGraph[i]->subGraphNodes.size() ; j++ )
        {
            Node *thisnode=graph->wholeSubGraph[i]->subGraphNodes[j]; // used to check every node in this subgraph is in this window or not
            if ( ! ( thisnode->paintConflict ) )
            {
                if ( ! ( thisnode->x[0] >= rightX || thisnode->x[1] <= leftX ) )
                {
                    if ( ! ( thisnode->y[0] >= upY || thisnode->y[1] <= downY ) )
                    {
                        // thisnode is 2-paintable and is in this window
                        subGraphSet[k]=graph->wholeSubGraph[i]; // add the subgraph into the subGraphSet of this window.
                        int total_red_area=0; // calculate the color difference "in this window",and store it into the subgraph's colorDiff
                        int total_green_area=0;
                        for( int l=0 ; l<subGraphSet[k]->subGraphNodes.size() ; l++ )
                        {
                            if ( subGraphSet[k]->subGraphNodes[l]->paintColor == RED )
                            {
                                total_red_area+=AreaInTheWindow(subGraphSet[k]->subGraphNodes[l]);
                            }
                            else
                            {
                                total_green_area+=AreaInTheWindow(subGraphSet[k]->subGraphNodes[l]);   
                            }
                        }
                        subGraphSet[k]->colorDiff[index] = ( total_red_area - total_green_area );
                        k++;
                        break;
                    }
                }
            }
        }
    }
}

int Window::AreaInTheWindow(Node *u)
{
    int left=( u->x[0] > leftX ) ? u->x[0] : leftX;
    int right=( u->x[1] < rightX ) ? u->x[1] : rightX;
    int up=( u->y[1] < upY ) ? u->y[1] : upY;
    int down=( u->y[0] > downY ) ? u->y[0] : downY;
    if ( left < right && down < up )
    {
        return (up-down)*(right-left);
    }
    else
    {
        return 0;
    }
}

SubGraph::SubGraph()
{}