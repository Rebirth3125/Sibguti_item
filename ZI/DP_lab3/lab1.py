
import random

from math import sqrt, ceil

def is_prime(n):    
    for x in range(1, 5):
        if pow(random.randrange(n - 1) + 1, (n - 1), n) != 1:
            return False
    return True

def get_prime(left, right):
    while True:
        p = random.randint(left, right)
        if is_prime(p):
            return p

def pow_mod(base, exp, module):
    result = 1
    base %= module

    if base == 0:    
        return 0

    while exp > 0:
        if exp & 1 == 1:
            result = (result * base) % module
        exp >>= 1
        base = (base ** 2) % module

    return result

def extended_euclidean_algorithm(a, b):
    u = [a, 1, 0]
    v = [b, 0, 1]

    while v[0] != 0:
        q = u[0] // v[0]
        t = [u[0] % v[0], u[1] - q * v[1], u[2] - q * v[2]]
        u, v = v, t
    return u

def diffie_hellman_algorithm():
    q, p, g, Xa, Xb = 0, 0, 0, 0, 0

    print("Generated:")
    while True:
        q = get_prime(0, 10 ** 9)
        p = 2 * q + 1
        if is_prime(p):      
            for i in range (1, p - 1):
                if pow_mod(i, q, p) != 1:
                    g = i
                    break
    print(f"q = {q}\np = {p}")

    while pow_mod(g, q, p) != 1:
        g = random.randint(1, p - 1)
    print(f"g = {g}\n")

    Xa = get_prime(1, p)
    Xb = get_prime(1, p)
    print(f"Secret keys:\nXa = {Xa}\nXb = {Xb}\n")

    Ya = pow_mod(g, Xa, p)
    Yb = pow_mod(g, Xb, p)
    print(f"Open Keys:\nYa = {Ya}\nYb = {Yb}")

    Zab = pow_mod(Yb, Xa, p)
    Zba = pow_mod(Ya, Xb, p)
    print(f"Zab = {Zab}\nZba = {Zba}\n")
    return Zab, Zba

def giant_baby_step(a, p, y):

    # y = a ^ x mod p

    k = m = ceil(sqrt(p + 1))

    baby = {pow_mod(a, j, p) * y % p: j for j in range(m)}
    giant = [pow_mod(a, (m * i), p) for i in range(1, k + 1)]

    for i, el in enumerate(giant, 1):
        if (j := baby.get(el)) is not None:
            return i * m - j

    return None

if __name__ == "__main__":
    a = get_prime(10 ** 3, 10 ** 4)
    x = get_prime(10 ** 5, 10 ** 7)
    p = get_prime(10 ** 5, 10 ** 7)
    print(f"a = {a}; x = {x}; p = {p}")
    print(y := pow_mod(a, x, p))

    print(giant_baby_step(a, p, y))

    print('')

    a = get_prime(10 ** 5, 10 ** 6)
    b = get_prime(10 ** 5, 10 ** 6)
    print(f"a = {a}, b = {b}")
    print('saad', extended_euclidean_algorithm(5,22), '\n')

    print("euclidean(123456, 765432) = ", extended_euclidean_algorithm(123456, 765432), '\n')