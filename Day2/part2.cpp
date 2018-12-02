#include <string>
#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <tuple>

std::map<char, int> frequency_count(const std::string& str)
{
	std::map<char, int> count;
	for (auto c : str)
	{
		count[c]++;
	}
	return count;
}

std::tuple<int,int> hamming_distance(const std::string& l, const std::string& r)
{
	if (l.length() != r.length())
		return std::make_tuple(-1, -1);

	int distance = 0;
	int position = 0;
	bool first = true;
	for (auto i = l.begin(), j = r.begin(); i != l.end(); ++i, ++j)
	{
		if (*i != *j)
		{
			++distance;
			first = false;
		}

		if (first)
			position++;
	}

	return std::make_tuple(distance, position);
}

int main(int argc, char*argv)
{
	std::ifstream infile("input.txt");

	//std::cout << hamming_distance("abcde", "aacde") << "\n";

	std::vector<std::string> codes;
	while (infile.is_open() && !infile.eof())
	{
		std::string code;
		infile >> code;
		codes.push_back(code);
	}

	for (auto outer : codes)
	{
		for (auto inner : codes)
		{
			auto res = hamming_distance(outer, inner);
			if(std::get<0>(res) == 1)
			{
				std::cout << "Codes are: " << outer << " and " << inner << "\n";
				std::cout << "Position is: " << std::get<1>(res) << "\n";
				std::cout << "Common letters are: ";
				for (int i = 0; i < outer.length(); ++i)
				{
					if (i != std::get<1>(res)) std::cout << outer[i];
				}
				std::cout << "\n";
				
				return 0;
			}
		}
	}

	return 0;
}