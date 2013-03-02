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
	using std::chrono::high_resolution_clock;
	high_resolution_clock::time_point startTime = high_resolution_clock::now();

	// vector to hold primes
	std::vector<int> orderLeftPrimes;

	// read from file if present
	std::ifstream is(data_file);
	if (is.good())
	{
		std::istream_iterator<int> start(is), end;
		orderLeftPrimes = std::vector<int>(start, end);
	}
	else
	{
		orderLeftPrimes.reserve(OperationLimits::MaxQueryArg);
		orderLeftPrimes.push_back(2);
		orderLeftPrimes.push_back(3);
		orderLeftPrimes.push_back(5);
		orderLeftPrimes.push_back(7);

		std::vector<int> found;
		std::vector<int> workingSet(orderLeftPrimes);

		int base = 1;
		int foo = 0;
		int bar = 0;
		for (int power10 = 1; power10 < 9; ++power10)
		{
			base *= 10;
			for(int i = 1; i < 10; ++i)
			{
				int ibase = base * i;
				for (auto x : workingSet) 
				{
					//std::cout << x + base << '\n';
					foo = x + ibase;
					bar = foo % 10;
					if ((bar == 3 || bar == 7) && IsPrime(foo))
					{
						found.push_back(x + ibase);
					}
				}
			}
			orderLeftPrimes.insert(orderLeftPrimes.end(), found.begin(), found.end());
			workingSet = found;
			found.clear();
			std::cout << "WORKINGSET SIZE = " << workingSet.size() << "\n";
		}

		// write to file
		std::ofstream output_file(data_file, std::ios::binary);
		std::ostream_iterator<int> output_iterator(output_file, "\n");
		std::copy(orderLeftPrimes.begin(), orderLeftPrimes.end(), output_iterator);
	}

	high_resolution_clock::time_point endTime = high_resolution_clock::now();
	std::chrono::duration<double> time_span = std::chrono::duration_cast<std::chrono::duration<double>>(endTime - startTime);

	if (false)
	{
		int i = 1;
		for (auto x = orderLeftPrimes.begin(); x != orderLeftPrimes.end(); ++x, ++i) {
			std::cout << i << ": " << *x << '\n';
		}

	}

	int size = orderLeftPrimes.size();
	if (size > 10) std::cout << "10: " << orderLeftPrimes[9] << " ref: 47\n";
	if (size > 100) std::cout << "100: " << orderLeftPrimes[99] << " ref: 5167\n";
	if (size > 1000) std::cout << "1000: " << orderLeftPrimes[999] << " ref: 8391283\n";
	std::cout << orderLeftPrimes.size() << " Entries\n";
	std::cout << "Last entry = " << orderLeftPrimes[orderLeftPrimes.size()-1] << "\n";
	std::cout << "It took me " << time_span.count() << " seconds.";

	

	std::cin.get();
	return 0;
}