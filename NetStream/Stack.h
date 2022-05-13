#pragma once

template <class T>
class Stack
{
	size_t count;
	Node<T>* last;

public:
	~Stack()
	{
		Clear();
	}
	Stack() : count(0), last(nullptr) {}


	void Push(const T& value)
	{
		Node<T>* newNode = new Node<T>(value);

		if (last != nullptr)
			newNode->SetNext(last);

		last = newNode;
		count++;
	}
	T Pop()
	{
		Node<T>* toPop = last;

		if (toPop != nullptr)
		{
			T data = toPop->GetData();
			last = toPop->GetNext();
			count--;

			delete toPop;
			return data;
		}
		else
			throw std::exception("Can't pop empty stack!");


	}
	T& Peek()
	{
		Node<T>* toPeek = last;

		if (toPeek != nullptr)
			return toPeek->GetData();
		else
			throw std::exception("Can't peek empty stack!");
	}

	void Clear()
	{
		while (last != nullptr)
		{
			Node<T>* prevNode = last->GetNext();
			delete last;
			last = prevNode;
		}
		count = 0;
	}

	//WHY IS IT HERE
	void Print()
	{
		Node<T>* nextNode = last;
		while (nextNode != nullptr)
		{
			std::cout << nextNode->GetData() << ' ';
			nextNode = nextNode->GetNext();
		}
		std::cout << endl;
	}

	bool IsEmpty()
	{
		return count < 1;
	}
	size_t Count()
	{
		return count;
	}


	Stack<T>(const Stack<T>& other) : Stack<T>()
	{
		Node<T>* currentCopy = nullptr;
		Node<T>* nextOriginal = nullptr;

		if (other.last != nullptr)
		{
			count++;
			last = new Node<T>(other.last->GetData());
			currentCopy = last;
			nextOriginal = other.last->GetNext();
		}
		while (nextOriginal != nullptr)
		{
			count++;
			Node<T>* newElement = new Node<T>(nextOriginal->GetData());
			currentCopy->SetNext(newElement);
			currentCopy = currentCopy->GetNext();
			nextOriginal = nextOriginal->GetNext();
		}
		//last = currentCopy;
	}
};