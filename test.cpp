#include "image.h"
#include <iostream>
#include <vector> 
#include <string>  
#include <dirent.h>
#include <opencv2/core/core.hpp>

#include <fstream>   
//#include <windows.h>  
////#include <gdiplus.h>  
#pragma comment(lib, "gdiplus.lib") 

using namespace std;
//using namespace Gdiplus; 

int main(int argc, char* argv[]){
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


    //test get all files of a fixed format in a directory
    struct dirent *ptr;      
    DIR *dir;  
    string PATH = "/home/wangs/workspace/INF442 WangSUN/Projet/app";
    string PATHpos = PATH+"/pos";
    string PATHneg = PATH+"/neg";
    vector<string> posfiles;
    vector<string> negfiles;
    
    //read all files in app/pos
    dir=opendir(PATHpos.c_str());   
    cout << "File list: pos/"<< endl;  
    while((ptr=readdir(dir))!=NULL)  
    {   
        //do not consider '.' and '..' the two directories  
        if(ptr->d_name[0] == '.')  
            continue;
        //cout << ptr->d_name << endl;  
        posfiles.push_back(ptr->d_name);  
    }  
      
    for (int i = 0; i < posfiles.size(); ++i)  
    {  
        cout << posfiles[i] << " ";  
    }
    cout << endl;

    //read all files in app/neg
    dir=opendir(PATHneg.c_str());     
    cout << "File list: neg/"<< endl;  
    while((ptr=readdir(dir))!=NULL)  
    {   
        //do not consider '.' and '..' the two directories  
        if(ptr->d_name[0] == '.')  
            continue;
        //cout << ptr->d_name << endl;  
        negfiles.push_back(ptr->d_name);  
    }  
      
    for (int i = 0; i < negfiles.size(); ++i)  
    {  
        cout << negfiles[i] << " ";  
    }
    cout << endl;

    closedir(dir);

    // vector<double> v(10, 9);
    // for(int i=0; i<10; i++){
    //     cout << v[i] << endl;
    // }

    // GdiplusStartupInput gdiplusstartupinput;  
    // ULONG_PTR gdiplustoken;  
    // GdiplusStartup(&gdiplustoken, &gdiplusstartupinput, NULL);  
    // wstring infilename(L"1.jpg");  
    // string outfilename("color.txt");  
    // //读图片  
    // Bitmap* bmp = new Bitmap(infilename.c_str());  
    // UINT height = bmp->GetHeight();  
    // UINT width = bmp->GetWidth();  
    // cout << "width " << width << ", height " << height << endl;  
    // Color color;  
    // ofstream fout(outfilename.c_str());  
    // fout<<"structure"<<endl;  
  
    // for (int y = 0; y < height; y++)
    //     for (int x = 0; x < width; x++)  {  
    //         bmp->GetPixel(x, y, &color);  
    //         fout << x << ";" << y << ";"  
    //             << (int)color.GetRed() << ";"  
    //             << (int)color.GetGreen() << ";"  
    //             << (int)color.GetBlue() << endl;  
    //     }  
  
    //     fout.close();  
    //     delete bmp;  
    //     GdiplusShutdown(gdiplustoken);

    return 0;
}


// #include <iostream>
// #include <opencv2/core/core.hpp>
// #include <opencv2/highgui/highgui.hpp>
// using namespace std;
// //#define key 0xa9
// int key[8]={0,1,0,0,0,0,1,0};
// using namespace cv;
// int main(int argc, char* argv[])
// {
//     IplImage *img = cvLoadImage("00.jpg",1);
// 	//cvSaveImage("test111.jpg",img);
//     CvScalar s;
// 	cvNamedWindow("Image",1);
// 	cvShowImage("Image",img);
// 	int flag=1;
//     for(int ii = 0;ii < img->height;ii++)
// 	{
//         for(int jj = 0;jj < img->width;jj++)
// 		{
// 		   s = cvGet2D(img,ii,jj); // get the (i,j) pixel value
// 		   int col[3][8];
// 		   memset(col,0,sizeof(col));
// 		   for(int i=0;i<3;i++)
// 		   {
// 			   int num=0;
// 			   if(flag)
// 				   cout<<"像素值：  "<<(int)s.val[i]<<endl;