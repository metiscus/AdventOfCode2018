#include <algorithm>
#include <fstream>
#include <iostream>
#include <chrono>
#include <vector>
#include <string>
#include <sstream>
#include <iomanip>
#include <map>
#include <set>
#include <tuple>
#include <cstring>
#include <iterator>
#include <algorithm>

struct Vertex
{
	Vertex(std::string n = "")
		: name(n)
		, visited(false)
	{ }

	bool visited;
	std::string name;
	std::set<std::string> requires;

	inline bool canDo(const std::set<std::string>& facts) const
	{
		if (visited) return false;
		std::set<std::string> set;
		std::set_intersection(requires.begin(), requires.end(), facts.begin(), facts.end(), std::inserter(set, set.begin()));
		return (set == requires);
	}
};

bool operator<(const Vertex& l, const Vertex& r)
{
	return l.name < r.name;
}


int main(int argc, char** argv)
{
	std::ifstream infile("input.txt");
	
	std::map<std::string, Vertex> verts;

	while (!infile.eof())
	{
		std::string a;
		std::string b;
		std::string trash;
		
		infile >> trash >> a >> trash >> trash >> trash >> trash >> trash >> b >> trash >> trash;
		if (verts.count(a) == 0)
		{
			verts.emplace(std::make_pair(a, Vertex(a)));
		}

		if (verts.count(b) == 0)
		{
			verts.emplace(std::make_pair(b, Vertex(b)));
		}

		verts[b].requires.insert(a);		
	}

	std::string result = "";
	std::set<std::string> facts;
	
	while (result.size() != verts.size())
	{
		for (auto& itr : verts)
		{
			if (itr.second.canDo(facts))
			{
				result += itr.first;
				itr.second.visited = true;
				facts.insert(itr.first);
				break;
			}
		}
	}

	// print the order here
	std::cout << result << "\n";

	// now we need to determine with 5 helpers how long this would take

	return 0;
}