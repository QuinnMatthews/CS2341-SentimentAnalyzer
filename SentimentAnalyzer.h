#include <vector>
#include <iostream>
#include "Token.h"
#include "Tweet.h"

#ifndef SENTIMENTANALYZER_H
#define SENTIMENTANALYZER_H

class SentimentAnalyzer
{
    private:
        std::vector<Token> trainingTokens;
        std::vector<Tweet> tweets;
    public:
        void train(std::istream&);
        void predict(std::istream&);
        void evaluatePredictions(std::istream&);
};

#endif