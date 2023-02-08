#include <vector>
#include "Token.h"
#include "DSString.h"

#ifndef TWEET_H
#define TWEET_H

class Tweet
{
    private:
        std::vector<Token> tokens;
        size_t id;
    public:
        Tweet(size_t, DSString);
        Sentiment getSentiment();
};

#endif