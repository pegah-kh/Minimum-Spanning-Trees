#include <cmath>
#include <algorithm>
#include <iostream>
#include <vector>
#include <chrono>
using namespace std::chrono;
#include "boruvka.hpp"

//what do we expect from our data structures?
//we need to access the weight of each edge, so the edge : first end,second end,weight
//for Union find we need to know the source (representer)

void boruvka(Graph *Graph){
    auto start = high_resolution_clock::now();
    int nNodes = Graph->nNodes;int nEdges = Graph->nEdges;
    struct Edge* Edge = Graph->edge;

    //subset structure is to keep the track of a set
    subset* subsets = new subset[nNodes];
    //and to keep the cheapest way from a subset(node in a subset) to outside we define:
    //it's actually the index of the corresponding edge
    int* cheapest = new int[nNodes];

    //initialize:
    for (int i = 0; i < nNodes; i++)
    {
        subsets[i].parent = i;
        subsets[i].rank = 0;
        cheapest[i] = -1;
    }
    
    //we will have a while and its conditions will depend on the number of subsets we have, at the end it will be one
    int nSets = nNodes;
    //and to save the weight of the MST
    int weightMst = 0;

    std::cout << "node1 : node2 : edge weight" << std::endl;

    while (nSets > 1)
    {
        //in every loop of this while we shoud go through each of the subsets and find the optimal way to connect it 
        //to another subset, the optimal way here means the cheapest edge from this subset to outside

        for (int i = 0; i < nNodes; i++)
        {
            cheapest[i] = -1;
        }//this is because in each loop we again look at each subset and each time this value probably changes

        for (int i = 0; i < nEdges; i++)//we go through all of the edges
        {
            int firstSet = find(subsets,Edge[i].start);
            int secondSet = find(subsets,Edge[i].end);


            if (firstSet == secondSet)
            {
                continue;//we don't need to do anything about this edge, it's already in the MST
            }else{
                if (cheapest[firstSet] == -1 || Edge[cheapest[firstSet]].weight > Edge[i].weight)
                {
                    cheapest[firstSet] = i;
                }
                if (cheapest[secondSet] == -1 || Edge[cheapest[secondSet]].weight > Edge[i].weight)
                {
                    cheapest[secondSet] = i;
                }
                
            }
        }

        //now we will add the choosen edges
        for (int i = 0; i < nNodes; i++)
        {
            if (cheapest[i] != -1)//if this is -1, it means that we already have the edge connected to this node in the MST OR the edges connected to this node are not optimal to connect two sets
            {
                int firstSet = find(subsets,Edge[cheapest[i]].start);
                int secondSet = find(subsets,Edge[cheapest[i]].end);
                if (firstSet == secondSet)//we have to check this condition because we may connect two sets not directly in the for
                {
                    continue;
                }else{
                    Union(subsets,firstSet,secondSet);
                    weightMst += Edge[cheapest[i]].weight;
                    nSets--;
                    std::cout << Edge[cheapest[i]].start << " " << Edge[cheapest[i]].end << " " << Edge[cheapest[i]].weight << std::endl;
                }
            }
            
        }
        
        
    }
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);
    std::cout << "the process took  " << duration.count() << "  micro seconds" << std::endl;
    std::cout << "the weight of MST is : " << weightMst << std::endl;
    return;
}

int find(struct subset* subsets, int i){

    //to find the node that represents the subset
    while (subsets[i].parent != i)
    {
        i = subsets[i].parent;
        /*subsets[i].parent = find(subsets,subsets[i].parent);*/
    }
    return i;
}

void Union(struct subset* subsets, int x, int y){
    int s1 = find(subsets,x);
    int s2 = find(subsets,y);

    if (s1 == s2){
        return;
    }

    if (subsets[s1].rank > subsets[s2].rank)
    {
        subsets[s2].parent = s1;
    }else{
        if (subsets[s1].rank < subsets[s2].rank)
        {
            subsets[s1].parent = s2;
        }else{
            subsets[s1].parent = s2;
            subsets[s2].rank++;
        }
    } 
}

void receiveGraph(Graph* Graph){
    int nNodes,nEdges;
    std::cout << "number of nodes? ";
    std::cin >> nNodes;
    std::cout << std::endl;
    std::cout << "number of edges? ";
    std::cin >> nEdges;
    Graph->nNodes = nNodes;
    Graph->nEdges = nEdges;
    Graph->edge = new Edge[Graph->nEdges];

    int first,second,weight;
    for (int i = 0; i < Graph->nEdges; i++)
    {
        std::cout << "first node of the " << i << "-em edge is ? ";
        std::cin >> first;
        std::cout << std::endl << "second node of the " << i << "-em edge is ? ";
        std::cin >> second;
        std::cout << std::endl << "the weight of this edge is : ";
        std::cin >> weight;
        std::cout << std::endl;

        (Graph->edge)[i].start = first;
        (Graph->edge)[i].end = second;
        (Graph->edge)[i].weight = weight;
    }
    
}


int main(){

    Graph *Graph = new struct Graph;
    receiveGraph(Graph);
    boruvka(Graph);
    return 0;
}