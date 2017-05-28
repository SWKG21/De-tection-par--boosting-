#include "CImg.h"
//#include "mpi.h" //with mpi.h errors, without, no problem
#include <iostream>
#include <vector>

using namespace std;
using namespace cimg_library;

vector<vector<double> > readUnImage(string filename){
    const char* file = filename.c_str();
    CImg<unsigned char> image(file); // seulement pour jpg ici, non raw
    int count = 0;
    vector<vector<double> > imageData;
    CImg<unsigned char>::iterator it = image.begin();
    vector<double> line;
    while(it != image.end()) {
        line.push_back(*it / 255.0);
        count++;
        ++it;
        cout << *it / 255.0 << " ";
        if (count%112 == 0){
            imageData.push_back(line);
            line.clear();
            cout << endl;
        }
    }
    cout << endl;
    cout << count << endl;
    cout << image.width() << " " << imageData[0].size() << endl;
    cout << image.height() << " " << imageData.size() << endl;

    return imageData;
}

int main() {
  
  //print out a h×w image as a vector in [0,1]^3hw.
  //I can tell you why we'll choose T to be unsigned char, 
  //though: this data type defines an integer between  00  and  255255 , 
  //i.e. a byte. Since 24-bit color depth (enough to exceed the capacity of 
  //the human eye to tell colors apart) requires  33  bytes  × 8× 8  bits  =24=24  bit storage, 
  //reading an image into a CImg<unsigned char> object will yield three bytes per pixel. 
  vector<vector<double> > imageData = readUnImage("im2.jpg");
  return 0;
}