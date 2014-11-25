make bf && ./BellmanFordDummy test.dot > 1 && ./BellmanFordPar2 test.dot > 2 && diff 1 2 | grep Run

