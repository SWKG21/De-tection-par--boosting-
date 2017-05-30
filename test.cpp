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

int main(int argc, char** argv){
    // vector<vector<int> > tmp;
    // int size = 12*15;
    // vector<int> line;
    // int count = 0;
    // int i = 0;
    // while(i<size){
    //     line.push_back(i);
    //     count++;
    //     i++;
    //     if (count % 12 ==0){
    //         tmp.push_back(line);
    //         line.clear();
    //     }
    // }

    // for(int i=0; i<15; ++i){
    //     for(int j=0; j<12; ++j){
    //         cout << tmp[i][j] << " ";
    //     }
    //     cout << endl;
    // }

    // int count = 0;
    // int n1 = (112-8)/4+1;
    // int n2 = (92-8)/4+1;
    // for(int i=0; i<n1; ++i){
    //     int s = 0;
    //     for(int j=0; j<n2; ++j){
    //         s += 4 * (n1-i) * (n2-j);
    //     }
    //     count += s;
    //     cout << s << endl;
    // }
    // cout << "variation : " << count/4 << endl;
    // cout << "nombre de carac : " << count << endl;


    // //test the function insert of vector
    // vector<double> v1;
    // v1.push_back(1);
    // v1.push_back(2);
    // v1.push_back(3);
    // vector<double> v2;
    // v2.push_back(4);
    // v2.push_back(5);
    // v2.push_back(6);
    // v1.insert(v1.end(), v2.begin(), v2.end());
    // cout << v1.size() << endl;
    // for(int i=0; i<v1.size(); ++i){
    //     cout << v1[i] << " ";
    // }
    // cout << endl;


    // //test MPI read an image
    // MPI_Init(&argc, &argv);
    // Image* img = new Image("im2.jpg");
    // vector<double> ch = img->characters();
    // vector<double>::iterator it = ch.begin();
    // int count = 0;
    // while(it != ch.end()){
    //     cout << *it << " ";
    //     ++it;
    //     if(count % 4 == 0) cout << endl;
    // }
    // MPI_Finalize();


    // //test get all files of a fixed format in a directory
    // struct dirent *ptr;      
    // DIR *dir;  
    // string PATH = "/home/wangs/workspace/INF442 WangSUN/Projet/app";
    // string PATHpos = PATH+"/pos";
    // string PATHneg = PATH+"/neg";
    // vector<string> posfiles;
    // vector<string> negfiles;
    
    // //read all files in app/pos
    // dir=opendir(PATHpos.c_str());   
    // cout << "File list: pos/"<< endl;  
    // while((ptr=readdir(dir))!=NULL)  
    // {   
    //     //do not consider '.' and '..' the two directories  
    //     if(ptr->d_name[0] == '.')  
    //         continue;
    //     //cout << ptr->d_name << endl;  
    //     posfiles.push_back(ptr->d_name);  
    // }  
      
    // for (int i = 0; i < posfiles.size(); ++i)  
    // {  
    //     cout << posfiles[i] << " ";  
    // }
    // cout << endl;

    // //read all files in app/neg
    // dir=opendir(PATHneg.c_str());     
    // cout << "File list: neg/"<< endl;  
    // while((ptr=readdir(dir))!=NULL)  
    // {   
    //     //do not consider '.' and '..' the two directories  
    //     if(ptr->d_name[0] == '.')  
    //         continue;
    //     //cout << ptr->d_name << endl;  
    //     negfiles.push_back(ptr->d_name);  
    // }  
      
    // for (int i = 0; i < negfiles.size(); ++i)  
    // {  
    //     cout << negfiles[i] << " ";  
    // }
    // cout << endl;

    // closedir(dir);

    // vector<double> v(10, 9);
    // for(int i=0; i<10; i++){
    //     cout << v[i] << endl;
    // }
    
    MPI_Init(&argc, &argv);
    int size, rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    srand(rank);

    string PATH1 = "app";
    const char* path1 = PATH1.c_str();
    //Instances* train_ins = new Instances(path1);
    //train_ins->print_information();
    //Classifier* cl = new Classifier(train_ins);
    vector<Image*> ins;
    for(int i=0; i<1; ++i){
        stringstream ss;
        ss << i;
        string name = PATH1+"/pos/im"+ss.str()+".txt";
        Image* img = new Image(name);
        img->setLabel(1);
        ins.push_back(img);
    }
    for(int i=0; i<2; ++i){
        stringstream ss;
        ss << i;
        string name = PATH1+"/neg/im"+ss.str()+".txt";
        Image* img = new Image(name);
        img->setLabel(-1);
        ins.push_back(img);
    }
    MPI_Barrier(MPI_COMM_WORLD);
    cout << "all images created" << endl;

    for(int i=0; i<ins.size(); ++i){
        ins[i]->initialize();
    }
    Instances* train_inst = new Instances(ins);
    Classifier* cl = new Classifier(train_inst);
    
    MPI_Finalize();
    
    //string PATH2 = "dev";
    //const char* path2 = PATH2.c_str();
    //Instances* valid_ins = new Instances(path2);
    //Boosting* boo = new Boosting(valid_ins, cl);

    // string name = "app/pos/im0.txt";
    // const char* filename = name.c_str();
    // ifstream file;
    // file.open(filename, ios::out | ios::in);
    // if (!file.is_open()) {
    //     cout << "Error: could not open matrix file '" << filename << "'" << endl;
    // }
    // int n = 0;
    // file >> n;
    // cout << n;
    return 0;
}
