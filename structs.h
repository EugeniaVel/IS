#ifndef LAB1V2_STRUCTS_H
#define LAB1V2_STRUCTS_H
#include <vector>
#include <iostream>
#include <list>
#define OBSTACLE 1
#define TRAVERSABLE 0
#define PATH 2

struct Node // ��������� ����
{
    int i; // ���������� �����
    int j; // ���������� �����
    double g; // ��������� ���� �� ������ �����
    double h; // ����������������� ��������� ���� �� ������
    double f; // ��������� ���� �� ����� + ����������������� ��������� �� ������
    const Node* parent; // ��������(���������� ������?)
    Node(int _i=-1, int _j=-1, double _g=0, double _h=0, Node* _parent=nullptr):i(_i), j(_j), g(_g), h(_h), f(_h+_g), parent(_parent){}
    bool operator ==(const Node& other) const //���������� ��������� ==
    {
        return this->i == other.i && this->j == other.j; //���������� ����� �� ���������� ����
    }
    bool operator <(const Node& other) const // ���������� ��������� ������
    {
        return this->i < other.i || (this->i == other.i && this->j < other.j);
    }
};

struct Result
{
    std::list<Node> path; // ����
    double cost; // ��������� ����
    double runtime; // ����� ������ ���������
    int steps; // ����� �������� ������ ���������
    int nodes_created; // ����� ��������� ������
};

struct Map //��������� �����
{
    int width; //������
    int height; //������
    std::vector<std::vector<int>> grid; //������ ������ ����(�����) �� ����� �����
    Map(int _width=0, int _height=0):width(_width), height(_height) //������ ����� � ������
    {
        grid = std::vector<std::vector<int>>(height, std::vector<int>(width, TRAVERSABLE)); //������ ������ ����� 0, ��� ������ ����������
    }
    void add_obstacle(int i, int j) // ��������� �����������
    {
        grid[i][j] = OBSTACLE;
    }
    bool cell_on_grid(int i, int j) // ���������, ���� �� �� ����� ��� ������
    {
        return (i >= 0 && j >=0 && i < height && j < width);
    }
    bool cell_is_obstacle(int i, int j) // ���������, �������� �� ������ ������������
    {
        return grid[i][j] == OBSTACLE; // ����������� ����� �������
    }
    std::vector<Node> get_neighbors(Node s, int connections=8) // �������� �������
    {
        //TODO - implement support of diagonal moves, �������� �������� �� ����������
        std::vector<std::pair<int, int>> deltas = {{0,1}, {1,0}, {-1,0}, {0,-1}};
        std::vector<Node> neighbors; //������ ������ ������ �� �����
        for(auto d:deltas) // � - ������
        {
            Node n(s.i + d.first, s.j + d.second); //������ ���� � � ������������ �������
            if(cell_on_grid(n.i, n.j) && !cell_is_obstacle(n.i, n.j)) // ���� ������ ���� �� ����� � ��� �� �����������
                neighbors.push_back(n); // ��������� ���� � ����� �������
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
        return neighbors; // ���������� ������ �������
    }
    void print(std::list<Node> path={}) //������� ����� � ���� �� �����
    {
        for(auto n:path) //�-����
            grid[n.i][n.j] = PATH; 
        for(int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++)
                if (grid[i][j] == OBSTACLE)//���� ������ - �����������, ������ �������
                    std::cout << "# ";
                else if (grid[i][j] == TRAVERSABLE)//���� ���������� ������, �� �������
                    std::cout << ". ";
                else //� ������ ������� ������� ������(��� ����, ��������)
                    std::cout << "@ ";
            std::cout<<std::endl;
        }
        for(auto n:path) //�-����
            grid[n.i][n.j] = TRAVERSABLE; 
    }
};
#endif //LAB1V2_STRUCTS_H
