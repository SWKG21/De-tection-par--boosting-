#ifndef CLASSIFIER_H_
#define CLASSIFIER_H_

#include <vector>
using namespace std;

class Classifier {

    public:
        Classifier(vector<vector<double> >& features, vector<int>& labels);
        ~Classifier() {};

        void OneWeakClassifier(int i, double error);
        void AllWeakClassifierMpi(double error);
        void Merge(int rank, int size, double* v1, double* v2);
        double* getOmega1() {return omega1_;}
        double* getOmega2() {return omega2_;}
        void setOmega1(int pos, double value) {omega1_[pos] = value;}
        void setOmega2(int pos, double value) {omega2_[pos] = value;}
        int getFeatureDimension() {return feature_dimension_;}

    private:
        vector<vector<double> > features_;
        vector<int> labels_;
        double* omega1_;
        double* omega2_;
        int feature_dimension_;
};

#endif    // END_CLASSIFIER_H_
