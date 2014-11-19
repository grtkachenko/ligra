#define WEIGHTED 1
#include "ligra.h"
#include <cilk/cilk.h>
#include <cilk/reducer_min.h>

const int nb_edges_cutoff = 5000;

template <class vertex>
void Compute(graph<vertex> GA, intT start) {
    intT n = GA.n;
    //initialize ShortestPathLen to "infinity"
    int* ShortestPathLen = newA(int,n);
    {parallel_for(intT i=0;i<n;i++) ShortestPathLen[i] = INT_MAX/2;}
    ShortestPathLen[start] = 0;
    bool* BigDegree = newA(bool, n);
    for (int i = 0; i < n; i++) {
        BigDegree[i] = GA.V[i].getInDegree() > nb_edges_cutoff;
    }
    for (int i = 0; i < n; i++) {
        bool updated = false;
        
        {for (int j = 0; j < n; j++) {
            if (!BigDegree[j]) {
                int curMin = INT_MAX / 2;
                for (int k = 0; k < GA.V[j].getInDegree(); k++) {
                    curMin = min(curMin, ShortestPathLen[GA.V[j].getInNeighbor(k)] + GA.V[j].getInWeight(k));
                }
                if (curMin < ShortestPathLen[j]) {
                    updated = true;
                    ShortestPathLen[j] = curMin; 
                }
            } else {
                cilk::reducer<cilk::op_min<int> > curMin;
                cilk_for (int k = 0; k < GA.V[j].getInDegree(); k++) {
                    curMin->calc_min(ShortestPathLen[GA.V[j].getInNeighbor(k)] + GA.V[j].getInWeight(k));
                }
                if (curMin.get_value() < ShortestPathLen[j]) {
                    updated = true;
                    ShortestPathLen[j] = curMin.get_value(); 
                }
            }
        }}
        if (!updated) break;
    }
    for (int i = 0; i < n; i++) {
        std::cout << ShortestPathLen[i] << " ";
    }
    std::cout << endl;
    free(ShortestPathLen);
}
