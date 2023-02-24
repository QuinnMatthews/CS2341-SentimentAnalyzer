#include "DSString.h"
#include "OleanderStemmingLibrary/src/english_stem.h"
#include "Sentiment.h"
#include "Token.h"
#include <codecvt>
#include <iostream>
#include <iomanip>
#include <locale>
#include <map>
#include <sstream>
#include <vector>

#ifndef SENTIMENTANALYZER_H
#define SENTIMENTANALYZER_H

class SentimentAnalyzer {
private:
    std::map<DSString, Sentiment> trainingTokens; //<TokenString, Sentiment>
    size_t trainingTokenCount; // Total number of tokens that have been read in
    std::map<DSString, SentimentValue> tweets; //<TweetID, PredictedTweetSentiment>

    // Analyzes a specific string to determine sentiment
    SentimentValue predictString(const DSString&);

    // Returns a list of tokenized words
    static std::vector<Token> tokenizeTweet(DSString tweetstr);

    // Returns a sanitized/tokenized version of a word
    static DSString sanitizeWord(DSString tokenstr);

    // Stem a word
    static DSString stemWord(DSString tokenstr);

public:
    void train(std::istream&);
    void predict(std::istream&, std::ostream&);
    void evaluatePredictions(std::istream&, std::ostream&);
};

#endif