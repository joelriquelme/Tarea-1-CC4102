#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <random>
#include <chrono>
#include <limits>
#include "structures.cpp"

/*
Función para generar n puntos aleatorios en el rango [min_val, max_val]

param n: Número de puntos a generar
param min_val: Valor mínimo en el rango
param max_val: Valor máximo en el rango

return: Vector de puntos generados
*/ 
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

/*
Función para calcular la distancia euclidiana entre dos puntos

param p1: Primer punto
param p2: Segundo punto

return: Distancia euclidiana entre los dos puntos
*/
double euclidean_distance(const Point& p1, const Point& p2) {
    return std::sqrt(std::pow(p1.x - p2.x, 2) + std::pow(p1.y - p2.y, 2));
}

/*
Función para encontrar dentro de un conjunto de puntos el punto más cernano a uno dado.

param points: Conjunto de puntos
param p: Punto de referencia

return: Punto más cercano al p dentro de points. 
        Si hay más de uno, se retorna el primer p. 
        Si points está vacío, se retorna un p.
*/
std::pair<Point, int> closest_point(const std::vector<Point>& points, const Point& p) {
    double min_distance = std::numeric_limits<double>::max();
    Point closest;
    closest = p;

    int i = 0;
    int find_i = 0;
    for (const auto& point : points) {
        double distance = euclidean_distance(point, p);
        if (distance < min_distance) {
            min_distance = distance;
            closest = point;
            find_i = i;
        }
        i++;
    }
    return {closest, find_i};
}

/*
Función para entontrar los dos puntos más cercanos entre un conjunto de puntos.

param points: Conjunto de puntos

return: Par de puntos más cercanos. Si hay más de un par, se retorna el primero encontrado.
*/
std::pair<Point, Point> closest_points(const std::vector<Point>& points) {
    double min_distance = std::numeric_limits<double>::max();
    std::pair<Point, Point> closest;

    for (int i = 0; i < points.size(); ++i) {
        for (int j = i + 1; j < points.size(); ++j) {
            double distance = euclidean_distance(points[i], points[j]);
            if (distance < min_distance) {
                min_distance = distance;
                closest = {points[i], points[j]};
            }
        }
    }

    return closest;
}

/*
Función para encontrar el medoide de un conjunto de puntos.

param points: Conjunto de puntos

return: Medoide del conjunto de puntos. Si existe más de un medoide se retorna el primero encontrado. 
*/
Point findMedoid(const std::vector<Point>& points) {
    Point medoid = points[0];
    double minRadius = std::numeric_limits<double>::max();

    for (const Point& candidate : points) {
        double maxDist = 0.0;

        for (const Point& p : points) {
            double d = euclidean_distance(candidate, p);
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

/*
El MinMax split policy corresponde a lo siguiente: Se considera todos los posibles pares de
puntos, y alternadamente se van agregando el punto más cercano a alguno de estos centros
(esto garantiza que la división sea balanceada) y se calcula el radio cobertor máximo entre estos
dos grupos resultantes. Esto se prueba para todo par de puntos y se elige el par que tenga el
mínimo radio cobertor máximo

param points: Conjunto de puntos

return: Par de clusters que minimiza el radio cobertor máximo.
*/

std::pair<Cluster, Cluster> minmax_split(const std::vector<Point>& points) {
    double minRadius = std::numeric_limits<double>::max();
    std::pair<Cluster, Cluster> bestClusters;

    for (int i = 0; i < points.size(); ++i) {
        for (int j = i + 1; j < points.size(); ++j) {
            Cluster c1, c2;
            std::vector<Point> points_temp;
            points_temp = points;
            c1.medoid = points[i];
            c2.medoid = points[j];
            
            int turn = 0;
            for (int k = 0; k < points_temp.size(); ++k) {
                if (turn % 2 == 0){
                    std::pair<Point, int> closest = closest_point(points_temp, c1.medoid);
                    c1.add_point(closest.first);
                    points_temp.erase(points_temp.begin() + closest.second);
                    
                    
                }
                else{
                    std::pair<Point, int> closest = closest_point(points_temp, c2.medoid);
                    c2.add_point(closest.first);
                    points_temp.erase(points_temp.begin() + closest.second);
                }
            }    
            double maxRadius = std::max(c1.radius(), c2.radius());
            if (maxRadius < minRadius) {
                minRadius = maxRadius;
                bestClusters = {c1, c2};
            }
        }
    }
    return bestClusters;
}