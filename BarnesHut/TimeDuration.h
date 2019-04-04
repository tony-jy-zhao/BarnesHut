#pragma once
#include <functional>
#include <unordered_map>

class TimeDuration
{
public:
	TimeDuration(int);
	double calculateTime(const std::function<void(void)> &func);
private:
	const int rounds_;
};
