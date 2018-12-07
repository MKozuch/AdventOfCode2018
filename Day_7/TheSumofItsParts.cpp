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
#include <regex>
#include <set>
#include <string>
#include <tuple>
#include <valarray>

using namespace std;

struct Node;
struct NodeComp;
//using NodePtr = shared_ptr<Node>;
using NodePtr = Node * ;
using NodeValue = char;
using NodeList = std::vector<NodePtr>;

struct NodeComp
{
   bool operator()(const NodePtr left, const NodePtr right);
};

struct Node
{
   Node(char _value, NodePtr _parent) : value(_value){
      if (_parent) {
         _parent->children.insert(NodePtr(this));
         parents.insert(_parent);
      }
   }

   const NodeValue value;
   set <NodePtr, NodeComp> parents;
   set <NodePtr, NodeComp> children;

   NodePtr findDescendant(char val) {
      if (val = value)
         return NodePtr(this);

      for (auto child : children) {
         if (child->value == val)
            return child;
         
         auto n = child->findDescendant(val);
         if (n)
            return n;
      }
      return nullptr;
   }

   NodePtr findAscendant(char val);
};


bool NodeComp::operator()(const NodePtr left, const NodePtr right)
{
   return left->value < right->value;
}


NodePtr makeNode(char value, NodePtr parent) {
   return NodePtr(new Node(value, parent));
}


void traverseGraphDown( NodePtr peak, std::function<void(NodeValue)> fun) {
   if (!peak)
      return;

   fun(peak->value);
   for (auto item : peak->parents)
      traverseGraphDown(item, fun);
}

void traverseGraphUp(NodePtr root, std::function<void(NodeValue)> fun) {
   if (!root)
      return;

   fun(root->value);
   for (auto item : root->children)
      traverseGraphUp(item, fun);
}


NodePtr findNodeInList(const NodeList &nodeList, NodeValue val) {
   for (const auto &node : nodeList) {
      if (node->value == val) {
         return node;
      }
   }
   return nullptr;
}

NodePtr findPeakInList(const NodeList &nodeList) {
   for (const auto &node : nodeList) {
      if (node->children.empty()) {
         return node;
      }
   }
   return nullptr;
}

NodePtr findRootInList(const NodeList &nodeList) {
   for (const auto &node : nodeList) {
      if (node->parents.empty()) {
         return node;
      }
   }
   return nullptr;
}


int distFromRoot(const NodePtr node, const NodePtr root) {
   // TODO
}


int main(int argc, char** argv)
{
   auto t1 = std::chrono::high_resolution_clock::now();

   std::string fName;

   if (argc < 2) {
      fName = ".\\..\\test.txt";
   }
   else
      fName = argv[1];

   std::ifstream stream;
   stream.open(fName);

   if (!stream.is_open())
      return 1;

   std::string line;
   vector<NodePtr> nodeList;

   while (std::getline(stream, line)) {
      if (line.size() != 48)
         return 1;

      auto parentVal = line.at(5);
      auto childVal = line.at(36);
      
      NodePtr parent = nullptr;
      NodePtr child = nullptr;

      parent = findNodeInList(nodeList, parentVal);
      child = findNodeInList(nodeList, childVal);

      if (parent) {
         if (child) {
            parent->children.insert(child);
            child->parents.insert(parent);
         }
         else {
            child = makeNode(childVal, parent);
            nodeList.push_back(child);
         }
      }
      else {
         parent = makeNode(parentVal, nullptr);
         nodeList.push_back(parent);

         if (child) {
            parent->children.insert(child);
            child->parents.insert(parent);
         }
         else {
            child = makeNode(childVal, parent);
            nodeList.push_back(child);
         }
      }
   } 

   auto peak = findPeakInList(nodeList);
   auto root = findRootInList(nodeList);

   if (peak)
      traverseGraphUp(root, [](NodeValue val) {cout << val; });

   auto t2 = std::chrono::high_resolution_clock::now();
   auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();
   printf("\n");
   printf("Took %ld ms", elapsed);
}


// sort nodelist by 1-distance from root 2-alphabetically