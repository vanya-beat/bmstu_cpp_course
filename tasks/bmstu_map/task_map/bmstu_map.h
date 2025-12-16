#include <cstddef>
#include <cstdint>
#include <iostream>
#include <iterator>
#include <stack>
#include <stdexcept>
#include <utility>
#include "abstract_iterator.h"

namespace bmstu
{
// ==================== AVL Tree Node ====================
template <typename K, typename V>
struct tree_node
{
	tree_node(const K& k, const V& v)
		: key(k), value(v), left(nullptr), right(nullptr), height(1)
	{
	}

	K key;
	V value;
	uint8_t height;
	tree_node* left;
	tree_node* right;
};

// ==================== AVL Balanced Tree ====================
template <typename K, typename V>
class avl_balanced_tree
{
   public:
	avl_balanced_tree() : root_(nullptr), size_(0) {}
	~avl_balanced_tree() { clear(root_); }

	void insert(const K& key, const V& value)
	{
		root_ = insert(key, value, root_);
	}

	void remove(const K& key) { root_ = remove(key, root_); }

	tree_node<K, V>* find(const K& key) { return find(key, root_); }

	const tree_node<K, V>* find(const K& key) const { return find(key, root_); }

	bool contains(const K& key) const { return find(key) != nullptr; }

	size_t size() const { return size_; }

	bool empty() const { return size_ == 0; }

	tree_node<K, V>* get_root() { return root_; }

	const tree_node<K, V>* get_root() const { return root_; }

	void print() { print_tree_(root_, 1); }

	void inorder_print()
	{
		inorder_print(root_);
		std::cout << "\n";
	}

   private:
	uint8_t height(tree_node<K, V>* node) const
	{
		return node ? node->height : 0;
	}

	int getBalance(tree_node<K, V>* node) const
	{
		return node ? height(node->left) - height(node->right) : 0;
	}

	void updateHeight(tree_node<K, V>* node)
	{
		if (node)
		{
			node->height =
				1 + std::max(height(node->left), height(node->right));
		}
	}

	tree_node<K, V>* insert(const K& key, const V& value, tree_node<K, V>* node)
	{
		if (node == nullptr)
		{
			++size_;
			return new tree_node<K, V>(key, value);
		}

		if (key < node->key)
		{
			node->left = insert(key, value, node->left);
		}
		else if (key > node->key)
		{
			node->right = insert(key, value, node->right);
		}
		else
		{
			// Ключ уже существует, обновляем значение
			node->value = value;
			return node;
		}

		updateHeight(node);
		return balance(node);
	}

	tree_node<K, V>* remove(const K& key, tree_node<K, V>* node)
	{
		if (node == nullptr)
			return nullptr;

		if (key < node->key)
		{
			node->left = remove(key, node->left);
		}
		else if (key > node->key)
		{
			node->right = remove(key, node->right);
		}
		else
		{
			// Найден узел для удаления
			if ((node->left == nullptr) || (node->right == nullptr))
			{
				tree_node<K, V>* temp = node->left ? node->left : node->right;

				if (temp == nullptr)
				{
					temp = node;
					node = nullptr;
				}
				else
				{
					*node = *temp;
				}

				delete temp;
				--size_;
			}
			else
			{
				tree_node<K, V>* temp = findMinPtr(node->right);
				node->key = temp->key;
				node->value = temp->value;
				node->right = remove(temp->key, node->right);
			}
		}

		if (node == nullptr)
			return nullptr;

		updateHeight(node);
		return balance(node);
	}

	tree_node<K, V>* find(const K& key, tree_node<K, V>* node) const
	{
		if (node == nullptr)
			return nullptr;

		if (key < node->key)
			return find(key, node->left);
		else if (key > node->key)
			return find(key, node->right);
		else
			return node;
	}

	tree_node<K, V>* findMinPtr(tree_node<K, V>* node)
	{
		if (node == nullptr)
			return nullptr;

		while (node->left != nullptr)
			node = node->left;
		return node;
	}

	tree_node<K, V>* rotateWithLeftChild(tree_node<K, V>* k2)
	{
		tree_node<K, V>* k1 = k2->left;
		k2->left = k1->right;
		k1->right = k2;

		updateHeight(k2);
		updateHeight(k1);

		return k1;
	}

	tree_node<K, V>* rotateWithRightChild(tree_node<K, V>* k1)
	{
		tree_node<K, V>* k2 = k1->right;
		k1->right = k2->left;
		k2->left = k1;

		updateHeight(k1);
		updateHeight(k2);

		return k2;
	}

	tree_node<K, V>* doubleWithLeftChild(tree_node<K, V>* k3)
	{
		k3->left = rotateWithRightChild(k3->left);
		return rotateWithLeftChild(k3);
	}

	tree_node<K, V>* doubleWithRightChild(tree_node<K, V>* k1)
	{
		k1->right = rotateWithLeftChild(k1->right);
		return rotateWithRightChild(k1);
	}

	tree_node<K, V>* balance(tree_node<K, V>* node)
	{
		int balanceFactor = getBalance(node);

		if (balanceFactor > 1)
		{
			if (getBalance(node->left) >= 0)
				return rotateWithLeftChild(node);
			else
				return doubleWithLeftChild(node);
		}

		if (balanceFactor < -1)
		{
			if (getBalance(node->right) <= 0)
				return rotateWithRightChild(node);
			else
				return doubleWithRightChild(node);
		}

		return node;
	}

	void inorder_print(tree_node<K, V>* node)
	{
		if (node == nullptr)
			return;

		inorder_print(node->left);
		std::cout << "[" << node->key << ":" << node->value << "] ";
		inorder_print(node->right);
	}

	void clear(tree_node<K, V>* node)
	{
		if (node != nullptr)
		{
			clear(node->left);
			clear(node->right);
			delete node;
		}
	}

	void print_tree_(tree_node<K, V>* node, int space)
	{
		if (node == nullptr)
			return;

		space += 5;
		print_tree_(node->right, space);

		for (int i = 0; i < space; ++i)
			std::cout << " ";

		std::cout << node->key << ":" << node->value << "\n";
		print_tree_(node->left, space);
	}

	tree_node<K, V>* root_ = nullptr;
	size_t size_ = 0;
};

// ==================== Map Class ====================
template <typename K, typename V>
class map
{
   public:
	using key_type = K;
	using mapped_type = V;
	using value_type = std::pair<const K, V>;


	struct iterator : public abstract_iterator<iterator,
											   std::pair<const K, V>,
											   std::bidirectional_iterator_tag>
	{
		tree_node<K, V>* current_;
		std::stack<tree_node<K, V>*> stack_;

		iterator() : current_(nullptr) {}

		explicit iterator(tree_node<K, V>* root, bool is_end = false)
			: current_(nullptr)
		{
			if (!is_end && root)
			{
				// Находим самый левый узел для begin()
				tree_node<K, V>* node = root;
				while (node)
				{
					stack_.push(node);
					node = node->left;
				}

				if (!stack_.empty())
				{
					current_ = stack_.top();
					stack_.pop();
				}
			}
		}

		typename iterator::reference operator*() const override
		{
			// Используем временный статический объект
			static std::pair<const K, V> temp;
			// Используем const_cast для обновления через placement new
			new (const_cast<std::pair<const K, V>*>(&temp))
				std::pair<const K, V>(current_->key, current_->value);
			return temp;
		}

		typename iterator::pointer operator->() const override
		{
			return &(**this);
		}

		iterator& operator++() override
		{
			if (!current_)
				return *this;

			// Переходим к следующему узлу в порядке in-order
			if (current_->right)
			{
				tree_node<K, V>* node = current_->right;
				while (node)
				{
					stack_.push(node);
					node = node->left;
				}
			}

			if (stack_.empty())
			{
				current_ = nullptr;
			}
			else
			{
				current_ = stack_.top();
				stack_.pop();
			}

			return *this;
		}

		// Остальные методы остаются без изменений...

		iterator operator++(int) override
		{
			iterator temp = *this;
			++(*this);
			return temp;
		}

		iterator& operator--() override
		{
			// Реализация для bidirectional_iterator
			// В данной реализации можно оставить пустым
			return *this;
		}

		iterator operator--(int) override
		{
			iterator temp = *this;
			--(*this);
			return temp;
		}

		iterator& operator+=(
			const typename iterator::difference_type& n) override
		{
			for (int i = 0; i < n; ++i)
				++(*this);
			return *this;
		}

		iterator& operator-=(
			const typename iterator::difference_type& n) override
		{
			for (int i = 0; i < n; ++i)
				--(*this);
			return *this;
		}

		iterator operator+(
			const typename iterator::difference_type& n) const override
		{
			iterator temp = *this;
			temp += n;
			return temp;
		}

		iterator operator-(
			const typename iterator::difference_type& n) const override
		{
			iterator temp = *this;
			temp -= n;
			return temp;
		}

		bool operator==(const iterator& other) const override
		{
			return current_ == other.current_;
		}

		bool operator!=(const iterator& other) const override
		{
			return current_ != other.current_;
		}

		explicit operator bool() const override { return current_ != nullptr; }

		typename iterator::difference_type operator-(
			const iterator& other) const override
		{
			// Упрощенная реализация
			return 0;
		}
	};

	map() = default;
	~map() = default;

	void insert(const K& key, const V& value) { tree_.insert(key, value); }

	void insert(const value_type& pair)
	{
		tree_.insert(pair.first, pair.second);
	}

	V& operator[](const K& key)
	{
		auto node = tree_.find(key);
		if (node == nullptr)
		{
			tree_.insert(key, V());
			node = tree_.find(key);
		}
		return node->value;
	}

	V* find(const K& key)
	{
		auto node = tree_.find(key);
		return node ? &node->value : nullptr;
	}

	const V* find(const K& key) const
	{
		auto node = tree_.find(key);
		return node ? &node->value : nullptr;
	}

	V& at(const K& key)
	{
		auto node = tree_.find(key);
		if (node == nullptr)
		{
			throw std::out_of_range("Key not found in map");
		}
		return node->value;
	}

	const V& at(const K& key) const
	{
		auto node = tree_.find(key);
		if (node == nullptr)
		{
			throw std::out_of_range("Key not found in map");
		}
		return node->value;
	}

	// Удаление
	void erase(const K& key) { tree_.remove(key); }

	// Проверка наличия ключа
	bool contains(const K& key) const { return tree_.contains(key); }

	// Размер и проверка на пустоту
	size_t size() const { return tree_.size(); }

	bool empty() const { return tree_.empty(); }

	// Очистка
	void clear()
	{
		tree_.~avl_balanced_tree();
		new (&tree_) avl_balanced_tree<K, V>();
	}

	void print() { tree_.print(); }

	void inorder_print() { tree_.inorder_print(); }

	// Итераторы
	iterator begin() { return iterator(tree_.get_root(), false); }

	iterator end() { return iterator(nullptr, true); }

   private:
	avl_balanced_tree<K, V> tree_;
};

}  // namespace bmstu
