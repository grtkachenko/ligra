#define WEIGHTED 1
#include "ligra.h"

template <class vertex>
void Compute(graph<vertex> GA, intT start) {
    intT n = GA.n;
    //initialize ShortestPathLen to "infinity"
    int* ShortestPathLen = newA(int,n);
    {parallel_for(intT i=0;i<n;i++) ShortestPathLen[i] = INT_MAX/2;}
    ShortestPathLen[start] = 0;
    for (int i = 0; i < n; i++) {
        bool updated = false;
        {for (int j = 0; j < n; j++) {
            int curMin = INT_MAX / 2;
            for (int k = 0; k < GA.V[j].getInDegree(); k++) {
                curMin = min(curMin, ShortestPathLen[GA.V[j].getInNeighbor(k)] + GA.V[j].getInWeight(k));
            }
            if (curMin < ShortestPathLen[j]) {
                ShortestPathLen[j] = curMin;
                updated = true;
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
