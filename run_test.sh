./BellmanFord test.dot > 1
./BellmanFordDummy test.dot > 2
./BellmanFordPar1 test.dot > 3
echo "Ligra VS Dummy" 
diff 1 2 | grep "Run"

echo "Ligra VS Par1" 
diff 1 3 | grep "Run"
