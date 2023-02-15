/*
 * This file should help you to create a minimal interface for your string class based on
 * a dynamically allocated char array. Note that c-strings use `\0` as a terminator symbol
 * but your class should store its length in a member variable. Do not use c-string functions
 * from <string.h> or <cstring> other than for conversion from a c-sting.
 */

#ifndef DSSTRING_H
#define DSSTRING_H

#include <iostream>

class DSString {

private:
    size_t len; // the length of the string
    char* data; // a pointer to a character array containing the string with a `\0` terminator
    // Note: we keep the terminator only so we can return a c-string version in function c_str().

public:
    DSString();
    DSString(const char*); // constructor that converts a cstring
    DSString(const std::string&); // constructor that converts a std::string

    // rule-of-five
    DSString(const DSString&); // copy constructor
    DSString(DSString&&); // move constructor
    ~DSString(); // destructor
    DSString& operator=(const DSString&); // assignment operator
    DSString& operator=(DSString&&); // move assignment operator

    size_t length() const; // returns the length of the string

    char& operator[](size_t); // returns a reference to the character at the given index
    char& at(size_t); // returns a reference to the character at the given index with bounds checking

    /**
     * Overloaded operator+ which appends the string in the argument to this string
     */
    DSString operator+(const DSString&) const;

    /**
     * Overloaded operator+= which appends the string in the argument to this string
     */
    DSString& operator+=(const DSString&);

    /**
     * Overloaded operators for comparison
     */
    bool operator==(const DSString&) const;
    bool operator<(const DSString&) const;
    bool operator>(const DSString&) const;
    bool operator<=(const DSString&) const;
    bool operator>=(const DSString&) const;
    bool operator!=(const DSString&) const;

    /**
     * @brief Returns a new string object that contains a sequence of characters from this string object.
     *
     * @param start the index of where to start
     * @param numChars the number (count) of characters to copy into the substring
     * 
     * @return a DSString object containing the requested substring
     **/
    DSString substring(size_t start, size_t numChars) const;

    /**
     * @brief Remove the char at the specified index
     * 
     * @param index of the char to remove
     */
    void remove(size_t index);

    /**
     * @brief Returns a new string object with all characters in lowercase
     *
     * @return DSString
     */
    DSString toLower() const;

    /**
     * @brief Returns a pointer to a null-terminated c-string holding the contents of this object
     * 
     * @return char*
    */
    char* c_str() const;

    /**
     * @brief Returns a std::string object with the same contents as this DSString
     * 
     * @return std::string 
     */
    std::string string() const;

    /**
     * Overloaded stream insertion operator to print the contents of this
     * string to the output stream in the first argument. Overload the stream
     * extraction operator to read a string from the input stream.
     **/
    friend std::ostream& operator<<(std::ostream& in, const DSString& str);
    friend std::istream& operator>>(std::istream& in, DSString& str);
    friend std::istream& getline(std::istream& in, DSString& str, char delim);
    friend std::istream& getline(std::istream& in, DSString& str);

    /**
     * @brief Finds the first occurrence of the given substring
     * 
     * @param str The substring to search for
     * @param start The index to start searching from
     * @return size_t The index of the first occurrence of the substring or npos if not found
     * 
    */
    size_t find(const DSString&, size_t start = 0);

    /**
     * @brief Finds the first occurrence of the given substring
     * 
     * @param str The cstring to search for (must be null terminated)
     * @param start The index to start searching from
     * @return size_t The index of the first occurrence of the substring or npos if not found
     * 
    */
    size_t find(const char*, size_t start = 0);

    /**
     * @brief Finds the first occurrence of the given character
     * 
     * @param c The character to search for
     * @param start The index to start searching from
     * @return size_t The index of the first occurrence of the char or npos if not found
     * 
    */
    size_t find(char, size_t start = 0);

    //Constant to represent the maximum size of a string
    //Also used to represent that no match was found
    static const size_t npos = -1;
};

#endif
