#pragma once
#include <exception>
#include <vector>
#include <string>
#include <tuple>
#include <functional>
#include <algorithm>

template <class Tk, class Td>
class Tree
{
public:
	int length; //длина дерева
protected:
	class Node
	{
	public:
		int bal;
		Tk key;		//ключ объекта
		Td data;	//значение объекта в элементе
		Node *left;	// указатель на левого сына
		Node *right;//указатель на правого сына
		
			Node(Tk k, Td d){//конструктор с параметрами
				key = k;
				data = d;
				left = right = NULL;
		}
	};

public:
	Tree();//конструктор без параметров
	Tree(const Tree<Tk, Td>& anotherTree);//конструктор копирования
	~Tree(void);//деструктор
	int sizeOfTree();//опрос размера дерева
	void deleteTree();//очистка дерева
	bool EmptyTree();//проверка дерева на пустоту
	Td& readObject(Tk key, int& looked);//доступ к данным с заданным ключом
	bool addNewIndex(Tk key, Td obj, int& looked);//включение данных с заданным ключом
	bool deleteIndex(Tk key, int& looked);//удаление данных с заданным ключом
	void show_tree();//вывод структуры дерева на экран
    void go_LtR();//обход узлов дерева по схеме
	int lengthofThree();//определение длины внешнего пути дерева  (рекурсивно)
	Node *root;	//указатель на корень
private:
	void CopyTree(Node *r);//вспомогательная функция для копирования дерева
	void ClearTree(Node *r);//вспомогательная функция для очистки дерева
	void Show(Node *r, int level);//вспомогательная функция для вывода структуры
	void countlev(Node *r, int level, int &sum);//вспомогательная функция для определения внешнего пути
	Node* BSTPredecessor(Node *x);//рекурсивный алгоритм поиска предыдущего по ключу узла
	Node* BSTSuccessor(Node *x);//рекурсивный алгоритм поиска следующего по ключу узла
	Node* Max(Node *t);//поиск максимального по ключу узла в поддереве 
	Node* RParent(Node *t,Node *x);//поиск ближайшего правого родителя для заданного узла дерева
	Node* Min(Node *t);//поиск минимального по ключу узла в поддереве 
	Node* LParent(Node *t,Node *x);//поиск ближайшего левого родителя для заданного узла дерева

public:
	class Iterator
	{
	private:
		Tree *ptr;//указатель на объект коллекции
		Node *cur;//указатель на текущий элемент коллекции
	public:
		Iterator(Tree<Tk,Td> &tree){//конструктор
			ptr = &tree;
			cur = NULL;
		}
		
		void beg(){//установка на первый
			cur = ptr->root;
			if (cur != NULL)
				while(cur->left != NULL) 
					cur = cur->left;
		}
		
		void end(){//установка на последний
			cur = NULL;
		}
		
		void next(){//установка на следующий
			cur = ptr->BSTSuccessor(cur);
		}
		
		void prev(){//установка на предыдущий
			cur = ptr->BSTPredecessor(cur);
		}
		
		bool is_off() const{//проверка состояния итератора
			return (cur == NULL);
		}
		
		Td& operator*(){//доспуп к данным текущего элемента
			if (cur != NULL)
				return cur -> data;
			else
				throw exception("Итератор за пределами дерева");
		}
	};
	class rIterator
	{
	private:
		Tree* ptr;//указатель на объект коллекции
		Node* cur;//указатель на текущий элемент коллекции
	public:
		rIterator(Tree<Tk, Td>& tree) {//конструктор
			ptr = &tree;
			cur = NULL;
		}

		void rbeg() {//установка на первый
			cur = ptr->root;
			if (cur != NULL)
				while (cur->right != NULL)
					cur = cur->right;
		}

		void rend() {//установка на последний
			cur = NULL;
		}

		void rnext() {//установка на следующий
			
			cur = ptr->BSTPredecessor(cur);
		}

		void rprev() {//установка на предыдущий
			cur = ptr->BSTSuccessor(cur);
		}

		bool is_off() const {//проверка состояния итератора
			return (cur == NULL);
		}

		Td& operator*() {//доспуп к данным текущего элемента
			if (cur != NULL)
				return cur->data;
			else
				throw exception("Итератор за пределами дерева");
		}
	};


	friend class Iterator;
	friend class rIterator;
};

template<class Tk,class Td>
	Tree<Tk,Td>::Tree(void)//конструктор без параметров
{
	length = 0;
	root = NULL; //в начале дерево пусто
}

template<class Tk,class Td>
	Tree<Tk,Td>::Tree(const Tree<Tk,Td>& anotherTree)//конструктор копирования
{
	root = NULL;
	length = 0;
	CopyTree(anotherTree.root);
}

template<class Tk,class Td>
	void Tree<Tk,Td>::CopyTree(Node *r)//копирование дерева
{
	if (r == NULL)
		return;
	int tmp;
	addNewIndex(r -> key, r -> data, tmp);
	CopyTree(r->left);
	CopyTree(r->right);
}

template<class Tk,class Td>
	Tree<Tk, Td>::~Tree(void)//деструктор
{
	deleteTree();
}

template<class Tk, class Td>
	int Tree<Tk, Td>::sizeOfTree()//опрос размера дерева
{
	return length;
}

template<class Tk, class Td>
	void Tree<Tk, Td>::deleteTree()//очистка дерева
{
	ClearTree(root);
	///looked = length;
	root = NULL;
	length = 0;
}

template<class Tk, class Td>
	void Tree<Tk, Td>::ClearTree(Node *r)//очистка по обходу LtR дерева
{
	if (r == NULL)
		return;
	ClearTree(r->left); 
	r->left = NULL;
	Node *rtree = r->right;
	r->right = NULL;
	delete r;
	ClearTree(rtree);
}

template<class Tk, class Td>
	bool Tree<Tk, Td>::EmptyTree()//проверка дерева на пустоту
{
	return (length == 0 && root == NULL);
}

template<class Tk, class Td>
	Td& Tree<Tk, Td>::readObject(Tk key, int& looked)//доступ к данным с заданным ключом
{
	looked = 1;
	Node* t = root;
	while(t != NULL && key != (t->key))//проход по дереву до задонного ключа
	{
		if(key < (t->key))//если ключ меньше
			t = t->left;//идем влево
		else
			t = t->right;//идем влево
		looked++;
	}
	if(t == NULL)//если ключа не существует
		throw exception("Данного ключа не существует");
	return t->data;
}

template<class Tk, class Td>
	bool Tree<Tk, Td>::addNewIndex(Tk key, Td obj, int& looked)//включение данных с заданным ключом
{
	looked = 0;
	if(root == NULL){//добавление в начало
		root = new Node(key, obj);
		length++;
		return true;
	}

	Node *t = root, *pred = NULL;
	while(t != NULL)
	{
		pred = t;
		if(key == (t->key))//если ключи совпадают
			return false;
		looked++;
		if(key < (t->key))//если ключ меньше 
			t = t->left;//добавляем влево
		else 
			t = t->right;//иначе добавляем вправо
	}

	if(key < (pred->key)) 
		pred->left = new Node(obj, key);
	else
		pred->right = new Node(obj, key);
	length++;
	return true;
}

template<class Tk, class Td>
	bool Tree<Tk, Td>::deleteIndex(Tk key, int& looked)//удаление данных с заданным ключом
{
	Node *t = root, *pred = NULL, *next = NULL, *t0 = NULL;
	looked = 1;

	while(t != NULL && t->key != key)
	{
		pred = t; //Запоминаем родителя удаляемого
		if(key < t->key) //если ключ меньше 
			t = t->left;//идем влево
		else 
			t = t->right;//идем вправо
		looked++;
	}

	if(t == NULL) //Удаляемый не найден
		return false;

	if(t->left != NULL && t->right != NULL){ //Оба потомка у удаляемого есть
		t0 = t;	//Запоминаем удаляемого
		pred = t;
		t = t->right;
		while(t->left != NULL){
			pred = t;
			t = t->left;
			looked++;
		}
		t0->key = t->key;
		t0->data = t->data;
		next = t->right;
	}
	else //Менее двух потомков у удаляемого
		if(t->left == NULL && t->right == NULL) //Нет потомков у удаляемого
			next = NULL;
		else 
			if(t->left == NULL) //Нет левого потомка у удаляемого
				next = t->right;
			else 
				if(t->right == NULL) //Нет правого потомка у удаляемого
					next = t->left;

	if(pred == NULL) //Нет родителя у удаляемого
		root = next;
	else
		if(t->key < pred->key)	//Удаляемый левым потомком pred
			pred->left = next;
		else					//Удаляемый правым потомком pred
			pred->right = next;

	delete t;
	length--;
	return true;
}

template <class Tk,class Td>
	void Tree<Tk,Td>::go_LtR()//обход структуры по LtR
{   
	if (root == NULL)
		throw exception("Нет данных");

	Node *it = root;
	vector<Node*> up(length);
	int top = 0;
 
	while (it != NULL) {
			while (it != NULL) {
					if (it->right != NULL)
							up [top++] = it->right;

					up [top++] = it;
					it = it->left;
			}

			it = up [--top];

			while (top != 0 && it->right == NULL) {                      
			cout << it->key << "->";
					it = up [--top];
			}

			cout << it->key << "->";

			if (top == 0)
					break;

			it = up [--top];
	}
	cout << endl;
}


static std::string ch_hor = "-", ch_ver = "|", ch_ddia = "/", ch_rddia = "\\", ch_udia = "\\", ch_ver_hor = "|-", ch_udia_hor = "\\-", ch_ddia_hor = "/-", ch_ver_spa = "| ";

template <class Tk,class Td>
	void Tree<Tk,Td>::Show(typename Tree<Tk,Td>::Node *node, int level)//вспомогательная функция для вывода структуры
{
#define _MAX(x, y) ((x) > (y) ? (x) : (y))
#define _MIN(x, y) ((x) < (y) ? (x) : (y))

		auto RepStr = [](std::string const& s, size_t cnt) {
			if (ptrdiff_t(cnt) < 0)
				throw std::runtime_error("RepStr: Bad value " + std::to_string(ptrdiff_t(cnt)) + "!");
			std::string r;
			for (size_t i = 0; i < cnt; ++i)
				r += s;
			return r;
		};
		std::function<std::tuple<std::vector<std::string>, size_t, size_t>(typename Tree<Tk, Td>::Node* node, bool)> Rec;
		Rec = [&RepStr, &Rec](typename Tree<Tk, Td>::Node* node, bool left) {
			std::vector<std::string> lines;
			if (!node)
				return std::make_tuple(lines, size_t(0), size_t(0));
			auto sval = std::to_string(node->data) + "," + std::to_string(node->bal);
			//if (sval.size() % 2 != 1) sval += " ";
			auto resl = Rec(node->left, true), resr = Rec(node->right, false);
			auto const& vl = std::get<0>(resl);
			auto const& vr = std::get<0>(resr);
			auto cl = std::get<1>(resl), cr = std::get<1>(resr), lss = std::get<2>(resl), rss = std::get<2>(resr);
			size_t lv = sval.size();
			size_t ls = vl.size() > 0 ? lss : size_t(0);
			size_t rs = vr.size() > 0 ? rss : size_t(0);
			size_t lis = ls == 0 ? lv / 2 : _MAX(int(lv / 2 + 1 - (ls - cl)), 0);
			size_t ris = rs == 0 ? (lv + 1) / 2 : _MAX(int((lv + 1) / 2 - cr), (lis > 0 ? 0 : 1));
			size_t dashls = (ls == 0 ? 0 : ls - cl - 1 + lis - lv / 2), dashrs = (rs == 0 ? 0 : cr + ris - (lv + 1) / 2);
			//DEB(node->value); DEB(lv); DEB(ls); DEB(rs); DEB(cl); DEB(cr); DEB(lis); DEB(ris); DEB(dashls); DEB(dashrs); std::cout << std::endl;
			lines.push_back(
				(ls == 0 ? "" : (RepStr(" ", cl) + ch_ddia + RepStr(ch_hor, dashls))) +
				sval + (rs == 0 ? "" : (RepStr(ch_hor, dashrs) + ch_rddia + RepStr(" ", rs - cr - 1)))
			);
			//if (lines.back().size() != ls + lis + ris + rs)
			//    throw std::runtime_error("Dump: First line wrong size, got " + std::to_string(lines.back().size()) + ", expected " + std::to_string(ls + lis + ris + rs));
			for (size_t i = 0; i < _MAX(vl.size(), vr.size()); ++i) {
				std::string sl = RepStr(" ", ls), sr = RepStr(" ", rs);
				if (i < vl.size()) sl = vl[i];
				if (i < vr.size()) sr = vr[i];
				sl = sl + RepStr(" ", lis);
				sr = RepStr(" ", ris) + sr;
				lines.push_back(sl + sr);
			}
			return std::make_tuple(lines, (left || ls + lis == 0 || lv % 2 == 1) ? ls + lis : ls + lis - 1, ls + lis + ris + rs);
		};
		auto v = std::get<0>(Rec(node, true));
		for (size_t i = 0; i < v.size(); ++i)
			std::cout << v[i] << std::endl;

#undef _MAX
#undef _MIN
}

template<class Tk, class Td>
	void Tree<Tk, Td>::show_tree()//вывод структуры дерева на экран
{
	if(root == NULL){
		return;
	}
	Show(root,0);
}

template<class Tk, class Td>
	int Tree<Tk, Td>::lengthofThree()//определение длины внешнего пути дерева 
{
	if(root == NULL) 
		return -1;
	int sum = 0;
	countlev(root,1,sum);
	return sum;
}

template <class Tk, class Td>
	void Tree<Tk,Td>::countlev(Node *r, int level, int &sum)//вспомогательная функция для определения внешнего пути
{
	if(r==NULL) 
		return;
	countlev(r->left, level+1, sum);
	countlev(r->right, level+1, sum);
	if(r->right == NULL || r->left == NULL)
		sum += level;
}

template <class Tk,class Td> typename Tree<Tk,Td>::Node* Tree<Tk,Td>::BSTSuccessor(typename Tree<Tk,Td>::Node *x)
{
	if(x == NULL) 
		return NULL;
	if(x->right != NULL)
	{
		Node* t = x->right;
		while(t->left != NULL) 
			t = t->left;
		return t;
	}
	else 
		return LParent(root,x);
}

template <class Tk,class Td> typename Tree<Tk,Td>::Node* Tree<Tk,Td>::LParent(typename Tree<Tk,Td>::Node *t,typename Tree<Tk,Td>::Node *x){
	if(t == x)
		return NULL;
	if(x->key < t->key){
		Node *rp = LParent(t->left, x);
		if(rp != NULL)
			return rp;
		else 
			return t;
	}
	else 
		return LParent(t->right, x);
}

template <class Tk,class Td> typename Tree<Tk,Td>::Node* Tree<Tk,Td>::BSTPredecessor(typename Tree<Tk,Td>::Node *x){
	if(x == NULL) 
		return NULL;
	if(x->left != NULL)
	{
		Node* t = x->left;
		while(t->right != NULL) 
			t = t->right;
		return t;
	}
	else 
		return RParent(root,x);
}

template <class Tk,class Td> typename Tree<Tk,Td>::
	Node* Tree<Tk,Td>::RParent(typename Tree<Tk,Td>::Node *t,typename Tree<Tk,Td>::Node *x){
	if(t == x) 
		return NULL;
	if(x->key > t->key){
		Node *rp = RParent(t->right, x);
		if(rp != NULL) 
			return rp;
		else 
			return t;
	}
	else 
		return RParent(t->left, x);
}
