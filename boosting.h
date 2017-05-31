#ifndef BOOSTING_H_
#define BOOSTING_H_

#include "image.h"
#include "classifier.h"

class Boosting{

    public:
        Boosting(vector<Image*> valid_instances, Classifier* classifier);
        ~Boosting() {};

        Classifier* getClassifier() const {return classifier_;}
        int getFeature_dimension() const { return feature_dimension_; }
        int PredictLabel(int i, Image* image_to_validate);
        int Error(int i, Image* image_to_validate);
        double* minClassifierMpi();
        void Boost(int N);
        int ClassifyFinal(double theta, Image* image_to_classify);

    private:
        vector<Image*> valid_instances_;
        vector<double> lambda_;
        int feature_dimension_;
        Classifier* classifier_;
        vector<double> omega1Boosted_;
        vector<double> omega2Boosted_;
        double sumOfAlpha_;
};

#endif    // END_BOOSTING_H_
