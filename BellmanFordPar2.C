#define WEIGHTED 1
#include "ligra.h"
#include <cilk/cilk.h>
#include <cilk/reducer_min.h>

const int nb_edges_cutoff = 5000;

template <class vertex>
void Compute(graph<vertex> GA, intT start) {
    intT n = GA.n;
    intT m = GA.m;
    //initialize ShortestPathLen to "infinity"
    int* ShortestPathLen = newA(int,n);
    {parallel_for(intT i=0;i<n;i++) ShortestPathLen[i] = INT_MAX/2;}
    ShortestPathLen[start] = 0;
    int* edge_from = newA(int, m);
    int* edge_to = newA(int, m);
    int* edge_w = newA(int, m);
    int cur = 0;

    for (int i = 0; i < n; i++) {
        for (int k = 0; k < GA.V[i].getInDegree(); k++) {
            edge_from[cur] = GA.V[i].getInNeighbor(k); 
            edge_to[cur] = i;
            edge_w[cur] = GA.V[i].getInWeight(k);
            cur++;
        }
    }
    for (int i = 0; i < n; i++) {
        bool updated = false;
        {cilk_for (int j = 0; j < m; j++) {
            int curValue = ShortestPathLen[edge_from[j]] + edge_w[j];
            if (ShortestPathLen[edge_to[j]] > curValue) {
                updated = true; 
                ShortestPathLen[edge_to[j]] = curValue; 
            }
        }}
        if (!updated) break;
    }
    for (int i = 0; i < n; i++) {
        std::cout << ShortestPathLen[i] << " ";
    }
    std::cout << endl;
    free(ShortestPathLen);
    free(edge_from);
    free(edge_to);
    free(edge_w);
}
