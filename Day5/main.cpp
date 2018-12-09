#include <string>
#include <iostream>
#include <fstream>
#include <ctype.h>
#include <set>

std::string folded_polymer_length(const std::string& polymer);
std::string remove_polymer_unit(const std::string& polymer, std::string unit);

int main(int arg, char** argv)
{
	std::ifstream infile("input.txt");
	std::string polymer;
	infile >> polymer;

	std::string tmp = folded_polymer_length(polymer);
	std::cout << "Part 1 solution is: " << tmp.length() << "\n";
	polymer = tmp; 
	// optimization: the reduction that happens is being held back by certain letters,
	// which one is the important factor

	std::string units = "abcdefghijklmnopqrstuvwxyz";
	for (int i = 0; i < units.length(); ++i)
	{
		std::string unit;
		unit += units[i];
		std::string polymer_test = remove_polymer_unit(polymer, unit);
		unit[0] = toupper(units[i]);
		std::cout << "Removal of unit " << units[i] << " length is " << folded_polymer_length(remove_polymer_unit(polymer_test, unit)).length() << "\n";
	}

	return 0;
}

std::string remove_polymer_unit(const std::string& polymer, std::string unit)
{
	std::string ret = polymer;
	std::string::size_type n = unit.length();
	for (std::string::size_type i = ret.find(unit);
		i != std::string::npos;
		i = ret.find(unit)
	)
		ret.erase(i, n);
	return ret;
}

std::string folded_polymer_length(const std::string& polymer)
{
	//now fold until there are no more folds
	std::string theString = polymer;
	bool didFold = false;
	do {
		std::string tmp;
		didFold = false;
		for (auto i = 0ULL; i < theString.length() - 1; ++i)
		{
			if ((tolower(theString[i]) == tolower(theString[i + 1])) &&
				(theString[i] != theString[i + 1]))
			{
				// it annihilates
				didFold = true;
				++i;
			}
			else
			{
				tmp += theString[i];
			}
		}

		tmp += theString[theString.length() - 1];
		theString = tmp;

	} while (didFold);

	return theString;
}