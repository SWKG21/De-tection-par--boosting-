#include "CImg.h"
#include <iostream>
#include <vector>
#include <fstream>
#include <dirent.h>
#include <string>
#include <sstream>

using namespace std;
using namespace cimg_library;

void transformUnImage(string filename){ //CImg can NOT read images which are not in this directory. So we can not transform all images in different files with one time
    string namejpg = filename+".jpg";
    const char* forJpg = namejpg.c_str();
    CImg<unsigned char> image(forJpg); // seulement pour jpg ici, non raw
    
    string nametxt = filename+".txt";
    const char* forTxt = nametxt.c_str();
    ofstream outfile(forTxt);
    if (outfile.is_open()){
        outfile << image.width() << endl;
        outfile << image.height() << endl;
        CImg<unsigned char>::iterator it = image.begin();
        int i = *it;
        outfile << i;
        ++it;
        while(it != image.end()) {
            i = *it;
            outfile << endl << i;
            ++it;
        }
        outfile.close();
    }
}


void transformImages(int n){ //path until the directory which contains two sub-directories: pos/ and neg/
    for(int i=0; i<n+1; ++i){
        stringstream ss;
        ss << i;
        string imgname = "im"+ss.str();
        transformUnImage(imgname);
    }
}

void readUnTxt(string filename){
    const char* file = filename.c_str();
    char buffer[112*92];  
    ifstream in(file);  
    if (!in.is_open()){ cout << "Error opening file"; exit (1); }
    while (!in.eof() ){
        in.getline(buffer,100);
        cout << buffer << endl;
    }  
}


vector<vector<double> > readUnImage(string filename){
    const char* file = filename.c_str();
    CImg<unsigned char> image(file); // seulement pour jpg ici, non raw
    int count = 0;
    vector<vector<double> > imageData;
    CImg<unsigned char>::iterator it = image.begin();
    vector<double> line;
    while(it != image.end()) {
        line.push_back(*it / 255.0);
        cout << *it / 255.0 << " ";
        count++;
        if (count%112 == 0){
            imageData.push_back(line);
            line.clear();
            cout << endl;
        }
        ++it;
    }
    cout << endl;
    cout << count << endl;
    cout << image.width() << " " << imageData[0].size() << " " << imageData[91].size()<< endl;
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
  
  //vector<vector<double> > imageData = readUnImage("im2.jpg");
  //transformUnImage("im2.jpg");
  transformImages(817);
  transformImages(4414);
//   transformImages("app/pos/", 817);
//   transformImages("app/neg/", 4414);
//   transformImages("dev/pos/", 817);
//   transformImages("dev/neg/", 4414);
//   transformImages("test/pos/", 817);
//   transformImages("test/neg/", 4414);
  //readUnTxt("im2.jpg.txt");

//   ofstream out("out.txt");
//   if (out.is_open()){
//       out << "This is a line.\n";
//       out << "This is another line.\n";
//       out.close();
//     }

  return 0;
}
