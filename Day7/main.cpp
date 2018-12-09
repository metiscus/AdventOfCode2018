#include <algorithm>
#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <cctype>
#include <map>
#include <set>
#include <tuple>
#include <cstring>
#include <iterator>

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

struct Helper
{
	std::string task;
	int end;
	bool tasked;

	Helper()
	{
		end = 0;
		tasked = false;
	}

	inline bool canHelp(int time) const
	{
		return time >= end;
	}

	inline void done()
	{
		task = "";
		end = 0;
		tasked = false;
	}

	inline void addTask(std::string str, int dur)
	{
		task = str;
		end = dur;
		tasked = true;
	}

};

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

	std::set<std::string> facts;
	
	auto next_task = [&verts](std::set<std::string>& facts)
	{
		for (auto& itr : verts)
		{
			if (itr.second.canDo(facts))
			{
				itr.second.visited = true;

				return itr.first;
			}
		}

		return std::string("");
	};

	auto task_len = [](std::string s)
	{
		return 60 + std::tolower(s[0]) - 'a' + 1;
	};

	std::string result;
	int time = 0;
	std::vector<Helper> helpers(5);

	while (result.length() != verts.size())
	{
		auto load_up = [&helpers, &time, next_task, task_len, &facts]() {
			// load up all workers that can
			for (auto& helper : helpers)
			{
				if (helper.canHelp(time))
				{
					std::string task = next_task(facts);
					if (task != "")
						helper.addTask(task, time + task_len(task));
					else
						break; // no more free tasks now so advance the clock
				}
			}
		};

		load_up();
		// find the time to advance to and correctly handle when there are 
		// multiple helpers done at onces
		int index;
		do {
			int nt = 1000000;
			index = -1;
			for (int id = 0; id<helpers.size(); ++id)
			{
				if (helpers[id].tasked && helpers[id].end < nt)
				{
					nt = helpers[id].end;
					index = id;
				}
			}
	
			// the next time to advance to is found
			if (index != -1)
			{
				std::cout << helpers[index].task;
				time = nt;
				facts.insert(helpers[index].task);
				result += helpers[index].task;
				helpers[index].done();
				load_up();
			}
		} while (index != -1);
	}
	std::cout << "\n";
	std::cout << "T: " << time << "\n";
	return 0;
}