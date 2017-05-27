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
        vector<vector<double> > getData() const {return imageData_;}
        int getWidth() const {return width_;}
        int getHeight() const {return height_;}
        int getFeature_dimension() const { return feature_dimension_; }
        int getLabel() const {return label_;}
        void setLabel(int label) {label_= label;}

        double valueOf(int x, int y);
        double sumOf(int x, int y);
        double integralOf(int x, int y);
        double integral();

        double type1(Rectangle* r);
        double type2(Rectangle* r);
        double type3(Rectangle* r);
        double type4(Rectangle* r);

        vector<double> featuresMpi();

    private:
        string name_;
        vector<vector<double> > imageData_;
        int width_;
        int height_;
        int feature_dimension_;
        int label_; //-1 for neg, 1 for pos

};

#endif    // END_IMAGE_H_