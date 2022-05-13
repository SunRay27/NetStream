#pragma once


//   LIST IS LIFE 
//
//   LIST IS WIFE




template <class T>
class List
{
	size_t curElement; size_t count;
	Node<T>* head, * tail, * current, * previous;

private:
#pragma region Low-level private functions

	//Basic field accessors
	void SetHeadNode(Node<T>* n) { head = n; }
	void SetTailNode(Node<T>* n) { tail = n; }
	void SetCurrentNode(Node<T>* n) { current = n; }
	//void SetCurrentValue(T n) { current->SetData(n); }
	void SetPreviousNode(Node<T>* n) { previous = n; }
	Node<T>* GetHeadNode() { return head; }
	Node<T>* GetTailNode() { return tail; }
	Node<T>* GetCurrentNode() { return current; }
	T& GetCurrentValue() { return GetCurrentNode()->GetData(); }
	Node<T>* GetPreviousNode() { return previous; }

	//Some basic movement logic
	Node <T>* GetNode(size_t i)
	{
		if (i<0 || i > count - 1)
			return nullptr;

		if (curElement > i)
			MoveToHead();

		for (size_t j = curElement; j < i; j++)
			MoveForward();

		return GetCurrentNode();
	}
	void MoveForward()
	{
		SetPreviousNode(GetCurrentNode());
		SetCurrentNode(GetCurrentNode()->GetNext());
		curElement++;
	}
	void MoveToHead()
	{
		SetPreviousNode(nullptr);
		SetCurrentNode(head);
		curElement = 0;
	}
	void MoveToTail()
	{
		MoveToHead();
		while (GetCurrentNode()->GetNext() != nullptr)
			MoveForward();
	}

	//and exception thrower
	void CheckBounds(size_t index) const
	{
		if (index < 0 || index > count - 1 || count == 0)
			throw std::exception("Index is out of bounds");
	}
#pragma endregion

	void QuickSortAscending(size_t first, size_t last)
	{
		if (first < last)
		{
			size_t left = first, right = last;
			T& middle = At((left + right) / 2);
			do
			{
				while (At(left) < middle) left++;
				while (At(right) > middle)
					//I don't use signed indexes
					// so I have nothing but to suffer
					if ((int)right - 1 > -1)
						right--;
				if (left <= right)
				{
					Swap(left, right);
					left++;
					//I don't use signed indexes
					// so I have nothing but to suffer
					if ((int)right - 1 > -1)
						right--;
				}
			} while (left <= right);


			QuickSortAscending(first, right);
			QuickSortAscending(left, last);
		}
	}
	void QuickSortDescending(size_t first, size_t last)
	{
		if (first < last)
		{
			size_t left = first, right = last;
			T& middle = At((left + right) / 2);
			do
			{
				while (At(left) > middle) left++;
				while (At(right) < middle) 					
					//I don't use signed indexes
					// so I have nothing but suffer
					if ((int)right - 1 > -1)
						right--;
				if (left <= right)
				{
					Swap(left, right);
					left++;
					//I don't use signed indexes
					// so I have nothing but suffer
					if ((int)right - 1 > -1)
						right--;
				}
			} while (left < right);


			QuickSortDescending(first, right);
			QuickSortDescending(left, last);
		}
	}

	void QuickSortPtrAscending(size_t first, size_t last)
	{
		if (first < last)
		{
			size_t left = first, right = last;
			T& middle = At((left + right) / 2);
			do
			{
				while (*At(left) < *middle) 
					left++;
				while (*At(right) > *middle) 
					//I don't use signed indexes
					// so I have nothing but suffer
					if ((int)right - 1 > -1)
						right--;

				if (left <= right)
				{
					Swap(left, right);
					left++;
					//I don't use signed indexes
					// so I have nothing but suffer
					if ((int)right - 1 > -1)
						right--;
				}
			} while (left <= right);


			QuickSortPtrAscending(first, right);
			QuickSortPtrAscending(left, last);
		}
	}
	void QuickSortPtrDescending(size_t first, size_t last)
	{
		if (first < last)
		{
			size_t left = first, right = last;
			T& middle = At((left + right) / 2);
			do
			{
				while (*At(left) > *middle) 
					left++;

				while (*At(right) < *middle)

					//I don't use signed indexes
					// so I have nothing but suffer
					if((int)right - 1 > -1)
						right--;
				
				if (left <= right)
				{
					Swap(left, right);
					left++;

					//I don't use signed indexes
					// so I have nothing but suffer
					if ((int)right - 1 > -1)
						right--;
				}
			} while (left < right);

			

			QuickSortPtrDescending(first, right);
			QuickSortPtrDescending(left, last);
		}
	}

public:
	~List() { Clear(); }
	List() : count(0), curElement(0), head(nullptr), tail(nullptr), current(nullptr), previous(nullptr) {}

	List(const std::initializer_list<T>& list) : List()
	{
		AddRange(list);
	}
	

	void RemoveAll(const T& value)
	{
		for (size_t i = 0; i < Count(); i++)
			if (value == At(i))
			{
				RemoveAt(i);
				i--;
			}
	}
	void RemoveAt(size_t index)
	{
		//Check if index is ok
		CheckBounds(index);

		Node<T>* previousNode = GetNode(index - 1);
		Node<T>* toDelete = GetNode(index);
		Node<T>* nextNode = GetNode(index + 1);

		//Fix head/tail pointers if toDelete element is head/tail
		if (toDelete == head)
			head = nextNode;
		if (toDelete == tail)
			tail = previousNode;

		//Connect previousNode with nextNode
		if (previousNode != nullptr)
			previousNode->SetNext(nextNode);

		//Fix curElement pointer (it might be toDelete node)
		MoveToHead();
		count--;
		delete toDelete;
	}

	void Reverse()
	{
		if (count < 2)
			return;

		size_t i = 0;           
		size_t j = count - 1;

		while (i < j) 
		{
			Swap(i, j);
			i++;
			j--;
		}
	}

	void Sort(bool ascending)
	{
		if (count == 0)
			return;

		if (ascending)
			QuickSortAscending(0, count - 1);
		else
			QuickSortDescending(0, count - 1);
	}

	void SortPtr(bool ascending)
	{
		if (count == 0)
			return;

		if (ascending)
			QuickSortPtrAscending(0, count - 1);
		else
			QuickSortPtrDescending(0, count - 1);
	}

	void Swap(size_t i, size_t j)
	{
		CheckBounds(i);
		CheckBounds(j);

		//Our job is done here
		if (j == i)
			return;

		//Make i < j because it matters
		if (i > j)
		{
			size_t t = i;
			i = j;
			j = t;
		}

		//Get all usefull nodes
		Node<T>* iNodePrevious = GetNode(i - 1);
		Node<T>* iNode = GetNode(i);
		Node<T>* iNodeNext = GetNode(i + 1);

		Node<T>* jNodePrevious = GetNode(j - 1);
		Node<T>* jNode = GetNode(j);
		Node<T>* jNodeNext = GetNode(j + 1);




		//swap head/tail if needed
		if (iNode == head)
			head = jNode;
		if (jNode == tail)
			tail = iNode;

		//fix node link which was before iNode
		if (iNodePrevious != nullptr)
			iNodePrevious->SetNext(jNode);

		//if nodes are neighbours we just reassign links
		if (i + 1 == j)
		{
			//just swap
			jNode->SetNext(iNode);
			iNode->SetNext(jNodeNext);
		}
		else
		{
			//fix node link which was before jNode
			if (jNodePrevious != nullptr)
				jNodePrevious->SetNext(iNode);
			//swap
			jNode->SetNext(iNodeNext);
			iNode->SetNext(jNodeNext);
		}

		MoveToHead();
	}

	bool Contains( T& value)
	{
		for (size_t i = 0; i < count; i++)
			if (value == At(i))
				return true;

		return false;
	}
	long IndexOf( T& value)
	{
		for (size_t i = 0; i < count; i++)
			if (value == At(i))
				return i;

		return -1;
	}


	void AddRange(const std::initializer_list<T>& list)
	{
		for (auto elem : list)
			Add(elem);
	}
	void AddRange(List<T>& listToAdd)
	{
		for (size_t i = 0; i < listToAdd.Count(); i++)
			Add(listToAdd[i]);
	}
	void Add(const T& value)
	{
		Node<T>* toAdd = new Node<T>(value);

		//Check if list is clean and if so, setup global pointers
		if (IsEmpty())
			head = current = previous = tail = toAdd;
		else
		{
			//or just add new value to tail
			tail->SetNext(toAdd);
			tail = toAdd;
			MoveToHead();
		}
		count++;

	}
	void Insert(const T& value, size_t index)
	{
		//Check if index is ok
		CheckBounds(index);

		MoveToHead();

		//previous can be nullptr!!!
		Node<T>* previousNode = GetNode(index - 1);
		Node<T>* moveNode = GetNode(index);
		Node<T>* toInsert = new Node<T>(value);

		//Set new links
		if (previousNode != nullptr)
			previousNode->SetNext(toInsert);
		toInsert->SetNext(moveNode);

		//set global pointers
		if (moveNode == head)
			head = toInsert;

		count++;
		MoveToHead();
	}


	T& At(size_t index)
	{
		CheckBounds(index);
		return GetNode(index)->GetData();
	}
	T& operator[] (size_t index) { return At(index); }

	void Clear()
	{
		//if head is nullptr, that means list is BROKEN, or just clean
		//But it defenetly cant be broken, I'm sure
		//if (head != nullptr)// ---> yeah! I'm so confident! YAY
			if (!IsEmpty())
			{
				//Delete all elements from head
				MoveToHead();
				do
				{
					MoveForward();
					delete GetPreviousNode();
				} while (GetCurrentNode() != nullptr);
			}
		count = 0;
		curElement = 0;
		head = nullptr;
	}
	bool IsEmpty()
	{
		return count < 1;
	}
	size_t Count()
	{
		return count;
	}


	//WHY IS IT HERE
	void Print()
	{
		MoveToHead();
		for (size_t i = 0; i < Count(); i++)
		{
			std::cout << GetCurrentValue() << ' ';
			MoveForward();
		}
		std::cout << endl;
	}

	//copy constructor
	List<T>(const List<T>& other) : List<T>()
	{
		Node<T>* currentCopy = nullptr;
		Node<T>* nextOriginal = nullptr;

		if (other.head != nullptr)
		{
			count++;
			head = new Node<T>(other.head->GetData());
			//head->SetData();
			//head->SetNext(other.head->GetNext());
			currentCopy = head;
			nextOriginal = other.head->GetNext();
		}
		while (nextOriginal != nullptr)
		{
			count++;
			Node<T>* newElement = new Node<T>(nextOriginal->GetData());
			currentCopy->SetNext(newElement);
			currentCopy = currentCopy->GetNext();
			nextOriginal = nextOriginal->GetNext();
		}
		tail = currentCopy;
		current = head;
	}
};