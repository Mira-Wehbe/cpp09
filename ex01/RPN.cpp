#include "RPN.hpp"
RPN::RPN(){}
RPN::~RPN(){}
void RPN::calculate(const std::string& expression)
{
	std::stringstream ss(expression);
	std::string token;

	while (ss >> token)
	{
		if (token.length() == 1 && token[0] >= '0' && token[0] <= '9')
		{
			numbers.push(token[0] - '0');
		}
		else if (token == "+" || token == "-" ||
				 token == "*" || token == "/")
		{
			if (numbers.size() < 2)
				throw std::runtime_error("Error");

			int second = numbers.top();
			numbers.pop();

			int first = numbers.top();
			numbers.pop();

			int result;

			if (token == "+")
				result = first + second;
			else if (token == "-")
				result = first - second;
			else if (token == "*")
				result = first * second;
			else
			{
				if (second == 0)
					throw std::runtime_error("Error");
				result = first / second;
			}

			numbers.push(result);
		}
		else
			throw std::runtime_error("Error");
	}

	if (numbers.size() != 1)
		throw std::runtime_error("Error");

	std::cout << numbers.top() << std::endl;
}