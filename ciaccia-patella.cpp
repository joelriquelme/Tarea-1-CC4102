#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <random>
#include <chrono>
#include <limits>
#include "structures.cpp"





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

int main() {
    int n = 10; // Número de puntos a generar
    double min_val = 0.0; // Valor mínimo en el rango
    double max_val = 100.0; // Valor máximo en el rango

    std::vector<Point> points = generate_random_points(n, min_val, max_val);

    // Imprimir los puntos generados
    std::cout << "Puntos generados:" << std::endl;
    for (const auto& point : points) {
        std::cout << "(" << point.x << ", " << point.y << ")" << std::endl;
    }

    Point test_point = Point(50.0, 50.0); // Punto de prueba

    Point closest = closest_point(points, test_point);

    // Imprimir el punto más cercano al punto de prueba
    std::cout << "Punto más cercano a (" << test_point.x << ", " << test_point.y << "): (" << closest.x << ", " << closest.y << ")" << std::endl;

    return 0;
}

// Algoritmo Ciaccia-Patella (CP)
Node ciaccia_patella(const std::vector<Point>& P) {
    int n = P.size();
    std::random_device rd;
    std::mt19937 gen(rd());
    std::vector<Point> F;
    
    // Paso 2: Selección aleatoria de k puntos
    int k = std::min(B, n);
    std::vector<Point> samples;
    std::sample(P.begin(), P.end(), std::back_inserter(samples), k, gen);
    F.insert(F.end(), samples.begin(), samples.end());

}