#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <sys/time.h>
#include <sys/resource.h>
#include <iomanip>
#include "../include/Huffman.hpp"
#include "../include/Errors.hpp"

using namespace std;

int main(int argc,char** argv){
    //Parse args
    if(argc != 4){
        cout << "Usage: ./bin/huff <input file> <output file> <mode>" << endl;
        return 1;
    }
    string input = argv[1];
    string output = argv[2];
    string mode = argv[3];
    try{
        if(mode != "-c" && mode != "-d"){
            invalid_mode e("Invalid mode. Please use '-c' for compress or '-d' for decompress.");
            throw e;
        }
        //Start timer
        struct timeval start, end;
        gettimeofday(&start, NULL);
        //Compress or decompress
        Huffman huff;
        if(mode == "-c"){
            if(input == output){
                invalid_file e("Input and output files must be different.");
                throw e;
            }
            string table_file = output + "_table.txt";
            huff.compress(input, output, table_file);
        }
        else{
            if(input == output){
                invalid_file e("Input and output files must be different.");
                throw e;
            }
            string table_file = input + "_table.txt";
            huff.decompress(input, output, table_file);
        }
        //End timer
        gettimeofday(&end, NULL);
        double time_taken;
        time_taken = (end.tv_sec - start.tv_sec) * 1e6;
        time_taken = (time_taken + (end.tv_usec - start.tv_usec)) * 1e-6;
        //Print time
        cout << "Time taken by program is : " << fixed << time_taken << setprecision(6);
        cout << " sec" << endl;
        return 0;
    }catch(file_not_open e){
        cout << e.message << endl;
        return 1;
    }catch(invalid_file e){
        cout << e.message << endl;
        return 1;
    }catch(invalid_mode e){
        cout << e.message << endl;
        return 1;
    }
}