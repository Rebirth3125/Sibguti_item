import random

def fast_modular_exponentiation(base, exponent, modulus):
    result = 1
    base = base % modulus  

    while exponent > 0:
        if exponent % 2 == 1:
            result = (result * base) % modulus
        exponent = exponent // 2
        base = (base * base) % modulus
    
    return result

base = random.randint(100,1000000)
exponent = random.randint(100,1000000)
modulus = random.randint(100,1000000)

result = fast_modular_exponentiation(base, exponent, modulus)
print(f"{base}^{exponent} % {modulus} = {result}")
