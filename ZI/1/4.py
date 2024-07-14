def baby_step_giant_step(g, y, p):
    
    m = int((p**0.5) + 1)
    
    baby_steps = {}
    for j in range(m):
        baby_steps[pow(g, j, p)] = j
    
    gm_inv = pow(g, -m, p)
    
    
    for i in range(m):
        value = (y * pow(gm_inv, i, p)) % p
        if value in baby_steps:
            
            j = baby_steps[value]
            x = i * m + j
            return x
    
    return None


g = 3  # База
y = 9  # Целевое знач
p = 29  # Модуль

result = baby_step_giant_step(g, y, p)
print(f"Дискретный логарифм: {result}")