/*
 * ЗАДАНИЕ: Реализация std::map на основе AVL-дерева
 *
 * Цель: Создать полноценный аналог std::map с использованием
 * самобалансирующегося AVL-дерева в качестве внутренней структуры данных.
 *
 * Что нужно реализовать:
 *
 * 1. AVL Tree (avl_balanced_tree):
 *    - insert() - вставка пары ключ-значение с балансировкой
 *    - remove() - удаление по ключу с балансировкой
 *    - find() - поиск узла по ключу
 *    - balance() - балансировка дерева (проверка высот и вызов поворотов)
 *    - Повороты: rotateWithLeftChild, rotateWithRightChild,
 *                doubleWithLeftChild, doubleWithRightChild
 *    - Вспомогательные: heightOfTree(), findMinPtr()
 *
 * 2. Iterator (map::iterator):
 *    - Конструктор для инициализации (найти самый левый узел для begin)
 *    - operator*() - разыменование (вернуть std::pair<const K, V>)
 *    - operator++() - переход к следующему элементу в in-order обходе
 *    - Использовать std::stack для обхода дерева
 *
 * 3. Map (map):
 *    - Все публичные методы уже реализованы и используют AVL дерево
 *    - После реализации AVL дерева и итератора, map будет работать полностью
 *
 * Особенности AVL дерева:
 * - Для каждого узла разность высот левого и правого поддеревьев <= 1
 * - После вставки/удаления может потребоваться 1-2 поворота для балансировки
 * - 4 случая дисбаланса: LL, LR, RR, RL (Left-Left, Left-Right, и т.д.)
 *
 * Тестирование:
 * - Запустите тесты: ./tasks/bmstu_map/bmstu_map
 * - Все 18 тестов должны пройти успешно после полной реализации
 */

#include <algorithm>
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
// ЗАДАНИЕ ДЛЯ СТУДЕНТОВ:
// Реализуйте самобалансирующееся AVL-дерево с поддержкой вставки, удаления и
// поиска по ключам. Дерево должно автоматически балансироваться после каждой
// операции.
template <typename K, typename V>
class avl_balanced_tree
{
   public:
	avl_balanced_tree() : root_(nullptr), size_(0) {}
	~avl_balanced_tree() { clear(root_); }

	void insert(const K& key, const V& value)
	{
		this->insert(key, value, root_);
	}

	void remove(const K& key) { this->remove(key, root_); }

	tree_node<K, V>* find(const K& key) { return this->find(key, root_); }

	const tree_node<K, V>* find(const K& key) const
	{
		return this->find(key, root_);
	}

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
	tree_node<K, V>* insert(const K& key,
							const V& value,
							tree_node<K, V>*& node)
	{
		// TODO: Реализовать вставку узла в AVL дерево
		// 1. Если node == nullptr, создать новый узел
		// 2. Рекурсивно вставить в левое или правое поддерево в зависимости от
		// ключа
		// 3. Обновить значение, если ключ уже существует
		// 4. Вызвать balance(node) для балансировки
		if (node == nullptr)
		{
			node = new tree_node<K, V>(key, value);
			size_++;
		}
		else if (key < node->key)
		{
			insert(key, value, node->left);
		}
		else if (key > node->key)
		{
			insert(key, value, node->right);
		}
		else
		{
			node->value = value;
		}
		balance(node);
		return node;
	}

	void remove(const K& key, tree_node<K, V>*& node)
	{
		// TODO: Реализовать удаление узла из AVL дерева
		// 1. Найти узел с заданным ключом
		// 2. Обработать 4 случая:
		//    - Узел без детей (лист)
		//    - Узел с одним ребенком (левым или правым)
		//    - Узел с двумя детьми (заменить на минимальный из правого
		//    поддерева)
		// 3. Декрементировать size_
		// 4. Вызвать balance(node) для балансировки
		if (node == nullptr)
		{
			return;
		}
		if (key < node->key)
		{
			remove(key, node->left);
		}
		else if (key > node->key)
		{
			remove(key, node->right);
		}
		else
		{
			if (node->left != nullptr && node->right != nullptr)
			{
				tree_node<K, V>* min_right = findMinPtr(node->right);
				node->key = min_right->key;
				node->value = min_right->value;
				remove(node->key, node->right);
			}
			else
			{
				tree_node<K, V>* old_node = node;
				node = (node->left != nullptr) ? node->left : node->right;
				delete old_node;
				size_--;
			}
		}
		balance(node);
	}

	tree_node<K, V>* find(const K& key, tree_node<K, V>* node) const
	{
		// TODO: Реализовать поиск узла по ключу
		// Рекурсивно искать в левом или правом поддереве
		if (node == nullptr)
		{
			return nullptr;
		}
		if (key < node->key)
		{
			return find(key, node->left);
		}
		else if (key > node->key)
		{
			return find(key, node->right);
		}
		else
		{
			return node;
		}
	}

	tree_node<K, V>* findMinPtr(tree_node<K, V>* node)
	{
		// TODO: Найти узел с минимальным ключом в поддереве
		// Подсказка: идти влево, пока возможно
		if (node == nullptr)
		{
			return nullptr;
		}
		while (node->left != nullptr)
		{
			node = node->left;
		}
		return node;
	}

	uint8_t heightOfTree(tree_node<K, V>* t)
	{
		// TODO: Вычислить высоту дерева
		// Высота пустого дерева = 0
		// Высота дерева = 1 + max(высота левого, высота правого)
		return t == nullptr ? 0 : t->height;
	}

	void rotateWithLeftChild(tree_node<K, V>*& k2)
	{
		// TODO: Реализовать правый поворот (rotation with left child)
		// Используется для балансировки Left-Left случая
		tree_node<K, V>* k1 = k2->left;
		k2->left = k1->right;
		k1->right = k2;
		k2->height =
			std::max(heightOfTree(k2->left), heightOfTree(k2->right)) + 1;
		k1->height = std::max(heightOfTree(k1->left), k2->height) + 1;
		k2 = k1;
	}

	void rotateWithRightChild(tree_node<K, V>*& k1)
	{
		// TODO: Реализовать левый поворот (rotation with right child)
		// Используется для балансировки Right-Right случая
		tree_node<K, V>* k2 = k1->right;
		k1->right = k2->left;
		k2->left = k1;
		k1->height =
			std::max(heightOfTree(k1->left), heightOfTree(k1->right)) + 1;
		k2->height = std::max(heightOfTree(k2->right), k1->height) + 1;
		k1 = k2;
	}

	void doubleWithLeftChild(tree_node<K, V>*& k3)
	{
		// TODO: Реализовать двойной поворот Left-Right
		// Сначала левый поворот на левом ребенке, затем правый поворот на узле
		rotateWithRightChild(k3->left);
		rotateWithLeftChild(k3);
	}

	void doubleWithRightChild(tree_node<K, V>*& k1)
	{
		// TODO: Реализовать двойной поворот Right-Left
		// Сначала правый поворот на правом ребенке, затем левый поворот на узле
		rotateWithLeftChild(k1->right);
		rotateWithRightChild(k1);
	}

	void balance(tree_node<K, V>*& t)
	{
		// TODO: Реализовать балансировку AVL дерева
		// 1. Вычислить разность высот левого и правого поддеревьев
		// 2. Если разность > 1, выполнить соответствующие повороты:
		//    - Left-Left: rotateWithLeftChild
		//    - Left-Right: doubleWithLeftChild
		//    - Right-Right: rotateWithRightChild
		//    - Right-Left: doubleWithRightChild
		if (t == nullptr)
		{
			return;
		}
		if (static_cast<int>(heightOfTree(t->left)) -
				static_cast<int>(heightOfTree(t->right)) >
			1)
		{
			if (heightOfTree(t->left->left) >= heightOfTree(t->left->right))
			{
				rotateWithLeftChild(t);
			}
			else
			{
				doubleWithLeftChild(t);
			}
		}
		else if (static_cast<int>(heightOfTree(t->right)) -
					 static_cast<int>(heightOfTree(t->left)) >
				 1)
		{
			if (heightOfTree(t->right->right) >= heightOfTree(t->right->left))
			{
				rotateWithRightChild(t);
			}
			else
			{
				doubleWithRightChild(t);
			}
		}
		t->height = std::max(heightOfTree(t->left), heightOfTree(t->right)) + 1;
	}

	void inorder_print(tree_node<K, V>* node)
	{
		if (node == nullptr)
		{
			return;
		}
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
		{
			return;
		}
		space += 5;
		this->print_tree_(node->right, space);
		for (int i = 0; i < space; ++i)
		{
			std::cout << " ";
		}
		std::cout << node->key << ":" << node->value << "\n";
		this->print_tree_(node->left, space);
	}

	tree_node<K, V>* root_ = nullptr;
	size_t size_ = 0;
};

// ==================== Map Class ====================
// ЗАДАНИЕ ДЛЯ СТУДЕНТОВ:
// Используя реализованное AVL-дерево, создайте полноценный аналог std::map
// с поддержкой вставки, удаления, поиска и итерации по элементам в порядке
// возрастания ключей.
template <typename K, typename V>
class map
{
   public:
	using key_type = K;
	using mapped_type = V;
	using value_type = std::pair<const K, V>;

	// ==================== Iterator ====================
	// ЗАДАНИЕ ДЛЯ СТУДЕНТОВ:
	// Реализуйте итератор для обхода элементов map в порядке возрастания ключей
	// (in-order обход AVL дерева). Итератор должен поддерживать операции
	// инкремента и разыменования.
	struct iterator : public abstract_iterator<iterator,
											   std::pair<const K, V>,
											   std::bidirectional_iterator_tag>
	{
		tree_node<K, V>* current_;
		std::stack<tree_node<K, V>*> stack_;
		mutable typename iterator::value_type pair_cache_;

		iterator() : current_(nullptr), pair_cache_(K{}, V{}) {}

		explicit iterator(tree_node<K, V>* root, bool is_end = false)
			: current_(nullptr), pair_cache_(K{}, V{})
		{
			// TODO: Реализовать инициализацию итератора
			// Для begin(): нужно найти самый левый узел
			// Для end(): current_ должен остаться nullptr
			if (!is_end && root)
			{
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
			// TODO: Реализовать разыменование
			// Нужно вернуть std::pair<const K, V> с текущим ключом и значением
			if (current_)
			{
				const_cast<K&>(pair_cache_.first) = current_->key;
				pair_cache_.second = current_->value;
			}
			return pair_cache_;
		}

		typename iterator::pointer operator->() const override
		{
			// TODO: Реализовать оператор стрелки
			if (current_)
			{
				const_cast<K&>(pair_cache_.first) = current_->key;
				pair_cache_.second = current_->value;
			}
			return &pair_cache_;
		}

		iterator& operator++() override
		{
			// TODO: Реализовать переход к следующему элементу (in-order)
			// Подсказка: используйте стек для обхода дерева
			if (current_ == nullptr)
				return *this;

			if (current_->right)
			{
				tree_node<K, V>* node = current_->right;
				while (node)
				{
					stack_.push(node);
					node = node->left;
				}
			}

			if (!stack_.empty())
			{
				current_ = stack_.top();
				stack_.pop();
			}
			else
			{
				current_ = nullptr;
			}
			return *this;
		}

		iterator operator++(int) override
		{
			iterator temp = *this;
			++(*this);
			return temp;
		}

		iterator& operator--() override
		{
			// Необязательно для bidirectional_iterator в этой реализации
			return *this;
		}

		iterator operator--(int) override
		{
			// Необязательно для bidirectional_iterator в этой реализации
			return *this;
		}

		iterator& operator+=(
			const typename iterator::difference_type& n) override
		{
			// Не требуется для bidirectional_iterator
			return *this;
		}

		iterator& operator-=(
			const typename iterator::difference_type& n) override
		{
			// Не требуется для bidirectional_iterator
			return *this;
		}

		iterator operator+(
			const typename iterator::difference_type& n) const override
		{
			// Не требуется для bidirectional_iterator
			return *this;
		}

		iterator operator-(
			const typename iterator::difference_type& n) const override
		{
			// Не требуется для bidirectional_iterator
			return *this;
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
			// Не требуется точное вычисление для bidirectional_iterator
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

	iterator end() { return iterator(tree_.get_root(), true); }

   private:
	avl_balanced_tree<K, V> tree_;
};

}  // namespace bmstu
