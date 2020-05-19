#include <stdlib.h>
#include <stdio.h>
using namespace std;

bool isCoprime(const int, const int);

int main (int argc, char *argv[])
{
    if (argc != 2) {
        printf("Usage ./coprime n; where n is the number of coprimes you want to find\n");
        return 0;
    }
    int n = atoi(argv[1]);
    int count = 0;
    for (int i = 2; ; i++) {
        for (int j = i; j < i+10; j++) {
            // if both are even then we know they aren't coprime
            if (i % 2 != 0 || j % 2 != 0) {
                if (isCoprime(i, j))
                    count++;
                if (count >= n) {
                    printf("Found %i coprimes\n", count);
                    return 0;
                }
            }
        }
    }
    return 0;
}

bool isCoprime(const int x, const int y)
{
    int a = x;
    int b = y;
    int remainder = 1;
    if (a == b)
        return false;
    if (a > b) {
        while (remainder != 0) {
            remainder = a % b;
            a = b;
            b = remainder;
            if (remainder == 1) {
                printf("integers %i and %i are coprimes\n", x, y);
                return true;
            }
        }
    } else if (a < b) {
        while (remainder != 0) {
            remainder = b % a;
            b = a;
            a = remainder;
            if (remainder == 1) {
                printf("integers %i and %i are coprimes\n", x, y);
                return true;
            }
        }
    }
    return false;
}
