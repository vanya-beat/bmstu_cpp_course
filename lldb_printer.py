import lldb

class SimpleVectorSyntheticProvider:
    def __init__(self, valobj, internal_dict):
        self.valobj = valobj
        self.size = 0
        self.capacity = 0
        self.data_ptr = 0
        self.element_type = None
        self.element_size = 0
        self.update()

    def update(self):
        try:
            self.size = self.valobj.GetChildMemberWithName("size_").GetValueAsUnsigned(0)
            self.capacity = self.valobj.GetChildMemberWithName("capacity_").GetValueAsUnsigned(0)
            data_holder = self.valobj.GetChildMemberWithName("data_")
            self.data_ptr = data_holder.GetChildMemberWithName("raw_ptr_").GetValueAsUnsigned(0)
            self.element_type = self.valobj.GetType().GetTemplateArgumentType(0)
            if not self.element_type.IsValid():
                self.element_type = self.valobj.GetTarget().FindFirstType("int")
            self.element_size = self.element_type.GetByteSize()
        except:
            self.size = self.capacity = self.data_ptr = 0

    def num_children(self):
        return self.size + 3

    def get_child_at_index(self, index):
        try:
            if index == 0:
                return self.valobj.CreateValueFromExpression("size", f"{self.size}")
            elif index == 1:
                return self.valobj.CreateValueFromExpression("capacity", f"{self.capacity}")
            elif index == 2:
                return self.valobj.CreateValueFromExpression("data_ptr", f"(void*){self.data_ptr}")
            elif 3 <= index < self.size + 3:
                element_index = index - 3
                element_addr = self.data_ptr + element_index * self.element_size
                return self.valobj.CreateValueFromAddress(f"[{element_index}]", element_addr, self.element_type)
        except:
            return None
        return None

    def has_children(self):
        return True

    def to_string(self):
        return f"bmstu::simple_vector<{self.element_type.GetName() if self.element_type else '???'}>"

class BasicStringSyntheticProvider:
    def __init__(self, valobj, internal_dict):
        self.valobj = valobj
        self.size = 0
        self.ptr = None
        self.update()

    def update(self):
        try:
            self.size = self.valobj.GetChildMemberWithName("size_").GetValueAsUnsigned()
            self.ptr = self.valobj.GetChildMemberWithName("ptr_")
        except Exception as e:
            print(f"<error: {str(e)}>")

    def num_children(self):
        return self.size + 2

    def get_child_at_index(self, index):
        if index == 0:
            return self.valobj.CreateValueFromData("size", lldb.SBData.CreateDataFromInt(self.size), self.valobj.GetType().GetBasicType(lldb.eBasicTypeUnsignedLongLong))
        elif index == 1:
            return self.ptr
        elif index < self.size + 2:
            try:
                char_addr = self.ptr.GetValueAsUnsigned() + (index - 2)
                char = self.valobj.GetProcess().ReadUnsignedFromMemory(char_addr, 1, lldb.SBError())
                if char:
                    return self.valobj.CreateValueFromData(f"[{index - 2}]", lldb.SBData.CreateDataFromInt(char), self.valobj.GetType().GetBasicType(lldb.eBasicTypeChar))
            except Exception as e:
                print(f"<error: {str(e)}>")
        return None

    def get_child_index(self, name):
        if name == "size":
            return 0
        elif name == "ptr":
            return 1
        elif name.startswith("[") and name.endswith("]"):
            try:
                return int(name[1:-1]) + 2
            except ValueError:
                pass
        return None

    def to_string(self):
        if self.size == 0:
            return "bmstu::string (size=0)"
        try:
            error = lldb.SBError()
            data = self.valobj.GetProcess().ReadMemory(self.ptr.GetValueAsUnsigned(), self.size, error)
            if error.Success():
                return f"bmstu::string (size={self.size}, data=\"{data.decode('utf-8', errors='replace')}\")"
            return f"bmstu::string (size={self.size}, data=<invalid>)"
        except Exception as e:
            return f"bmstu::string (size={self.size}, data=<error: {str(e)}>)"

class WStringSyntheticProvider:
    def __init__(self, valobj, internal_dict):
        self.valobj = valobj
        self.size = 0
        self.ptr = None
        self.update()

    def update(self):
        try:
            self.size = self.valobj.GetChildMemberWithName("size_").GetValueAsUnsigned()
            self.ptr = self.valobj.GetChildMemberWithName("ptr_")
        except Exception as e:
            print(f"<error: {str(e)}>")

    def num_children(self):
        return self.size + 2

    def get_child_at_index(self, index):
        if index == 0:
            return self.valobj.CreateValueFromData("size", lldb.SBData.CreateDataFromInt(self.size), self.valobj.GetType().GetBasicType(lldb.eBasicTypeUnsignedLongLong))
        elif index == 1:
            return self.ptr
        elif index < self.size + 2:
            try:
                wchar_addr = self.ptr.GetValueAsUnsigned() + (index - 2) * 4
                wchar = self.valobj.GetProcess().ReadUnsignedFromMemory(wchar_addr, 4, lldb.SBError())
                if wchar:
                    return self.valobj.CreateValueFromData(f"[{index - 2}]", lldb.SBData.CreateDataFromInt(wchar), self.valobj.GetType().GetBasicType(lldb.eBasicTypeUnsignedInt))
            except Exception as e:
                print(f"<error: {str(e)}>")
        return None

    def get_child_index(self, name):
        if name == "size":
            return 0
        elif name == "ptr":
            return 1
        elif name.startswith("[") and name.endswith("]"):
            try:
                return int(name[1:-1]) + 2
            except ValueError:
                pass
        return None

    def to_string(self):
        if self.size == 0:
            return "bmstu::wstring (size=0)"
        try:
            error = lldb.SBError()
            data = self.valobj.GetProcess().ReadMemory(self.ptr.GetValueAsUnsigned(), self.size * 4, error)
            if error.Success():
                chars = []
                for i in range(self.size):
                    chunk = data[i*4 : (i+1)*4]
                    wchar = int.from_bytes(chunk, byteorder='little')
                    chars.append(chr(wchar))
                return f"bmstu::wstring (size={self.size}, data=\"{''.join(chars)}\")"
            return f"bmstu::wstring (size={self.size}, data=<invalid>)"
        except Exception as e:
            return f"bmstu::wstring (size={self.size}, data=<error: {str(e)}>)"

def __lldb_init_module(debugger, internal_dict):
    debugger.HandleCommand("type synthetic delete -x 'bmstu::simple_vector<.*>'")
    debugger.HandleCommand("type summary delete -x 'bmstu::simple_vector<.*>'")

    debugger.HandleCommand(
        "type synthetic add -x 'bmstu::simple_vector<.*>' "
        "--python-class lldb_printer.SimpleVectorSyntheticProvider"
    )
    debugger.HandleCommand("type synthetic add bmstu::basic_string<char> --python-class lldb_printer.BasicStringSyntheticProvider")
    debugger.HandleCommand("type synthetic add bmstu::basic_string<wchar_t> --python-class lldb_printer.WStringSyntheticProvider")

if __name__ == "__main__":
    __lldb_init_module(lldb.debugger, None)