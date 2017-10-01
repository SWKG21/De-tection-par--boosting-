#include "load.h"
#include "boosting.h"
#include <mpi.h>
#include <iostream>
#include <vector>  
#include <fstream>
#include <string>

using namespace std;

int main(int argc, char* argv[]){
    MPI_Init(&argc, &argv);
    int size, rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Status status;

    vector<int> valid_labels;
    vector<vector<double> > valid_features;
    loadLabelsFeatures("result/dev", valid_labels, valid_features);
    double* omega1 = new double[382536];
    loadCFOmega1(omega1);
    double* omega2 = new double[382536];
    loadCFOmega2(omega2);
    Boosting* bst = new Boosting(valid_features, valid_labels, omega1, omega2);
    bst->Boost(5);

    MPI_Finalize();

    if (rank == 0){
        double* omg1 = bst->getOmega1Boosted();
        string nametxt1 = "result/boosted/Bomega1.txt";
        const char* forTxt1 = nametxt1.c_str();
        ofstream outfile1(forTxt1);
        if (outfile1.is_open()){
            for(int i=0; i<bst->getFeature_dimension(); ++i){
                outfile1 << omg1[i] << endl;
                if (omg1[i] != 0) cout << "one in omg1=" << omg1[i] <<
                    endl;
            }
            outfile1.close();
            cout << "wrote in Bomega1.txt!" << endl;
        }
        else{
            cerr << "can not open Bomega1.txt" << endl;
        }        

        double* omg2 = bst->getOmega2Boosted();
        string nametxt2 = "result/boosted/Bomega2.txt";
        const char* forTxt2 = nametxt2.c_str();
        ofstream outfile2(forTxt2);
        if (outfile2.is_open()){
            for(int i=0; i<bst->getFeature_dimension(); ++i){
                outfile2 << omg2[i] << endl;
                if (omg2[i] != 0) cout << "one in omg2=" << omg2[i] << endl;
            }
            outfile2.close();
            cout << "wrote in Bomega2.txt!" << endl;
        }
        else{
            cerr << "can not open Bomega1.txt" << endl;
        }

        double sumOfAlpha = bst->getSumOfAlpha();
        string nametxt3 = "result/boosted/sumOfAlpha.txt";
        const char* forTxt3 = nametxt3.c_str();
        ofstream outfile3(forTxt3);
        if (outfile3.is_open()){
            outfile3 << sumOfAlpha << endl;
            cout << sumOfAlpha << endl;
            outfile3.close();
            cout << "wrote in sumOfAlpha.txt!" << endl;
        }
        else{
            cerr << "can not open sumOfAlpha.txt" << endl;
        }
    }
      
    return 0;
}
