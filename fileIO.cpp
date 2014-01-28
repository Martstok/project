#include <iostream>
#include <fstream>
#include "fileIO.h"
#include <sstream>
#include <string>
#include <cstring>
using namespace std;


void printToFile(string data, char* filename){

    ofstream outputFile;
    outputFile.open(filename, ios::app);
    string output = data;
    outputFile << output << ";\n";
    outputFile.close();
}
