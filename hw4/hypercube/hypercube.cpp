#include "hypercube.h"

using namespace std;


void HyperCube::read(istream& input) {
    // TODO: implement
    int nodeNumber;
    input >> nodeNumber;
    for(int i=0; i < nodeNumber; i++)
    {
        string myNode;
        input >> myNode;
        permitted.insert(myNode);
    }
}

SearchResult HyperCube::search(const string& start) const {
    // TODO: implement
    // Initialize ending node of all 1's.
    std::string end(start.size(), '1');

    // Instantiate priority queue to use for A* algorithm.
    std::priority_queue<Node, std::vector<Node>, NodePriority> unvisited;

    // Initialize the number of expansions used by the algorithm and path.
    size_t expansions = 0;
    std::vector<std::string> path;

    // Add any variables needed for A* algorithm
    std::map<std::string, std::string> visited;
    // Implement A* algorithm. Remember to add node label strings on the path
    // to the results vector. Order of the strings in the vector matters. Make
    // sure to return a SearchResult object.
    Node cursor = Node(start,"",0);
    unvisited.push(cursor);
    visited.insert(pair<string,string>(unvisited.top().word,unvisited.top().pred));
    unvisited.pop();

    while(cursor.word != end)
    {
        expansions++;
        //finds all neighbors
        for(unsigned int i=0; i <cursor.word.size(); i++)
        {

            string temp = cursor.word;
            if (temp[i] == '0')
            {
                temp[i] = '1';
            }
            else
            {
                temp[i] = '0';
            }
            map<string,string>::iterator it = visited.find(temp);
            if (it != visited.end())
            {
                continue;
            }
            else
            {
                set<string>::iterator it = permitted.find(temp);
                //string found
                if(it != permitted.end())
                {
                    Node newNode = Node(temp, cursor.word, cursor.g+1);
                    unvisited.push(newNode);
                }
                else
                {
                    continue; 
                }
            }
        }
        if (unvisited.top().word == end)
        {
            cursor = Node(unvisited.top().word, unvisited.top().pred, unvisited.top().g);
            path.push_back(cursor.word);
            break;
        }
        if(unvisited.empty())
        {
            SearchResult result(false, expansions);
            return result;
        }
        visited.insert(pair<string,string>(unvisited.top().word, unvisited.top().pred));
        cursor = Node(unvisited.top().word, unvisited.top().pred, unvisited.top().g);
        unvisited.pop();
    }
    string backtrack = cursor.pred;
    while(backtrack != "")
    {
        path.push_back(backtrack);
        map<string,string>::iterator it = visited.find(backtrack);
        backtrack = it->second;
    }
    reverse(path.begin(),path.end());
    SearchResult result(path,expansions);
    return result; 
}









