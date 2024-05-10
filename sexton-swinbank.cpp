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

return: retorna un vector de clusters de tamaño entre b y B. Siendo b = B/2.
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
        //asignamos el elemento encontrado a cp
        cp = C_out[cp_index];
        C_out.erase(C_out.begin() + cp_index);
    }
    //5.2 Si no, se define cp = {}.
    else{
        cp = Cluster();
    }
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


/*
Funcion que dado un conjunto de puntos C_in, retorna un conjunto una tupla (g, r, a) donde g es el medoide primario de Cin, r es llamado el
radio cobertor y a la dirección del hijo respectivo.

param C_in: Un conjunto de puntos

return: Una tupla (g, r, a) donde g es el medoide primario de Cin, r es llamado el radio cobertor y a la dirección del hijo respectivo.
*/

Entry OutputHoja(const std::vector<Point>& C_in) {
    // 1. Sea g el medoide primario de _Cin. Sea r = 0. Sea C = {} (el que corresponderá al nodo hoja).
    Point g = findMedoid(C_in);
    double r = 0;
    std::vector<Entry> C;
    C.reserve(C_in.size());
    for (const auto& point : C_in) {
        Entry temp = Entry(point, 0.0, nullptr);
        C.push_back(temp);
        r = std::max(r, euclidean_distance(g, point));
    }
    
    // 4. Retornamos (g, r, a)
    return Entry(g, r, new std::vector<Entry>(C)); // Se usa new para asignar memoria dinámicamente
}


Entry OutputInterno(const std::vector<Entry>& C_mra) {
    std::vector<Point> medoides;
    for (const auto& entry : C_mra) {
        medoides.push_back(entry.p);
    }
    Point G = findMedoid(medoides);
    double R = 0;
    std::vector<Entry> C;
    C.reserve(C_mra.size());
    for (const auto& entry : C_mra) {
        C.push_back(entry);
        R = std::max(R, euclidean_distance(G, entry.p) + entry.cr);
    }
    return Entry(G, R, new std::vector<Entry>(C)); // Se usa new para asignar memoria dinámicamente
}


std::vector<Entry>* AlgoritmoSS(const std::vector<Point>& C_in, int B) {
    Entry result = Entry(Point(0,0), 0.0, nullptr);
    if (C_in.size() <= B) {
        result = OutputHoja(C_in);
    }
    else {
        std::vector<Cluster> C_out = makeClusters(C_in, B);
        std::vector<Entry> C;         
        for (int i = 0; i < C_out.size(); i++) {
            std::vector<Point> C_in_tmp = C_out[i].points;
            Entry tmp = OutputHoja(C_in_tmp);
            C.push_back(tmp);
        }
        while (C.size() > B) {
            //4.1 Sea C_in_2 = {g|(g, r, a) ∈ C}. Sea C_out = Cluster(Cin). Sea Cmra = {}.
            std::vector<Point> C_in_2;
            // Define C_in_2
            for (int i = 0; i < C.size(); i++) {
                C_in_2.push_back(C[i].p);
            }
            // Define C_out
            std::vector<Cluster> C_out = makeClusters(C_in_2, B);
            // Define Cmra
            std::vector<std::vector<Entry>> C_mra;
            // 4.2 Por cada c ∈ C_out: Sea s = {(g, r, a)|(g, r, a) ∈ C ∧ g ∈ c}, se añade s a Cmra
            for (int i = 0; i < C_out.size(); i++) {
                Cluster c = C_out[i];

                for (int j = 0; j < C.size(); j++) {
                    std::vector<Entry> s;
                    Entry temp_entry = C[j];
                    // En python: if C[j].p in c.points:
                    if (std::find(c.points.begin(), c.points.end(), temp_entry.p) != c.points.end()) {
                        s.push_back(temp_entry);
                    }
                    C_mra.push_back(s);

                    //find(v.begin(), v.end(), busqueda) != v.end();
                }
                
            }
            //4.3 Sea C = {}.
            C.clear(); 
            //4.4 Por cada s ∈ Cmra: Añadir OutputInterno(s) a C
            std::cout << C_mra.size() << std::endl;
            for (int i = 0; i < C_mra.size(); i++) {
                std::vector<Entry> s = C_mra[i];
                Entry temp = OutputInterno(s);
                std::cout << "Tamaño de 'a' de cada elemento de Cmra: " << temp.child_page->size() << std::endl;
                C.push_back(temp);
            }
        }
        //5. Sea (g, r, a) = OutputInterno(C)
        result = OutputInterno(C);
    }
    //6. Se retorna a
    //imprime el tamaño de a
    std::cout << "Tamaño de a: " << result.child_page->size() << std::endl;
    std::cout << "r dentro de SSalgorithm " << result.cr << std::endl;
    return result.child_page;
}

// int main(){
//     int n = 150; // Número de puntos a generar
//     int B = 128; // Tamaño máximo de un cluster
//     double min_val = 0.0; // Valor mínimo en el rango
//     double max_val = 1.0; // Valor máximo en el rango

//     std::vector<Point> points = generate_random_points(n, min_val, max_val);

//     // // Imprimir los puntos generados
//     // std::cout << "Puntos generados:" << std::endl;
//     // for (const auto& point : points) {
//     //     std::cout << "(" << point.x << ", " << point.y << ")" << std::endl;
//     // }

//     // std::vector<Cluster> clusters = makeClusters(points, B);

//     // // Imprimir los clusters generados
//     // std::cout << "Clusters generados:" << std::endl;
//     // int i = 1;
//     // for (const auto& cluster : clusters) {
//     //     std::cout << "Cluster " << i++ << ":" << std::endl;
//     //     std::cout << "Medoid: (" << cluster.medoid.x << ", " << cluster.medoid.y << ")" << std::endl;
//     //     // tamaño del cluster
//     //     std::cout << "Tamaño del cluster: " << cluster.points.size() << std::endl;
//     //     std::cout << "Radio: " << cluster.radius() << std::endl;
//     // }

//     // probar minmax_split  
//     std::pair<Cluster, Cluster> bestClusters = minmax_split(points); 
//     // tamaño de los clusters
//     std::cout << "Tamaño de los clusters: " << bestClusters.first.points.size() << " y " << bestClusters.second.points.size() << std::endl;
    

// }


int main() {
    int n = 10; // Número de puntos a generar
    double min_val = 0.0; // Valor mínimo en el rango
    double max_val = 1.0; // Valor máximo en el rango

    std::vector<Point> points = generate_random_points(n, min_val, max_val);

    int B = 128; // Tamaño máximo de un cluster
    
    // Entry result = OutputHoja(points);
    // // Imprimir tupla (g, r, a)
    // std::cout << "Medoide: (" << result.p.x << ", " << result.p.y << ")" << std::endl;
    // std::cout << "Radio: " << result.cr << std::endl;
    // // direccion en memoria de a
    // std::cout << "Direccion de memoria de a: " << result.child_page << std::endl;
    
    // std::vector<Entry>* a = result.child_page;
    // // imprimir el tamaño del vector
    // for (int i = 0; i < a->size(); i++){
    //     std::cout << "(" << (*a)[i].p.x << ", " << (*a)[i].p.y << ")" << std::endl;
    //     std::cout << "Direccion de memoria de a: " << (*a)[i].child_page << std::endl;
    //     std::cout << "Radio: " << (*a)[i].cr << std::endl;
    // }

    // // testing OutputInterno
    // std::vector<Entry> results_outputHoja;
    // for(int i = 0; i < n; i++) {
    //     std::vector<Point> points_2 = generate_random_points(n, min_val, max_val);
    //     Entry temp_result = OutputHoja(points_2);
    //     results_outputHoja.push_back(temp_result);
    // }
    // Entry result_2 = OutputInterno(results_outputHoja);
    // // Imprimir tupla (G, R, A)
    // std::cout << "Medoide: (" << result_2.p.x << ", " << result_2.p.y << ")" << std::endl;
    // std::cout << "Radio: " << result_2.cr << std::endl;
    // // direccion en memoria de A
    // std::cout << "Direccion de memoria de A: " << result_2.child_page << std::endl;

    // std::vector<Entry>* A = result_2.child_page;

    // // imprimir el tamaño del vector
    // for (int i = 0; i < A->size(); i++){
    //     std::cout << "(" << (*A)[i].p.x << ", " << (*A)[i].p.y << ")" << std::endl;
    //     std::cout << "Direccion de memoria de A: " << (*A)[i].child_page << std::endl;
    //     std::cout << "Radio: " << (*A)[i].cr << std::endl;
    // }

    //testing algoritmoSS
    // std::vector<Point> points_3 = generate_random_points(n, min_val, max_val);
    // std::vector<Entry>* resultado = AlgoritmoSS(points_3, B);

    // std::cout << "TEST ALGORITMO N <= B"<< std::endl;
    // for (int i = 0; i < resultado->size(); i++){
    //     std::cout << "(" << (*resultado)[i].p.x << ", " << (*resultado)[i].p.y << ")" << std::endl;
    //     std::cout << "Direccion de memoria de A: " << (*resultado)[i].child_page << std::endl;
    //     std::cout << "Radio: " << (*resultado)[i].cr << std::endl;
    // }


    std::cout << "TEST ALGORITMO N > B"<< std::endl;
    int n_2 = 250;
    std::vector<Point> points_4 = generate_random_points(n_2, min_val, max_val);
    std::vector<Entry>* resultado_2 = AlgoritmoSS(points_4, B);
    std::cout << "size del resultado: " << resultado_2->size() << std::endl;

 

    for (int i = 0; i < resultado_2->size(); i++){
         std::cout << "(" << (*resultado_2)[i].p.x << ", " << (*resultado_2)[i].p.y << ")" << std::endl;
         std::cout << "Direccion de memoria de A: " << (*resultado_2)[i].child_page << std::endl;
         std::cout << "Radio: " << (*resultado_2)[i].cr << std::endl;
    }

    // imprimir tamaño de los hijos de resultado_2
    for (int i = 0; i < resultado_2->size(); i++){
        std::cout << "Tamaño de los hijos de resultado_2: " << (*resultado_2)[i].child_page->size() << std::endl;
    }

    // defino el primer hijo de resultado_2
    std::vector<Entry>* hijo_1 = (*resultado_2)[0].child_page;
    // imprimir tamaño de los hijos de hijo_1
    for (int i = 0; i < hijo_1->size(); i++){
        std::cout << "Punto: " << (*hijo_1)[i].p.x << (*hijo_1)[i].p.y << std::endl;
    }
    


    return 0;
}