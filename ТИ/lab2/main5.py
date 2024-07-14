import numpy as np
import random

def generate_random_bits(filename, num_bits):
    bits = ''.join(random.choice(['0', '1']) for _ in range(num_bits))
    write_binary_file(filename, bits)

def read_binary_file(filename):
    with open(filename, 'rb') as file:
        content = file.read()
    bits = ''.join(format(byte, '08b') for byte in content)
    return bits

bits = read_binary_file('random_bits.txt')
print(bits)


def write_binary_file(filename, bits):
    bytes_array = bytearray(int(bits[i:i+8], 2) for i in range(0, len(bits), 8))
    with open(filename, 'wb') as file:
        file.write(bytes_array)

def hamming_encode(bits):
    n = 15
    k = 11
    encoded_bits = []
    
    for i in range(0, len(bits), k):
        chunk = bits[i:i+k]
        
        if len(chunk) < k:
            chunk += '0' * (k - len(chunk))
        
        p1 = str(int(chunk[0]) ^ int(chunk[2]) ^ int(chunk[4]) ^ int(chunk[6]) ^ int(chunk[8]) ^ int(chunk[10]) ^ int(chunk[12])) if len(chunk) > 12 else '0'
        p2 = str(int(chunk[1]) ^ int(chunk[2]) ^ int(chunk[5]) ^ int(chunk[6]) ^ int(chunk[9]) ^ int(chunk[10]) ^ int(chunk[13])) if len(chunk) > 13 else '0'
        p3 = str(int(chunk[3]) ^ int(chunk[4]) ^ int(chunk[5]) ^ int(chunk[6]) ^ int(chunk[11]) ^ int(chunk[12]) ^ int(chunk[13])) if len(chunk) > 13 else '0'
        
        encoded_chunk = p1 + p2 + chunk[0] + p3 + chunk[1:11]
        encoded_bits.append(encoded_chunk)
    
    return ''.join(encoded_bits)

def hamming_decode(bits):
    n = 15
    k = 11
    decoded_bits = []
    
    for i in range(0, len(bits), n):
        chunk = bits[i:i+n]
        
        p1 = str(int(chunk[0]) ^ int(chunk[2]) ^ int(chunk[4]) ^ int(chunk[6]) ^ int(chunk[8]) ^ int(chunk[10]) ^ int(chunk[12])) if len(chunk) > 12 else '0'
        p2 = str(int(chunk[1]) ^ int(chunk[2]) ^ int(chunk[5]) ^ int(chunk[6]) ^ int(chunk[9]) ^ int(chunk[10]) ^ int(chunk[13])) if len(chunk) > 13 else '0'
        p3 = str(int(chunk[3]) ^ int(chunk[4]) ^ int(chunk[5]) ^ int(chunk[6]) ^ int(chunk[11]) ^ int(chunk[12]) ^ int(chunk[13])) if len(chunk) > 13 else '0'
        
        error_position = int(p3 + p2 + p1, 2)
        
        if error_position != 0:
            chunk = list(chunk)
            chunk[error_position - 1] = '1' if chunk[error_position - 1] == '0' else '0'
            chunk = ''.join(chunk)
        
        decoded_chunk = chunk[2] + chunk[4:7] + chunk[8:12]
        decoded_bits.append(decoded_chunk)
    
    return ''.join(decoded_bits)


def simulate_channel_noise(bits, error_prob):
    noisy_bits = ''
    for bit in bits:
        if np.random.random() < error_prob:
            bit = '0' if bit == '1' else '1'
        noisy_bits += bit
    return noisy_bits

def main():
    num_bits = 1000  
    generate_random_bits('random_bits.txt', num_bits)
    
    bits = read_binary_file('random_bits.txt')
    
    error_probs = [0.0001, 0.001, 0.01, 0.1]
    
    for error_prob in error_probs:
        encoded_bits = hamming_encode(bits)
        noisy_bits = simulate_channel_noise(encoded_bits, error_prob)
        decoded_bits = hamming_decode(noisy_bits)
        
        error_count = sum(1 for x, y in zip(bits, decoded_bits) if x != y)
        print(f"Вероятность ошибки p = {error_prob}: Количество ошибок после декодирования: {error_count}")

if __name__ == "__main__":
    main()