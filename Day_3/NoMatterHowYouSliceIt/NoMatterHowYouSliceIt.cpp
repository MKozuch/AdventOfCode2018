#include "NoMatterHowYouSliceIt.h"
#include <string>
#include <fstream>
#include <cstdio>
#include <iostream>
#include <regex>
#include <algorithm>
#include <valarray>
#include <numeric>
#include <list>
#include <map>

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

struct Claim
{
   unsigned int claimNo;
   unsigned int xPos;
   unsigned int yPos;
   unsigned int width;
   unsigned int length;
};

bool isOverlapping(Claim left, Claim right)
{
   bool a =               left.xPos <= right.xPos + right.width;
   bool b =  left.xPos + left.width > right.xPos;
   bool c =               left.yPos <= right.yPos + right.length;
   bool d = left.yPos + left.length > right.yPos;

   return a && b && c && d;
}

unsigned int nonOverlappingClaim(std::list<Claim> &claimList)
{
   std::map<unsigned int, bool> overlappingMap;
   for (auto claim : claimList)
      overlappingMap.insert_or_assign(claim.claimNo, false);

   for (auto left = claimList.begin(); left != claimList.end(); left++) {
      for (auto right = left; right != claimList.end(); right++) {
         if ((*left).claimNo != (*right).claimNo) {
            if (isOverlapping(*left, *right)) {
               overlappingMap[(*left).claimNo] |= true;
               overlappingMap[(*right).claimNo] |= true;
            }
         }
      }
   }

   for (auto item : overlappingMap)
      if (!item.second)
         return item.first;
   return 0;
}


int main(int argc, char** argv)
{
	std::string fName;

   if (argc < 2) {
      fName = ".\\input.txt";
   }
	else
		fName = argv[1];

	std::ifstream stream;
	stream.open(fName);

	if (!stream.is_open())
		return 1;

	std::string line;
	std::valarray<std::valarray<unsigned int>> fabricGrid(std::valarray<unsigned int>((unsigned int)0, INIT_FABRIC_SIZE), INIT_FABRIC_SIZE);
   std::list<Claim> claimList;

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
      claimList.push_back({ claimNo, xPos, yPos, width, length });

		auto fabricLen = fabricGrid.size();
		auto fabricWidth = fabricGrid[0].size();
		auto fabricSize = fabricLen;

		while (fabricLen <= yPos + length || fabricWidth <= xPos + width){
			auto copy = fabricGrid;
			fabricGrid.resize(fabricSize * 2, std::valarray<unsigned int>((unsigned int)0, fabricSize*2));
			
			fabricLen = fabricGrid.size();
			fabricWidth = fabricGrid[0].size();
			fabricSize = fabricLen;

			for (unsigned int i = 0; i < copy.size(); ++i) {
				std::copy(std::begin(copy[i]), std::end(copy[i]), std::begin(fabricGrid[i]));
			}
      }

      for (unsigned int row = yPos; row < yPos + length; ++row) {
         for (unsigned int col = xPos; col < xPos + width; ++col) {
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
   printf("%d\n", nonOverlappingClaim(claimList));
   
   std::string n;
   std::cin >> n;
}
