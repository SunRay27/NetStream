#pragma once


template <class T>
class Queue
{
	size_t count;
	Node<T>* last, * first;

public:
	~Queue()
	{
		Clear();
	}
	Queue() : count(0), last(nullptr), first(nullptr) {}


	void Enqueue(const T& value)
	{
		Node<T>* newNode = new Node<T>(value);

		if (count != 0)
		{
			last->SetNext(newNode);
			last = newNode;
		}
		else
			first = last = newNode;

		count++;
	}
	T Dequeue()
	{
		Node<T>* toPop = first;

		if (toPop != nullptr)
		{
			T data = toPop->GetData();
			first = toPop->GetNext();
			count--;

			delete toPop;
			return data;
		}
		else
			throw std::exception("Can't dequeue empty queue!");
	}
	T& Peek()
	{
		Node<T>* toPeek = first;

		if (toPeek != nullptr)
			return toPeek->GetData();
		else
			throw std::exception("Can't peek empty queue!");
	}


	size_t Count()
	{
		return count;
	}
	bool IsEmpty()
	{
		return count < 1;
	}
	void Clear()
	{
		//if (!IsEmpty())
			while (first != nullptr)
			{
				Node<T>* nextNode = first->GetNext();
				delete first;
				first = nextNode;
			}
		count = 0;
	}



	//WHY IS IT HERE
	void Print()
	{
		Node<T>* nextNode = first;
		while (nextNode != nullptr)
		{
			std::cout << nextNode->GetData() << ' ';
			nextNode = nextNode->GetNext();
		}
		std::cout << endl;
	}

	//copy constructor
	Queue<T>(const Queue<T>& other) : Queue<T>()
	{
		Node<T>* currentCopy = nullptr;
		Node<T>* nextOriginal = nullptr;

		if (other.first != nullptr)
		{
			count++;
			first = new Node<T>(other.first->GetData());
			currentCopy = first;
			nextOriginal = other.first->GetNext();
		}
		while (nextOriginal != nullptr)
		{
			count++;
			Node<T>* newElement = new Node<T>(nextOriginal->GetData());
			currentCopy->SetNext(newElement);
			currentCopy = currentCopy->GetNext();
			nextOriginal = nextOriginal->GetNext();
		}
		last = currentCopy;
	}

};