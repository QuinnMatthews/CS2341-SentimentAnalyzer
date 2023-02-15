#include "DSWString.h"
#include <algorithm>
#include <iostream>
#include <sstream>
#include <vector>

using namespace std;

int main() {
    DSWString myString = L"Hello, World!";
    wcout << myString << endl;

    // this uses the one argument constructor in DSWString and then the inherited operator=
    myString = L"Good bye!";
    wcout << myString << endl;

    wcout << L"substr: " << myString.substring(5, 3) << endl;

    DSWString a = L"test";
    DSWString b = a;

    wcout << boolalpha;
    wcout << (a == b) << endl;

    // use initialization list (see DSVector.h)
    vector<DSWString> strings
        = { DSWString(L"bbb"), DSWString(L"aaa"), DSWString(L"ddd"), DSWString(L"eee"), DSWString(L"ÑÑÑ") };

    // find strings
    for (const auto& s : strings)
        wcout << s << "\n";

    wcout << "found ddd: " << (find(strings.begin(), strings.end(), DSWString(L"ddd")) != strings.end()) << endl;
    wcout << "found ÑÑÑ: " << (find(strings.begin(), strings.end(), DSWString(L"ÑÑÑ")) != strings.end()) << endl;

    // sorting
    sort(strings.begin(), strings.end());

    for (const auto& s : strings)
        wcout << s << "\n";

    // now we can do more efficient search
    wcout << "found ddd: " << binary_search(strings.begin(), strings.end(), DSWString(L"ddd")) << endl;
    wcout << "found zzz: " << binary_search(strings.begin(), strings.end(), DSWString(L"zzz")) << endl;

    wstringstream ss(L"Test Streams");
    DSWString inputdString, inputdString2;
    ss >> inputdString >> inputdString2;
    wcout << inputdString << '\n' << inputdString2 << endl;

    wstringstream ss2(L"Test Stream with spaces using getline");
    DSWString inputString3;
    getline(ss2, inputString3);
    wcout << inputString3 << endl;

    wstringstream ss3(L"Test Stream with spaces using getline and delimiter;abcdefg");
    DSWString inputString4;
    getline(ss3, inputString4, ';');
    wcout << inputString4 << endl;

    DSWString removeChar = L"Hello: World";
    removeChar.remove(5);
    wcout << removeChar << endl;

    return 0;
}
