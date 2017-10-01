#include "load.h"
#include <mpi.h>
#include <iostream>
#include <vector>  
#include <fstream>
#include <string>
#include <cstdlib>

using namespace std;

int main(int argc, char** argv){
    string s = argv[1];
    vector<double> features;
    loadImageFeatures(s, features);
    cout << "load image ok" << endl;
    double* Bomega1;
    loadBoostedOmega1(Bomega1);
    cout << "load Bomega1 ok" << endl;
    double* Bomega2;
    loadBoostedOmega2(Bomega2);
    cout << "load Bomega2 ok" << endl;
    double sumOfAlpha;
    loadSumOfAlpha(&sumOfAlpha);
    
    //string ss = argv[2];
    //double theta = atof(ss.c_str());
    double theta = 0.5;
    cout << "theta = " << theta << endl;
    double F = 0.0;
    for(int i=0; i<382536; ++i){
        if (Bomega1[i] != 0 || Bomega2[i] != 0){
            F += Bomega1[i] * features[i] + Bomega2[i]; 
        } 
    }
    int predict = (F >= theta * sumOfAlpha ? 1 : -1);
    if (predict == 1) cout << "this image is a face !" << endl;
    if (predict == -1) cout << "this image is NOT a face !" << endl;
}