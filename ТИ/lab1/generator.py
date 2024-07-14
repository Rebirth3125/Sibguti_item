# import random

# def generate_random_string(length):
#     characters = ['a', 'b', 'c']
#     return ''.join(random.choice(characters) for _ in range(length))

# def generate_file(filename, size_kb):
#     size_bytes = size_kb * 1024
#     with open(filename, 'w') as file:
#         while file.tell() < size_bytes:
#             random_string = generate_random_string(1)
#             file.write(random_string)

# filename = '1'
# size_kb = 20

# generate_file(filename, size_kb)


import random

def generate_file_with_probabilities(filename, size_kb, probabilities):
    size_bytes = size_kb * 1024
    characters = ['a', 'b', 'c']

    with open(filename, 'w') as file:
        while file.tell() < size_bytes:
            random_string = random.choices(characters, probabilities, k=1)[0]
            file.write(random_string)

filename = '2'
size_kb = 20
character_probabilities = [0.3, 0.4, 0.3]  

generate_file_with_probabilities(filename, size_kb, character_probabilities)