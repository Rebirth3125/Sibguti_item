import random
import sympy

def is_prime_fermat(p, k=5):
    if p <= 1:
        return False
    if p <= 3:
        return True

    for _ in range(k):
        a = random.randint(2, p - 2)
        if pow(a, p - 1, p) != 1:
            return False
    return True

def generate_keypair(p):
    if not is_prime_fermat(p):
        raise ValueError("p is not a prime number")
    
    q = sympy.randprime(2, p - 1)
    while True:
        a = random.randint(2, p - 1)
        if sympy.gcd(a, p - 1) == 1:
            break
    b = pow(a, q, p)
    public_key = (p, q, b)
    private_key = a
    return public_key, private_key

def encrypt(public_key, plaintext):
    p, q, b = public_key
    r = random.randint(2, p - 1)
    c1 = pow(b, r, p)
    c2 = [(ord(char) * pow(q, r, p)) % p for char in plaintext]
    return (c1, c2)

def decrypt(ciphertext, private_key, public_key):
    c1, c2 = ciphertext
    p, _, _ = public_key
    a = private_key
    q = sympy.mod_inverse(a, p - 1)
    plaintext = ''.join([chr((char * q) % p) for char in c2])
    return plaintext

# Пример использования:
p = 997

if is_prime_fermat(p):
    public_key, private_key = generate_keypair(p)
    plaintext = "Hello, World!"
    ciphertext = encrypt(public_key, plaintext)
    decrypted_text = decrypt(ciphertext, private_key, public_key)
    print("Original Text: ", plaintext)
    print("Decrypted Text: ", decrypted_text)
else:
    print("p is not a prime number")
