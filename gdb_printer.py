import gdb.printing

class SimpleVectorPrinter:
    def __init__(self, val):
        self.val = val

    def to_string(self):
        try:
            size = int(self.val['size_'])
            capacity = int(self.val['capacity_'])
            return f"bmstu::simple_vector<char> (size = {size}, capacity = {capacity})"
        except gdb.error as e:
            return f"<error: {e}>"

    def children(self):
        try:
            size = int(self.val['size_'])
            capacity = int(self.val['capacity_'])
            data_ptr = self.val['data_']['raw_ptr_']
            yield "size", size
            yield "capacity", capacity
            for i in range(size):
                element = (data_ptr + i).dereference()
                yield f"[{i}]", f"{element} \"{chr(int(element))}\""
            yield "Raw View", f"(data._=(raw_ptr._={data_ptr}), size._={size}, capacity._={capacity})"
        except gdb.error as e:
            yield "<error>", f"Error accessing elements: {e}"

    def display_hint(self):
        return 'array'

class BasicStringPrinter:
    def __init__(self, val):
        self.val = val

    def to_string(self):
        try:
            size = int(self.val['size_'])
            ptr = self.val['ptr_']
            if ptr:
                data = ptr.string("utf-8", length=size, errors='replace')
                return f"bmstu::basic_string<char> (size = {size}, data = \"{data}\")"
            else:
                return f"bmstu::basic_string<char> (size = {size}, data = \"\")"
        except gdb.error as e:
            return f"<error: {e}>"

    def children(self):
        try:
            size = int(self.val['size_'])
            ptr = self.val['ptr_']
            yield "size", size
            if ptr:
                data = ptr.string("utf-8", length=size, errors='replace')
                yield "data", f"\"{data}\""
                for i in range(size):
                    char = ptr[i]
                    char_repr = chr(char) if 32 <= char <= 126 else '?'
                    yield f"[{i}]", f"{char} '{char_repr}'"
            yield "Raw View", f"(ptr._={ptr}, size._={size})"
        except gdb.error as e:
            yield "<error>", f"Error accessing elements: {e}"

    def display_hint(self):
        return 'string'

class WStringPrinter:
    def __init__(self, val):
        self.val = val

    def to_string(self):
        try:
            size = int(self.val['size_'])
            ptr = self.val['ptr_']
            if ptr:
                data = ptr.string("utf-32", length=size, errors='replace')
                return f"bmstu::basic_string<wchar_t> (size = {size}, data = \"{data}\")"
            else:
                return f"bmstu::basic_string<wchar_t> (size = {size}, data = \"\")"
        except gdb.error as e:
            return f"<error: {e}>"

    def children(self):
        try:
            size = int(self.val['size_'])
            ptr = self.val['ptr_']
            yield "size", size
            if ptr:
                data = ptr.string("utf-32", length=size, errors='replace')
                yield "data", f"\"{data}\""
                for i in range(size):
                    yield f"[{i}]", f"{ptr[i]} \"{chr(int(ptr[i]))}\""
            yield "Raw View", f"(ptr._={ptr}, size._={size})"
        except gdb.error as e:
            yield "<error>", f"Error accessing elements: {e}"

    def display_hint(self):
        return 'string'

def lookup_type(val):
    type_name = val.type.name
    if type_name:
        if type_name.startswith('bmstu::simple_vector<'):
            return SimpleVectorPrinter(val)
        elif type_name.startswith('bmstu::basic_string<char>'):
            return BasicStringPrinter(val)
        elif type_name.startswith('bmstu::basic_string<wchar_t>'):
            return WStringPrinter(val)
    return None

def register_printers():
    pp = gdb.printing.RegexpCollectionPrettyPrinter("bmstu_printers")
    pp.add_printer('bmstu::simple_vector', '^bmstu::simple_vector<.*>$', SimpleVectorPrinter)
    pp.add_printer('bmstu::basic_string<char>', '^bmstu::basic_string<char>$', BasicStringPrinter)
    pp.add_printer('bmstu::basic_string<wchar_t>', '^bmstu::basic_string<wchar_t>$', WStringPrinter)
    gdb.printing.register_pretty_printer(gdb.current_objfile(), pp, replace=True)

register_printers()