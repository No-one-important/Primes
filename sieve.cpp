#include <chrono>
#include <ctime>
#include <iostream>
#include <bitset>
#include <map>
#include <cstring>
#include <cmath>

using namespace std;
using namespace std::chrono;

class BitArray {
    uint32_t *array;
    size_t arrSize;

    inline static size_t arraySize(size_t size) {
        return (size >> 5) + ((size & 0x31) > 0);
    }

    inline static size_t index(size_t n) {
        return (n >> 5);
    }

    inline static uint32_t getSubindex(size_t n, uint32_t d) {
        return d & uint32_t(uint32_t(0x01) << (n % 32));
    }

    inline void setFalseSubindex(size_t n, uint32_t &d) {
        d &= ~uint32_t(uint32_t(0x01) << (n % (8*sizeof(uint32_t))));
    }
public:
    explicit BitArray(size_t size) : arrSize(size) {
        array = new uint32_t[arraySize(size)];
        std::memset(array, 0xFF, (size >> 3) + ((size & 0x07) > 0));
    }

    ~BitArray() {delete [] array;}

    bool get(size_t n) const {
        return getSubindex(n, array[index(n)]);
    }

    static constexpr uint32_t rol(uint32_t x, uint32_t n) {
        return (x<<n) | (x>>(32-n));
    }

    void setFlagsFalse(size_t n, size_t skip) {
        auto rolling_mask = ~uint32_t(1 << n % 32);
        auto roll_bits = skip % 32;
        while (n < arrSize) {
            array[index(n)] &= rolling_mask;
            n += skip;
            rolling_mask = rol(rolling_mask, skip);
        }
    }
};

class prime_sieve
{
    private:

        long sieveSize = 0;
        BitArray Bits;
        static const std::map<const long long, const int> resultsDictionary;

        bool validateResults()
        {
            auto result = resultsDictionary.find(sieveSize);
            if (resultsDictionary.end() == result)
                return false;
            return result->second == countPrimes();
        }

    public:

        prime_sieve(long n): Bits(n), sieveSize(n)
        {
        }

        ~prime_sieve()
        {
        }

        void runSieve()
        {
            int factor = 3;
            int q = (int) sqrt(sieveSize);

            while (factor <= q)
            {
                for (int num = factor; num < sieveSize; num += 2)
                {
                    if (Bits.get(num))
                    {
                        factor = num;
                        break;
                    }
                }
                Bits.setFlagsFalse(factor * factor, factor + factor);

                factor += 2;
            }
        }

        void printResults()
        {
            printf("2\n");

            int count = (sieveSize >= 2);                             // Starting count (2 is prime)
            for (int num = 3; num <= sieveSize; num+=2)
            {
                if (Bits.get(num))
                {
                    printf("%d\n", num);
                    count++;
                }
            }

            printf("\n");
            printf("Count: %d", count);
        }

        int countPrimes()
        {
            int count =  (sieveSize >= 2);;
            for (int i = 3; i < sieveSize; i+=2)
                if (Bits.get(i))
                    count++;
            return count;
        }
};

int main (int argc, char *argv []) {
    if (argc < 2) {
        printf("Usage: %s <sieve size>\n", argv[0]);
        return 1;
    }
    prime_sieve sieve(atoi(argv[1]));
    sieve.runSieve();

    // print sieve output
    sieve.printResults();
}