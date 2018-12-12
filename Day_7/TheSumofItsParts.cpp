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
#include <queue>

using namespace std;

struct Node;
struct NodeComp;
using NodePtr = Node * ;
using NodeValue = char;
using NodeList = std::vector<NodePtr>;


struct NodeComp
{
   bool operator()(const NodePtr left, const NodePtr right) const;
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

bool NodeComp::operator()(const NodePtr left, const NodePtr right) const
{
   return left->value < right->value;
}


NodePtr makeNode(char value, NodePtr parent) {
   return NodePtr(new Node(value, parent));
}

void BreadthFirstSearch(NodePtr root, const NodeList &graph) {
   enum class NodeColor { White, Grey, Black };

   struct NodeProp {
      enum class NodeColor color = NodeColor::White;
      int dist = 0;
      NodePtr parent = nullptr;
   };

   using PropList = std::map<NodePtr, NodeProp>;
   PropList propList;
   
   using NodeQueue = std::queue<NodePtr>;
   NodeQueue Q;


   for (const auto &node : graph)
      propList.insert_or_assign(node, NodeProp());

   propList[root].color = NodeColor::Grey;
   propList[root].dist = 0;
   Q.push(root);

   printf("%c", root->value);

   while (!Q.empty()) {
      const NodePtr u = Q.front();
      Q.pop();

      auto &uProp = propList[u];

      for (const auto child : u->children) {
         auto &prop = propList[child];
         if (prop.color == NodeColor::White) {
            prop.color = NodeColor::Grey;
            prop.dist = uProp.dist + 1;
            prop.parent = u;
            Q.push(child);

            printf("%c", child->value);
         }
      }

      uProp.color = NodeColor::Black;
   }
}

void traverseGraph(NodeList &_graph, std::function<void(NodeValue)> fun) {
   NodeList graph;
   std::copy(_graph.begin(), _graph.end(), std::back_inserter(graph));

   std::sort(graph.begin(), graph.end(), [](const NodePtr &a, const NodePtr & b) -> bool {return a->value < b->value; });

   while (!graph.empty()) {
      for (auto it = graph.begin(); it != graph.end(); ++it) {
         auto item = *it;
         if (item->parents.empty()) {
            fun(item->value);
            for (auto child : item->children)
               child->parents.erase(item);
            graph.erase(it);
            break;
         }
      }
   }

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


int main(int argc, char** argv)
{
   auto t1 = std::chrono::high_resolution_clock::now();

   std::string fName;

   if (argc < 2) {
       fName = ".\\..\\input.txt";
      //fName = ".\\..\\test.txt";
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
   traverseGraph(nodeList, [](NodeValue val) {printf("%c", val); });

   auto t2 = std::chrono::high_resolution_clock::now();
   auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();
   printf("\n");
   printf("Took %ld ms", elapsed);
}
