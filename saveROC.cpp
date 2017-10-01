#include "load.h"
#include "confusion_matrix.h"
#include <mpi.h>
#include <iostream>
#include <vector>  
#include <fstream>
#include <string>
#include <cstdlib>

using namespace std;

int main(int argc, char** argv){
    vector<int> test_labels;
    vector<vector<double> > testFeatures;
    loadLabelsFeatures("result/test", test_labels, testFeatures);
    double* Bomega1 = new double[382536];
    loadBoostedOmega1(Bomega1);
    double* Bomega2 = new double[382536];
    loadBoostedOmega2(Bomega2);
    double sumOfAlpha;
    loadSumOfAlpha(&sumOfAlpha);

    //the times for theta
    int n = atoi(argv[1]);

    ConfusionMatrix* confusion_matrices = new ConfusionMatrix[n+1];
    
    for(int k=0; k<n+1; ++k){
        double theta = -1.0 + 2/(double)n * k;
        //cout << "theta = " << theta << endl;
        for(int i=0; i<testFeatures.size(); ++i){
            double F = 0.0;
            for(int j=0; j<382536; ++j){
                //if (Bomega1[j] != 0 || Bomega2[j] != 0){
                    F += Bomega1[j] * testFeatures[i][j] + Bomega2[j]; 
                //} 
            }
            int predict = (F >= theta * sumOfAlpha ? 1 : -1);
            //cout << "predict = " << predict << endl;
            confusion_matrices[k].AddPrediction(test_labels[i], predict);
        }
    }

    for (int t = 0; t < n+1; t++) {
        //cout << "TP = " << confusion_matrices[t].GetTP() << endl;
        //cout << "TN = " << confusion_matrices[t].GetTN() << endl;
        //cout << "FP = " << confusion_matrices[t].GetFP() << endl;
        //cout << "FN = " << confusion_matrices[t].GetFN() << endl;
        confusion_matrices[t].PrintRocEvaluation();
    }

    delete[] confusion_matrices;
}
