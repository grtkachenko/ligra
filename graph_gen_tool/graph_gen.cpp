#include <iostream>
#include <stdio.h>
#include <stdlib.h>

using namespace std;

const int MAX_W = 1000;

int main(int argc, char* argv[]) {
    freopen("../test_graph", "w", stdout);
    int n = atoi(argv[1]);
    int m = n * (n - 1);
    int cur = 0;
    printf("WeightedAdjacencyGraph\n%d\n%d\n", n, m);
    for (int i = 0; i < n; i++) {
        printf("%d\n", cur);
        cur += n - 1;
    }
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (i != j) {
                printf("%d %d\n", j, -MAX_W / 2 + rand() % MAX_W);
            }
        }
    }
}
