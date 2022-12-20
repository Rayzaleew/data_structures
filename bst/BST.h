#include <stack>
#include <string>
#include <list>

using std::list;

#define IterOutOfRangeEx "Iterator out of range"
#define WrongKeyEx "Wrong key"

template <class K, class V>  //Key, Value
class BST
{
protected:
	class Node;

public:
	class Iterator
	{
	public:
		Iterator() {};
		Iterator(BST& bst, Node* node) { this->bst = &bst; current = node; }
		V& operator *() const
		{
			if (current != nullptr)
				return current->value;

			throw IterOutOfRangeEx;
		}
		bool operator++(int value) //переход к следующему узлу при прямом обходе
		{
			return (current = bst->GetNext(current)) != nullptr;
		}
		bool operator--(int value)  //переход к предыдущему узлу при прямом обходе
		{
			return (current = bst->GetPrev(current)) != nullptr;
		}
		bool operator == (Iterator iter) const { return bst == iter.bst && current == iter.current; }
		bool operator != (Iterator iter) const { return !(*this == iter); }
	protected:
		BST* bst = nullptr;
		Node* current = nullptr;

		friend class BST;
	};

	class ReverseIterator
	{
	public:
		ReverseIterator() {};
		ReverseIterator(BST& bst, Node* node) { this->bst = &bst; current = node; }
		V& operator *() const
		{
			if (current != nullptr)
				return current->value;

			throw IterOutOfRangeEx;
		}
		bool operator++(int value) //переход к предыдущему узлу при прямом обходе
		{
			return (current = bst->GetPrev(current)) != nullptr;
		}
		bool operator--(int value) // переход к следующему узлу при прямом обходе
		{
			return (current = bst->GetNext(current)) != nullptr;
		}
		bool operator == (ReverseIterator iter) const { return bst == iter.bst && current == iter.current; }
		bool operator != (ReverseIterator iter) const { return !(*this == iter); }
	protected:
		BST* bst = nullptr;
		Node* current = nullptr;

		friend class BST;
	};

	BST() {};
	BST(const BST<K, V>& bst) { if (bst.size > 0) BypassTree(bst.root, BypassMode::AddToTree); };
	~BST() { Clear(); }
	int GetSize() const { return size; }
	void Clear();
	bool IsEmpty() const { return size == 0; }
	V Get(K key) const;
	void Set(K key, V newValue);
	bool Add(K key, V value);
	bool Remove(K key);
	std::list<K> GetKeysList() const; //возвращает список ключей по схеме t -> L -> R
	int GetReadElementsCount() const { return readElements; } //опрос числа узлов дерева, просмотренных предыдущей операцией
	void Print() const;
	int GetHeight();  //высота дерева

	Iterator Begin(); //запрос прямого итератора, установленного на узел дерева с минимальным ключом
	Iterator End() { return Iterator(*this, nullptr); } //запрос «неустановленного» прямого итератора
	ReverseIterator Rbegin(); //запрос обратного итератора, установленного на узел дерева с максимальным ключом
	ReverseIterator Rend() { return ReverseIterator(*this, nullptr); } //запрос «неустановленного» обратного итератора
protected:
	int size = 0;
	mutable int readElements = 0; //число узлов дерева, просмотренных предыдущей операцией
	Node* root = nullptr;

	void GetKeysTLR(Node* root, list<K>& list) const;

	enum class BypassMode { AddToTree, DeleteFromMemory }; //режим обхода дерева
	void BypassTree(Node* root, BypassMode mode); //обход дерева, где для каждого узла выполняется действие согласно режиму обхода

	bool Insert(Node* curRoot, K key, V value); //рекурсивная вставка
	void GetMaxDepth(Node* curRoot, int curDepth, int* maxDepth); //рекурсивное получение макс. глубины узла
	void PrintLevels(Node* root, int level) const; //рекурсивный вывод дерева
	bool FindNodeByKey(Node* curRoot, K key, Node** resultNode) const;
	bool Remove(Node* curNode, Node* curParent, K key); //рекурсивное удаление
	Node* GetPrev(Node* node) const; //следующий узел при прямом обходе
	Node* GetNext(Node* node) const; //предыдущий узел при прямом обходе

	class Node
	{
	public:
		K key;
		V value;
		Node* left = nullptr;
		Node* right = nullptr;

		Node() {};
		Node(K key, V value) : key(key), value(value) {};

		Node* GetMaxInR() const //нахождение узла с макс. ключом в правом поддереве
		{
			if (this->right == nullptr)
				return nullptr;

			Node* max = this->right;
			while (max->right != nullptr)
				max = max->right;

			return max;
		}
		Node* GetMinInL() const //нахождение узла с мин. ключом в левом поддереве
		{
			if (this->left == nullptr)
				return nullptr;

			Node* min = this->left;
			while (min->left != nullptr)
				min = min->left;

			return min;
		}
	};

	friend class Iterator;
	friend class ReverseIterator;
};

//открытые методы дерева

template<class K, class V>
inline void BST<K, V>::Clear()
{
	if (IsEmpty())
		return;

	size = 0;
	BypassTree(this->root, BypassMode::DeleteFromMemory);
}

template<class K, class V>
inline V BST<K, V>::Get(K key) const
{
	readElements = 0;
	Node* node = nullptr;
	if (FindNodeByKey(this->root, key , &node) == false)
		throw WrongKeyEx;
	else
		return node->value;
}

template<class K, class V>
inline void BST<K, V>::Set(K key, V newValue)
{
	readElements = 0;
	Node* node = nullptr;
	if (FindNodeByKey(this->root, key, &node) == false)
		throw WrongKeyEx;

	node->value = newValue;
}

template<class K, class V>
inline bool BST<K, V>::Add(K key, V value)
{
	readElements = 0;
	if (IsEmpty())
	{
		this->root = new Node(key, value);
		size++;
		return true;
	}
	return Insert(this->root, key, value);
}

template<class K, class V>
inline bool BST<K, V>::Remove(K key)
{
	readElements = 0;
	return Remove(this->root, nullptr, key);
}

template<class K, class V>
inline list<K> BST<K, V>::GetKeysList() const
{
	list<K> keys;
	GetKeysTLR(this->root, keys);

	return keys;
}

template<class K, class V>
inline void BST<K, V>::Print() const
{
	if (IsEmpty())
	{
		std::cout << "Tree is empty" << std::endl;
		return;
	}

	PrintLevels(root, 0);
}

template<class K, class V>
inline int BST<K, V>::GetHeight()
{
	int maxDepth = 0;
	readElements = 0;

	GetMaxDepth(this->root, 0, &maxDepth);
	return maxDepth;
}

template<class K, class V>
inline  typename BST<K, V>::Iterator BST<K, V>::Begin()
{
	if (IsEmpty())
		return End();

	Node* min = root->GetMinInL();
	min = (min == nullptr ? root : min);

	return Iterator(*this, min);
}

template<class K, class V>
inline  typename BST<K, V>::ReverseIterator BST<K, V>::Rbegin()
{
	if (IsEmpty())
		return Rend();

	Node* max = root->GetMaxInR();
	max = (max == nullptr ? root : max);

	return ReverseIterator(*this, max);
}

//приватные методы дерева

template<class K, class V>
inline bool BST<K, V>::FindNodeByKey(Node* curRoot, K key, Node** resultNode) const
{
	if (curRoot == nullptr)
		return false;

	readElements++;
	if (curRoot->key < key) //спуск в правое поддерево
		return FindNodeByKey(curRoot->right, key, resultNode);
	else if (curRoot->key > key) //спуск в левое поддерево
		return FindNodeByKey(curRoot->left, key, resultNode);
	else //узел найден
	{
		*resultNode = curRoot;
		return true;
	}
}

template<class K, class V>
inline bool BST<K, V>::Remove(Node* curNode, Node* curParent, K key)
{
	if (curNode == nullptr)
		return false;

	readElements++;
	if (key < curNode->key)  //спуск в левое поддерево
		return Remove(curNode->left, curNode, key);
	if (key > curNode->key) //спуск в правое поддерево
		return Remove(curNode->right, curNode, key);

	Node* replaceNode; //узел, который встанет на место удалённого

	if (curNode->left == nullptr && curNode->right == nullptr)  //нет потомков
	{
		replaceNode = nullptr;
	}
	else if (curNode->left == nullptr) //есть только правый потомок
	{
		replaceNode = curNode->right;
	}
	else if (curNode->right == nullptr) //есть только левый потомок
	{
		replaceNode = curNode->left;
	}
	else // есть оба потомка
	{
		//поиск в правом поддереве узла с мин. ключом (min) и его родителя (curParent)
		curParent = curNode;
		Node* min = curNode->right;

		readElements++;
		while (min->left != nullptr)
		{
			curParent = min;
			min = min->left;
			readElements++;
		}

		//значения удаляемого узла теперь равны min
		curNode->key = min->key;
		curNode->value = min->value;

		//теперь удаляемый узел - min
		//curParent и replaceNode указывают на его родителя и замену
		curNode = min;
		replaceNode = min->right;
	}

	if (curParent == nullptr) 
		this->root = replaceNode;
	else 
	{
		if (curParent->left == curNode) curParent->left = replaceNode;
		if (curParent->right == curNode) curParent->right = replaceNode;
	}

	delete curNode;
	size--;
	return true;
}

template<class K, class V>
inline typename BST<K, V>::Node* BST<K, V>::GetPrev(Node* node) const
{
	if (node == nullptr)
		return nullptr;

	//если есть левый потомок, предыдущий - либо он, либо элемент с макс. ключом в правом поддереве
	if (node->left != nullptr)
	{
		Node* max = node->left->GetMaxInR();
		return (max == nullptr ? node->left : max);
	}

	//иначе - спуск от корня с поиском элемента с макс. ключом, меньшим, чем у node
	Node* current = this->root;
	Node* lastSuccessNode = nullptr;

	while (current != nullptr)
	{
		if (current->key == node->key)
			break;

		if (current->key > node->key) //спуск в левое поддерево
		{
			current = current->left;
		}
		else //спуск в правое поддерево
		{
			lastSuccessNode = current;
			current = current->right;
		}
	}

	return lastSuccessNode;
}

template<class K, class V>
inline typename BST<K, V>::Node* BST<K, V>::GetNext(Node* node) const
{
	if (node == nullptr)
		return nullptr;

	//если есть правый потомок, следующий - либо он, либо элемент с мин. ключом в его левом поддереве
	if (node->right != nullptr)
	{
		Node* min = node->right->GetMinInL();
		return (min == nullptr ? node->right : min);
	}
	//иначе - спуск от корня с поиском элемента с мин. ключом, большим, чем у node

	Node* current = this->root;
	Node* lastSuccessNode = nullptr;

	while (current != nullptr)
	{
		if (current->key == node->key)
			break;

		if (current->key > node->key) //спуск в левое поддерево
		{
			lastSuccessNode = current;
			current = current->left;
		}
		else //спуск в правое поддерево
		{
			current = current->right;
		}
	}

	return lastSuccessNode;
}

template<class K, class V>
inline void BST<K, V>::GetKeysTLR(Node* root, list<K>& list) const
{
	if (root == nullptr)
		return;

	list.push_back(root->key); //добавление ключа в список (t)
	GetKeysTLR(root->left, list); //спуск в левое поддерево (L)
	GetKeysTLR(root->right, list); //спуск в правое поддерево (R)
}

template<class K, class V>
inline void BST<K, V>::BypassTree(Node* root, BypassMode mode)
{
	Node* node = root;
	std::stack<Node*> _stack;
	_stack.push(node);

	while (_stack.empty() == false)
	{
		node = _stack.top();
		_stack.pop(); //берём верхний элемент в стеке

		//пытаемся добавить в стек его потомков
		if (node->left != nullptr)
			_stack.push(node->left);
		if (node->right != nullptr)
			_stack.push(node->right);

		//выполняем над ним действие согласно режиму обхода
		if (mode == BypassMode::AddToTree)
			Add(node->key, node->value);
		else if (mode == BypassMode::DeleteFromMemory)
			delete node;
	}
}

template<class K, class V>
inline bool BST<K, V>::Insert(Node* curRoot, K key, V value)
{
	readElements++;

	if (curRoot->key < key)
	{
		if (curRoot->right == nullptr)
		{
			curRoot->right = new Node(key, value);
			size++;
			return true;
		}
		return Insert(curRoot->right, key, value);
	}
	else if (curRoot->key > key)
	{
		if (curRoot->left == nullptr)
		{
			curRoot->left = new Node(key, value);
			size++;
			return true;
		}
		return Insert(curRoot->left, key, value);
	}
	else
		return false;
}

template<class K, class V>
inline void BST<K, V>::GetMaxDepth(Node* curRoot, int curDepth, int* maxDepth)
{
	if (curRoot == nullptr)
		return;

	readElements++;

	if (curRoot->left == nullptr && curRoot->right == nullptr) //если у узла нет потомков, его глубина может быть максимальной во всём дереве
	{
		if (curDepth > *maxDepth)
			*maxDepth = curDepth;
		return;
	}

	GetMaxDepth(curRoot->left, curDepth + 1, maxDepth); //спуск в левое поддерево
	GetMaxDepth(curRoot->right, curDepth + 1, maxDepth); //спуск в правое поддерево
}

template<class K, class V>
inline void BST<K, V>::PrintLevels(Node* root, int level) const
{
	if (root == nullptr)
		return;

	int levelOffset = 3;
	PrintLevels(root->right, level + 1);

	std::cout << std::string(levelOffset * level, ' ') << root->key << std::endl;

	PrintLevels(root->left, level + 1);
}