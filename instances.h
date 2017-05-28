#ifndef INSTANCES_H_
#define INSTANCES_H_

#include <vector>

#include "image.h"

class Instances {

    public:
        Instances(const char* path);
        ~Instances();

        int getNum_instances() const { return num_instances_; }
        int getNum_pos() const { return num_pos_; }
        int getNum_neg() const { return num_neg_; }
        int getFeature_dimension() const { return feature_dimension_; }

        vector<Image*> getInstances() const { return instances_; }
        Image* getInstance(int index) const { return instances_[index]; }

        void print_information() const;

    private:
        vector<Image*> instances_;
        const char* input_file_;

        int feature_dimension_;
        int num_instances_;
        int num_pos_;
        int num_neg_;
};

#endif    // END_INSTANCES_H_
