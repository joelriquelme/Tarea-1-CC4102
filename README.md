# Diseño y Análisis de Algoritmos
# Tarea 1
**Integrantes**: 
- Alan Chávez V.
- Diego Orellana V.
- Joel Riquelme P.

**Sección**: 1

**Profesor**: Benjamín Bustos

**Profesores Auxiliares**: Sergio Rojas y Diego Salas

**Fecha de Entrega**: 12 de Abril de 2024

## Compilación y Ejecución

Para compilar el archivo `main.cpp`, fue utilizado `g++` con `std=c++17`.

Si quiere compilar y ejecutar el archivo `main.cpp`, ejecute:

```
make run
```

En caso de no tener instalado lo necesario para utilizar el comando `make`, puedo utilizar los siguientes comandos para realizar la misma acción:

```
//Compilar el archivo main.cpp
g++ -std=c++17 main.cpp -o main.out

//Ejecutar
./main.out
```
## Experimento

Una vez ejecutado el código, se le pedirá el que ingrese el exponente $m$.

Luego se construirán los M-Tree's con los distintos metodos con una muestra $P$ de puntos, siendo la cantidad de puntos $|P| = n = 2^m$. Posterior a la construcción de ambos arboles, se procederá a realizar las busquedas con un set $Q$ de 100 puntos elegidos generados aleatoriamente. Destacar que tanto para $P$ como para $Q$, los puntos generados se encuentras uniformemente distribuidos en el rango $[0,1]$.

Las busquedas se realizarán de la forma $\{ (p, r) | \forall p \in Q, r = 0.02\}$ siendo $p$ el punto de busqueda y $r$ el radio de la consulta.

Se mostrarán en pantalla la cantidades de accesos a disco para cada Árbol y para cada query, finalmente se calculará el promedio de estos accesos y su intervalo de confianza asociado.



## Estructuras de Datos  

Todas las estructuras de datos implementadas en este proyecto se encuentran en el archivo `structures.cpp`. A continuación se describen las estructuras implementadas:

### 1. Punto (Point)

Para la implementación de un punto en un plano cartesiano, se utilizó la clase `Point` que contiene dos atributos `x` y `y` de tipo `double` que representan las coordenadas del punto. Además, se implementaron el operador `==` para comparar dos puntos.

### 2. Cluster (Cluster)

La clase `Cluster` representa un conjunto de puntos en un plano cartesiano. Esta clase contiene un vector de puntos llamado `points` y un punto llamado `medoid` que representa el medoid del cluster. Además, se implementaron los métodos `addPoint` para agregar un punto al cluster, `recalculate_Medoid` para calcular el medoide del cluster, `merge` para unir dos clusters y `radius` para el cálculo del radio del cluster.

### 3. Entrada (Entry)

La clase `Entry` representa una tupla `(p, cr, a)`, donde `p` es un punto, `r` es el radio cobertor y `a` una dirección en disco a la página de su hijo. Esta clase contiene los atributos `p`, `cr` y `child_page` de tipo `Point`, `double` y `vector<Entry>*` respectivamente.

## Algoritmos de Construcción

### 2. Algoritmo de Construcción Sexton-Swinbank (SS)

Para la implementación de este algoritmo de construcción se definieron las siguientes funciones:

- `makeClusters`: Esta función recibe un vector de puntos (`vector<Point>`) y un entero `B` que representa la cantidad máxima de puntos que puede tener un nodo. Se retorna un vector de Clusters (`vector<Clusters>`).
- `OutputHoja`: Esta función recibe un vector de puntos (`vector<Point>`). Se retorna una entrada de tipo `Entry` que contiene el medoide de los puntos, el radio cobertor y un puntero que apunta a un vector de entradas, es decir a Hojas (`p, 0.0, nullptr`).
- `OutputNoHoja`: Esta función recibe un vector de entradas (`vector<Entry>`). Se retorna una entrada de tipo `Entry` que contiene el medoide de los todos las entradas, el radio cobertor y un puntero que apunta a un vector de entradas, es decir a un Nodo.

Finalmente se implementó el método Sexton-Swinbank en la función `AlgoritmoSS` que recibe un vector de puntos (`vector<Point>`) y un entero `B` que representa la cantidad máxima de puntos que puede tener un nodo. Este algoritmo retorna un puntero a una entrada de tipo `Entry` que contiene la raíz del árbol.
