#include "Sentiment.h"

SentimentValue::SentimentValue() = default;

SentimentValue::SentimentValue(Sentiment s) {
    if (s == POSTIVE) {
        posCount = 1;
        negCount = 0;
    } else if (s == NEGATIVE) {
        posCount = 0;
        negCount = 1;
    } else {
        posCount = 0;
        negCount = 0;
    }
}

void SentimentValue::addTrainingData(Sentiment s) {
    if (s == POSTIVE) {
        posCount++;
    } else if (s == NEGATIVE) {
        negCount++;
    }
}

Sentiment SentimentValue::getSentiment() {
    //Check if postive count and negative count are close
    const double minDeviation = 0.02; //Required deviation to not be nuetral

    //Divide by 0 error shouldn't be possible since SentimentValue will always be initialized with a sentiment
    const double posPercent = (double) posCount / (posCount + negCount);
    const double negPercent = (double) negCount / (posCount + negCount); 

    if (posPercent > 0.5 + minDeviation) {
        return POSTIVE;
    } else if (negPercent > 0.5 + minDeviation) {
        return NEGATIVE;
    } else {
        return NUETRAL;
    }
} 

double SentimentValue::getConfidence(int totalTokens) {
    Sentiment sentiment = getSentiment();

    if(sentiment == POSTIVE) {
        return ((double) (posCount-negCount) / totalTokens) * 1000;
    } else if (sentiment == NEGATIVE) {
        return ((double) (negCount-posCount) / totalTokens) * 1000;
    } else {
        return 0;
    }
}

