import re
from graphviz import Digraph


# Parsing table for syntax analysis (based on provided grammar rules)
parsing_table = {
    'S': {'identifier': ('O', 'S\'')},
    'S\'': {'identifier': ('O', 'S\''), '$': ('e',)},
    'O': {'identifier': ('I', 'D', 'E', 'V')},
    'E': {'identifier': ('T', 'R'), 'number': ('T', 'R'), 'brace_left': ('P',)},
    'P': {'brace_left': ('B', 'E', 'B\'', 'R')},
    'R': {'plus': ('A', 'E'), 'minus': ('A', 'E'), 'mul': ('A', 'E'), 'div': ('A', 'E'), 'semicolon': ('e', ), 'brace_right': ('e', )},
    'T': {'identifier': ('I',), 'number': ('N',)},
    'I': {'identifier': ('identifier',)},
    'D': {':=': (':=',)},
    'N': {'number': ('number',)},
    'B': {'brace_left': ('(',)},
    'B\'': {'brace_right': (')',)},
    'A': {'plus': ('+',), 'minus': ('-',), 'mul': ('*',), 'div': ('/',)},
    'V': {'semicolon': (';',)},
}
terms = ['identifier', 'number', ':=', '(', ')', '-', '+', '*', '/', ';']


def syntax_parser(tokens):
    stack = ['$', 'S']  # Initialize the stack with the start symbol
    index = 0  # Start at the first token in the input
    parse_tree = []  # Store the parsing steps to build the parse tree

    print("\nDebugging Table:")
    print("\t\tМагазин\t\t\t\t\tВход\t\t\tВыход")
    print("-" * 80)

    while stack:
        current_token_type = tokens[index][0] if index < len(tokens) else '$'
        current_token_value = tokens[index][1] if index < len(tokens) else ''  # Get token value
        print(f"{' '.join(map(str, reversed(stack))).ljust(30)}\t{current_token_type.ljust(15)}\t", end="")
        top = stack.pop()

        # проверка совпадения стэка и текущего токена
        if top == current_token_type or top == current_token_value:
            print("Match")
            index += 1

        # если не совпадает, то ищем правило
        elif top in parsing_table:
            rule = parsing_table.get(top, {}).get(current_token_type) or parsing_table.get(top, {}).get(current_token_value)

            # если правило нашлось, то добавляем в дерево, убираем из стэка
            if rule:
                print(f"{top} -> {' '.join(rule)}")

                if rule[0] in terms:
                    parse_tree.append((top, (current_token_value,)))
                else:
                    parse_tree.append((top, rule))

                for symbol in reversed(rule):
                    if symbol != 'e':
                        stack.append(symbol)
            else:
                raise ValueError(f"Синтаксическая ошибка, встретился '{tokens[index][1]}', а подходящего вывода из "
                                 f"нетерминала '{top}' не нашлось")

        else:
            raise ValueError(f"Неожиданный символ '{top}' в стеке.")

    if index < len(tokens) or stack:
        raise ValueError("Неожиданное завершение синтаксического анализа.")

    return parse_tree


def dummy_lexer(input_str):
    tokens = []
    lexemes = input_str.split()
    for lex in lexemes:
        kind = classify_token(lex)
        if kind == 'unknown':
            raise ValueError(f"Тип токена '{lex}' не может быть определён")
        tokens.append((kind, lex))

    return tokens


# Функция для классификации накопленного буфера
def classify_token(buffer):
    match buffer:
        case _ if re.fullmatch(r"[a-zA-Z][a-zA-Z0-9]*", buffer) or re.fullmatch(r"_[a-zA-Z0-9]*", buffer):
            return "identifier"
        case _ if (re.fullmatch(r"[0-9]+", buffer)
                   or re.fullmatch(r"[0-9]+\.[0-9]+", buffer)
                   or re.fullmatch(r"[1-9]\.[0-9]+e[+, -][1-9][0-9]*", buffer)):
            return 'number'
        case ":=":
            return "assignment"
        case ";":
            return "semicolon"
        case "+":
            return "plus"
        case "-":
            return "minus"
        case "/":
            return "div"
        case "*":
            return "mul"
        case "(":
            return "brace_left"
        case ")":
            return "brace_right"
        case _:
            return "unknown"




def visualize_parse_tree(parse_tree):
    dot = Digraph()
    node_count = 0
    tree_nodes = []  # Список для хранения узлов дерева

    def add_node(label, parent=None):
        nonlocal node_count
        node_id = f"node{node_count}"
        dot.node(node_id, label)

        if parent:
            dot.edge(parent, node_id)

        node_count += 1
        return node_id

    for non_terminal, production in parse_tree:
        # Ищем последний узел с таким же родителем
        parent_id = None
        for i in range(len(tree_nodes) - 1, -1, -1):
            if tree_nodes[i]['label'] == non_terminal:
                parent_id = tree_nodes[i]['node_id']  # Получаем id последнего найденного родителя
                tree_nodes.pop(i)  # Удаляем его из списка
                break

        # Если не нашли подходящий узел среди предыдущих созданных, то создаём новый
        parent_id = add_node(non_terminal) if parent_id is None else parent_id

        for symbol in production:
            child_id = add_node(symbol, parent_id)
            tree_nodes.append({'label': symbol, 'node_id': child_id})

    dot.render("parse_tree", format="png", view=True)


# Основная программа
while True:
    try:
        filename = input("Введите имя файла: ")
        with open(filename, 'r') as file:
            input_str = file.read()
            dummy_tokens = dummy_lexer(input_str)
            print("dummy tokens:\n", dummy_tokens)
            tree = syntax_parser(dummy_tokens)
            for elem in tree:
                print(elem)
            visualize_parse_tree(tree)
    except FileNotFoundError:
        print("Файл не найден. Пожалуйста, проверьте имя файла и попробуйте снова.")
    except ValueError as e:
        print(f"Ошибка: {e}")

