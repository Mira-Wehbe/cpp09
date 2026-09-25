#include "PmergeMe.hpp"
#include <iostream>
#include <climits>
#include <cstdlib>
#include <stdexcept>
#include <algorithm>
#include <utility>

PmergeMe::PmergeMe() {}
PmergeMe::PmergeMe(const PmergeMe &other) { (void)other; }
PmergeMe &PmergeMe::operator=(const PmergeMe &other) { (void)other; return *this; }
PmergeMe::~PmergeMe() {}

//comparison counter
size_t	PmergeMe::comparisons = 0;
bool	PmergeMe::lessCount(int a, int b)//every time we compare 2 nbr we pass from here
{
	comparisons++;
	return (a < b);
}

void	PmergeMe::resetComparisons() { comparisons = 0; }
size_t	PmergeMe::getComparisons() { return comparisons; }
//validation
std::vector<int> PmergeMe::parseInput(int argc, char **argv)//validation
{
	std::vector<int> numbers;//[]
	for (int i = 1; i < argc; i++)//loop through argument
	{
		std::string arg(argv[i]);//kl elemnt bhtu bi arg
		if (arg.empty())
			throw std::runtime_error("empty argument");
		size_t start = 0;//from first charc
		if (arg[0] == '+')//if first charct is +
			start = 1;//start from second char
		if (start == arg.size())//yane arg "+" size =1 and start =1
			throw std::runtime_error("malformed argument => " + arg);
		for (size_t j = start; j < arg.size(); j++)
		{
			if (arg[j] < '0' || arg[j] > '9')
				throw std::runtime_error("non numeric argument => " + arg);
		}
		long value = std::strtol(arg.c_str(), NULL, 10);//strg to nbr strtol want c-string that why we do c_str
		if (value <= 0 || value > INT_MAX)
			throw std::runtime_error("value out of range => " + arg);
		numbers.push_back(value);
	}
	if (numbers.empty())
		throw std::runtime_error("no arguments");
	return numbers;
}

//the jacobsthal order we should insert
//in what order should i insert my pending b elements
std::vector<size_t>	PmergeMe::jacobsthalOrder(size_t n)//lal pend
{
	std::vector<size_t>	order;//[]
	if (n < 2)//n elmnt
		return order;//if number<2 ize bl pend bas [2]
	std::vector<size_t>	jac;
	jac.push_back(1);
	jac.push_back(3);//jac=[1,3]
	while (jac.back() < n)//jac.back ekhr nbr mwjud bl jac li ana wsltlu la hl2
		jac.push_back(jac[jac.size() - 1] + 2 * jac[jac.size() - 2]);//j(n)=j(n-1)+2*j(n-2)
		//[1,3,5]
	size_t prev = 1;
	for (size_t i = 1; i < jac.size(); i++)
	{
		size_t t = jac[i];//t=3(first iteration)
		if (t > n)
			t = n;//t tser 11 bs n msln 6 mnshn ma wsl la b11 weml reverse whk
		for (size_t idx = t; idx > prev; idx--)
			order.push_back(idx);//hone beml revrse yabe b2 b3 btser b3 b2
		prev = jac[i];//btser prev =3 (bsht8l 4 5)(1,3,5,11)(bmshe 4,5 baaden mn 6 lal 11)
		if (prev >= n)
			break;
	}
	return order;//lorder li ha emlu insert
}
//input->pair elemnt->separate smal/large->sort the larger first->build mainchain->insert smaller using joca
//->sorted result
static std::vector<int>	fordJohnsonVector(std::vector<int> input)
{
	size_t n = input.size();
	if (n <= 1)
		return input;
	bool hasStray = (n % 2 != 0);
	int stray = 0;
	if (hasStray)
	{
		stray = input.back();//ekhr wehd li ma ilu couple
		input.pop_back();
	}
	std::vector<std::pair<int, int> > pairs;
	for (size_t i = 0; i < input.size(); i += 2)
	{
		int a = input[i];
		int b = input[i + 1];
		if (PmergeMe::lessCount(a, b))//(small,big)
			pairs.push_back(std::make_pair(a, b));
		else
			pairs.push_back(std::make_pair(b, a));
	}
	std::vector<int> largers;
	for (size_t i = 0; i < pairs.size(); i++)
		largers.push_back(pairs[i].second);
	std::vector<int> sortedLargers = fordJohnsonVector(largers);
	
	std::vector<std::pair<int, int> > sortedPairs;
	std::vector<bool> used(pairs.size(), false);//used[flase,false..3adad lpair false]
	for (size_t i = 0; i < sortedLargers.size(); i++)
	{
		for (size_t j = 0; j < pairs.size(); j++)
		{
			if (!used[j] && pairs[j].second == sortedLargers[i])
			{
				sortedPairs.push_back(pairs[j]);
				used[j] = true;//[true,false,...]
				break;
			}
		}
	}
	std::vector<int> mainChain = sortedLargers;
	mainChain.insert(mainChain.begin(), sortedPairs[0].first);//b1 before a1 (b1,a1,a2,a3,...)

	//posA[k] = index of a(k+1) inside mainChain (b1 at 0 so a1 at 1, a2 at 2 ...)
	std::vector<size_t> posA;
	for (size_t i = 0; i < sortedPairs.size(); i++)
		posA.push_back(i + 1);

	//the stray is like one more b without an a so it go in the jacobsthal order too
	size_t pendSize = sortedPairs.size() + (hasStray ? 1 : 0);
	std::vector<size_t> order = PmergeMe::jacobsthalOrder(pendSize);
	for (size_t k = 0; k < order.size(); k++)
	{
		size_t idx = order[k] - 1;
		int value;
		size_t limit;//we search only in [0, limit)
		if (idx < sortedPairs.size())
		{
			value = sortedPairs[idx].first;//b
			limit = posA[idx];//b < its a, so search only before a (this is what save comparisons)
		}
		else
		{
			value = stray;//stray has no a -> search all the chain
			limit = mainChain.size();
		}
		std::vector<int>::iterator pos = std::lower_bound(mainChain.begin(), mainChain.begin() + limit, value, PmergeMe::lessCount);
		size_t insertIdx = pos - mainChain.begin();
		mainChain.insert(pos, value);
		for (size_t j = 0; j < posA.size(); j++)//every a after the new nbr moved 1 step right
			if (posA[j] >= insertIdx)
				posA[j]++;
	}
	if (hasStray && pendSize == 1)
	{
		std::vector<int>::iterator pos = std::lower_bound(mainChain.begin(), mainChain.end(), stray, PmergeMe::lessCount);
		mainChain.insert(pos, stray);
	}
	return mainChain;
}

std::vector<int> PmergeMe::sortVector(std::vector<int> input)
{
	return fordJohnsonVector(input);
}

//deque
static std::deque<int>	fordJohnsonDeque(std::deque<int> input)
{
	size_t n = input.size();
	if (n <= 1)
		return input;
	bool hasStray = (n % 2 != 0);
	int stray = 0;
	if (hasStray)
	{
		stray = input.back();
		input.pop_back();
	}

	std::deque<std::pair<int, int> > pairs;
	for (size_t i = 0; i < input.size(); i += 2)
	{
		int a = input[i];
		int b = input[i + 1];
		if (PmergeMe::lessCount(a, b))
			pairs.push_back(std::make_pair(a, b));
		else
			pairs.push_back(std::make_pair(b, a));
	}

	std::deque<int> largers;
	for (size_t i = 0; i < pairs.size(); i++)
		largers.push_back(pairs[i].second);

	std::deque<int> sortedLargers = fordJohnsonDeque(largers);
	std::deque<std::pair<int, int> > sortedPairs;
	std::deque<bool> used(pairs.size(), false);
	for (size_t i = 0; i < sortedLargers.size(); i++)
	{
		for (size_t j = 0; j < pairs.size(); j++)
		{
			if (!used[j] && pairs[j].second == sortedLargers[i])
			{
				sortedPairs.push_back(pairs[j]);
				used[j] = true;
				break;
			}
		}
	}
	std::deque<int> mainChain = sortedLargers;
	mainChain.push_front(sortedPairs[0].first);//b1 before a1
	std::vector<size_t> posA;
	for (size_t i = 0; i < sortedPairs.size(); i++)
		posA.push_back(i + 1);

	//the stray is like one more b without an a, so it goes in the jacobsthal order too
	size_t pendSize = sortedPairs.size() + (hasStray ? 1 : 0);
	std::vector<size_t> order = PmergeMe::jacobsthalOrder(pendSize);
	for (size_t k = 0; k < order.size(); k++)
	{
		size_t idx = order[k] - 1;//0-based index of the b we insert
		int value;
		size_t limit;//we search only in [0, limit)
		if (idx < sortedPairs.size())
		{
			value = sortedPairs[idx].first;//b
			limit = posA[idx];//b < its a, so search only before a (this is what saves comparisons)
		}
		else
		{
			value = stray;//stray has no a -> search all the chain
			limit = mainChain.size();
		}
		std::deque<int>::iterator pos = std::lower_bound(mainChain.begin(), mainChain.begin() + limit, value, PmergeMe::lessCount);
		size_t insertIdx = pos - mainChain.begin();
		mainChain.insert(pos, value);
		for (size_t j = 0; j < posA.size(); j++)//every a after the new nbr moved 1 step right
			if (posA[j] >= insertIdx)
				posA[j]++;
	}
	if (hasStray && pendSize == 1)//only 1 pair + stray: jacobsthalOrder(1) is empty
	{
		std::deque<int>::iterator pos = std::lower_bound(mainChain.begin(), mainChain.end(), stray, PmergeMe::lessCount);
		mainChain.insert(pos, stray);
	}
	return mainChain;
}//same as above but here using deque

std::deque<int>	PmergeMe::sortDeque(std::deque<int> input)
{
	return fordJohnsonDeque(input);
}