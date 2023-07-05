#ifndef ERRORS_HPP
#define ERRORS_HPP
#include <iostream>


using namespace std;

//Erro para caso o arquivo não seja aberto
struct file_not_open{
    string message;
    file_not_open(string message){
        this->message = message;
    }
};

//Erro caso o modo seja inválido
struct invalid_mode{
    string message;
    invalid_mode(string message){
        this->message = message;
    }
};

//Erro caso o arquivo seja inválido
struct invalid_file{
    string message;
    invalid_file(string message){
        this->message = message;
    }
};

#endif

