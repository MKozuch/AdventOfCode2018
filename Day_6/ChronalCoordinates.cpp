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
#include <tuple>
#include <set>
#include <chrono>

#define INIT_FABRIC_SIZE 2

using Point = std::pair<int, int>;

void expandPlane(std::valarray<std::valarray<unsigned int>> &fabricGrid) {
   auto fabricSize = fabricGrid.size();

   auto copy = fabricGrid;
   fabricGrid.resize(fabricSize * 2, std::valarray<unsigned int>((unsigned int)0, fabricSize * 2));

   for (unsigned int i = 0; i < copy.size(); ++i) {
      std::copy(std::begin(copy[i]), std::end(copy[i]), std::begin(fabricGrid[i]));
   }
}

void printFabric(const std::valarray<std::valarray<int>> &fabricGrid) {
   if (fabricGrid.size() < 100 ) {
      for (const auto &row : fabricGrid) {
         for (const auto &cell : row) {
            if (cell)
               printf("%d", cell);
            else
               printf("-");
         }
         printf("\n");
      }
   }
}

int manhattanDist(const Point &left, const Point &right) 
{
   return std::abs(left.first - right.first) + std::abs(left.second - right.second);
}

int closestPoint(const Point &refPoint, const std::vector<Point> &pointList) {
   int closestId = 0;
   int closestDist = INT_MAX;

   std::vector<int> distList;

   for (int i = 0; i < pointList.size(); ++i) {
      auto dist = manhattanDist(pointList[i], refPoint);
      distList.push_back(dist);
   }

   auto minDistIt = std::min_element(distList.begin(), distList.end());
   if (minDistIt == distList.end())
      return 0;

   auto minDist = *minDistIt;

   if (std::count(distList.begin(), distList.end(), minDist) == 1)
      return std::distance(distList.begin(), minDistIt);
   else
      return 0;
}

int main(int argc, char** argv)
{
   auto t1 = std::chrono::high_resolution_clock::now();

	std::string fName;

   if (argc < 2) {
      fName = ".\\..\\input.txt";
   }
	else
		fName = argv[1];

	std::ifstream stream;
	stream.open(fName);

	if (!stream.is_open())
		return 1;

	std::string line;
   std::vector<Point> pointList;

   int maxX = 0;
   int maxY = 0;

	while (std::getline(stream, line)) {

		std::regex number("[[:d:]]+");
		std::smatch match;

		std::regex_search(line, match, number);

		auto resBegin = std::sregex_iterator(line.begin(), line.end(), number);
		auto resEnd = std::sregex_iterator();
		auto matches = std::distance(resBegin, resEnd);

		if (line.empty())
			break;
		if (matches != 2)
			return 1;

		int x = std::stoi((*resBegin++).str());
		int y = std::stoi((*resBegin++).str());

      maxX = x > maxX ? x : maxX;
      maxY = y > maxY ? y : maxY;

      pointList.push_back({ x,y });
   } 

   auto planeSize = std::max(maxX, maxY) + 1;
   std::valarray<std::valarray<int>> plane(std::valarray<int>((int)0, planeSize), planeSize);
   
   std::map<int, int> areaCount;
   std::set<int> rejected;

   for (auto i = 0; i < planeSize; ++i) {
      for (auto j = 0; j < planeSize; ++j) {
         auto pointId = closestPoint({ i,j }, pointList);
         plane[i][j] = pointId;

         if (i == 0 || j == 0 || i == planeSize - 1 || j == planeSize - 1 || areaCount[pointId] == -1)
            areaCount[pointId] = -1;
         else
            areaCount[pointId] += 1;
      }
   }

   printFabric(plane);

   auto maxAreaIt = std::max_element(areaCount.cbegin(), areaCount.cend(), [](const std::pair<int, int> &left, const std::pair<int, int> &right) {return left.second < right.second; });
   if (maxAreaIt != areaCount.cend() && (*maxAreaIt).second != -1) {
      auto maxAreaId = (*maxAreaIt).first;
      auto maxAreaValue = (*maxAreaIt).second;
      printf("%d\n", maxAreaValue);
   }
   else
      printf("0\n");

   auto t2 = std::chrono::high_resolution_clock::now();
   auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();
   printf("Took %d ms", elapsed);
      
   std::string n;
   std::cin >> n;
}
