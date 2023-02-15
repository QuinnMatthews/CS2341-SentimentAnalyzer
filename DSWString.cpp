#include "DSWString.h"

DSWString::DSWString() {
    len = 0;
    data = new wchar_t[1];
    data[len] = '\0';
}

// constructor that converts a cstring
DSWString::DSWString(const wchar_t* cstr) {
    //Determine length of cstring
    len = 0;
    for (size_t i = 0; cstr[i] != '\0'; i++) {
        len++;
    }

    //Create new char array the correct size
    data = new wchar_t[len + 1];

    //Copy cstring into wchar_t array
    for (size_t i = 0; i < len; i++) {
        data[i] = cstr[i];
    }

    //Add null terminator
    data[len] = '\0';
}

//constructor that converts a std::string
DSWString::DSWString(const std::wstring& str) {
    len = str.length();
    data = new wchar_t[len + 1];

    //Copy each character from string into char array
    for (size_t i = 0; i < len; i++) {
        data[i] = str.at(i);
    }

    //Add null terminator
    data[len] = '\0';
}

// copy constructor
DSWString::DSWString(const DSWString& cpyFrm) {
    len = cpyFrm.len;
    data = new wchar_t[len + 1];

    //Copy each character
    for (size_t i = 0; i < len; i++) {
        data[i] = cpyFrm.data[i];
    }

    //Add null terminator
    data[len] = '\0';
}

//Move constructor
DSWString::DSWString(DSWString&& moveFrm) {
    len = moveFrm.len;
    data = moveFrm.data;

    moveFrm.data = nullptr;
    moveFrm.len = 0;
}

//destructor
DSWString::~DSWString() {
    //Clean up char array
    delete[] data;
}

//assignment operator
DSWString& DSWString::operator=(const DSWString& cpyFrm) {
    //Clean up old data
    delete data;

    len = cpyFrm.len;
    data = new wchar_t[len + 1];

    //Copy each character
    for (size_t i = 0; i < len; i++) {
        data[i] = cpyFrm.data[i];
    }

    //Add null terminator
    data[len] = '\0';

    return *this;
}

// move assignment operator
DSWString& DSWString::operator=(DSWString&& moveFrm) {
    //Clean up old data
    delete[] data;

    len = moveFrm.len;
    data = moveFrm.data;

    moveFrm.data = nullptr;
    moveFrm.len = 0;

    return *this;
}

/**
     * @brief returns the length of the string
     * 
     * @return int containing the length of the string
     */
size_t DSWString::length() const { return len; }

// returns a reference to the character at the given index
wchar_t& DSWString::operator[](size_t index) { return data[index]; }

// returns a reference to the char at the given index and includes bounds checking
wchar_t& DSWString::at(size_t index) {
    if (index >= len) {
        throw std::out_of_range("Index out of range");
    }

    return data[index];
}

/**
     * Overloaded operator+ which appends the string in the argument to this string
     */
DSWString DSWString::operator+(const DSWString& rhs) const {
    size_t combinedLen = len + rhs.length() + 1;
    wchar_t* newData = new wchar_t[combinedLen];
    newData[len] = '\0';

    for (size_t i = 0; i < len; i++) {
        newData[i] = data[i];
    }
    for (size_t i = 0; i < rhs.length(); i++) {
        newData[len + i] = rhs.data[i];
    }

    return DSWString(newData);
}

bool DSWString::operator==(const DSWString& rhs) const {
    //verify lengths are equal
    if (len != rhs.len) {
        return false;
    }

    //Loop through each char and compare
    for (size_t i = 0; i < len; i++) {
        if (data[i] != rhs.data[i]) {
            //if any char is not equal we can return false
            return false;
        }
    }

    //All chars are equal
    return true;
}

bool DSWString::operator<(const DSWString& rhs) const {
    for (size_t i = 0; i < len && i < rhs.len; i++) {
        if (data[i] < rhs.data[i]) {
            return true;
        } else if (data[i] > rhs.data[i]) {
            return false;
        }
    }

    //If we get here, all chars are equal up to the length of the shorter string
    //If the strings are equal, return false
    if (len == rhs.len) {
        return false;
    }

    //If the strings are not equal, return true if this string is shorter
    return len < rhs.len;
}

bool DSWString::operator>(const DSWString& rhs) const {
    for (size_t i = 0; i < len && i < rhs.len; i++) {
        if (data[i] > rhs.data[i]) {
            return true;
        } else if (data[i] < rhs.data[i]) {
            return false;
        }
    }

    //If we get here, all chars are equal up to the length of the shorter string
    //If the strings are equal, return false
    if (len == rhs.len) {
        return false;
    }

    //If the strings are not equal, return true if this string is longer
    return len > rhs.len;
}

bool DSWString::operator!=(const DSWString& rhs) const {
    //check if lengths are equal
    if (len != rhs.len) {
        //if lengths are not equal, strings are not equal
        return true;
    }

    //Loop through each char and compare
    for (size_t i = 0; i < len; i++) {
        if (data[i] != rhs.data[i]) {
            //if any char is not equal we can return true
            return true;
        }
    }

    //All chars are equal
    return false;
}

bool DSWString::operator<=(const DSWString& rhs) const {
    //TODO: This would probably be more efficient if we checked this ourselves
    return this->operator<(rhs) || this->operator==(rhs);
}

bool DSWString::operator>=(const DSWString& rhs) const {
    //TODO: This would probably be more efficient if we checked this ourselves
    return this->operator>(rhs) || this->operator==(rhs);
}

/**
     * The substring method returns a new string object that contains a
     * sequence of characters from this string object.
     *
     * @param start - the index of where to start
     * @param numChars - the number (count) of characters to copy into
     *    the substring
     * @return a DSWString object containing the requested substring
     **/
DSWString DSWString::substring(size_t start, size_t numChars) const {
    //Validate start and numChars
    if (start + numChars > len) {
        throw std::out_of_range("Substring out of range");
    }

    wchar_t* newData = new wchar_t[numChars + 1];

    //Copy chars from 'start' to 'start + numChars'
    for (size_t i = 0; i < numChars; i++) {
        newData[i] = data[i + start];
    }

    //Add null terminator
    newData[numChars] = '\0';

    //Create new DSWString object
    DSWString newstr = DSWString(newData);

    //Clean up memory
    delete[] newData;

    //Return the new DSWString
    return newstr;
}

void DSWString::remove(size_t index) {
    //verify that index is valid
    if (index > len) {
        throw std::out_of_range("Index out of range");
    }

    //Create new char[] 1 shorter than the old (still need room for null terminator)
    wchar_t* newdata = new wchar_t[len];

    size_t j = 0;
    for (size_t i = 0; i < len; i++) {
        if (i != index) {
            newdata[j] = data[i];
            j++;
        }
    }

    //Clean up old data
    delete[] data;
    data = newdata;
    len = len - 1;

    //Add null terminator
    data[len] = '\0';
}

/**
     * @brief Returns a new string object with all characters in lowercase
     *
     * @return DSWString
     */
DSWString DSWString::toLower() const {
    wchar_t* newData = new wchar_t[len + 1];
    newData[len] = '\0';

    for (size_t i = 0; i < len; i++) {
        //Check if the character is in the range of uppercase letters
        if (data[i] >= 'A' && data[i] <= 'Z') {
            //Add 32 to the character to get the lowercase version
            newData[i] = data[i] + 32;
        } else {
            //Otherwise just copy the character
            newData[i] = data[i];
        }
    }

    return DSWString(newData);
}

/**
     * the c_str function returns a pointer a null-terminated c-string holding the
     * contents of this object. Since data already has a `\0`
     * at the end of the string in DSWString so you can just return a pointer to data.
     **/
wchar_t* DSWString::c_str() const { return data; }

/**
     * @brief return an std::string version of our DSWString
     * 
     * @return std::string 
     */
std::wstring DSWString::string() const { return std::wstring(data); }

/**
     * Overloaded stream insertion operator to print the contents of this
     * string to the output stream in the first argument.
     **/
std::wostream& operator<<(std::wostream& out, const DSWString& dsstr) {
    out.write(dsstr.data, dsstr.len);

    return out;
}

/**
     *  Overload the stream extraction operator to read a string from the input stream.
    */
std::wistream& operator>>(std::wistream& in, DSWString& dsstr) {
    //Clean up old data
    delete[] dsstr.data;
    dsstr.len = 0;

    size_t tmpLen = 5; // Length of the newData char[], scales exponentially to minimize copying
    wchar_t* newData = new wchar_t[tmpLen];

    wchar_t tmpchar; // Holds each char from the input buffer

    //Read in each char from the input buffer until we hit a space or newline
    while (in.get(tmpchar) && tmpchar != ' ' && tmpchar != '\n') {
        dsstr.len++; //Holds the actual length of the data that's been read in

        //Scale if needed
        if (dsstr.len > tmpLen) {
            //Expand char[]
            tmpLen = tmpLen * 2;
            wchar_t* tmpData = new wchar_t[tmpLen];

            //Copy data from old/shorter char[] to new/longer char[]
            for (size_t i = 0; i < dsstr.len - 1; i++) {
                tmpData[i] = newData[i];
            }

            //delete old char[]
            delete[] newData;
            newData = nullptr;

            //Set newData equal to tmpData
            newData = tmpData;
        }

        newData[dsstr.len - 1] = tmpchar;
    }

    //Create an appropriatly sized new array
    dsstr.data = new wchar_t[dsstr.len + 1];
    for (size_t i = 0; i < dsstr.len; i++) {
        dsstr.data[i] = newData[i];
    }
    //Add null terminator
    dsstr.data[dsstr.len] = '\0';
    //Clean up memory
    delete[] newData;

    return in;
}

std::wistream& getline(std::wistream& in, DSWString& dsstr) {
    //Clean up old data
    delete[] dsstr.data;
    dsstr.len = 0;

    size_t tmpLen = 5; // Length of the newData char[], scales exponentially to minimize copying
    wchar_t* newData = new wchar_t[tmpLen];

    wchar_t tmpchar; // Holds each char from the input buffer

    //Read in each char from the input buffer until we hit a newline
    while (in.get(tmpchar) && tmpchar != '\n') {
        dsstr.len++; // Holds the actual length of the data that's been read in

        //Scale if needed
        if (dsstr.len > tmpLen) {
            //Expand char[]
            tmpLen = tmpLen * 2;
            wchar_t* tmpData = new wchar_t[tmpLen];

            //Copy data from old/shorter char[] to new/longer char[]
            for (size_t i = 0; i < dsstr.len - 1; i++) {
                tmpData[i] = newData[i];
            }

            //delete old char[]
            delete[] newData;
            newData = nullptr;

            //Set newData equal to tmpData
            newData = tmpData;
        }

        newData[dsstr.len - 1] = tmpchar;
    }

    //Create an appropriatly sized new array
    dsstr.data = new wchar_t[dsstr.len + 1];
    for (size_t i = 0; i < dsstr.len; i++) {
        dsstr.data[i] = newData[i];
    }
    //Add null terminator
    dsstr.data[dsstr.len] = '\0';
    //Clean up memory
    delete[] newData;

    return in;
}

std::wistream& getline(std::wistream& in, DSWString& dsstr, wchar_t delim) {
    //Clean up old data
    delete[] dsstr.data;
    dsstr.len = 0;

    size_t tmpLen = 5; // Length of the newData char[], scales exponentially to minimize copying
    wchar_t* newData = new wchar_t[tmpLen];

    wchar_t tmpchar; // Holds each char from the input buffer

    //Read in each char from the input buffer until we hit a newline
    while (in.get(tmpchar) && tmpchar != delim) {
        dsstr.len++; // Holds the actual length of the data that's been read in

        //Scale if needed
        if (dsstr.len > tmpLen) {
            //Expand char[]
            tmpLen = tmpLen * 2;
            wchar_t* tmpData = new wchar_t[tmpLen];

            //Copy data from old/shorter char[] to new/longer char[]
            for (size_t i = 0; i < dsstr.len - 1; i++) {
                tmpData[i] = newData[i];
            }

            //delete old char[]
            delete[] newData;
            newData = nullptr;

            //Set newData equal to tmpData
            newData = tmpData;
        }

        newData[dsstr.len - 1] = tmpchar;
    }

    //Create an appropriatly sized new array
    dsstr.data = new wchar_t[dsstr.len + 1];
    for (size_t i = 0; i < dsstr.len; i++) {
        dsstr.data[i] = newData[i];
    }
    //Add null terminator
    dsstr.data[dsstr.len] = '\0';
    //Clean up memory
    delete[] newData;

    return in;
}

/**
     * @brief Finds the first occurrence of the given substring
     * 
     * @param substr The string to search for
     * @param start The index to start searching from
     * @return size_t The index of the first occurrence of the substring or npos if not found
    */
size_t DSWString::find(const DSWString& substr, size_t start) {
    //validate start index
    if (start >= len) {
        throw std::out_of_range("Start index out of range");
    }
    //validate substring length
    if (substr.len + start > len) {
        throw std::out_of_range("Substring length out of range");
    }

    //Loop through each character in this string
    for (size_t i = start; i < len; i++) {
        //Check if the current character matches the first character of the substring
        if (data[i] == substr.data[0]) {
            //Loop through each character in the substring
            for (size_t j = 0; j < substr.len; j++) {
                //Check if the current character in the substring matches the current character in this string
                if (data[i + j] != substr.data[j]) {
                    //If the characters don't match, break out of the loop
                    break;
                } else if (j == substr.len - 1) {
                    //If we get here, all characters in the substring matched
                    //Return the index of the first character in the substring
                    return i;
                }
            }
        }
    }

    //Substring not found, return npos
    return npos;
}

/**
     * @brief Finds the first occurrence of the given substring
     * 
     * @param substr The cstring to search for (must be null terminated)
     * @param start The index to start searching from
     * @returns The index of the first character in the substring, or npos if the substring is not found
    */
size_t DSWString::find(const wchar_t* substr, size_t start) {
    //Validate start index
    if (start >= len) {
        throw std::out_of_range("Start index out of range");
    }
    //Determine length of substring
    size_t substrLen = 0;
    while (substr[substrLen] != '\0') {
        substrLen++;
    }
    //Validate substring length
    if (substrLen + start > len) {
        return npos;
    }

    //Loop through each character in this string
    for (size_t i = start; i < len; i++) {
        //Check if the current character matches the first character of the substring
        if (data[i] == substr[0]) {
            //Loop through each character in the substring
            for (size_t j = 0; j < substrLen; j++) {
                //Check if the current character in the substring matches the current character in this string
                if (data[i + j] != substr[j]) {
                    //If the characters don't match, break out of the loop
                    break;
                } else if (j == substrLen - 1) {
                    //If we get here, all characters in the substring matched
                    //Return the index of the first character in the substring
                    return i;
                }
            }
        }
    }

    //Substring not found, return npos
    return npos;
}

/**
     * @brief Finds the first occurrence of the given substring
     * 
     * @param c The character to search for
     * @param start The index to start searching from
    */
size_t DSWString::find(wchar_t c, size_t start) {
    //Validate start index
    if (start >= len) {
        throw std::out_of_range("Start index out of range");
    }

    //Loop through each character in this string
    for (size_t i = start; i < len; i++) {
        //Check if the current character matches the given character
        if (data[i] == c) {
            //Return the index of the character
            return i;
        }
    }

    //Character not found, return npos
    return npos;
}