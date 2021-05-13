#include <iostream>
#include <vector>
#include <list>
#include <iterator>
#include <algorithm>

using namespace std;

vector<size_t> read_input(istream& stream) {
    size_t tree_size;
    stream >> tree_size;
    size_t code_size = tree_size - 2;

    vector<size_t> v;
    v.reserve(code_size);

    for (size_t i = 0; i < code_size; i++) {
        size_t elem;
        stream >> elem;
        v.push_back(elem);
    }

    return v;
}

void write_output(const vector<size_t>& tree, ostream& stream) {
    for (size_t i = 0; i < tree.size() - 1; i++) {
        stream << tree[i] << " ";
    }
    stream << tree[tree.size() - 1] << endl;
}

list<size_t> get_leaves(vector<size_t> code) {
    size_t tree_size = code.size() + 2;

    vector<size_t> all_values;
    all_values.reserve(tree_size);
    for (size_t i = 1; i <= tree_size; i++) {
        all_values.push_back(i);
    }

    sort(code.begin(), code.end());

    list<size_t> leaves;
    set_difference(
        all_values.begin(), all_values.end(),
        code.begin(), code.end(),
        inserter(leaves, leaves.begin())
    );

    return leaves;
}

vector<size_t> tree_from_pruffer_code(const vector<size_t>& code) {
    size_t tree_size = code.size() + 2;
    vector<size_t> tree(tree_size, 0);

    list<size_t> leaves(get_leaves(code));

    for (size_t i = 0; i < code.size(); i++) {
        // coloca a menor folha como filha do pai
        size_t parent = code[i];
        size_t leaf = leaves.front();
        tree[leaf - 1] = parent;

        // remove a folha da lista pois ela já foi adicionada
        leaves.pop_front();

        // checa se o pai tornou-se folha
        vector<size_t>::const_iterator find_result = find(code.begin() + i + 1, code.end(), parent);
        if (find_result == code.end()) {
            // insere o pai como folha na posição adequada da lista
            leaves.insert(
                upper_bound(leaves.begin(), leaves.end(), parent),
                parent
            );
        }
    }

    size_t f1 = leaves.front();
    size_t f2 = leaves.back();

    if (f1 > f2) {
        tree[f1 - 1] = f2;
    }
    else {
        tree[f2 - 1] = f1;
    }

    return tree;
}

int main() {
    vector<size_t> code(read_input(cin));
    vector<size_t> tree(tree_from_pruffer_code(code));
    write_output(tree, cout);

    return 0;
}