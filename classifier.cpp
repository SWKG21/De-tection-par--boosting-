#include "instances.h"
#include "image.h"
#include "classifier.h"
#include <mpi.h>
#include <vector>
#include <cstdlib>
#include <math.h>
#include <iostream>
using namespace std;

Classifier::Classifier(Instances* train_instances){
    train_instances_ = train_instances;
    feature_dimension_ = train_instances->getFeature_dimension();
    for(int i=0; i<feature_dimension_; ++i){
        omega1_.push_back(1.0);
        omega2_.push_back(0.0);
    }
    
}


// //use mpi here!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// void Classifier::setFeaturesMpi(){
//     Instances* train_ins = this->train_instances_;
//     int n = train_ins->getNum_instances();
//     vector<Image*> ins = train_ins->getInstances();
//     for(int i=0; i<n; ++i){
//         ins[i]->setFeaturesMpi();
//     }
// }


void Classifier::OneWeakClassifier(int i, double error){
    //set the step first
    double step = 0.000001;
    vector<Image*> ins = train_instances_->getInstances();
    int k = 1;
    while(true){
        int m = (int)((rand()/(double)RAND_MAX) * ins.size());
        Image* img = ins[m];
        int c = img->getLabel();
        double X = img->getFeatures()[i];
        int h = omega1_[i] * X + omega2_[i] >= 0 ? 1 : -1;
        omega1_[i] = omega1_[i] - step * (h - c) * X;
        omega2_[i] = omega2_[i] - step * (h - c);
        k++;
        if (fabs(step * (h - c)) < error){
            cout << "Error for omega1_[" << i << "] : " << step * (h - c) * X << endl;
            cout << "Error for omega2_[" << i << "] : " << step * (h - c) << endl;
            cout << "K = " << k << endl;
            cout << "step = " << step << endl;
            break;
        }
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
    for(int i=rank; i<d; i+=size){
        OneWeakClassifier(i, error);
    }
    
    if(rank != 0){
        MPI_Send(&omega1_, d, MPI_DOUBLE, 0, 442, MPI_COMM_WORLD);
        MPI_Send(&omega2_, d, MPI_DOUBLE, 0, 442, MPI_COMM_WORLD);
    }
    else{
        for(int i=1; i<size; ++i){
            vector<double> recv1;
            vector<double> recv2;
            MPI_Recv(&recv1, d, MPI_DOUBLE, i, 442, MPI_COMM_WORLD, &status);
            MPI_Recv(&recv2, d, MPI_DOUBLE, i, 442, MPI_COMM_WORLD, &status);
            Merge(i, size, recv1, recv2);
        }   
    }
}


void Classifier::Merge(int rank, int size, vector<double> v1, vector<double> v2){
    if ( v1.size() != omega1_.size() || v2.size() != omega2_.size()) cerr << "Merge Error !" << endl;
    else{
        for(int i=rank; i<this->getFeature_dimension(); i+=size){
            omega1_[i] = v1[i];
            omega2_[i] = v2[i];
        }
    }
}
