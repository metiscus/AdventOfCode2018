#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <cstdint>

/*
	Now note that I decided to just bump up the stack size significantly for this.
	I'm certain that there were better ways to work around the overflow but I didn't feel clever.
*/

struct Node
{
	static int cnt;
	int id;
	std::vector<Node> children;
	std::vector<int> metadata;
	int64_t value_;

	int64_t value()
	{
		if (value_ != -1) return value_;

		// my value depends on the number of children I have
		if (children.size() == 0) // no children, my value is the value of the sum of my metadata
		{
			int64_t ret = 0;
			for (auto i : metadata)
			{
				ret += i;
			}
			value_ = ret;
			return ret;
		}
		else
		{
			// my metadata refer as indicies to the values of my children
			// when they exist that is...
			int64_t total = 0;
			for (int i = 0; i < metadata.size(); ++i)
			{
				int node = metadata[i];
				if (node > 0 && (node - 1) < children.size())
				{
					total += children[node - 1].value();
				}
			}

			value_ = total;
			return total;
		}
	}


	Node() : id (++cnt), value_(-1) { }
};

int Node::cnt = 0;
int64_t part1 = 0;
int64_t part2 = 0;

std::istream& operator>>(std::istream& is, Node& n)
{
	int numChildren = 0;
	int numMetadata = 0;
	int total = 0;
	if (!is.eof())
	{
		is >> numChildren;
		is >> numMetadata;
		n.children.resize(numChildren);
		for (int i = 0; i < numChildren; ++i)
		{
			is >> n.children[i];
		}

		n.metadata.resize(numMetadata);
		for (int i = 0; i < numMetadata; ++i)
		{
			is >> n.metadata[i];
			total += n.metadata[i];
			part1 += n.metadata[i];
		}
	}

	return is;
}

int main(int argc, char** argv)
{
	std::ifstream infile("input.txt");
	Node tree;
	infile >> tree;

	std::cout << tree.value() << "\n";
	return 0;
}