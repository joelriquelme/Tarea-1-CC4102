#include <iostream>
#include <vector>

constexpr int MAX_NODE_SIZE = 4096; // Tamaño máximo de un nodo en disco

// Clase para representar un punto en el espacio
class Point {
public:
    double x, y;

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

double distance(const Point& p1, const Point& p2) {
    return std::sqrt(std::pow(p1.x - p2.x, 2) + std::pow(p1.y - p2.y, 2));
}

Point findMedoide(const std::vector<Point>& points) {
    Point medoid = points[0];
    double minRadius = std::numeric_limits<double>::max();

    for (const Point& candidate : points) {
        double maxDist = 0.0;

        for (const Point& p : points) {
            double d = distance(candidate, p);
            if (d > maxDist)
                maxDist = d;
        }

        if (maxDist < minRadius) {
            minRadius = maxDist;
            medoid = candidate;
        }
    }

    return medoid;
}

// Clase para repesentar un Cluster
class Cluster {
public:
    Point medoid;
    std::vector<Point> points;

    Cluster() {}

    Cluster(const Point& _medoid, const std::vector<Point>& _points) : medoid(_medoid), points(_points) {}

    // Función para añadir un punto al cluster
    void add_point(const Point& point) {
        points.push_back(point);
    }

    // Función para recalcular el medoide del cluster
    void recalculate_medoid() {
        medoid = findMedoide(points);
    }

    // Función para unir dos clusters
    void merge(const Cluster& other) {
        points.insert(points.end(), other.points.begin(), other.points.end());
        recalculate_medoid();
    }

    // Función que retorna el radio del cluster
    double radius() const {
        double max_distance = 0.0;

        for (const Point& p : points) {
            double distancia = distance(medoid, p);
            if (distancia > max_distance) {
                max_distance = distancia;
            }
        }

        return max_distance;
    }
};