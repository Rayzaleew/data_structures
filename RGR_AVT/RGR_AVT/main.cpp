#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include "Graph.h"
#include "VertexMap.h"
#include "Input.h"
#include "Tasks.h"

using namespace std;

using commandView = pair<int, string>;
using _Vertex = Vertex<string, int>;
using _Edge = Edge<_Vertex, int, string>;
using RGZGraph = Graph<_Vertex, _Edge>;

#define SUCCESS_INPUT 0
#define BACK_TO_MENU -1

RGZGraph* graph = new RGZGraph();
VertexMap<string, _Vertex*> vertexMap;
RGZGraph::VertexesIterator vIterator(*graph);
RGZGraph::EdgesIterator eIterator(*graph);
RGZGraph::OutputEdgesIterator outEIterator(*graph, nullptr);

FirstTask<_Vertex, _Edge>* task1 = nullptr;
SecondTask<_Vertex, _Edge>* task2 = nullptr;

void printCommands(vector<commandView> commandsView)
{
	auto printCommand = [](commandView view)
	{
		cout << (int)view.first << " - " << view.second;
	};

	for (int i = 0, j = commandsView.size() / 2; i < commandsView.size() / 2; i++, j++)
	{
		printCommand(commandsView[i]);

		int k = (int)commandsView[i].first / 10;
		int len = 1;

		while (k != 0)
		{
			len++; k /= 10;
		}

		cout << string(70 - len - commandsView[i].second.length(), ' ');
		printCommand(commandsView[j]);

		cout << endl;
	}

	if (commandsView.size() % 2 != 0)
	{
		printCommand(commandsView.back());
		cout << endl;
	}

}

bool getVertex(_Vertex** vertex, string hint)
{
	*vertex = vertexMap.Get(Input<string>::Get(hint));

	if (*vertex == nullptr)
	{
		cout << "������� �� �������\n";
		return false;
	}
	return true;
}

bool getTwoVertixes(_Vertex** array)
{
	for (int i = 0; i < 2; i++)
	{
		if (!getVertex(&array[i], "��� ������� #" + to_string(i + 1)))
			return false;
	}
	return true;
}

bool getEdge(_Edge** edge)
{
	_Vertex* vertexes[2];

	if (!getTwoVertixes(vertexes))
		return false;

	*edge = graph->GetEdge(vertexes[0], vertexes[1]);

	if (*edge == nullptr)
	{
		cout << "����� �� �������\n";
		return false;
	}
		
	return true;
}

void setNamesToAllVertexes()
{
	typename Graph<_Vertex, _Edge>::VertexesIterator iter(*graph);
	vector<int> code;
	code.push_back(0);

	while (iter != iter.End())
	{
		string name;

		for (int i = 0; i < code.size(); i++)
			name.insert(name.end(), (char)(code[i] + 97));

		(*iter).SetName(name);

		vertexMap.Add(name, &(*iter));

		if (code.back() == 26)
			code.push_back(0);
		else
			code.back()++;

		iter++;
	}
}

void updateGraphHelpers()
{
	vIterator = RGZGraph::VertexesIterator(*graph);
	eIterator = RGZGraph::EdgesIterator(*graph);
	outEIterator = RGZGraph::OutputEdgesIterator(*graph, nullptr);
	vertexMap.Clear();
}

class CommandsHandler
{
public:
	virtual int HandleInput() = 0;
protected:
	virtual void PrintCommands() = 0;
};

class GraphCommands : public CommandsHandler
{
public:
	GraphCommands() { PrintCommands(); }

	enum class Command { Create, CreateVDF, CreateVEDF, IsDirected, GetSaturation, GetForm, SetForm, 
		GetEdgesCount, AddEdge, GetEdge, SetEdgeData, GetEdgeData, SetEdgeWeight, GetEdgeWeight,
		RemoveEdge, GetVertexesCount, AddVertex, SetVertexData, GetVertexData, SwitchName, RemoveVertex, 
		Print, PrintCommands, ReturnToMenu
	};

	int HandleInput()
	{
		int input = Input<int>::Get("����� �������");

		switch (input)
		{
		case (int)Command::Create:
			delete graph;
			graph = new RGZGraph();
			updateGraphHelpers();
			break;
		case (int)Command::CreateVDF:
		{
			delete graph;
			RGZGraph::Form form = (RGZGraph::Form)(Input<bool>::Get("����� ����� L/M"));
			graph = new RGZGraph(Input<int>::Get("���������� ������"), Input<bool>::Get("������������ �� ����"), form);
			updateGraphHelpers();
			setNamesToAllVertexes();
			break;
		}
		case (int)Command::CreateVEDF:
		{
			delete graph;
			RGZGraph::Form form = (RGZGraph::Form)(Input<bool>::Get("����� ����� L/M"));
			graph = new RGZGraph(Input<int>::Get("���������� ������"), Input<int>::Get("���������� ��������� ����"), Input<bool>::Get("������������ �� ����"), form);
			updateGraphHelpers();
			setNamesToAllVertexes();
			break;
		}
		case (int)Command::PrintCommands:
			PrintCommands();
			break;
		case (int)Command::ReturnToMenu:
			return BACK_TO_MENU;
		case (int)Command::IsDirected:
			cout << graph->IsDirected() << endl;
			break;
		case (int)Command::GetForm:
			cout << (graph->GetForm() == RGZGraph::Form::L ? "L\n" : "M\n");
			break;
		case (int)Command::Print:
			graph->Print();
			break;
		case (int)Command::SetForm:
			graph->SetForm(RGZGraph::Form(Input<bool>::Get("����� ����� (L/M)")));
			break;
		case (int)Command::AddVertex:
		{
			string name = Input<string>::Get("��� �������");
			_Vertex* v = vertexMap.Get(name);
			if (v != nullptr)
			{
				cout << "������� � ����� ������ ��� ����������\n";
				break;
			}
			v = graph->AddVertex();
			v->SetName(name);
			vertexMap.Add(name, v);
			break;
		}
		case (int)Command::RemoveVertex:
		{
			string name = Input<string>::Get("��� �������");
			_Vertex* v = vertexMap.Get(name);
			if (v == nullptr)
			{
				cout << "������� � ����� ������ �� ����������\n";
				break;
			}

			graph->RemoveVertex(v);
			vertexMap.Remove(name);
			break;
		}
		case (int)Command::AddEdge:
		{
			_Vertex* vertexes[2];
			if (!getTwoVertixes(vertexes))
				break;

			_Edge* edge = graph->AddEdge(vertexes[0], vertexes[1]);
			cout << (edge == nullptr ? "Fail\n" : "Success\n");
			break;
		}
		case (int)Command::RemoveEdge:
		{
			_Vertex* vertexes[2];
			if (!getTwoVertixes(vertexes))
				break;

			cout << "����� ������: " << graph->RemoveEdge(vertexes[0], vertexes[1]) << endl;
			break;
		}
		case (int)Command::GetEdge:
		{
			_Edge* edge; if (!getEdge(&edge)) break;
			edge->Print();
			cout << endl;
			break;
		}
		case (int)Command::SetEdgeData:
		{
			_Edge* edge; if (!getEdge(&edge)) break;
			edge->SetData(Input<string>::Get("������"));
			break;
		}
		case (int)Command::GetEdgeData:
		{
			_Edge* edge; if (!getEdge(&edge)) break;
			if (edge->HasData())
				cout << edge->GetData() << endl;
			else
				cout << "������ �� ������\n";
			break;
		}
		case (int)Command::SetEdgeWeight:
		{
			_Edge* edge; if (!getEdge(&edge)) break;
				edge->SetWeight(Input<int>::Get("���"));
			break;
		}
		case (int)Command::GetEdgeWeight:
		{
			_Edge* edge; if (!getEdge(&edge)) break;
			if (edge->HasWeight())
				cout << edge->GetWeight() << endl;
			else
				cout << "��� �� �����\n";;
			break;
		}
		case (int)Command::SetVertexData:
		{
			_Vertex* vertex;
			if (getVertex(&vertex, "��� �������") == false)
				break;

			vertex->SetData(Input<int>::Get("������"));
			break;
		}
		case (int)Command::GetVertexData:
		{
			_Vertex* vertex;
			if (getVertex(&vertex, "��� �������") == false)
				break;

			if (vertex->HasData())
				cout << vertex->GetData() << endl;
			else
				cout << "������ �� ������\n";
			break;
		}
		case (int)Command::GetEdgesCount:
			cout << graph->GetEdgesCount() << endl;
			break;
		case (int)Command::GetVertexesCount:
			cout << graph->GetVertexesCount() << endl;
			break;
		case (int)Command::GetSaturation:
			cout << graph->GetSaturation() << endl;
			break;
		case (int)Command::SwitchName:
			cout << "����� ������: " << vertexMap.SwitchName(Input<string>::Get("������ ��� �������"), Input<string>::Get("����� ��� �������")) << endl;
			break;
		default:
			cout << "�������� ����� �������\n";
		};

		return SUCCESS_INPUT;
	}

protected:

	vector<commandView> commands = {
	{(int)Command::Create, "������� ��c��� L-����"},
	{(int)Command::CreateVDF, "������� ���� � ������ ���-�� ������, ���, �����"},
	{(int)Command::CreateVEDF, "������� ���� � ������ ���-�� ������ � ����. ����, ���, �����"},
	{(int)Command::IsDirected, "��������������� �� ����"},
	{(int)Command::GetSaturation, "����. ������������"},
	{(int)Command::GetForm, "�������� �����"},
	{(int)Command::SetForm, "������ �����"},
	{(int)Command::GetEdgesCount, "���������� ����"},
	{(int)Command::AddEdge, "�������� �����"},
	{(int)Command::GetEdge, "�������� �����"},
	{(int)Command::SetEdgeData, "������ ������ �����"},
	{(int)Command::GetEdgeData, "�������� ������ �����"},
	{(int)Command::SetEdgeWeight, "������ ��� �����"},
	{(int)Command::GetEdgeWeight, "�������� ��� �����"},
	{(int)Command::RemoveEdge, "������� �����"},
	{(int)Command::GetVertexesCount, "���������� ������"},
	{(int)Command::AddVertex, "�������� �������"},
	{(int)Command::SetVertexData, "������ ������ �������"},
	{(int)Command::GetVertexData, "�������� ������ �������"},
	{(int)Command::SwitchName, "������� ��� �������"},
	{(int)Command::RemoveVertex, "������� �������"},
	{(int)Command::Print, "������� ����"},
	{(int)Command::PrintCommands, "�������� ����� � ������� �������"},
	{(int)Command::ReturnToMenu, "����� �� �������� ����"} };

	void PrintCommands() { system("CLS"); printCommands(commands); }
};

class IteratorsCommands : public CommandsHandler
{
public:
	IteratorsCommands() { PrintCommands(); }

	enum class Command {
		VIteratorBegin, VIteratorValue, VIteratorInc, VIteratorEnd,
		EIteratorBegin, EIteratorValue, EIteratorInc, EIteratorEnd,
		OutEIteratorBegin, OutEIteratorValue, OutEIteratorInc, OutEIteratorEnd, 
		PrintCommands, ReturnToMenu
	};

	int HandleInput()
	{
		int input = Input<int>::Get("����� �������");

		switch (input)
		{
		case (int)Command::VIteratorBegin:
			vIterator = vIterator.Begin();
			break;
		case (int)Command::VIteratorValue:
		{
			try { (*vIterator).Print(); }
			catch (const char* ex) { cout << ex; }
			cout << endl;
			break;
		}
		case (int)Command::VIteratorInc:
			cout << "����� ������: " << vIterator++ << endl;
			break;
		case (int)Command::VIteratorEnd:
			vIterator = vIterator.End();
			break;
		case (int)Command::EIteratorBegin:
			eIterator = eIterator.Begin();
			break;
		case (int)Command::EIteratorValue:
		{
			try { (*eIterator).Print(); }
			catch (const char* ex) { cout << ex; }
			cout << endl;
			break;
		}
		case (int)Command::EIteratorInc:
			cout << "����� ������: " << eIterator++ << endl;
			break;
		case (int)Command::EIteratorEnd:
			eIterator = eIterator.End();
			break;
		case (int)Command::OutEIteratorBegin:
		{
			_Vertex* vertex;

			if (!getVertex(&vertex, "��� �������"))
				break;
			
			outEIterator = RGZGraph::OutputEdgesIterator(*graph, vertex);
			break;
		}
		case (int)Command::OutEIteratorValue:
		{
			try { (*outEIterator).Print(); }
			catch (const char* ex) { cout << ex; }
			cout << endl;
			break;
		}
		case (int)Command::OutEIteratorInc:
			cout << "����� ������: " << outEIterator++ << endl;
			break;
		case (int)Command::OutEIteratorEnd:
			outEIterator = outEIterator.End();
			break;
		case (int)Command::PrintCommands:
			PrintCommands();
			break;
		case (int)Command::ReturnToMenu:
			return BACK_TO_MENU;
		default:
			cout << "�������� ����� �������\n";
		};

		return SUCCESS_INPUT;
	}

protected:

	vector<commandView> commands = {
	{(int)Command::VIteratorBegin, "�������� ������: ���������� � ������"},
	{(int)Command::VIteratorValue, "�������� ������: �������� ��������"},
	{(int)Command::VIteratorInc, "�������� ������: ��������� �������"},
	{(int)Command::EIteratorBegin, "�������� ����: ���������� � ������"},
	{(int)Command::EIteratorValue, "�������� ����: �������� ��������"},
	{(int)Command::EIteratorInc, "�������� ����: ��������� �����"},
	{(int)Command::OutEIteratorBegin, "�������� ���. ����: ���������� � ������"},
	{(int)Command::OutEIteratorValue, "�������� ���. ����: �������� ��������"},
	{(int)Command::OutEIteratorInc, "�������� ���. ����: ��������� �����"},
	{(int)Command::PrintCommands, "�������� ����� � ������� �������"},
	{(int)Command::ReturnToMenu, "����� �� �������� ����"} };

	void PrintCommands() { system("CLS"); printCommands(commands); }
};

template <class T>
class TaskCommands : public CommandsHandler
{
public:
	TaskCommands(T** task, string description): task(task), taskDescription(description) { PrintCommands(); }

	enum class Command {
		Create, Restart, Result, PrintCommands, ReturnToMenu
	};

	int HandleInput()
	{
		int input = Input<int>::Get("����� �������");

		switch (input)
		{
		case (int)Command::Create:
		{
			delete* task;
			try { *task = new T(*graph); }
			catch (const char* ex) 
			{ 
			cout << ex << endl; 
			*task = nullptr;
			}		
			break;
		}
		case (int)Command::Restart:
		{
			if (*task == nullptr)
			{
				cout << "�������� ������ �������\n";
				break;
			}

			try { static_cast<Task<_Vertex, _Edge>*>(*task)->Restart(); }
			catch (const char* ex)
			{
				cout << ex << endl;
			}
			
			break;
		}
		case (int)Command::Result:
			if (*task == nullptr)
			{
				cout << "�������� ������ �������\n";
				break;
			}
			static_cast<Task<_Vertex, _Edge>*>(*task)->Result();
			break;
		case (int)Command::PrintCommands:
			PrintCommands();
			break;
		case (int)Command::ReturnToMenu:
			return BACK_TO_MENU;
		default:
			cout << "�������� ����� �������\n";
		};

		return SUCCESS_INPUT;
	}

protected:

	vector<commandView> commands = {
	{(int)Command::Create, "������� ������, ����������� � ����� � ������"},
	{(int)Command::Restart, "������ ������� ������"},
	{(int)Command::Result, "������� ���������"},
	{(int)Command::PrintCommands, "�������� ����� � ������� �������"},
	{(int)Command::ReturnToMenu, "����� �� �������� ����"} };

	T** task;
	string taskDescription;

	void PrintCommands() { system("CLS"); cout << taskDescription << endl; printCommands(commands); }
};

class MainMenu
{
public:

	void HandleCurrentMenuInput()
	{
		if (handler != nullptr)
		{
			int code = handler->HandleInput();

			if (code == BACK_TO_MENU)
			{
				delete handler;
				handler = nullptr;
				system("CLS");
			}

			return;
		}

		cout << "1 - ����\n";
		cout << "2 - ������� ��� ����������\n";
		cout << "3 - ������� �2\n";
		cout << "4 - ������� �3\n";
		cout << "5 - �����\n";

		int input = Input<int>::Get("����� �������");

		switch (input)
		{
		case 1:
			handler = new GraphCommands();
			break;
		case 2:
			handler = new IteratorsCommands();
			break;
		case 3:
			handler = new TaskCommands<FirstTask<_Vertex, _Edge>>(&task1, task1Description);
			break;
		case 4:
			handler = new TaskCommands<SecondTask<_Vertex, _Edge>>(&task2, task2Description);
			break;
		case 5:
			exit(0);
			break;
		default:
			cout << "������������ ����� �������\n";
			break;
		}

	}

protected:

	CommandsHandler* handler = nullptr;
};

int main()
{
	setlocale(LC_ALL, "Russian");
	srand(time(NULL));

	MainMenu menu;

	while (true)
	{
		menu.HandleCurrentMenuInput();
	}
}