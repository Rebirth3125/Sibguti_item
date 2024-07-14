import os
import random

def generate_one_time_pad(length):
    return bytes([random.randint(0, 255) for _ in range(length)])

def encrypt(plaintext, one_time_pad):
    ciphertext = [char ^ pad_char for char, pad_char in zip(plaintext, one_time_pad)]
    return bytes(ciphertext)

def decrypt(ciphertext, one_time_pad):
    plaintext = [char ^ pad_char for char, pad_char in zip(ciphertext, one_time_pad)]
    return bytes(plaintext)

# Пример использования:
plaintext = b"Hello, World!"
one_time_pad = generate_one_time_pad(len(plaintext))
ciphertext = encrypt(plaintext, one_time_pad)
decrypted_text = decrypt(ciphertext, one_time_pad)
print("Original Text: ", plaintext)
print("Decrypted Text: ", decrypted_text)
