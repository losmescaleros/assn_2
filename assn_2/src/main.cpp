#include <stdio.h>
#include <string>
#include <iostream>

#include <main.h>

std::string studentFile;
std::string strategy;

FILE *out;

bool isValidStrategyArg(std::string strategy);
void addRecord(std::string record);
std::string findRecord(int key);
void deleteRecord(int key);

int main(int argc, char* argv[])
{
	if (argc != 3)
	{
		std::cout << "Invalid number of arguments" << std::endl;
		exit(EXIT_ERROR);
	} 
	else
	{
		strategy = argv[1];
		studentFile = argv[2];

		// Is this a valid strategy argument?
		if (!isValidStrategyArg(strategy))
		{
			std::cout << "Invalid strategy argument: " << strategy << std::endl;
			exit(EXIT_ERROR);
		}
		
		bool exit = false;
		while (!exit)
		{
			std::string cmdString;
			std::getline(std::cin, cmdString);

			std::string cmd;
			size_t index = cmdString.find_first_of(" ", 0);
			std::cout << cmdString.substr(0, index);

			// Add 
			// Delete
			// Find
			// End 
		}
	}
}

bool isValidStrategyArg(std::string strategy)
{
	if (strategy.compare(STRATEGY_BEST_FIT) == 0)
	{
		return true;
	}
	else if (strategy.compare(STRATEGY_FIRST_FIT) == 0)
	{
		return true;
	}
	else if (strategy.compare(STRATEGY_WORST_FIT) == 0)
	{
		return true;
	}

	// Strategy argument did not match valid arg
	return false;
}

void addRecord(std::string record)
{
	// Parse the record string
	int key;
	// If record exists, print message
	std::cout << "Record with SID=" << key << "exists" << std::endl;
}

std::string findRecord(int key)
{

}

void deleteRecord(int key)
{

}