#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>

using std::vector;
using std::cout;
using std::cin;
using std::endl;
using std::string;

struct PersonInfo {
	string name;
	vector<string> phones;
};


int main() {
	string fileName = "people_phones.txt";
	std::ifstream inputFileStream(fileName, std::ifstream::in);
	/* std::ifstream inputFileStream("people_phones.txt"); */
	int count{0};
	string line{""}, word{""};
	vector<PersonInfo> people;

	while (getline(inputFileStream, line)) {
		PersonInfo info;
		std::istringstream record(line);
		record >> info.name;

		while (record >> word) {
			info.phones.push_back(word);
		}
		people.push_back(info);
	}
}
