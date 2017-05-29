#include "image.h"
#include <CImg.h>
#include <mpi.h>
#include <iostream>
#include <vector>

using namespace std;
using namespace cimg_library;


Image::Image(string name){
    name_ = name;
    const char* filename = name.c_str();

    // //CImg doesn't work with mpi. So transform all jpg to txt and read txt instead.
    // CImg<unsigned char> image(file); // seulement pour jpg ici, non raw
    // CImg<unsigned char>::iterator it = image.begin();
    
    //initialise the data of pixels of this image(read width, height and data)
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Error: could not open matrix file '" << filename << "'" << endl;std::exit(1);
    }
    file >> width_;
    file >> height_;

    int cpt = 0;
    vector<vector<double> > tmp;
    vector<double> line;
    while(cpt<width_*height_) {
        double data = 0.0;
        file >> data;
        line.push_back(data);
        cpt++;
        if (cpt % width_ == 0){
            tmp.push_back(line);
            line.clear();
        }
    }
    imageData_ = tmp;
    file.close();
    
    //initialise the vector of integral and sumColomn. This memorisation is for making the complexity of calculing the integral lower
    vector<double> line2(width_);
    for(int i=0; i<height_; ++i){
        sumColumn_.push_back(line2);
        integral_.push_back(line2);
    }
    
    //initialise the vector of features and feature's dimension
    int count = 0;
    int n1 = (width_-8)/4+1;
    int n2 = (height_-8)/4+1;
    for(int i=0; i<n1; ++i){
        int s = 0;
        for(int j=0; j<n2; ++j){
            s += 4 * (n1-i) * (n2-j);
        }
        count += s;
    }
    vector<double> v(count, 0.0);
    feature_dimension_ = count;
    features_ = v;
}


double Image::valueOf(int x, int y){
    return imageData_[y][x];
}


double Image::sumOf(int x, int y){//memorisation
    if(sumColumn_[y][x] == 0.0){
        if (y==0){
            sumColumn_[y][x] = valueOf(x,y);
        }
        else{
            sumColumn_[y][x] = sumColumn_[y-1][x] + valueOf(x,y);
        }
        return sumColumn_[y][x];
    }
    else{
        return sumColumn_[y][x];
    } 
}


double Image::integralOf(int x, int y){//memorisation
    if(integral_[y][x] == 0.0){
        if (x==0){
            integral_[y][x] = sumOf(x,y);
        }
        else{
            integral_[y][x] = integral_[y][x-1] + sumOf(x,y);
        }
        return integral_[y][x];
    }
    else{
        return integral_[y][x];
    }
}


double Image::integral(){
    return integralOf(width_-1, height_-1);
}


double Image::type1(Rectangle* r){
    int midX = ( r->getEndX() + r->getStartX() ) / 2;      //r->getStartX works too?????
    return integralOf(r->getEndX(), r->getEndY()) + 2 * integralOf(midX, r->getStartY()) + integralOf(r->getStartX(), r->getEndY())
         - integralOf(r->getEndX(), r->getStartY()) - 2 * integralOf(midX, r->getEndY()) - integralOf(r->getStartX(), r->getStartY());
}


double Image::type2(Rectangle* r){
    int midY = ( r->getEndY() + r->getStartY() ) / 2;
    return integralOf(r->getStartX(), r->getEndY()) + 2 * integralOf(r->getEndX(), midY) + integralOf(r->getStartX(), r->getStartY())
        - integralOf(r->getEndX(), r->getEndY()) - 2 * integralOf(r->getStartX(), midY) - integralOf(r->getEndX(), r->getStartY());
}


double Image::type3(Rectangle* r){
    int oneFourth = ( 3 * r->getStartX() + r->getEndX() ) / 4;
    int threeFourth = ( r->getStartX() + 3 * r->getEndX() ) / 4;
    return 2 * integralOf(threeFourth, r->getEndY()) + 2 * integralOf(oneFourth, r->getStartY()) + integralOf(r->getEndX(), r->getStartY()) + integralOf(r->getStartX(), r->getEndY())
        - 2 * integralOf(threeFourth, r->getStartY()) - 2 * integralOf(oneFourth, r->getEndY()) - integralOf(r->getEndX(), r->getEndY()) - integralOf(r->getStartX(), r->getStartY());
}


double Image::type4(Rectangle* r){
    int midX = ( r->getEndX() + r->getStartX() ) / 2;
    int midY = ( r->getEndY() + r->getStartY() ) / 2;
    return 2 * ( integralOf(r->getEndX(), midY) + integralOf(r->getStartX(), midY) + integralOf(midX, r->getEndY()) + integralOf(midX, r->getStartY()) )
        - 4 * integralOf(midX, midY) - integralOf(r->getEndX(), r->getEndY()) - integralOf(r->getStartX(), r->getEndY()) 
        - integralOf(r->getEndX(), r->getStartY()) - integralOf(r->getStartX(), r->getStartY());
}


//use mpi here!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
void Image::setFeaturesMpi(){
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
    //                 Rectangle* r = new Rectangle(4*m, 4*n, 8+4*i+4*m, 8+4*j+4*n);
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
                Rectangle* r = new Rectangle(4*m, 4*n, 8+4*i+4*m, 8+4*j+4*n);
                //4 types of feature for every rectangle 
                f.push_back(type1(r));
                f.push_back(type2(r));
                f.push_back(type3(r));
                f.push_back(type4(r));
                delete r;
            }
        }
    }

    //every machine except root send vector of feature then root receive their vectors. use Send and Recv bloquant.
    if (rank != 0){
        MPI_Send(f, f.size(), MPI_DOUBLE, 0, 442, MPI_COMM_WORLD);
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
            vector<double> recv;
            MPI_Recv(recv, count, MPI_DOUBLE, r, 442, MPI_COMM_WORLD, &status);
            f.insert(f.end(), recv.begin(), recv.end());
        }
        this->features_ = f;
    }
    //there are 95634 rectangles, thus 4*95634=382536 features for one image
}
