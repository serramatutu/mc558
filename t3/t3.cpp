#include <iostream>
#include <limits>
#include <queue>
#include <vector>
#include <list>

using namespace std;

vector<list<size_t> > read_input(istream& s) {
    size_t nodes, edges;
    cin >> nodes >> edges;

    vector<list<size_t> > graph(nodes);

    for (size_t i = 0; i < edges; i++) {
        size_t p1, p2;
        cin >> p1 >> p2;

        graph[p1].push_back(p2);
        graph[p2].push_back(p1);
    }

    return graph;
}

size_t get_largest_erdos_number(const vector<list<size_t> >& graph) {
    vector<size_t> erdos_numbers(graph.size(), 0);
    size_t max_erdos_number = 0;

    vector<bool> visited(graph.size(), false);
    size_t num_visited = 0;

    queue<size_t> q;
    
    q.push(0);
    visited[0] = true;
    num_visited++;

    while(!q.empty()) {
        size_t current = q.front();
        q.pop();

        max_erdos_number = max(max_erdos_number, erdos_numbers[current]);
        
        for (list<size_t>::const_iterator next = graph[current].begin(); next != graph[current].end(); next++) {
            if (visited[*next]) {
                continue;
            }

            erdos_numbers[*next] = erdos_numbers[current] + 1;

            visited[*next] = true;
            num_visited++;
            q.push(*next);
        }
    }

    if (num_visited < graph.size()) {
        return numeric_limits<size_t>::max();
    }

    return max_erdos_number;
}

int main() {
    vector<list<size_t> > graph(read_input(cin));
    size_t largest_erdos_number = get_largest_erdos_number(graph);
    
    if (largest_erdos_number == numeric_limits<size_t>::max()) {
        cout << "infinito" << endl;
    }
    else {
        cout << largest_erdos_number << endl;
    }

    return 0;
}