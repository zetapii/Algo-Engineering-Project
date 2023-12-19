import random
import networkx as nx

def generate_sparse_graph(num_nodes, num_edges, file_name):
    G = nx.Graph()

    G.add_nodes_from(range(num_nodes))

    for i in range(num_edges):
        u, v = random.sample(range(num_nodes), 2)
        G.add_edge(u, v)

    with open(file_name, 'w') as f:
        f.write(f"{num_nodes},{num_edges}\n")
        for u, v in G.edges():
            f.write(f"{u},{v}\n")

    return G


def generate_dense_graph(num_nodes, num_edges, file_name):
    G = nx.Graph()

    G.add_nodes_from(range(num_nodes))

    max_edges = (num_nodes * (num_nodes - 1)) // 2

    all_edges = [(u, v) for u in range(num_nodes) for v in range(u+1, num_nodes)]

    edge_list = random.sample(all_edges, num_edges)

    G.add_edges_from(edge_list)

    with open(file_name, 'w') as f:
        f.write(f"{num_nodes},{num_edges}\n")
        for u, v in G.edges():
            f.write(f"{u},{v}\n")

    return G

generate_dense_graph(100000,0.7*100000,"graph-dense.txt")
