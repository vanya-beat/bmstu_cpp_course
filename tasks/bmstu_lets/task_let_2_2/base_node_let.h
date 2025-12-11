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
		ForwardListNode<T>* current = head;

		for (size_t i = 1; i < data.size(); ++i)
		{
			current->next = new ForwardListNode<T>(data[i]);
			current = current->next;
		}
	}
}

template <typename T>
void delete_list(ForwardListNode<T>*& head)
{
	if (head->next != nullptr)
		delete_list(head->next);
	delete head;
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
void reverse_list(ForwardListNode<T>*& head)
{
	std::vector<T> vec{};
	ForwardListNode<T>* current = head;

	while (current->next != nullptr)
	{
		vec.push_back(current->data);
		current = current->next;
	}
	vec.push_back(current->data);

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
void remove_duplicates(ForwardListNode<T>*& head)
{
	ForwardListNode<T>* current = head;
	ForwardListNode<T>* to_delete = {};

	while (current->next != nullptr)
	{
		if (current->data == current->next->data)
		{
			to_delete = current->next;
			current->next = current->next->next;
			delete to_delete;
		}
		current = current->next;
	}
}
