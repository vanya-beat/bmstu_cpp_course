#ifndef BMSTU_STRING_H_
#define BMSTU_STRING_H_

#include <stdexcept>
#include <iosfwd>
#include <initializer_list>

namespace bmstu {

// Forward declarations
template<typename CharT> class BasicString;

// Common string type aliases
using String    = BasicString<char>;
using WString   = BasicString<wchar_t>;
using U16String = BasicString<char16_t>;
using U32String = BasicString<char32_t>;

template<typename CharT>
class BasicString {
public:
    // Construction/destruction
    BasicString() noexcept 
        : length_(0), data_(new CharT[1]{0}) {}
    
    explicit BasicString(size_t count) 
        : length_(count), data_(new CharT[count + 1]) {
        std::fill_n(data_, count, CharT(' '));
        data_[count] = CharT(0);
    }
    
    BasicString(const CharT* str) 
        : length_(str ? str_length(str) : 0),
          data_(new CharT[length_ + 1]) {
        copy_chars(str, data_, length_);
    }
    
    BasicString(std::initializer_list<CharT> chars)
        : length_(chars.size()),
          data_(new CharT[length_ + 1]) {
        std::copy(chars.begin(), chars.end(), data_);
        data_[length_] = CharT(0);
    }
    
    // Copy/move semantics
    BasicString(const BasicString& other)
        : BasicString(other.data_) {}
        
    BasicString(BasicString&& other) noexcept
        : length_(other.length_), data_(other.data_) {
        other.length_ = 0;
        other.data_ = nullptr;
    }
    
    ~BasicString() { release(); }
    
    BasicString& operator=(const BasicString& rhs) {
        if (this != &rhs) {
            BasicString temp(rhs);
            swap(temp);
        }
        return *this;
    }
    
    BasicString& operator=(BasicString&& rhs) noexcept {
        if (this != &rhs) {
            release();
            length_ = rhs.length_;
            data_ = rhs.data_;
            rhs.length_ = 0;
            rhs.data_ = nullptr;
        }
        return *this;
    }
    
    // Element access
    CharT& at(size_t pos) {
        if (pos >= length_) {
            throw std::out_of_range("Invalid position");
        }
        return data_[pos];
    }
    
    const CharT& at(size_t pos) const {
        return const_cast<BasicString*>(this)->at(pos);
    }
    
    CharT& operator[](size_t pos) { return data_[pos]; }
    const CharT& operator[](size_t pos) const { return data_[pos]; }
    
    const CharT* c_str() const noexcept { 
        return data_ ? data_ : empty_string(); 
    }
    
    // Capacity
    size_t size() const noexcept { return length_; }
    bool empty() const noexcept { return length_ == 0; }
    
    // Operations
    BasicString& operator+=(const BasicString& str) {
        return append(str);
    }
    
    BasicString& operator+=(CharT ch) {
        CharT str[] = {ch, CharT(0)};
        return append(str);
    }
    
    BasicString& append(const BasicString& str) {
        if (str.empty()) return *this;
        
        CharT* new_data = new CharT[length_ + str.length_ + 1];
        copy_chars(data_, new_data, length_);
        copy_chars(str.data_, new_data + length_, str.length_);
        new_data[length_ + str.length_] = CharT(0);
        
        release();
        data_ = new_data;
        length_ += str.length_;
        return *this;
    }
    
    // Friends
    friend BasicString operator+(const BasicString& lhs, const BasicString& rhs) {
        BasicString result(lhs);
        return result += rhs;
    }
    
    template<typename Stream>
    friend Stream& operator<<(Stream& os, const BasicString& str) {
        return os << str.c_str();
    }
    
    template<typename Stream>
    friend Stream& operator>>(Stream& is, BasicString& str) {
        str.clear();
        CharT ch;
        while (is.get(ch) && ch != '\n') {
            str += ch;
        }
        return is;
    }
    
    void clear() noexcept {
        release();
        length_ = 0;
        data_ = new CharT[1]{0};
    }
    
    void swap(BasicString& other) noexcept {
        std::swap(length_, other.length_);
        std::swap(data_, other.data_);
    }
    
private:
    size_t length_;
    CharT* data_;
    
    static const CharT* empty_string() noexcept {
        static const CharT empty[] = {0};
        return empty;
    }
    
    static size_t str_length(const CharT* str) noexcept {
        const CharT* end = str;
        while (*end) ++end;
        return end - str;
    }
    
    static void copy_chars(const CharT* src, CharT* dest, size_t count) noexcept {
        std::copy(src, src + count, dest);
    }
    
    void release() noexcept {
        delete[] data_;
        data_ = nullptr;
    }
};

} // namespace bmstu

#endif // BMSTU_STRING_H_
