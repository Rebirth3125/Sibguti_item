from dataclasses import dataclass
from typing import List
from tkinter import *
from tkinter import filedialog
from os import path
from functools import partial
import tkinter.font as font
import json

window = Tk()

myFont = font.Font(family='Helvetica', size=12)
dark_bg = '#333333' 
light_text = '#FFFFFF'  
button_bg = '#555555'  
button_fg = '#FFFFFF'  
input_bg = '#555555'  
input_fg = '#FFFFFF'  

txt = Entry(window, width=40, font=myFont, bg=input_bg, fg=input_fg)
text = Text(window, width=40, height=10, bg=dark_bg, fg=light_text, font=myFont)
rules = Text(window, width=40, height=15, bg=dark_bg, fg=light_text, font=myFont)
current_status = Label(window, text="", font=myFont, bg=dark_bg, fg=light_text)

@dataclass
class Machine:
    Q: List[str]
    V: List[str]
    Rules: List[List[str]]
    Start_state: str
    Current_state: str
    Start_stack: str
    Stack: str
    Transform_stack: str
    End: List[str]
    in_transform: List[str]
    in_stack: List[str]

def parse_rules(rules_str):
    rules = []
    for rule in rules_str.strip().split("\n"):
        parts = rule.strip().split(", ")
        parts = ['λ' if x == 'lambda' else x for x in parts]
        parts = ['ε' if x == 'EPS' else x for x in parts]
        rules.append(parts)
    return rules

def machine_input_from_text(filename):
    with open(filename, "r") as file:
        lines = file.read().splitlines()

    data = {}
    rules_str = "" 
    for line in lines:
        if line.startswith("rules:"):
            rules_str += line[len("rules:"):].strip() + "\n"  
        elif rules_str:
            rules_str += line.strip() + "\n"
        else:
            key, value = line.split(": ")
            data[key] = value.split(", ")

    if rules_str:
        data["rules"] = parse_rules(rules_str)

    states = data["states"]
    alphabet = data["alphabet"]
    in_stack = data["in_stack"]
    in_transform = data["in_transform"]
    rules = data["rules"]
    start = data["start"][0]
    stack = data["start_stack"][0]
    end = data["end"]

    machine = Machine(states, alphabet, rules, start, start, stack, stack, "", end, in_transform, in_stack)
    return machine


def generate_func_tab(machine):
    rules.delete('1.0', END)
    rules.insert(END, "МП-преобразователь:\n")
    rules.insert(END, f"P({machine.Q}, {machine.V}, {machine.in_stack}, {machine.in_transform}, δ, {machine.Start_state}, {machine.Start_stack}, {machine.End})\n")
    rules.insert(END, "Функции переходов:\n")
    for rule in machine.Rules:
        rules.insert(END, f"({rule[0]}, {rule[1]}, {rule[2]}) → ({rule[3]}, {rule[4]}, {rule[5]})\n")


def clicked():
    file = filedialog.askopenfilename(filetypes=[("Text Files", "*.txt"), ("All Files", "*.*")], initialdir=path.dirname(__file__))
    if not file:
        return
    result = machine_input_from_text(file)
    if result:
        generate_func_tab(result)
        btn_check_chain.config(command=partial(check_button, result))  # Обновление команды кнопки

btn_check_chain = Button(window, text="Проверить цепочку", bg=button_bg, fg=button_fg, padx=10, pady=10, font=myFont)
btn_check_chain.grid(row=4, column=2, sticky="e")

def check_button(machine):
    chain = txt.get()
    if chain == 'quit':
        return 0
    machine.Current_state = machine.Start_state
    machine.Stack = machine.Start_stack
    machine.Transform_stack = ""
    if all([c in machine.V for c in chain]):
        check_chain(chain, machine)
    else:
        text.delete('1.0', END)
        current_status.config(text = "NO!")


def check_chain(chain, machine):
    text.delete('1.0', END)
    text.insert(END, f"Стек: {machine.Stack}\n")
    step = 1
    for i in chain:
        text.insert(END, f"Шаг {step}\n")
        trans_check = 0
        text.insert(END, f"Цепочка {i}\n")
        text.insert(END, f"Текущий стек: {machine.Stack}\n")
        text.insert(END, f"Текущий перевод: {machine.Transform_stack}\n")
        for j in machine.Rules:
            if (machine.Current_state != j[0]) or (i != j[1]) or (machine.Stack[0] != j[2]):
                trans_check += 1
                continue
    
            text.insert(END, f"Правило: ({j[0]}, {j[1]}, {j[2]}) -> ({j[3]}, {j[4]}, {j[5]})\n\n")
            machine.Current_state = j[3]
            if j[4] != "ε" and j[4] != "λ":
                machine.Stack = j[4] + machine.Stack[1:]
            elif j[4] == "ε":
                machine.Stack = machine.Stack[1:]
            if j[5] != "λ":
                machine.Transform_stack = machine.Transform_stack + j[5]
            break
        step += 1
        if trans_check == len(machine.Rules):
            text.insert(END, f"Ошибка. Нет перехода для текущего состояния.\n\n")            
            current_status.config(text = "Ошибка. Нет перехода для текущего состояния.")
            return
    while TRUE:
        if len(machine.Stack) == 0 and machine.Current_state in machine.End:
            text.insert(END, f"Цепочка проходит заданный МП-преобразователь.\n"
                             f"Полученный перевод: {machine.Transform_stack}\n\n")
            current_status.config(text = f"Yes.\nПолученный перевод: {machine.Transform_stack}")
            return
        text.insert(END, f"Шаг {step}\n")
        trans_check = 0
        text.insert(END, f"Цепочка: λ\n")
        text.insert(END, f"Текущий стек: {machine.Stack}\n")
        text.insert(END, f"Текущий перевод: {machine.Transform_stack}\n")
        for j in machine.Rules:
            if (machine.Current_state != j[0]) or ("λ" != j[1]) or (machine.Stack[0] != j[2]):
                trans_check += 1
                continue

            text.insert(END, f"Правило: ({j[0]}, {j[1]}, {j[2]}) -> ({j[3]}, {j[4]}, {j[5]})\n\n")
            machine.Current_state = j[3]
            if j[4] == "ε":
                machine.Stack = machine.Stack[1:]
            elif j[4] == "λ":
                continue
            if j[5] != "λ":
                machine.Transform_stack = machine.Transform_stack + j[5]
            break
        step += 1
        if trans_check == len(machine.Rules):
            text.insert(END, f"Ошибка. Нет перехода для текущего состояния.\n\n")            
            current_status.config(text = "Ошибка. Нет перехода для текущего состояния.")
            return

def setup_ui():
    window.title("МП преобразователь")
    window.geometry('1000x700')
    window.configure(bg=dark_bg)

    # Заголовок
    lbl = Label(window, text="МП:", font=myFont, bg=dark_bg, fg=light_text)
    lbl.grid(row=0, column=0, sticky="w", padx=10, pady=10)

    try:
        result = machine_input_from_text('I.txt')  # Укажите здесь имя вашего файла
        if result:
            generate_func_tab(result)
            btn_check_chain = Button(window, text="Проверить цепочку", command=partial(check_button, result), bg=dark_bg, fg=light_text, padx=10, pady=10, font=myFont)
            btn_check_chain.grid(row=4, column=2, sticky="e")
    except FileNotFoundError as e:
        current_status.config(text="Файл не найден: " + str(e))

    lbl_left = Label(window, text="Функции переходов", font=myFont, bg=dark_bg, fg=light_text)
    lbl_left.grid(row=1, column=0, padx=10, pady=5)

    rules.grid(row=2, column=0, padx=10, pady=5, rowspan=3, sticky="nsew")

    lbl_txt = Label(window, text="Цепочка для проверки", font=myFont, bg=dark_bg, fg=light_text)
    lbl_txt.grid(row=1, column=1, padx=10, pady=5)

    txt.grid(row=2, column=1, padx=10, pady=5)

    lbl_text = Label(window, text="Проверка построения цепочки", font=myFont, bg=dark_bg, fg=light_text)
    lbl_text.grid(row=3, column=1, padx=10, pady=5)

    text.grid(row=4, column=1, padx=10, pady=5)

    # Статусная строка и кнопка проверки цепочки перемещены в нижнюю часть интерфейса
    status = Label(window, text="Статус", font=myFont, bg=dark_bg, fg=light_text)
    status.grid(row=5, column=0, padx=10, pady=5)

    current_status.grid(row=5, column=1, padx=10, pady=5, sticky="w")
    window.mainloop()

if __name__ == '__main__':
    setup_ui()