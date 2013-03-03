#include <vector>
#include <boost\assert.hpp>

#include <iostream>
#include <ctime>
#include <ratio>
#include <chrono>
#include <fstream>
#include <iterator>

enum OperationLimits { MinQueryArg = 1, MaxQueryArg = 2166, MaxPower10 = 9 } ;
const static std::string data_file("primes.dat");

bool IsPrime(int number)
{
	// TODO throw for negative values?
	BOOST_ASSERT(number >= 1);
	if (number < 3 || number % 2 == 0)
	{
		return number == 2 ? true : false;
	}
	for (int i=3; i*i<number; i += 2)
	{
		if (number % i == 0)
		{
			return false;
		}
	}
	return true;
}

//1  2  3  4  5   6   7   8   9   10  11  12  13  14  15  16   17   18   19
//2, 3, 5, 7, 13, 17, 23, 37, 43, 47, 53, 67, 73, 83, 97, 113, 137, 167, 173
int main(int argc, char* argv[])
{
	// test data: 1, 3, 10, 100, 1000, 2166 (< 1000000000)
	using std::chrono::high_resolution_clock;
	high_resolution_clock::time_point startTime = high_resolution_clock::now();

	// vector to hold primes
	unsigned int targetIndex = 2166;
	std::vector<int> orderedLeftPrimes;
	std::vector<int> stordedLeftPrimes;

	// read from file if present
	std::ifstream is(data_file);
	if (is.good())
	{
		std::istream_iterator<int> start(is), end;
		stordedLeftPrimes = std::vector<int>(start, end);
	}

	// if required value is contained within the pre-calculated set then copy across and return
	if (stordedLeftPrimes.size() <= targetIndex)
	{
		orderedLeftPrimes = stordedLeftPrimes;
	}
	// otherwise extend range beyond the pre-calculated
	else if (orderedLeftPrimes.size() < targetIndex)
	{
		// start with known primes below 10 as seed set
		orderedLeftPrimes.reserve(targetIndex);
		orderedLeftPrimes.push_back(2);
		orderedLeftPrimes.push_back(3);
		orderedLeftPrimes.push_back(5);
		orderedLeftPrimes.push_back(7);

		std::vector<int> found;
		std::vector<int> workingSet(orderedLeftPrimes);

		int base = 1;
		bool done = false;
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
					unsigned int foo = found.size() + orderedLeftPrimes.size();
					if (foo < stordedLeftPrimes.size() && stordedLeftPrimes.at(foo) == testValue)
					{
						found.push_back(testValue);
					}
					else if ((trailingDigit == 3 || trailingDigit == 7) && IsPrime(testValue))
					{
						found.push_back(testValue);
					}
					// stop processing when have target number of primes
					foo = found.size() + orderedLeftPrimes.size();
					if (foo >= targetIndex)
					{
						done = true;
						break;
					}
				}
			}

			orderedLeftPrimes.insert(orderedLeftPrimes.end(), found.begin(), found.end());
			workingSet = found;
			found.clear();
		}

		// write to file
		if (orderedLeftPrimes.size() > stordedLeftPrimes.size())
		{
			std::ofstream output_file(data_file, std::ios::binary);
			std::ostream_iterator<int> output_iterator(output_file, "\n");
			std::copy(orderedLeftPrimes.begin(), orderedLeftPrimes.end(), output_iterator);
			output_file.close();
		}
	}

	high_resolution_clock::time_point endTime = high_resolution_clock::now();
	std::chrono::duration<double> time_span = std::chrono::duration_cast<std::chrono::duration<double>>(endTime - startTime);

	if (false)
	{
		int i = 1;
		for (auto x = orderedLeftPrimes.begin(); x != orderedLeftPrimes.end(); ++x, ++i) {
			std::cout << i << ": " << *x << '\n';
		}

	}

	int size = orderedLeftPrimes.size();
	std::cout << "result : the " << targetIndex << " left truncatable prime is: " << orderedLeftPrimes.at(targetIndex-1) << "\n";
	if (size >= 10) std::cout << "10: " << orderedLeftPrimes[9] << " ref: 47\n";
	if (size >= 100) std::cout << "100: " << orderedLeftPrimes[99] << " ref: 5167\n";
	if (size >= 1000) std::cout << "1000: " << orderedLeftPrimes[999] << " ref: 8391283\n";
	std::cout << orderedLeftPrimes.size() << " Entries\n";
	std::cout << "Last entry = " << orderedLeftPrimes[orderedLeftPrimes.size()-1] << "\n";
	std::cout << "It took me " << time_span.count() << " seconds.";

	

	std::cin.get();
	return 0;
}