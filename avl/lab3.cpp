#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <conio.h>
#include <ctime>
#include "class_new.h"
//#include "class.h"
#include "AVLclass.h"

using namespace std;

typedef long long MyTypeD;
typedef long long MyTypeK;

MyTypeD lrand()
{
	return rand()<<16 | rand();
}

void test_add(int n, Tree <MyTypeK, MyTypeD> &tree, AVL_Tree<MyTypeK, MyTypeD> &treeAVL) {
	//Tree <MyTypeK, MyTypeD> tree;
	//AVL_Tree<MyTypeK, MyTypeD> treeAVL;
	srand((unsigned)time(NULL));	//Инициализация генератора ПСЧ
	MyTypeK* m = new MyTypeK[n];
	//MyTypeD data = 1;
	int nn = 0;
	int n2 = 0;
	for (int i = 0; i < n; i++)
	{
		m[i] = i + 1;
		tree.addNewIndex(m[i], m[i], nn);
		treeAVL.add(m[i], m[i], n2);
	}
}

void test_rand(int n){
	Tree <MyTypeK, MyTypeD> tree;
	AVL_Tree<MyTypeK, MyTypeD> treeAVL;
	srand((unsigned)time(NULL));	//Инициализация генератора ПСЧ
	MyTypeK* m = new MyTypeK[n];
	MyTypeD data = 1;
	int nn = 0;
	int n2 = 0;
	for(int i = 0; i < n; i++)
	{
		m[i] = lrand();
		tree.addNewIndex(m[i], data, nn);
		treeAVL.add(m[i], data, n2);
	}
	
	nn = 0;
	n2 = 0;
	long long I = 0, D = 0, S = 0;
	long long I2 = 0, D2 = 0, S2 = 0;

	for(int i = 0; i < n/2; i++)
		if(i%10 == 0)
		{
			tree.deleteIndex(lrand(), nn);
			treeAVL.delet(lrand(), n2);
			D += nn;
			D2 += n2;
			tree.addNewIndex(m[rand()%n], data, nn);
			treeAVL.add(m[rand()%n], data, n2);
			I += nn;
			I2 += n2;
			try
			{
				tree.readObject(lrand(), nn);
				treeAVL.readObject(lrand(), n2);
			}
			catch(exception&) { }
			S += nn;
			S2 += n2;
		}
		else
		{
			int ind = rand() % n;
			tree.deleteIndex(m[ind], nn);
			treeAVL.delet(m[ind], n2);
			D += nn;
			D2 += n2;
			MyTypeD data = lrand();
			tree.addNewIndex(data, data, nn);
			treeAVL.add(data, data, n2);
			I += nn;
			I2 += n2;
			m[ind] = data;
			try
			{
				tree.readObject(m[rand()%n], nn);
				treeAVL.readObject(m[rand()%n], n2);
			}
			catch(exception&) { }
			S += nn;
			S2 += n2;
		}

	//cout << "Размер дерева после удаления BST: " << tree.sizeOfTree() << "\tAVL: " << treeAVL.sizeOfTree() << endl;
	//cout << n << endl;
	cout << /**/"1.39*log2(n) = " << 1.39*(log((double)n)/log(2.)) << endl;
	cout << /**/"Вставка BST: " << I/(n/2.) << "\t" << I2/(n/2.) << endl;
	cout << /**/"Удаление BST: " << D/(n/2.) << "\t" << D2/(n/2.) << endl;
	cout << /**/"Поиск BST: " << S/(n/2.) << "\t" << S2/(n/2.) <<endl;

	delete[] m;
}

void test_sort(int n)
{
	Tree<MyTypeK, MyTypeD> tree;
	AVL_Tree<MyTypeK, MyTypeD> treeAVL;

	MyTypeK* m = new MyTypeK[n];
	MyTypeD v = 1;
	int nn = 0;
	int n2 = 0;
	for(int i = 0; i < n; i++)
	{
		m[i] = i*1000;
		tree.addNewIndex(m[i], v, nn);
		treeAVL.add(m[i], v, n2); 

	}
	
	nn = 0;
	n2 = 0;
	long long I = 0;
	long long D = 0;
	long long S = 0;
	long long I2 = 0;
	long long D2 = 0;
	long long S2 = 0;

	for(int i = 0; i < n/2; i++)
		if(i%10 == 0)
		{
			tree.deleteIndex(lrand(), nn);
			treeAVL.delet(lrand(), n2);
			D += nn;
			D2 += n2;
			tree.addNewIndex(m[rand()%n], v, nn);
			treeAVL.add(m[rand()%n], v, n2);
			I += nn;
			I2 += n2;
			try
			{
				tree.readObject(lrand(), nn);
				treeAVL.readObject(lrand(), n2);
			}
			catch(exception&) { }
			S += nn;
			S2 += n2;
		}
		else
		{
			int ind = rand()%n;
			tree.deleteIndex(m[ind], nn);
			treeAVL.delet(m[ind], n2);
			D += nn;
			D2 += n2;
			MyTypeK key = lrand()%1000*n;
			key = key + key%2;
			tree.addNewIndex(key, v, nn);
			treeAVL.add(key, v, n2);
			I += nn;
			I2 += n2;
			m[ind]=key;
			try
			{
				tree.readObject(m[rand()%n], nn);
				treeAVL.readObject(m[rand()%n], n2);
			}
			catch(exception&) { }
			S += nn;
			S2 += n2;
		}

	//cout << "Размер дерева после удаления BST: " << tree.sizeOfTree()<< " AVL: " << treeAVL.sizeOfTree() << endl;
	//cout << "n/2 = " << n / 2. << endl;
	cout<</**/"Вставка BST: " << I/(n/2.) << "\t" << I2/(n/2.) << endl;
	cout<</**/"Удаление BST: " << D/(n/2.) << "\t" << D2/(n/2.) << endl;
	cout<</**/"Поиск BST: " << S/(n/2.) << "\t" << S2/(n/2.) << endl;
	delete[] m;
}

bool CheckLimits(const Tree <MyTypeD, MyTypeK>::Iterator& it)
{
	if (it.is_off())
		return true;
	else
		return false;
}

bool CheckLimits(const Tree <MyTypeD, MyTypeK>::rIterator& it)
{
	if (it.is_off())
		return true;
	else
		return false;
}

int main(int argc, char* argv[])
{
	Tree <MyTypeK, MyTypeD> tree;
	Tree <MyTypeK, MyTypeD>::Iterator iter(tree);
	Tree <MyTypeK, MyTypeD>::rIterator riter(tree);
	AVL_Tree<MyTypeK, MyTypeD> t;
	AVL_Tree<MyTypeK, MyTypeD>::Iterator it(t);
	AVL_Tree<MyTypeK, MyTypeD>::rIterator rit(t);
	
	int nnn = 0, looked = 0;
	MyTypeD tmp;
	MyTypeK key;
	bool isExit = false;	//Флаг выхода
	setlocale(LC_ALL, "Russian");

	cout << "Меню:" << endl;
	cout << "0) опрос размера дерева" << endl;
	cout << "1) очистка дерева" << endl;
	cout << "2) проверка дерева на пустоту" << endl;
	cout << "3) доступ к данным с заданным ключом" << endl;
	cout << "4) включение данных с заданным ключом" << endl;
	cout << "5) удаление данных с заданным ключом" << endl;
	cout << "6) определение длины внешнего пути дерева" << endl;
	cout << "7) вывод структуры дерева на экран" << endl;
	cout << "8) опрос числа просмотренных операцией узлов дерева" << endl;
	cout << "9) обход узлов дерева по схеме LtR" << endl;
	cout << "Итератор:" << endl;
	cout << "\t12) установка на первый узел в дереве с  минимальным  ключом" << endl;
	cout << "\t13) установка на end()" << endl;
	cout << "\t14) установка на следующий" << endl;
	cout << "\t15) установка на предыдущий" << endl;
	cout << "\t16) проверка состояния итератора" << endl;
	cout << "\t17) доступ по чтению к данным текущего узла в дереве" << endl;
	cout << "\t18) доступ записи к данным текущего узла в дереве" << endl;
	cout << "Обратный итератор:" << endl;
	cout << "\t19) установка на первый узел в дереве с  максимальным  ключом" << endl;
	cout << "\t20) установка на rend()" << endl;
	cout << "\t21) установка на следующий" << endl;
	cout << "\t22) установка на предыдущий" << endl;
	cout << "\t23) проверка состояния итератора" << endl;
	cout << "\t24) доступ по чтению к данным текущего узла в дереве" << endl;
	cout << "\t25) доступ записи к данным текущего узла в дереве" << endl;
	cout << "26) Заполнить для тестов " << endl;
	cout << "27) Выход" << endl;
	cout << "\t28) тест 1" << endl;
	cout << "\t29) тест 2" << endl;
	
	while(!isExit)
	{
		int ans;	//Код нажатой клавиши
		cin >> ans;
		
		try
		{
			switch(ans)
			{
			case 0:	
				cout << "Размер дерева: " << t.sizeOfTree() << endl;
				break;
			
			case 1:	
				t.deleteTree();
				cout<< "Дерево очищено"<<endl;
				break;

			case 2:
				if (t.EmptyTree()) {
					cout << "Дерево пусто" << endl;
				}
				else {
					cout << "Дерево не пусто" << endl;
				}
				
				break;

			case 3:			
				cout << "Введите ключ: ";
				cin >> key;
				cout << "Значение по заданному ключу: " << t.readObject(key, looked) << endl;
				break;
			
			case 4:
				cout << "Введите ключ: ";
				cin >> key;
				cout << "Значение: ";
				cin >> tmp;
				if (t.add(key, tmp, looked)) {
					cout << "Узел добавлен" << endl;
				}
				else {
					cout << "Ошибка!" << endl;
				}
				
				break;

			case 5:
				cout << "Введите ключ: ";
				cin >> key;
				if (t.delet(key, looked)) {
					cout << "Узел удален" << endl;
				}
				else {
					cout << "Ошибка!" << endl;
				}
				
				break;

			case 6:
				cout << "Длина внешнего пути: " << t.lengthofThree() << endl;
				break;
			
			case 7:
				t.show_tree();
				break;
			
			case 8:
				cout << "Количество итераций последней операции: " << looked << endl;
				break;

            case 9:
				t.go_LtR();
				break;

			/*case 10:
				{
					Tree<MyTypeD, MyTypeK> tmp(t);
					cout << "Исходное дерево:" << endl;
					t.show_tree();
					cout << "Скопированное дерево:" << endl;
					tmp.show_tree();
				}
				break;*/

			case 11:
				break;

			case 12:
				it.beg();
				cout << endl;
				break;

			case 13:
				it.end();
				cout << endl;
				break;

			case 14:
				it.next();
				cout << CheckLimits(it)<<endl;
				break;
			
			case 15:
				it.prev();
				cout << CheckLimits(it)<<endl;
				break;
			
			case 16:
				if (CheckLimits(it)) {
					cout << "Итератор находится в пределах дерева " << endl;
				}
				else {
					cout << "Итератор находится за пределами дерева " << endl;
				}
				
				break;
				
			case 17:
				cout << "Значение итератора: " << * it << endl;
				break;

			case 18:
				cout << "Новое значение: ";
				cin >> *it;
				break;
			case 19:
				rit.rbeg();
				cout << endl;
				break;

			case 20:
				rit.rend();
				cout << endl;
				break;

			case 21:
				rit.rnext();
				cout << CheckLimits(rit) << endl;
				break;

			case 22:
				rit.rprev();
				cout << CheckLimits(rit) << endl;
				break;

			case 23:
				cout << CheckLimits(rit) << endl;
				break;

			case 24:
				cout << "Значение итератора: " << * rit << endl;
				break;

			case 25:
				cout << "Новое значение: ";
				cin >> *rit;
				break;

			case 26:
				cout << "Введите размер: ";
				cin >> nnn;
				test_add(nnn, tree, t);
				break;

			case 27:	//Выход
				isExit = true;
				break;

			case 28:
				cout << "Введите размер" << endl;
				cin >> nnn;
				test_rand(nnn);
				break;

			case 29:
				cout << "Введите размер" << endl;
				cin >> nnn;
				test_sort(nnn);
				break;

			default:	//Нажата клавиша, не соответствующая ни одной команде
				cout << "Неизвестная команда" << endl;
				break;
			}
		}
		catch(exception& exc)
		{
			cout << exc.what() << endl;
		}
	}
	return 0;
}
