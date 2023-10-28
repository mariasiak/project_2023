test:
	g++ source/*.*pp -o project
	./project -d dataset/input.dat -q dataset/query.dat -k 5 -L 5 -o out.txt -N 5 -R 5000

lsh:
	g++ mains/lsh_main.cpp source/*.*pp -o lsh
	./lsh -d dataset/input.dat -q dataset/query.dat -k 5 -L 5 -o out.txt -N 5 -R 5000

cube:
	g++ mains/hypercube_main.cpp source/*.*pp -o cube
	./cube -d dataset/input.dat -q dataset/query.dat -k 5 -M 5 -probes 2 -o out_hc.txt -N 5 -R 2000

cluster:
	g++ mains/cluster_main.cpp source/*.*pp -o cluster
	./cluster -i dataset/input.dat -c cluster.conf -o out_cluster.txt -complete -m Classic

