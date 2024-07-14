import numpy as np

def generate_random_matrix(n, m):
    # Генерируем случайную порождающую матрицу размером n x m
    return np.random.randint(2, size=(n, m))

def main():
    n = int(input("Введите количество строк: "))
    m = int(input("Введите количество столбцов: "))

    matr = generate_random_matrix(n, m)

    print(f"Сгенерированная порождающая матрица {n} на {m}:")
    for row in matr:
        print(row)

    print("Размерность кода: ", n)
    print("Количество кодовых слов: ", pow(2, n))

    count = m + 1
    for code in range(len(matr)):
        for i in range(code + 1, len(matr)):
            cou = 0
            for j in range(len(matr[i])):
                if matr[code][j] != matr[i][j]:
                    cou += 1
            if cou < count:
                count = cou

    print("Минимальное кодовое расстояние: ", count)

if __name__ == "__main__":
    main()
