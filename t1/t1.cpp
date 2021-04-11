/*
MC558 - Turma A - Professor Orlando Lee
Lucas Valente Viegas de Oliveira Paes
RA 220958
*/

#include <iostream>
#include <algorithm>
#include <vector>
#include <vector>

using namespace std;

typedef struct SequenceElement {
    public:
    size_t index;
    int value;

    friend bool operator>(const SequenceElement& l, const SequenceElement& r) {
        return 
            l.value == r.value ?
            l.index > r.index :
            l.value > r.value;
    }
} SequenceElement;

typedef vector<SequenceElement> Sequence;

typedef vector<vector<size_t> > Graph;

/** Lê a sequência de entrada conforme a especificação do enunciado */
vector<SequenceElement> read_input() {
    size_t n;
    cin >> n;

    vector<SequenceElement> v;
    v.reserve(n);

    for (size_t i = 0; i < n; i++) {
        int input;
        cin >> input;

        SequenceElement elem;
        elem.index = i;
        elem.value = input;

        v.push_back(elem);
    }

    return v;
}

/** Escreve um grafo na forma de lista de adjacências na sequência*/
void print_graph(const Graph& graph) {
    for (size_t i = 0; i < graph.size(); i++) {
        for (size_t j = 0; j < graph[i].size(); j++) {
            // + 1 porque a saída espera índices começando em 0
            cout << graph[i][j] + 1 << " ";
        }
        cout << endl;
    }
}

/** Reduz uma sequência de tamanho N a uma sequência de tamanho N-1 de acordo com a propriedade do enunciado, adicionando-a ao grafo */
void reduce_sequence_and_add_graph(Sequence& sequence, Graph& graph) {
    SequenceElement first = sequence[0];

    // Caso o primeiro elemento tenha mais vizinhos do que há vértices,
    // a sequência obviamente não é gráfica. Nesse caso, reduzimos a sequência a uma
    // sequência inválida, com -1 no final.
    if (first.value >= (int)sequence.size()) {
        sequence[sequence.size() - 1].value = -1;
        return;
    }

    // subtrai um e desloca para a esquerda, adicionando na lista de adjacência
    for (int i = 1; i <= first.value; i++) {
        sequence[i - 1].index = sequence[i].index;
        sequence[i - 1].value = sequence[i].value - 1;

        graph[first.index].push_back(sequence[i].index);
        graph[sequence[i].index].push_back(first.index);
    }

    rotate(sequence.begin() + first.value, sequence.begin() + first.value + 1, sequence.end());

    // remove o último elemento da sequência
    sequence.pop_back();
}

/** Determina se uma sequência de inteiros é gráfica */
vector<vector<size_t> > graph_from_graphical_sequence(Sequence sequence) {
    vector<vector<size_t> > empty;
    vector<vector<size_t> > graph(sequence.size());

    while (sequence.size() > 2 && sequence[0].value > 0) {
        // caso base: min(d(v)) < 0
        if (sequence.back().value < 0) {
            return empty;
        }

        reduce_sequence_and_add_graph(sequence, graph);

        // ordena a sequência em ordem decrescente
        sort(sequence.begin(), sequence.end(), greater<SequenceElement>());
    }

    // caso base: min(d(v)) < 0
    if (sequence.back().value < 0) {
        return empty;
    }

    // caso base: grafo de 2 vértices com ambos os vértices isolados
    // caso base: grafo de mais de 2 vértices com todos os graus 0
    if (sequence[0].value == 0 && sequence[1].value == 0) {
        return graph;
    }

    // caso base: grafo de 2 vértices com só uma aresta conectando ambos os vértices
    if (sequence[0].value == 1 && sequence[1].value == 1) {
        return graph;
    }

    // caso base: grafo de 2 vértices, a sequência não é gráfica
    return empty;
}

int main() {
    vector<SequenceElement> sequence(read_input());

    vector<vector<size_t> > graph(graph_from_graphical_sequence(sequence));
    if (graph.size() == 0 && sequence.size() > 0) {
        cout << "Não é sequência gráfica!";
    }
    else {
        print_graph(graph);
    }

    return 0;
}