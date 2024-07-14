import random
import math
p = random.randint(10,100)
g = 5 

numbers_to_check = [p, g]

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

def generate_keys():
    
    a_secret = random.randint(1, p - 1)  
    a_partial_key = (g ** a_secret) % p  
    print(a_partial_key)
    
    b_secret = random.randint(1, p - 1)  
    b_partial_key = (g ** b_secret) % p   
    print(b_partial_key)

    swaped_key_A = (b_partial_key ** a_secret) % p
    swaped_key_B = (a_partial_key ** b_secret) % p
    
    return swaped_key_A, swaped_key_B


key_subscriber_A, key_subscriber_B = generate_keys()

print("Ключ абонента A:", key_subscriber_A)
print("Ключ абонента B:", key_subscriber_B)
