import random as rn
from LibLab2 import Key_to_RSA
from LibLab1 import PowMod


def print_Graph(Graph, ColorList):
    for i in range(len(Graph)):
        rows = ""
        if i <= 8:
            rows += str(ColorList[i]) + "  "
        else:
            rows += str(ColorList[i]) + " "
        for j in range(len(Graph[i])):
            if j >= 9:
                rows += " " + str(Graph[i][j]) + " "
            else:
                rows += str(Graph[i][j]) + " "
        print(rows)


def Creater_N_and_M():
    #N = rn.randint(2, 4)
    N = int(input("Количество вершин: "))
    M = rn.randint(1, pow(N, 2))

    f2 = open("NM.txt", 'w')
    f2.write(str(N)+"\n")
    f2.write(str(M))
    f2.close()

    return N, M


def Create_Graph():
    N, M = Creater_N_and_M()
    string = "B,R,G"
    list = string.split(",")
    print(f"Коллекция цветов: {string}")

    print(f"N = {N}, M = {M}")

    Color = ""
    for i in range(N):
        Color += list[i % len(list)]+" "
    ColorList = Color.split(" ")

    Graph = [[0] * N for i in range(N)]
    Copy_Graph = [[0] * N for i in range(N)]

    print(f"Начальный граф")
    print()
    print("  " + Color)

    for i in range(len(Graph)):
        rows = ""
        rows += ColorList[i]+" "
        for j in range(len(Graph[i])):
            if(i == j):
                Graph[i][j] = "0"
            rows += str(Graph[i][j])+" "
        print(rows)
    print()
    print(f"Алиса заполняет граф")
    count = 0

    print()
    print("   " + Color)

    # Определяем где соединяются вершины(ребра)
    for k in range(M):
        i = rn.randint(0, len(Graph) - 1)
        j = rn.randint(0, len(Graph[i]) - 1)
        f2 = open("NM.txt", 'a')
        f2.write("\n")
        f2.write(str(i)+" ")
        f2.write(str(j)+"\n")
        f2.close()
        # print(f'i = {i}, j={j}')
        while ColorList[i] == ColorList[j]:
            i = rn.randint(0, len(Graph) - 1)
            j = rn.randint(0, len(Graph[i]) - 1)
            # print(f'i2 = {i}, j2={j}')
        Graph[i][j] = 1
        Graph[j][i] = 1
        Copy_Graph[i][j] = 1
        Copy_Graph[j][i] = 1
        count += 1

    # Печатаем граф
    print_Graph(Graph, ColorList)

    string = "R,G,B"
    list = string.split(",")
    Color = ""
    for i in range(N):
        Color += list[i % len(list)]+" "
    ColorListRevers = Color.split(" ")

    print("Меняем цвета вершин")
    print("   " + Color)
    print_Graph(Graph, ColorListRevers)
    return N, M, Graph, Copy_Graph, Color, ColorList


def Coloring_Graph():
    N, M, Graph, Copy_Graph, Color, ColorList = Create_Graph()

    print()
    print(f"N = {N}, M = {M}")
    print("\n" +
          "Алиса задумывает для каждой вершины значение r и заменяет в его двоичной записи последние 2 бита\n" +
          "00 - красный, 01 - зеленый, 10 - синий\n")

    for i in range(N):
        for j in range(N):
            r = rn.randint(0, 1000)
            if ColorList[j] == 'R':
                r = format(r, 'b')
                r = r[:-2] + "00"
                r = int(format(int(r), 'd'), base=2)

            if ColorList[j] == 'G':
                r = format(r, 'b')
                r = r[:-2] + "01"
                r = int(format(int(r), 'd'), base=2)

            if ColorList[j] == 'B':
                r = format(r, 'b')
                r = r[:-2] + "10"
                r = int(format(int(r), 'd'), base=2)
            Copy_Graph[i][j] = r
            Copy_Graph[j][i] = r

    print()
    print("  " + Color)
    print_Graph(Copy_Graph, ColorList)
    print(f"Далее Алиса для каждой вершины генерирует RSA значения (P,Q,N = PQ,c,d)\n" +
          "И возводя значение 'r' в степень по модулю, вычисляет Z для каждой вершины\n ")

    Nr = 0
    temper = [[0] * 2 for i in range(N*N)]

    Zv_Graph = [[0] * N for i in range(N)]
    for i in range(len(Copy_Graph)):
        for j in range(len(Copy_Graph[i])):
            Nv, Dv, Cv = Key_to_RSA()
            Zv = PowMod(Copy_Graph[i][j], Dv, Nv)
            Zv_Graph[i][j] = Zv
            Zv_Graph[j][i] = Zv
            temper[Nr][0] = Nv
            temper[Nr][1] = Nv
            Nr += 1

    print()
    print("  " + Color)

    print_Graph(Zv_Graph, ColorList)

    print(f"\nИ отправялет Бобу комплект из графа Z,N и d")
    print()
    print("-----------------------------------")
    print(f"Действия Боба:")
    print("-----------------------------------")
    print(f"Боб выбирает ребро в графе")
    print(f"сообщает Алисе, какое именно ребро он выбрал . В ответ Алиса высылает числа cv1 и cv2,\n" +
          "соответствующие вершинам этого ребра.\n" +
          "После этого Боб вычисляет по формуле")

    for k in range(N):
        for z in range(N):
            if Graph[k][z] == 1:
                count = 0
                for i in range(len(Zv_Graph)):
                    for j in range(len(Zv_Graph[i])):
                        if Zv_Graph[i][j] != Zv_Graph[k][z]:
                            count += 1
                        else:
                            continue
                # Выбраное ребро Боба
                print(f"count = {Zv_Graph[k][k]}")

                # Вычесление Z для обеих вершин
                Z1 = PowMod(Zv_Graph[k][k], temper[count][1], temper[count][0])

                count1 = 0
                for i in range(len(Zv_Graph)):
                    for j in range(len(Zv_Graph[i])):
                        if Zv_Graph[i][j] != Zv_Graph[z][k]:
                            count1 += 1
                        else:
                            continue

                print(f"z2 = {Zv_Graph[k][k]}")
                Z2 = PowMod(Zv_Graph[z][z], temper[count1]
                            [1], temper[count1][0])

                new_r1 = format(Z1, 'b')
                new_r2 = format(Z2, 'b')
                print(f"new_r1 = {new_r1}")
                print(f"new_r2 = {new_r2}")
                new_r1 = new_r1[-1] + new_r1[-2]
                new_r2 = new_r2[-1] + new_r2[-2]

                print(f"new_r1 = {new_r1}")
                print(f"new_r2 = {new_r2}")
                # сравнивает два младших бита
                if new_r1 == new_r2:
                    print(f"Алиса не обманула Боба")
                else:
                    print(f"Алиса не обманула Боба")
            else:
                continue
