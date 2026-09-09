#ifndef BITCOINEXCHANGE_HPP
#define BITCOINEXCHANGE_HPP
#include <map>
#include <string>
class BitcoinExchange
{
private:
    std::map<std::string, float> database;
    bool isValidDate(const std::string& date) const;
    bool isLeapYear(int year) const;
    int daysInMonth(int year, int month) const;
    bool isValidNumber(const std::string& value) const;
    float getRate(const std::string& date) const;
    void loadDatabase(const std::string& filename);
public:
    BitcoinExchange();
    BitcoinExchange(const BitcoinExchange& other);
    BitcoinExchange& operator=(const BitcoinExchange& other);
    ~BitcoinExchange();
    void processInput(const std::string& filename);
};

#endif