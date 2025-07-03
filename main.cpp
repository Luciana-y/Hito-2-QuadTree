#include <iostream>

template <typename T>
struct Point { //un punto bidimensional que contiene información
    int x,y;
    T data;

    Point(int x, int y, T data) : x(x), y(y), data(data) {} //constructor por defecto
};

template <typename T>
class QuadTree {
private:
    struct Node { // el nodo dentro del arbol, cada uno tiene 4 hijos en 4 direcciones
        Point<T> point;
        Node* nw=nullptr, ne=nullptr, sw=nullptr, se=nullptr;
        Node* parent=nullptr;
        bool isLeaf=true;
        Node(int x, int y, T data){ point.x = x; point.y = y; point.data = data; }
    };

    Node* root = nullptr;
    size_t size = 0;
};


int main() {
    std::cout << "wa!" << std::endl;
    return 0;
}