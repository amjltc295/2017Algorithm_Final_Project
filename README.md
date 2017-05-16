# 2017Algorithm_Final_Project

## Synopsis

~~At the top of the file there should be a short introduction and/ or overview that explains **what** the project is. This description should match descriptions added for package managers (Gemspec, package.json, etc.)~~

NTUEE Algorithm 2017 Final Project

ICCAD 2015 Contest - Color Balancing for Double Patterning

## Algorithm

```
Read file

Add edges on the nodes (for x, y distance between nodes < alpha, beta) in Graph 

Do DFS on Graph
    Build Graph.wholeSubGraphMap. (Find the nodes connected by edge, create)
    Find the conflict. (Find whether the subraphs is 2-colorable)
    Find the bondage of color bounding box. (don't care about the conflict subgraphs)

Create windowsMap in Graph

For each SubGraph in Graph.wholeSubGraphMap
    Find windows which it belongs to
    Put each SubGraph into Window.subGraphMap

For each window in windowsMap
    For each subgraph in window.subGraphInWindow
        Do DFS find color difference of each SubGraph (subgraph.color_diff)
            Check if colored
    Sum colored nodesSet
    Do Greedy to balance (find minimum color differece, exclude colored nodes)
    


```

## Code Example

~~Show what the library does as concisely as possible, developers should be able to figure out **how** your project solves their problem by looking at the code example. Make sure the API you are showing off is obvious, and that your code is short and concise.~~

## Motivation

~~A short description of the motivation behind the creation and maintenance of the project. This should explain **why** the project exists.~~

## Installation

```
git clone https://github.com/amjltc295/2017Algorithm_Final_Project.git directory_name
cd directory_name
make
./DPT_balance_color ($input_file_name) ($input_file_name).out
```

~~Provide code examples and explanations of how to get the project.~~

## API Reference

~~Depending on the size of the project, if it is small and simple enough the reference docs can be added to the README. For medium size to larger projects it is important to at least provide a link to where the API reference docs live.~~

## Tests

~~Describe and show how to run the tests with code examples.~~

## Contributors

Ya-Liang Chang

~~Let people know how they can dive into the project, include important links to things like issue trackers, irc, twitter accounts if applicable.~~

## License

~~A short snippet describing the license (MIT, Apache, etc.)~~
