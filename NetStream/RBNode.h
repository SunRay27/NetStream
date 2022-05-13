



template <class TKey, class TValue>
class RBNode
{
	bool isBlack;
	TKey key;
	TValue value;
	RBNode<TKey, TValue>* left, * right, * parent;


public:
	RBNode()
	{
		this->isBlack = true;
		this->key = TKey();
		this->value = TValue();
		this->parent = nullptr;
		this->left = nullptr;
		this->right = nullptr;
	}
	RBNode(bool isBlack, const TKey key, const TValue value, RBNode<TKey, TValue>* parent = nullptr, RBNode<TKey, TValue>* left = nullptr, RBNode<TKey, TValue>* right = nullptr)
	{
		this->isBlack = isBlack;
		this->key = key;
		this->value = value;
		this->parent = parent;
		this->left = left;
		this->right = right;
	}
	/*
	RBNode(const RBNode& other)
	{
		this->isBlack = other.isBlack;
		this->key = other.key;
		this->value = other.value;
		this->parent = other.parent;
		this->left = other.left;
		this->right = other.right;
	}
	*/
#pragma region getters/setters lol
	void SetParent(RBNode<TKey, TValue>* parent)
	{
		this->parent = parent;
	}
	void SetLeft(RBNode<TKey, TValue>* left)
	{
		this->left = left;
	}
	void SetRight(RBNode<TKey, TValue>* right)
	{
		this->right = right;
	}
	void SetKey(const TKey newKey)
	{
		this->key = newKey;
	}
	void SetBlack()
	{
		isBlack = true;
	}
	void SetRed()
	{
		isBlack = false;
	}
	void SetIsBlack(bool newValue)
	{
		isBlack = newValue;
	}
	void SetValue(const TValue value)
	{
		this->value = value;
	}


	TValue& GetValue()
	{
		return value;
	}
	TKey& GetKey()
	{
		return key;
	}
	bool IsBlack()
	{
		return isBlack;
	}



	RBNode<TKey, TValue>* GetLeft()
	{
		return left;
	}
	RBNode<TKey, TValue>* GetRight()
	{
		return right;
	}
	RBNode<TKey, TValue>* GetParent()
	{
		return parent;
	}
	RBNode<TKey, TValue>* GetGrandparent()
	{
		return parent->parent;
	}
#pragma endregion

};