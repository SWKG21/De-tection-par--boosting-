#include "image.h"
//#include <CImg.h>
#include <mpi.h>
#include <iostream>
#include <vector>
#include <fstream>

using namespace std;

void Image::initialize(){
    const char* filename = name_.c_str();
    //initialise the data of pixels of this image(read width, height and data)
    ifstream file;
    file.open(filename, ios::out | ios::in);
    if (!file.is_open()) {
        cout << "Error: could not open matrix file '" << filename << "'" << endl;
    }
    file >> width_;
    file >> height_;

    double** imageData_;
    double** sumColumn_;
    double** integral_;
    imageData_ = new double * [height_];
    sumColumn_ = new double * [height_];
    integral_ = new double * [height_];
    for(int i=0; i<height_; ++i){
        imageData_[i] = new double[width_];
        sumColumn_[i] = new double[width_];
        integral_[i] = new double[width_];
    }
    int count = 0;
    while(count<width_*height_) {
        int i = count/width_;
        int j = count%width_;
        file >> imageData_[i][j];
        count++;
    }
    file.close();

    this->integralAll(integral_, sumColumn_, imageData_);
    delete[] imageData_;
    delete[] sumColumn_;
    //use mpi here!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    setFeaturesMpi(integral_);
    feature_dimension_ = features_.size();
    //cout << name_ << "'s features creation finished" << endl;
    delete[] integral_;
}


double Image::sumOf(double** sumColumn_, double** imageData_, int x, int y){//memorisation
    if(sumColumn_[y][x] == 0.0){
        if (y==0){
            sumColumn_[y][x] = imageData_[y][x];
        }
        else{
            sumColumn_[y][x] = sumColumn_[y-1][x] + imageData_[y][x];
        }
        return sumColumn_[y][x];
    }
    else{
        return sumColumn_[y][x];
    } 
}


double Image::integralOf(double** integral_, double** sumColumn_, double** imageData_, int x, int y){//memorisation
    if(integral_[y][x] == 0.0){
        if (x==0){
            integral_[y][x] = sumOf(sumColumn_, imageData_, x, y);
        }
        else{
            integral_[y][x] = integral_[y][x-1] + sumOf(sumColumn_, imageData_, x, y);
        }
        return integral_[y][x];
    }
    else{
        return integral_[y][x];
    }
}


void Image::integralAll(double** integral_, double** sumColumn_, double** imageData_){
    for(int i=0; i<width_; ++i){
        for(int j=0; j<height_; ++j){
            integralOf(integral_, sumColumn_, imageData_, i, j);
        }
    }
}


double Image::getIntegral(double** integral_, int x, int y){
    return integral_[y][x];
}


double Image::type1(double** integral_, Rectangle* r){
    int midX = ( r->getEndX() + r->getStartX() ) / 2;      //r->getStartX works too?????
    return getIntegral(integral_, r->getEndX(), r->getEndY()) + 2 * getIntegral(integral_, midX, r->getStartY()) + getIntegral(integral_, r->getStartX(), r->getEndY())
         - getIntegral(integral_, r->getEndX(), r->getStartY()) - 2 * getIntegral(integral_, midX, r->getEndY()) - getIntegral(integral_, r->getStartX(), r->getStartY());
}


double Image::type2(double** integral_, Rectangle* r){
    int midY = ( r->getEndY() + r->getStartY() ) / 2;
    return getIntegral(integral_, r->getStartX(), r->getEndY()) + 2 * getIntegral(integral_, r->getEndX(), midY) + getIntegral(integral_, r->getStartX(), r->getStartY())
        - getIntegral(integral_, r->getEndX(), r->getEndY()) - 2 * getIntegral(integral_, r->getStartX(), midY) - getIntegral(integral_, r->getEndX(), r->getStartY());
}


double Image::type3(double** integral_, Rectangle* r){
    int oneFourth = ( 3 * r->getStartX() + r->getEndX() ) / 4;
    int threeFourth = ( r->getStartX() + 3 * r->getEndX() ) / 4;
    return 2 * getIntegral(integral_, threeFourth, r->getEndY()) + 2 * getIntegral(integral_, oneFourth, r->getStartY()) + getIntegral(integral_, r->getEndX(), r->getStartY())
         + getIntegral(integral_, r->getStartX(), r->getEndY()) - 2 * getIntegral(integral_, threeFourth, r->getStartY()) - 2 * getIntegral(integral_, oneFourth, r->getEndY())
          - getIntegral(integral_, r->getEndX(), r->getEndY()) - getIntegral(integral_, r->getStartX(), r->getStartY());
}


double Image::type4(double** integral_, Rectangle* r){
    int midX = ( r->getEndX() + r->getStartX() ) / 2;
    int midY = ( r->getEndY() + r->getStartY() ) / 2;
    return 2 * ( getIntegral(integral_, r->getEndX(), midY) + getIntegral(integral_, r->getStartX(), midY) + getIntegral(integral_, midX, r->getEndY())
         + getIntegral(integral_, midX, r->getStartY()) ) - 4 * getIntegral(integral_, midX, midY) - getIntegral(integral_, r->getEndX(), r->getEndY())
          - getIntegral(integral_, r->getStartX(), r->getEndY()) - getIntegral(integral_, r->getEndX(), r->getStartY()) - getIntegral(integral_, r->getStartX(), r->getStartY());
}


//use mpi here!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
void Image::setFeaturesMpi(double** integral_){
    //sequentiel
    // vector<double> f;
    // for(int i=0; i<27; ++i){
    //     for(int j=0; j<22; ++j){
    //         //size fixed with width = 8+4i, height = 8+4j
    //         for(int m=0; m<27-i; ++m){
    //             for(int n=0; n<22-j; ++n){
    //                 //position fixed with (4*m, 4*n)
    //                 Rectangle* r = new Rectangle(4*m, 4*n, 8+4*i+4*m, 8+4*j+4*n);
    //                 f.push_back(type1(r));
    //                 f.push_back(type2(r));
    //                 f.push_back(type3(r));
    //                 f.push_back(type4(r));
    //                 delete r;
    //             }
    //         }
    //     }
    // }

    //parallel
    int size, rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Status status;
    vector<double> f;

    // //when #machine less than 27, use i to distribute the work.
    // //local calcul of vector of feature
    // for(int i=rank; i<27; i+=size;){
    //     for(int j=0; j<22; ++j){
    //         //size fixed with width = 8+4i, height = 8+4j
    //         for(int m=0; m<27-i; ++m){
    //             for(int n=0; n<22-j; ++n){
    //                 //position fixed with (4*m, 4*n)
    //                 Rectangle* r = new Rectangle(4*m, 4*n, 8+4*i+4*m-1, 8+4*j+4*n-1);
    //                 //4 types of feature for every rectangle 
    //                 f.push_back(type1(r));
    //                 f.push_back(type2(r));
    //                 f.push_back(type3(r));
    //                 f.push_back(type4(r));
    //                 delete r;
    //             }
    //         }
    //     }
    // }
    // //every machine except root send vector of feature then root receive their vectors. use Send and Recv bloquant.
    // if (rank != 0){
    //     MPI_Send(f, f.size(), MPI_DOUBLE, 0, 442, MPI_COMM_WORLD);
    // }
    // else{
    //     for(int r=1; r<size; ++r){
    //         //root should count the number of feature for every machine
    //         int count = 0;
    //         for(int i=r; i<27; i+=size){
    //             for(int j=0; j<22; ++j){
    //                 count += 4 * (27-i) * (22-j);
    //             }
    //         }
    //         //root receive vector of feature of every machine
    //         vector<double> recv;
    //         MPI_Recv(recv, count, MPI_DOUBLE, r, 442, MPI_COMM_WORLD, &status);
    //         f.insert(f.end(), recv.begin(), recv.end());
    //     }  
    // }

    //when #machine less than 27*22=594, we use (i,j) to distribute the work.
    //we have 594 pieces of work. we distribute them to size machines 
    for(int n=rank; n<594; n+=size){
        //for machine rank, arrange No.n work
        int i = n / 22;
        int j = n % 22;
        //size fixed with width = 8+4i, height = 8+4j
        for(int m=0; m<27-i; ++m){
            for(int n=0; n<22-j; ++n){
                //position fixed with (4*m, 4*n)
                Rectangle* r = new Rectangle(4*m, 4*n, 8+4*i+4*m-1, 8+4*j+4*n-1);
                //4 types of feature for every rectangle 
                f.push_back(type1(integral_, r));
                f.push_back(type2(integral_, r));
                f.push_back(type3(integral_, r));
                f.push_back(type4(integral_, r));
                delete r;
            }
        }
    }
    //every machine except root send vector of feature then root receive their vectors. use Send and Recv bloquant.
    if (rank != 0){
        int sendsize = f.size();
        double* send = new double[sendsize];
        for(int i=0; i<sendsize; ++i){
            send[i] = f[i];
        }
        MPI_Send(send, sendsize, MPI_DOUBLE, 0, 442, MPI_COMM_WORLD);
        delete[] send;
    }
    else{
        for(int r=1; r<size; ++r){
            //root should count the number of feature for every machine
            int count = 0;
            for(int n=r; n<594; n+=size){
                //for machine rank, arrange No.n work
                int i = n / 22;
                int j = n % 22;
                count += 4 * (27-i) * (22-j);
            }
            //root receive vector of feature of every machine
            double* recv = new double[count];
            MPI_Recv(recv, count, MPI_DOUBLE, r, 442, MPI_COMM_WORLD, &status);
            vector<double> tmp(count);
            for(int i=0; i<count; ++i){
                tmp[i] = recv[i];
            }
            f.insert(f.end(), tmp.begin(), tmp.end());
            delete[] recv;
        }
        this->features_ = f;
    }
    //there are 95634 rectangles, thus 4*95634=382536 features for one image
}
