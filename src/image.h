#ifndef IMAGE_H_
#define IMAGE_H_

#include "rectangle.h"
#include <vector>
#include <string>

using namespace std;

class Image{

    public:
        Image(string name);
        ~Image() {};

        string getName() const {return name_;}
        //vector<vector<double> > getData() const {return imageData_;}
        int getWidth() const {return width_;}
        int getHeight() const {return height_;}
        int getLabel() const {return label_;}
        void setLabel(int label) {label_= label;}
        double** getIntegral() const { return integral_; }

        double sumOf(double** sumColumn_, double** imageData_, int x, int y);
        double integralOf(double** integral_, double** sumColumn_, double** imageData_, int x, int y);
        void integralAll(double** integral_, double** sumColumn_, double** imageData_);
        double getIntegral(double** integral_, int x, int y);

        double type1(double** integral_, Rectangle* r);
        double type2(double** integral_, Rectangle* r);
        double type3(double** integral_, Rectangle* r);
        double type4(double** integral_, Rectangle* r);

        vector<double> getFeaturesMpi();
        void initialize();

    private:
        string name_;
        double** integral_;
        int width_;
        int height_;
        int label_; //-1 for neg, 1 for pos

};

#endif    // END_IMAGE_H_
