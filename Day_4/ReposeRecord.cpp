#include <cstdio>
#include <fstream>
#include <string>
#include <iostream>
#include <optional>
#include <set>
#include <vector>
#include <algorithm>
#include <chrono>
#include <map>

using namespace std;


enum class Action {BeginShift, FallAsleep, WakeUp};

struct LogEntry {
	string timestamp;
	Action action;
	int minute;
	int guardId;
};

using DutyLog = std::vector<LogEntry>;

inline bool operator< (const LogEntry &lhs, const LogEntry &rhs) {
	return lhs.timestamp < rhs.timestamp;
}

LogEntry parseLine(const string &line) {
	LogEntry ret;
	ret.timestamp = line.substr(1, 16);

	auto poundIndex = line.find("#");
	if (poundIndex != string::npos) {
		auto strId = line.substr(poundIndex+1, line.find(" ", poundIndex)-poundIndex);
	    ret.guardId = stoi(strId);
		ret.minute = -1;
	}
	else {
		auto strMin = line.substr(line.find(":")+1, 2);
		ret.minute = stoi(strMin);
		ret.guardId = -1;
	}

	if (line.find("begins shift") != string::npos)
		ret.action = Action::BeginShift;
	else if (line.find("falls asleep") != string::npos)
		ret.action = Action::FallAsleep;
	else if (line.find("wakes up") != string::npos)
		ret.action = Action::WakeUp;

	return ret;
}

// [1518-11-01 00:00] Guard #10 begins shift
// [1518-11-01 00:05] falls asleep
// [1518-11-01 00:25] wakes up

int main(int argc, char** argv) {

	string fName;
	if (argc < 2)
		fName = "test.txt";
	else
		fName = argv[1];	
	
	ifstream inStream;
	inStream.open(fName);
	if (!inStream.is_open()) {
		throw("no file fond");
		return 1;
	}

	DutyLog dutyLog;

	string line;
	while (getline(inStream, line)) {
		if (line.empty())
			break;

		dutyLog.push_back(parseLine(line));
	}

	std::sort(dutyLog.begin(), dutyLog.end());

	using Minute = int;
	using GuardID = int;
	using Count = int;
	using SleepLedger = map<GuardID, map<Minute, Count>>;
	using MinutesTotalByGuard = map<GuardID, Count>;

	using MinuteCountByGuard = map < Minute, map<GuardID, Count>>;

	GuardID currentGuard = -1;
	Minute sleepStartMinute = -1;

	SleepLedger sleepLedger;
	MinutesTotalByGuard minutesTotalByGuard;
	MinuteCountByGuard minuteCountByGuard;

	for (const auto &logItem : dutyLog) {
		if (logItem.action == Action::BeginShift) {
			currentGuard = logItem.guardId;
			continue;
		}
		if (logItem.action == Action::FallAsleep) {
			sleepStartMinute = logItem.minute;
			continue;
		}
		if (logItem.action == Action::WakeUp) {
			Minute sleepStopMinute = logItem.minute;
			auto &currentGuardLedger = sleepLedger[currentGuard];
			for (Minute minute = sleepStartMinute; minute < sleepStopMinute; ++minute) {
				currentGuardLedger[minute] += 1;
				minutesTotalByGuard[currentGuard] += 1;

				(minuteCountByGuard[minute])[currentGuard] += 1;
			}
		}
	}

	// Strategy 1
	auto pp = max_element(minutesTotalByGuard.begin(), minutesTotalByGuard.end(),
		[](const pair<GuardID, Count>& p1, const pair<GuardID, Count>& p2) {
			return p1.second < p2.second; });
	auto sleepiestGuardId = pp->first;

	auto dd = max_element(sleepLedger[sleepiestGuardId].begin(), sleepLedger[sleepiestGuardId].end(),
		[](const pair<Minute, Count>& p1, const pair<Minute, Count>& p2) {
			return p1.second < p2.second; });

	auto sleepiestMinute = dd->first;

	printf("%d\n", sleepiestGuardId * sleepiestMinute);


	// Strategy 2
	vector<tuple<Minute, GuardID, Count>> kek;
	for (const auto &item : minuteCountByGuard) {
		Minute minute = item.first;

		auto red = max_element(item.second.begin(), item.second.end(),
			[](const pair<GuardID, Count>& p1, const pair<GuardID, Count>& p2) {
			    return p1.second < p2.second; });

		kek.push_back({ minute, red->first, red->second });
	}

	auto fil = max_element(kek.begin(), kek.end(),
		[](const tuple<Minute, GuardID, Count> &left, const tuple<Minute, GuardID, Count> &right) {
		    return get<2>(left) < get<2>(right); });

	printf("%d\n", get<0>(*fil) * get<1>(*fil));

	return 0;
}