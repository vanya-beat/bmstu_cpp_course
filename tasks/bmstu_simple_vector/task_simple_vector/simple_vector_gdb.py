import gdb

class SimpleVectorPrinter:
    class _iterator:
        def __init__(self, _size, _data):
            self._count = 0
            self._size = _size
            self._data = _data

        def __iter__(self):
            return self

        def __next__(self):
            if self._count == self._size:
                raise StopIteration
            count = self._count
            self._count += 1
            return (f"[{count}]", self._data["raw_ptr_"][count])

    def __init__(self, val):
        self.val = val

    def to_string(self):
        size = self.val["size_"]
        return "size=%s" % (str(size))

    def children(self):
        return self._iterator(self.val["size_"], self.val["data_"])


def lookup_function(val):
    if "bmstu::simple_vector" in str(val.type) and str(val.type).endswith(">"):
        return SimpleVectorPrinter(val)
    return None

gdb.pretty_printers.append(lookup_function)

# класс собран исходя из структуры исходников:
# /usr/share/gcc/python/libstdcxx/v6
