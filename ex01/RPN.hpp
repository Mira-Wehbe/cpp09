#ifndef RPN_HPP
#define RPN_HPP
#include <iostream>
#include <stack>
#include <sstream>
#include <string>
#include <stdexcept>

class RPN
{
	private:
		std::stack<int> numbers;

		RPN(const RPN& other);
		RPN& operator=(const RPN& other);

	public:
		RPN();
		~RPN();

		void calculate(const std::string& expression);
};
#endif