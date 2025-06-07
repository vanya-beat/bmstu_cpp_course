# .vscode/bmstu_list_printer.py
import gdb
import gdb.printing

class BmstuListPrinter:
    def __init__(self, val):
        self.val = val
        self.size = int(val['size_'])
        self.head = val['head_']
        self.tail = val['tail_']

    def to_string(self):
        return f"bmstu::list<{self.val.type.template_argument(0)}> (size={self.size})"

    def children(self):
        current_node = self.head['next_node_']
        end_node = self.tail.address
        
        index = 0
        while current_node != end_node and index < self.size:
            try:
                # Дереференс указателя на узел
                node = current_node.dereference()
                # Получаем значение
                value = node['value_']
                yield f"[{index}]", value
                
                # Переходим к следующему узлу
                current_node = node['next_node_']
                index += 1
            except gdb.MemoryError:
                yield f"[{index}]", "<memory error>"
                break
            except Exception as e:
                yield f"[{index}]", f"<error: {str(e)}>"
                break

    def display_hint(self):
        return "array"

def build_pretty_printer():
    pp = gdb.printing.RegexpCollectionPrettyPrinter("bmstu_list")
    pp.add_printer('bmstu::list', '^bmstu::list<.*>$', BmstuListPrinter)
    return pp

gdb.printing.register_pretty_printer(
    gdb.current_objfile(),
    build_pretty_printer(),
    replace=True
)
