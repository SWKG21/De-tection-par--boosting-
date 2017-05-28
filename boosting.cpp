#include "boosting.h"
#include <mpi.h>
#include <math.h>

using namespace std;


Boosting::Boosting(Instances* valid_instances, Classifier* classifier){
    valid_instances_ = valid_instances;
    feature_dimension_ = valid_instances->getFeature_dimension();
    classifier_ = classifier;
    //initialise lambda_
    int n = valid_instances->getNum_instances();
    vector<double> v(n, 1/(double)n);
    lambda_ = v;
    //initialise omega1Boosted and omega2Boosted
    vector<double> omega1Boosted(feature_dimension_);
    vector<double> omega2Boosted(feature_dimension_);
}


//use mpi here!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
void Boosting::setFeaturesMpi(){
    Instances* valid_ins = this->valid_instances_;
    int n = valid_ins->getNum_instances();
    vector<Image*> ins = valid_ins->getInstances();
    for(int i=0; i<n; ++i){
        ins[i]->setFeaturesMpi();
    }
}


int Boosting::PredictLabel(int i, Image* image_to_validate){// ith classifier for ith feature
    double X = image_to_validate->getFeatures()[i];
    return classifier_->getOmega1()[i] * X + classifier_->getOmega2()[i] >= 0 ? 1 : -1;
}


int Boosting::Error(int i, Image* image_to_validate){// ith classifier for ith feature
    return this->PredictLabel(i, image_to_validate) == image_to_validate->getLabel() ? 0 : 1;
}


vector<double> Boosting::minClassifierMpi(){
    int size, rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Status status;

    vector<double> min; 
    min.push_back(1000.0);//for min error. MAX of C++???
    min.push_back(0.0);// for min classifier
    int n = valid_instances_->getNum_instances();
    vector<Image*> vimg = valid_instances_->getInstances();
    int d = this->getFeature_dimension();

    //local calcul
    for(int i=rank; i<d; i+=size){
        double s = 0.0;
        for(int j=0; j<n; ++j){
            s += lambda_[j] * Error(i, vimg[j]);
        }
        if (s < min[0]){
            min[0] = s;
            min[1] = i;
        }
    }
    
    if(rank != 0){
        //non-root
        MPI_Send(&min, 2, MPI_DOUBLE, 0, 442, MPI_COMM_WORLD);
    }
    else{
        //root
        for(int i=1; i<size; ++i){
            vector<double> recv;
            MPI_Recv(&recv, 2, MPI_DOUBLE, i, 442, MPI_COMM_WORLD, &status);
            if (recv[0] < min[0]){
                min[0] = recv[0];
                min[1] = recv[1];
            }
        }
        return min;  
    }

    return min;
}


//use mpi here!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
void Boosting::Boost(int N){
    int n = valid_instances_->getNum_instances();
    vector<Image*> vimg = valid_instances_->getInstances();
    for(int k=0; k<=N; ++k){
        vector<double> min = this->minClassifierMpi();
        double alpha = 0.5 * log( (1.0-min[0]) / min[0] );
        omega1Boosted_[(int)min[1]] = alpha * this->getClassifier()->getOmega1()[(int)min[1]];
        omega2Boosted_[(int)min[1]] = alpha * this->getClassifier()->getOmega2()(int)min[1]];

        //update lambda_
        double s = 0.0;
        for(int j=0; j<n; ++j){
            lambda_[j] = lambda_[j] * exp( - vimg[j]->getLabel() * alpha * this->PredictLabel((int)min[1], vimg[j]));
            s += lambda_[j];
        }
        //renormaliser almbda_
        for(int j=0; j<n; ++j){
            lambda_[j] = lambda_[j] / s;
        }

        sumOfAlpha_ += alpha;
    }
}


int Boosting::ClassifyFinal(double theta, Image* image_to_classify){
    int d = this->getFeature_dimension();
    double F = 0.0;
    for(int i=0; i<d; ++i){
        if (omega1Boosted_[i] != 0 || omega2Boosted_[i] != 0){
            F += omega1Boosted_[i] * image_to_classify->getFeatures()[i] + omega2Boosted_[i]; 
        } 
    }
    return F >= theta * sumOfAlpha_ ? 1 : -1;
}

