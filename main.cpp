#include <iostream>
#include <fstream>
#include "SentimentAnalyzer.h"
#include "DSString.h"

using namespace std;

int main(int argc, char** argv)
{  
    DSString trainingDataset, testDataset, testDatasetTruth;
    if(argc == 1) {
        trainingDataset = "data/train_dataset_20k.csv";
        testDataset = "data/test_dataset_10k.csv";
        testDatasetTruth = "data/test_dataset_sentiment_10k.csv";
    }
    else if(argc != 4) {
        cout << "Usage: ./sentiment <training dataset> <test dataset> <test dataset truth>" << endl;
        return 0;
    } else {
        trainingDataset = argv[1];
        testDataset = argv[2];
        testDatasetTruth = argv[3];
    }

    SentimentAnalyzer analyzer = SentimentAnalyzer();

    /*
    * ---------------------
    * ------Training-------
    * ---------------------
    */

    //Open training dataset
    ifstream trainDataStream;
    trainDataStream.open(trainingDataset.c_str());

    //Verify file is open
    if(!trainDataStream.is_open()) {
        throw runtime_error("Unable to open File 1");
    } else {
        cout << "File 1 Opened" << endl;
    }

    analyzer.train(trainDataStream);
    cout << "Training Complete" << endl;

    trainDataStream.close();

    /*
    * ---------------------
    * -------Testing-------
    * ---------------------
    */

    //Open test dataset
    ifstream testDataStream;
	testDataStream.open(testDataset.c_str());
	
	//Verify file is open
	if(!testDataStream.is_open()) {
		throw runtime_error("Unable to open File 2");
	} else {
        cout << "File 2 Opened" << endl;
    }

    analyzer.predict(testDataStream);
    
    testDataStream.close();

    /*
    * ---------------------
    * ------Analyzing------
    * ---------------------
    */

    //Open ground truth dataset
    ifstream truthDataStream;
	truthDataStream.open(testDatasetTruth.c_str());
	
	//Verify file is open
	if(!truthDataStream.is_open()) {
		throw runtime_error("Unable to open File 3");
	}

    analyzer.evaluatePredictions(truthDataStream);
    
    truthDataStream.close();

    return 0;
}