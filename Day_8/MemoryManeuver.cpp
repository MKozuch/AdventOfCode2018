#include <algorithm>
#include <chrono>
#include <cstdio>
#include <functional>
#include <fstream>
#include <iostream>
#include <list>
#include <map>
#include <memory>
#include <numeric>
#include <optional>
#include <regex>
#include <set>
#include <string>
#include <tuple>
#include <valarray>
#include <queue>

using namespace std;

//#define TEST

int main(int argc, char** argv)
{
   auto t1 = std::chrono::high_resolution_clock::now();

   std::string fName;

   if (argc < 2) {
#ifdef TEST
      fName = ".\\..\\test.txt";
#else
      fName = ".\\..\\input.txt";
#endif // TEST
   }
   else
      fName = argv[1];

   std::ifstream stream;
   stream.open(fName);

   if (!stream.is_open())
      return 1;

   auto next = [&stream]()->int{
      string line;
      stream >> line;
      return stoi(line);
   };

   function<int()> parseNode = [&next, &parseNode]()->int {
      int childrenCount = next();
      int metaCount = next();
      int metaSum = 0;

      for (auto i = 0; i < childrenCount; ++i) {
         metaSum += parseNode();
      }
      for (auto i = 0; i < metaCount; ++i) {
         metaSum += next();
      }

      return metaSum;
   };

   auto count = parseNode();
   printf("%d\n", count);

   return 0;
}