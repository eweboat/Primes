#pragma once
#include <vector>

namespace Primes
{

// return true iff number is prime
bool IsPrime(int number);

// populate list with n left truncatable primes
void FindLeftTruncatablePrimes(std::vector<unsigned int>& orderedLeftPrimes, unsigned int targetIndex);

}