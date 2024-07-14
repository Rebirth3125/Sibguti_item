#include <iostream>
#include <map>
#include <math.h>
#include <time.h>
#include <vector>
#include "Lab1/Lab1algs.hpp"

// Описание найдете у Рябко и Фионова.

formula get_random_formula_Sh() { // Не используем функцию из библиотеки, 
                                  // так как она не генирирует взаимно простые числа
    formula f;
    f.base = rand() % (RAND_MAX) + 1;
    f.exponent = rand() % (RAND_MAX) + 1;
    f.module = rand() % (RAND_MAX) + 1;

    while (!isPrime(f.base)) {
        f.base = rand() % (RAND_MAX) + 1;
    }

    while (!isPrime(f.module)) {
        f.module = rand() % (RAND_MAX) + 1;
    }

    while (f.exponent >= f.module) {
        f.exponent = rand() % (RAND_MAX) + 1;
    }

    return f;
}

int main() {
    formula f;
    srand(time(NULL));
    f = get_random_formula_Sh();
    long int ans = compute(f.base, f.exponent, f.module);
    std::vector<int> solv = solve(f.base, ans, f.module);
    std::cout << "base=" << f.base << "\n" << "mod=" << f.module << "\n" << "ans=" << ans << "\n";
    for(size_t i = 0; i < solv.size(); i++) {
        std::cout << "solv=" << solv[i] << "\n"; // Найденный x
        ans = compute(f.base, solv[i], f.module);
        std::cout << "new ans=" << ans << "\n"; // Должно ans==y
    }
    return 0;
}