#include "DSString.h"
#include <vector>

#ifndef SENTIMENT_H
#define SENTIMENT_H

enum SentimentValue { POSTIVE = 4, NEGATIVE = 0, NUETRAL = 2 };

class Sentiment {
private:
    unsigned int posCount;
    unsigned int negCount;

public:
    Sentiment();
    Sentiment(SentimentValue);
    void addTrainingData(SentimentValue);
    SentimentValue getSentiment(const double& minDeviation = 0.15);
    double getConfidence(int c);

    static SentimentValue negateSentiment(SentimentValue);
};

#endif