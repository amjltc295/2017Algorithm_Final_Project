#include "graph.h"

void DFS_color_diff_subGraph(SubGraph * subGraphPtr) {

}

bool subGraphCompByColorDiff ( const SubGraph * A, const SubGraph * B) {
    if ( A->colorDiff > B->colorDiff ) return true;
    else return false;
}

/* 
 * Let (list of color_diff of subgraphs in this window) = color_diff_list
 * Sort the color_diff_list
 * Let (sum of the color_diff_list) = color_diff_sum = 0
 * For each_fixed = 0 to color_diff_list.size:
 *     For i = each_fixed to color_diff_list.size:
 *         if (subgrph of i not colored) && (color_diff_sum > 0):
 *             filp(subgraph of color_diff_list[i])
 *             color_dif_list[i] *= -1
 *         color_diff_sum += color_diff_list[i]
 *         
 *
 */
void greedyForColorBalance(Window * windowPtr) {
    vector<SubGraph *> color_diff_list;
    int color_diff_sum = 0;
    map<int, SubGraph *>::iterator subGraph_iter;
    for (subGraph_iter = windowPtr->subGraphSet.begin(); subGraph_iter != windowPtr->subGraphSet.end(); subGraph_iter++) {
        color_diff_list.push_back(subGraph_iter->second);
    }
        
    sort(color_diff_list.begin(), color_diff_list.end(), subGraphCompByColorDiff);
    
}

/*
 *  For each window in windowsMap
 *      For each subgraph in window.subGraphInWindow
 *          Do DFS find color difference of each SubGraph (subgraph.color_diff)
 *          Check if colored
 *      Sum colored nodesSet
 *      Do Greedy to balance (find minimum color differece, exclude colored nodes)
 */
void colorBalance(Graph * graphPtr) {

    map<int, Window *>::iterator window_iter;
    map<int, SubGraph *>::iterator subGraph_iter;
    for (window_iter = graphPtr->windowsMap.begin(); window_iter != graphPtr->windowsMap.end(); window_iter++) {
        Window * windowPtr = window_iter->second;
        for (subGraph_iter = windowPtr->subGraphSet.begin(); subGraph_iter != windowPtr->subGraphSet.end(); subGraph_iter++) {
            DFS_color_diff_subGraph(subGraph_iter->second);
        }
        //Sum colored nodesSet

        //Do Greedy to balance
        greedyForColorBalance(windowPtr);

    }
}


