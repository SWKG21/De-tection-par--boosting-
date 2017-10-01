#ifndef BOOSTING_H_
#define BOOSTING_H_

#include "image.h"
#include "classifier.h"

class Boosting{

    public:
        Boosting(vector<vector<double> >& valid_features, vector<int>& valid_labels, double* omega1, double* omega2);
        ~Boosting();
        int getFeature_dimension() const { return feature_dimension_; }
        int PredictLabel(int i, vector<double>& feature_to_validate);
        int Error(int i, vector<double>& feature_to_validate, int label_to_validate);
        double* minClassifierMpi();
        void Boost(int N);
        double* getOmega1Boosted() {return omega1Boosted_;}
        double* getOmega2Boosted() {return omega2Boosted_;}
        double getSumOfAlpha() {return sumOfAlpha_;}

    private:
        vector<vector<double> > valid_features_;
        vector<int> valid_labels_;
        double* omega1_;
        double* omega2_;
        vector<double> lambda_;
        int feature_dimension_;
        double* omega1Boosted_;
        double* omega2Boosted_;
        double sumOfAlpha_;
};

#endif    // END_BOOSTING_H_
