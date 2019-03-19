#include "load.h"
#include "classifier.h"
#include <mpi.h>
#include <iostream>
#include <vector>  
#include <fstream>
#include <sstream>
#include <string>
#include <cmath>
#include "CImg.h"

using namespace std;

int main(int argc, char* argv[]){
    MPI_Init(&argc, &argv);
    int size, rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Status status;
    srand(rank);

    vector<int> labels;
    vector<vector<double> > features;
    loadLabelsFeatures("result/app", labels, features);
    Classifier* clf = new Classifier(features, labels);
    clf->AllWeakClassifierMpi(0.01);

    MPI_Finalize();
    
    if (rank == 0){
        double* omg1 = clf->getOmega1();
        //cout << omg1->size() << endl;
        string nametxt1 = "result/CF/omega1.txt";
        const char* forTxt1 = nametxt1.c_str();
        ofstream outfile1(forTxt1);
        cout << "try to open omega1.txt" << endl;
        if (outfile1.is_open()){
            for(int i=0; i<clf->getFeatureDimension(); ++i){
                outfile1 << omg1[i] << endl;
            }
            outfile1.close();
            cout << "wrote in omega1" << endl;
        }
        else{
           cerr << "can not open omega1!" << endl;
        }

        double* omg2 = clf->getOmega2();
        string nametxt2 = "result/CF/omega2.txt";
        const char* forTxt2 = nametxt2.c_str();
        ofstream outfile2(forTxt2);
        cout << "try to open omega2.txt" << endl;
        if (outfile2.is_open()){
            for(int i=0; i<clf->getFeatureDimension(); ++i){
                outfile2 << omg2[i] << endl;
            }
            outfile2.close();
            cout << "wrote in omega2" << endl;
        }
        else{
           cerr << "can not open omega2!" << endl;
        }
    }   
}
