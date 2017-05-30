#ifndef IMAGE_H_
#define IMAGE_H_

#include "rectangle.h"
#include <vector>
#include <string>

using namespace std;

class Image{

    public:
        Image(string filename);
        ~Image() {};

        string getName() const {return name_;}
        //vector<vector<double> > getData() const {return imageData_;}
        int getWidth() const {return width_;}
        int getHeight() const {return height_;}
        int getFeature_dimension() const { return feature_dimension_; }
        int getLabel() const {return label_;}
        void setLabel(int label) {label_= label;}
        vector<double> getFeatures() const { return features_; }

        double valueOf(double** imageData_, int x, int y);
        double sumOf(double** sumColumn_, double** imageData_, int x, int y);
        double integralOf(double** integral_, double** sumColumn_, double** imageData_, int x, int y);
        void integralAll(double** integral_, double** sumColumn_, double** imageData_);
        double getIntegral(double** integral_, int x, int y);

        double type1(double** integral_, Rectangle* r);
        double type2(double** integral_, Rectangle* r);
        double type3(double** integral_, Rectangle* r);
        double type4(double** integral_, Rectangle* r);

        void setFeaturesMpi(double** integral_);
        void initialize();

    private:
        string name_;
        vector<double> features_;
        int width_;
        int height_;
        int feature_dimension_;
        int label_; //-1 for neg, 1 for pos

};

#endif    // END_IMAGE_H_
