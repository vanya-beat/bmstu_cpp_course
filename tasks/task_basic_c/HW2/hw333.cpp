#include <iostream>
using namespace std;
using std::cin;
using std::cout;
using std::endl;

class String;
String operator+(const String& left, const String& right);

class String
{
	int size;
	char* str;
public:
	int get_size() const
	{
		return size;
	}
	const char* get_str() const
	{
		return str;
	}
	char* get_str() 
	{
		return str;
	}
	/*_________________*/
	explicit String(int size = 80)
	{
		this->size = size;
		this->str = new char[size] {};
		cout << (size == 80 ? "Default" : "Size") << "Constructor:\t" << this << endl;
	}
	String(const char str[])
	{
		this->size = 1 + 1;
		this->str = new char[size] {};
		for (int i = 0; str[i]; i++)
		{
			this->str[i] = str[i];
		}
		cout << "Constructor1:\t" << this << endl;
	}
	String(const char str)
	{
		this->size = 2;
		this->str = new char[size] {};
		this->str[0] = str;
		cout << "Constructor2:\t" << this << endl;
	}
	String(const String& other)
	{
		this->size = other.size;
		this->str = new char[size] {};
		for (int i = 0; i < size; i++) this->str[i] = other.str[i];
		cout << "CopyConstructor:\t" << this << endl;
	}
	String(String&& other) noexcept
	{
		this->size = other.size;
		this->str = other.str;
		other.str = nullptr;
		cout << "MoveConstructor:\t" << this << endl;

	}
	~String()
	{
		delete[] this->str;
		cout << "Destructor:\t" << this << endl;
	}
	String& operator=(const String& other)
	{
		if (this == &other)return *this;
		delete[] this->str;
		this->size = other.size;
		this->str = new char[size] {};
		for (int i = 0; i < size; i++) this->str[i] = other.str[i];
		cout << "CopyAssignment:\t" << this << endl;
		return *this;
	}

	String& operator=(String&& other) noexcept
	{
		if (this == &other)return *this;
		delete[] this->str;
		this->size = other.size;
		this->str = other.str;
		other.str = nullptr;
		cout << "MoveAssignment:\t" << this << endl;
		return *this;
	}
	String& operator+=(const String& other)
	{	
		return (*this = *this + other);
	}
	void Print() const
	{
		cout << "Size:\t" << size << endl;
		cout << "str:\t" << str << endl;
	}
	char operator[](const int i) const
	{
		return this->get_str()[i];
	}
};
ostream& operator<<(ostream& os, const String& obj)
{
	return os << obj.get_str();
}
String operator+(const String& left, const String& right)
{
	String result(left.get_size() + right.get_size() - 1);
	for (int i = 0; i < left.get_size(); i++)
		result.get_str()[i] = left[i];
	for (int i = 0; i < right.get_size(); i++)
		result.get_str()[i+left.get_size()-1] = right[i];
	return result;

}
bool operator==(const String& left, const String& right)
{
	if (left.get_size() != right.get_size()) return false;
	else
	{
		for (int i = 0; i < left.get_size(); i++)
		{
			if (left[i] != right[i]) return false;
		}
	}

	return true;
}
bool operator!=(const String& left, const String& right)
{
	return !(left == right);
}


//#define CONSTRUCTORS_CHECK
//#define ASSIGNMENT_CHECK

int main()
{
	setlocale(LC_ALL, "");
#ifdef CONSTRUCTORS_CHECK

	String str;
	str.Print();
	String str1 = "Hello";
	str1.Print();
	cout << str1 << endl;
	String str2 = str1;
	cout << str2 << endl;
	str = str2;
	cout << str << endl;
#endif // CONSTRUCTORS_CHECK
#ifdef ASSIGNMENT_CHECK
	String str1 = "Hello";
	String str2;
	str1 = str1;
	cout << "str1: " << str1 << endl;
	cout << "str2: " << str2 << endl;
#endif // ASSIGNMENT_CHECK
	String str1 = "Hello";
	String str2 = "World";
	String str3 = str1 + str2;
	cout << str3 << endl;
	/*str1 += str2;
	cout << str1 << endl;*/
	String str4;
	str4 = str1 + str2;
	cout << str4 << endl;
	cout << str3[4] << endl;
	str2 = str1;
	cout << (str1 == str2) << endl;
	cout << (str1 != str2) << endl;
	String str5 = str3[4];
	cout << str5 << endl;
	str2 = str3[4];
	cout << str2 << endl;

    return 0;
}