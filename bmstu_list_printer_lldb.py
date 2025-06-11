import lldb

def __lldb_init_module(debugger, internal_dict):
    debugger.HandleCommand('type synthetic add -x "bmstu::list<.*>" --python-class bmstu_list_printer_lldb.BmstuListProvider')
    debugger.HandleCommand('type summary add -x "bmstu::list<.*>" --python-function bmstu_list_printer_lldb.list_summary')
    print("bmstu::list printer loaded successfully")

class BmstuListProvider:
    def __init__(self, valobj, internal_dict):
        self.valobj = valobj
        self.size = 0
        self.elements = []
        self.update()

    def update(self):
        self.size = self.valobj.GetChildMemberWithName('size_').GetValueAsUnsigned(0)
        self.elements = []

        head = self.valobj.GetChildMemberWithName('head_')
        tail = self.valobj.GetChildMemberWithName('tail_')
        current = head.GetChildMemberWithName('next_node_')

        for i in range(self.size):
            if current.GetValueAsUnsigned() == 0 or current.GetValueAsUnsigned() == tail.GetLoadAddress():
                break

            value = current.GetChildMemberWithName('value_')
            self.elements.append((f'[{i}]', value))
            current = current.GetChildMemberWithName('next_node_')

    def num_children(self):
        return len(self.elements)

    def get_child_at_index(self, index):
        if index < 0 or index >= len(self.elements):
            return None
        return self.elements[index][1]

    def get_child_index(self, name):
        try:
            return int(name.lstrip('[').rstrip(']'))
        except:
            return -1

    def has_children(self):
        return len(self.elements) > 0

def list_summary(valobj, internal_dict):
    try:
        # Доступ к оригинальному (не synthetic) объекту
        original = valobj.GetNonSyntheticValue()
        size = original.GetChildMemberWithName('size_').GetValueAsUnsigned(0)
        elem_type = valobj.GetType().GetTemplateArgumentType(0).GetName()
        return f"bmstu::list<{elem_type}> (size={size})"
    except Exception as e:
        return f"bmstu::list<?> (size=?) [Error: {str(e)}]"






