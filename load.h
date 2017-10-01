#ifndef LOAD_H_
#define LOAD_H_

#include <vector>
#include <string>

using namespace std;

void loadLabelsFeatures(string PATH, vector<int>& labels, vector<vector<double> >& features);
void loadCFOmega1(double* omega1);
void loadCFOmega2(double* omega2);
void loadBoostedOmega1(double* Bomega1);
void loadBoostedOmega2(double* Bomega2);
void loadSumOfAlpha(double* sumOfAlpha);
void loadImageFeatures(string s, vector<double>& features);

#endif    // END_LOAD_H_
