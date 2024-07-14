class Grammar:
    def __init__(self):
        self.VT = []
        self.VN = []
        self.P = {}
        self.S = ""

def count_non_term_sym(grammar, sequence):
    length = 0
    for sym in sequence:
        if sym in grammar.VT:
            length += 1
    return length

if __name__ == "__main__":
    grammar = Grammar()
    grammar.VT = ["1", "2"]
    grammar.VN = ["A"]
    grammar.P = {"A": ["1AAAAAA", "2", ""]}
    grammar.S = "A"

    borders = input("Введите диапазон цепочек ОТ и ДО: ").split()
    left_border = int(borders[0])
    right_border = int(borders[1])

    rules = [(grammar.S, [])]
    used_sequence = set()

    while rules:
        rule = rules.pop()
        sequence, path = rule

        if sequence in used_sequence:
            continue

        used_sequence.add(sequence)
        no_term = True

        for symbol in sequence:
            if symbol in grammar.VN:
                no_term = False
                for elem in grammar.P.get(symbol, []):
                    temp = sequence[:sequence.index(symbol)] + elem + sequence[sequence.index(symbol) + 1:]
                    if left_border <= count_non_term_sym(grammar, temp) <= right_border and temp not in used_sequence:
                        new_path = list(path)
                        new_path.append(temp)
                        rules.append((temp, new_path))
            elif symbol not in grammar.VT:
                no_term = False
                print("Цепочка", sequence, "не разрешима")
                break

        if no_term and left_border <= len(sequence) <= right_border:
            print("Путь:", " -> ".join(path))
            print("λ" if sequence == "" else sequence)