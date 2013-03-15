#include "LeftTruncatablePrimes.h"
#include <fstream>
#include <cmath>
#include <boost\assert.hpp>
#include <boost\lexical_cast.hpp>
#include "OperationParameters.h"

using namespace Primes;

// test for primeness
// in: number to test
// return true if number is prime, false otherwise
bool Primes::IsPrime(int number)
{
	if (number < 3 || number % 2 == 0)
	{
		return number == 2 ? true : false;
	}
	for (int i=3; i*i<=number; i += 2)
	{
		if (number % i == 0)
		{
			return false;
		}
	}
	return true;
}

// populate list with n left truncatable primes
// limits: targetIndex must be <= OperationLimits::MaxQueryArg
// in: number of primes required
// inout: reference to list of primes which may be appended to or truncated
// throws on error
void Primes::FindLeftTruncatablePrimes(std::vector<unsigned int>& orderedLeftPrimes, unsigned int targetIndex)
{
	// error checking
	if (targetIndex > OperationLimits::MaxQueryArg)
	{
		throw std::runtime_error("FindLeftTruncatablePrimes targetIndex too great ("
			+ boost::lexical_cast<std::string>(targetIndex) +"). Must be less than " 
			+ boost::lexical_cast<std::string>(OperationLimits::MaxQueryArg + 1));
	}
	if (pow(10.f, OperationLimits::MaxPower10) > std::numeric_limits<unsigned int>::max())
	{
		throw std::logic_error("OperationLimits::MaxPower10 is greater than available storage");
	}

	// read from file if present
	std::vector<unsigned int> stordedLeftPrimes;
	std::ifstream inputFileStream(data_file);
	//if (inputFileStream.good())
	//{
	//	std::istream_iterator<unsigned int> start(inputFileStream), end;
	//	stordedLeftPrimes = std::vector<unsigned int>(start, end);
	//}

	// if required value is contained within the pre-calculated set then copy across and return
	if (stordedLeftPrimes.size() > targetIndex)
	{
		orderedLeftPrimes = stordedLeftPrimes;
	}
	// otherwise extend range beyond the pre-calculated
	else if (orderedLeftPrimes.size() < targetIndex)
	{
		// start with known primes below 10 as seed set
		orderedLeftPrimes.reserve(targetIndex);
		orderedLeftPrimes.push_back(2); orderedLeftPrimes.push_back(3); orderedLeftPrimes.push_back(5); orderedLeftPrimes.push_back(7);

		// find left truncatable primes by building set from working set of known primes and test values with leading digits added
		int base = 1;
		bool done = false;
		std::vector<unsigned int> found;
		std::vector<unsigned int> workingSet(orderedLeftPrimes);
		for (int power10 = 1; done == false && power10 < OperationLimits::MaxPower10; ++power10)
		{
			base *= 10;
			for(int leadingDigit = 1; done == false && leadingDigit < 10; ++leadingDigit)
			{
				for (auto x : workingSet) 
				{
					int testValue = (base * leadingDigit ) + x ;
					int trailingDigit = testValue % 10;
					// left truncatable primes must end it 3 or 7
					unsigned int numLeftPrimesFound = found.size() + orderedLeftPrimes.size();
					// look for precalculated value to verify against otherwise run is prime test, store prime results
					if (( numLeftPrimesFound < stordedLeftPrimes.size() && stordedLeftPrimes.at(numLeftPrimesFound) == testValue )
						 || IsPrime(testValue) )
					{
						found.push_back(testValue);
						// stop processing when have target number of primes
						if (numLeftPrimesFound + 1 == targetIndex)
						{
							done = true;
							break;
						}
					}
					
				}
			}

			// insert recently found primes into 
			orderedLeftPrimes.insert(orderedLeftPrimes.end(), found.begin(), found.end());
			workingSet = found;
			found.clear();
		}

		// write to file if new values have been added
		if (orderedLeftPrimes.size() > stordedLeftPrimes.size())
		{
			std::ofstream output_file(data_file, std::ios::binary);
			std::ostream_iterator<unsigned int> output_iterator(output_file, "\n");
			std::copy(orderedLeftPrimes.begin(), orderedLeftPrimes.end(), output_iterator);
			output_file.close();
		}
	}
}