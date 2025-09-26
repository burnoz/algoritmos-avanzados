#include <iostream>
#include <queue>
#include <vector>
// Huffman encoding in C++
#include <map>
#include <string>

using namespace std;

// Node for Huffman tree
struct Node {
    char character; // Character stored (empty for internal nodes)
    double prob;    // Probability/frequency
    int tag;        // 0 or 1 (used for code generation)
    Node *left;
    Node *right;

    Node(char c, double p) : character(c), prob(p), tag(-1), left(nullptr), right(nullptr) {}
};

// Comparator for priority queue (min-heap by probability, then character)
struct Compare {
    bool operator()(Node* a, Node* b) {
        if (a->prob == b->prob)
            return a->character > b->character;
        return a->prob > b->prob;
    }
};

// Preorder traversal to print probabilities
void preorder(Node* node) {
    if (!node) return;
    cout << node->prob << " ";
    preorder(node->left);
    preorder(node->right);
}

// Generate codes for each character (recursive)
void get_codes(Node* node, string code, map<char, string>& table) {
    if (!node) return;
    // Leaf node: store code
    if (!node->left && !node->right && node->character != '\0') {
        table[node->character] = code;
        return;
    }
    if (node->left) get_codes(node->left, code + "0", table);
    if (node->right) get_codes(node->right, code + "1", table);
}

// Build Huffman tree from characters and probabilities
Node* build_huffman_tree(const vector<char>& alphabet, const vector<double>& probs) {
    priority_queue<Node*, vector<Node*>, Compare> pq;
    int n = alphabet.size();

    for (int i = 0; i < n; ++i) {
        pq.push(new Node(alphabet[i], probs[i]));
    }

    while (pq.size() > 1) {
        Node* a = pq.top(); pq.pop();
        Node* b = pq.top(); pq.pop();

        Node* merged = new Node('\0', a->prob + b->prob); // Internal node
        
        merged->left = a; merged->right = b;
        a->tag = 0; b->tag = 1;
        
        pq.push(merged);
    }
    
    return pq.top();
}

// Encode a string using the code table
string encode(const string& text, const map<char, string>& table) {
    string result;
    for (char c : text) {
        result += table.at(c);
    }
    return result;
}

// Decode a string using the Huffman tree
string decode(const string& encoded, Node* root) {
    string result;
    Node* node = root;
    for (char bit : encoded) {
        if (bit == '0') node = node->left;
        else node = node->right;
        // If leaf, append character
        if (!node->left && !node->right) {
            result += node->character;
            node = root;
        }
    }
    return result;
}

int main() {
    // Example: match the notebook
    vector<char> alphabet = {'A', 'B', 'C', 'D', 'E'};
    vector<double> probs = {0.35, 0.1, 0.2, 0.2, 0.15};

    // Build Huffman tree
    Node* huffman_root = build_huffman_tree(alphabet, probs);

    // Print probabilities in preorder
    cout << "Preorder probabilities: ";
    preorder(huffman_root);
    cout << endl;

    // Get codes for each character
    map<char, string> code_table;
    get_codes(huffman_root, "", code_table);
    cout << "Huffman codes:\n";
    for (auto& p : code_table) {
        cout << p.first << ": " << p.second << endl;
    }

    // Encode and decode a test string
    string test = "ADADADA";
    string encoded = encode(test, code_table);
    cout << "Encoded: " << encoded << endl;
    string decoded = decode(encoded, huffman_root);
    cout << "Decoded: " << decoded << endl;

    // Clean up tree nodes (optional, for larger examples use smart pointers)
    // ...
    return 0;
}


