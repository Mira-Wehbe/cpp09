#include "PmergeMe.hpp"
#include <iostream>
#include <sys/time.h>

static double nowMicro()
{
	struct timeval	tv;
	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000000.0 + tv.tv_usec);
}

template <typename Container>
static void	printContainer(const std::string &label, const Container &c)
{
	std::cout << label;
	for (typename Container::const_iterator it = c.begin(); it != c.end(); ++it)
		std::cout << " " << *it;
	std::cout << std::endl;
}

int	main(int argc, char **argv)
{
	if (argc < 2)
	{
		std::cerr << "Error: no arguments" << std::endl;
		return (1);
	}
	std::vector<int> numbers;
	try
	{
		numbers = PmergeMe::parseInput(argc, argv);
	}
	catch (const std::exception &e)
	{
		std::cerr << "Error: " << e.what() << std::endl;
		return (1);
	}

	printContainer("Before:", numbers);
	/* std::vector: fill the container + sort, all inside the timer */
	double	startVec = nowMicro();//get the time we start
	std::vector<int>	vecInput(numbers.begin(), numbers.end());//create the vector contain the nbr
	std::vector<int>	sortedVector = PmergeMe::sortVector(vecInput);//sorted using ford-johson
	double	endVec = nowMicro();//get the time we and

	/* std::deque: fill the container + sort, all inside the timer */
	double	startDeq = nowMicro();
	std::deque<int>		deqInput(numbers.begin(), numbers.end());
	std::deque<int>		sortedDeque = PmergeMe::sortDeque(deqInput);
	double	endDeq = nowMicro();
	printContainer("After:", sortedDeque);
	std::cout << "Time to process a range of " << numbers.size()<< " elements with std::vector : " << (endVec - startVec) << " us" << std::endl;
	std::cout << "Time to process a range of " << numbers.size()<< " elements with std::deque : " << (endDeq - startDeq) << " us" << std::endl;
	return (0);
}