#include <iostream>
#include <vector>
#include <list>

using namespace std;

const size_t ONE_WAY = 1;
const size_t TWO_WAY = 2;

vector<list<size_t> > read_input() {
    size_t n_vertex, n_edge;
    cin >> n_vertex >> n_edge;

    vector<list<size_t> > g(n_vertex);

    for (size_t i = 0; i < n_edge; i++) {
        size_t from, to, type;
        cin >> from >> to >> type;

        g[from].push_back(to);
        if (type == TWO_WAY) {
            g[to].push_back(from);
        }
    }

    return g;
}

typedef struct DfsVisitResult {
    size_t n_visited;
    size_t time;
} DfsVisitResult;

DfsVisitResult dfs_visit(size_t node, vector<list<size_t> >& g, vector<bool>& visited, size_t time) {
    DfsVisitResult result;
    result.n_visited = 0;
    result.time = time;
    if (visited[node]) {
        return result;
    }

    visited[node] = true;
    result.n_visited = 1;
    result.time = time + 1;

    for (list<size_t>::iterator it = g[node].begin(); it != g[node].end(); it++) {
        DfsVisitResult r = dfs_visit(*it, g, visited, time);
        result.time += r.time;
        result.n_visited += r.n_visited;
    }

    result.time++;

    return result;
}

size_t dfs_last_to_finish(vector<list<size_t> >& g) {
    size_t time = 0, last_visited;
    vector<bool> visited(g.size(), false);
    for (vector<bool>::iterator it = visited.begin(); it != visited.end(); it++) {
        if (!*it) {
            last_visited = it - visited.begin();
            DfsVisitResult r = dfs_visit(last_visited, g, visited, time);
            time += r.time;
        }
    }
    return last_visited;
}

vector<list<size_t> > transpose(vector<list<size_t> >& g) {
    vector<list<size_t> > gt(g.size());

    for (vector<list<size_t> >::iterator vertex = g.begin(); vertex != g.end(); vertex++) {
        size_t vertex_index = vertex - g.begin();
        for (list<size_t>::iterator neighbor = vertex->begin(); neighbor != vertex->end(); neighbor++) {
            gt[*neighbor].push_back(vertex_index);
        }
    }

    return gt;
}

bool is_fully_connected(vector<list<size_t> >& g, size_t start_vertex) {
    vector<bool> visited(g.size(), false);
    DfsVisitResult r = dfs_visit(start_vertex, g, visited, 0);
    return g.size() == r.n_visited;
}

bool is_strongly_connected(vector<list<size_t> >& g) {
    size_t last_visited = dfs_last_to_finish(g);
    vector<list<size_t> > gt(transpose(g));
    return is_fully_connected(gt, last_visited);
}

int main() {
    vector<list<size_t> > g(read_input());

    if (is_strongly_connected(g)) {
        cout << "Adequado." << endl;
    }
    else {
        cout << "Inadequado." << endl;
    }
}