#include "BitcoinExchange.hpp"

#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <cctype>
#include <limits>

BitcoinExchange::BitcoinExchange(){}
BitcoinExchange::BitcoinExchange(const BitcoinExchange& other)
    : database(other.database){}
BitcoinExchange& BitcoinExchange::operator=(const BitcoinExchange& other)
{
    if (this != &other)
        database = other.database;
    return *this;
}
BitcoinExchange::~BitcoinExchange(){}

bool BitcoinExchange::isLeapYear(int year) const//check if leap year for month 2
{
    if (year % 400 == 0)
        return true;
    if (year % 100 == 0)
        return false;
    return (year % 4 == 0);
}

int BitcoinExchange::daysInMonth(int year, int month) const//Return number of days in a month
{
    if (month == 2)
    {
		if (isLeapYear(year))
			return 29;
		else
			return 28;
	}
    if (month == 4 || month == 6 || month == 9 || month == 11)
        return 30;
    return 31;
}

bool BitcoinExchange::isValidDate(const std::string& date) const//Validate YYYY-MM-DD
{
    if (date.length() != 10)//2011-01-01 has exactly 10 char
        return false;
    if (date[4] != '-' || date[7] != '-')
        return false;
    for (int i = 0; i < 10; ++i)
    {
        if (i == 4 || i == 7)
            continue;//here when we arrive to - - skip it
        if (!std::isdigit(static_cast<unsigned char>(date[i])))
            return false;
    }
    int year = std::atoi(date.substr(0, 4).c_str());
    int month = std::atoi(date.substr(5, 2).c_str());
    int day = std::atoi(date.substr(8, 2).c_str());
    if (year < 0)
        return false;
    if (month < 1 || month > 12)
        return false;
    if (day < 1 || day > daysInMonth(year, month))
        return false;
    return true;
}

bool BitcoinExchange::isValidNumber(const std::string& value) const//Validate that the string represents a number
{
    if (value.empty())
        return false;
    std::stringstream ss(value);//we put the value inside a string
    float number;
    char extra;
    ss >> number;//number get the ss value
    if (ss.fail())//when we convert hello to nbr doesnt success
        return false;
    if (ss >> extra)//is there anything that i should read it
        return false;
    if (number < 0)
        return false;
    if (number > 1000)
        return false;
    return true;
}

float BitcoinExchange::getRate(const std::string& date) const
{
    std::map<std::string, float>::const_iterator it;
    it = database.lower_bound(date);
    if (it != database.end() && it->first == date)
        return it->second;
    if (it == database.begin())
        throw std::runtime_error("date is too early");
    --it;
    return it->second;
}

void BitcoinExchange::loadDatabase(const std::string& filename)//read from data.csv
{
    std::ifstream file(filename.c_str());
	//.c_str() is used because in c++ ifstream expects a c-style string
    if (!file.is_open())
        throw std::runtime_error("could not open database file");
    std::string line;
    std::getline(file, line);//make me skip first line data ,exchange
    while (std::getline(file, line))
    {
        if (line.empty())
            continue;
        std::size_t comma = line.find(',');
        if (comma == std::string::npos)
            continue;
        std::string date = line.substr(0, comma);
        std::string rateString = line.substr(comma + 1);
        std::stringstream ss(rateString);
        float rate;
        ss >> rate;//transform it to float
        if (!ss.fail())
            database[date] = rate;//if not fail we put it in the map database[date] = rate
    }
    file.close();
}

void BitcoinExchange::processInput(const std::string& filename)
{
    loadDatabase("data.csv");
    std::ifstream file(filename.c_str());
    if (!file.is_open())
        throw std::runtime_error("could not open file");
    std::string line;
    std::getline(file, line);

    while (std::getline(file, line))
    {
        if (line.empty())
            continue;

        std::size_t pipe = line.find('|');
        if (pipe == std::string::npos)
        {
            std::cerr << "Error: bad input => " << line << std::endl;
            continue;
        }

        std::string date = line.substr(0, pipe);
        std::string valueString = line.substr(pipe + 1);

        std::size_t start = date.find_first_not_of(" \t");//"    2011-01-01   "it point to the 2 the index of 2
        std::size_t end = date.find_last_not_of(" \t");//same thing tge index of last one

        if (start == std::string::npos)//ize ken klu "      "
        {
            std::cerr << "Error: bad input => " << line << std::endl;
            continue;
        }

        date = date.substr(start, end - start + 1);//take from start to end
        start = valueString.find_first_not_of(" \t");//now for the part after "|   3"fir the 3 to avoid the space before 3
        end = valueString.find_last_not_of(" \t");

        if (start == std::string::npos)
        {
            std::cerr << "Error: bad input => " << line << std::endl;
            continue;
        }
        valueString = valueString.substr(start, end - start + 1);
        if (!isValidDate(date))
        {
            std::cerr << "Error: bad input => " << line << std::endl;
            continue;
        }
        if (!isValidNumber(valueString))
        {
            std::stringstream ss(valueString);
            float value;
            ss >> value;
            if (!ss.fail() && value < 0)
                std::cerr << "Error: not a positive number." << std::endl;
            else if (!ss.fail() && value > 1000)
                std::cerr << "Error: too large a number." << std::endl;
            else
                std::cerr << "Error: bad input => " << line << std::endl;

            continue;
        }
        std::stringstream ss(valueString);
        float value;
        ss >> value;
        try
        {
            float rate = getRate(date);
            float result = value * rate;
            std::cout << date<< " => "<< value<< " = "<< result<< std::endl;
        }
        catch (const std::exception& e)
        {
            std::cerr << "Error: " << e.what() << std::endl;
        }
    }
    file.close();
}