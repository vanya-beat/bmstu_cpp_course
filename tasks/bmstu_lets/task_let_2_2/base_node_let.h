#pragma once
#include <cmath>
#include <cstdint>
#include <sstream>
#include <string>
#include <vector>

template <typename T>
struct ForwardListNode
{
	T data;
	ForwardListNode* next = nullptr;
	ForwardListNode() = default;
	ForwardListNode(const T& data_) : data(data_) {}
	ForwardListNode(T&& data_) : data(std::move(data_)) {}
	ForwardListNode(const ForwardListNode& other) : data(other.data)
	{
		if (other.next)
		{
			next = new ForwardListNode(*other.next);
		}
	}
	ForwardListNode(ForwardListNode&& other) noexcept
		: data(std::move(other.data))
	{
		if (other.next)
		{
			next = other.next;
			other.next = nullptr;
		}
	}
	ForwardListNode(const T& data_, ForwardListNode* next_)
		: data(data_), next(next_)
	{
	}
	ForwardListNode(T&& data_, ForwardListNode* next_)
		: data(std::move(data_)), next(next_)
	{
	}
};

template <typename T>
void create_next(ForwardListNode<T>*& next,
				 const std::vector<T>& data,
				 size_t index)
{
	next = new ForwardListNode<T>(data[index]);
	if (index + 1 < data.size())
	{
		create_next(next->next, data, index + 1);
	}
}

template <typename T>
void create_list(ForwardListNode<T>*& head, const std::vector<T>& data)
{
	if (!data.empty())
	{
		head = new ForwardListNode<T>(data[0]);
		if (data.size() > 1)
			create_next(head->next, data, 1);
	}
}

template <typename T>
void delete_next(ForwardListNode<T>*& next)
{
	if (next->next != nullptr)
		delete_next(next->next);
	delete next;
}

template <typename T>
void delete_list(ForwardListNode<T>*& head)
{
	delete_next(head);
}

template <typename T>
void print_list(const ForwardListNode<T>* head, std::ostream& os)
{
	os << "[";
	while (head->next != nullptr)
	{
		os << head->data << ", ";
		head = head->next;
	}
	os << head->data << "]";
}

template <typename T>
std::vector<T> list_to_vector(ForwardListNode<T>*& next, std::vector<T> vec)
{
	if (next != nullptr)
	{
		vec.push_back(next->data);
		vec = list_to_vector(next->next, vec);
	}
	delete next;
	return vec;
}

template <typename T>
void reverse_list(ForwardListNode<T>*& head)
{
	std::vector<T> vec{};
	vec = list_to_vector(head, vec);
	size_t size = vec.size();
	T save{};
	for (size_t i = 0; i < size / 2; ++i)
	{
		save = vec[i];
		vec[i] = vec[size - i - 1];
		vec[size - i - 1] = save;
	}
	create_list(head, vec);
}

template <typename T>
void remove_duplicates_(ForwardListNode<T>*& next)
{
	if (next->next != nullptr)
	{
		if (next->data == next->next->data)
		{
			next->next = next->next->next;
		}
		remove_duplicates_(next->next);
	}
}

template <typename T>
void remove_duplicates(ForwardListNode<T>*& head)
{
	remove_duplicates_(head);
}
