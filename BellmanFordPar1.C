#define WEIGHTED 1
#include "ligra.h"
#include <cilk/cilk.h>
#include <cilk/reducer_min.h>
#include <cilk/cilk_api.h>

const int nb_edges_cutoff = 5000;

volatile bool updated = false;

template <class vertex>
void update(graph<vertex> & GA, int from, int to, int * ShortestPathLen) {
    for (int j = from; j < to; j++) {
        int curMin = INT_MAX / 2;
        for (int k = 0; k < GA.V[j].getInDegree(); k++) {
            int from_d = ShortestPathLen[GA.V[j].getInNeighbor(k)];
            curMin = min(curMin, from_d + GA.V[j].getInWeight(k));
        }
        if (curMin < ShortestPathLen[j]) {
            updated = true;
            ShortestPathLen[j] = curMin; 
        }
    }
}

template <class vertex>
int* Compute(graph<vertex> GA, intT start) {
    intT n = GA.n;
    //initialize ShortestPathLen to "infinity"
    int * ShortestPathLen = newA(int,n);
    {parallel_for(intT i=0;i<n;i++) ShortestPathLen[i] = INT_MAX/2;}
    ShortestPathLen[start] = 0;
    int num_workers = __cilkrts_get_nworkers();
    int* from = newA(int, num_workers);
    int* to = newA(int, num_workers);
    int dist = n / num_workers;
    from[0] = 0;
    to[0] = dist;
    for (int i = 1; i < num_workers; i++) {
        from[i] = to[i - 1];
        to[i] = from[i] + dist;
    }
    to[num_workers - 1] = n;

    std::cout << n << std::endl;
    for (int i = 0; i < num_workers; i++) {
        std::cout << from[i] << " " << to[i] << std::endl;
    }

    int it = 0;
    bool have_neg_cycle = true;
    for (int i = 0; i < n; i++) {
        updated = false;
        it++;
        {
            for (int j = 0; j < num_workers - 1; j++) {
               cilk_spawn update(GA, from[j], to[j], ShortestPathLen);
            } 
            update(GA, from[num_workers - 1], to[num_workers - 1], ShortestPathLen);
            cilk_sync;
        }
        if (!updated) {
            have_neg_cycle = false;
            break;
        }
    }
    if (have_neg_cycle) {
        for (int i = 0; i < n; i++) {
            ShortestPathLen[i] = -(INT_MAX/2);
        }
    }
    std::cout << "Iterations " << (it + 1) << std::endl;
    std::cout << "Have neg cycle " << have_neg_cycle << std::endl;

    free(from);
    free(to);
    return ShortestPathLen;
}
