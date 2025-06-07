import lldb

def bmstu_list_summary(valobj, internal_dict):
    try:
        size = valobj.GetChildMemberWithName("size_").GetValueAsUnsigned(0)
        head = valobj.GetChildMemberWithName("head_")
        tail = valobj.GetChildMemberWithName("tail_")
        
        if size == 0:
            return "bmstu::list<{}> (size=0, empty)".format(
                valobj.GetType().GetTemplateArgumentType(0).GetName()
            )
     
        elements = []
        current = head.GetChildMemberWithName("next_node_")
        end = tail
        
        index = 0
        while current.GetValueAsUnsigned(0) != end.GetValueAsUnsigned(0) and index < 100: 
            node = current.Dereference()
            value = node.GetChildMemberWithName("value_")
            
            if value.GetType().IsPointerType():
                value = value.Dereference()
            
            elements.append("[{}] = {}".format(index, value.GetValue()))
            current = node.GetChildMemberWithName("next_node_")
            index += 1
        
        return "bmstu::list<{}> (size={}, contents=[{}])".format(
            valobj.GetType().GetTemplateArgumentType(0).GetName(),
            size,
            ", ".join(elements)
        )
    
    except Exception as e:
        return "bmstu::list<...> (error: {})".format(str(e))

def __lldb_init_module(debugger, internal_dict):
    debugger.HandleCommand(
        'type summary add -F bmstu_list_lldb.bmstu_list_summary "bmstu::list<.*>" -w bmstu'
    )
    print("[LLDB] Registered pretty-printer for bmstu::list")
