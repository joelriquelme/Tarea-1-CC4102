#include <iostream>
#include <vector>

constexpr int MAX_NODE_SIZE = 4096; // Tamaño máximo de un nodo en disco


// Clase para representar un punto en el espacio
class Point {
public:
    double x, y;

    // Sobrecarga del operador de igualdad
    bool operator==(const Point& other) const {
        return x == other.x && y == other.y;
    }

    // Sobrecarga del operador de desigualdad
    bool operator!=(const Point& other) const {
        return !(*this == other);
    }

    Point() : x(0.0), y(0.0) {}
    
    Point(double _x, double _y) : x(_x), y(_y) {}

    
};

// Clase para representar una entrada en el M-Tree
class Entry {
public:
    Point p;
    double cr; // Radio cobertor
    int child_page; // Dirección en disco de la página del hijo

    Entry(const Point& _p, double _cr, int _child_page) : p(_p), cr(_cr), child_page(_child_page) {}
};

constexpr int B = MAX_NODE_SIZE / sizeof(Entry); // Factor de ramificación del árbol

// Clase para representar un nodo en el M-Tree
class Node {
public:
    std::vector<Entry> entries;

    Node() {}

    bool is_leaf() const {
        return entries.empty();
    }

    bool is_full() const {
        return entries.size() >= B;
    }

    void add_entry(const Entry& entry) {
        entries.push_back(entry);
    }
};


// Clase para representar el M-Tree
class MTree {
public:
    Node root;

    MTree() {}

    void insert(const Entry& entry) {
        insert_entry(entry, root);
    }

    void insert_entry(const Entry& entry, Node& node) {
        if (node.is_leaf()) {
            node.add_entry(entry);
            // Realizar aquí la lógica para dividir el nodo si está lleno y rebalancear el árbol si es necesario
        } else {
            // Realizar aquí la lógica para buscar el nodo hijo adecuado y llamar recursivamente a insert_entry
        }
    }

    // Otros métodos como búsqueda, eliminación, etc., se pueden agregar aquí
};