#include "SentimentAnalyzer.h"

void SentimentAnalyzer::train(std::istream& trainingData)
{
    std::cout << "Training..." << std::endl;
    DSString line;
    getline(trainingData, line); //Skip first line
    while (getline(trainingData, line))
    {
        //line CSV format: Sentiment,id,Date,Query,User,Tweet
        std::stringstream ss(line.c_str());
        DSString sentimentstr, id, date, query, user, tweet;

        //Get Sentiment 
        getline(ss, sentimentstr, ',');
        Sentiment sentiment; 
        if(sentimentstr == "4") {
            sentiment = POSTIVE;
        } else if (sentimentstr == "0") {
            sentiment = NEGATIVE;
        } else {
            sentiment = NUETRAL;
        }

        //Get other Variables, likely not used but need to get to end for tweet 
        getline(ss, id, ',');
        getline(ss, date, ',');
        getline(ss, query, ',');
        getline(ss, user, ',');

        //Get tweet
        getline(ss, tweet);

        //Tokenize tweet
        std::stringstream tweet_ss(tweet.c_str());
        DSString tokenstr;
        while(getline(tweet_ss, tokenstr, ' '))
        {
            //Remove punctuation
            for (size_t i = 0; i < tokenstr.length(); i++)
            {
                if (ispunct(tokenstr[i]))
                {
                    tokenstr.remove(i);
                    i--;
                }
            }

            //Check if token already exists
            if (trainingTokens.find(tokenstr) != trainingTokens.end()) {
                //Token already exists, add training data
                trainingTokens[tokenstr].addTrainingData(sentiment);
            } else {
                //Token does not exist yet
                trainingTokens.insert(std::pair<DSString, SentimentValue>(tokenstr, SentimentValue(sentiment)));
            }
            trainingTokenCount++;
        }
    }
    std::cout << "Training... Done" << std::endl;
    std::cout << "Total Number of Training Tokens: " << trainingTokenCount 
    << " (" << trainingTokens.size() << " unique values)" << std::endl;
}

void SentimentAnalyzer::predict(std::istream& tweetStream) {
    std::cout << "Predicting..." << std::endl;
    //Read in data
    DSString line;
    getline(tweetStream, line); //Skip first line
    while (getline(tweetStream, line))
    {
        double sentimentVal = 0;
        //line CSV format: id,Date,Query,User,Tweet
        std::stringstream ss(line.c_str());
        DSString id, Date, Query, User, Tweet;

        getline(ss, id, ',');
        getline(ss, Date, ',');
        getline(ss, Query, ',');
        getline(ss, User, ',');
        getline(ss, Tweet);


        //Tokenize tweet
        std::stringstream tweet_ss(Tweet.c_str());
        DSString tokenstr;
        while(getline(tweet_ss, tokenstr, ' '))
        {
            //Remove punctuation
            for (size_t i = 0; i < tokenstr.length(); i++)
            {
                if (ispunct(tokenstr[i]))
                {
                    tokenstr.remove(i);
                    i--;
                }
            }

            //Check if token exists
            if (trainingTokens.find(tokenstr) != trainingTokens.end()) {
                //Token exists, add to sentiment value
                Sentiment sentiment = trainingTokens[tokenstr].getSentiment();
                double confidence = trainingTokens[tokenstr].getConfidence(trainingTokenCount);
                if (sentiment == POSTIVE) {
                    //Add postive confidence to sentiment value
                    sentimentVal += confidence;
                } else if (sentiment == NEGATIVE) {
                    //Add negative confidence to sentiment value
                    sentimentVal -= confidence;
                }
            } else {
                //TODO: (maybe) Token does not exist, should we do anything?
            }
            std::cout << tokenstr << " sentiment Value: " << sentimentVal << std::endl;
            //Record predicted sentiment
            if (sentimentVal > 0) {
                //Positive
                tweets.insert(std::pair<DSString, Sentiment>(id, POSTIVE));
            } else if (sentimentVal < 0) {
                //Negative
                tweets.insert(std::pair<DSString, Sentiment>(id, NEGATIVE));
            } else {
                //Nuetral
                //We should probably just guess? Seems to be more likely to be Positive
                tweets.insert(std::pair<DSString, Sentiment>(id, POSTIVE));
            }
        }
    }
    std::cout << "Predicting... Done" << std::endl;
    std::cout << "Total Predictions Made: " << tweets.size() << std::endl;
}

void SentimentAnalyzer::evaluatePredictions(std::istream& truthStream) {
    int correctCount = 0;
    int totalCount = 0;
    //Read in data
    DSString line;
    getline(truthStream, line); //Skip first line
    while (getline(truthStream, line))
    {
        //line CSV format: sentiment,id
        std::stringstream ss(line.c_str());

        DSString sentimentstr, id;
        getline(ss, sentimentstr, ',');
        getline(ss, id, '\r');
        std::cout << "Evaluating: " << id << " Sentiment: " << sentimentstr;
        //Get Sentiment 
        Sentiment sentiment; 
        if(sentimentstr == "4") {
            sentiment = POSTIVE;
        } else if (sentimentstr == "0") {
            sentiment = NEGATIVE;
        } else {
            sentiment = NUETRAL;
        }

        //Check if tweet exists
        if (tweets.find(id) != tweets.end()) {
            //Tweet exists, check if sentiment matches
            if (tweets[id] == sentiment) {
                //Sentiment matches, add to correct count
                correctCount++;
                std::cout << " Correct!" << std::endl;
            } else {
                std::cout << " Incorrect!" << std::endl;
            }
            //Add to total count
            totalCount++;
        } else {
            //Tweet was never analyzed, this probably shouldn't happen
            std::cout << "Tweet " << id << " was not analyzed" << std::endl;
        }
    }
    std::cout << "Accuracy: " << (double)correctCount / (double)totalCount << std::endl;
}