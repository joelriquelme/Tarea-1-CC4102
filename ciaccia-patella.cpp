#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <random>
#include <chrono>
#include <limits>
#include "structures.cpp"

// Función para calcular la distancia euclidiana entre dos puntos
double eucludean_distance(const Point& p1, const Point& p2) {
    return std::sqrt(std::pow(p1.x - p2.x, 2) + std::pow(p1.y - p2.y, 2));
}

// Funcion para redistribuir los clusters de tamaño menor a B/2
void redistribute_clusters(std::vector<std::vector<Point>>& F_sets, int B) {
    int k = F_sets.size();
    std::vector<bool> redistributed(k, false);

    for (int i = 0; i < k; ++i) {
        if (F_sets[i].size() < B/2) { // Adjusted threshold for redistribution
            std::vector<Point> small_cluster = F_sets[i]; // Store the points of the small cluster

            // Iterate over each point in the small cluster
            for (const auto& point : small_cluster) {
                double min_distance = std::numeric_limits<double>::max();
                int closest_cluster_index = -1;

                // Compare the current point with all other clusters
                for (int j = 0; j < k; ++j) {
                    if (i != j && !redistributed[j] && F_sets[j].size() >= B) {
                        // Compare the current point with all points in the current cluster
                        for (const auto& other_point : F_sets[j]) {
                            double distance = eucludean_distance(point, other_point);
                            if (distance < min_distance) {
                                min_distance = distance;
                                closest_cluster_index = j;
                            }
                        }
                    }
                }

                // Move the point to the cluster with the closest distance
                if (closest_cluster_index != -1) {
                    F_sets[closest_cluster_index].push_back(point);
                }
            }

            redistributed[i] = true; // Mark the small cluster as redistributed
            F_sets[i].clear(); // Clear the small cluster
        }
    }
}


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

// para transformar de points a Entry
std::vector<Entry> transform_to_entries(const std::vector<Point>& points) {
    std::vector<Entry> entries;
    for (const auto& point : points) {
        double cr = 0.0; // For simplicity, setting covering radius to 0
        int child_page = -1; // For simplicity, setting child page to -1
        Entry entry(point, cr, child_page);
        entries.push_back(entry);
    }
    return entries;
}

// Funcion para paso 5
void random_select(std::vector<std::vector<Point>>& F_sets, const std::vector<Point>& P, int B) {
    
        std::cout << "Paso 5" << std::endl;
        int n = P.size();
        
        std::random_device rd;
        std::mt19937 gen(rd());
        std::vector<Point> F;

        // Paso 2: Selección aleatoria de k puntos
        int roof = std::ceil(static_cast<double>(n) / B);
        int k = std::min(B, roof);
        std::cout << n << "\n";
        std::cout << k << "\n";
        std::sample(P.begin(), P.end(), std::back_inserter(F), k, gen);

        // Paso 3
        std::vector<std::vector<Point>> new_F_sets;
        new_F_sets.resize(k); // Establecemos el tamaño del vector F_sets a k

        for (const auto& point : P) {
            Point closest = closest_point(F, point);
            int idx = std::find(F.begin(), F.end(), closest) - F.begin();
            new_F_sets[idx].push_back(point);
        }

        // Step 4: Redistribution stage
        redistribute_clusters(new_F_sets, B);

        // Borramos los cluster vacios
        new_F_sets.erase(std::remove_if(new_F_sets.begin(), new_F_sets.end(), [](const std::vector<Point>& cluster) {
            return cluster.empty();
        }), new_F_sets.end());
        std::cout << "Conjuntos de puntos asociados a cada muestra: (Post Eliminacion)" << std::endl;
        for (int i = 0; i < new_F_sets.size(); ++i) {
            std::cout << "Muestra " << i << ": ";
            std::cout << new_F_sets[i].size();
            std::cout << std::endl;
        }


        // If there's still only one cluster, repeat the process
        if (new_F_sets.size() == 1) {
            std::cout << "No se logro!" << std::endl;
            random_select(F_sets, P, B);
        } else {
            // Otherwise, assign the new sets to F_sets
            F_sets = new_F_sets;
            std::cout << "Se salio del paso  5!!" << std::endl;
            
        }
        
    
}




// Algoritmo Ciaccia-Patella (CP)
void ciaccia_patella(const std::vector<Point>& P) {
    int n = P.size();
    std::random_device rd;
    std::mt19937 gen(rd());
    std::vector<Point> F;
    
    // Paso 1: Si P <= B Mandamos a todos los puntos al arbol
    if (n <= B) {
        //MTree T;
        std::vector<Entry> entries = transform_to_entries(P);
        for (const auto& entry : entries) {
            //T.insert(entry);
        }
        std::cout << "AL ARBOOOOL!!" << std::endl;
        //return T;
    }
    else {
    // Paso 2: Selección aleatoria de k puntos
    int roof = std::ceil(static_cast<double>(n) / B);
    int k = std::min(B, roof);
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
    std::cout << "aloAAAAAAAAAssAAAAAAA1" << std::endl;

    // Pasos 4: Etapa de redistribución.
    redistribute_clusters(F_sets, B);

    // Imprimir el resultado de asignación de puntos a muestras
    std::cout << "Conjuntos de puntos asociados a cada muestra: (Post distribucion)" << std::endl;
    for (int i = 0; i < F_sets.size(); ++i) {
        std::cout << "Muestra " << i << ": ";
        std::cout << F_sets[i].size();
        //for (const auto& point : F_sets[i]) {
        //    std::cout << "(" << point.x << ", " << point.y << ") ";
        //}
        std::cout << std::endl;
    }

    // Borramos los cluster vacios
    F_sets.erase(std::remove_if(F_sets.begin(), F_sets.end(), [](const std::vector<Point>& cluster) {
        return cluster.empty();
    }), F_sets.end());
    std::cout << "Conjuntos de puntos asociados a cada muestra: (Post Eliminacion)" << std::endl;
    for (int i = 0; i < F_sets.size(); ++i) {
        std::cout << "Muestra " << i << ": ";
        std::cout << F_sets[i].size();
        std::cout << std::endl;
    }

    // Paso 5: Si solo hay un cluster se vuelve al paso 2
    if (F_sets.size() == 1) {
        random_select(F_sets,P,B);
        std::cout << "TEST1" << std::endl;
        std::cout << F_sets.size() << std::endl;
        for (int i = 0; i < F_sets.size(); ++i) {
            std::cout << "TEST" << std::endl;
            ciaccia_patella(F_sets[i]);
            
        }
    }
    else {
    // Paso 6: hacer CP recursivamente con cada cluster
    for (int i = 0; i < F_sets.size(); ++i) {
        ciaccia_patella(F_sets[i]);
        // Merge the subtree into T
        // Implement merging logic here
    }}
    }
}

int main() {
    int n = 1000; // Número de puntos a generar
    double min_val = 0.0; // Valor mínimo en el rango
    double max_val = 100.0; // Valor máximo en el rango

    std::vector<Point> points = generate_random_points(n, min_val, max_val);
    Point test_point = Point(50.0, 50.0); // Punto de prueba
    Point closest = closest_point(points, test_point);

    // Imprimir el punto más cercano al punto de prueba
    std::cout << "Punto más cercano a (" << test_point.x << ", " << test_point.y << "): (" << closest.x << ", " << closest.y << ")" << std::endl;
    std::cout << "BBBBBBBBB" << std::endl;
    std::cout << B << std::endl;
    ciaccia_patella(points);
    std::cout << "FIIIIN" << std::endl;
    return 0;
}