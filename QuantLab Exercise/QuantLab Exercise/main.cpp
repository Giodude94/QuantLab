#include <fstream>
#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <tuple>


int main() {

	//Constructing and Initializing the map that is to be used.
	std::map<std::string, std::vector<std::tuple<std::string, int, int>>> tradeInfo;

	//Name of the file to read from.
	std::ifstream reader("input.csv");

	//Creating the file to write to if it is not already in the directory.
	std::ofstream writer;
	writer.open("output.csv");

	//Attempting to open the input file
	if (!reader.is_open())
		std::cout << "Could not open the file." << std::endl;

	//Declaring and Initializing variables that are to be used in the program
	unsigned long long maxTimeGap = 0;
	int volume = 0;
	int weightedAveragePrice = 0;
	int maxPrice = 0;


	std::string time = "";
	std::string symbol;
	std::string quantity;
	std::string price;


	while (reader.peek() != EOF) {

		std::getline(reader, time, ',');
		std::getline(reader, symbol, ',');
		std::getline(reader, quantity, ',');
		std::getline(reader, price, '\n');

		//Creating the tuple to be inserted into the map using the symbol as the key.
		auto temp = std::make_tuple(time, std::stoi(quantity), std::stoi(price));

		//Pushing into the vector the tuple holding the symbols' time, quantity, and price using the symbol as the key.
		tradeInfo[symbol].push_back(temp);

	}

	//Iterating through each key in the map, there is only one.
	for (std::map<std::string, std::vector<std::tuple<std::string, int, int>>>::iterator it = tradeInfo.begin(); it != tradeInfo.end(); it++) {

		//Iterating through the vector that holds the tuple of each trade symbol.
		for (auto vecIt = it->second.begin(); vecIt != it->second.end(); vecIt++) {

			//adds the running total of volume for every trade in each symbol.
			volume += std::get<1>(*vecIt);
			
			//adds the running total of quantities * prices for each trade in each symbol.
			weightedAveragePrice += std::get<1>(*vecIt) * std::get<2>(*vecIt);

			//Calculation for finding the max Price per symbol
			if (maxPrice < std::get<2>(*vecIt))
				maxPrice = std::get<2>(*vecIt);

			if (*vecIt != it->second.back()) {//If the VecIterator is not at the last element in the vector then we can do the maxTimeGap Calculations
				auto nextVec = std::next(vecIt);
				if ((std::stoull(std::get<0>(*nextVec)) - std::stoull(std::get<0>(*vecIt))) > maxTimeGap) {
					maxTimeGap = ((std::stoull(std::get<0>(*nextVec))) - (std::stoull(std::get<0>(*vecIt))));
				}

			}
			else {//Cannot do the maxTimeGap calculations here because we are at the end of the vector. Do nothing.

			}
		}

		//Once we have iterated through every tuple in the vector we divide the weightedAveragePrice, which is currently holding the running total of each trade's price multiplied by its quantity, by the volume to get the weighted average price.
		weightedAveragePrice /= volume;

		//Writing the desired information into the output.csv file per sybol.
		writer << it->first << "," << maxTimeGap << "," << volume << "," << weightedAveragePrice << "," << maxPrice << std::endl;

		// Setting each variable that was used to find the calculations for each symbol back to zero.
		volume = 0;
		maxTimeGap = 0;
		weightedAveragePrice = 0;
		maxPrice = 0;
	}





	reader.close();


	return 0;
}