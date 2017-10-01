#include "boosting.h"
#include <mpi.h>
#include <math.h>
#include <float.h>

using namespace std;


Boosting::Boosting(vector<vector<double> >& valid_features, vector<int>& valid_labels, double* omega1, double* omega2){
    valid_features_ = valid_features;
    valid_labels_ = valid_labels;
    feature_dimension_ = valid_features[0].size();
    omega1_ = omega1;
    omega2_ = omega2;
    //initialise lambda_
    int n = valid_features.size();
    vector<double> v(n, 1/(double)n);
    lambda_ = v;
    //initialise omega1Boosted and omega2Boosted
    omega1Boosted_ = new double[feature_dimension_];
    omega2Boosted_ = new double[feature_dimension_];
    sumOfAlpha_ = 0.0;
}


int Boosting::PredictLabel(int i, vector<double>& feature_to_validate){// ith classifier for ith feature
    double X = feature_to_validate[i];
    return omega1_[i] * X + omega2_[i] >= 0 ? 1 : -1;
}


int Boosting::Error(int i, vector<double>& feature_to_validate, int label_to_validate){// ith classifier for ith feature
    return PredictLabel(i, feature_to_validate) == label_to_validate ? 0 : 1;
}


double* Boosting::minClassifierMpi(){
    int size, rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Status status;

    double* min = new double[2];
    min[0] = DBL_MAX;//for min error. MAX of C++
    min[1] = 0.0;// for min classifier
    int n = valid_features_.size();
    int d = feature_dimension_;
    //local calcul
    for(int i=rank; i<d; i+=size){
        double s = 0.0;
        for(int j=0; j<n; ++j){
            s += lambda_[j] * Error(i, valid_features_[j], valid_labels_[j]);
            cout << "(i, j) : " << i << ", " << j << endl;
        }
        if (s != 0 && s < min[0]){               //min[0] can not be 0!!!
            min[0] = s;
            min[1] = (double)i;
        }
    }
    
    if(rank != 0){
        //non-root
        MPI_Send(min, 2, MPI_DOUBLE, 0, 442, MPI_COMM_WORLD);
    }
    else{
        //root
        for(int i=1; i<size; ++i){
            double* recv = new double[2];
            MPI_Recv(recv, 2, MPI_DOUBLE, i, 442, MPI_COMM_WORLD, &status);
            if (recv[0] < min[0]){
                min[0] = recv[0];
                min[1] = recv[1];
            }
            delete[] recv;
        } 
    }
    return min;
}


//use mpi here!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
void Boosting::Boost(int N){
    int size, rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Status status;
    int n = valid_features_.size();
    for(int k=0; k<N; ++k){
        cout << "k=" << k << endl;
        double* min = minClassifierMpi();
        //cout << "min found" << endl;
        //for non-root, min not true!!!
        //root give min to all machines
        MPI_Bcast(min,2,MPI_DOUBLE,0,MPI_COMM_WORLD);
        cout << "min[0] in rank " << rank << " after broadcast = " << min[0] << endl;
        double alpha = 0.5 * log( (1.0-min[0]) / (double)min[0] );
        
        omega1Boosted_[(int)min[1]] += alpha * omega1_[(int)min[1]];
        omega2Boosted_[(int)min[1]] += alpha * omega2_[(int)min[1]];

        //update lambda_
        double s = 0.0;
        for(int j=0; j<n; ++j){
            lambda_[j] = lambda_[j] * exp( - valid_labels_[j] * alpha *
                  PredictLabel((int)min[1], valid_features_[j]));
            s += lambda_[j];
        }
        //renormaliser lambda_
        for(int j=0; j<n; ++j){
            lambda_[j] = lambda_[j] / s;
        }

        sumOfAlpha_ += alpha;
    }
    cout << endl;
    cout << "rank " << rank << " boost finished!!!" << endl;
    cout << endl;
}
