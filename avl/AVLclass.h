#include "iostream"
#include "class_new.h"
//#include "class.h"

template <class Tk, class Td>
	class AVL_Tree: public Tree<Tk, Td>
{
public:
	bool add(Tk k, Td val, int &looked);//Булевская фуекция вставки элемента 
	bool delet(Tk key, int &looked);//Булевская фуекция удаления элемента 
	~AVL_Tree(){}	
protected:
	int nodes;//счетчик	
	Node* AVL_Insert(Node *t, Tk k, Td val, bool &h, bool &ins);//Вставка элемента в AVL-дерево
	Node* AVL_Delete(Node* t, Tk k, bool &h, bool &deleted);//Удаление элемента в AVL-дереве
	Node* Del(Node *t, Node *t0, bool &h);
	Node* RL(Node* t);// RL - поворот
	Node* L(Node* t);// L - поворот
	Node* LR(Node* t);// LR - поворот
	Node* R(Node* t);// R - поворот
};

template<class Tk,class Td>
	bool AVL_Tree<Tk,Td>::add(Tk k, Td val, int &looked)
{
	bool h, ins;
	nodes = 0;
	root = AVL_Insert(root, k, val, h, ins);
	looked = nodes;
	return ins;
}

template<class Tk,class Td>
	bool AVL_Tree<Tk,Td>::delet(Tk key, int &looked)
{
	bool h, deleted;
	nodes = 0;
	root = AVL_Delete(root, key, h, deleted);
	looked = nodes;
	return deleted;
}

template<class Tk,class Td>
	typename Tree<Tk,Td>::Node* AVL_Tree<Tk,Td>::AVL_Insert(typename Tree<Tk,Td>::Node *t, Tk k, Td val, bool &h, bool &ins)
{
	nodes++;
	bool hh, insert;
	if(t == NULL)
	{
		t = new Node(k, val);
		t->bal = 0;
		h = true;
		ins = true;
		length++;
		return t;
	}
	h = false;
	if(k == t->key)
	{
		ins = false;
		return t;
	}
	if(k < t->key)
	{
		t->left = AVL_Insert(t->left, k, val, hh, insert);
		if(hh)
		{
			//выросла левая ветвь
			if (t->bal == 1)
			{
				t->bal = 0;
			}
			else
				if(t->bal == 0)
				{
					t->bal = -1;
					h = true;
				}
				else
					if(t->left->bal == -1)
						t = R(t);
					else
						t = LR(t);
		}
	}
	else
	{
		t->right = AVL_Insert(t->right, k, val, hh, insert);
		if(hh)
			//выросла правая ветвь
			if (t->bal == -1)
				t->bal = 0;
			else
				if(t->bal == 0)
				{
					t->bal = 1;
					h = true;
				}
				else
					if(t->right->bal == 1)
						t = L(t);
					else
						t = RL(t);
	}
	ins = insert;
	return t;
}

template<class Tk,class Td>
	typename Tree<Tk,Td>::Node* AVL_Tree<Tk,Td>::AVL_Delete(typename Tree<Tk,Td>::Node* t, Tk k, bool &h, bool &deleted)
{
	nodes++;
	bool hh, del;
	Node *x;
	h = false;
	if(t == NULL)
	{
		deleted = false;
		return NULL;
	}
	if(k < t->key)
	{
		t->left = AVL_Delete(t->left, k, hh, del);
		if(hh)
		{
			if(t->bal == -1)
			{
				t->bal = 0;
				h = true;
			}
			else
				if(t->bal == 0)
					t->bal = 1;
			else
				if(t->right->bal == 0)
					t = L(t);
			else
			{
				nodes++;
				h = true;
				if (t->right->bal == 1)
					t = L(t);
				else
					t = RL(t);
			}
		}
		deleted = del;
		return t;
	}
	else if (k > t->key)
	{
		t->right = AVL_Delete(t->right, k, hh, del);
		if (hh) {
			if (t->bal == 1)
			{
				t->bal = 0;
				h = true;
			}
			else if (t->bal == 0) t->bal = -1;
			else if (t->left->bal == 0) t = R(t);
			else
			{
				h = true;
				if (t->left->bal == -1)
					t = R(t);
				else
					t = LR(t);
			}
		}
		deleted = del;
		return t;
	}
	deleted = true;
	nodes++;
	if((t->left == NULL) && (t->right == NULL))
	{
		delete t;
		length--;
		h = true;
		return NULL;
	}
	if(t->left == NULL)
	{
		x = t->right;
		delete t;
		length--;
		h = true;
		return x;
	}
	if(t->right == NULL)
	{
		x = t->left;
		delete t;
		length--;
		h = true;
		return x;
	}
	t->right = Del(t->right, t, hh);
	if(hh)
		if(t->bal == 1)
		{
			t->bal = 0;
			h = true;
		}
		else
			if(t->bal == 0)
				t->bal = -1;
			else
			{
				x = t->left;
				if (x->bal == 0)
					t = R(t);
				else
				{
					h = true;
					if (x->bal == -1)
						t = R(t);
					else
						t = LR(t);
				}
			}
	return t;
}
	
template<class Tk,class Td>
	typename Tree<Tk,Td>::Node* AVL_Tree<Tk,Td>::Del(typename Tree<Tk,Td>::Node *t, Node *t0, bool &h)
{
	nodes++;
	Node *x;
	bool hh;
	h = false;
	if(t->left != NULL)
	{
		t->left = Del(t->left, t0, hh);
		if(hh)
			if(t->bal == -1)
			{
				t->bal = 0;
				h = true;
			}
			else
				if(t->bal == 0)
					t->bal = 1;
				else
				{
					nodes++;
					if(t->right->bal == 0)
						t = L(t);
					else
					{
						h = true;
						if (t->right->bal == 1)
							t = L(t);
						else
							t = RL(t);
					}
				}
		return t;
	}
	else
	{
		t0->key = t->key;
		t0->data = t->data;
		x = t->right;
		length--;
		nodes++;
		delete t;
		h = true;
		return x;
	}
}

template<class Tk,class Td>
	typename Tree<Tk,Td>::Node* AVL_Tree<Tk,Td>::R(typename Tree<Tk,Td>::Node* t)
{
	nodes += 2;
	Node *x = t->left;
	t->left = x->right;
	x->right = t;

	if(x->bal == -1)
	{
		t->bal = 0;
		x->bal = 0;
	}
	else
	{
		t->bal = -1;
		x->bal = 1;
	}
	return x;
}

template<class Tk,class Td>
	typename Tree<Tk,Td>::Node* AVL_Tree<Tk,Td>::LR(typename Tree<Tk,Td>::Node* t)
{
	nodes += 4;
	Node* x = t->left;
	Node* y = x->right;

	x->right = y->left;
	y->left = x;
	t->left = y->right;
	y->right = t;

	if(y->bal == -1)
	{
		t->bal = 1;
		x->bal = 0;
	}

	if(y->bal == 0)
	{
		t->bal = 0;
		x->bal = 0;
	}

	if(y->bal == 1)
	{
		t->bal = 0;
		x->bal = -1;
	}
	y->bal = 0;
	return y;
}

template<class Tk,class Td>
	typename Tree<Tk,Td>::Node* AVL_Tree<Tk,Td>::L(typename Tree<Tk,Td>::Node* t)
{
	nodes += 2;
	Node *x = t->right;
	t->right = x->left;
	x->left = t;
	if(x->bal == 1)
	{
		t->bal = 0;
		x->bal = 0;
	}
	else
	{
		t->bal = 1;
		x->bal = -1;
	}
	return x;
}

template<class Tk,class Td>
	typename Tree<Tk,Td>::Node* AVL_Tree<Tk,Td>::RL(typename Tree<Tk,Td>::Node* t)
{
	nodes += 4;
	Node* x = t->right;
	Node* y = x->left;

	x->left = y->right;
	y->right = x;
	t->right = y->left;
	y->left = t;

	if(y->bal == -1)
	{
		t->bal = 0;
		x->bal = 1;
	}

	if(y->bal == 0)
	{
		t->bal = 0;
		x->bal = 0;
	}

	if(y->bal == 1)
	{
		t->bal = -1;
		x->bal = 0;
	}
	y->bal = 0;
	return y;
}
