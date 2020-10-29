#include <iostream>
#include <string>
#include <map>
#include <set>
#include <algorithm>
#include <queue>

struct Request
{
	std::string id;
	char side;
	int quantity;
	int price;

	friend std::istream& operator>>(std::istream& input, Request& req)
	{
		input >> req.id >> req.side >> req.quantity >> req.price;
		return input;
	}
};

void printSet(const std::set<std::string>& st)
{
	for (const auto& trade : st)
		std::cout << trade << " ";
	std::cout << '\n';
}

bool buy(Request& rq, std::map<int, std::queue<Request> >& Sell)
{
	if (Sell.empty() || Sell.begin()->first > rq.price)
		return false;

	std::map<std::pair<std::string, int>, int> sold; // <trader, price> -> quantity
	std::map<int, int> bought; // price -> quantity

	auto start = Sell.begin();

	while (rq.quantity > 0 && !Sell.empty() && start->first <= rq.price)
	{
		auto& Queue = start->second;
		while (!Queue.empty() && rq.quantity > 0)
		{
			int dec = std::min(rq.quantity, Queue.front().quantity);
			rq.quantity -= dec;
			Queue.front().quantity -= dec;

			sold[{Queue.front().id, Queue.front().price}] += dec;
			bought[Queue.front().price] += dec;

			if (Queue.front().quantity == 0)
				Queue.pop();
		}

		if (Queue.empty())
			start = Sell.erase(start);
	}

	std::set<std::string> aggressorExe;

	for (const auto& cur : sold)
	{
		std::string trade = cur.first.first + "-"; // trader-
		trade += std::to_string(cur.second) + "@" + std::to_string(cur.first.second); // count@price
		aggressorExe.insert(trade);
	}

	for (const auto& cur : bought)
	{
		std::string trade = rq.id + "+"; // trader+
		trade += std::to_string(cur.second) + "@" + std::to_string(cur.first); // count@price
		aggressorExe.insert(trade);
	}

	if (!aggressorExe.empty())
		printSet(aggressorExe);

	return rq.quantity == 0;
}

bool sell(Request& rq, std::map<int, std::queue<Request> >& Buy)
{
	if (Buy.empty() || Buy.rbegin()->first < rq.price)
		return false;

	std::map<std::pair<std::string, int>, int> bought; // <trader, price> -> quantity
	std::map<int, int> sold; // price -> quantity

	auto start = Buy.rbegin().base();
	--start;

	while (rq.quantity > 0 && !Buy.empty() && start->first >= rq.price)
	{
		auto& Queue = start->second;
		while (!Queue.empty() && rq.quantity > 0)
		{
			int dec = std::min(rq.quantity, Queue.front().quantity);
			rq.quantity -= dec;
			Queue.front().quantity -= dec;

			bought[{Queue.front().id, Queue.front().price}] += dec;
			sold[Queue.front().price] += dec;

			if (Queue.front().quantity == 0)
				Queue.pop();
		}

		if (Queue.empty())
			Buy.erase(start--);
	}

	std::set<std::string> aggressorExe;

	for (const auto& cur : bought)
	{
		std::string trade = cur.first.first + "+"; // trader+
		trade += std::to_string(cur.second) + "@" + std::to_string(cur.first.second); // count@price
		aggressorExe.insert(trade);
	}

	for (const auto& cur : sold)
	{
		std::string trade = rq.id + "-"; // trader-
		trade += std::to_string(cur.second) + "@" + std::to_string(cur.first); // count@price
		aggressorExe.insert(trade);
	}

	if (!aggressorExe.empty())
		printSet(aggressorExe);

	return rq.quantity == 0;
}

int main()
{
	std::map<int, std::queue<Request> > Sell, Buy;

	Request rq;

	while (std::cin >> rq)
	{
		bool matched;
		if (rq.side == 'B')
			matched = buy(rq, Sell);
		else
			matched = sell(rq, Buy);

		if (!matched)
			(rq.side == 'B' ? Buy : Sell)[rq.price].push(rq);
	}

}