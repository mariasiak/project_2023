all:
	g++ mains/lsh_main.cpp source/*.*pp -o lsh
	g++ mains/hypercube_main.cpp source/*.*pp -o cube
	g++ mains/cluster_main.cpp source/*.*pp -o cluster
	g++ mains/graph_search_main.cpp source/*.*pp -o graph_search

lsh:
	g++ mains/lsh_main.cpp source/*.*pp -o lsh

lsh_run:
	./lsh -d dataset/input.dat -q dataset/query.dat -dl dataset/input_l.csv -ql dataset/query_l.csv -k 100 -L 100 -o stats10/lsh/lsh_k_100_L_100.out -N 1 -R 0

cube:
	g++ mains/hypercube_main.cpp source/*.*pp -o cube

cube_run:
	./cube -d dataset/input.dat -q dataset/query.dat -dl dataset/input_l.csv -ql dataset/query_l.csv -k 15 -M 5 -probes 4 -o stats10/cube/hc_k_15_p_4.out -N 1 -R 0

cluster:
	g++ mains/cluster_main.cpp source/*.*pp -o cluster

cluster_run:
	./cluster -i dataset/input.dat -dl dataset/input_nn1.csv -c cluster.conf -o cluster.out -complete -m Classic

graph_search:
	g++ mains/graph_search_main.cpp source/*.*pp -o graph_search

gnns_run:
	./graph_search -d dataset/input.dat -q dataset/query.dat -dl dataset/input_l.csv -ql dataset/query_l.csv -k 50 -E 30 -R 1 -N 1 -I 20 -m 1 -o stats10/gnns/gnns.out

mrng_run:
	./graph_search -d dataset/input.dat -q dataset/query.dat -dl dataset/input_l.csv -ql dataset/query_l.csv -k 50 -E 30 -R 1 -N 1 -I 20 -m 2 -o stats10/mrng/mrng.out

clear: 
	rm -f -v cluster cube lsh graph_search *.out