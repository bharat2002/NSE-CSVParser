#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <iostream>
#include <algorithm>
#pragma once
struct Stock {
    std::string symbol;
    double open;
    double high;
    double low;
    double prevClose;
    double ltp;
    std::string indicativeClose; // Can be "-"
    double change;
    double percentChange;
    long volumeShares;
    double valueCrores;
    double fiftyTwoWeekHigh;
    double fiftyTwoWeekLow;
    double thirtyDayChange;
    double threeSixtyFiveDayChange;
};

class CsvParser {
public:
    static std::vector<Stock> parse(const std::string& filePath) {
        std::vector<Stock> stocks;
        std::ifstream file(filePath);
        if (!file.is_open()) {
            throw std::runtime_error("Cannot open file: " + filePath);
        }

        std::string line;
        bool isHeader = true;
        std::vector<std::string> headers;

        // Process the file line by line
        while (std::getline(file, line)) {
            // Remove leading and trailing spaces/newlines from the header
            line = trim(line);

            if (isHeader) {
                // Process the header row, split by commas
                std::stringstream ss(line);
                std::string header(ss.str());
                std::string lasttwochar (header.substr(header.size()-1, header.size()));
                if("\"" != lasttwochar){
                    continue;
                }
                isHeader = false;
                continue;  // Skip processing the header row in subsequent iterations
            }

            std::stringstream ss(line);
            std::string field;
            Stock stock;

            try {
                // Parse each column while handling spaces, special characters, and commas
                std::getline(ss, stock.symbol, ','); // Parse SYMBOL

                stock.open = parseDouble(ss);
                stock.high = parseDouble(ss);
                stock.low = parseDouble(ss);
                stock.prevClose = parseDouble(ss);
                stock.ltp = parseDouble(ss);

                std::getline(ss, stock.indicativeClose, ','); // Indicative Close (could be "-")

                stock.change = parseDouble(ss);
                stock.percentChange = parseDouble(ss);
                stock.volumeShares = parseLong(ss);
                stock.valueCrores = parseDouble(ss);
                stock.fiftyTwoWeekHigh = parseDouble(ss);
                stock.fiftyTwoWeekLow = parseDouble(ss);
                stock.thirtyDayChange = parseDouble(ss);
                stock.threeSixtyFiveDayChange = parseDouble(ss);

                stocks.push_back(stock);
            } catch (const std::exception& e) {
                std::cerr << "Error parsing row: " << e.what() << std::endl;
            }
        }

        return stocks;
    }

private:
    // Function to trim leading and trailing spaces and newline characters
    static std::string trim(const std::string& str) {
        size_t start = str.find_first_not_of(" \n\r\t");
        size_t end = str.find_last_not_of(" \n\r\t");
        return (start == std::string::npos || end == std::string::npos) ? "" : str.substr(start, end - start + 1);
    }

    // Remove commas from large numbers
    static std::string removeCommas(const std::string& str) {
        std::string result = str;
        result.erase(std::remove(result.begin(), result.end(), ','), result.end());
        return result;
    }

    static double parseDouble(std::stringstream& ss) {
        std::string field;
        std::string ignorechar;
        std::getline(ss, ignorechar, '\"');
        std::getline(ss,field,'\"');
        std::getline(ss, ignorechar, ',');
        // Handle missing or invalid data
        if (field.empty() || field == "-" || field == "NA") {
            return 0;
        }
        return std::stod(removeCommas(field)); // Remove commas before conversion
    }

    static long parseLong(std::stringstream& ss) {
        std::string field;
        std::string ignorechar;
        std::getline(ss, ignorechar, '\"');
        std::getline(ss,field,'\"');
        std::getline(ss, ignorechar, ',');
        // Handle missing or invalid data
        if (field.empty() || field == "-" || field == "NA") {
            return 0;
        }
        return std::stol(removeCommas(field)); // Remove commas before conversion
    }
};
