make bf && ./BellmanFordDummy -d test.dot > 1 && ./BellmanFordPar1 -d test.dot > 2 && diff 1 2

