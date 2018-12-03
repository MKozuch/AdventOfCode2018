#include "NoMatterHowYouSliceIt.h"
#include <string>
#include <fstream>
#include <cstdio>
#include <iostream>
#include <regex>
#include <algorithm>
#include <valarray>
#include <numeric>

#define INIT_FABRIC_SIZE 2

void printFabric(const std::valarray<std::valarray<unsigned int>> &fabricGrid) {
	if (fabricGrid.size() < 100 && fabricGrid[0].size() < 100) {
		for (const auto &row : fabricGrid) {
			for (const auto cell : row) {
				if (cell)
					printf("%d", cell);
				else
					printf("-");
			}
			printf("\n");
		}
	}
}

int main(int argc, char** argv)
{
	std::string fName;

	if (argc < 2)
		//fName = "F:\\Code\\AdventOfCode2018\\Day_3\\test2.txt";
	 fName = "F:\\Code\\AdventOfCode2018\\Day_3\\input.txt";
   //fName = "C:\\Users\\MKozuch\\Desktop\\Day_3\\NoMatterHowYouSliceIt\\test.txt";
   //fName = "C:\\Users\\MKozuch\\Desktop\\Day_3\\NoMatterHowYouSliceIt\\input.txt";
	else
		fName = argv[1];

	std::ifstream stream;
	stream.open(fName);

	if (!stream.is_open())
		return 1;

	std::string line;
	std::valarray<std::valarray<unsigned int>> fabricGrid(std::valarray<unsigned int>((unsigned int)0, INIT_FABRIC_SIZE), INIT_FABRIC_SIZE);

	while (std::getline(stream, line)) {

		std::regex number("[[:d:]]+");
		std::smatch match;

		std::regex_search(line, match, number);

		auto resBegin = std::sregex_iterator(line.begin(), line.end(), number);
		auto resEnd = std::sregex_iterator();
		auto matches = std::distance(resBegin, resEnd);

		if (line.empty())
			break;
		if (matches != 5)
			return 1;

		unsigned int claimNo = std::stoi((*resBegin++).str());
		unsigned int xPos = std::stoi((*resBegin++).str());
		unsigned int yPos = std::stoi((*resBegin++).str());
		unsigned int width = std::stoi((*resBegin++).str());
		unsigned int length = std::stoi((*resBegin++).str());

		auto fabricLen = fabricGrid.size();
		auto fabricWidth = fabricGrid[0].size();
		auto fabricSize = fabricLen;

		while (fabricLen <= yPos + length || fabricWidth <= xPos + width){
			auto copy = fabricGrid;
			fabricGrid.resize(fabricSize * 2, std::valarray<unsigned int>((unsigned int)0, fabricSize*2));
			
			fabricLen = fabricGrid.size();
			fabricWidth = fabricGrid[0].size();
			fabricSize = fabricLen;

			for (int i = 0; i < copy.size(); ++i) {
				std::copy(std::begin(copy[i]), std::end(copy[i]), std::begin(fabricGrid[i]));
			}
         }

      for (int row = yPos; row < yPos + length; ++row) {
         for (int col = xPos; col < xPos + width; ++col) {
			 (fabricGrid[row])[col] += 1;
         }
      }
   } 

   unsigned int count = 0;

   printFabric(fabricGrid);

   auto fabricLen = fabricGrid.size();
   auto fabricWidth = fabricGrid[0].size();

   auto count2 = std::accumulate(std::begin(fabricGrid), std::end(fabricGrid), 0,
      [](const int &sum, const std::valarray<unsigned int> &row) {
      return sum + std::count_if(std::begin(row), std::end(row), [](const unsigned int &val) {return val > 1; });
   });
   printf("%d\n", count2);
}
