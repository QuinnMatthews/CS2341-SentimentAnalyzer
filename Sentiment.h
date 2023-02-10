#include <vector>
#include "DSString.h"

#ifndef SENTIMENT_H
#define SENTIMENT_H

enum Sentiment {POSTIVE, NEGATIVE, NUETRAL};

class SentimentValue
{
    private:
        unsigned int posCount;
        unsigned int negCount;
    public:
        SentimentValue();
        SentimentValue(Sentiment);
        void addTrainingData(Sentiment);
        Sentiment getSentiment();
        double getConfidence(int c);
};

#endif