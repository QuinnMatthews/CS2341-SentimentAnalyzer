#include <vector>
#include <map>
#include <iostream>
#include <sstream>
#include "DSString.h"
#include "Sentiment.h"

#ifndef SENTIMENTANALYZER_H
#define SENTIMENTANALYZER_H

class SentimentAnalyzer
{
    private:
        std::map<DSString, SentimentValue> trainingTokens; //<TokenString, SentimentValue>
        size_t trainingTokenCount; //Total number of tokens that have been read in
        std::map<DSString, Sentiment> tweets; //<TweetID, PredictedTweetSentiment>
    public:
        void train(std::istream&);
        void predict(std::istream&);
        void evaluatePredictions(std::istream&);
};

#endif