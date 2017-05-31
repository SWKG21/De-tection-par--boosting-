#include "image.h"
#include "instances.h"
#include "classifier.h"
#include "boosting.h"
#include <mpi.h>
#include <iostream>
#include <vector>  
#include <dirent.h>
#include <fstream>
#include <stdlib.h>
#include <string>
#include <sstream>

using namespace std;

int main(int argc, char* argv[]){
    MPI_Init(&argc, &argv);
    int size, rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    srand(rank);

    string PATH1 = "app";
    const char* path1 = PATH1.c_str();
    vector<Image*> tra;
    for(int i=0; i<1; ++i){
        stringstream ss;
        ss << i;
        string name = PATH1+"/pos/im"+ss.str()+".txt";
        Image* img = new Image(name);
        img->setLabel(1);
        tra.push_back(img);
    }
    for(int i=0; i<2; ++i){
        stringstream ss;
        ss << i;
        string name = PATH1+"/neg/im"+ss.str()+".txt";
        Image* img = new Image(name);
        img->setLabel(-1);
        tra.push_back(img);
    }
    if(rank == 0) cout << "all images created" << endl;
    for(int i=0; i<tra.size(); ++i){
        tra[i]->initialize();
        cout << tra[i]->getFeature_dimension() << endl;
    }
    Classifier* clf = new Classifier(tra);
    clf->AllWeakClassifierMpi(0.0001);

    string PATH2 = "dev";
    const char* path2 = PATH2.c_str();
    vector<Image*> val;
    for(int i=0; i<1; ++i){
        stringstream ss;
        ss << i;
        string name = PATH2+"/pos/im"+ss.str()+".txt";
        Image* img = new Image(name);
        img->setLabel(1);
        val.push_back(img);
    }
    for(int i=0; i<2; ++i){
        stringstream ss;
        ss << i;
        string name = PATH2+"/neg/im"+ss.str()+".txt";
        Image* img = new Image(name);
        img->setLabel(-1);
        val.push_back(img);
    }
    if(rank == 0) cout << "all images created" << endl;
    for(int i=0; i<val.size(); ++i){
        val[i]->initialize();
        cout << val[i]->getFeature_dimension() << endl;
    }
    Boosting* bst = new Boosting(val, clf);
    bst->Boost(1000);
    
    MPI_Finalize();
}
