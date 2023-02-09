#include <iostream>
#include <fstream>
#include "SentimentAnalyzer.h"
#include "DSString.h"

using namespace std;

int main(int argc, char** argv)
{  
    if(argc != 3) {
        throw invalid_argument("Expected 3 arguments, got: " + argc);
    }

    const DSString trainingDataset = argv[1];
    const DSString testDataset = argv[2];
    const DSString testDatasetTruth = argv[3];

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
        throw runtime_error("Unable to open File");
    }

    analyzer.train(trainDataStream);

    trainDataStream.close();

    /*
    * ---------------------
    * -------Testing-------
    * ---------------------
    */

    //Open test dataset
    ifstream testDataStream;
	trainDataStream.open(testDataset.c_str());
	
	//Verify file is open
	if(!testDataStream.is_open()) {
		throw runtime_error("Unable to open File");
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
		throw runtime_error("Unable to open File");
	}

    analyzer.predict(truthDataStream);
    
    truthDataStream.close();

    return 0;
}