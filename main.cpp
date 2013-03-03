#include <vector>
#include <iostream>
#include <chrono>
#include <sstream>
#include <boost\assert.hpp>
#include <boost\lexical_cast.hpp>
#include "LeftTruncatablePrimes.h"
#include "OperationParameters.h"

using namespace Primes;

int main(int argc, char** argv)
{
	// I have not supported unit testing for this exercise but in practice I might validate against the following data
	// test data: -1, 0, 1, 3, 10, 100, 1000, 2166, 2167

	unsigned int targetIndex;
	try
	{
		// put build array out of command line args and process
		{
			std::vector<std::string> argVector(argv+1, argv + argc);
			if (argVector.size() != 1)
			{
				throw std::runtime_error("invalide number of arguements (" + boost::lexical_cast<std::string>(argVector.size()) + ")\n");
			}
			int negativeTestVal;
			std::stringstream ss; 
			ss << argVector.at(0);
			ss >> targetIndex;
			ss.seekg(0);
			ss >> negativeTestVal;
			if (ss.fail()) 
			{
				throw std::runtime_error("could not read index value in command line argument: " + argVector.at(0) + "\n");
			}
			if (targetIndex < OperationLimits::MinQueryArg)
			{
				throw std::runtime_error("index arguement must be greater than zero (" + argVector.at(0) + ")\n");
			}
			if (negativeTestVal < 0)
			{
				throw std::runtime_error("index arguement must be positive (" + argVector.at(0) + ")\n");
			}
		}

		// take initial time stamp
		using std::chrono::high_resolution_clock;
		high_resolution_clock::time_point startTime = high_resolution_clock::now();

		// Populate list of primes
		std::vector<unsigned int> orderedLeftPrimes;
		FindLeftTruncatablePrimes(orderedLeftPrimes, targetIndex);

		// take post time stamp
		high_resolution_clock::time_point endTime = high_resolution_clock::now();
		std::chrono::duration<double> time_span = std::chrono::duration_cast<std::chrono::duration<double>>(endTime - startTime);

		// output results
		unsigned int indexTrailingDigit = targetIndex % 10;
		std::string ordinality = "th";
		ordinality = indexTrailingDigit == 1 ? "st" : ordinality;
		ordinality = indexTrailingDigit == 2 ? "nd" : ordinality;
		ordinality = indexTrailingDigit == 3 ? "rd" : ordinality;
		std::cout << "result : the " << targetIndex << ordinality << " left truncatable prime = " << orderedLeftPrimes.at(targetIndex-1) << "\n";
		std::cout << "execution time: " << time_span.count() << " seconds.\n";

    }
	catch (std::logic_error& ex)
	{
		std::cerr << "A systematic error has occured: " << ex.what() << "\nExiting...\n" << std::endl;
	}
	catch (std::runtime_error& ex)
	{
		std::cerr << "An runtime error has occured: " << ex.what() << "\nExiting...\n" << std::endl;
	}
	catch (std::exception& ex)
	{
		std::cerr << "An unexpected error occured: " << ex.what() << "\nExiting...\n" << std::endl;
	}
	catch (...)
	{
		std::cerr << "An unexpected error occured: Exiting...\n" << std::endl;
	}

	std::cout << "Press enter key to exit\n";
	std::cin.get();
	return 0;
}