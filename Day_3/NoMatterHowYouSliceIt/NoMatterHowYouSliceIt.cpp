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

int main(int argc, char** argv)
{

   std::string fName;

   if (argc < 2)
      fName = "C:\\Users\\MKozuch\\Desktop\\Day_3\\NoMatterHowYouSliceIt\\test.txt";
      //fName = "C:\\Users\\MKozuch\\Desktop\\Day_3\\NoMatterHowYouSliceIt\\input.txt";
   else
      fName = argv[1];

   std::ifstream stream;
   stream.open(fName);

   std::string line;
   std::valarray<std::valarray<unsigned int>> fabricGrid(std::valarray<unsigned int>((unsigned int)0, INIT_FABRIC_SIZE), INIT_FABRIC_SIZE);

   while (std::getline(stream, line)) {

      std::regex number("[[:d:]]+");
      std::smatch match;

      std::regex_search(line, match, number);

      auto resBegin = std::sregex_iterator(line.begin(), line.end(), number);
      auto resEnd = std::sregex_iterator();
      auto matches = std::distance(resBegin, resEnd);

      if (matches != 5) 
         return 1;

      unsigned int claimNo = std::stoi((*resBegin++).str());
      unsigned int xPos    = std::stoi((*resBegin++).str());
      unsigned int yPos    = std::stoi((*resBegin++).str());
      unsigned int width   = std::stoi((*resBegin++).str());
      unsigned int length  = std::stoi((*resBegin++).str());

      auto fabricLen = fabricGrid.size();
      auto fabricWidth = fabricGrid[0].size();

      while (fabricLen <= yPos + length) {
         fabricGrid.resize(fabricLen * 2, std::valarray<unsigned int>((unsigned int)0, fabricWidth));
         fabricLen *= 2;
      }

      while (fabricWidth <= xPos + width) {
         for (auto &row : fabricGrid) {
            row.resize(fabricWidth * 2, 0);
         }
         fabricWidth *= 2;
      }

      for (int row = yPos; row < yPos + length; ++row) {
         for (int col = xPos; col < xPos + length; ++col) {
            fabricGrid[row][col]++;
         }
      }
   } 

   unsigned int count = 0;

   auto fabricLen = fabricGrid.size();
   auto fabricWidth = fabricGrid[0].size();

   //for (int row = 0; row < fabricLen; ++row)
   //   for (int col = 0; col < fabricWidth; ++col)
   //      if (fabricGrid[row][col] > 1)
   //         ++count;
   //printf("%d\n", count);

   auto count2 = std::accumulate(std::begin(fabricGrid), std::end(fabricGrid), 0,
      [](const int &sum, const std::valarray<unsigned int> &row) {
      return sum + std::count_if(std::begin(row), std::end(row), [](const unsigned int &val) {return val > 1; });
   });
   printf("%d\n", count2);
}



// 33684 too low

//126504 too high