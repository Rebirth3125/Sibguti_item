import random
import sympy

def generate_keypair():
    q = sympy.randprime(100, 1000)
    alpha = random.randint(2, q - 1)
    private_key = random.randint(2, q - 1)
    beta = pow(alpha, private_key, q)
    return [q, alpha, private_key, beta]

def is_prime_fermat(number_to_check, k=5):
    if number_to_check[0] <= 1:
        return False
    if number_to_check[0] <= 3:
        return True

    for _ in range(k):
        a = random.randint(2, number_to_check[0] - 2)
        if pow(a, number_to_check[0] - 1, number_to_check[0]) != 1:
            return False
    return True

def encrypt(public_key, plaintext):
    q, alpha, beta = public_key[:3]
    k = random.randint(2, q - 2)
    c1 = pow(alpha, k, q)
    c2 = [(ord(char) * pow(beta, k, q)) % q for char in plaintext]
    return (c1, c2)

def decrypt(ciphertext, private_key, public_key):
    q, _, _, _ = public_key
    c1, c2 = ciphertext
    s = pow(c1, private_key, q)
    plaintext = ''.join([chr((char * sympy.mod_inverse(s, q)) % q) for char in c2])
    return plaintext

# Пример использования:
public_key = generate_keypair()
private_key = public_key[2] 
plaintext = "Hello, World!"
ciphertext = encrypt(public_key, plaintext)
decrypted_text = decrypt(ciphertext, private_key, public_key)
print("Original Text: ", plaintext)
print("Decrypted Text: ", decrypted_text)