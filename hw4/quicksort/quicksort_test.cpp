#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include "quicksort.h"

using namespace std;


int main() {
    srand(time(NULL));
    vector<int> test;
    for (int i =0; i < 100000; i++) {
        test.push_back(rand() % 1000);
    }

    quicksort(test, std::greater<int> {});

    for (int i = 0; i < 100000; i++) {
        cout << test[i] << " ";
    }
    cout << endl;

    return 0;
}
