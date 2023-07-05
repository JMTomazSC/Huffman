#ifndef Huffman_hpp
#define Huffman_hpp


#include <iostream>
#include <string>
#include <fstream>

using namespace std;

//Estrutura do nó da árvore de Huffman
typedef struct Node{
    unsigned char data;
    long long unsigned frequency;
    struct Node *left, *right;

    Node(unsigned char data, long long unsigned frequency){
        left = right = NULL;
        this->data = data;
        this->frequency = frequency;
    }
}Node;

//Classe que implementa a codificação e decodificação de Huffman
class Huffman{
    public:
    Node* huffmanTree(long long unsigned int* freq);
    Node* get_min(Node* nodes[], int& size);
    void findFrequency(string input, long long unsigned int freq[]);
    void build_heap(Node* nodes[], int size);
    void heapify(Node* nodes[], int size, int idx);
    void trade(Node* &a, Node* &b);
    void insert(Node* nodes[], int& size, Node* node);
    void build_table(Node* root, string str, string* table);
    void write_binary(string input, string compressed, long long int count);
    string read_binary(string input);
    void compress(string input, string output, string tableFile);
    void decompress(string input, string output, string tableFile);

};

#endif