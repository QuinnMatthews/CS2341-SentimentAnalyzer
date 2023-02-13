#include <vector>
#include <map>
#include <iostream>
#include <sstream>
#include <locale>
#include <codecvt>
#include <vector>
#include "DSString.h"
#include "Sentiment.h"
#include "OleanderStemmingLibrary/src/english_stem.h"

#ifndef SENTIMENTANALYZER_H
#define SENTIMENTANALYZER_H

class SentimentAnalyzer
{
    private:
        std::map<DSString, SentimentValue> trainingTokens; //<TokenString, SentimentValue>
        size_t trainingTokenCount; //Total number of tokens that have been read in
        std::map<DSString, Sentiment> tweets; //<TweetID, PredictedTweetSentiment>

        //Returns a list of tokenized words
        static std::vector<DSString> tokenizeTweet(DSString tweetstr);

        //Returns a sanitized/tokenized version of a word
        static DSString tokenizeWord(DSString tokenstr); 
    public:
        void train(std::istream&);
        void predict(std::istream&);
        void evaluatePredictions(std::istream&);
};

#endif