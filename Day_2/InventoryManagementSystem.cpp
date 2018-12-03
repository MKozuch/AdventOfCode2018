#include <fstream>
#include <cstdio>
#include <iostream>
#include <string>
#include <set>
#include <vector>
#include <algorithm>


int partOne(const std::string &fName);
bool hasMultiples(const std::string &str, unsigned int multiCount);
bool hasDoubles(const std::string &str);
bool hasTriples(const std::string &str);

std::string partTwo(const std::string &fName);
bool compareIds(const std::string &left, const std::string &right);
std::string commonPart(const std::string &left, const std::string &right);


int main(int argc, char** argv){
	
	if (argc < 2)
		return 1;

	std::string fName = argv[1];
	printf("%d\n", partOne(fName));
	printf("%s\n", partTwo(fName).data());

	return 0;
}


int partOne(const std::string & fName)
{
	std::ifstream iStream;
	iStream.open(fName);

	if (!iStream.is_open())
		return 1;

	std::string line;
	int tripleCount = 0;
	int doubleCount = 0;

	while (std::getline(iStream, line)) {
		if (hasDoubles(line))
			++doubleCount;
		if (hasTriples(line))
			++tripleCount;
	}

	return doubleCount * tripleCount;
}

bool hasMultiples(const std::string &str, unsigned int multiCount)
{
	std::set<char> charSet;
	for (const auto chr : str)
		charSet.insert(chr);

	for (const auto chr : charSet) {
		auto count = std::count(str.begin(), str.end(), chr);
		if (count == multiCount)
			return true;
	}
	return false;
}

bool hasDoubles(const std::string & str)
{
	return hasMultiples(str, 2);
}

bool hasTriples(const std::string & str)
{
	return hasMultiples(str, 3);
}


std::string partTwo(const std::string & fName)
{
	std::ifstream iStream;
	iStream.open(fName);

	if (!iStream.is_open())
		return {};

	std::vector<std::string> idVector;
	std::string line;

	while (std::getline(iStream, line))
		idVector.push_back(line);

	auto startingID = idVector.cbegin();
	while (startingID != idVector.end()) {
		for (auto it = startingID+1; it < idVector.cend(); ++it) {
			if (compareIds(*startingID, *it))
				return commonPart(*startingID, *it);
		}
		++startingID;
	}

	return  {};
}

bool compareIds(const std::string & left, const std::string & right)
{
	if (left.length() != right.length())
		return false;

	unsigned int mismatchCount = 0;

	for (unsigned int i = 0; i < std::min(left.length(), right.length()); ++i) {
		if (left[i] != right[i])
			++mismatchCount;
	}
	return mismatchCount <= 1;
}

std::string commonPart(const std::string & left, const std::string & right)
{
	std::string ret;

	for (unsigned int i = 0; i < std::min(left.length(), right.length()); ++i) {
		if (left[i] == right[i]) {
			char c = left.at(i);
			ret += c;
		}
	}

	return ret;
}
