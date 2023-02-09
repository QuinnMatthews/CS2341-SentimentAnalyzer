#include <vector>
#include "DSString.h"

#ifndef TOKEN_H
#define TOKEN_H

enum Sentiment {POSTIVE, NEGATIVE, NUETRAL};

class Token
{
    private:
        DSString value;
        unsigned int posCount;
        unsigned int negCount;
    public:
        Token(DSString);
        void addTrainingData(Sentiment);
        Sentiment getSentiment();
        double getConfidence();
};

#endif