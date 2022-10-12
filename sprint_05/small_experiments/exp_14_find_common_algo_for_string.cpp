// CPP program to demonstrate working of string
// find to search a string
#include <iostream>
#include <string>
#include <algorithm>

using namespace std;

int main()
{
	string str = "moscow motovilikha murmansk"s;
	string substrToSearch = "mo";

	// Find FIRST occurrence of "mo"
    size_t firstOccurrence = str.find(substrToSearch);

    // Find LAST occurrence of "mo"
    size_t lastOccurrence = str.rfind(substrToSearch);

    string substr = "moscow"s.substr(0, substrToSearch.size());



    cout << "FIRST occurrence position: " << firstOccurrence << endl;
    cout << "LAST occurrence position: " << lastOccurrence << endl;
    cout << "substr = " << substr << endl;


	return 0;
}
