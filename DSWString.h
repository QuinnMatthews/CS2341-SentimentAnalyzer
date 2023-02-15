/*
 * This file should help you to create a minimal interface for your string class based on
 * a dynamically allocated char array. Note that c-strings use `\0` as a terminator symbol
 * but your class should store its length in a member variable. Do not use c-string functions
 * from <string.h> or <cstring> other than for conversion from a c-sting.
 */

#ifndef DSWSTRING_H
#define DSWSTRING_H

#include <iostream>

class DSWString {

private:
    size_t len; // the length of the string
    wchar_t* data; // a pointer to a character array containing the string with a `\0` terminator
    // Note: we keep the terminator only so we can return a c-string version in function c_str().

public:
    DSWString();
    DSWString(const wchar_t*); // constructor that converts a cstring
    DSWString(const std::wstring&); // constructor that converts a std::wstring

    // rule-of-five
    DSWString(const DSWString&); // copy constructor
    DSWString(DSWString&&); // move constructor
    ~DSWString(); // destructor
    DSWString& operator=(const DSWString&); // assignment operator
    DSWString& operator=(DSWString&&); // move assignment operator

    size_t length() const; // returns the length of the string

    wchar_t& operator[](size_t); // returns a reference to the character at the given index
    wchar_t& at(size_t); // returns a reference to the character at the given index with bounds checking

    /**
     * Overloaded operator+ which appends the string in the argument to this string
     */
    DSWString operator+(const DSWString&) const;

    /**
     * Overloaded operator+= which appends the string in the argument to this string
     */
    DSWString& operator+=(const DSWString&);

    /**
     * Overloaded operators for comparison
     */
    bool operator==(const DSWString&) const;
    bool operator<(const DSWString&) const;
    bool operator>(const DSWString&) const;
    bool operator<=(const DSWString&) const;
    bool operator>=(const DSWString&) const;
    bool operator!=(const DSWString&) const;

    /**
     * @brief Returns a new string object that contains a sequence of characters from this string object.
     *
     * @param start the index of where to start
     * @param numChars the number (count) of characters to copy into the substring
     * 
     * @return a DSWString object containing the requested substring
     **/
    DSWString substring(size_t start, size_t numChars) const;

    /**
     * @brief Remove the char at the specified index
     * 
     * @param index of the char to remove
     */
    void remove(size_t index);

    /**
     * @brief Returns a new string object with all characters in lowercase
     *
     * @return DSWString
     */
    DSWString toLower() const;

    /**
     * @brief Returns a pointer to a null-terminated c-string holding the contents of this object
     * 
     * @return char*
    */
    wchar_t* c_str() const;

    /**
     * @brief Returns a std::wstring object with the same contents as this DSWString
     * 
     * @return std::wstring 
     */
    std::wstring string() const;

    /**
     * Overloaded stream insertion operator to print the contents of this
     * string to the output stream in the first argument. Overload the stream
     * extraction operator to read a string from the input stream.
     **/
    friend std::wostream& operator<<(std::wostream& in, const DSWString& str);
    friend std::wistream& operator>>(std::wistream& in, DSWString& str);
    friend std::wistream& getline(std::wistream& in, DSWString& str, wchar_t delim);
    friend std::wistream& getline(std::wistream& in, DSWString& str);

    /**
     * @brief Finds the first occurrence of the given substring
     * 
     * @param str The substring to search for
     * @param start The index to start searching from
     * @return size_t The index of the first occurrence of the substring or npos if not found
     * 
    */
    size_t find(const DSWString&, size_t start = 0);

    /**
     * @brief Finds the first occurrence of the given substring
     * 
     * @param str The cstring to search for (must be null terminated)
     * @param start The index to start searching from
     * @return size_t The index of the first occurrence of the substring or npos if not found
     * 
    */
    size_t find(const wchar_t*, size_t start = 0);

    /**
     * @brief Finds the first occurrence of the given character
     * 
     * @param c The character to search for
     * @param start The index to start searching from
     * @return size_t The index of the first occurrence of the char or npos if not found
     * 
    */
    size_t find(wchar_t, size_t start = 0);

    //Constant to represent the maximum size of a string
    //Also used to represent that no match was found
    static const size_t npos = -1;
};

#endif
