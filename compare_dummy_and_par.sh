make bf && ./BellmanFordDummy test.dot > 1 && ./BellmanFordPar1 test.dot > 2 && diff 1 2 | grep Run

