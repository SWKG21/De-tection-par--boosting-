#include "instances.h"
#include "image.h"
#include "classifier.h"
#include <mpi.h>
#include <vector>
#include <cstdlib>
#include <math.h>
#include <iostream>
using namespace std;

Classifier::Classifier(vector<Image*> train_instances){
    train_instances_ = train_instances;
    feature_dimension_ = train_instances[0]->getFeature_dimension();
    for(int i=0; i<feature_dimension_; ++i){
        omega1_.push_back(1.0);
        omega2_.push_back(0.0);
    }
    
}


void Classifier::OneWeakClassifier(int i, double error){
    //set the step first
    double step = 0.1;
    int k = 1;
    while(true){
        int m = (int)((rand()/(double)RAND_MAX) * train_instances_.size());
        Image* img = train_instances_[m];
        int c = img->getLabel();
        double X = img->getFeatures()[i];
        int h = (omega1_[i] * X + omega2_[i]) >= 0 ? 1 : -1;
        omega1_[i] = omega1_[i] - step * (double)(h - c) * X;
        omega2_[i] = omega2_[i] - step * (double)(h - c);
        if (fabs(step * (h - c)) < error){
            cout << "Error for omega1_[" << i << "] : " << step * (double)(h - c) * X << endl;
            cout << "Error for omega2_[" << i << "] : " << step * (double)(h - c) << endl;
            cout << "K = " << k << endl;
            cout << "step = " << step << endl;
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
    int d = this->getFeature_dimension();
    if (rank == 0) cout << "dimension of feature : " << d << endl;
    for(int i=rank; i<d; i+=size){
        cerr << "rank " << rank << " classifier " << i << " start!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl;
        OneWeakClassifier(i, error);

    }
    
    if(rank != 0){
        double* tmp1 = new double[d];
        double* tmp2 = new double[d];
        for(int i=0; i<d; ++i){
            tmp1[i] = omega1_[i];
            tmp2[i] = omega2_[i];
        }
        cerr << "rank " << rank << " start send to root!!!!!!!" << endl;
        MPI_Send(tmp1, d, MPI_DOUBLE, 0, 442, MPI_COMM_WORLD);
        MPI_Send(tmp2, d, MPI_DOUBLE, 0, 442, MPI_COMM_WORLD);
        cerr << "rank " << rank << " sent to root!!!!!!!" << endl;
        delete[] tmp1;
        delete[] tmp2;
    }
    else{
        for(int i=1; i<size; ++i){
            double* recv1 = new double[d];
            double* recv2 = new double[d];
            cerr << "start receive from rank " << i << endl;
            MPI_Recv(recv1, d, MPI_DOUBLE, i, 442, MPI_COMM_WORLD, &status);
            MPI_Recv(recv2, d, MPI_DOUBLE, i, 442, MPI_COMM_WORLD, &status);
            cerr << "received from rank " << rank << endl; 
            Merge(i, size, recv1, recv2);
            delete[] recv1;
            delete[] recv2;
        }
        cout << "classifiers faibles created ! " << endl;
    }
}


void Classifier::Merge(int rank, int size, double* v1, double* v2){
    if ( sizeof(v1) != omega1_.size() || sizeof(v2) != omega2_.size()) cerr << "Merge Error !" << endl;
    else{
        for(int i=rank; i<this->getFeature_dimension(); i+=size){
            omega1_[i] = v1[i];
            omega2_[i] = v2[i];
        }
    }
}
