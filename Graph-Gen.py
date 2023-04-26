import random
import networkx as nx

def generate_sparse_graph(num_nodes, num_edges, file_name):
    # Initialize empty graph
    G = nx.Graph()

    # Add nodes to the graph
    G.add_nodes_from(range(num_nodes))

    # Add edges to the graph
    for i in range(num_edges):
        # Generate a random edge between two nodes
        u, v = random.sample(range(num_nodes), 2)
        G.add_edge(u, v)

    # Write the graph to the output file
    with open(file_name, 'w') as f:
        f.write(f"{num_nodes},{num_edges}\n")
        for u, v in G.edges():
            f.write(f"{u},{v}\n")

    return G


def generate_dense_graph(num_nodes, num_edges, file_name):
    # Initialize empty graph
    G = nx.Graph()

    # Add nodes to the graph
    G.add_nodes_from(range(num_nodes))

    # Compute the maximum number of possible edges
    max_edges = (num_nodes * (num_nodes - 1)) // 2

    # Generate a list of all possible edges
    all_edges = [(u, v) for u in range(num_nodes) for v in range(u+1, num_nodes)]

    # Sample num_edges random edges from the list of all possible edges
    edge_list = random.sample(all_edges, num_edges)

    # Add the sampled edges to the graph
    G.add_edges_from(edge_list)

    # Write the graph to the output file
    with open(file_name, 'w') as f:
        f.write(f"{num_nodes},{num_edges}\n")
        for u, v in G.edges():
            f.write(f"{u},{v}\n")

    return G

generate_dense_graph(100000,0.7*100000,"graph-dense.txt")