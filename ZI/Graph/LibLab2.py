import hashlib as hs
from LibLab1 import PowMod
from LibLab1 import IsPrime
from LibLab1 import Evklid
import tkinter.filedialog as fd
import random


def write_file(string, message):
    f2 = open(string, 'wb+')
    f2.write(message)
    f2.close()


def read_file():
    file_name = fd.askopenfilename(filetypes=(("TXT files", "*.txt"),
                                              ("All files", "*.*")))
    with open(file_name, 'rb') as f:
        fb = f.read()
        print(fb)
        message = int.from_bytes(fb, byteorder='big')
        mas_ar = list(fb)
        for j in mas_ar:
            print(j)

    print('_____________________')
    f.close()
    return message, mas_ar


def cipherShamir():
    step = pow(10, 3)
    message, mas_ar = read_file()
    print('сообщение разбитое в список')
    print(message)
    flag = True
    p = random.randint(0, step)
    while flag == True:
        if IsPrime(p) == True:
            flag = False
        else:
            p = random.randint(0, step)
            IsPrime(p)
    print(f"message = {message}")
    print(f"P = {p}")
    Ca = random.randint(0, step)
    Da = random.randint(0, step)
    CaDa = Ca * Da

    while PowMod(CaDa, 1, p-1) != 1:
        Ca = random.randint(0, step)
        Da = random.randint(0, step)
        CaDa = Ca * Da
    print(f"CaDa = {CaDa}")

    Cb = random.randint(0, step)
    Db = random.randint(0, step)
    CbDb = Cb * Db

    while PowMod(CbDb, 1, p - 1) != 1:
        Cb = random.randint(0, step)
        Db = random.randint(0, step)
        CbDb = Cb * Db
    print(f"CbDb = {CbDb}")
    if(message < p):
        new_message = message.to_bytes((message.bit_length() + 7)//8, 'big')
        print(f"message<p, Сообщение m передано сразу, message={new_message}")
        write_file("close1.txt", new_message)

    else:
        if(message >= p):
            str = b''
            for j in range(len(mas_ar)):
                print(f"mas_ar = {mas_ar[j]}")
                x1 = PowMod(mas_ar[j], Ca, p)
                print(f"X1 = {x1}")
                x2 = PowMod(x1, Cb, p)
                print(f"X2 = {x2}")
                x3 = PowMod(x2, Da, p)
                print(f"X3 = {x3}")
                x4 = PowMod(x3, Db, p)
                print(f"X4 = {x4}")
                if(x4 == mas_ar[j]):
                    new_message2 = x4.to_bytes(
                        (x4.bit_length() + 7) // 8, 'big')
                    print(f"Сообщение m = {new_message2} доставлено")
                    str += new_message2

                else:
                    print("Перезапуск алгоритма")
                    cipherShamir()
            write_file("close1.txt", str)


def el_gamal():
    flag = True
    q = random.randint(0, pow(10, 9))
    p = 2 * q + 1
    while flag == True:
        if IsPrime(q) == True:
            if IsPrime(p) == True:
                flag = False
            else:
                q = random.randint(0, pow(10, 9))
                p = 2 * q + 1
                IsPrime(q)
        else:
            q = random.randint(0, pow(10, 9))
            p = 2 * q + 1
            IsPrime(q)

    g = random.randint(1, p - 1)
    while PowMod(g, q, p) == 1:
        g = random.randint(2, p - 2)
    print(f"G = {g}")

    message, mas_ar = read_file()
    Cb = random.randint(2, p - 2)
    Db = PowMod(g, Cb, p)
    str = b''
    for i in range(len(mas_ar)):
        k = random.randint(1, p - 2)
        r = PowMod(g, k, p)
        e = mas_ar[i] * PowMod(Db, k, p) % p
        mx = e * PowMod(r, p - 1 - Cb, p) % p
        new_message2 = mx.to_bytes((mx.bit_length() + 7) // 8, 'big')
        print(f"Сообщение m = {new_message2} доставлено")
        str += new_message2
    write_file("close2.txt", str)


def Vernam():
    message, mas_ar = read_file()
    print(f"message = {message}")
    print("___________________________")
    str = b''
    for j in mas_ar:
        print(j)
        temp = bin(j)
        text = list(temp)

        print(f"Элемент mas_ar = {j}")
        text.pop(0)
        text.pop(0)
        element = ""
        for i in text:
            element += i
        print(f"elementB ={element}")

        key = random.randint(0, 256)
        print(f"key = {key}")
        temp = bin(key)
        key = list(temp)
        key.pop(0)
        key.pop(0)
        key1 = ""
        for i in key:
            key1 += i
        print(f"key1 = {key1}")

        print(f"Глубина символа {len(text)}, глубина ключа {len(key)}")
        # int() takes first argument as string
        intSum = int(element, 2) + int(key1, 2)
        result = bin(intSum)[2:]  # remove 0b
        print(f"Закодирвали {result}")
        # int() takes first argument as string
        intSum = int(result, 2) - int(key1, 2)
        result = bin(intSum)[2:]  # remove 0b
        print(f"Decod = {result}")
        result = int(result, base=2)

        if result == j:
            print(f"Сообщение передано {result}")
            new_message = result.to_bytes(
                (result.bit_length() + 7) // 8, 'big')
            str += new_message
    print("___________________________")
    write_file("close3.txt", str)


def Key_to_RSA():
    p = random.randint(0, pow(10, 3))
    q = random.randint(0, pow(10, 3))
    flag = True
    while flag == True:
        if IsPrime(p) == True:
            if IsPrime(q):
                flag = False
            else:
                p = random.randint(0, pow(10, 3))
                q = random.randint(0, pow(10, 3))
        else:
            p = random.randint(0, pow(10, 3))
            q = random.randint(0, pow(10, 3))
    N = p*q
    #print(f"P = {p}, Q = {q}, N = {N}")
    f = (p-1)*(q-1)
    #print(f"f = {f}")
    #print("Используем первое просто число")
    div_d = random.randint(1, f-1)
    Trash, Trash1, div_c = Evklid(f, div_d)
    while PowMod(div_d*div_c, 1, f) != 1:
        div_d = random.randint(0, f)
        Trash, Trash1, div_c = Evklid(f, div_d)

    if(div_c < 0):
        div_c = div_c + f
    if(PowMod(div_d*div_c, 1, f) == 1):
        #    print("ok")
        #    print(f"Выбираем значение открытого ключа {div_d} с соблюдением условий")
        #    print(f"1<{div_d}<{f}, {div_d} и {f} – взаимно простые числа (их НОД=1)")
        #    print(f"d = {div_d}")
        #    print(f"Находим C = {div_c}")
        return N, div_d, div_c


def RSA():
    N, div_d, div_c = Key_to_RSA()
    print('__________________________________')
    message, mas_ar = read_file()
    print(f"message = {message}")
    str = b''
    for j in range(len(mas_ar)):
        inputer = PowMod(mas_ar[j], div_d, N)
        sms = PowMod(inputer, div_c, N)
        new_message = sms.to_bytes((sms.bit_length() + 7) // 8, 'big')
        str += new_message
    print(f"str ={str}")
    write_file("close4.txt", str)
