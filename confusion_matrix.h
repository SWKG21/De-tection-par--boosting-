#ifndef CONFUSION_MATRIX_H_
#define CONFUSION_MATRIX_H_

class ConfusionMatrix {

    public:
        ConfusionMatrix();
        ~ConfusionMatrix();

        void AddPrediction(int label, int predicted_label);

        void PrintEvaluation() const;
        void PrintRocEvaluation() const;

        int GetTP() const;
        int GetTN() const;
        int GetFP() const;
        int GetFN() const;

        double f_score() const;
        double precision() const;
        double error_rate() const;
        double detection_rate() const;
        double false_alarm_rate() const;

    private:
        // First dimension:     actual label
        // Second dimension:    predicted label
        int confusion_matrix_[2][2];
};

#endif    // END_CONFUSION_MATRIX_H_
