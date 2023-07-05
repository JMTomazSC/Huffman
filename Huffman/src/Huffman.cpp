#include <iostream>
#include <algorithm>
#include <cstring>
#include <sstream>
#include "../include/Huffman.hpp"
#include "../include/Errors.hpp"

//Quantidade de caracteres ASCII
#define ASCII_CHAR 256

using namespace std;

//Função que encontra a frequencia de cada caracter no arquivo
void Huffman::findFrequency(string input, long long unsigned int freq[]){
    ifstream file(input, ios::binary);

    if(file){
        char c;
        //Como utiliza-se o stringstream para ler o arquivo
        //é necessário fazer o tratamento para espaços e quebras de linha
        while(file.get(c)){
            if(c == ' '){
                c = '&';
            }
            if(c == '\n'){
                c = '#';
            }
            freq[static_cast<unsigned char> (c)]++;
        }

        file.close();
    }else{
        file_not_open e("Error opening file.");
        throw e;
    }
}

//Função que troca dois nós de lugar
void Huffman::trade(Node* &a, Node* &b){
    Node* aux = a;
    a = b;
    b = aux;
}

//Função auxiliar que garante a integridade da heap
void Huffman::heapify(Node* nodes[], int size, int idx){
    int min = idx;
    int left = 2 * idx + 1;
    int right = 2 * idx + 2;

    if(left < size && nodes[left]->frequency < nodes[min]->frequency){
        min = left;
    }

    if(right < size && nodes[right]->frequency < nodes[min]->frequency){
        min = right;
    }

    if(min != idx){
        trade(nodes[min], nodes[idx]);
        heapify(nodes, size, min);
    }
}

//Função que constroi a heap
void Huffman::build_heap(Node* nodes[], int size){
    int idx = (size - 1);
    for(int i = (idx - 1)/ 2; i >= 0; i--){
        heapify(nodes, size, i);
    }
}

//Função que retorna o menor nó da heap
Node* Huffman::get_min(Node* nodes[], int& size){
    Node* min = nodes[0];
    nodes[0] = nodes[size - 1];
    size--;
    heapify(nodes, size, 0);
    return min;
}

//Função que insere um nó na heap
void Huffman::insert(Node* nodes[], int& size, Node* node){
    size++;
    int i = size - 1;
    while(i > 0 && node->frequency < nodes[(i - 1) / 2]->frequency){
        nodes[i] = nodes[(i - 1) / 2];
        i = (i - 1) / 2;
    }
    nodes[i] = node;
}

//Função que constroi a arvore de huffman
Node* Huffman::huffmanTree(long long unsigned int* freq){
    Node* nodes[ASCII_CHAR] = { nullptr };
    int size = 0;

    for(int i = 0; i < ASCII_CHAR; i++){
        if(freq[i] > 0){
            nodes[i] = new Node(static_cast<char> (i), freq[i]);
            insert(nodes, size, nodes[i]);
        }

    }

    while(size > 1){

        Node* left = get_min(nodes, size);
        Node* right = get_min(nodes, size);

        Node* parent = new Node(0, left->frequency + right->frequency);
        parent->left = left;
        parent->right = right;

        insert(nodes, size, parent);
    }
    return nodes[0];
        
}

//Função que constroi a tabela de codigos
void Huffman::build_table(Node* root, string str, string* table){
    if(root == nullptr){
        return;
    }

    if(root->data != '0'){
        table[static_cast<unsigned char> (root->data)] = str;
    }

    build_table(root->left, str + "0", table);
    build_table(root->right, str + "1", table);
}


//Função que escreve no arquivo em binario
void Huffman::write_binary(string input,string compressed, long long int count){
    //abre o arquivo em modo de escrita binária
    FILE* f = fopen(input.c_str(), "wb");
    if(f){
        unsigned char buffer = 0;
        int idx = 7;

        for(unsigned int i = 0; i < compressed.size(); i++){
           if(compressed[i] == '1'){
                buffer = buffer | (1 << idx);
            }
            idx--;

            if(idx < 0){
                fwrite(&buffer, sizeof(unsigned char), 1,f);
                buffer = 0;
                idx = 7;
            }
        }

        if(idx < 7){
            fwrite(&buffer, sizeof(unsigned char), 1,f);
        }

        fclose(f);
    }
    else{
        file_not_open e("Error opening file.");
        throw e;
    }
}

//Função que le o arquivo em binario
string Huffman::read_binary(string input){
    //abre o arquivo em modo de leitura binária
    FILE* f = fopen(input.c_str(), "rb");
    if(f){
        string compressed = "";
        unsigned char buffer;
        while(fread(&buffer, sizeof(unsigned char), 1,f)){
            for(int i = 7; i >= 0; i--){
                if((buffer & (1 << i))){
                    compressed += '1';
                }
                else{
                    compressed += '0';
                }
            }
        }
        fclose(f);
        return compressed;
    }
    else{
        file_not_open e("Error opening file.");
        throw e;
    }
}

//Função que comprime o arquivo
void Huffman::compress(string input, string output, string table_file){
    try{
        long long unsigned int freq[ASCII_CHAR] = { 0 };
        findFrequency(input, freq);
        ofstream file_table(table_file);

        //Constroi a arvore de huffman e o dicionário
        Node* root = huffmanTree(freq);
        string table[ASCII_CHAR];
        build_table(root, "", table);
        //Para a descompressão é necessário que o dicionário seja passado de alguma forma
        //Assim, cria-se um arquivo com o dicionário
        for(int i = 0; i < ASCII_CHAR; i++){
            if(freq[i] > 0){
                file_table << static_cast<char> (i) << " " << freq[i] << endl;
            }
        }
        file_table.close();

        ifstream file(input);
        string texto, line;
        if(file){
            //Adiciona o caracter que substitui a quebra de linha e monta um texto auxiliar
            while(getline(file, line)){
                texto += line + "#";
            }

            string compressed = "";
            for(int i = 0; i < texto.size();i++){
                if(texto[i] == ' '){
                    texto[i] = '&';
                }
                compressed += table[static_cast<unsigned char> (texto[i])];
            }
            //Escreve o texto comprimido no arquivo
            write_binary(output, compressed, texto.size());
            file.close();
        }else{
            file_not_open e("Error opening file.");
            throw e;
        }
    }catch(file_not_open e){
        cout << e.message << endl;
    }
}

//Função que descomprime o arquivo
void Huffman::decompress(string input, string output, string table_file){
    try{
        //Le o arquivo binario e o dicionario
        string compressed = read_binary(input);
        ifstream file(input, ios::binary);
        ofstream file_out(output, ios::binary);
        ifstream file_table(table_file);

        if(file && file_out && file_table){
            long long unsigned int freq[ASCII_CHAR] = { 0 };
            string word;
            unsigned int c = 0;
            
            //Le o dicionario e monta a tabela de frequencias
            while(getline(file_table, word)){
                stringstream ss(word);
                string character;
                string num;
                ss >> character;
                ss >> num;
                freq[static_cast<unsigned char> (character[0])] = stoi(num.c_str());
                c += stoi(num.c_str());
            }
            file_table.close();
            int count = 0;

            //Constroi a árvore de huffman
            Node* root = huffmanTree(freq);
            Node* aux = root;
            unsigned int aux_count = 0;
            while(count < compressed.size()){
                char bit = compressed[count];
                if(aux_count == c){
                    break;
                }
                if(bit == '1'){
                    aux = aux->right;
                }else if(bit == '0'){
                    aux = aux->left;
                }
                //Se o nó for uma folha, escreve o caracter no arquivo fazendo o tratamento de quebra de linha e espaço
                if(aux->left == nullptr && aux->right == nullptr){
                    if(aux->data == '&'){
                        aux->data = ' ';
                    }
                    if(aux->data == '#'){
                        aux->data = '\n';
                    }
                    file_out.put(aux->data);
                    aux = root;
                    aux_count++;
                }
                count++;

            }
            file.close();
            file_out.close();
        }else{
            file_not_open e("Error opening files.");
            cout << "Error opening files." << endl;
            throw e;
        }
    }catch(file_not_open e){
        cout << e.message << endl;
    }
}
