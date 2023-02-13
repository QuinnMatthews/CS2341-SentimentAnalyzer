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
        std::vector<DSString> tokens = tokenizeTweet(tweet.c_str());

        //Inset training tokens
        for(auto token : tokens) {
            if (trainingTokens.find(token) != trainingTokens.end()) {
                //Token already exists, add training data
                trainingTokens[token].addTrainingData(sentiment);
            } else {
                //Token does not exist yet
                trainingTokens.insert(std::pair<DSString, SentimentValue>(token, SentimentValue(sentiment)));
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
    while (getline(tweetStream, line)) //For each Tweet
    {
        double sentimentVal = 0;
        //line CSV format: id,Date,Query,User,Tweet
        std::stringstream ss(line.c_str());
        DSString id, Date, Query, User, Tweet;

        //Get other Variables, likely not used but need to get to end for tweet 
        getline(ss, id, ',');
        getline(ss, Date, ',');
        getline(ss, Query, ',');
        getline(ss, User, ',');

        //Get Tweet
        getline(ss, Tweet);

        //Tokenize tweet
        std::vector<DSString> tokens = tokenizeTweet(Tweet);

        for(auto token : tokens) {
            //Check if token exists
            if (trainingTokens.find(token) != trainingTokens.end()) {
                //Token exists, add to sentiment value
                Sentiment sentiment = trainingTokens[token].getSentiment();
                double confidence = trainingTokens[token].getConfidence(trainingTokenCount);
                if (sentiment == POSTIVE) {
                    //Add postive confidence to sentiment value
                    sentimentVal += confidence;
                } else if (sentiment == NEGATIVE) {
                    //Add negative confidence to sentiment value
                    sentimentVal -= confidence;
                }
            } else {
                //TODO: (maybe) Token does not exist, should we do anything?
                //Maybe add to our training tokens with an inferred sentiment based on context within the tweet
                //That way it can be used in future Tweets
            }
        }
        //TODO: Debug Statement, should be removed
        //std::cout << Tweet << '\n' << sentimentVal << '\n' << std::endl;

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
            if(tokens.size() == 1 && Tweet.at(0) == '@') {
                //Only 1 token that appears to be tagging someone else, seems slightly more likely to be neg
                tweets.insert(std::pair<DSString, Sentiment>(id, NEGATIVE));
            } else {
                tweets.insert(std::pair<DSString, Sentiment>(id, POSTIVE));
            }
            std::cout << "Nuetral: " << id << "(" << Tweet.at(0)<< "):\n"<< Tweet << '\n' << std::endl;
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
        //TODO: Debug Stmt, can be removed from finalized project
        //std::cout << "Evaluating: " << id << " Sentiment: " << sentimentstr;

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
                //TODO: Debug Stmt, can be removed from finalized project
                //std::cout << " Correct!" << std::endl;
            } else {
                //TODO: Debug Stmt, can be removed from finalized project
                //std::cout << " Incorrect!" << std::endl;
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

std::vector<DSString> SentimentAnalyzer::tokenizeTweet(DSString tweetstr) {
    std::stringstream tweet_ss(tweetstr.c_str());
    DSString tokenstr;
    std::vector<DSString> tokens;
    while(getline(tweet_ss, tokenstr, ' '))
    {
        tokenstr = tokenizeWord(tokenstr);

        //Make sure token is not empty
        if(tokenstr.length() == 0) {
            continue;
        }

        //Add token to list
        tokens.push_back(tokenstr);
    }
    return tokens;
}


DSString SentimentAnalyzer::tokenizeWord(DSString tokenstr) {
    //To Lowercase
    tokenstr = tokenstr.toLower();

    //Reduce to just letters
    for (size_t i = 0; i < tokenstr.length(); i++)
    {   
        if (!isalpha(tokenstr[i]))
        {
            tokenstr.remove(i);
            i--;
        }
    }
    
    //Convert char[] to wchar_t[]
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
    std::wstring tokenwstr_tmp = converter.from_bytes(tokenstr.string());
    
    //Stem Word
    stemming::english_stem<> StemEnglish;
    StemEnglish(tokenwstr_tmp);

    //Convert back to char[]
    tokenstr = converter.to_bytes(tokenwstr_tmp);

    return tokenstr;
}