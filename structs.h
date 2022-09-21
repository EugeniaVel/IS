#ifndef LAB1V2_STRUCTS_H
#define LAB1V2_STRUCTS_H
#include <vector>
#include <iostream>
#include <list>
#define OBSTACLE 1
#define TRAVERSABLE 0
#define PATH 2

struct Node // структура узел
{
    int i; // координата точки
    int j; // координата точка
    double g; // стоимость пути до данной точки
    double h; // предположительная стоимость пути до финиша
    double f; // стоимость пути до точки + предположительная стоимость до финиша
    const Node* parent; // родитель(предыдущая клетка?)
    Node(int _i=-1, int _j=-1, double _g=0, double _h=0, Node* _parent=nullptr):i(_i), j(_j), g(_g), h(_h), f(_h+_g), parent(_parent){}
    bool operator ==(const Node& other) const //перегрузка оператора ==
    {
        return this->i == other.i && this->j == other.j; //возвращаем равны ли координаты узла
    }
    bool operator <(const Node& other) const // перегрузка оператора меньше
    {
        return this->i < other.i || (this->i == other.i && this->j < other.j);
    }
};

struct Result
{
    std::list<Node> path; // путь
    double cost; // стоимость пути
    double runtime; // время работы алгоритма
    int steps; // число итераций работы алгоритма
    int nodes_created; // число созданных вершин
};

struct Map //структура карты
{
    int width; //ширина
    int height; //высота
    std::vector<std::vector<int>> grid; //создаём вектор грид(сетка) из целых чисел
    Map(int _width=0, int _height=0):width(_width), height(_height) //вводим длину и ширину
    {
        grid = std::vector<std::vector<int>>(height, std::vector<int>(width, TRAVERSABLE)); //каждая клетка равна 0, все клетки проходимые
    }
    void add_obstacle(int i, int j) // добавляем препятствие
    {
        grid[i][j] = OBSTACLE;
    }
    bool cell_on_grid(int i, int j) // проверяем, есть ли на карте эта клетка
    {
        return (i >= 0 && j >=0 && i < height && j < width);
    }
    bool cell_is_obstacle(int i, int j) // проверяем, является ли клетка препятствием
    {
        return grid[i][j] == OBSTACLE; // препятствия равны единице
    }
    std::vector<Node> get_neighbors(Node s, int connections=8) // получаем соседей
    {
        //TODO - implement support of diagonal moves, добавить движение по диагоналям
        std::vector<std::pair<int, int>> deltas = {{0,1}, {1,0}, {-1,0}, {0,-1}};
        std::vector<Node> neighbors; //создаём вектор соседи из узлов
        for(auto d:deltas) // д - дельты
        {
            Node n(s.i + d.first, s.j + d.second); //создаём узел н с координатами соседей
            if(cell_on_grid(n.i, n.j) && !cell_is_obstacle(n.i, n.j)) // если клетка есть на карте и она не препятствие
                neighbors.push_back(n); // добавляем узел в конец вектора
        }
        if(connections==8) {
            Node n(s.i + 1, s.j + 1);
            if (cell_on_grid(n.i, n.j) && !cell_is_obstacle(n.i, n.j) && !cell_is_obstacle(n.i - 1, n.j) &&
                !cell_is_obstacle(n.i, n.j - 1))
                neighbors.push_back(n);
            Node o(s.i + 1, s.j - 1);
            if (cell_on_grid(o.i, o.j) && !cell_is_obstacle(o.i, o.j) && !cell_is_obstacle(o.i, o.j + 1) &&
                !cell_is_obstacle(o.i - 1, o.j))
                neighbors.push_back(o);
            Node p(s.i - 1, s.j + 1);
            if (cell_on_grid(p.i, p.j) && !cell_is_obstacle(p.i, p.j) && !cell_is_obstacle(p.i + 1, p.j) &&
                !cell_is_obstacle(p.i, p.j - 1))
                neighbors.push_back(p);
            Node r(s.i - 1, s.j - 1);
            if (cell_on_grid(r.i, r.j) && !cell_is_obstacle(r.i, r.j) && !cell_is_obstacle(r.i + 1, r.j) &&
                !cell_is_obstacle(r.i, r.j + 1))
                neighbors.push_back(r);
        }
        return neighbors; // возвращаем вектор соседей
    }
    void print(std::list<Node> path={}) //выводим карту и путь на экран
    {
        for(auto n:path) //н-путь
            grid[n.i][n.j] = PATH; 
        for(int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++)
                if (grid[i][j] == OBSTACLE)//если клетка - препятствие, ставим решётку
                    std::cout << "# ";
                else if (grid[i][j] == TRAVERSABLE)//если проходимая клетка, то точечку
                    std::cout << ". ";
                else //в других случаях собачку ставим(это путь, наверное)
                    std::cout << "@ ";
            std::cout<<std::endl;
        }
        for(auto n:path) //н-путь
            grid[n.i][n.j] = TRAVERSABLE; 
    }
};
#endif //LAB1V2_STRUCTS_H
