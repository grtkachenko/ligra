echo "Ligra is running..." 
./BellmanFord -d test.dot > 1
echo "Dummy is running..." 
./BellmanFordDummy -d test.dot > 2
echo "Par is running..." 
./BellmanFordPar1 -d test.dot > 3
echo "Ligra VS Dummy" 
diff 1 2 | grep It 
diff 1 2 | grep Run 
echo "____________"
echo "Ligra VS Par1" 
diff 1 3 | grep It
diff 1 3 | grep Run 
