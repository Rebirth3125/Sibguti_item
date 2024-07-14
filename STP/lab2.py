
import random
import numpy as np
import math


def find_min_element(arr):
    min_value = min(arr)
    min_index = arr.index(min_value)
    return min_value, min_index


def bubble_sort(arr):
    n = len(arr)
    for i in range(n):
        for j in range(0, n - i - 1):
            if arr[j] > arr[j + 1]:
                arr[j], arr[j + 1] = arr[j + 1], arr[j]


def binary_search(arr, target):
    left, right = 0, len(arr) - 1
    while left <= right:
        mid = left + (right - left) // 2
        if arr[mid] == target:
            return mid
        elif arr[mid] < target:
            left = mid + 1
        else:
            right = mid - 1
    return -1


def find_min_element_2D(arr):
    min_value = float('inf')
    min_row, min_col = -1, -1
    for i in range(len(arr)):
        for j in range(len(arr[i])):
            if arr[i][j] < min_value:
                min_value = arr[i][j]
                min_row, min_col = i, j
    return min_value, min_row, min_col


def reverse_array(arr):
    return arr[::-1]


def rotate_array_left(arr, k):
    k = k % len(arr)
    return arr[k:] + arr[:k]


def replace_element(arr, old_value, new_value):
    return [new_value if x == old_value else x for x in arr]

arr1 = [5, 2, 9, 1, 5, 6]
min_value, min_index = find_min_element(arr1)
print("Минимальный элемент:", min_value)
print("Индекс минимального элемента:", min_index)

bubble_sort(arr1)
print("Отсортированный массив:", arr1)

arr2 = [1, 2, 3, 4, 5, 6, 7, 8, 9]
target_element = 5
index = binary_search(arr2, target_element)
print("Индекс элемента", target_element, "в массиве:", index)



subprogram_metrics = [
    {"eta_prime": 13, "eta1": 4, "eta2": 9, "N1": 7, "N2": 9, "N": 16}

]


subprogram_metrics_c = [
    {"eta_prime":16, "eta1": 4, "eta2": 10, "N1": 5, "N2": 10, "N": 15}
   
]

sum_eta_prime = sum(sub["eta_prime"] for sub in subprogram_metrics)
sum_eta1 = sum(sub["eta1"] for sub in subprogram_metrics)
sum_eta2 = sum(sub["eta2"] for sub in subprogram_metrics)
sum_N1 = sum(sub["N1"] for sub in subprogram_metrics)
sum_N2 = sum(sub["N2"] for sub in subprogram_metrics)
sum_N = sum(sub["N"] for sub in subprogram_metrics)

N_prime = sum_eta1 * math.log2(sum_eta1)
V_prime = (2 + sum_eta_prime) * math.log2(2 + sum_eta_prime)
V = sum_N * math.log2(sum_eta1)
L = V_prime / V
L_prime = (2 / sum_eta1) * (sum_eta2 / sum_N2)
I = (2 / sum_eta1) * (sum_eta2 / sum_N2) * (sum_N1 + sum_N2) * math.log2(sum_eta1 + sum_eta2)

S = 1 


T1 = V_prime / S
T2 = N_prime / S
T3 = I / S

print(f"N': {N_prime}")
print(f"V': {V_prime}")
print(f"V: {V}")
print(f"L: {L}")
print(f"I: {I}")
print(f"T1: {T1}")
print(f"T2: {T2}")
print(f"T3: {T3}")



lambda1 = L_prime* V
lambda2 = V_prime /V
print(f"lambda1: {lambda1}")
print(f"lambda2: {lambda2}")



sum_eta_prime = sum(sub["eta_prime"] for sub in subprogram_metrics_c)
sum_eta1 = sum(sub["eta1"] for sub in subprogram_metrics_c)
sum_eta2 = sum(sub["eta2"] for sub in subprogram_metrics_c)
sum_N1 = sum(sub["N1"] for sub in subprogram_metrics_c)
sum_N2 = sum(sub["N2"] for sub in subprogram_metrics_c)
sum_N = sum(sub["N"] for sub in subprogram_metrics_c)



N_prime = sum_eta1 * math.log2(sum_eta1)
V_prime = (2 + sum_eta_prime) * math.log2(2 + sum_eta_prime)
V = sum_N * math.log2(sum_eta1)
L = V_prime / V
L_prime = (2 / sum_eta1) * (sum_eta2 / sum_N2)
I = (2 / sum_eta1) * (sum_eta2 / sum_N2) * (sum_N1 + sum_N2) * math.log2(sum_eta1 + sum_eta2)

S = 1 


T1 = V_prime / S
T2 = N_prime / S
T3 = I / S
print("Метрики для с++:")

print(f"N': {N_prime}")
print(f"V': {V_prime}")
print(f"V: {V}")
print(f"L: {L}")
print(f"I: {I}")
print(f"T1: {T1}")
print(f"T2: {T2}")
print(f"T3: {T3}")






lambda1 = L_prime* V
lambda2 = V_prime /V
print(f"lambda1: {lambda1}")
print(f"lambda2: {lambda2}")
