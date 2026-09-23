#ifndef PMERGEME_HPP
# define PMERGEME_HPP

# include <vector>
# include <deque>
# include <string>

class PmergeMe
{
	public:
		PmergeMe();
		PmergeMe(const PmergeMe &other);
		PmergeMe &operator=(const PmergeMe &other);
		~PmergeMe();
		static std::vector<int>	parseInput(int argc, char **argv);
		static std::vector<int>	sortVector(std::vector<int> input);
		static std::deque<int>	sortDeque(std::deque<int> input);
		static std::vector<size_t>	jacobsthalOrder(size_t n);
};

#endif