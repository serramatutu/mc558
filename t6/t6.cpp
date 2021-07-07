#include <iostream>

#include <list>
#include <vector>
#include <queue>

using namespace std;

typedef struct DependecyGraph {
    size_t n_vertex;
    size_t n_edge;

    vector<list<size_t> > deps;
    vector<size_t> node_costs;
} DependecyGraph;

DependecyGraph read_input() {
    DependecyGraph g;
    cin >> g.n_vertex >> g.n_edge;
    g.deps.resize(g.n_vertex);
    g.node_costs.reserve(g.n_vertex);

    for (size_t i = 0; i < g.n_vertex; i++) {
        size_t cost;
        cin >> cost;

        g.node_costs.push_back(cost);
    }

    for (size_t i = 0; i < g.n_edge; i++) {
        size_t from, to;
        cin >> from >> to;
        
        g.deps[to].push_back(from);
    }

    return g;    
}

void ts_visit(const DependecyGraph& g, vector<bool>& visited, list<size_t>& ts, size_t start) {
    visited[start] = true;

    for (list<size_t>::const_iterator next = g.deps[start].begin(); next != g.deps[start].end(); next++) {
        if (!visited[*next]) {
            ts_visit(g, visited, ts, *next);
        }
    }
    
    ts.push_back(start);
}

list<size_t> topological_sort(const DependecyGraph& g) {
    vector<bool> visited(g.n_vertex, false);
    
    list<size_t> ts;

    for (size_t i = 0; i < g.n_vertex; i++) {
        if (!visited[i]) {
            ts_visit(g, visited, ts, i);
        }
    }

    return ts;
}

size_t largest_path_cost(const DependecyGraph& g) {
    list<size_t> ts(topological_sort(g));
    
    vector<size_t> cost(g.n_vertex, 0);
    size_t max_cost = 0;

    for (list<size_t>::iterator current = ts.begin(); current != ts.end(); current++) {
        cost[*current] = g.node_costs[*current];

        size_t max_dep_cost = 0;
        for (list<size_t>::const_iterator dep = g.deps[*current].begin(); dep != g.deps[*current].end(); dep++) {
            if (cost[*dep] > max_dep_cost) {
                max_dep_cost = cost[*dep];
            }
        }
        cost[*current] += max_dep_cost;

        if (cost[*current] > max_cost) {
            max_cost = cost[*current];
        }        
    }

    return max_cost;
}

int main() {
    DependecyGraph g(read_input());
    size_t cost = largest_path_cost(g);    
    cout << cost << endl;

    return 0;
}