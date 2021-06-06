#include <algorithm>
#include <iostream>
#include <map>
#include <vector>

using namespace std;

// Disjoint set implementation from CLRS
class DisjointSet {
    private:
    size_t _n_sets;
    map<size_t, size_t> _parent;

    public:
        DisjointSet(size_t n) {
            this->_n_sets = n;
            for (size_t i = 0; i < n; i++) {
                this->_parent[i] = i;
            }
        }

        size_t find_root(size_t k) {
            if (this->_parent[k] == k) {
                return k;
            }

            return find_root(this->_parent[k]);
        }

        void merge(size_t i, size_t j) {
            size_t root_i = this->find_root(i);
            size_t root_j = this->find_root(j);

            if (root_i != root_j) {
                this->_parent[i] = j;
                this->_n_sets--;
            }
        }

        size_t n_sets() const {
            return this->_n_sets;
        }
};

typedef struct GraphEdge {
    size_t v1;
    size_t v2;
    size_t weight;
} GraphEdge;

bool compare_graph_edge_by_weight(const GraphEdge& a, const GraphEdge& b) {
    return a.weight < b.weight;
}

typedef struct ReadInputResult {
    size_t n_cluster;
    size_t n_vertex;
    vector<GraphEdge> edges;
} ReadInputResult;

ReadInputResult read_input() {
    size_t n_vertex, n_edge, n_cluster;
    cin >> n_vertex >> n_edge >> n_cluster;

    vector<size_t> vertex(n_vertex);
    vector<GraphEdge> edges;
    edges.reserve(n_edge);

    for (size_t i = 0; i < n_edge; i++) {
        size_t v1, v2, weight;
        cin >> v1 >> v2 >> weight;

        GraphEdge e;
        e.v1 = v1;
        e.v2 = v2;
        e.weight = weight; 
        edges.push_back(e);  
    }

    ReadInputResult result;
    result.n_cluster = n_cluster;
    result.n_vertex = n_vertex;
    result.edges = edges;
    return result;
}

size_t mst_size_knuth(vector<GraphEdge>& edges, size_t n_vertex, size_t n_mst) {
    if (n_mst == n_vertex) {
        return 0;
    }

    size_t total_weight = 0;
    DisjointSet sets(n_vertex);

    sort(edges.begin(), edges.end(), compare_graph_edge_by_weight);
    
    for (vector<GraphEdge>::iterator edge = edges.begin(); edge != edges.end(); edge++) {
        size_t v1_root = sets.find_root(edge->v1);
        size_t v2_root = sets.find_root(edge->v2);
        if (v1_root != v2_root) {
            total_weight += edge->weight;
            sets.merge(v1_root, v2_root);

            if (sets.n_sets() == n_mst) {
                return total_weight;
            }
        }
    }

    return total_weight;
}

int main() {
    ReadInputResult input(read_input());
    size_t total_weight = mst_size_knuth(input.edges, input.n_vertex, input.n_cluster);
    cout << total_weight << endl;

    return 0;
}