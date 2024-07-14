import random

def PowMod(a,x,p):
    dvoichmassiv = []
    resultarray = []
    result = 1
    while x>= 1:
        dvoichmassiv.append(int(x%2))
        x/=2
    n = len(dvoichmassiv)
    resultarray.append(a)
    for i in range(n):
        resultarray.append((resultarray[i] * resultarray[i]) % p)
    for j in range(n):
        if dvoichmassiv[j] > 0:
            result *= dvoichmassiv[j] * resultarray[j]
    result = result%p
    del dvoichmassiv
    del resultarray
    return result

def Evklid(num1, num2):
    if num1 == 0:
        return (num2, 0, 1)
    else:
        div, x, y = Evklid(num2 % num1, num1)
    return (div, y - (num2 // num1) * x, x)#div,x,y

def IsPrime(Q):
    if Q<=1:
        return bool(False)
    b = int(pow(Q,0.5))
    for i in range(2,b+1):
        if Q%i == 0:
            return bool(False)
    return bool(True)

def D_X():
    flag= True
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
    #while pow(g,q,p)!=1:
    while PowMod(g,q,p) == 1:
        g = random.randint(1,p-1)
    print(f"G = {g}")

    Bob = random.randint(1,p)
    Alice = random.randint(1,p)

    SicretBob = PowMod(g,Bob,p)
    SicretAlice = PowMod(g,Alice,p)

    SicretKey1 = PowMod(SicretAlice,Bob,p)
    SicretKey2 = PowMod(SicretBob,Alice,p)

    if SicretKey1 == SicretKey2:
        print(f"Q = {q}")
        print(f"P = {p}")
        print(f"Bob = {Bob}")
        print(f"Alice = {Alice}")
        print(f"SicretBob = {SicretBob}")
        print(f"SicretAlice = {SicretAlice}")
        print(f"SicretKey1 = {SicretKey1}")
        print(f"SicretKey2 = {SicretKey2}")
        print("Секретные ключи равны, можно общаться")
        return SicretKey1
    else:
        D_X()


def StepBaby_StepColos(a,p,y):
    resultfor_m = []
    resultfor_k = []
    a = 2
    p = 5
    y = 3
    print(f"P = {p}")
    print(f"a = {a}")
    m = k = random.randint(0, 10)
    sum = m * k
    while sum <= p:
        m = k = random.randint(0, 10)
        sum=m*k

    m = k = 3
    print("m = {}, k = {}".format(m, k))
    for j in range(0,m):
        if j == 0:
            resultfor_m.append(y % p)
        else:
            resultfor_m.append(((pow(a,j))*y)%p)
        print(resultfor_m)
    for i in range(0,k):
        resultfor_k.append(pow(a,(i+1)*m)%p)
        print(resultfor_k)

    for j in range(0,m):
        for i in range(0,k):
            if resultfor_m[i] == resultfor_k[j] :
                print("Я зашел")
                index_i = i+1
                print(f"vivel index_i = {index_i}")
                index_j = j
                print(f"vivel index_j = {index_j}")
                print("Нашел")

                x = index_i * m - index_j

                del resultfor_m
                del resultfor_k

                print("X = {}".format(x))
                return x