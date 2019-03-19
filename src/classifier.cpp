#include "image.h"
#include "classifier.h"
#include <mpi.h>
#include <vector>
#include <cstdlib>
#include <math.h>
#include <iostream>
using namespace std;

Classifier::Classifier(vector<vector<double> >& features, vector<int>& labels){
    features_ = features;
    labels_ = labels;
    feature_dimension_ = features_[0].size();
    omega1_ = new double[feature_dimension_];
    omega2_ = new double[feature_dimension_];
    for(int i=0; i<feature_dimension_; ++i){
        omega1_[i] = 1.0;
        omega2_[i] = 0.0;
    }
}


void Classifier::OneWeakClassifier(int i, double error){
    //set the step first
    double step = 0.001;
    int k = 1;
    while(true){
        int m = (int)((rand()/(double)RAND_MAX) * features_.size());
        int c = labels_[m];
        double X = features_[m][i];
        int h = (omega1_[i] * X + omega2_[i]) >= 0 ? 1 : -1;
        omega1_[i] = omega1_[i] - step * (double)(h - c) * X;
        omega2_[i] = omega2_[i] - step * (double)(h - c);
        if ( k > 30 && fabs(step * (double)(h - c)) < error){
            cout << "Error for omega1_[" << i << "] : " << step * (double)(h - c) * X << endl;
            cout << "Error for omega2_[" << i << "] : " << step * (double)(h - c) << endl;
            cout << "K = " << k << endl;
            cout << "omega1_[" << i << "] : " << omega1_[i] << endl;
            cout << "omega2_[" << i << "] : " << omega2_[i] << endl;
            cout << endl;
            break;
        }
        k++;
    }
}


//use mpi here!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
void Classifier::AllWeakClassifierMpi(double error){
    //parallel
    int size, rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Status status;

    //382536 caracs for one image. we use rank to distribute the work
    for(int i=rank; i<feature_dimension_; i+=size){
        OneWeakClassifier(i, error);
    }
    
    if(rank != 0){
        MPI_Send(omega1_, feature_dimension_, MPI_DOUBLE, 0, 442, MPI_COMM_WORLD);
        MPI_Send(omega2_, feature_dimension_, MPI_DOUBLE, 0, 442, MPI_COMM_WORLD);
    }
    else{
        for(int i=1; i<size; ++i){
            double* recv1 = new double[feature_dimension_];
            double* recv2 = new double[feature_dimension_];
            MPI_Recv(recv1, feature_dimension_, MPI_DOUBLE, i, 442, MPI_COMM_WORLD, &status);
            MPI_Recv(recv2, feature_dimension_, MPI_DOUBLE, i, 442, MPI_COMM_WORLD, &status);
            cout << "received from rank " << i << endl;
            Merge(i, size, recv1, recv2);
            delete[] recv1;
            delete[] recv2;
        }
        cout << "classifiers faibles created ! " << endl;
    }
}


void Classifier::Merge(int rank, int size, double* v1, double* v2){
    cout << "feature dimension in merge " << feature_dimension_ << endl;
    for(int i=rank; i<feature_dimension_; i+=size){
        omega1_[i] = v1[i];
        omega2_[i] = v2[i];
    }
}
