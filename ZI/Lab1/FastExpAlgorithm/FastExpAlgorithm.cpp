#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Для самопроверки: https://planetcalc.ru/8979/

//===----------------- - - - -  -  -  -  -   -   -   -
//  Алгоритм быстрого возведения в степень по модулю
//===----------------- - - - -  -  -  -  -   -   -   -

/* Несколько этапов:
    1. Инициализируем случайными значениями формулу
    2. Берем целую часть log2 от степени(t - полученная целая часть)
    3. Строим ряд до 2^t и берем модуль(m) у каждого элемента
        a^(2^0)%m  a^(2^1)%m  a^(2^2)%m  a^(2^3)%m . . . a^(2^t)%m

        а^(2^0)%m = a % m
        a^(2^1)%m = (a % m) * (a % m)
        a^(2^2)%m = (a^(2^1) % m) * (a^(2^1) % m)
        и так до a^(2^t)%m включительно

    4. Переводим степень в двоичную систему(expB)
    5. Перемножаем все значения рядов(a^64, a^32, a^16 . . . ), где совпадает 1 в разряде у expB
    6. У результата из 5 пункта берем mod m

    Пример(из "Криптограф. методы защиты инф-ии", Рябко, Фионов, с.13-14):

        1. 3^100 % 7
        2. log2(100) = 6(следовательно, ряд до 2^6 (т.е. до 64) )
        3.
            a % m |  a^2 % m  |  a^4 % m   |   a^8 % m   |  a^16 % m  |  a^32 % m  |  a^64 % m
              3     (3*3)%7=2   (2*2)%7=4     (4*4)%7=2    (2*2)%7=4     (4*4)%7=2    (2*2)%7=4
              |      ^ ^    |    ^ ^    |      ^ ^    |     ^ ^    |      ^ ^    |     ^ ^
              |      | |    |    | |    |      | |    |     | |    |      | |    |     | |
              ----------    --------    ----------    ---------    ----------    ---------
        4. 100 = 1100100(2)
        5. a^64(=4)     a^32(=2)    a^16    a^8     a^4(=4)     a^2     a
            *              *                           *
            1              1         0       0         1         0      0
            =              =                           =
            4              2                           4
        Получили 4, 2 и 4. Перемножаем их между собой:
            4 * 2 * 4 = 32
        6. 32 % 7 = 4

        Ответ: 3^100 % 7 = 4
*/
typedef struct {
    long int base;
    long int exponent;
    long int module;
} formula;

formula get_random_formula() {
    formula f;
    f.base = rand() % (RAND_MAX) + 1;
    f.exponent = rand() % (RAND_MAX) + 1;
    f.module = rand() % (RAND_MAX) + 1;
    return f;
}

int main() {
    srand(time(NULL));
    long int row;

    long long int compisition = 1;
    formula f = get_random_formula();  // (1)
    printf("base: %ld\nexponent: %ld\nmodule: %ld\n",
           f.base,
           f.exponent,
           f.module);

    long int log = floor(log2(f.exponent));  // (2)

    for (int i = 0; i <= log; i++) {
        if (i == 0) {
            row = f.base % f.module;  // (3)
        } else {
            row = (row * row) % f.module;  // (3)
        }

        if ((f.exponent >> i) & 1) {                       // Если совпадает единичка в разряде
            compisition = (compisition * row) % f.module;  // (5)
        }
    }
    printf("res: %lld\n", compisition);
    return 0;
}