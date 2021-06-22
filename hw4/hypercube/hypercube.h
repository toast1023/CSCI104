#ifndef HYPER_CUBE_H
#define HYPER_CUBE_H

#include <fstream>
#include <string>
#include <vector>
#include <queue>
#include <set>
#include <sstream>
#include <iostream>
#include <map>
#include <algorithm>

struct Node {
	std::string word;
	std::string pred;
	int g;
	int h;

	Node(const std::string& word, const std::string pred, int g) : word(word), pred(pred), g(g), h(0) {
        // TODO: calculate h given the word.
        for(unsigned int i=0; i<word.size(); i++)
        {
            if(word[i] == '0')
            {
                h++;
            }
        }
	}
};

struct NodePriority {
	bool operator()(const Node& lhs, const Node& rhs) {
        // TODO: implement priority for nodes.
        int f1;
        int f2;
        f1 = lhs.g + lhs.h;
        f2 = rhs.g + rhs.h;
        if(f1 > f2)
        {
            return true;
        }
        else if (f1 < f2)
        {
            return false; 
        }
        else
        {
            if(lhs.h > rhs.h)
            {
                return true; 
            }
            else if (lhs.h < rhs.h)
            {
                return false; 
            }
            else
            {
                int num1 = stoi(lhs.word,0,2);
                int num2 = stoi(rhs.word,0,2);
                if (num1 > num2)
                {
                    return true;
                }
                else if (num2 > num1)
                {
                    return false; 
                }
            }
        }
        return false; 
	}
};

struct SearchResult {
    // Whether or not there is a path.
    bool found;

    // The node labels on the path.
    std::vector<std::string> path;

    // The number of expansions made during search.
    size_t expansions;

    // Convenience for if found.
    SearchResult(std::vector<std::string> path, size_t expansions) : found(true), path(path), expansions(expansions) {}

    // Convenience for if not found.
    SearchResult(bool found, size_t expansions) : found(found), expansions(expansions) {}
};

class HyperCube {
public:
    // TODO: take an input file stream containing the permitted nodes for the
    // n-hypercube and populate the hypercube permitted node labels with the
    // set of nodes it lists. Do not change the signature.
    void read(std::istream& input);

    // TODO: take the starting node label string and return a vector of strings
    // containing the node labels on the optimal path from the starting node to
    // the ending node. If not path is found, the returned vector should
    // contain the string "no path". The last string in the returned vector
    // should otherwise be the number of expansions.
    SearchResult search(const std::string& start) const;

private:
    //all allowed nodes
   std::set<std::string> permitted;
   // TODO: optionally add any helper methods.
};

#endif
