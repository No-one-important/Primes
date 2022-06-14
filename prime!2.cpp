#include <iostream>
#include <cmath>

#define limit 1000000

bool isPrime(int n) {
    if (n == 1) return false;
    if (n == 2) return true;
    //if (n % 2 == 0) return false;
    for (int i = 3; i <= sqrt(n); i += 2) {
        if (n % i == 0) return false;
    }
    return true;
}

int main() {
    int count = 0;
    for (int i = 1; i <= limit; i++) {
        if (isPrime(i)) {
            count++;
            // std::cout << i << std::endl;
        }
    }

    std::cout << "Count: " << count << std::endl;
}