#include "pch.h"

#include <cstdio>
#include <fstream>
#include <string>
#include <iostream>
#include <optional>
#include <set>
#include <vector>
#include <algorithm>
#include <chrono>

std::optional<int> partOne(std::string fName) {
	std::ifstream iStream;
	int count = 0;
	std::string line;

	iStream.open(fName, std::ios_base::in);
	while (std::getline(iStream, line)) {
		try {
			count += std::stoi(line);
		}
		catch (...) {
			return {};
		}
	}
	return count;
}

std::optional<int> partTwo(std::string fName) {
	std::ifstream iStream;
	int count = 0;
	std::string line;
	std::set<int> history;

	iStream.open(fName, std::ios_base::in);

	while (true) {
		std::getline(iStream, line);
		if (line.empty()) {
			iStream.clear();
			iStream.seekg(0, std::ios::beg);
		}
		else {
			try {
				count += std::stoi(line);
				if (history.find(count) != history.end())
					return count;
				history.insert(count);
			}
			catch (...) {
				return {};
				continue;
			}
		}
	}

	return {};
}

int main(int argc, char** argv) {

	if (argc < 2)
		return 1;

	std::string fName = argv[1];
	
	auto countOne = partOne(fName);
	if (countOne.has_value())
		printf("%d\n", countOne.value());

	auto start = std::chrono::steady_clock::now();
	auto countTwo = partTwo(fName);
	auto end = std::chrono::steady_clock::now();
	
	if (countTwo.has_value())
		printf("%d\n", countTwo.value());

	return 0;
}