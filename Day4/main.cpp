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

struct Event
{
	std::tm tm;
	std::time_t timestamp;
	int guard;

	bool operator<(const Event& rhs) const
	{
		return timestamp < rhs.timestamp;
	}

	enum Action
	{
		WakesUp,
		FallsAsleep,
		StartsShift
	};
	Action action;
};

std::vector<Event> read_events();
std::vector<Event> find_days_events(const std::vector<Event>& eventList, int day);
std::pair<int, int> find_top_value(const std::map<int, int>& map);

int main(int argc, char** argv)
{
	auto events = read_events();

	// find all of the events that apply to a specific day
	// within those events sum the times asleep to the current active guard
	int currentDay = events[0].tm.tm_yday;
	int currentHour = events[0].tm.tm_hour;
	int currentMinute = events[0].tm.tm_min;
	int currentGuard = -1;

	std::map<int, int> guards;
	std::map<int, std::map<int, int>> guardSleep;
	int sleepStartTime = 0;
	for (int day = 0; day < 364; ++day)
	{
		auto todaysEvents = find_days_events(events, day);
		for (auto evnt : todaysEvents)
		{
			switch (evnt.action)
			{
			case Event::StartsShift:
				//std::cout << "Guard " << evnt.guard << " begins shift.\n";
				currentGuard = evnt.guard;
				break;

			case Event::FallsAsleep:
				//std::cout << "Guard " << currentGuard << " falls asleep at " << evnt.tm.tm_min << ".\n";
				sleepStartTime = evnt.tm.tm_min;
				break;

			case Event::WakesUp:
				//std::cout << "Guard " << currentGuard << " wakes up at " << evnt.tm.tm_min << ".\n";
				guards[currentGuard] += (evnt.tm.tm_min - sleepStartTime);
				//std::cout << "Guard " << currentGuard << " sleep coverage: \n";
				for (int i = sleepStartTime; i < evnt.tm.tm_min; ++i)
				{
					//std::cout << "\t" << i << "\n";
					guardSleep[currentGuard][i] ++;
				}
				break;
			}
		}

		sleepStartTime = 0;
	}

	std::pair<int, int> topGuard;
	for (auto entry : guards)
	{
		if (entry.second > topGuard.second)
		{
			topGuard = entry;
		}
	}

	// for part 1 solution we need to find the key associated with the largest value in the guardSleep map entry that maps to our top_guard
	{
		int minute = find_top_value(guardSleep[topGuard.first]).first;
		std::cout << "Part 1 solution is guard number: " << topGuard.first << " at minute " << minute << "\n";
		std::cout << "Part 1 solution is: " << topGuard.first * minute << "\n";
	}

	// for part 2 solution we need to find the key in the guardSleep map associated with the largest value in the guardSleep map
	// simplest way I can think of is a find_top_minute function 
	topGuard = { 0, 0 };
	int topMinCount = 0;
	for (auto table : guardSleep)
	{
		// entry is minute, hit count
		auto entry = find_top_value(table.second);
		if (entry.second > topMinCount)
		{
			topMinCount = entry.second;
			topGuard.first = table.first;
			topGuard.second = entry.first;
		}
	}
	std::cout << "Part 2 guard: " << topGuard.first << " minute: " << topGuard.second << "\n";
	std::cout << "Part 2 answer: " << topGuard.first * topGuard.second << "\n";
}

std::pair<int, int> find_top_value(const std::map<int, int>& map)
{
	std::pair<int, int> top = { 0, 0 };
	for (auto entry : map)
	{
		if (entry.second > top.second)
		{
			top = entry;
		}
	}

	return top;
}

std::vector<Event> read_events()
{
	std::vector<Event> events;
	std::ifstream infile("input.txt");
	while (infile.is_open() && !infile.eof())
	{
		std::string line;
		std::getline(infile, line);

		if (line.length() == 0)
		{
			break;
		}

		std::stringstream ss;
		ss << line.substr(1);

		Event evnt;
		memset(&evnt.tm, 0, sizeof(evnt.tm));

		//ss >> std::get_time(&evnt.tm, "[%Y-%m-%d %H:%M]");
		char trash;
		ss >> evnt.tm.tm_year >> trash >> evnt.tm.tm_mon >> trash >> evnt.tm.tm_mday >> trash >> evnt.tm.tm_hour >> trash >> evnt.tm.tm_min;
		evnt.tm.tm_yday = evnt.tm.tm_mon * 31 + evnt.tm.tm_mday; // yeah hax ofc
		evnt.timestamp = 60 * 24 * evnt.tm.tm_yday + evnt.tm.tm_hour * 60 + evnt.tm.tm_min;

		std::string action;
		do {
			ss >> action;
		} while (action == "]" || action == "" || action == " ");

		if (action == "wakes")
		{
			evnt.action = Event::WakesUp;
			events.emplace_back(evnt);
		}
		else if (action == "falls")
		{
			evnt.action = Event::FallsAsleep;
			events.emplace_back(evnt);
		}
		else if (action == "Guard")
		{
			evnt.action = Event::StartsShift;

			// parse guard line
			std::string nr;
			ss >> nr;
			evnt.guard = std::atoi(nr.substr(1).c_str());
			events.emplace_back(evnt);
		}
	}

	// sort in time order
	std::sort(events.begin(), events.end());
	return events;
}

std::vector<Event> find_days_events(const std::vector<Event>& eventList, int day)
{
	std::vector<Event> events;

	for (auto itr : eventList)
	{
		if (itr.tm.tm_yday == day)
		{
			events.emplace_back(itr);
		}
	}

	return events;
}