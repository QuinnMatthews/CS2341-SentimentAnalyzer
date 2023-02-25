#include "SentimentAnalyzer.h"

void SentimentAnalyzer::train(std::istream& trainingData) {
    std::cout << "Training..." << std::endl;
    DSString line;
    getline(trainingData, line); // Skip first line
    while (getline(trainingData, line)) {
        // line CSV format: SentimentValue,id,Date,Query,User,Tweet
        std::stringstream ss(line.c_str());
        DSString sentimentstr, id, date, query, user, tweet;

        // Get SentimentValue
        getline(ss, sentimentstr, ',');
        SentimentValue sentiment;

        if (sentimentstr == "4") {
            sentiment = POSTIVE;
        } else if (sentimentstr == "0") {
            sentiment = NEGATIVE;
        } else {
            sentiment = NUETRAL;
        }

        // Get other Variables, likely not used but need to get to end for tweet
        getline(ss, id, ',');
        getline(ss, date, ',');
        getline(ss, query, ',');
        getline(ss, user, ',');

        // Get tweet
        getline(ss, tweet);

        // Tokenize tweet
        std::vector<Token> tokens = tokenizeTweet(tweet.c_str());

        // Inset training tokens
        for (auto token : tokens) {
            SentimentValue _sentiment = sentiment;

            // If token is negated, flip sentiment
            if (token.negated) {
                _sentiment = Sentiment::negateSentiment(sentiment);
            }

            if (trainingTokens.find(token.value) != trainingTokens.end()) {
                // Token already exists, add training data
                trainingTokens[token.value].addTrainingData(_sentiment);
            } else {
                // Token does not exist yet
                trainingTokens.insert(
                    std::pair<DSString, Sentiment>(token.value, Sentiment(_sentiment))
                );
            }

            trainingTokenCount++;
        }
    }

    std::cout << "Training... Done" << std::endl;
    std::cout << "Total Number of Training Tokens: " << trainingTokenCount 
                << " (" << trainingTokens.size() << " unique values)" 
                << std::endl;
}

void SentimentAnalyzer::predict(std::istream& tweetStream, std::ostream& outputStream) {
    std::cout << "Predicting..." << std::endl;

    // Read in data
    DSString line;
    getline(tweetStream, line); // Skip first line
    while (getline(tweetStream, line)) { // For each Tweet
        double sentimentVal = 0;
        // line CSV format: id,Date,Query,User,Tweet
        std::stringstream ss(line.c_str());
        DSString id, Date, Query, User, Tweet;

        // Get other Variables, likely not used but need to get to end for tweet
        getline(ss, id, ',');
        getline(ss, Date, ',');
        getline(ss, Query, ',');
        getline(ss, User, ',');

        // Get Tweet
        getline(ss, Tweet);

        if(tweets.find(id) != tweets.end()) {
            std::cerr << "Warning duplicate Tweet: " << id << std::endl;
            continue;
        }

        // Analyze Tweet sentiment 
        SentimentValue tweetSentiment = predictString(Tweet);

        // Store/Output Sentiment 

        tweets.insert(std::pair<DSString, SentimentValue>(id, tweetSentiment));
        outputStream << tweetSentiment << ", " << id << '\n';
    }

    outputStream << std::flush;
    std::cout << "Predicting... Done" << std::endl;
    std::cout << "Total Predictions Made: " << tweets.size() << std::endl;
}

void SentimentAnalyzer::evaluatePredictions(std::istream& truthStream, std::ostream& outputStream) {
    int correctCount = 0;
    int totalCount = 0;
    // <Predicted SentimentValue, Correct SentimentValue, Id>
    std::vector<std::tuple<SentimentValue, SentimentValue, DSString>> incorrectPredictions; 
    // Read in data
    DSString line;
    getline(truthStream, line); // Skip first line
    while (getline(truthStream, line)) {

        // line CSV format: sentiment,id
        std::stringstream ss(line.c_str());

        DSString sentimentstr, id;
        getline(ss, sentimentstr, ',');
        getline(ss, id, '\r');

        // Get SentimentValue
        SentimentValue CorrectSentiment;
        if (sentimentstr == "4") {
            CorrectSentiment = POSTIVE;
        } else if (sentimentstr == "0") {
            CorrectSentiment = NEGATIVE;
        } else {
            CorrectSentiment = NUETRAL;
        }

        // Check if tweet exists
        if (tweets.find(id) != tweets.end()) {
            // Tweet exists, check if sentiment matches
            if (tweets[id] == CorrectSentiment) {
                // SentimentValue matches
                correctCount++;
            } else {
                // SentimentValue does not match
                incorrectPredictions.push_back(std::tuple(tweets[id], CorrectSentiment, id));
            }

            // Add to total count
            totalCount++;
        } else {
            // Tweet was never analyzed, this probably shouldn't happen
            std::cout << "Tweet " << id << " was not analyzed" << std::endl;
        }
    }
    double accuracy = (double) correctCount / totalCount;
    std::cout << "Accuracy: " << accuracy << std::endl;
    outputStream << std::fixed << std::setprecision(3) << accuracy << std::endl;
    for(auto& incorrectPrediction : incorrectPredictions) {
        outputStream << std::get<0>(incorrectPrediction) << ", "
            << std::get<1>(incorrectPrediction) << ", "
            << std::get<2>(incorrectPrediction)
            << '\n';
    }
    outputStream << std::flush;
}

SentimentValue SentimentAnalyzer::predictString(const DSString& str) {
        double sentimentVal = 0;
        double minDeviation = 0.15;

        // Tokenize string
        std::vector<Token> tokens = tokenizeTweet(str);

        while(minDeviation >= 0.1 && sentimentVal == 0) {
            for (auto token : tokens) {
                // Check if token exists
                if (trainingTokens.find(token.value) == trainingTokens.end()) {
                    // Token does not exist, skip
                    continue;
                }

                SentimentValue sentiment = trainingTokens[token.value].getSentiment(minDeviation);
                double confidence = trainingTokens[token.value].getConfidence(trainingTokenCount);

                // If negation, flip sentiment
                if (token.negated) {
                    sentiment = Sentiment::negateSentiment(sentiment);
                }

                if (sentiment == POSTIVE) {
                    sentimentVal += confidence;
                } else if (sentiment == NEGATIVE) {
                    sentimentVal -= confidence;
                }
            }
            minDeviation -= 0.001;
        }

        return (sentimentVal < 0) ? NEGATIVE : POSTIVE;
}

std::vector<Token> SentimentAnalyzer::tokenizeTweet(DSString tweetstr) {
    std::stringstream tweet_ss(tweetstr.c_str());
    DSString tokenstr;
    std::vector<Token> tokens;
    while (getline(tweet_ss, tokenstr, ' ')) {
        // Sanitize token
        tokenstr = sanitizeWord(tokenstr);

        // Skip empty tokens
        if (tokenstr.length() == 0) {
            continue;
        }

        tokens.push_back(Token { stemWord(tokenstr), false });

        // Check for negations
        for (size_t i = 0; i < tokens.size(); i++) {
            const auto& val = tokens[i].value;

            if (val == "not" || val == "no" || (val.length() > 3 && val.substring(val.length()-3, 3) == "n't")) {
                if (i + 1 < tokens.size()) {
                    auto& token = tokens[i + 1];
                    token.negated = true;
                }
            }
        }
    }

    return tokens;
}

DSString SentimentAnalyzer::sanitizeWord(DSString tokenstr) {
    if (tokenstr.length() == 0) {
        return "";
    }

    // To Lowercase
    tokenstr = tokenstr.toLower();

    // Reduce to just letters and apostrophes
    for (size_t i = 0; i < tokenstr.length(); i++) {
        if (!isalpha(tokenstr[i]) && tokenstr[i] != '\'') {
            tokenstr.remove(i);
            i--;
        }
    }

    // Reduce 3+ duplicate letters to single letter
    for (size_t i = 1; i < tokenstr.length(); i++) {
        if (i > 1 && tokenstr.at(i) == tokenstr.at(i - 1) && tokenstr.at(i) == tokenstr.at(i - 2)) {
            tokenstr.remove(i);
            tokenstr.remove(i - 1);
            i -= 2;
        }
    }

    return tokenstr;
}

DSString SentimentAnalyzer::stemWord(DSString tokenstr) {
    // Convert char[] to wchar_t[]
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
    std::wstring tokenwstr_tmp = converter.from_bytes(tokenstr.string());

    // Stem Word
    stemming::english_stem<> StemEnglish;
    StemEnglish(tokenwstr_tmp);

    // Convert back to char[]
    tokenstr = converter.to_bytes(tokenwstr_tmp);

    return tokenstr;
}
