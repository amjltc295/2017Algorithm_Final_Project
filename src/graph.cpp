#include "graph.h"
#include <iostream>
#include <cassert>
#include <iomanip>
#include <string>
using namespace std;

//#define DEBUG
#ifdef DEBUG
#define DEBUG_MSG(str) do { cout << str; } while( false )
#else
#define DEBUG_MSG(str) do { } while ( false )
#endif

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
    area = (x[1]-x[0]) * (y[1]-y[0]);
    paintConflict = false; // valid = 1 means this subgraph is colorable
    inColorDensityWindow = false;
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

///////////////////// SubGraph Methods ///////////////////////
SubGraph::SubGraph()
{
    colored = false;
    totalArea = 0;
    maxAreaWindowIdAndArea.first = 0;
    maxAreaWindowIdAndArea.second = 0;

}

void SubGraph::IsColflict()
{
    for(int i = 0; i < subGraphNodes.size(); i++)
    {
        subGraphNodes[i]->paintConflict = true;
    }
}

bool SubGraph::flipColor()
{
    if (colored)
        return false;
    int subGraphNodesSize = subGraphNodes.size();
    for ( int i = 0; i < subGraphNodesSize; i++) {
        if (subGraphNodes[i]->paintColor == RED)
            subGraphNodes[i]->paintColor = GREEN;
        else if (subGraphNodes[i]->paintColor == GREEN)
            subGraphNodes[i]->paintColor = RED;
        else {
            cout << "Error in SubGraph::flipColor(): node not colored" << endl; 
            return false;
        }
    }
    map<int, int>::iterator iter;
    for (iter = colorDiff.begin(); iter != colorDiff.end(); ++iter) {
        iter->second *= -1;
    }
    
    return true;
}
/////////////// End of SubGraph Methods ///////////////////////



//////////////////// Window Methods //////////////////////////

Window::Window(const int& i, const int& x0, const int& x1, const int& y0, const int& y1, Graph * g)
{
    index = i;
    leftX = x0;
    rightX = x1;
    downY = y0;
    upY = y1;
    recursiveTime = 0;
    color_diff_sum = 0;
    color_A_density = 0;
    color_B_density = 0;
    gPtr = g;
}

//Add subgraph (a node set) to the window and calculate its colorDiff
void Window::addSubGraph(Graph *graph)
{
    int k=0; // just a index of subGraphSet.
    // For each subgraph in the Graph
    for ( int i = 0 ; i < graph->wholeSubGraph.size() ; i++ )
    {

        int subgraphTotalArea = 0;
        int total_red_area = 0; 
        int total_green_area = 0;
        bool subGraphInWindow = false;

        // For each node in the subgraph
        for ( int j = 0 ; j < graph->wholeSubGraph[i]->subGraphNodes.size() ; j++ )
        {
            // Used to check every node in this subgraph is in this window or not
            Node *thisnode = graph->wholeSubGraph[i]->subGraphNodes[j];
            subgraphTotalArea += thisnode->area;
            // Check if this node is 2-colorable
            if (thisnode->paintConflict){
                assert(j==0);
                break;
            }
            // Check if this node is in this window
            if ( ! ( thisnode->x[0] >= rightX || thisnode->x[1] <= leftX ) ) {
                if ( ! ( thisnode->y[0] >= upY || thisnode->y[1] <= downY ) ) {
                    subGraphInWindow = true;
                }
            }
            // Calculate area in this window for color_diff
            if ( thisnode->paintColor == RED ){
                total_red_area += AreaInTheWindow(thisnode);
            } else {
                total_green_area += AreaInTheWindow(thisnode);   
            }
        }
        if (subGraphInWindow) {
            // Add the subgraph into the subGraphSet of this window.
            subGraphSet[k] = graph->wholeSubGraph[i]; 
            subGraphSet[k]->colorDiff[index] = ( total_red_area - total_green_area );
            int subGraphAreaInThisWindow = total_red_area + total_green_area;
            subGraphSet[k]->areaInEachWindow[index] = subGraphAreaInThisWindow;
            // Update where the subgraph mainly exists
            if (subGraphAreaInThisWindow > subGraphSet[k]->maxAreaWindowIdAndArea.second) {
                pair<int, int> maxWindow(index, subGraphAreaInThisWindow);
                subGraphSet[k]->maxAreaWindowIdAndArea = maxWindow;
            }
            k++;
        }
    }
}

//Calculater the node's area in the window
int Window::AreaInTheWindow(Node *u)
{
    int left = ( u->x[0] > leftX ) ? u->x[0] : leftX;
    int right = ( u->x[1] < rightX ) ? u->x[1] : rightX;
    int up = ( u->y[1] < upY ) ? u->y[1] : upY;
    int down = ( u->y[0] > downY ) ? u->y[0] : downY;
    if ( left < right && down < up )
    {
        return (up-down)*(right-left);
    }
    else
    {
        return 0;
    }
}

void Window::calculateColorDensity() 
{
    int U = (rightX - leftX) * (upY - downY); // the area of the color density window
    int CA = 0; // the total area of RED-colored shapes in a color density window (excluding the portion outside the color density window))
    int CB = 0; // the total area of GREEN-colored shapes in a color density window (excluding the portion outside the color density window))
    map<int, SubGraph *>::iterator subGraph_iter;
    for (subGraph_iter = subGraphSet.begin(); subGraph_iter != subGraphSet.end(); subGraph_iter++){
        SubGraph * subGraphPtr = subGraph_iter->second;
        for (int i = 0; i < subGraphPtr->subGraphNodes.size(); i++) {
            Node * nodePtr = subGraphPtr->subGraphNodes[i];
            int nodeArea = AreaInTheWindow(nodePtr);
            //cout << "\tNode " << nodePtr->id << ", color " << nodePtr->paintColor << ", area" << nodeArea << endl;
            assert(nodePtr->paintColor == RED || nodePtr->paintColor == GREEN);
            if (nodePtr->paintColor == RED)
                CA += nodeArea;
            else
                CB += nodeArea;
        }
    }
    color_A_density = (float)CA / (float)U * 100;
    color_B_density = (float)CB / (float)U * 100;
    //cout << "Calculate Color Density of window " << index << ": CA " << CA << ", CB " << CB << ", U " << U << endl;
}

struct subGraphCompByColorDiff {
    subGraphCompByColorDiff (const int windowIndex){
        this->windowIndex = windowIndex;
    }
    bool operator () (SubGraph *A, SubGraph *B) {
        int colorDiffA = A->colorDiff.find(this->windowIndex)->second;
        int colorDiffB = B->colorDiff.find(this->windowIndex)->second;
        if ( colorDiffA > colorDiffB ) return true;
        return false;
    }
    int windowIndex;
};

/* 
 * Let (list of subgraphs in this window for colorDiff comparison, excluding colored ones) be color_diff_list
 * Sort the color_diff_list
 * Let (sum of the color_diff_list) be color_diff_sum
 * color_diff_sum = (sum of colorDiff of colored subgraphs)
 * //Let (min of sum of colorDiff) be min_color_diff_sum = MAX_INT
 * //For each_fixed = 0 to color_diff_list.size:
 *     For j = each_fixed to color_diff_list.size:  //Cancel; only one time is enough
 *         if  (color_diff_sum > 0):
 *             // SubGraph::flipColor()
 *             filp(subgraph of color_diff_list[j])
 *             color_dif_list[j] *= -1 
 *         color_diff_sum += color_diff_list[j]
 *         set subgraph_j colored
 *   //  if color_diff_sum < min_color_diff_sum:
 *     //    min_color_diff_sum = color_diff_sum  
 *         
 *
 */
int Window::greedyForColorBalancing() {
    recursiveTime += 1;
    if (recursiveTime > 2)
        return 0;
    DEBUG_MSG("\tWindow " << index << " greedy" << endl);
    DEBUG_MSG(" \tid "  << setw(8)  << "Area "  << " colored" << endl);
    vector<SubGraph *> color_diff_list;
    color_diff_sum = 0;
    map<int, SubGraph *>::iterator subGraph_iter;
    map<int, int>::iterator winId_iter;
    //Create color_diff_list excluding colored ones
    //Also sum the colorDiff of colored ones
    int count = 1;
    for (subGraph_iter = subGraphSet.begin(); subGraph_iter != subGraphSet.end(); subGraph_iter++) {
        DEBUG_MSG( "\t" << setw(3) << count++ << " " 
            << setw(8) << subGraph_iter->second-> colorDiff[index] <<  " " 
            << setw(6)<< subGraph_iter->second->colored << endl);
        // Do not flip if colored by other window
        if (subGraph_iter->second->colored)
            color_diff_sum += subGraph_iter->second->colorDiff[index];
        // Ignore if the subgraph area in this window is too small (cross-window optimization)
        else if (subGraph_iter->second->maxAreaWindowIdAndArea.second * 0.1 > subGraph_iter->second->areaInEachWindow[index]) {
            // color_diff_sum += subGraph_iter->second->colorDiff[index];
            // Wait for other window to deal with
            
        }
        //Flip the subGraph if its colorDiff is negative and it's not colored
        else {
            if (subGraph_iter->second->colorDiff[index] < 0) {
                assert(subGraph_iter->second->flipColor());
            }

            color_diff_list.push_back(subGraph_iter->second);
        }
    }
        
    sort(color_diff_list.begin(), color_diff_list.end(), subGraphCompByColorDiff(index));
    
    
    for ( int i = 0; i < color_diff_list.size(); i++ ) {
        SubGraph * subGraphPtr = color_diff_list[i];
        if ( color_diff_sum > 0 ) {
            assert(subGraphPtr->flipColor());
        }
        color_diff_sum += subGraphPtr->colorDiff[index];
        //set subgraph colored
        subGraphPtr->colored = true;

        // Recursively optimize other window, but hard to control
        /*
        int subGraphInDiffWindowSize = subGraphPtr->areaInEachWindow.size();
        if ( subGraphInDiffWindowSize > 1) {
            assert(subGraphPtr->maxAreaWindowIdAndArea.first == index);
            
            for (winId_iter = subGraphPtr->areaInEachWindow.begin(); winId_iter != subGraphPtr->areaInEachWindow.end(); winId_iter++) {
                int windowId = winId_iter->first;
                // Deal with other window
                if (windowId != index) {
                    // gPtr->windowsMap[windowId]->color_diff_sum +=  subgraph area in that window
                    if (gPtr->windowsMap[windowId]->greedyForColorBalancing())
                        DEBUG_MSG( "\t Window " << windowId << " greedy again, for Window " << index << endl);
                }
            }
        }
        */
        
    }
    return recursiveTime; //Useless now
    //}
}

///////////// End of Window Methods ////////////////////////////


//////////////////// Graph Methods ////////////////////////////

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
        cout << "Node v" << node->id 
            << ": x1=" << node->x[0] << ", " 
            << "y1=" << node->y[0] << ", "
            << "x2=" << node->x[1] << ", " 
            <<"y2=" << node->y[1] << ", "
            << "paintConflict=" << node->paintConflict << endl;
    }

}

void Graph::output_unbalanced_color_graph(char * filepath)
{
    fstream fout;
    fout.open(filepath,ios::out);//open output file
    fout << "ALPHA=" << alpha << endl << "BETA=" << beta << endl << "OMEGA=" << omega << endl << "NoNode=" << nodesMap.size() << endl;
    map<int, Node *>::iterator it;
    for ( it = nodesMap.begin() ; it != nodesMap.end() ; it++ )
    {
        Node *node = (*it).second;
        fout << node->x[0] << ',' << node->y[0] << ',' << node->x[1] << ',' << node->y[1] << "," << node->paintColor << endl;
    }
    for ( int i=0 ; i<windows.size() ; i++)
    {
        fout << windows[i]->leftX << ',' << windows[i]->downY << ',' << windows[i]->rightX << ',' << windows[i]->upY <<endl;
    }
    DEBUG_MSG("Written debug output to " << filepath);
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

void Graph::sortNodesByX1()
{
    map<int, Node *>::iterator itN;
    map<int, vector<Node *> >::iterator itN2;
    for ( itN = nodesMap.begin() ; itN != nodesMap.end() ; itN++ )
    {
        Node *node = (*itN).second;
        itN2 = X1_SortedNodeMap.find(node->x[1]);
        if(itN2 == X1_SortedNodeMap.end())
        {
            vector<Node *> tempvector;
            tempvector.push_back(node);
            X1_SortedNodeMap[node->x[1]] = tempvector;
        }
        else
        {
            X1_SortedNodeMap[node->x[1]].push_back(node);
        }
    }
    DEBUG_MSG("sort by X1, and there are "<< X1_SortedNodeMap.size() << " different X1.\n");
    for ( itN2 = X1_SortedNodeMap.begin() ; itN2 != X1_SortedNodeMap.end() ; itN2++ )
    {
        vector<Node *> tempvector = (*itN2).second;
        DEBUG_MSG("X1="<< (*itN2).first <<endl);
        for(int i=0 ; i < tempvector.size() ; i++)
        {
            DEBUG_MSG("\tnode" << tempvector[i]->id << endl);
        }
    }
}

void Graph::sortNodesByY1()
{
    map<int, Node *>::iterator itN;
    map<int, vector<Node *> >::iterator itN2;
    for ( itN = nodesMap.begin() ; itN != nodesMap.end() ; itN++ )
    {
        Node *node = (*itN).second;
        itN2 = Y1_SortedNodeMap.find(node->y[1]);
        if(itN2 == Y1_SortedNodeMap.end())
        {
            vector<Node *> tempvector;
            tempvector.push_back(node);
            Y1_SortedNodeMap[node->y[1]] = tempvector;
        }
        else
        {
            Y1_SortedNodeMap[node->y[1]].push_back(node);
        }
    }
    DEBUG_MSG("sort by Y1, and there are "<< Y1_SortedNodeMap.size() << " different Y1.\n");
    for ( itN2 = Y1_SortedNodeMap.begin() ; itN2 != Y1_SortedNodeMap.end() ; itN2++ )
    {
        vector<Node *> tempvector = (*itN2).second;
        DEBUG_MSG("Y1="<< (*itN2).first <<endl);
        for(int i=0 ; i < tempvector.size() ; i++)
        {
            DEBUG_MSG("\tnode" << tempvector[i]->id << endl);
        }
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

void Graph::Build_Color_Graph()
{
    cout << "Building graph ..." << endl;
    sortNodesByID();
    sortNodesByX1();
    sortNodesByY1();

    for(int i = 0; i < nodes.size(); i++)
    {
        DEBUG_MSG("\tnode" << i << " ,x0=" << nodes[i]->x[0] << endl);
        map<int, vector<Node *> >::iterator itN2;
        map<int, vector<Node *> >::iterator itN3;
        
        //processing x0
        {
            itN2 = X1_SortedNodeMap.find(nodes[i]->x[0] - alpha);
            itN3 = X1_SortedNodeMap.find(nodes[i]->x[0]);
            if( itN2 == X1_SortedNodeMap.end() )
            {
                vector<Node *> dummyvector;
                X1_SortedNodeMap[nodes[i]->x[0]-alpha] = dummyvector;
                itN2 = X1_SortedNodeMap.find(nodes[i]->x[0] - alpha);
            }
            if( itN3 == X1_SortedNodeMap.end() )
            {
                vector<Node *> dummyvector;
                X1_SortedNodeMap[nodes[i]->x[0]] = dummyvector;
                itN3 = X1_SortedNodeMap.find(nodes[i]->x[0]);
            }
            DEBUG_MSG("\t  " << (*itN2).first << endl);
            itN2++; // don't want to find the x[0]-alpha
            itN3++; // check until nodes[i]->x[0]
            for( itN2 ; itN2 != itN3 ; itN2++ )
            {
                DEBUG_MSG("\t\t  " << (*itN2).first << endl);
                vector<Node *> tempvector = (*itN2).second;
                for(int j = 0 ; j < tempvector.size() ; j++)
                {
                    DEBUG_MSG("\t\t    proccessing node" << nodes[i]->id << " and node" << tempvector[j]->id << endl);
                    if (!( tempvector[j]->y[0] >= nodes[i]->y[1] || tempvector[j]->y[1] <= nodes[i]->y[0] ))
                    {
                        addEdge(nodes[i]->id,tempvector[j]->id, 1);
                    }
                }
            }
        }

        //processing y0
        {
            itN2 = Y1_SortedNodeMap.find(nodes[i]->y[0] - beta);
            itN3 = Y1_SortedNodeMap.find(nodes[i]->y[0]);
            if( itN2 == Y1_SortedNodeMap.end() )
            {
                vector<Node *> dummyvector;
                Y1_SortedNodeMap[nodes[i]->y[0] - beta] = dummyvector;
                itN2 = Y1_SortedNodeMap.find(nodes[i]->y[0] - beta);
            }
            if( itN3 == Y1_SortedNodeMap.end() )
            {
                vector<Node *> dummyvector;
                Y1_SortedNodeMap[nodes[i]->y[0]] = dummyvector;
                itN3 = Y1_SortedNodeMap.find(nodes[i]->y[0]);
            }
            DEBUG_MSG("\t  " << (*itN2).first << endl);
            itN2++; // don't want to find the y[0]-beta
            itN3++; // check until nodes[i]->y[0]
            for( itN2 ; itN2 != itN3 ; itN2++ )
            {
                DEBUG_MSG("\t\t  " << (*itN2).first << endl);
                vector<Node *> tempvector = (*itN2).second;
                for(int j = 0 ; j < tempvector.size() ; j++)
                {
                    DEBUG_MSG("\t\t    proccessing node" << nodes[i]->id << " and node" << tempvector[j]->id << endl);
                    if (!( tempvector[j]->x[0] >= nodes[i]->x[1] || tempvector[j]->x[1] <= nodes[i]->x[0] ))
                    {
                        addEdge(nodes[i]->id,tempvector[j]->id,1);
                    }
                }
            }
        }
    }
    DEBUG_MSG("\tAdd edges between nodes. There are "<< edges.size() << " edges" << endl);
}

//// user defined ////////////////////////////////////////////
void Graph::DFS()
{
    cout << "Detecting non-2-colorable subgraphs with DFS....." << endl;
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
                //non2ColorableSubGraph.push_back(tempsubgraph);
                tempsubgraph->IsColflict();
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
            PaintColor colorForNext = (paintThisWith == RED) ? GREEN : RED; // pass diff color to next node
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

//Find the color bouding box of the whole graph.
//Coloring Bounding Box: A smallest bounding box that contains all colored shapes. Note that there is only one coloring bounding box for a given layout design. A coloring bounding box may contain non-colored shapes.
void Graph::Find_Coloring_Bounding_Box()
{
    cout << "Finding Coloring Bounding Box ... " << endl;
    // x0 is the smaller one
    // colorBoundBox[0] = x_left, [1] = x_right, [2] = y_up, [3] = y_down;
    colorBoundBox[0] = colorBoundBox[3] = DIS_INF;
    colorBoundBox[1] = colorBoundBox[2] = -DIS_INF;
    map<int, Node *>::iterator itN;
    for (itN = nodesMap.begin(); itN != nodesMap.end(); ++itN)
    {
        Node *node = (*itN).second;
        if ( node->paintConflict == false )
        {
            if ( node->x[0] < colorBoundBox[0] )
            {
                colorBoundBox[0] = node->x[0];
            }
            if ( node->x[1] > colorBoundBox[1] )
            {
                colorBoundBox[1] = node->x[1];
            }
            if ( node->y[0] < colorBoundBox[3] )
            {
                colorBoundBox[3] = node->y[0];
            }
            if ( node->y[1] > colorBoundBox[2] )
            {
                colorBoundBox[2] = node->y[1];
            }
        }
    }
    DEBUG_MSG( "\t Coloring Bounding Box: x0=" << colorBoundBox[0] 
            << " ,x1=" << colorBoundBox[1] 
            << " ,y0=" << colorBoundBox[3] 
            << " ,y1=" << colorBoundBox[2] <<endl);
    // Exclude the node outside the color bounding box
    DEBUG_MSG("Excluding node outside the Color Bounding Box ..." << endl);
    for (itN = nodesMap.begin(); itN != nodesMap.end(); ++itN)
    {
        Node * nodePtr = (*itN).second;
        if (nodePtr->x[0] > colorBoundBox[1]
                || nodePtr->x[1] < colorBoundBox[0]
                || nodePtr->y[0] > colorBoundBox[2]
                || nodePtr->y[1] < colorBoundBox[3]) {
            nodePtr->inColorDensityWindow = false;
        }
        else {
            nodePtr->inColorDensityWindow = true;
        }
    }
}

//Build the color density windows of the whole graph
//Color Density Window: It is a square inside a coloring bounding box.
void Graph::Build_Color_Density_Windows()
{
    cout << "Building Color Density Windows ... " << endl;
    int start_Y = colorBoundBox[3]; // mean the bottom y of window.
    int start_X = colorBoundBox[0]; // mean the left x of window.
    int i = 0; // mean the counter of window.
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
            window = new Window(i,start_X,start_X+omega,start_Y,start_Y+omega, this);
            windowsMap[i] = window;
            windows.push_back(window);

            window->addSubGraph(this); // add subgraphes to the window.

            i++;
            start_X += omega;
        }
        start_X = colorBoundBox[0];
        start_Y += omega;
    }
    DEBUG_MSG("\tThere are " << i << " Color Density Windows." << endl);
    map<int, Window *>::iterator itN;
    for (itN = windowsMap.begin(); itN != windowsMap.end(); ++itN)
    {
        Window *window=(*itN).second;
        DEBUG_MSG("\t\tWindow " << window->index
                << " leftX=" << window->leftX 
                << " rightX=" << window->rightX
                << " downY=" << window->downY 
                << " upY=" << window->upY << endl);
        DEBUG_MSG("\t\tThere are "<< window->subGraphSet.size() << " subgraph in this tindow: " << endl);
        map<int, SubGraph *>::iterator itN2;
        for (itN2 = window->subGraphSet.begin(); itN2 != window->subGraphSet.end(); ++itN2)
        {
            SubGraph *subgraphinwindow=(*itN2).second;
            DEBUG_MSG("\t\t\tIn subgraph "<<(*itN2).first<<" ");
            for ( int l=0 ; l< subgraphinwindow->subGraphNodes.size() ; l++ )
            {
                DEBUG_MSG(" Node " << subgraphinwindow->subGraphNodes[l]->id);
            }
            DEBUG_MSG(" , color diff=" << subgraphinwindow->colorDiff[window->index]<<endl);
        }
        DEBUG_MSG(endl);
    }
}

/*
 *  For each window in windowsMap
 *      For each subgraph in window.subGraphInWindow
 *          //Do DFS find color difference of each SubGraph (subgraph.color_diff) // Done in addSubGraph
 *          //Check if colored
 *      //Sum colored nodesSet
 *      Do Greedy to balance (find minimum color differece, exclude colored nodes)
 */
void Graph::Balance_Color()
{
    cout << "Balancing Color for each window ... " << endl;
    map<int, Window *>::iterator window_iter;
    // map<int, SubGraph *>::iterator subGraph_iter;
    for (window_iter = windowsMap.begin(); window_iter != windowsMap.end(); window_iter++) {
        Window * windowPtr = window_iter->second;
        /*
        for (subGraph_iter = windowPtr->subGraphSet.begin(); subGraph_iter != windowPtr->subGraphSet.end(); subGraph_iter++) {
            DFS_color_diff_subGraph(subGraph_iter->second);
        }
        */
        //Sum colored nodesSet

        //Do Greedy to balance
        windowPtr->greedyForColorBalancing();
        DEBUG_MSG("\tWindow " << windowPtr->index << " color_diff_sum: " << windowPtr->color_diff_sum << endl);
    }

    
    /*
    map<int, Node *>::iterator node_iter;
    for (node_iter = nodesMap.begin(); node_iter != nodesMap.end(); node_iter++) {
        cout << node_iter->first << ", " << node_iter->second->paintColor << endl;
    }
    */
}

void Graph::Output_Result(char * filename)
{
    fstream fout;
    fout.open(filename, ios::out);//open output file

    cout << "Outputing resulting ... " << endl << endl;
    map<int, Window *>::iterator window_iter;
    map<int, SubGraph *>::iterator subGraph_iter;

    // WIN[d]=x_bottom_leftd ,y_bottom_leftd ,x_top_rightd ,y_top_rightd(color_A_densityd color_B_densityd)
    // ...
    for (window_iter = windowsMap.begin(); window_iter != windowsMap.end(); window_iter++) {
        Window * windowPtr = window_iter->second;
        windowPtr->calculateColorDensity();
        fout << "WIN[" << windowPtr->index << "]=" 
            << windowPtr->leftX << ","
            << windowPtr->downY << ","
            << windowPtr->rightX << ","
            << windowPtr->upY << "("
            << setprecision(2) << fixed << windowPtr->color_A_density << " "
            << setprecision(2) << fixed << windowPtr->color_B_density << ")"
            << endl;
    }

    /*
    for (int i = 0; i < non2ColorableSubGraph.size(); i++) {
        SubGraph * subGraphPtr = non2ColorableSubGraph[i];
        fout << "NO[" << i+1 << "]=" << 
    }
    */
    fout << "GROUP" << endl;
    // NO[i]=x_bottom_lefti ,y_bottom_lefti ,x_top_righti ,y_top_righti
    // ...
    int count = 1;
    bool noConflict = true;
    for (int i = 0; i < nodes.size(); i++) {
        if ( !nodes[i]->inColorDensityWindow ) {
            fout << "NO[" << count << "]=" 
                << nodes[i]->x[0] << ","
                << nodes[i]->y[0] << ","
                << nodes[i]->x[1] << ","
                << nodes[i]->y[1] 
                << endl;

            count ++;
        }
    }
    // CA[a]=x_bottom_lefta ,y_bottom_lefta ,x_top_righta ,y_top_righta
    // ...
    // CB[b]=x_bottom_leftb ,y_bottom_leftb ,x_top_rightb ,y_top_rightb
    // ...
    int subGraphSize = wholeSubGraph.size();
    for (int i = 0; i < subGraphSize; i++) {
        fout << "GROUP" << endl;
        for (int c = RED; c <= GREEN; c++) {
            count = 1;
            SubGraph * subGraphPtr = wholeSubGraph[i];
            for (int j = 0; j < subGraphPtr->subGraphNodes.size(); j++) {
                Node * nodePtr = subGraphPtr->subGraphNodes[j];
                if (nodePtr->paintColor == c){
                    char t = ( (c == RED) ? 'A' : 'B' );
                    fout << "C" << t  << "[" << count << "]="
                        << nodePtr->x[0] << ","
                        << nodePtr->y[0] << ","
                        << nodePtr->x[1] << ","
                        << nodePtr->y[1] 
                        << endl;
                    count ++;
                }
            }
        }

    }
    /*
    map<int, Node *>::iterator node_iter;
    for (node_iter = nodesMap.begin(); node_iter != nodesMap.end(); node_iter++) {
        fout << node_iter->first << ", " << node_iter->second->paintColor << endl;

    */
    fout.close();
    string line;
    fstream myfile (filename);
    if (myfile.is_open()) {
        while ( getline (myfile,line) ) {
            cout << line << '\n';
        }
        myfile.close();
    }
}

//////////////////// End of Graph Methods ////////////////////////////


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
    fout.close();
}

