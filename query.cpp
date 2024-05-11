#include "sexton-swinbank.cpp"


/*
La búsqueda tiene como input el M-Tree (T) donde se buscará una query Q = (q, r), donde q es
un punto y r es el radio de búsqueda. Es decir, (q, r) definen una bola. El objetivo es encontrar todos
los puntos de T que residen dentro de esta.
*/
std::pair<std::vector<Point>, int> search(const std::vector<Entry>* T, const Point& q, double r, int& counter) {
    std::pair<std::vector<Point>, int> result;
    std::vector<Point> points;
    //Para realizar la búsqueda, se verifica desde la raíz cada nodo hijo de esta:
    //Si el nodo es una hoja, se verifica para cada entrada si p cumple con dist(p, q) ≤ r. Si es así, se
    //agrega p a la respuesta.
    if ((*T)[0].cr == 0) {
        counter++;
        for (int i = 0; i < T->size(); i++) {
            if (euclidean_distance((*T)[i].p, q) <= r) {
                points.push_back((*T)[i].p);
            }
        }
    }
    //Si el nodo es interno (ver Figura 2), se verifica para cada entrada (p, cr, a) si dist(p, q) ≤ cr + r.
    //Si es así, se busca en su hijo a posibles respuestas. Si no se cumple esa desigualdad, se descarta 
    else {
        counter++;
        for (int i = 0; i < T->size(); i++) {
            if (euclidean_distance((*T)[i].p, q) <= r + (*T)[i].cr) {
                std::pair<std::vector<Point>, int> partial_result_total = search((*T)[i].child_page, q, r, counter);
                std::vector<Point> partial_result = partial_result_total.first;
                points.insert(points.end(), partial_result.begin(), partial_result.end());
            }
        }
    }
    //Se retorna la respuesta y el número de nodos visitados
    result.first = points;
    result.second = counter;
    return result;
}