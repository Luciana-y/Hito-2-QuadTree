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
    // -----------recursiones------------
    Point<T>* recursiveSearch(Node* node, int x, int y) {
        if (!node) return nullptr;
        if (node->point.x == x && node->point.y == y) { //si lo encuentra
            return &node->point;
        }
        //cuando el punto se encuentra igual de y va arriba(norte)
        //cuando el punto se encuentra igual de x va derecha(este)
        //tomando en cuenta que no ha encontrando el punto exacto aun
        if (x < node->point.x && y >= node->point.y) { // si el punto actual esta más a la derecha y abajo (ir arriba izquierda)
            return recursiveSearch(node->nw, x, y);
        }
        if (x >= node->point.x && y >= node->point.y) { //si el punto actual esta más a la izquierda y abajo (ir arriba derecha)
            return recursiveSearch(node->ne, x, y);
        }
        if (x < node->point.x && y < node->point.y) { //si el punto actual esta más a la derecha y arriba (ir abajo izquierda)
            return recursiveSearch(node->sw, x, y);
        }
        return recursiveSearch(node->se, x, y); //si ninguno funciona por defecto ira al que falta(abajo derecha)
    }
    //espacio O(n); tiempo O(n)
public:
    Point<T> search(const int x, const int y) {
        return recursiveSearch(root, x, y);
    }


};


int main() {
    std::cout << "wa!" << std::endl;
    return 0;
}