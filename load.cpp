#include "load.h"
#include <iostream>
#include <vector>  
#include <fstream>
#include <string>
#include <sstream>

using namespace std;

void loadLabelsFeatures(string PATH, vector<int>& labels, vector<vector<double> >& features){//ex.result/app
    for(int i=0; i<10; ++i){
        stringstream ss;
        ss << i;
        string name = PATH+"/pos/im"+ss.str()+".txt";
        ifstream file;
        file.open(name.c_str(), ios::out | ios::in);
        if (!file.is_open()) {
            cout << "Error: could not open file '" << name << "'" << endl;
        }
        //load labels
        int label;
        file >> label;
        labels.push_back(label);
        //load features
        vector<double> f;
        double data;
        for(int i=0; i<382536; ++i){
            file >> data;
            f.push_back(data);
        }
        features.push_back(f);
    }

    for(int i=0; i<20; ++i){
        stringstream ss;
        ss << i;
        string name = PATH+"/neg/im"+ss.str()+".txt";
        ifstream file;
        file.open(name.c_str(), ios::out | ios::in);
        if (!file.is_open()) {
            cout << "Error: could not open file '" << name << "'" << endl;
        }
        //load labels
        int label;
        file >> label;
        labels.push_back(label);
        //load features
        vector<double> f;
        double data;
        for(int i=0; i<382536; ++i){
            file >> data;
            f.push_back(data);
        }
        features.push_back(f);
    }
    cout << "load labels and features from " << PATH << " finished !" << endl;
    cout << features.size() << " features and " << labels.size() << " labels !" << endl;
    cout << "Every feature size : " << features[0].size() << endl;
}


void loadCFOmega1(double* omega1){
    string name = "result/CF/omega1.txt";
    ifstream file;
    file.open(name.c_str(), ios::out | ios::in);
    if (!file.is_open()) {
        cout << "Error: could not open file '" << name << "'" << endl;
    }
    double data;
    for(int i=0; i<382536; ++i){
        file >> data;  
        omega1[i] = data;
    }
    cout << "load CF/omega1 finished !" << endl;
}


void loadCFOmega2(double* omega2){
    string name = "result/CF/omega2.txt";
    ifstream file;
    file.open(name.c_str(), ios::out | ios::in);
    if (!file.is_open()) {
        cout << "Error: could not open file '" << name << "'" << endl;
    }
    double data;
    for(int i=0; i<382536; ++i){
        file >> data;
        omega2[i] = data;
    }
    cout << "load CF/omega2 finished !" << endl;
}


void loadBoostedOmega1(double* Bomega1){
    string name = "result/boosted/Bomega1.txt";
    ifstream file;
    file.open(name.c_str(), ios::out | ios::in);
    //cout << "ok here" << endl;
    if (!file.is_open()) {
        cout << "Error: could not open file '" << name << "'" << endl;
    }
    double data;
    //cout << "ok here" << endl;
    for(int i=0; i<382536; ++i){
        file >> data;
        Bomega1[i] = data;
        cout << "ok here" << endl;
    }
    cout << "load boosted/Bomega1 finished !" << endl;
}


void loadBoostedOmega2(double* Bomega2){
    string name = "result/boosted/Bomega2.txt";
    ifstream file;
    file.open(name.c_str(), ios::out | ios::in);
    if (!file.is_open()) {
        cout << "Error: could not open file '" << name << "'" << endl;
    }
    double data;
    for(int i=0; i<382536; ++i){
        file >> data;
        Bomega2[i] = data;
    }
    cout << "load boosted/Bomega2 finished !" << endl;
}


void loadSumOfAlpha(double* sumOfAlpha){
    string name = "result/boosted/sumOfAlpha.txt";
    ifstream file;
    file.open(name.c_str(), ios::out | ios::in);
    if (!file.is_open()) {
        cout << "Error: could not open file '" << name << "'" << endl;
    }
    file >> *sumOfAlpha;
    cout << "load sumOfAlpha finished !" << endl;
}


void loadImageFeatures(string name, vector<double>& features){
    ifstream file;
    file.open(name.c_str(), ios::out | ios::in);
    if (!file.is_open()) {
        cout << "Error: could not open file '" << name << "'" << endl;
    }
    double data;
    file >> data;
    for(int i=0; i<382536; ++i){
        file >> data;
        features.push_back(data);
    }
}
