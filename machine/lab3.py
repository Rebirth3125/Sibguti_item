import os
import pandas as pd
from sklearn import preprocessing
from sklearn.linear_model import Lars
from sklearn.model_selection import train_test_split

def measure_model_accuracy(data, test_size):
    data_X = preprocessing.normalize(data[:, :-1])
    data_Y = data[:, -1]
    train_x, test_x, train_y, test_y = train_test_split(data_X, data_Y, test_size=test_size, stratify=data_Y)
    lars_regression = Lars()
    lars_regression.fit(train_x, train_y)
    predicted = lars_regression.predict(test_x)
    success = 0
    for i in range(len(test_x)):
        if abs(test_y[i] - predicted[i]) < 1:
            success += 1
    return success / len(test_x) * 100

def main():
    file_name = os.path.dirname(__file__) + "//data//winequalityN.csv"
    test_size = 0.3
    n = 10

    data = pd.read_csv(file_name, header=0).fillna(0)
    data.loc[data.type == 'white', 'type'] = 0
    data.loc[data.type == 'red', 'type'] = 1

    data = data.to_numpy()
    print(f"Общий размер данных: {len(data)}\n{'~' * 10}")

    print(f'Все вина:')
    total = 0
    for i in range(n):
        acc = measure_model_accuracy(data, test_size)
        print(f"{i + 1:>2}. Точность: {acc:.6f}%")
        total += acc
    print(f'Средняя точность: {total / n:.6f}% за {n} проходов\n\n')

    print(f'Белые вина:')
    white = data[data[:, 0] == 0]
    total = 0
    for i in range(n):
        acc = measure_model_accuracy(white, test_size)
        print(f"{i + 1:>2}. Точность: {acc:.6f}%")
        total += acc
    print(f'Средняя точность: {total / n:.6f}% за {n} проходов\n\n')

    print(f'Красные вина:')
    red = data[data[:, 0] == 1]
    total = 0
    for i in range(n):
        acc = measure_model_accuracy(red, test_size)
        print(f"{i + 1:>2}. Точность: {acc:.6f}%")
        total += acc
    print(f'Средняя точность: {total / n:.6f}% за {n} проходов')

if __name__ == "__main__":
    main()