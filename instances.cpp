#include "instances.h"

#include <cstdio>
#include <iostream>
#include <dirent.h> 

using namespace std;

Instances::Instances(const char* path) { //path until the directory which contains two sub-directories: pos/ and neg/
    struct dirent *ptr;      
    DIR *dir;  
    string PATH = path;
    string PATHpos = PATH+"/pos/";
    string PATHneg = PATH+"/neg/";
    //vector<string> posfiles;
    //vector<string> negfiles;
    vector<Image*> instances;
    int num_instances;
    int num_pos;
    int num_neg;
    
    //read all files in app/pos
    dir=opendir(PATHpos.c_str());   
    cout << "File list: pos/"<< endl;  
    while((ptr=readdir(dir))!=NULL){   
        //do not consider '.' and '..' the two directories  
        if(ptr->d_name[0] == '.')  
            continue;
        //cout << ptr->d_name << endl; 
        //posfiles.push_back(ptr->d_name);
        Image* img = new Image(PATHpos+ptr->d_name);
        img->setLabel(1);
        instances.push_back(img);
        num_instances++;
        num_pos++;
        cout << PATHpos+ptr->d_name << endl;
    }  

    //read all files in app/neg
    dir=opendir(PATHneg.c_str());     
    cout << "File list: neg/"<< endl;  
    while((ptr=readdir(dir))!=NULL){   
        //do not consider '.' and '..' the two directories  
        if(ptr->d_name[0] == '.')  
            continue;
        //cout << ptr->d_name << endl;  
        //negfiles.push_back(ptr->d_name);
        Image* img = new Image(PATHneg+ptr->d_name);
        img->setLabel(-1);
        instances.push_back(img);
        num_instances++;
        num_neg++;
        cout << PATHneg+ptr->d_name << endl;
    }

    cout << "instances creation finished" << endl;

    closedir(dir);

    instances_ = instances;
    input_file_ = path;
    feature_dimension_ = instances[0]->getFeature_dimension();
    num_instances_ = num_instances;
    num_pos_ = num_pos;
    num_neg_ = num_neg;
}


Instances::~Instances() {
    for (unsigned int i = 0; i < instances_.size(); ++i) {
        delete(instances_[i]);
    } 
}


void Instances::print_information(){
    cout <<"Number of instances: "
        <<this->getNum_instances()
        <<" ("
        <<this->getNum_pos()
        <<" pos, " 
        <<this->getNum_neg()
        <<" neg)\n";
    cout <<"Feature dimension: "
        <<this->getFeature_dimension()
        <<endl;
}
