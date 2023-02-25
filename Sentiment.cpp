#include "Sentiment.h"

Sentiment::Sentiment() = default;

Sentiment::Sentiment(SentimentValue s) {
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

void Sentiment::addTrainingData(SentimentValue s) {
    if (s == POSTIVE) {
        posCount++;
    } else if (s == NEGATIVE) {
        negCount++;
    }
}

SentimentValue Sentiment::getSentiment(const double& minDeviation) {
    if (posCount == 0 && negCount == 0) {
        return NUETRAL;
    }

    // Check if postive count and negative count are close
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

double Sentiment::getConfidence(int totalTokens) {
    int diff = abs(posCount - negCount);
    return (double) diff/totalTokens * 1000;
}

SentimentValue Sentiment::negateSentiment(SentimentValue sentiment) {
    if(sentiment == POSTIVE) {
        return NEGATIVE;
    } else if(sentiment == NEGATIVE) {
        return POSTIVE;
    } else {
        return NUETRAL;
    }
}