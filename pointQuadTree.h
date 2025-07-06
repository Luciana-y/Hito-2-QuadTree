#ifndef POINTQUADTREE_H
#define POINTQUADTREE_H

#include <vector>

template <typename T>
struct Point {
    int x, y;
    T data;
    Point(int x, int y, T data) : x(x), y(y), data(data) {}
};

template <typename T>
class QuadTree {
public:
    struct Node {
        Point<T> point;
        Node *nw, *ne, *sw, *se;
        Node* parent;
        Node(int x, int y, T data)
            : point(x, y, data), nw(nullptr), ne(nullptr), sw(nullptr), se(nullptr), parent(nullptr) {}
    };

    Node* root = nullptr;
    size_t size = 0;

    Point<T>* recursiveSearch(Node* node, int x, int y) {
        if (!node) return nullptr;
        if (node->point.x == x && node->point.y == y) {
            return &node->point;
        }
        if (x < node->point.x && y >= node->point.y) {
            return recursiveSearch(node->nw, x, y);
        }
        if (x >= node->point.x && y >= node->point.y) {
            return recursiveSearch(node->ne, x, y);
        }
        if (x < node->point.x && y < node->point.y) {
            return recursiveSearch(node->sw, x, y);
        }
        return recursiveSearch(node->se, x, y);
    }

    Node* recursiveInsert(Node* node, int x, int y, T data, Node* parent) {
        if (node == nullptr) {
            size++;
            Node* newNode = new Node(x, y, data);
            newNode->parent = parent;
            return newNode;
        }
        if (node->point.x == x && node->point.y == y) {
            node->point.data = data;
            return node;
        }
        if (x < node->point.x && y >= node->point.y) {
            node->nw = recursiveInsert(node->nw, x, y, data, node);
        } else if (x >= node->point.x && y >= node->point.y) {
            node->ne = recursiveInsert(node->ne, x, y, data, node);
        } else if (x < node->point.x && y < node->point.y) {
            node->sw = recursiveInsert(node->sw, x, y, data, node);
        } else {
            node->se = recursiveInsert(node->se, x, y, data, node);
        }
        return node;
    }

    void collectSubtree(Node* node, std::vector<Point<T>>& points) {
        if (!node) return;
        points.push_back(node->point);
        collectSubtree(node->nw, points);
        collectSubtree(node->ne, points);
        collectSubtree(node->sw, points);
        collectSubtree(node->se, points);
    }

    void clearSubtree(Node* node) {
        if (!node) return;
        clearSubtree(node->nw);
        clearSubtree(node->ne);
        clearSubtree(node->sw);
        clearSubtree(node->se);
        delete node;
    }

    Node* recursiveDelete(Node* node, int x, int y, std::vector<Point<T>>& pointsToReinsert, bool& found) {
        if (!node) return nullptr;
        if (node->point.x == x && node->point.y == y) {
            found = true;
            if (node->nw) collectSubtree(node->nw, pointsToReinsert);
            if (node->ne) collectSubtree(node->ne, pointsToReinsert);
            if (node->sw) collectSubtree(node->sw, pointsToReinsert);
            if (node->se) collectSubtree(node->se, pointsToReinsert);

            if (node->nw) {
                clearSubtree(node->nw);
                node->nw = nullptr;
            }
            if (node->ne) {
                clearSubtree(node->ne);
                node->ne = nullptr;
            }
            if (node->sw) {
                clearSubtree(node->sw);
                node->sw = nullptr;
            }
            if (node->se) {
                clearSubtree(node->se);
                node->se = nullptr;
            }

            delete node;
            return nullptr;
        }

        if (x < node->point.x && y >= node->point.y) {
            node->nw = recursiveDelete(node->nw, x, y, pointsToReinsert, found);
        } else if (x >= node->point.x && y >= node->point.y) {
            node->ne = recursiveDelete(node->ne, x, y, pointsToReinsert, found);
        } else if (x < node->point.x && y < node->point.y) {
            node->sw = recursiveDelete(node->sw, x, y, pointsToReinsert, found);
        } else {
            node->se = recursiveDelete(node->se, x, y, pointsToReinsert, found);
        }
        return node;
    }

    QuadTree() = default;
    ~QuadTree() {
        clearSubtree(root);
    }

    void insert(const int x, const int y, T data) {
        root = recursiveInsert(root, x, y, data, nullptr);
    }

    void remove(const int x, const int y) {
        std::vector<Point<T>> pointsToReinsert;
        bool found = false;
        root = recursiveDelete(root, x, y, pointsToReinsert, found);
        if (found) {
            size--;
            for (auto& point : pointsToReinsert) {
                insert(point.x, point.y, point.data);
            }
        }
    }

    Point<T>* search(int x, int y) {
        return recursiveSearch(root, x, y);
    }

    [[nodiscard]] size_t getSize() const {
        return size;
    }

    [[nodiscard]] bool isEmpty() const {
        return size == 0;
    }

    Node* getRoot() const {
        return root;
    }
};

#endif // POINTQUADTREE_H