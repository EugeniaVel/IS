#include <iostream>
#include <list>
#include <set>
#include "structs.h"
#include "utils.h"
#include <chrono>
#include <cmath> // ��� ����������� ����� � ���������� ���������
#include <algorithm>

class BFS //breadth-first-search
{
public:
    Result find_path(Node start, Node goal, Map grid) // ������� ����, ����� ��������� ����, �������� ���� � ����� �����
    {
        auto time_now = std::chrono::high_resolution_clock::now();
        Result result; // ���������� ���� ���������
        int steps = 0; // �������� ����� ����
        start.g = 0; //��� - ��������� ���� �� ������ ����� = 0
        std::list<Node> OPEN; // ������ ������ �������� �����
        OPEN.push_back(start); // ��������� ������ � ����� ������
        std::set<Node> CLOSED; // ������ ��������� ��� �����, ������� ��� �����������
        CLOSED.insert(start); // ��������� ������ � ��������� ������������� ������
        bool pathfound = false; // ���� �� ������
        while(!OPEN.empty() && !pathfound) // ���� � ��������������� ����� �� ����� � ���� �� ������
        {
           Node current = OPEN.front(); //������� ���� - ������ ���� � ���������
           OPEN.pop_front(); //������� ������ ���� � ����������(?) �����
           steps++; // ��������� ���������� ��������
           auto neighbors = grid.get_neighbors(current); //������������� ���������� ��� ���������� �������, ���� �������
           for(auto n:neighbors) { //� - ��� ������
               if (CLOSED.find(n) == CLOSED.end()) // ���� ��������� ������ � ����� ��������� ����������
               {
                   n.g = current.g + 1; // ����������� ������� ��������� ����
                   n.parent = &(*CLOSED.find(current)); // �������� ������ ��������� � �������� ������
                   OPEN.push_back(n); // ��������� ������ � ����� ��������� ������
                   CLOSED.insert(n); // ��������� ������ � �������� ������
                   if(n == goal) { // ���� ����� - �����
                       result.path = reconstruct_path(n); // �������� ���� ����� ��, ��� ���� ������
                       result.cost = n.g; // ��������� ���� - ��� ��� ������
                       pathfound = true; // ���� ������, ���
                       break;
                    }
                }
            }
        }
        result.steps = steps; //��������
        result.nodes_created = CLOSED.size(); // ������� ������ �� ������ � �����(��� ������ ����� ��������)
        result.runtime = (std::chrono::high_resolution_clock::now() - time_now).count()/1e+9; // ����� ������(���� �������� �� ����, ��� � ���� ������ ��������, �������)
        return result; // �������� ��������� ���
    }
    std::list<Node> reconstruct_path(Node n) // ���������� ���� � ������
    {
        std::list<Node>path; // ���� ������ ����
        while(n.parent != nullptr) // ���� � ������ ���� ��������
        {
            path.push_front(n); // ��������� ������� ������ � ������ ����
            n = *n.parent; // ����� - ��������� �� �������� ������
        }
        path.push_front(n); // ��������� ������ ��� �������� � ������ ����
        return path; // ���������� ����
    }
};

class AStar //����� �������, �������� ������ �����
{
public:
    Result find_path(Node start, Node goal, Map grid, std::string metrictype="Octile", int connections=8, double hweight=1) //��������� ���������
    {
        //TODO - implement the main cycle of AStar algorithm
        auto time_now = std::chrono::high_resolution_clock::now();
        Result result; // ���������� ���� ���������
        int steps = 0; // �������� ����� ����
        start.g = 0; //��� - ��������� ���� �� ������ ����� = 0
        start.h=count_h_value(start, goal,metrictype); // ������� ���������
        start.f = start.g + hweight * start.h;
        std::list<Node> OPEN; // ������ ������ �������� �����
        OPEN.push_back(start); // ��������� ������ � ����� ������
        std::set<Node> CLOSED; // ������ ��������� ��� �����, ������� ��� �����������
        CLOSED.insert(start); // ��������� ������ � ��������� ������������� ������
        bool pathfound = false; // ���� �� ������
        while (!OPEN.empty() && !pathfound) // ���� � ��������������� ����� �� ����� � ���� �� ������
        {
            OPEN.sort([](const Node & a, const Node &b){
                if(a.f==b.f)
                    return a < b;
                return a.f < b.f;
            });
            Node current = OPEN.front(); //������� ���� - ������ ���� � ���������
            OPEN.pop_front(); //������� ������ ���� � ����������(?) �����
            steps++; // ��������� ���������� ��������
            auto neighbors = grid.get_neighbors(current,connections); //������������� ���������� ��� ���������� �������, ���� �������
            for (auto n: neighbors) { //� - ��� ������
                bool found = (CLOSED.find(n) != CLOSED.end());
                Node fn = *(CLOSED.find(n));
                n.h = count_h_value(n, goal,metrictype); // ������� ��������� ������
                n.g = current.g + 1; // ������� ��������� ���� �� ������
                n.f = n.g + hweight * n.h; // ������� ��������� ����� ���� ������
                if (!found || n.g < fn.g) //���� �� �� ���� � ���� ������� ��� ���� �� �� ������
                {
                    OPEN.push_back(n); // ��������� ������ � ����� ��������� ������
                    n.parent = &(*CLOSED.find(current)); //��������� �������� ����� current
                    CLOSED.erase(fn);
                    CLOSED.insert(n); // ��������� ������ � �������� ������
                }
                if (n == goal) { // ���� ����� - �����
                    result.path = reconstruct_path(n); // �������� ���� ����� ��, ��� ���� ������
                    result.cost = n.g; // ��������� ���� - ��� ��� ������;
                    pathfound = true; // ���� ������, ���
                    break;
                }
            }
        }
        result.steps = steps; //��������
        result.nodes_created = CLOSED.size(); // ������� ������ �� ������ � �����(��� ������ ����� ��������)
        result.runtime = (std::chrono::high_resolution_clock::now() - time_now).count()/1e+9; // ����� ������(���� �������� �� ����, ��� � ���� ������ ��������, �������)
        return result;
    }
    double count_h_value(Node current, Node goal, std::string metrictype="Octile") //
    {
        //TODO - add support of all three metrics
        double dx = abs(goal.i - current.i);
        double dy = abs(goal.j - current.j);
        if (metrictype=="Octile"){ // �������� ������������ �������
            double min = dx;
            if (min>dy){
                min = dy;
            }
            current.h = abs(dx - dy) + 1.44 * min;
        }
        else if(metrictype=="Euclidean"){ // �������� ������������(?) �������
            current.h = sqrt(dx*dx+dy*dy);
        }
        else if(metrictype=="Chebyshev"){ // �������� ������� ��������(��� �, � �� ����)
            double max = dx;
            if(dy>max){
                max = dy;
            }
            current.h = max;
        }
        else{ // ��� ������� ����������
            current.h = dx + dy;
        }
        return current.h;
    }
    std::list<Node> reconstruct_path(Node current)
    {
        //TODO - reconstruct path using back pointers ������� ����, ��������� ��������� �������
        std::list<Node>path;
        while(current.parent!=nullptr){
            path.push_front(current); // ��������� ������� ������ � ������ ����
            current = *current.parent; // ����� - ��������� �� �������� ������
        }
        path.push_front(current); // ��������� ������ ��� �������� � ������ ����
        return path; // ���������� ����
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
    Loader loader; // ���������� ������
    loader.load_instance(argv[1]); // ������, ����������, ��������� ���� � ��������
    Result result; // ��������� ���������� ���� ���������
    if(loader.algorithm == "BFS") // ���� �������� � ����������� ����� - ���
    {
        BFS bfs; // ��������� ���������� ���
        result = bfs.find_path(loader.start, loader.goal, loader.grid); // ������� ��������� �� ��������� ���
    }
    else
    {
        if(loader.algorithm == "Dijkstra") // ���� ��������, �� �������, �� ��� ���������(?)
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