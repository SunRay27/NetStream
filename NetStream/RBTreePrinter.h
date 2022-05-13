#pragma once
#include <windows.h>

using namespace std;

template <typename TKey, typename TValue>
class RBTreePrinter
{
	struct DrawNode
	{
		DrawNode* previous;
		string printValue;

		DrawNode(DrawNode* previous, string printValue)
		{
			this->previous = previous;
			this->printValue = printValue;
		}
	};
	void DrawNodeRecursive(DrawNode* p)
	{
		if (p == nullptr) 
			return;
		
		DrawNodeRecursive(p->previous);
		cout << p->printValue;
	}



	List<DrawNode*> drawNodes;
	RBNode<TKey, TValue>* nil;
public:

	RBTreePrinter(RBNode<TKey, TValue>* nil)
	{
		this->nil = nil;
	}
	void PrintTree(RBNode<TKey, TValue>* currentNode, DrawNode* previousDrawNode, bool isLeft)
	{
		if (currentNode == nil)
			return;

		string prevValue = "    ";
		DrawNode* drawNode = new DrawNode(previousDrawNode, prevValue);
		drawNodes.Add(drawNode);
		PrintTree(currentNode->GetRight(), drawNode, true);

		//draw root
		if (!previousDrawNode)
			drawNode->printValue = "——";
		else if (isLeft)
		{
			//right node
			drawNode->printValue = ".——";
			prevValue = "   |";
		}
		else
		{
			//left node
			drawNode->printValue = "`——";
			previousDrawNode->printValue = prevValue;
		}

		DrawNodeRecursive(drawNode);

		HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);

		
		if (currentNode->IsBlack())
			SetConsoleTextAttribute(out, 0x0 | 0x70);//black on white
		else
			SetConsoleTextAttribute(out, 0x4 | 0x70);//red on white

		cout << " " << currentNode->GetKey() << " " << currentNode->GetValue() << endl;
		SetConsoleTextAttribute(out, 0x0 | 0x70);

		if (previousDrawNode)
			previousDrawNode->printValue = prevValue;

		drawNode->printValue = "   |";

		PrintTree(currentNode->GetLeft(), drawNode, false);
	}

	~RBTreePrinter()
	{
		for (size_t i = 0; i < drawNodes.GetSize(); i++)
			delete drawNodes[i];
	}
};



