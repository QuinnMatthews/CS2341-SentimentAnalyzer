#include <iostream>
#include <vector>
#include <algorithm>
#include <sstream>
#include "DSString.h"

using namespace std;

int main()
{  
    DSString myString = "Hello, World!";
    cout << myString << endl;

    // this uses the one argument constructor in DSString and then the inherited operator=
    myString = "Good bye!";
    cout << myString << endl;

    cout << "substr: " << myString.substring(5, 3) << endl;

    DSString a = "test";
    DSString b = a;

    cout << boolalpha;
    cout << (a == b) << endl;

    // use initialization list (see DSVector.h)
    vector<DSString> strings = {
        DSString("bbb"),
        DSString("aaa"),
        DSString("ddd"),
        DSString("eee"),
        DSString("ccc")};

    // find strings
    for (const auto &s : strings)
        cout
            << s << "\n";

    cout << "found ddd: " << (find(strings.begin(), strings.end(), DSString("ddd")) != strings.end()) << endl;
    cout << "found zzz: " << (find(strings.begin(), strings.end(), DSString("zzz")) != strings.end()) << endl;

    // sorting
    sort(strings.begin(), strings.end());

    for (const auto &s : strings)
        cout
            << s << "\n";

    // now we can do more efficient search
    cout << "found ddd: " << binary_search(strings.begin(), strings.end(), DSString("ddd")) << endl;
    cout << "found zzz: " << binary_search(strings.begin(), strings.end(), DSString("zzz")) << endl;

    stringstream ss("Test Streams");
    DSString inputdString, inputdString2;
    ss >> inputdString >> inputdString2;
    cout << inputdString << '\n' << inputdString2 << endl;

    stringstream ss2("Test Stream with spaces using getline");
    DSString inputString3;
    getline(ss2, inputString3);
    cout << inputString3 << endl;

    stringstream ss3("Test Stream with spaces using getline and delimiter;abcdefg");
    DSString inputString4;
    getline(ss3, inputString4, ';');
    cout << inputString4 << endl;

    DSString removeChar = "Hello: World";
    removeChar.remove(5);
    cout << removeChar << endl;

    return 0;
}
