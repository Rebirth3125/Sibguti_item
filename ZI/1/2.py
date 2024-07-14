import random
import math

def alg2(a, b):
    print(f"({a}, {b})")
    if b > a:
        a,b = b,a                                                                                                                                                                                                                                                                                                                                                                                                                   
    if b == 0:
        return a, 1, 0
    
    nod, x1, y1 = alg2(b, a % b)
    print(f"{nod}, {x1}, {y1}")
    x = y1
    y = x1 - (a // b) * y1
    
    return nod, x, y


a = random.randint(100,1000000)
b = random.randint(100,1000000)
numbers_to_check = [a, b]

def test_ferma(p, k):
    if p == 2:
        return True
    if p % 2 == 1:
        return False
    for i in range(k):
        numbers_to_check = random.randint(1, p - 1)
        if math.gcd(numbers_to_check, p) != 1 or pow(numbers_to_check, p - 1, p) != 1:
            return False
    return True

nod, x, y = alg2(a, b)

print(f"nod({a}, {b}) = {nod}")
print(f"{a} * {x} + {b} * {y} = {nod}")

