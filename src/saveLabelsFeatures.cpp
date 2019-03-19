#include "image.h"
#include <mpi.h>
#include <iostream>
#include <vector>  
#include <fstream>
#include <string>
#include <sstream>

using namespace std;

int main(int argc, char** argv){
    MPI_Init(&argc, &argv);
    int size, rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    //MPI_Status status;
    
    //all read images
    string PATH = argv[1];
    vector<Image*> images;
    for(int i=0; i<10; ++i){
        stringstream ss;
        ss << i;
        string name = PATH+"/pos/im"+ss.str()+".txt";
        Image* img = new Image(name);
        img->setLabel(1);
        images.push_back(img);
    }
    for(int i=0; i<20; ++i){
        stringstream ss;
        ss << i;
        string name = PATH+"/neg/im"+ss.str()+".txt";
        Image* img = new Image(name);
        img->setLabel(-1);
        images.push_back(img);
    }

    //root send integral of image to all machines
    if (rank == 0){
        for(int i=0; i<images.size(); ++i){
            images[i]->initialize();
            cout << "root finished initialize image" << i << endl;
        }
    }
    for(int i=0; i<images.size(); ++i){
        for(int j=0; j<92; ++j){
            MPI_Bcast(images[i]->getIntegral()[j], 112, MPI_DOUBLE, 0, MPI_COMM_WORLD);
        }
    }
    
    cout << "integral created by root and synchronized to all machines" << endl;

    for(int i=0; i<images.size(); ++i){
        //MPI here!!! Root must send all datas need for the calcul MPI!!!
        vector<double> f = images[i]->getFeaturesMpi();
        //for rank!=0, f here is not true!!!
        cout << "image " << i << " calcul features by mpi finished" << endl;
        if (rank == 0){
            string nametxt = "result/"+images[i]->getName();
            const char* forTxt = nametxt.c_str();
            ofstream outfile(forTxt);
            if (outfile.is_open()){
                //label of image is the first element in his file
                outfile << images[i]->getLabel() << endl;
                //then the features
                for(int i=0; i<f.size(); ++i){
                    outfile << f[i] << endl;
                }
                cout << "root wrote features in " << nametxt << endl;
                outfile.close();
            }
        }
    }

    MPI_Finalize();
}
