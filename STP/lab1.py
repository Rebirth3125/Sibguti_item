import random
import numpy as np
import math

def simulate_program_writing(eta):
    vocabulary = [str(i) for i in range(eta)]
    program = []

    program_length = random.randint(1, eta)  

    for _ in range(program_length):
        chosen_word = random.choice(vocabulary)
        program.append(chosen_word)

    return program

def calculate_statistics(eta, num_simulations):
    program_lengths = []

    for _ in range(num_simulations):
        program = simulate_program_writing(eta)
        program_lengths.append(len(program))

    mean_length = np.mean(program_lengths)
    variance = np.var(program_lengths)
    std_deviation = np.sqrt(variance)
    relative_expected_error = std_deviation / mean_length

    return mean_length, variance, std_deviation, relative_expected_error

def calculate_theoretical_values(eta):
    mean_length = 0.9 * eta * math.log2(eta)
    variance = (math.pi**2 * eta**2) / 6
    std_deviation = math.sqrt(variance)
    relative_expected_error = 1 / (2 * math.log2(eta))

    return mean_length, variance, std_deviation, relative_expected_error




eta_values = [16, 32, 64, 128]
num_simulations = 1000

for eta in eta_values:
    print(f"Для η = {eta}:")
    mean_length, variance, std_deviation, relative_expected_error = calculate_statistics(eta, num_simulations)
    print(f"Статистические оценки:")
    print(f"Длина программы: {mean_length:.2f}")
    print(f"Дисперсия длины: {variance:.2f}")
    print(f"Среднеквадратическое отклонение: {std_deviation:.2f}")
    print(f"Относительная погрешность: {relative_expected_error:.2f}")
    
    theoretical_mean, theoretical_variance, theoretical_std_deviation, theoretical_relative_expected_error = calculate_theoretical_values(eta)
    print(f"Теоретические значения:")
    print(f"Длина программы: {theoretical_mean:.2f}")
    print(f"Дисперсия длины: {theoretical_variance:.2f}")
    print(f"Среднеквадратическое отклонение: {theoretical_std_deviation:.2f}")
    print(f"Относительная погрешность: {theoretical_relative_expected_error:.2f}")
    print()


eta = 16
program_words = simulate_program_writing(eta)

program_text = ' '.join(program_words)

program_words = program_text.split()

vocabulary_length = len(set(program_words))
program_length = len(program_words)
predicted_program_length_from_vocabulary = vocabulary_length

print("Длина словаря в тексте программе:", vocabulary_length)
print("Длина программы в тексте программы:", program_length)
print("Рассчитанная длина программы по размеру словаря:", predicted_program_length_from_vocabulary)

eta = 16  
predicted_program_length_from_eta = eta

if predicted_program_length_from_eta == program_length:
    print("Значение eta совпадает с рассчитанной длиной программы по тексту.")
else:
    print("Значение eta не совпадает с рассчитанной длиной программы по тексту.")