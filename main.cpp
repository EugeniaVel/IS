#include <iostream>
#include <list>
#include <set>
#include "structs.h"
#include "utils.h"
#include <chrono>
#include <cmath> // для квадратного корня в нахождении эвристики
#include <algorithm>

class BFS //breadth-first-search
{
public:
    Result find_path(Node start, Node goal, Map grid) // находим путь, введя начальный узел, конечный узел и сетку карты
    {
        auto time_now = std::chrono::high_resolution_clock::now();
        Result result; // переменная типа результат
        int steps = 0; // итерации равны нулю
        start.g = 0; //джи - стоимость пути до данной точки = 0
        std::list<Node> OPEN; // создаём список открытых узлов
        OPEN.push_back(start); // добавляем начало в конец списка
        std::set<Node> CLOSED; // создаём контейнер для узлов, которые уже просмотрели
        CLOSED.insert(start); // добавляем начало в контейнер рассмотренных вершин
        bool pathfound = false; // путь не найден
        while(!OPEN.empty() && !pathfound) // пока в нерассмотренных узлах не пусто и путь не найден
        {
           Node current = OPEN.front(); //текущий узел - первый узел в раскрытых
           OPEN.pop_front(); //удаляем первый узел в контейнере(?) оупен
           steps++; // добавляем количество итераций
           auto neighbors = grid.get_neighbors(current); //автоматически определяем тип переменной соседей, ищем соседей
           for(auto n:neighbors) { //н - это соседи
               if (CLOSED.find(n) == CLOSED.end()) // если найденные соседи в конце закрытого контейнера
               {
                   n.g = current.g + 1; // увеличиваем текущую стоимость пути
                   n.parent = &(*CLOSED.find(current)); // родителя соседа добавляем в закрытый список
                   OPEN.push_back(n); // добавляем соседа в конец открытого списка
                   CLOSED.insert(n); // добавляем соседа в закрытый список
                   if(n == goal) { // если сосед - финиш
                       result.path = reconstruct_path(n); // итоговый путь такой же, как путь соседа
                       result.cost = n.g; // стоимость пути - это джи соседа
                       pathfound = true; // путь найден, йоу
                       break;
                    }
                }
            }
        }
        result.steps = steps; //итерации
        result.nodes_created = CLOSED.size(); // сколько вершин мы прошли и какие(для печати карты наверное)
        result.runtime = (std::chrono::high_resolution_clock::now() - time_now).count()/1e+9; // время работы(даже смотреть не хочу, что в этой строке написано, страшно)
        return result; // получили результат юху
    }
    std::list<Node> reconstruct_path(Node n) // вспоминаем путь к соседу
    {
        std::list<Node>path; // путь списка узел
        while(n.parent != nullptr) // пока у соседа есть родитель
        {
            path.push_front(n); // добавляем каждого соседа в начало пути
            n = *n.parent; // сосед - указатель на родителя соседа
        }
        path.push_front(n); // добавляем соседа без родителя в начало пути
        return path; // возвращаем путь
    }
};

class AStar //класс азвезда, алгоритм делать будем
{
public:
    Result find_path(Node start, Node goal, Map grid, std::string metrictype="Octile", int connections=8, double hweight=1) //добавляем результат
    {
        //TODO - implement the main cycle of AStar algorithm
        auto time_now = std::chrono::high_resolution_clock::now();
        Result result; // переменная типа результат
        int steps = 0; // итерации равны нулю
        start.g = 0; //джи - стоимость пути до данной точки = 0
        start.h=count_h_value(start, goal,metrictype); // считаем эвристику
        start.f = start.g + hweight * start.h;
        std::list<Node> OPEN; // создаём список открытых узлов
        OPEN.push_back(start); // добавляем начало в конец списка
        std::set<Node> CLOSED; // создаём контейнер для узлов, которые уже просмотрели
        CLOSED.insert(start); // добавляем начало в контейнер рассмотренных вершин
        bool pathfound = false; // путь не найден
        while (!OPEN.empty() && !pathfound) // пока в нерассмотренных узлах не пусто и путь не найден
        {
            OPEN.sort([](const Node & a, const Node &b){
                if(a.f==b.f)
                    return a < b;
                return a.f < b.f;
            });
            Node current = OPEN.front(); //текущий узел - первый узел в раскрытых
            OPEN.pop_front(); //удаляем первый узел в контейнере(?) оупен
            steps++; // добавляем количество итераций
            auto neighbors = grid.get_neighbors(current,connections); //автоматически определяем тип переменной соседей, ищем соседей
            for (auto n: neighbors) { //н - это соседи
                bool found = (CLOSED.find(n) != CLOSED.end());
                Node fn = *(CLOSED.find(n));
                n.h = count_h_value(n, goal,metrictype); // считаем эвристику соседа
                n.g = current.g + 1; // считаем стоимость пути до соседа
                n.f = n.g + hweight * n.h; // считаем стоимость всего пути соседа
                if (!found || n.g < fn.g) //если мы не были в этой вершине или путь до неё меньше
                {
                    OPEN.push_back(n); // добавляем соседа в конец открытого списка
                    n.parent = &(*CLOSED.find(current)); //соседский родитель равен current
                    CLOSED.erase(fn);
                    CLOSED.insert(n); // добавляем соседа в закрытый список
                }
                if (n == goal) { // если сосед - финиш
                    result.path = reconstruct_path(n); // итоговый путь такой же, как путь соседа
                    result.cost = n.g; // стоимость пути - это джи соседа;
                    pathfound = true; // путь найден, йоу
                    break;
                }
            }
        }
        result.steps = steps; //итерации
        result.nodes_created = CLOSED.size(); // сколько вершин мы прошли и какие(для печати карты наверное)
        result.runtime = (std::chrono::high_resolution_clock::now() - time_now).count()/1e+9; // время работы(даже смотреть не хочу, что в этой строке написано, страшно)
        return result;
    }
    double count_h_value(Node current, Node goal, std::string metrictype="Octile") //
    {
        //TODO - add support of all three metrics
        double dx = abs(goal.i - current.i);
        double dy = abs(goal.j - current.j);
        if (metrictype=="Octile"){ // добавили диагональную метрику
            double min = dx;
            if (min>dy){
                min = dy;
            }
            current.h = abs(dx - dy) + 1.44 * min;
        }
        else if(metrictype=="Euclidean"){ // добавили евклидовскую(?) метрику
            current.h = sqrt(dx*dx+dy*dy);
        }
        else if(metrictype=="Chebyshev"){ // добавили метрику Чебышева(или у, я не знаю)
            double max = dx;
            if(dy>max){
                max = dy;
            }
            current.h = max;
        }
        else{ // это метрика Манхеттена
            current.h = dx + dy;
        }
        return current.h;
    }
    std::list<Node> reconstruct_path(Node current)
    {
        //TODO - reconstruct path using back pointers находим путь, используя указатели обратно
        std::list<Node>path;
        while(current.parent!=nullptr){
            path.push_front(current); // добавляем каждого соседа в начало пути
            current = *current.parent; // сосед - указатель на родителя соседа
        }
        path.push_front(current); // добавляем соседа без родителя в начало пути
        return path; // возвращаем путь
    }
};

int main(int argc, char* argv[]) //argc - argumnet counter, argv - argument values
{
    for(int i=0; i<argc; i++)
        std::cout<<argv[i]<<"\n";
    if(argc<2)
    {
        std::cout << "Name of the input XML file is not specified."<<std::endl;
        return 1;
    }
    Loader loader; // переменная лоадер
    loader.load_instance(argv[1]); // видимо, встроенный, загружаем файл с матрицей
    Result result; // объявляем переменную типа результат
    if(loader.algorithm == "BFS") // если алгоритм в загруженном файле - бфс
    {
        BFS bfs; // объявляем переменную бфс
        result = bfs.find_path(loader.start, loader.goal, loader.grid); // находим результат по алгоритму бфс
    }
    else
    {
        if(loader.algorithm == "Dijkstra") // если дейкстра, то астарим, но без эвристики(?)
            loader.hweight = 0;
        AStar astar;
        result = astar.find_path(loader.start, loader.goal, loader.grid, loader.metrictype, loader.connections, loader.hweight);
        std::cout<<loader.metrictype<<" "<<loader.connections<<"\n";
    }
    loader.grid.print(result.path);
    std::cout<<"Cost: "<<result.cost<<"\nRuntime: "<<result.runtime
    <<"\nSteps: "<<result.steps<<"\nNodes created: "<<result.nodes_created<<std::endl;
    return 0;
}