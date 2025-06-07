#include "gtest/gtest.h"

#include "base_node_let.h"

TEST(ForwardDummyList, CreateList)
{
    ForwardListNode<int>* head = nullptr;
    std::vector<int> data = {1, 2, 3, 4, 5};

    create_list(head, data);

    ForwardListNode<int>* current = head;
    for (const auto& value : data)
    {
        ASSERT_NE(current, nullptr);
        EXPECT_EQ(current->data, value);
        current = current->next;
    }

    EXPECT_EQ(current, nullptr);

    delete_list(head);
}

TEST(ForwardDummyList, PrintList)
{
	ForwardListNode<int>* head = nullptr;
	std::vector<int> data = {1, 2, 3, 4, 5};
	create_list(head, data);

	std::stringstream output;
	print_list(head, output);

	EXPECT_EQ(output.str(), "[1, 2, 3, 4, 5]");

	delete_list(head);
}

TEST(ForwardDummyList, PrintList1)
{
	ForwardListNode<int>* head = nullptr;
	std::vector<int> data = {1};
	create_list(head, data);

	std::stringstream output;
	print_list(head, output);

	EXPECT_EQ(output.str(), "[1]");

	delete_list(head);
}

TEST(ForwardDummyList, ReverseList)
{
	ForwardListNode<int>* head = nullptr;
	std::vector<int> data = {1, 2, 3, 4, 5};
	create_list(head, data);

	reverse_list(head);

	std::vector<int> reversed_data = {5, 4, 3, 2, 1};
	ForwardListNode<int>* current = head;
	for (const auto& value : reversed_data)
	{
		ASSERT_NE(current, nullptr);
		EXPECT_EQ(current->data, value);
		current = current->next;
	}

	EXPECT_EQ(current, nullptr);

	delete_list(head);
}

TEST(ForwardDummyList, RemoveDuplicates)
{
	ForwardListNode<int>* head = nullptr;
	std::vector<int> data = {1, 2, 2, 3, 4, 4, 5};
	create_list(head, data);

	remove_duplicates(head);

	std::vector<int> unique_data = {1, 2, 3, 4, 5};
	ForwardListNode<int>* current = head;
	for (const auto& value : unique_data)
	{
		ASSERT_NE(current, nullptr);
		EXPECT_EQ(current->data, value);
		current = current->next;
	}

	EXPECT_EQ(current, nullptr);

	delete_list(head);
}