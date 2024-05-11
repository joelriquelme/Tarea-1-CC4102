#include "query.cpp"

int main() {
    int n = 10; // Número de puntos a generar
    int B = 128; // Máximo numero de entradas en un nodo
    double min_val = 0.0; // Valor mínimo en el rango
    double max_val = 1.0; // Valor máximo en el rango

    std::vector<Point> points = generate_random_points(n, min_val, max_val);

    //testing algoritmoSS

    //testing algoritmoSS con N <= B
    std::cout << "======================================================"<< std::endl;
    std::cout << "CONSTRUCCIÓN CON ALGORITMO SS (N <= B)"<< std::endl;
    std::cout << "======================================================"<< std::endl;
    std::cout << "Cantidad de puntos:  " << n << std::endl;
    std::cout << "" << std::endl;

    std::vector<Entry>* resultado = AlgoritmoSS(points, B);
    std::cout << "" << std::endl;
    std::cout << "Tamaño del nodo: " << resultado->size() << std::endl;
    std::cout << ""  << std::endl;
    std::cout << "-----------Entradas del nodo primario:-----------"<< std::endl;
    std::cout << "" << std::endl;
    for (int i = 0; i < resultado->size(); i++){
        std::cout << "(" << (*resultado)[i].p.x << ", " << (*resultado)[i].p.y << ")" << std::endl;
        std::cout << "Direccion de memoria de A: " << (*resultado)[i].child_page << std::endl;
        std::cout << "Radio: " << (*resultado)[i].cr << std::endl;
    }
    
    //testing algoritmoSS con N > B
    std::cout << "" << std::endl;
    std::cout << "======================================================"<< std::endl;
    std::cout << "CONSTRUCCIÓN CON ALGORITMO SS (N > B)"<< std::endl;
    std::cout << "======================================================"<< std::endl;
    std::cout << "Ingresa el exponente de 2: " << std::endl;
    int exponente;
    std::cin >> exponente;

    n = pow(2, exponente);
    std::cout << "Cantidad de puntos: " << n << std::endl;
    
    points = generate_random_points(n, min_val, max_val);
    resultado = AlgoritmoSS(points, B);

    std::cout << "" << std::endl;
    std::cout << "Tamaño del primer nodo: " << resultado->size() << std::endl;
    std::cout << ""  << std::endl;
    std::cout << "-----------Entradas del nodo primario:-----------"<< std::endl;
    std::cout << "" << std::endl;
    for (int i = 0; i < resultado->size(); i++){
        std::cout << "Entrada número " << i << std::endl;
        std::cout << "Medoide de la entrada " << " (" << (*resultado)[i].p.x << ", " << (*resultado)[i].p.y << ")" << std::endl;
        std::cout << "Direccion de memoria del hijo: " << (*resultado)[i].child_page << std::endl;
        std::cout << "Radio: " << (*resultado)[i].cr << std::endl;
    }

    //definior el hijo del primer nodo como el nodo hijo
    std::vector<Entry>* hijo = (*resultado)[0].child_page;
    std::cout << "" << std::endl;
    std::cout << "-----------Algunos puntos del hijo de la primera entrada:-----------"<< std::endl;
    std::cout << "" << std::endl;
    for (int i = 0; i < hijo->size(); i += 15){
        std::cout << "(" << (*hijo)[i].p.x << ", " << (*hijo)[i].p.y << ")" << std::endl;
    }

    //hacer query con un punto aleatorio
    std::cout << "" << std::endl;
    std::cout << "======================================================"<< std::endl;
    std::cout << "HACER BUSQUEDA EN M-TREE GENERADO"<< std::endl;
    std::cout << "======================================================"<< std::endl;
    std::cout << "" << std::endl;
    Point q = Point(0.5, 0.5);
    std::cout << "Punto de la query: (" << q.x << ", " << q.y << ")" << std::endl;
    double r = 0.02;
    std::cout << "Radio de la query: " << r << std::endl;

    

    std::cout << "" << std::endl;
    std::cout << "Realizando query..." << std::endl;

    
    std::cout << "" << std::endl;
    std::cout << "-----------Resultados de la querys:-----------"<< std::endl;
    std::cout << "" << std::endl;

    //hacer 100 querys
    std::vector<Point> points_query = generate_random_points(100, min_val, max_val);
    for (int i = 0; i < 100; i++){
        int counter = 0;
        std::pair<std::vector<Point>, int> result = search(resultado, points_query[i], r, counter);
        std::cout << result.second << std::endl;
    }

    return 0;
}