#include "DSString.h"

/* 
 * Implement the functions defined in DSString.h. You may add more functions as needed
 * for the project. 
 *
 * Note that c-strings use `\0` as a terminator symbol
 * but your class should store its length in a member variable. Do not use c-string functions 
 * from <string.h> or <cstring> other than for conversion from a c-sting.
 */  

    /**
     * Make sure you implement the rule-of-three and use proper memory management.
     * To help you get started, you can implement the following:
     **/
    DSString::DSString() {
        len = 0;
        data = new char[1];
        data[len] = '\0';
    }

    // constructor that converts a cstring
    DSString::DSString(const char* cstr) {
        //Determine length of cstring
        len = 0;
        for(size_t i = 0; cstr[i] != '\0'; i++){
            len++;
        }

        //Create new char array the correct size
        data = new char[len + 1];

        //Copy cstring into char array
        for(size_t i = 0; i < len; i++){
            data[i] = cstr[i];
        }

        //Add null terminator
        data[len] = '\0';
    }
    // you can also provide  DSString(const string &); for std::string
    DSString::DSString(const std::string& str) {
        len = str.length();
        data = new char[len + 1];

        //Copy each character from string into char array
        for(size_t i = 0; i < len; i++){
            data[i] = str.at(i);
        }

        //Add null terminator
        data[len] = '\0';
    }

    // rule-of-three
    
    // copy constructor
    DSString::DSString(const DSString& cpyFrm) {
        len = cpyFrm.len;
        data = new char[len + 1];

        //Copy each character
        for(size_t i = 0; i < len; i++){
            data[i] = cpyFrm.data[i];
        }

        //Add null terminator
        data[len] = '\0';
    }

    // destructor
    DSString::~DSString() {
        //Clean up char array
        delete[] data;
    }   

    // assignment operator                       
    DSString& DSString::operator=(const DSString& cpyFrm) {
        //Clean up old data
        delete data;

        len = cpyFrm.len;
        data = new char[len + 1];

        //Copy each character
        for(size_t i = 0; i < len; i++){
            data[i] = cpyFrm.data[i];
        }

        //Add null terminator
        data[len] = '\0';
        
        return *this;
    } 
    //TODO: you can also implement the move versions for the big 5 (C+11)

    /**
     * @brief returns the length of the string
     * 
     * @return int containing the length of the string
     */
    size_t DSString::length() const {
        return len;
    } 

    // returns a reference to the character at the given index
    char& DSString::operator[](size_t index) {
        return data[index];
    } 

    /**
     * Overloaded operator+ which appends the string in the argument to this string
     */
    DSString DSString::operator+(const DSString& rhs) const{
        size_t combinedLen = len + rhs.length() + 1;
        char* newData = new char[combinedLen];
        newData[len] = '\0';

        for (size_t i = 0; i < len; i++) {
            newData[i] = data[i];
        }
        for (size_t i = 0; i < rhs.length(); i++) {
            newData[len + i] = rhs.data[i];
        }

        return DSString(newData);
    }

    /**
     * Standard relational operators to compare and order your strings.
     * TODO: Feel free to add additional.
     **/
    bool DSString::operator==(const DSString& rhs) const {
        //verify lengths are equal
        if(len != rhs.len){
            return false;
        }

        //Loop through each char and compare
        for(size_t i = 0; i < len; i++){
            if(data[i] != rhs.data[i]){
                //if any char is not equal we can return false
                return false;
            }
        }

        //All chars are equal
        return true;
    }

    bool DSString::operator<(const DSString& rhs) const {
        for(size_t i = 0; i < len && i < rhs.len; i++){
            if(data[i] < rhs.data[i]) {
                return true;
            } else if(data[i] > rhs.data[i]) {
                return false;
            }
        }

        //If we get here, all chars are equal up to the length of the shorter string
        //If the strings are equal, return false
        if(len == rhs.len) {
            return false;
        }

        //If the strings are not equal, return true if this string is shorter
        return len < rhs.len;
    }

    /**
     * The substring method returns a new string object that contains a
     * sequence of characters from this string object.
     *
     * @param start - the index of where to start
     * @param numChars - the number (count) of characters to copy into
     *    the substring
     * @return a DSString object containing the requested substring
     **/
    DSString DSString::substring(size_t start, size_t numChars) const {
        //Validate start and numChars
        if(start + numChars > len){
            throw std::out_of_range("Substring out of range");
        }

        char* newData = new char[numChars + 1];

        //Copy chars from 'start' to 'start + numChars'
        for(size_t i = 0; i < numChars; i++){
            newData[i] = data[i + start];
        }

        //Add null terminator
        newData[numChars] = '\0';
        return DSString(newData);
    }

    /**
     * @brief Returns a new string object with all characters in lowercase
     *
     * @return DSString
     */
    DSString DSString::toLower() const {
        char* newData = new char[len + 1];
        newData[len] = '\0';
        
        for (size_t i = 0; i < len; i++) {
            //Check if the character is in the range of uppercase letters
            if(data[i] >= 'A' && data[i] <= 'Z') {
                //Add 32 to the character to get the lowercase version
                newData[i] = data[i] + 32;
            } else {
                //Otherwise just copy the character
                newData[i] = data[i];
            }
        }

        return DSString(newData);
    } 

    /**
     * the c_str function returns a pointer a null-terminated c-string holding the
     * contents of this object. Since data already has a `\0`
     * at the end of the string in DSString so you can just return a pointer to data.
     **/
    char* DSString::c_str() const {
        return data;
    }

    /**
     * @brief return an std::string version of our DSString
     * 
     * @return std::string 
     */
    std::string DSString::string() const {
        return std::string(data);
    }

    /**
     * Overloaded stream insertion operator to print the contents of this
     * string to the output stream in the first argument.
     **/
    std::ostream& operator<<(std::ostream& out, const DSString& dsstr) {
        out.write(dsstr.data, dsstr.len);

        return out;
    }
    /**
     *  Overload the stream extraction operator to read a string from the input stream.
    */
    std::istream& operator>>(std::istream& in, DSString& dsstr) {
        char* newData;
        //TODO: read in the string from the input stream
        return in;
    }

    // You are free to add more functionality to the class.  For example,
    // you may want to add a find(...) function that will search for a
    // substring within a string or a function that breaks a string into words.
