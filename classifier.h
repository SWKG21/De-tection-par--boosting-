#ifndef CLASSIFIER_H_
#define CLASSIFIER_H_

#include "image.h"
#include <vector>

class Classifier {

    public:
        //Classifier(Instances* train_instances, int k): train_instances_(train_instances), feature_dimension_(train_instances->feature_dimension()), k_(k) {};
        Classifier(vector<Image*> train_instances);
        ~Classifier() {};

        //void setFeaturesMpi();
        int getFeature_dimension() const { return feature_dimension_; }
        void OneWeakClassifier(int i, double error);
        void AllWeakClassifierMpi(double error);
        void Merge(int rank, int size, double* v1, double* v2);
        vector<double> getOmega1() const {return omega1_;}
        vector<double> getOmega2() const {return omega2_;}

    private:
        vector<Image*> train_instances_;
        vector<double> omega1_;
        vector<double> omega2_;
        int feature_dimension_;
};

#endif    // END_CLASSIFIER_H_
