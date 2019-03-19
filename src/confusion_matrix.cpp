#include "confusion_matrix.h"

#include <iostream>

using namespace std;

ConfusionMatrix::ConfusionMatrix() {
    for (int i = 0; i < 2; ++i) {
        for (int j = 0; j < 2; ++j) {
            confusion_matrix_[i][j] = 0;
        }
    }
}

ConfusionMatrix::~ConfusionMatrix() {}

void ConfusionMatrix::AddPrediction(int label, int predicted_label) {
    if (label == -1 && predicted_label == -1) confusion_matrix_[0][0]++;
    if (label == -1 && predicted_label == 1) confusion_matrix_[0][1]++;
    if (label == 1 && predicted_label == -1) confusion_matrix_[1][0]++;
    if (label == 1 && predicted_label == 1) confusion_matrix_[1][1]++;
}

void ConfusionMatrix::PrintEvaluation() const{
    // Prints the confusion matrix
    cout <<"\t\tPredicted\n";
    cout <<"\t\tVisage\tNon-visage\n";
    cout <<"Actual\tVisage\t"
        <<GetTN() <<"\t"
        <<GetFP() <<endl;
    cout <<"\tNon-visage\t"
        <<GetFN() <<"\t"
        <<GetTP() <<endl <<endl;
    // Prints the estimators
    cout <<"Error rate\t\t"
        <<error_rate() <<endl;
    cout <<"False alarm rate\t"
        <<false_alarm_rate() <<endl;
    cout <<"Detection rate\t\t"
        <<detection_rate() <<endl;
    cout <<"F-score\t\t\t"
        <<f_score() <<endl;
    cout <<"Precision\t\t"
        <<precision() <<endl;
}

void ConfusionMatrix::PrintRocEvaluation() const{
    cout <<false_alarm_rate() <<"\t";
    cout <<detection_rate() <<endl;
}

int ConfusionMatrix::GetTP() const {
    return confusion_matrix_[1][1];
}

int ConfusionMatrix::GetTN() const {
    return confusion_matrix_[0][0];
}

int ConfusionMatrix::GetFP() const {
    return confusion_matrix_[0][1];
}

int ConfusionMatrix::GetFN() const {
    return confusion_matrix_[1][0];
}

double ConfusionMatrix::f_score() const {
    double p = precision();
    double r = detection_rate();
    return 2 * p * r / (p + r);
}

double ConfusionMatrix::precision() const {
    return (double)GetTP() / (double)(GetTP() + GetFP());
}

double ConfusionMatrix::error_rate() const {
    return (double)(GetFP() + GetFN()) / 
        (double)(GetFP() + GetFN() + GetTP() + GetTN());
}

double ConfusionMatrix::detection_rate() const {
    return (double)GetTP() / (double)(GetTP() + GetFN());
}

double ConfusionMatrix::false_alarm_rate() const {
    return (double)GetFP() / (double)(GetFP() + GetTN());
}
