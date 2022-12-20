#pragma once
#include "Graph.h"
#include "Input.h"
#include <algorithm>

#define FirstTask Task1v2
#define SecondTask Task2v12



template <class Vertex, class Edge>
class Task
{
public:
	Task(Graph<Vertex, Edge>& graph): graph(&graph) { }
	void Set(Graph<Vertex, Edge>& graph) { this->graph = &graph; Restart(); }
	virtual ~Task() = default;
	virtual void Restart() = 0;
	virtual void Result() = 0;
protected:
	Graph<Vertex, Edge>* graph;
};

#pragma region Task1

string task1Description = "Определение кратчайших по числу ребер путей между всеми парами вершин орграфа";

template <class Vertex, class Edge>
class Task1v2 : public Task<Vertex, Edge>
{
public:
	Task1v2(Graph<Vertex, Edge>& graph) : Task<Vertex, Edge>(graph)
	{
		if (graph.IsDirected() == false)
			throw "Граф должен быть ориентированным";
		Restart();
	};
	virtual void Restart();
	virtual void Result();
protected:
	vector<vector<string>> result;
	vector<vector<int>> GetPathMatrix();
	vector<Vertex*> GetAllVertexes();
	vector<vector<int>> Dijkstra(int s, vector<vector<int>>& weights);
	vector<string> IndexesToVertexNames(vector<int>& indexes, vector<Vertex*>& vertexes);
};

template<class Vertex, class Edge>
inline void Task1v2<Vertex, Edge>::Result()
{
	if (result.empty())
		cout << "Пути не найдены\n";
	for (auto path : result)
	{
		for (int i = 0; i < path.size() - 1; i++)
			cout << path[i] << "->";

		cout << path.back();
		cout << endl;
	}
}

template<class Vertex, class Edge>
inline void Task1v2<Vertex, Edge>::Restart()
{
	result.clear();

	vector<vector<int>> paths = GetPathMatrix();
	vector<Vertex*> vertexes = GetAllVertexes();

	for (int i = 0; i < paths.size(); i++)
	{
		vector<vector<int>> pathsFromV = Dijkstra(i, paths);

		for (auto path : pathsFromV)
		{
			if (path.empty() == false)
			{
				result.push_back(IndexesToVertexNames(path, vertexes));
			}
		}

	}
}

template<class Vertex, class Edge>
inline vector<vector<int>> Task1v2<Vertex, Edge>::GetPathMatrix()
{
	int vertexes = Task<Vertex, Edge>::graph->GetVertexesCount();

	vector<vector<int>> weights;

	for (int i = 0; i < vertexes; i++)
	{
		weights.push_back(vector<int>());
		for (int j = 0; j < vertexes; j++)
			weights[i].push_back(numeric_limits<int>::max());
	}

	typename Graph<Vertex, Edge>::EdgesIterator iter(*Task<Vertex, Edge>::graph);

	while (iter != iter.End())
	{
		Vertex* v1 = (*iter).V1();
		Vertex* v2 = (*iter).V2();

		weights[v1->index][v2->index] = 1;

		if (Task<Vertex, Edge>::graph->IsDirected() == false)
			weights[v2->index][v1->index] = 1;

		iter++;
	}

	return weights;
}

template<class Vertex, class Edge>
inline vector<Vertex*> Task1v2<Vertex, Edge>::GetAllVertexes()
{
	vector<Vertex*> vertexes;
	typename Graph<Vertex, Edge>::VertexesIterator iter(*Task<Vertex, Edge>::graph);

	while (iter != iter.End())
	{
		vertexes.push_back(&(*iter));
		iter++;
	}

	return vertexes;
}

template<class Vertex, class Edge>
inline vector<vector<int>> Task1v2<Vertex, Edge>::Dijkstra(int s, vector<vector<int>>& matrix)
{
	vector<vector<int>> paths;
	paths.resize(matrix.size());

	for (int i = 0; i < matrix.size(); i++)
	{
		if (matrix[s][i] != INT_MAX)
			paths[i].push_back(i);
	}

	vector <bool> visited(matrix.size(), false);

	for (int count = 0; count < matrix.size() - 1; count++)
	{
		int min = INT_MAX;
		int u;
		for (int i = 0; i < matrix.size(); i++)
		{
			if (!visited[i] && matrix[s][i] <= min)
			{
				min = matrix[s][i]; u = i;
			}
		}

		visited[u] = true;

		for (int i = 0; i < matrix.size(); i++)
		{
			if (!visited[i] && matrix[u][i] != INT_MAX && matrix[s][u] != INT_MAX &&
				matrix[s][u] + matrix[u][i] < matrix[s][i])
			{
				matrix[s][i] = matrix[s][u] + matrix[u][i];


				paths[i] = vector<int>(paths[u]);
				paths[i].push_back(i);
			}
		}
	}

	for (auto& path : paths)
	{
		if (path.empty() == false)
			path.insert(path.begin(), s);
	}

	return paths;
}

template<class Vertex, class Edge>
inline vector<string> Task1v2<Vertex, Edge>::IndexesToVertexNames(vector<int>& indexes, vector<Vertex*>& vertexes)
{
	vector<string> result;

	for (auto index : indexes)
		result.push_back(vertexes[index]->GetName());

	return result;
}

#pragma endregion

#pragma region Task2

string task2Description = "Построение минимального остовного дерева относительно заданного взвешенного неориентированного графа методом Прима";


template <class Vertex, class Edge>
class Task2v12 : public Task<Vertex, Edge>
{
public:
	Task2v12(Graph<Vertex, Edge>& graph) : Task<Vertex, Edge>(graph)
	{
		if (graph.IsDirected())
			throw "Граф должен быть неориентированным";
		Restart();
	};
	virtual void Restart();
	virtual void Result();
protected:
	vector<int> result;
};

template<class Vertex, class Edge>
inline void Task2v12<Vertex, Edge>::Result()
{
	for (int i = 1; i < result.size(); i++)
	{
		cout << "Вершина " << i << " " << " Вторая вершина "  << result[i] << endl;
	}
}

template<class Vertex, class Edge>
inline void Task2v12<Vertex, Edge>::Restart()
{
	result.clear();
	int weightOstTree = 0;
	int size = Task<Vertex, Edge>::graph->GetVertexesCount();
	long long* keys = new long long[size];
	int* AncestorsEdge = new int[size]; // предки каждой вершины
	int* q = new int[size];
	for (unsigned i = 0; i < size; ++i) {
		keys[i] = 1000000000000000000;
		AncestorsEdge[i] = -1;
		q[i] = 1;
	}
	//Vertex* vertex = Task<Vertex, Edge>::graph->getVertex(0);
	keys[0] = 0;
	int n = size;
	while (n != 0) {
		int minVertex = -1;
		for (unsigned i = 0; i < size; ++i) {
			if (q[i] == 1 && (minVertex == -1 || keys[i] < keys[minVertex])) {
				minVertex = i;
			}
		}
		for (unsigned i = 0; i < size; ++i) {
			if (Task<Vertex, Edge>::graph->Graph<Vertex, Edge>::IsEdgeIntExists(minVertex, i)) {
				//string strMinVertex = to_string(minVertex); Graph<Vertex, Edge>::
				//string strI = to_string(i);
				Edge* edge = Task<Vertex, Edge>::graph->Graph<Vertex, Edge>::GetIntEdge(minVertex, i);
				int weight = edge->GetWeight();
				if (i != minVertex && q[i] == 1 && keys[i] > weight) {
					AncestorsEdge[i] = minVertex;
					keys[i] = weight;
					weightOstTree += weight;
				}
			}
		}
		--n;
		q[minVertex] = 0;
	}
	for (int i = 0; i < size; i++) {
		result.push_back(AncestorsEdge[i]);
	}
}

#pragma endregion