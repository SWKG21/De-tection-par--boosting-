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

        int getFeature_dimension() const { return feature_dimension_; }
        void WeakClassifierMpi(double e, int K);
        void Merge(int rank, int size, vector<double> v1, vector<double> v2);
        // int PredictLabel(Image* image_to_classify);
        // int Error(Image* image_to_classify);

    private:
        Instances* train_instances_;
        vector<double> omega1;
        vector<double> omega2;
        int feature_dimension_;
        //int k_;
};

#endif CLASSIFIER_H_