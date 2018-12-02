#include <set>
#include <cstdint>
#include <iostream>
#include <fstream>
#include <vector>

int main(int argc, char** argv)
{
	//read in list of frequency changes
	std::vector<int64_t> changes;
	std::ifstream infile("input.txt");
	std::cout << "Loading input";
	while (infile.is_open() && !infile.eof())
	{
		int64_t num = 0;
		infile >> num;
		changes.push_back(num);

		if (changes.size() % 100 == 0)
		{
			std::cout << ".";
		}
	}
	std::cout << "done!\n";
	std::cout << "List size is: " << changes.size() << "\n";

	// now iterate over the list until a dupe is found
	std::set<int64_t> frequency_history;
	int64_t frequency = 0;
	auto itr = changes.begin();
	do {
		if (frequency_history.count(frequency) == 1)
		{
			std::cout << "The first repeat is " << frequency << "\n";
			break;
		}
		else
		{
			frequency_history.insert(frequency);
			frequency += *(itr++);

			if (itr == changes.end())
			{
				itr = changes.begin();
			}
		}
	} while (true);

	int a;
	std::cin >> a;
}