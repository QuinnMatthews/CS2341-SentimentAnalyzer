#include "DSString.h"
#include "OleanderStemmingLibrary/src/english_stem.h"
#include "Sentiment.h"
#include "Token.h"
#include <codecvt>
#include <iostream>
#include <locale>
#include <map>
#include <sstream>
#include <vector>

#ifndef SENTIMENTANALYZER_H
#define SENTIMENTANALYZER_H

class SentimentAnalyzer {
private:
    std::map<DSString, SentimentValue> trainingTokens; //<TokenString, SentimentValue>
    size_t trainingTokenCount; //Total number of tokens that have been read in
    std::map<DSString, Sentiment> tweets; //<TweetID, PredictedTweetSentiment>

    std::map<DSString, DSString> contractions; //<Contraction, Meaning>

    //Returns a list of tokenized words
    static std::vector<Token> tokenizeTweet(DSString tweetstr);

    //Returns a sanitized/tokenized version of a word
    static DSString sanitizeWord(DSString tokenstr);

    //Check if a token is a contraction and returns a vector of the expanded tokens
    static std::vector<DSString> expandContraction(DSString tokenstr);

    //Stem a word
    static DSString stemWord(DSString tokenstr);

public:
    void train(std::istream&);
    void predict(std::istream&);
    void evaluatePredictions(std::istream&);
};

#endif