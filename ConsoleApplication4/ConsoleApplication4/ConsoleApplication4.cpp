// ConsoleApplication4.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <cstdio>
#include <fstream>
#include <queue>
#include <stack>
#include <map>

#include <windows.h>
#include <wincodec.h>
#include <gdiplus.h>


class gameMap {
private:
	std::vector<std::vector<bool>> map;
	int size;
public:
	int getSize() {
		return this->size;
	}
	std::vector<std::vector<bool>> getMap() {
		return this->map;
	}
	void getSizeFromFile(std::ifstream& file) {
		if (!file.is_open())
			throw std::exception();
		std::string str = "";
		std::string buf;
		while (std::getline(file, buf, '\n'))
			str = buf;
		this->size = str.length();
	}
	gameMap(int n) {
		this->size = n;
		for (int i = 0; i < this->size; i++) {
			map.push_back(std::vector<bool>());
			for (int j = 0; j < this->size; j++) {
				map[i].push_back(false);
				map[i][j] = false;

			}
		}
	}
	gameMap(std::ifstream& file) {
		//this->getSizeFromFile(file);
		this->size = 10;
		while (!file.eof()) {
			/*for (int i = 0; i < this->size; i++) {
				std::vector<bool> tmp;
				for (int j = 0; j < this->size; j++) {
					bool l;
					file.read((char*)&l, sizeof(l));
					tmp.push_back(l);
				}
				this->map.push_back(tmp);
			}*/
			this->map.push_back(std::vector<bool>(this->size));
			std::copy_n(std::istream_iterator<int>(file), this->size, this->map.back().begin());
		}
	}
	void printMapOnConsol() {
		for (int i = 0; i < this->size; i++) {
			for (int j = 0; j < this->size; j++) {
				std::cout << this->map[i][j] << " ";
			}
			std::cout << std::endl;
		}
	}
};

struct point {

	int vert;
	int parentVert;
	double dist;
	double heuristic;
	bool visited;
	void print() {
		std::cout << "vertex: " << vert << std::endl;
		std::cout << "parent: " << parentVert << std::endl;
		std::cout << "dist to parent = " << dist << std::endl;
		std::cout << "heuristic dist = " << heuristic << std::endl;
		std::cout << "|====================|" << std::endl;
	}
};
std::pair<int, int> findPos(int size, int vert) {
	int str = 0;
	int stlb = 0;
	if ((vert / size) * size == vert)
		str = vert / size - 1;
	else
		str = vert / size;
	if ((vert % size) == 0)
		stlb = size - 1;
	else
		stlb = vert % size - 1;
	std::pair<int, int> pos;
	pos.first = str;
	pos.second = stlb;
	return pos;
}
int findVertex(int i, int j, int size) {
	int currI = 0;
	int currJ = 0;
	int vert = 0;
	if (i == 0)
		vert = j + 1;
	else {
		while (currI < i) {
			vert = vert + size;
			currI++;
		}
		if (j == (size - 1))
			vert = vert + size;
		else
			while (currJ <= j) {
				vert = vert + 1;
				currJ++;
			}
	}
	return vert;
}
std::vector<point> convert_path(std::vector < std::vector<point>>& g, int start, int finish) {
	std::vector<point> result;
	std::stack<point> tmp;
	point currVert;
	std::pair<int, int> pos = findPos(g.size(), finish);
	currVert.vert = finish;
	currVert.parentVert = g[pos.first][pos.second].parentVert;
	while (currVert.parentVert != -1) {
		tmp.push(currVert);
		std::pair<int, int> currP = findPos(g.size(), currVert.vert);
		currVert.parentVert = g[currP.first][currP.second].vert;
	}
	while (!tmp.empty()) {
		result.push_back(tmp.top());
		tmp.pop();
	}
	return result;
}
int manhettenDist(int size, int vert, int finish) {  // возвращал int
	std::pair<int, int> finishPos = findPos(size, finish);
	std::pair<int, int> vertPos = findPos(size, vert);
	int num = 0;
	if (finishPos.first == vertPos.first)
		goto next;
	if (finishPos.first<vertPos.first)
		while (finishPos.first < vertPos.first) {
			num++;
			vertPos.first--;
		}
	else 
		while (finishPos.first > vertPos.first) {
			num++;
			vertPos.first++;
		}
next:
	if (finishPos.second == vertPos.second)
		goto ret;
	if (finishPos.second<vertPos.second)
		while (finishPos.second < vertPos.second) {
			num++;
			vertPos.second--;
		}
	else
		while (finishPos.second > vertPos.second) {
			num++;
			vertPos.second++;
		}
ret:
	return (num * 10);
}

std::pair<int, std::vector<int>> manhettenDistForOne(int size, int vert, int finish) {
	std::pair<int, int> finishPos = findPos(size, finish);
	std::pair<int, int> vertPos = findPos(size, vert);
	std::vector<int> path;  //
	int num = 0;
	if (finishPos.first == vertPos.first)
		goto next;
	if (finishPos.first < vertPos.first)
		while (finishPos.first < vertPos.first) {
			path.push_back(findVertex(vertPos.first, vertPos.second, size));  //
			num++;
			vertPos.first--;
		}
	else
		while (finishPos.first > vertPos.first) {
			path.push_back(findVertex(vertPos.first, vertPos.second, size));  //
			num++;
			vertPos.first++;
		}
next:
	if (finishPos.second == vertPos.second)
		goto ret;
	if (finishPos.second < vertPos.second)
		while (finishPos.second < vertPos.second) {
			path.push_back(findVertex(vertPos.first, vertPos.second, size));  //
			num++;
			vertPos.second--;
		}
	else
		while (finishPos.second > vertPos.second) {
			path.push_back(findVertex(vertPos.first, vertPos.second, size));  // 
			num++;
			vertPos.second++;
		}
ret:
	path.push_back(finish);
	std::pair<int, std::vector<int>> ret;
	ret.first = num * 10;
	ret.second = path; 
	return ret;  
}
class Graph_Map {
private:
	int size;
	std::vector<std::vector<int>> mainMap;
public:
	Graph_Map() {
		this->size = 0;
	}
	Graph_Map(int n) {
		this->size = n;
		for (int i = 0; i < this->size; i++) {
			mainMap.push_back(std::vector<int>());
			for (int j = 0; j < this->size; j++) {
				mainMap[i].push_back(0);
				mainMap[i][j] = 0;
			}
		}
	}
	/*Graph_Map(файловая переменная) {

	}*/
	Graph_Map(std::vector<std::vector<int>> M) {
		this->size = M.size();
		this->mainMap = M;
	}
	int& operator ()(int x,int y) {
		return mainMap[x][y];
	}
	int returnSize() {
		return this->size;
	}
	std::vector<std::vector<int>> returnMap() {
		return this->mainMap;
	}
	
	std::vector<point> findNeighbors(int vertex) {
		std::pair<int, int> pos = findPos(this->size, vertex);
		int* masI = new int[8];
		int* masJ = new int[8];
		masI[0] = pos.first - 1;
		masI[1] = pos.first - 1;
		masI[2] = pos.first - 1;
		masI[3] = pos.first;
		masI[4] = pos.first;
		masI[5] = pos.first + 1;
		masI[6] = pos.first + 1;
		masI[7] = pos.first + 1;

		masJ[0] = pos.second - 1;
		masJ[1] = pos.second;
		masJ[2] = pos.second + 1;
		masJ[3] = pos.second - 1;
		masJ[4] = pos.second + 1;
		masJ[5] = pos.second - 1;
		masJ[6] = pos.second;
		masJ[7] = pos.second + 1;

		std::vector<point> neighbors;
		for (int i = 0; i < 8; i++) {
			if (masI[i] < 0 || masI[i] == this->size || masJ[i] < 0 || masJ[i] == this->size)
				continue;
			if (mainMap[masI[i]][masJ[i]] == 0)  // клетка - препядствие не помещается в вектор соседей
				continue;
			else {
				point curr;
				curr.vert = findVertex(masI[i], masJ[i], this->size);
				curr.parentVert = vertex;
				neighbors.push_back(curr);
			}
		}
		// подсчет расстояний в массиве соседей
		for (int i = 0; i < neighbors.size(); i++) {
			std::pair<int, int> currNeighborPos = findPos(this->size, neighbors[i].vert);
			if ((currNeighborPos.first == pos.first) || (currNeighborPos.second == pos.second)) {
				neighbors[i].dist = 10;
			}
			else
				neighbors[i].dist = std::sqrt(200);
		}
		delete[] masI;
		delete[] masJ;
		return neighbors;
	}
	// конвертация из карты в карту point (используется в findPath_first)
	std::vector<std::vector<point>> convert() {
		std::vector<std::vector<point>> g;
		point zerop;
		zerop.vert = 0;
		zerop.dist = 0;
		zerop.parentVert = 0;
		for (int i = 0; i < this->size; i++) {
			g.push_back(std::vector<point>());
			for (int j = 0; j < this->size; j++) {
				point tmp;
				tmp.vert = findVertex(i, j, this->size);
				tmp.dist = 10;
				tmp.parentVert = -1;
				tmp.visited = false;
				g[i].push_back(zerop);
				g[i][j]=tmp;
			}
		}
		return g;
	}
	//  расстояние от стартовой вершины до каждой другой (только под прямыми углами)
	std::vector<int> newDistanseForEach(int start) {
		std::vector<int> dist;
		std::pair<int, int> pos = findPos(this->size, start);
		if (mainMap[pos.first][pos.second] == 0) {
			//dist.push_back(-1);
			std::cout << "ERROR: start point on wall!" << std::endl;
			abort();
		}
		
		for (int i = 0; i < this->size; i++) {
			for (int j = 0; j < this->size; j++) {
				if (mainMap[i][j] == 0)
					dist.push_back(-1);//-1
				else {
					int currVert = findVertex(i, j, this->size);
					int currDist = manhettenDist(this->size, start, currVert);
					dist.push_back(currDist);
				}	
			}
		}
		return dist;
	}
	// если клетка не проходима, то не из нее не в нее нельзя ==> больжен быть вектор -1, + мб сделать главную диагональ всю -1
	std::vector<std::vector<int>> matrixConvert() {
		std::vector<std::vector<int>> matrix;
		std::vector<int> possibil;
		for (int i = 0; i < this->size; i++) {
			for (int j = 0; j < this->size; j++) {
				if (this->mainMap[i][j] == 0)
					possibil.push_back(0);
				else
					possibil.push_back(1);
			}
		}
		for (int i = 1; i <= this->size * this->size; i++) {
			if (possibil[i - 1] == 1) {
				std::vector<int> tmp = newDistanseForEach(i);
				matrix.push_back(tmp);
			}
			else {
				std::vector<int> neg(this->size * this->size, -1);//-1
				matrix.push_back(neg);
			}
		}
		for (int i = 0; i < this->size* this->size; i++) {
			for (int j = 0; j < this->size* this->size; j++) {
				if (i != j)
					continue;
				else
					matrix[i][j] = -1;//-1
			}
		}

		return matrix;
	}
	std::vector<int> findPathDijkstra(int start, int finish) {
		std::vector<std::vector<int>> matrix = this->matrixConvert();
		std::vector< std::vector <std::pair <int, int>>> matrixPair;
		std::vector<int> parent(this->size * this->size);
		int* dist = new int[this->size * this->size];
		int* used = new int[this->size * this->size];
		for (int i = 0; i < this->size * this->size; i++)
		{
			dist[i] = INT_MAX;
			used[i] = 0;
		}
		dist[start] = 0;
		for (int i = 0; i < this->size * this->size; i++)
		{
			int c = INT_MAX, min = 0, v = -1;
			for (int j = 0; j < this->size * this->size; j++)
			{
				if ((dist[j] < c) && (!used[j]))
				{
					min = j;
					c = dist[j];
					v = j;
				}
			}
			for (int j = 0; j < this->size * this->size; j++)
			{
				if (matrix[min][j] != -1)
					if ((dist[j] > matrix[min][j] + c) && !used[j]) {
						dist[j] = matrix[min][j] + c;
						parent[j] = v;
					}
						
			}
			used[min] = 1;
		}
		//восстановление пути
		std::vector<int> path;
		for (int i = finish; i != start; i = parent[i])
			path.push_back(i);
		path.push_back(start);
		reverse(path.begin(), path.end());

		std::vector<int> res;
		for (int i = 0; i < this->size * this->size; i++)
			res.push_back(dist[i]);
		return res;

		/*for (int i = 0; i < matrix.size(); i++) {
			matrixPair.push_back(std::vector< std::pair<int, int>>());
			for (int j = 0; j < matrix.size(); j++) {
				matrixPair[i].push_back(std::make_pair(0, 0));
				matrixPair[i][j].second = matrix[i][j];
				matrixPair[i][j].first = j;
			}
		}

		std::vector<int> d(matrixPair.size(), INT_MAX), p(matrixPair.size());
		d[start] = 0;
		std::priority_queue< std::pair <int, int> > q;
		q.push(std::make_pair(0, start));
		while (!q.empty())
		{
			int v = q.top().second, cur_d = -q.top().first;
			q.pop();
			if (cur_d > d[v]) continue;
			for (size_t j = 0; j < matrixPair[v].size(); ++j)
			{
				int to = matrixPair[v][j].first, len = matrixPair[v][j].second;
				if (d[v] + len < d[to])
				{
					d[to] = d[v] + len;
					p[to] = v;
					q.push(std::make_pair(-d[to], to));
				}
			}
		}
		return d;*/
	}
	std::vector<point> findPath(int start, int finish) { // заготовка для A*
		std::vector<point> openList;
		std::vector<point> closedList;
		std::vector<point> path;
		point activ;
		activ.vert = start;
		activ.parentVert = -1;
		openList = findNeighbors(start);
		closedList.push_back(activ);
		for (int i = 0; i < openList.size(); i++) {
			//int currNum = manhettenDist(this->size, openList[i].vert, finish);
			//openList[i].heuristic = currNum;
		}
		int max = -1;
		int vertMax;
		for (int i = 0; i < openList.size(); i++) {
			if ((openList[i].dist + openList[i].heuristic) > max) {
				max = openList[i].dist + openList[i].heuristic;
				vertMax = openList[i].vert;
			}
		}
		path.push_back(openList[vertMax]); // алгоритм дейкстры от одного к другому
		activ = openList[vertMax];
		return openList;
	}
	void printMapOnConsol() {
		for (int i = 0; i < this->size; i++) {
			for (int j = 0; j < this->size; j++) {
				std::cout << mainMap[i][j] << " ";
			}
			std::cout << std::endl;
		}
	}
};

// class of one unit (each vertex of dynamic graph)
class UnitOld {
private:
	double x;
	double y;
	double speed;
	double turningSpeed;
public:
	/*Unit() {
		this->x = 0.0;
		this->y = 0.0;
		this->speed = 0.0;
		this->turningSpeed = 0.0;
	}*/
	UnitOld(int a = 0) {
		this->x = 0.0;
		this->y = 0.0;
		this->speed = 0.0;
		this->turningSpeed = 0.0;
	}
	UnitOld(double x, double y, double speed,double turningS) {
		this->x = x;
		this->y = y;
		this->speed = speed;
		this->turningSpeed = turningS;
	}
	void setPosition(double x, double y) {
		this->x = x;
		this->y = y;
	}
	void setSpeed(double speed) {
		this->speed = speed;
	}
	void setTurningSpeed(double turningS) {
		this->turningSpeed = turningS;
	}
	std::pair<double, double> giveExectlyPosition() {
		std::pair<double, double> pos;
		pos.first = this->x;
		pos.second = this->y;
		return pos;
	}
	std::pair<int, int> givePositionOnAllMap() {
		std::pair<int, int> pos;
		pos.first = (int)this->x;
		pos.second = (int)this->y;
		return pos;
	}
	double giveSpeed() {
		return speed;
	}
	double giveTurningSpeed() {
		return turningSpeed;
	}
	void printUnitDataOnConsol() {
		std::cout << "Position: " << std::endl << "x = " << this->x << "; y = " << this->y << std::endl;
		std::cout << "Speed = " << this->speed << std::endl;
		std::cout << "Turning speed = " << this->turningSpeed << std::endl;
	}
};
// main unit located in 0[] 
// other vector elements - field cells next to the main unit [0]
// and enemies / friends (may not be nearby)
class DynamicGraph {
private:
	std::vector<UnitOld> currentUnit;
	int size;
public:
	DynamicGraph() {
		UnitOld T(0);
		this->size = 10; // only 3x3 arround current
		for (int i = 0; i < this->size; i++) {
			this->currentUnit.push_back(T);
		}
	}
	// первый элемент вектора - текущий юнит, вокруг которого сканируется территория
	DynamicGraph(int n, UnitOld U) {
		UnitOld T(0); //  cells around	current Unit [0]
		this->size = n;
		this->currentUnit.push_back(U);
		this->currentUnit.resize(this->size);
		for (int i = 0; i < this->size; i++) {
			this->currentUnit.push_back(T);
		}
	}
	int returnSize() {
		return this->size;
	}
	void insertVertex(UnitOld U) {
		this->size++;
		this->currentUnit.push_back(U);
	}
	// конвертировать элемент карты в юнита, для последующего добавления в граф
	UnitOld convertInUnit(int x, int y) {
		UnitOld U(x, y, 0, 0);
		return U;
	}
	void generateModul(Graph_Map map, UnitOld U) {
		this->currentUnit.push_back(U);
		int mapSize = map.returnSize();
		std::pair<int, int> pos = U.givePositionOnAllMap();
		std::pair<int, int> corner_1;
		corner_1.first = pos.first - 5;
		if (corner_1.first < 0)
			corner_1.first = 0;
		corner_1.second = pos.second + 5;
		if (corner_1.second > mapSize)
			corner_1.second = mapSize;
		for (int i = corner_1.second; i < 10; i++) {
			for (int j = corner_1.first; j < 10; j++) {
				UnitOld tmp = convertInUnit(corner_1.first, corner_1.second);
				this->currentUnit.push_back(tmp);
			}
		}

	}
	void printGraphAboutCurrentUnit() {
		std::cout << std::endl;
		for (int i = 0; i < this->size; i++) {
			std::cout << "Unit namber " << i << std::endl;
			this->currentUnit[i].printUnitDataOnConsol();
			std::cout << "==========" << std::endl;
		}
	}

};

void testFunc(int size, int vert) {
	int str = 0;
	int stlb = 0;
	if ((vert / size) * size == vert)
		str = vert / size - 1;
	else
		str = vert / size;
	if ((vert % size) == 0)
		stlb = size - 1;
	else
		stlb = vert % size - 1;

	std::cout << "vert = " << vert << std::endl;
	std::cout << "size = " << size << std::endl;
	std::cout << "pos: (" << str << ", " << stlb << ")" << std::endl;
}
void testFunc_2(int i, int j, int size) {
	int currI = 0;
	int currJ = 0;
	int vert = 0;
	if (i == 0)
		vert = j + 1;
	else {
		while (currI < i) {
			vert = vert + size;
			currI++;
		}
		if (j == (size - 1))
			vert = vert + size;
		else
			while (currJ <= j) {
				vert = vert + 1;
				currJ++;
			}
	}
	std::cout << "pos: (" << i << ", " << j << ")" << std::endl;
	std::cout << "size = " << size << std::endl;
	std::cout << "vert = " << vert << std::endl;
}

Graph_Map read_form_image(const TCHAR *file) {

	std::vector<std::vector<int>> tmpG;

	Gdiplus::GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR gdiplusToken;
	Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
	{
		Gdiplus::Bitmap input_image(file);
		auto width = input_image.GetWidth();
		auto height = input_image.GetHeight();

		tmpG.resize(width); // square images
		for (int i = 0; i < height; i++)
			tmpG[i].resize(height);
		for (int i = 0; i < width; i++) {
			for (int j = 0; j < height; j++) {
				Gdiplus::Color currentColor;
				auto status = input_image.GetPixel(i, j, &currentColor);
				if (currentColor.GetValue() == Gdiplus::Color::White)
					tmpG[j][i] = 1;
				else
					tmpG[j][i] = 0;
			}
		}
	}
	Gdiplus::GdiplusShutdown(gdiplusToken);
	Graph_Map graph(tmpG);

	return graph;
}
int main()
{
	auto graphImage = read_form_image(TEXT("image1.bmp"));
	graphImage.printMapOnConsol();
	std::vector <int> dist;
	dist = graphImage.newDistanseForEach(1);
	for (int i = 0; i < dist.size(); i++)
		std::cout << dist[i] << " ";

	std::cout << std::endl;
	std::cout << std::endl;
	std::cout << "TEST MATRIX:" << std::endl << std::endl;
	std::vector<std::vector<int>> matrix = graphImage.matrixConvert();
	for (int i = 0; i < matrix.size(); i++) {
		for (int j = 0; j < matrix.size(); j++) {
			std::cout << matrix[i][j] << " ";
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
	std::cout << std::endl;
	std::cout << std::endl;
	std::vector<int> distt;
	distt = graphImage.findPathDijkstra(0,24);
	for (int i = 0; i < distt.size(); i++)
		std::cout << distt[i] << " ";
	/*std::pair<int, std::vector<int>> t;
	t = manhettenDistForOne(5, 13, 20);
	std::cout << t.first << std::endl;
	std::cout << "path: ";
	for (int i = 0; i < t.second.size(); i++)
		std::cout << t.second[i] << " ";
	std::vector<std::vector<int>> map;
	for (int i = 0; i < 5; i++) {
		map.push_back(std::vector<int>());
		for (int j = 0; j < 5; j++) {
			map[i].push_back(1);
		}
	}

	Graph_Map testMap(map);
	testMap.printMapOnConsol();
	std::vector<std::vector<point>> test;
	test = testMap.convert();
	for (int i = 0; i < test.size(); i++) {
		for (int j = 0; j < test.size(); j++) {
			test[i][j].print();
		}
		std::cout << std::endl;
	}

	std::pair<int, std::vector<int>> t;
	t = manhettenDistForOne(5, 13, 20);
	std::cout << t.first << std::endl;
	std::cout << "path: ";
	for (int i = 0; i < t.second.size(); i++) {
		std::cout << t.second[i] << " ";
	}*/
	

	return 0;
}

// Запуск программы: CTRL+F5 или меню "Отладка" > "Запуск без отладки"
// Отладка программы: F5 или меню "Отладка" > "Запустить отладку"

// Советы по началу работы 
//   1. В окне обозревателя решений можно добавлять файлы и управлять ими.
//   2. В окне Team Explorer можно подключиться к системе управления версиями.
//   3. В окне "Выходные данные" можно просматривать выходные данные сборки и другие сообщения.
//   4. В окне "Список ошибок" можно просматривать ошибки.
//   5. Последовательно выберите пункты меню "Проект" > "Добавить новый элемент", чтобы создать файлы кода, или "Проект" > "Добавить существующий элемент", чтобы добавить в проект существующие файлы кода.
//   6. Чтобы снова открыть этот проект позже, выберите пункты меню "Файл" > "Открыть" > "Проект" и выберите SLN-файл.
