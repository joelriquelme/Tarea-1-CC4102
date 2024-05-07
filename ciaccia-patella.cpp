#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <random>
#include <chrono>
#include <limits>
#include "structures.cpp"

// Función para generar n puntos aleatorios en el rango [min_val, max_val]
std::vector<Point> generate_random_points(int n, double min_val, double max_val) {
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::mt19937 gen(seed);
    
    std::uniform_real_distribution<> dis_x(min_val, max_val);
    std::uniform_real_distribution<> dis_y(min_val, max_val);

    std::vector<Point> points;
    for (int i = 0; i < n; ++i) {
        double x = dis_x(gen);
        double y = dis_y(gen);
        points.emplace_back(x, y);
    }
    return points;
}

// Función para calcular la distancia euclidiana entre dos puntos
double eucludean_distance(const Point& p1, const Point& p2) {
    return std::sqrt(std::pow(p1.x - p2.x, 2) + std::pow(p1.y - p2.y, 2));
}

// Función para obtener el punto más cercano en un conjunto de puntos
Point closest_point(const std::vector<Point>& points, const Point& p) {
    double min_distance = std::numeric_limits<double>::max();
    Point closest;

    for (const auto& point : points) {
        double distance = eucludean_distance(point, p);
        if (distance < min_distance) {
            min_distance = distance;
            closest = point;
        }
    }

    return closest;
}

// Algoritmo Ciaccia-Patella (CP)
void ciaccia_patella(const std::vector<Point>& P) {
    int n = P.size();
    std::random_device rd;
    std::mt19937 gen(rd());
    std::vector<Point> F;
    
    // Paso 2: Selección aleatoria de k puntos
    int k = std::min(B, (n/B));
    std::cout << n << "\n";
    std::cout << k << "\n";
    std::sample(P.begin(), P.end(), std::back_inserter(F), k, gen);

    

    // Paso 3: Asignación de cada punto a su muestra más cercana
    std::vector<std::vector<Point>> F_sets;
    F_sets.resize(k); // Establecemos el tamaño del vector F_sets a k

    for (const auto& point : P) {
        Point closest = closest_point(F, point);
        int idx = std::find(F.begin(), F.end(), closest) - F.begin();
        F_sets[idx].push_back(point);
    }

    std::cout << "Conjuntos de puntos asociados a cada muestra (Pre redistribucion):" << std::endl;
    for (int i = 0; i < k; ++i) {
        std::cout << "Muestra " << i << ": ";
        std::cout << F_sets[i].size();
        //for (const auto& point : F_sets[i]) {
        //    std::cout << "(" << point.x << ", " << point.y << ") ";
        //}
        std::cout << std::endl;
    }

    // // Pasos 4: Etapa de redistribución.
    // for (int i = 0; i < k; ++i) {
    //     if (F_sets[i].size() < (B/2)) {
    //         F.erase(std::remove(F.begin(), F.end(), samples[i]), F.end());
    //         for (const auto& p : F_sets[i]) {
    //             Point closest = closest_point(samples, p);
    //             int idx = std::find(samples.begin(), samples.end(), closest) - samples.begin();
    //             F_sets[idx].push_back(p);
    //         }
    //     }
    // }

    // Paso 4: Etapa de redistribución.
    for (int i = 0; i < k; ++i) {
        if (F_sets[i].size() < (B/2)) {
            std::vector<Point> redistributed_points;
            // Almacenar temporalmente los puntos que necesitan ser redistribuidos
            for (const auto& p : F_sets[i]) {
                std::cout << i  << std::endl;
                Point closest = closest_point(F, p);
                std::cout << closest.x << " " << closest.y  << std::endl;
                //int idx = std::find(F.begin(), F.end(), closest) - F.begin();
                redistributed_points.push_back(p);
                F_sets[i].erase(std::remove(F_sets[i].begin(), F_sets[i].end(), p), F_sets[i].end());
            }
            std::cout << "alo1" << std::endl;
            // Eliminar la muestra del vector F
            F.erase(std::remove(F.begin(), F.end(), F[i]), F.end());
            std::cout << "alo2" << std::endl;
            // Agregar los puntos redistribuidos a sus conjuntos correspondientes
            for (const auto& redistributed_point : redistributed_points) {
                Point closest = closest_point(F, redistributed_point);
                int idx = std::find(F.begin(), F.end(), closest) - F.begin();
                F_sets[idx].push_back(redistributed_point);
            }
        }
    }

    // Imprimir el resultado de asignación de puntos a muestras
    std::cout << "Conjuntos de puntos asociados a cada muestra: (Post distribucion)" << std::endl;
    for (int i = 0; i < k; ++i) {
        std::cout << "Muestra " << i << ": ";
        std::cout << F_sets[i].size();
        //for (const auto& point : F_sets[i]) {
        //    std::cout << "(" << point.x << ", " << point.y << ") ";
        //}
        std::cout << std::endl;
    }


    //std::cout << "Contenidos de F:" << std::endl;
    //for (const auto& point : F) {
    //    std::cout << "(" << point.x << ", " << point.y << ")" << std::endl;
    //}
    
}

int main() {
    int n = 1000; // Número de puntos a generar
    double min_val = 0.0; // Valor mínimo en el rango
    double max_val = 100.0; // Valor máximo en el rango

    std::vector<Point> points = generate_random_points(n, min_val, max_val);

    // Imprimir los puntos generados
    //std::cout << "Puntos generados:" << std::endl;
    //for (const auto& point : points) {
    //    std::cout << "(" << point.x << ", " << point.y << ")" << std::endl;
    //}

    Point test_point = Point(50.0, 50.0); // Punto de prueba

    Point closest = closest_point(points, test_point);

    // Imprimir el punto más cercano al punto de prueba
    std::cout << "Punto más cercano a (" << test_point.x << ", " << test_point.y << "): (" << closest.x << ", " << closest.y << ")" << std::endl;

    ciaccia_patella(points);
    return 0;
}