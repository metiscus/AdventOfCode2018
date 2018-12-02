#if 0
#include <string>
#include <iostream>
#include <fstream>
#include <map>

std::map<char, int> frequency_count(const std::string& str)
{
	std::map<char, int> count;
	for (auto c : str)
	{
		count[c]++;
	}
	return count;
}

int main(int argc, char*argv)
{
	std::ifstream infile("input.txt");

	int has2 = 0;
	int has3 = 0;
	while (infile.is_open() && !infile.eof())
	{
		std::string code;
		infile >> code;
		auto count = frequency_count(code);
		
		bool got_2 = false;
		bool got_3 = false;
		for (auto itr : count)
		{
			if (itr.second == 3 && !got_3)
			{
				has3++;
				got_3 = true;
			}
			
			if(itr.second == 2 && !got_2)
			{
				got_2 = true;
				has2++;
			}
		}
	}

	std::cout << "Final 2s:" << has2 << " 3s:" << has3 << " Solution:" << has2 * has3 << "\n";
	return 0;
}
#endif