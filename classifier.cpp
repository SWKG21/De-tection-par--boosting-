#include "instances.h"
#include "image.h"
#include "classifier.h"
#include <vector>
#include <cstdlib>
#include <mpi.h>
using namespace std;

Classifier::Classifier(Instances* train_instances){
    train_instances_ = train_instances;
    feature_dimension_ = train_instances->getFeature_dimension();
    for(int i=0; i<feature_dimension_; ++i){
        omega1.push_back(1.0);
        omega2.push_back(0.0);
    }
    
}

void Classifier::WeakClassifierMpi(double e, int K){ //e, K are global?????
    //parallel
    int size, rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Status status;

    //382536 caracs for one image. we use rank to distribute the work
    int d = this->getFeature_dimension();
    for(int i=rank; i<d; i+=size){
        //h(i), rank decides i
        vector<Image*> ins = train_instances_->getInstances();
        for(int k=1; k<=K; ++k){
            int m = (int)((rand()/(double)RAND_MAX) * ins.size());
            Image* img = ins[m];
            int c = img->getLabel();
            //TODO, use mpi here!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
            double X = img->featuresMpi()[i];
            int h = omega1[i] * X + omega2[i] >= 0 ? 1 : -1;
            omega1[i] = omega1[i] - e * (h - c) * X;
            omega2[i] = omega2[i] - e * (h - c); 
        }
    }
    
    if(rank != 0){
        MPI_Send(&omega1, d, MPI_DOUBLE, 0, 442, MPI_COMM_WORLD);
        MPI_Send(&omega2, d, MPI_DOUBLE, 0, 442, MPI_COMM_WORLD);
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

    //srand(rank);!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
}


void Classifier::Merge(int rank, int size, vector<double> v1, vector<double> v2){
    if ( v1.size() != omega1.size() || v2.size() != omega2.size()) cerr << "Merge Error !" << endl;
    else{
        for(int i=rank; i<this->getFeature_dimension(); i+=size){
            omega1[i] = v1[i];
            omega2[i] = v2[i];
        }
    }
}


// int Classifier::PredictLabel(Image* image_to_classify){
//     int count_pos = 0;
//     int count_neg = 0;
//     vector<double> features = image_to_classify->featuresMpi();
//     for(int i=0; i<this->getFeature_dimension(); ++i){
//         if (omega1[i] * features[i] + omega2[i] >= 0){
//             count_pos++;
//         }
//         else{
//             count_neg++;
//         }
//     }
//     return count_pos > count_neg ? 1 : -1;
// }


// int Classifier::Error(Image* image_to_classify){
//     return this->PredictLabel(image_to_classify) == image_to_classify->getLabel() ? 0 : 1;
// }