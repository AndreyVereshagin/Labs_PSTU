#pragma once
#include <GL/glut.h>
#include <stdio.h>
#include <iostream>
#include <vector>
#include <sstream>
using namespace std;

int n;
int** help;
int* result;
int*** mat;

int WinW;
int WinH;

const int maxSize = 20;
int amountVerts;

template<class T>
class Graph
{
	vector<T> vertexList;
	vector<T> labelList;
	bool* visitedVerts = new bool[vertexList.size()];

public:
	int adjMatrix[maxSize][maxSize] = { 0 };
	Graph();
	~Graph();
	void DrawGraph();
	void InsertEdge(const T& vertex1, const T& vertex2, int weight);
	void InsertVertex(const T& vertex);
	void DeleteVertex();
	int GetVertPos(const T& vertex);
	bool IsEmpty();
	bool IsFull();
	int GetAmountVerts();
	int GetAmountEdges();
	int GetWeight(const T& vertex1, const T& vertex2);
	vector<T> GetNbrs(const T& vertex);
	void Print();
};

int R;

struct vertexCoord
{
	int x, y;
};
vertexCoord vertC[20];

Graph<int> graph;	

void answer(int*** mat, int n, int** help, int* path)
{

	for (int l = 0; l < n; l++)
	{
		for (int i = 0; i < n; i++)
		{
			int min = 1000000;

			for (int j = 0; j < n; j++)
				if (mat[i][j] && min > *mat[i][j])
					min = *mat[i][j];

			for (int j = 0; j < n; j++)
				if (mat[i][j])
					*mat[i][j] -= min;
		}

		for (int j = 0; j < n; j++)
		{
			int min = 1000000;

			for (int i = 0; i < n; i++)
				if (mat[i][j] && min > *mat[i][j])
					min = *mat[i][j];

			for (int i = 0; i < n; i++)
				if (mat[i][j])
					*mat[i][j] -= min;
		}

		for (int i = 0; i < n; i++)
			for (int j = 0; j < n; j++)
				help[i][j] = 0;

		for (int i = 0; i < n; i++)
			for (int j = 0; j < n; j++)
			{
				if (mat[i][j] && !*mat[i][j])
				{
					int hmin = 1000000;
					int vmin = 1000000;

					for (int l = 0; l < n; l++)
						if (l != i && mat[l][j] && hmin > *mat[l][j])
							hmin = *mat[l][j];

					for (int l = 0; l < n; l++)
						if (l != j && mat[i][l] && vmin > *mat[i][l])
							vmin = *mat[i][l];

					help[i][j] = hmin + vmin;
				}
			}

		int mcost = 0, mi = 0, mj = 0;
		for (int i = 0; i < n; i++)
			for (int j = 0; j < n; j++)

				if (mat[i][j] && mcost < help[i][j])
				{
					mcost = help[i][j];
					mi = i;
					mj = j;
				}

		path[mi] = mj;

		for (int i = 0; i < n; i++) mat[i][mj] = nullptr;

		for (int i = 0; i < n; i++) mat[mi][i] = nullptr;

		mat[mj][mi] = nullptr;
	}
}

void prep(int***& mat, int& n, int**& help, int*& result)
{
	n = amountVerts;

	help = new int* [n];

	result = new int[n];

	mat = new int** [n];

	for (int i = 0; i <= n; i++)
	{
		help[i] = new int[n];
	}

	for (int i = 0; i < n; i++)
	{
		mat[i] = new int* [n];

		for (int j = 0; j < n; j++)
		{
			if (graph.adjMatrix[i][j] == 0) {
				mat[i][j] = nullptr;
				continue;
			}
			mat[i][j] = new int(graph.adjMatrix[i][j]);
		}
	}
}

void salesman(int*** mat, int n, int** help, int* result)
{
	prep(mat, n, help, result);
	int s = 0;
	answer(mat, n, help, result);
	cout << "\nОтрезки путей: ";

	for (int i = 0, j = 0; i < n; i++)
	{
		j = result[i];
		cout << i + 1 << " -> " << j + 1 << '\t';
		s += graph.adjMatrix[i][j];
	}

	cout << endl;
	cout << "\nКратчайший путь: ";
	int temp = 0;

	for (int l = 0; l < n;)
	{
		for (int i = 0, j = 0; i < n; i++)
		{
			if (temp == 0 || i + 1 == temp)
			{
				if (temp == 0) cout << i + 1;
				j = result[i];
				temp = j + 1;
				if (temp > 0)	cout << " -> " << temp;
				l++;
			}
		}
	}

	cout << "\nРасстояние: " << s;
	cout << endl;
}


template<class T>
std::vector<T> Graph<T>::GetNbrs(const T& vertex) {

	std::vector<T> nbrsList; // создание списка соседей
	int vertPos = this->GetVertPos(vertex); // вычисление позиции vertex в матрице смежности

	if (vertPos != (-1)) {

		for (int i = 0, vertListSize = this->vertexList.size(); i < vertListSize; ++i) {

			if (this->adjMatrix[vertPos][i] != 0 && this->adjMatrix[i][vertPos] != 0) nbrsList.push_back(this->vertexList[i]); // вычисление соседей

		}
	}

	return nbrsList;
}



template<class T>
void Graph<T>::InsertVertex(const T& vertex) {
	if (!this->IsFull()) {
		this->vertexList.push_back(vertex);
	}

	else {
		cout << "Граф уже заполнен. Невозможно добавить новую вершину " << endl;
		return;
	}
}

template<class T>
void Graph<T>::DeleteVertex() {

	this->vertexList.pop_back();

}


template<class T>
int Graph<T>::GetAmountEdges() {
	int amount = 0; // обнуляем счетчик

	if (!this->IsEmpty()) { // проверяем, что граф не пуст

		for (int i = 0, vertListSize = this->vertexList.size();
			i < vertListSize; ++i) {

			for (int j = 0; j < vertListSize; ++j) {

				if (this->adjMatrix[i][j] ==
					this->adjMatrix[j][i] &&
					this->adjMatrix[i][j] != 0) // находим рёбра
					amount += 1; // считаем количество рёбер
			}
		}
		return (amount / 2); // приводим счетчик к корректному результату и возвращаем его
	}

	else
		return 0; // если граф пуст, возвращаем 0
}

template<class T>
int Graph<T>::GetWeight(const T& vertex1, const T& vertex2) {
	if (!this->IsEmpty()) {
		int vertPos1 = GetVertPos(vertex1);
		int vertPos2 = GetVertPos(vertex2);

		return adjMatrix[vertPos1][vertPos2];
	}

	return 0;
}

template<class T>
int Graph<T>::GetAmountVerts() {
	return this->vertexList.size();
}

template<class T>
bool Graph<T>::IsEmpty() {
	if (this->vertexList.size() != 0)
		return false;

	else
		return true;
}

template<class T>
bool Graph<T>::IsFull() {
	return (vertexList.size() == maxSize);
}

template <class T>
int Graph<T>::GetVertPos(const T& vertex) {
	for (int i = 0; i < this->vertexList.size(); ++i) {

		if (this->vertexList[i] == vertex)
			return i;
	}

	return -1;
}

template<class T>
Graph<T>::Graph() {
	for (int i = 0; i < maxSize; ++i)
	{
		for (int j = 0; j < maxSize; ++j)
		{
			this->adjMatrix[i][j] = 0;
		}
	}
}

template<class T>
Graph<T>::~Graph() {

}

Graph<int> makeGraph()
{
	Graph<int> graph; // создание графа, содержащего вершины с номерами целого типа

	int amountEdges, sourceVertex, targetVertex, edgeWeight; // создание необходимых для ввода графа переменных

	cout << "Введите количество вершин графа: "; cin >> amountVerts; cout << endl; // ввод количества рёбер графа в переменную amountVerts

	cout << "Введите количество ребер графа: "; cin >> amountEdges; cout << endl; // ввод количества рёбер графа в переменную amountEdges

	for (int i = 1; i <= amountVerts; ++i) {

		int* vertPtr = &i; // запоминаем адрес вершины с помощью указателя

		graph.InsertVertex(*vertPtr); //передаём ссылку на вершину в функцию InsertVertex; происходит вставка вершины в вектор вершин
	}

	for (int i = 0; i < amountEdges; ++i) {

		cout << "Исходная вершина: "; cin >> sourceVertex; cout << endl; // ввод исходной вершины

		int* sourceVertPtr = &sourceVertex; // запоминаем адрес исходной вершины

		cout << "Конечная вершина: "; cin >> targetVertex; cout << endl; // ввод вершины, до которой будет идти ребро от исходной вершины

		int* targetVertPtr = &targetVertex; // запоминаем адрес конечной вершины (до которой будет идти ребро от исходной вершины)

		cout << "Вес ребра: "; cin >> edgeWeight; cout << endl; // ввод числового значения веса ребра в переменную edgeWeight

		graph.InsertEdge(*sourceVertPtr, *targetVertPtr, edgeWeight); // вставка ребра весом edgeWeight между исходной и конечной вершинами
	}

	cout << endl;
	return graph;
}

template<class T>
void Graph<T>::InsertEdge(const T& vertex1, const T& vertex2, int weight) {

	if (this->GetVertPos(vertex1) != (-1) && this->GetVertPos(vertex2) != (-1)) {

		int vertPos1 = GetVertPos(vertex1);
		int vertPos2 = GetVertPos(vertex2);

		if (this->adjMatrix[vertPos1][vertPos2] != 0
			&& this->adjMatrix[vertPos2][vertPos1] != 0) {
			cout << "Ребро между вершинами уже есть" << endl;
			return;
		}

		else {
			this->adjMatrix[vertPos1][vertPos2] = weight;
			this->adjMatrix[vertPos2][vertPos1] = weight;
		}
	}

	else {
		cout << "Обеих вершин (или одной из них) нет в графе " << endl;
		return;
	}
}

template<class T>
void Graph<T>::Print() {
	if (!this->IsEmpty())
	{
		cout << "Матрица смежности графа: " << endl;

		cout << "=  ";
		for (int i = 0; i < vertexList.size(); ++i)
		{
			cout << vertexList[i] << " " << " ";
		}
		cout << endl;

		for (int i = 0; i < this->vertexList.size(); ++i)
		{
			cout << this->vertexList[i] << " ";
			for (int j = 0; j < this->vertexList.size(); ++j) {
				cout << " " << this->adjMatrix[i][j] << " ";
			}
			cout << endl;
		}
	}
	else
	{
		cout << "Граф пуст " << endl;
	}
}

void setCoord(int i, int n)
{
	int R_;
	int x0 = WinW / 2;
	int y0 = WinH / 2;

	if (WinW > WinH)
	{
		R = 5 * (WinH / 13) / n;
		R_ = WinH / 2 - R - 10;
	}

	else {
		R = 5 * (WinW / 13) / n;
		R_ = WinW / 2 - R - 10;
	}

	float theta = 2.0f * 3.1415926f * float(i) / float(n);
	float y1 = R_ * sin(theta) + y0;
	float x1 = R_ * cos(theta) + x0;

	vertC[i].x = x1;
	vertC[i].y = y1;
}

void drawCircle(int x, int y, int R) //рисуем круг в заданных координатах
{
	glColor3f(1.0, 1.0, 0.6);
	float x1, y1;
	glBegin(GL_POLYGON);

	for (int i = 0; i < 360; i++)
	{
		float theta = 2.0f * 3.1415926f * float(i) / float(360);
		y1 = R * sin(theta) + y;
		x1 = R * cos(theta) + x;;
		glVertex2f(x1, y1);
	}

	glEnd();

	glColor3f(0.0f, 0.0f, 0.0f);
	float x2, y2;
	glBegin(GL_LINE_LOOP);

	for (int i = 0; i < 360; i++)
	{
		float theta = 2.0f * 3.1415926f * float(i) / float(360);
		y2 = R * sin(theta) + y;
		x2 = R * cos(theta) + x;
		glVertex2f(x2, y2);
	}

	glEnd();
}

void drawText(int nom, int x1, int y1)
{
	GLvoid* font = GLUT_BITMAP_HELVETICA_18;
	string s = to_string(nom);
	glRasterPos2i(x1 - 5, y1 - 5);

	for (int j = 0; j < s.length(); j++) glutBitmapCharacter(font, s[j]);
}

void drawVertex(int n)
{
	for (int i = 0; i < n; i++) {
		drawCircle(vertC[i].x, vertC[i].y, R);
		drawText(i + 1, vertC[i].x, vertC[i].y);
	}
}

void drawLine(int text, int x0, int y0, int x1, int y1) //ребро неориентированный взвешенный граф
{
	glColor3f(0.0f, 0.0f, 0.0f);
	glBegin(GL_LINES);
	glVertex2i(x0, y0);
	glVertex2i(x1, y1);
	glEnd();

	drawText(text, (x0 + x1) / 2 + 10, (y0 + y1) / 2 + 10);
}


template<class T>
void Graph<T>::DrawGraph()
{
	int n = vertexList.size();

	for (int i = 0; i < n; i++)
	{
		setCoord(i, n);
	}

	for (int i = 0; i < n; i++)
	{
		for (int j = i + 1; j < n; j++)
		{
			int a = adjMatrix[i][j];

			if (a != 0)
			{
				drawLine(a, vertC[i].x, vertC[i].y, vertC[j].x, vertC[j].y);
			}
		}
	}

	drawVertex(n);
}

void reshape(int w, int h)
{
	WinW = w;
	WinH = h;

	glViewport(0, 0, (GLsizei)WinW, (GLsizei)WinH);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, (GLdouble)WinW, 0, (GLdouble)WinH);
	glutPostRedisplay();
}

void drawMenuText(string text, int x1, int y1)
{
	GLvoid* font = GLUT_BITMAP_HELVETICA_18;
	string s = text;
	glRasterPos2i(x1 + 5, y1 - 20);

	for (int j = 0; j < s.length(); j++) glutBitmapCharacter(font, s[j]);
}

void drawMenu()
{
	int shift = 60;
	int height = WinH;


	glBegin(GL_LINE_LOOP);
	glVertex2i(shift - 10, height - 90);
	glVertex2i(shift + 145, height - 90);
	glVertex2i(shift + 145, height - 580);
	glVertex2i(shift - 10, height - 580);
	glEnd();

	glColor3d(1.0, 1.0, 0.6);
	glBegin(GL_QUADS);
	glVertex2i(shift, height - shift - 100);
	glVertex2i(shift + 135, height - shift - 100);
	glVertex2i(shift + 135, height - shift - 50);
	glVertex2i(shift, height - shift - 50);
	glEnd();
	glColor3d(0.0, 0.0, 0.0);
	drawMenuText("Insert element", shift, height - shift - 62);

	glColor3d(1.0, 1.0, 0.6);
	glBegin(GL_QUADS);
	glVertex2i(shift, height - shift - 200);
	glVertex2i(shift + 135, height - shift - 200);
	glVertex2i(shift + 135, height - shift - 150);
	glVertex2i(shift, height - shift - 150);
	glEnd();
	glColor3d(0.0, 0.0, 0.0);
	drawMenuText("Delete element", shift, height - shift - 162);

	glColor3d(1.0, 1.0, 0.6);
	glBegin(GL_QUADS);
	glVertex2i(shift, height - shift - 300);
	glVertex2i(shift + 135, height - shift - 300);
	glVertex2i(shift + 135, height - shift - 250);
	glVertex2i(shift, height - shift - 250);
	glEnd();
	glColor3d(0.0, 0.0, 0.0);
	drawMenuText("Print matrix", shift, height - shift - 262);

	glColor3d(1.0, 1.0, 0.6);
	glBegin(GL_QUADS);
	glVertex2i(shift, height - shift - 400);
	glVertex2i(shift + 135, height - shift - 400);
	glVertex2i(shift + 135, height - shift - 350);
	glVertex2i(shift, height - shift - 350);
	glEnd();
	glColor3d(0.0, 0.0, 0.0);
	drawMenuText("Answer", shift, height - shift - 362);

	glColor3d(1.0, 1.0, 0.6);
	glBegin(GL_QUADS);
	glVertex2i(shift, height - shift - 500);
	glVertex2i(shift + 135, height - shift - 500);
	glVertex2i(shift + 135, height - shift - 450);
	glVertex2i(shift, height - shift - 450);
	glEnd();
	glColor3d(0.0, 0.0, 0.0);
	drawMenuText("New matrix", shift, height - shift - 462);
}

void mouseClick(int btn, int stat, int x, int y) {

	int shift = 60;
	int height = WinH;
	int height1 = WinH;


	if (stat == GLUT_DOWN) {

		if (x > shift && x < shift + 135 && y > height - height1 + shift + 50  && y < height - height1 + shift + 100)
		{
			int sourceVertex;
			int targetVertex;
			int edgeWeight;

			cout << "Исходная вершина: "; cin >> sourceVertex; cout << endl;
			int* sourceVertPtr = &sourceVertex;
			cout << "Конечная вершина: "; cin >> targetVertex; cout << endl;
			int* targetVertPtr = &targetVertex;

			if (sourceVertex > amountVerts || targetVertex > amountVerts) {
				amountVerts++;
				int* vertPtr = &amountVerts;
				graph.InsertVertex(*vertPtr);
			}

			cout << "Вес ребра: "; cin >> edgeWeight; cout << endl;
			graph.InsertEdge(*sourceVertPtr, *targetVertPtr, edgeWeight); // 
		}

		if (x > shift && x < shift + 135 && y > height - height1 + shift + 150 && y < height - height1 + shift + 200)
		{
			int sourceVertex;
			int targetVertex;
			int edgeWeight;

			cout << "Введите вершину: "; cin >> sourceVertex; cout << endl;
			int* sourceVertPtr = &sourceVertex;

			if (sourceVertex == amountVerts)
			{
				amountVerts--;
				graph.DeleteVertex();
			}

			else cout << "Невозможно удалить вершину \n";
		}

		if (x > shift && x < shift + 135 && y > height - height1 + shift + 250 && y < height - height1 + shift + 300)
		{
			graph.Print();
		}

		if (x > shift && x < shift + 135 && y > height - height1 + shift + 350 && y < height - height1 + shift + 400)
		{
			salesman(mat, n, help, result);
		}

		if (x > shift && x < shift + 135 && y > height - height1 + shift + 450 && y < height - height1 + shift + 500)
		{
			graph = makeGraph();
		}

	}

	glutPostRedisplay();
}

void display()
{
	glShadeModel(GL_SMOOTH);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, WinW, 0, WinH); //ставим начало координат в левый нижний угол
	glViewport(0, 0, WinW, WinH);
	glClearColor(1.0, 0.5, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);
	graph.DrawGraph();
	drawMenu();
	glutSwapBuffers();
}
