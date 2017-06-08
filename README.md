# 2017Algorithm_Final_Project

## Synopsis

~~At the top of the file there should be a short introduction and/ or overview that explains **what** the project is. This description should match descriptions added for package managers (Gemspec, package.json, etc.)~~

NTUEE Algorithm 2017 Final Project

[ICCAD 2015 Contest - Color Balancing for Double Patterning](http://cad-contest.el.cycu.edu.tw/problem_E/default.htm)

Build a graph and see if it's 2-colorable. Try to balance the two color in a color window.

## Directories & Files

```
ProblemE_test_case/         Officail test cases
bin/                        Execution file
lib/                        Supporting library for time usage
outputs/                    Output folder
plot_test_case/             Plot out input and output for visualization and debugging
    fast_balance_plot.sh        Shell to plot all balance-colored outputs
    fast_plot.sh                Shell to plot all inputs
    plot_picture.py             Plot all inputs
    plot_unbalanced.py          Plot all balance-colored outputs
src/                        Main source file
    Makefile                    Makefile
    graph.cpp                   All member functions
    graph.h                     Header file               
    main.cpp                    Main function
Makefile                    Makefile for all
README.md                   Readme file
fast_balance.sh             Shell to run all test cases for Linux

```

## Psudocode & Algorithm

Overall
```
Read file

Add edges on the nodes (for x, y distance between nodes < alpha, beta) in Graph 

// Graph::DFS()
Do DFS on Graph
    Build Graph.wholeSubGraphMap. (Find the nodes connected by edge, create)
    Find the conflict. (Find whether the subraphs is 2-colorable)
        If no confict, give RED and GREEN for later calculation of colorDiff

// Graph::Find_Coloring_Bounding_Box()
Find the bondage of color bounding box. (Exclude the conflict subgraphs)

// Graph::Build_Color_Density_Windos()
For each window
    Create windowsMap in Graph

    // Window::addSubGraph()
    For each SubGraph in Graph.wholeSubGraphMap
        Put each SubGraph into Window.subGraphMap it belongs to
    Find color difference of each SubGraph in each window (subgraph.color_diff)

    // Graph::Balance_Color()
    For each window in windowsMap
        For each subgraph in window.subGraphInWindow
            Find color difference of each SubGraph (subgraph.color_diff) //Done in addSubGraph
            //Check if colored
        //Sum colored nodesSet //Done in Greedy
        // Window::greedyForColorBalancing()
        Do Greedy to balance (find minimum color differece, exclude colored nodes)

// Graph::Output_Result()
For each window
    // Window::calculateColorDensity
    calcualte the color density
    print out the nodes with none, RED and Green color

```
Greedy // Window::greedyForColorBalancing()
```
Let (list of subgraphs in this window for colorDiff comparison, excluding colored ones) be color_diff_list
Flip all subGraph with negative colorDiff in color_diff_list
Sort the color_diff_list
Let (total color difference in this window) be color_diff_sum
color_diff_sum = (sum of colorDiff of colored subgraphs)
For i = each_fixed_index to color_diff_list.size:
    if  (color_diff_sum > 0):
        // SubGraph::flipColor()
        filp(subgraph of color_diff_list[i])
        //color_dif_list[i] *= -1 //Done in SubGraph::flipColor()
    color_diff_sum += color_diff_list[i]
    set subgraph[i] colored
```

## Installation

```
git clone https://github.com/amjltc295/2017Algorithm_Final_Project.git directory_name
cd directory_name
make
./DPT_balance_color ($input_file_name) ($input_file_name).out
```

## API Reference

~~Depending on the size of the project, if it is small and simple enough the reference docs can be added to the README. For medium size to larger projects it is important to at least provide a link to where the API reference docs live.~~

## Tests

For Linux
```
// Indivisual case
./DPT_balance_color ($input_file_name) ($input_file_name).out
// To run all test cases
bash fast_balance.sh
// To see the result plots
bash plot_test_case/fast_balance_plot.sh
```
~~Describe and show how to run the tests with code examples.~~

## Contributors

Ya-Liang Chang [amjltc295] (https://github.com/amjltc295)

Benjamin Kao [kaoben2731] (https://github.com/kaoben2731)


## License

MIT
