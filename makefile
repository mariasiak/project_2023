all:
	g++ mains/lsh_main.cpp source/*.*pp -o lsh
	g++ mains/hypercube_main.cpp source/*.*pp -o cube
	g++ mains/cluster_main.cpp source/*.*pp -o cluster

lsh:
	g++ mains/lsh_main.cpp source/*.*pp -o lsh

lsh_run:
	./lsh -d dataset/input.dat -q dataset/query.dat -k 5 -L 5 -o lsh.out -N 5 -R 5000

cube:
	g++ mains/hypercube_main.cpp source/*.*pp -o cube

cube_run:
	./cube -d dataset/input.dat -q dataset/query.dat -k 5 -M 5 -probes 2 -o hc.out -N 5 -R 2000

cluster:
	g++ mains/cluster_main.cpp source/*.*pp -o cluster

cluster_run:
	./cluster -i dataset/input.dat -c cluster.conf -o cluster.out -complete -m Classic

gnns:
	g++ mains/graph_search_main.cpp source/*.*pp -o graph_search
	./graph_search -d dataset/input.dat -q dataset/query.dat -k 5 -E 3 -R 1 -N 5 -I 20 -m 1 -o graph_search.out

clear: 
	rm -f -v cluster cube lsh graph_search *.out