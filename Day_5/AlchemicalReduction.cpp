#include <cstdio>
#include <fstream>
#include <string>
#include <iostream>
#include <optional>
#include <set>
#include <vector>
#include <algorithm>
#include <chrono>
#include <cctype>
#include <sstream>
#include <list>


void dump(std::string str) {
   std::ofstream outfStream;
   outfStream.open("C:\\Users\\MKozuch\\Desktop\\AdventOfCode2018\\Day_5\\dump.txt", std::ofstream::app);
   outfStream << str << std::endl;
}

bool simplifyString(std::istreambuf_iterator<char> begin, std::ostringstream &dest) {
   dest.str("");

   std::istreambuf_iterator<char> end;
   auto it = begin;
   bool isSimplified = true;

   auto left = *(it++);

   while(1) {
      
      if (it == end) {
         if ((left >= 'a' && left <= 'z') || (left >= 'A' && left <= 'Z'))
            dest.put(left);
         break;
      }

      auto right = *(it++);

      if (std::abs(left - right) == 32) {
         left = *(it++);
         isSimplified = false;
      }
      else {
         if((left >= 'a' && left <= 'z') || (left >= 'A' && left <= 'Z'))
            dest.put(left);
         left = right;
      }
   }

   return isSimplified;
}

std::optional<int> partOne(std::string fName) {
	std::ifstream inFileStream;
   inFileStream.open(fName);
   if (!inFileStream.is_open())
      return {};

   std::istringstream inStringStream;
   std::ostringstream outStringStream;
   bool isSimplified = simplifyString(std::istreambuf_iterator<char>(inFileStream), outStringStream);

   while (!isSimplified) {
      inStringStream.str(outStringStream.str());
      isSimplified = simplifyString(std::istreambuf_iterator<char>(inStringStream), outStringStream);
   }

   auto kke = outStringStream.str();

   outStringStream.seekp(0, std::ios::end);
   int size = outStringStream.tellp();
   return size;
}

std::optional<int> partOneV2(std::string fName) {
   std::ifstream inFileStream;
   inFileStream.open(fName);

   if (!inFileStream.is_open())
      return {};

   std::list<char> lst;
   while (!inFileStream.eof()) {
      auto c = inFileStream.get();
      if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))
         lst.push_back(c);
   }

   auto simplify = [&lst]()->bool {
      for (auto it = lst.begin(); it != lst.end(); ++it) {
         if ((std::next(it) != lst.end()) && abs(*it - *std::next(it)) == 32) {
            it = lst.erase(it);
            it = lst.erase(it);
            return false;
         }
      }
      return true;
   };

   while (!simplify());

   // dump(std::string(lst.begin(), lst.end()));
   return lst.size(); 
}

std::optional<int> partTwo(std::string fName) {
	std::ifstream iStream;
	return {};
}

int main(int argc, char** argv) {

   std::string fName;
   if (argc < 2) {
      return 1;
   }
   else
      fName = argv[1];

	auto countOne = partOne(fName);
	if (countOne.has_value())
		printf("%d\n", countOne.value());

	// auto start = std::chrono::steady_clock::now();
	// auto countTwo = partTwo(fName);
	// auto end = std::chrono::steady_clock::now();
	return 0;
}