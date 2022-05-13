#pragma once
#include <string>

#pragma region Tree_Iterator_classes
template <class TKey, class TValue>
class Iterator
{
public:
	virtual RBNode<TKey, TValue>* Next() = 0;
	virtual bool HasNext() = 0;
};
template <class TKey, class TValue>
class TreeDFTIterator : public Iterator<TKey, TValue>
{
private:
	Stack<RBNode<TKey, TValue>*> stack;
	RBNode<TKey, TValue>* nil;
	void PushToLeft(RBNode<TKey, TValue>* node)
	{
		if (node == nil)
			return;

		stack.Push(node);
		PushToLeft(node->GetLeft());
	}
public:
	TreeDFTIterator(RBNode<TKey, TValue>* start, RBNode<TKey, TValue>* nil)
	{
		this->nil = nil;

		PushToLeft(start);
	}
	RBNode<TKey, TValue>* Next() override
	{
		RBNode<TKey, TValue>* next = stack.Pop();
		PushToLeft(next->GetRight());
		return next;
	}
	bool HasNext() override
	{
		return stack.GetSize() != 0;
	}
};
template <class TKey, class TValue>
class TreeBFTIterator : public Iterator<TKey, TValue>
{
private:
	Queue<RBNode<TKey, TValue>*> queue;
	RBNode<TKey, TValue>* nil;
public:
	TreeBFTIterator(RBNode<TKey, TValue>* start, RBNode<TKey, TValue>* nil)
	{
		this->nil = nil;
		if (start == nil)
			return;

		queue.Enqueue(start);
	}
	RBNode<TKey, TValue>* Next() override
	{
		RBNode<TKey, TValue>* next = queue.Dequeue();
		if (next->GetLeft() != nil)
			queue.Enqueue(next->GetLeft());

		if (next->GetRight() != nil)
			queue.Enqueue(next->GetRight());

		return next;
	}
	bool HasNext() override
	{
		return queue.GetSize() != 0;
	}
};
template <typename TKey, typename TValue>
#pragma endregion 

class Map
{
private:
	RBNode<TKey, TValue>* root;
	RBNode<TKey, TValue>* nil;

	RBNode<TKey, TValue>* minKeyNode(RBNode<TKey, TValue>* root)
	{
		RBNode<TKey, TValue>* minv = root;
		while (minv->GetLeft() != nil)
		{
			minv = root->GetLeft();
		}
		return minv;
	}
	RBNode<TKey, TValue>* findNode(TKey key)
	{
		RBNode<TKey, TValue>* current = root;
		while (current != nil) {
			if (key == current->GetKey())
				return current;
			else
			{
				if (key < current->GetKey())
					current = current->GetLeft();
				else
					current = current->GetRight();
			}
		}
		throw invalid_argument("key " + std::to_string(key) + " is not present in collection");
	}
#pragma region Tree_transformations

	//i can't drown my demons, they know how to swim...
	void TransplantNode(RBNode<TKey, TValue>* value, RBNode<TKey, TValue>* replacedBy)
	{
		if (value->GetParent() == nil)
			root = replacedBy;
		else if (value == value->GetParent()->GetLeft())
			value->GetParent()->SetLeft(replacedBy);
		else
			value->GetParent()->SetRight(replacedBy);

		replacedBy->SetParent(value->GetParent());
	}
	void LeftRotate(RBNode<TKey, TValue>* node)
	{
		RBNode<TKey, TValue>* rightChild = node->GetRight();
		node->SetRight(rightChild->GetLeft());

		if (rightChild->GetLeft() != nil)
			rightChild->GetLeft()->SetParent(node);

		rightChild->SetParent(node->GetParent());

		if (node->GetParent() == nil)
			root = rightChild;
		else if (node == node->GetParent()->GetLeft())
			node->GetParent()->SetLeft(rightChild);
		else
			node->GetParent()->SetRight(rightChild);

		rightChild->SetLeft(node);
		node->SetParent(rightChild);
	}
	void RightRotate(RBNode<TKey, TValue>* node)
	{
		RBNode<TKey, TValue>* leftChild = node->GetLeft();
		node->SetLeft(leftChild->GetRight());

		if (leftChild->GetRight() != nil)
			leftChild->GetRight()->SetParent(node);

		leftChild->SetParent(node->GetParent());

		if (node->GetParent() == nil)
			root = leftChild;
		else if (node == node->GetParent()->GetRight())
			node->GetParent()->SetRight(leftChild);
		else
			node->GetParent()->SetLeft(leftChild);

		leftChild->SetRight(node);
		node->SetParent(leftChild);
	}
	void RebuildTreeInsert(RBNode<TKey, TValue>* currentNode)
	{
		RBNode<TKey, TValue>* uncle = nil;
		while (!currentNode->GetParent()->IsBlack())
		{
			if (currentNode->GetParent() != currentNode->GetGrandparent()->GetLeft())
			{
				uncle = currentNode->GetGrandparent()->GetLeft();

				if (!uncle->IsBlack())
				{
					uncle->SetBlack();
					currentNode->GetParent()->SetBlack();
					currentNode->GetGrandparent()->SetRed();
					currentNode = currentNode->GetGrandparent();
				}
				else
				{
					if (currentNode == currentNode->GetParent()->GetLeft())
					{
						currentNode = currentNode->GetParent();
						RightRotate(currentNode);
					}
					currentNode->GetParent()->SetBlack();
					currentNode->GetGrandparent()->SetRed();
					LeftRotate(currentNode->GetGrandparent());
				}
			}
			else
			{
				uncle = currentNode->GetGrandparent()->GetRight();

				if (!uncle->IsBlack()) {
					uncle->SetBlack();
					currentNode->GetParent()->SetBlack();
					currentNode->GetGrandparent()->SetRed();
					currentNode = currentNode->GetGrandparent();
				}
				else
				{
					if (currentNode != currentNode->GetParent()->GetLeft())
					{
						currentNode = currentNode->GetParent();
						LeftRotate(currentNode);
					}
					currentNode->GetParent()->SetBlack();
					currentNode->GetGrandparent()->SetRed();
					RightRotate(currentNode->GetGrandparent());
				}
			}
			if (currentNode == root)
				break;
		}
		root->SetBlack();
	}
	void RebuildTreeDelete(RBNode<TKey, TValue>* currentNode)
	{
		RBNode<TKey, TValue>* sibling;
		while (currentNode != root && currentNode->IsBlack())
		{
			if (currentNode == currentNode->GetParent()->GetLeft())
			{
				sibling = currentNode->GetParent()->GetRight();

				if (sibling->IsBlack() == false)
				{
					sibling->SetBlack();
					currentNode->GetParent()->SetRed();
					LeftRotate(currentNode->GetParent());
					sibling = currentNode->GetParent()->GetRight();
				}
				if (sibling->GetLeft()->IsBlack() && sibling->GetRight()->IsBlack())
				{
					sibling->SetRed();
					currentNode = currentNode->GetParent();
				}
				else if (sibling->GetRight()->IsBlack())
				{
					sibling->GetLeft()->SetBlack();
					sibling->SetRed();
					RightRotate(sibling);
					currentNode->GetParent()->SetRight(sibling);
				}
				else
				{
					sibling->SetIsBlack(currentNode->GetParent()->IsBlack());
					currentNode->GetGrandparent()->SetBlack();
					sibling->GetRight()->SetBlack();
					LeftRotate(currentNode->GetParent());
					currentNode = root;
				}
			}
			else
			{
				sibling = currentNode->GetParent()->GetLeft();

				if (sibling->IsBlack() == false)
				{
					sibling->SetBlack();
					currentNode->GetParent()->SetRed();
					RightRotate(currentNode->GetParent());
					sibling = currentNode->GetParent()->GetLeft();
				}
				if (sibling->GetLeft()->IsBlack() && sibling->GetRight()->IsBlack())
				{
					sibling->SetRed();
					currentNode = currentNode->GetParent();
				}
				else if (sibling->GetLeft()->IsBlack())
				{
					sibling->GetRight()->SetBlack();
					sibling->SetRed();
					LeftRotate(sibling);
					currentNode->GetParent()->SetLeft(sibling);
				}
				else
				{
					sibling->SetIsBlack(currentNode->GetParent()->IsBlack());
					currentNode->GetGrandparent()->SetBlack();
					sibling->GetLeft()->SetBlack();
					RightRotate(currentNode->GetParent());
					currentNode = root;
				}
			}
		}
		currentNode->SetBlack();
	}
#pragma endregion

public:

	Map()
	{
		nil = new RBNode<TKey, TValue>();

		//nil node pointer?
		nil->SetLeft(nil);
		nil->SetRight(nil);
		root = nil;
	}
	~Map()
	{
		Clear();
		delete nil;
	}

	void Insert(const TKey& key, const TValue& value)
	{
		if (GetKeys().Contains(key))
			throw std::invalid_argument("key " + std::to_string(key) + " is already present");

		RBNode<TKey, TValue>* newNode = new RBNode<TKey, TValue>(false, key, value, nil, nil, nil);
		RBNode<TKey, TValue>* curNode = root;
		RBNode<TKey, TValue>* target = curNode;

		//get node to attach new node to
		while (curNode != nil)
		{
			target = curNode;
			if (newNode->GetKey() < curNode->GetKey())
				curNode = curNode->GetLeft();
			else
				curNode = curNode->GetRight();
		}
		//no such node - map is empty
		if (target == nil)
		{
			newNode->SetBlack();
			root = newNode;
			return;
		}


		newNode->SetParent(target);

		//decide new node left/right position
		if (newNode->GetKey() < target->GetKey())
			target->SetLeft(newNode);
		else
			target->SetRight(newNode);

		//tree contains only 2 elements so we don't need to rebuild it
		if (newNode->GetGrandparent() == nil)
			return;

		RebuildTreeInsert(newNode);
	}
	TValue& Find(TKey key)
	{
		return findNode(key)->GetValue();
		throw invalid_argument("key " + std::to_string(key) + " is not present in collection");
	}
	List<TKey> GetKeys()
	{
		//just get 'em all via bft
		List<TKey> keys;
		Iterator<TKey, TValue>* bft = CreateBFTIterator();

		while (bft->HasNext())
			keys.Add(bft->Next()->GetKey());

		delete bft;
		return keys;
	}
	List<TValue> GetValues()
	{
		//just get 'em all via bft
		List<TValue> values;
		TreeBFTIterator<TKey, TValue>* bft = CreateBFTIterator();

		while (bft->HasNext())
			values.Add(bft->Next()->GetValue());

		delete bft;
		return values;
	}

	void Remove(const TKey& key)
	{
		RBNode<TKey, TValue>* toDelete = findNode(key);
		bool originalColorIsBlack = toDelete->IsBlack();
		RBNode<TKey, TValue>* child;

		//transplant nil nodes
		if (toDelete->GetLeft() == nil)
		{
			child = toDelete->GetRight();
			TransplantNode(toDelete, child);
		}
		else if (toDelete->GetRight() == nil)
		{
			child = toDelete->GetLeft();
			TransplantNode(toDelete, child);
		}
		else
		{
			//find smallest key in right subtree and save its color
			RBNode<TKey, TValue>* minNode = minKeyNode(toDelete->GetRight());
			originalColorIsBlack = minNode->IsBlack();
			child = minNode->GetRight();

			if (minNode->GetParent() != toDelete)
			{
				TransplantNode(minNode, minNode->GetRight());
				minNode->SetRight(toDelete->GetRight());
				minNode->GetRight()->SetParent(minNode);
			}

			TransplantNode(toDelete, minNode);
			minNode->SetLeft(toDelete->GetLeft());
			minNode->GetLeft()->SetParent(minNode);

			if (!originalColorIsBlack)
				minNode->SetBlack();
		}

		//delete target node
		toDelete->SetLeft(nullptr);
		toDelete->SetRight(nullptr);
		delete toDelete;

		//rebuild tree
		if (originalColorIsBlack)
			RebuildTreeDelete(child);
	}
	void Clear()
	{
		//just iterate and delete every node
		Iterator<TKey, TValue>* bft = CreateBFTIterator();
		while (bft->HasNext())
			delete bft->Next();

		delete bft;

		//except nil node
		root = nil;
	}
	bool Contains(const TKey& key)
	{
		//bin search
		RBNode<TKey, TValue>* current = root;
		while (current != nil) {
			if (key == current->GetKey())
				return true;
			else
			{
				if (key < current->GetKey())
					current = current->GetLeft();
				else
					current = current->GetRight();
			}
		}
		return false;
	}
	bool ContainsValue(const TValue& value)
	{
		TreeBFTIterator<TKey, TValue>* iterator = CreateBFTIterator();

		while (iterator->HasNext())
		{
			if (iterator->Next()->GetValue() == value)
			{

				delete iterator;
				return true;
			}
		}
		delete iterator;
		return false;
	}
	void Print()
	{
		RBTreePrinter<TKey, TValue> a(nil);
		cout << endl << endl;
		a.PrintTree(root, nullptr, false);
	}

	TreeDFTIterator<TKey, TValue>* CreateDFTIterator() { return new TreeDFTIterator<TKey, TValue>(root, nil); }
	TreeBFTIterator<TKey, TValue>* CreateBFTIterator() { return new TreeBFTIterator<TKey, TValue>(root, nil); }
};