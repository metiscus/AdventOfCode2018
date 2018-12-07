#include <array>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

struct Rectangle
{
	std::string name;
	int min_x;
	int min_y;
	int max_x;
	int max_y;

	inline bool overlaps(const Rectangle& other) const
	{
		return (other.max_x >= min_x) &&
			(other.min_x <= max_x) &&
			(other.max_y >= min_y) &&
			(other.min_y <= max_y);
	}
};

std::istream& operator>>(std::istream& in, Rectangle& r)
{
	in >> r.name;
	std::string trash;
	in >> trash;

	// split origin
	{
		std::string coord;
		in >> coord;
		auto comma = coord.find(',');
		coord[comma] = ' ';
		coord = coord.substr(0, coord.length() - 1);
		std::stringstream ss;
		ss << coord;
		ss >> r.min_x;
		ss >> r.min_y;
	}

	// parse
	{
		std::string coord;
		in >> coord;
		auto ex = coord.find('x');
		coord[ex] = ' ';
		std::stringstream ss;
		ss << coord;
		int temp;
		ss >> temp;
		r.max_x = temp + r.min_x;
		ss >> temp;
		r.max_y = temp + r.min_y;
	}

	return in;
}

int main(int argc, char** argv)
{
	std::fstream infile("input.txt");

	int *overlap = (int*)calloc(1000, sizeof(int)*1000);

	std::vector<Rectangle> rectangles;

	uint32_t covered = 0;
	while (infile.is_open() && !infile.eof())
	{
		std::string line;
		std::getline(infile, line);

		Rectangle r;
		std::stringstream ss;
		ss << line;
		ss >> r;

		rectangles.push_back(r);
		//apply rectangle to space
		for (int y = r.min_y; y < r.max_y; ++y)
		{
			for (int x = r.min_x; x < r.max_x; ++x)
			{
				overlap[y * 1000 + x]++;
			}
		}
	}

	for (uint32_t i = 0; i < 1000 * 1000; ++i)
	{
		if (overlap[i] > 1) ++covered;
	}

	std::cout << covered << "\n";
	
	// iterate over each rectangle checking if we intersect
	for (auto rect : rectangles)
	{
		bool good = true;
		for (auto inner : rectangles)
		{
			if (inner.name != rect.name)
			{
				if (rect.overlaps(inner))
				{
					good = false;
					break;
				}
			}
		}

		if (good)
		{
			std::cout << "Found good claim: " << rect.name << "\n";
			//break;
		}
	}
	return 0;
}