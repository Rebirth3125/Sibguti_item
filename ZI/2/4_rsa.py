import sympy

def generate_keypair():
    p = sympy.randprime(100, 1000)
    q = sympy.randprime(100, 1000)
    n = p * q
    phi = (p - 1) * (q - 1)
    e = sympy.randprime(2, phi - 1)
    d = sympy.mod_inverse(e, phi)
    return ((n, e), (n, d))

def encrypt(public_key, plaintext):
    n, e = public_key
    ciphertext = [pow(ord(char), e, n) for char in plaintext]
    return ciphertext

def decrypt(ciphertext, private_key):
    n, d = private_key
    plaintext = ''.join([chr(pow(char, d, n)) for char in ciphertext])
    return plaintext

# Пример использования:
public_key, private_key = generate_keypair()
plaintext = "Hello, World!"
ciphertext = encrypt(public_key, plaintext)
decrypted_text = decrypt(ciphertext, private_key)
print("Original Text: ", plaintext)
print("Decrypted Text: ", decrypted_text)
