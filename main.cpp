#include "DSString.h"
#include "SentimentAnalyzer.h"
#include <fstream>
#include <iostream>

using namespace std;

ifstream openFile(DSString filename);
ofstream openOutputFile(DSString filename);

int main(int argc, char** argv) {
    DSString trainingDataset, testDataset, testDatasetTruth, predictionOutput, incorrectPredictions;
    if (argc == 1) {
        trainingDataset = "data/train_dataset_20k.csv";
        testDataset = "data/test_dataset_10k.csv";
        testDatasetTruth = "data/test_dataset_sentiment_10k.csv";
        predictionOutput = "results.csv";
        incorrectPredictions = "accuracy.txt";
    } else if (argc != 6) {
        cout << "Usage: ./sentiment <training dataset> <test dataset> <test dataset truth> <prediction output> <incorrect prediction output>" << endl;
        return 0;
    } else {
        trainingDataset = argv[1];
        testDataset = argv[2];
        testDatasetTruth = argv[3];
        predictionOutput = argv[4];
        incorrectPredictions = argv[5];
    }

    SentimentAnalyzer analyzer = SentimentAnalyzer();

    //Train
    ifstream trainDataStream = openFile(trainingDataset);
    analyzer.train(trainDataStream);
    trainDataStream.close();

    //Predict
    ifstream testDataStream = openFile(testDataset);
    ofstream resultStream = openOutputFile(predictionOutput);
    analyzer.predict(testDataStream, resultStream);
    testDataStream.close();

    //Evaluate predictions
    ifstream truthDataStream = openFile(testDatasetTruth);
    ofstream accuracySteam = openOutputFile(incorrectPredictions);
    analyzer.evaluatePredictions(truthDataStream, accuracySteam);
    truthDataStream.close();

    return 0;
}

ifstream openFile(DSString filename) {
    ifstream file;
    file.open(filename.c_str());
    if (!file.is_open()) {
        throw runtime_error("Unable to open File");
    }
    return file;
}

ofstream openOutputFile(DSString filename) {
    ofstream file;
    file.open(filename.c_str());
    if(!file.is_open()) {
        throw runtime_error("Unable to open File");
    }
    return file;
}
