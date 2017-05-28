#ifndef CLASSIFIER_H_
#define CLASSIFIER_H_

#include "instances.h"
#include "image.h"
#include <vector>

class Classifier {

    public:
        //Classifier(Instances* train_instances, int k): train_instances_(train_instances), feature_dimension_(train_instances->feature_dimension()), k_(k) {};
        Classifier(Instances* train_instances);
        ~Classifier() {};

        void setFeaturesMpi();
        int getFeature_dimension() const { return feature_dimension_; }
        void OneWeakClassifier(int i, double error);
        void AllWeakClassifierMpi(double error);
        void Merge(int rank, int size, vector<double> v1, vector<double> v2);
        vector<double> getOmega1() const {return omega1_;}
        vector<double> getOmega2() const {return omega2_;}

    private:
        Instances* train_instances_;
        vector<double> omega1_;
        vector<double> omega2_;
        int feature_dimension_;
        //int k_;
};

#endif    // END_CLASSIFIER_H_
