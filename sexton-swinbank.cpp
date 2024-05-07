#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <random>
#include <chrono>
#include <limits>
#include "utils.cpp"

/*
Función que dada un conjuntos de puntos, retorna un conjunto de clusters de tamaño entre b y B

param C_in: Un set de puntos de tamaño mínimo b

return: retorna un vector de clusters de tamaño entre b y B.
*/

std::vector<Cluster> makeClusters(std::vector<Point> C_in, int B){
    //1. Se define C_out = {} y C = {}
    std::vector<Cluster> C_out;
    std::vector<Cluster> C;

    //2. Por cada punto p ∈ C_in se añade {p} a C.
    for (int i = 0; i < C_in.size(); i++){
        Cluster temp = Cluster();
        temp.add_point(C_in[i]);
        temp.recalculate_medoid();
        C.push_back(temp);
    }

    //3. Mientras |C| > 1:
    while (C.size() > 1){
        //3.1 Sea c1, c2 los pares más cercanos de clusters en C tal que |c1| ≥ |c2|.
        double min_distance = std::numeric_limits<double>::max();
        int c1_index = 0;
        int c2_index = 0;
        for (int i = 0; i < C.size(); i++){
            for (int j = i+1; j < C.size(); j++){
                double distance = euclidean_distance(C[i].medoid, C[j].medoid);
                if (distance < min_distance){
                    min_distance = distance;
                    c1_index = i;
                    c2_index = j;
                }
            }
        }
        if (C[c1_index].points.size() < C[c2_index].points.size()){
            std::swap(c1_index, c2_index);
        }
        
        //3.2 Si |c1 ∪ c2| ≤ B, se remueve c1 y c2 de C y se añade c1 ∪ c2 a C.
        if (C[c1_index].points.size() + C[c2_index].points.size() <= B){
            C[c1_index].merge(C[c2_index]);
            C.erase(C.begin() + c2_index);
        }

        //3.3 Si no, se remueve c1 de C y se añade c1 a Cout.
        else{
            C_out.push_back(C[c1_index]);
            C.erase(C.begin() + c1_index);
        }
    }

    //4. Sea c el último elemento de C
    Cluster c = C[0];
    
    //5. Si |C_out| > 0:
    Cluster cp = Cluster();
    if (C_out.size() > 0){
        //5.1 definimos cp como el vecino más cercano a c en C_out. Removemos cp de C_out
        double min_distance = std::numeric_limits<double>::max();
        int cp_index = 0;
        for (int i = 0; i < C_out.size(); i++){
            double distance = euclidean_distance(c.medoid, C_out[i].medoid);
            if (distance < min_distance){
                min_distance = distance;
                cp_index = i;
            }
        }
        //Revomemos cp de C_out
        Cluster cp = C_out[cp_index];
        C_out.erase(C_out.begin() + cp_index);
    }
    //5.2 Si no, se define cp = {}.

    //6. Si |c ∪ cp| ≤ B:
    if (c.points.size() + cp.points.size() <= B){
        //6.1 Añadimos c ∪ cp a C_out
        c.merge(cp);
        C_out.push_back(c);
    }
    //6.2 Si no, dividimos c ∪ cp en c1 y c2 usando MinMax split policy. Se añaden c1 y c2 a Cout.
    else{
        Cluster c_temp = c;
        c_temp.merge(cp);
        std::pair<Cluster, Cluster> bestClusters = minmax_split(c_temp.points);
        Cluster c1 = bestClusters.first;
        Cluster c2 = bestClusters.second;
        C_out.push_back(c1);
        C_out.push_back(c2);
    }
    
    //7. Se retorna C_out
    return C_out;  
}

int main(){
    int n = 1000; // Número de puntos a generar
    int B = 128; // Tamaño máximo de un cluster
    double min_val = 0.0; // Valor mínimo en el rango
    double max_val = 1.0; // Valor máximo en el rango

    std::vector<Point> points = generate_random_points(n, min_val, max_val);

    // // Imprimir los puntos generados
    // std::cout << "Puntos generados:" << std::endl;
    // for (const auto& point : points) {
    //     std::cout << "(" << point.x << ", " << point.y << ")" << std::endl;
    // }

    std::vector<Cluster> clusters = makeClusters(points, B);

    // Imprimir los clusters generados
    std::cout << "Clusters generados:" << std::endl;
    int i = 1;
    for (const auto& cluster : clusters) {
        std::cout << "Cluster " << i++ << ":" << std::endl;
        std::cout << "Medoid: (" << cluster.medoid.x << ", " << cluster.medoid.y << ")" << std::endl;
        std::cout << "Puntos:" << std::endl;
        for (const auto& point : cluster.points) {
            std::cout << "(" << point.x << ", " << point.y << ")" << std::endl;
        }
        std::cout << "Radio: " << cluster.radius() << std::endl;
    }

}

