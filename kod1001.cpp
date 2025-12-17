#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <locale.h>
#include <queue>
#include <limits.h>
using namespace std;

queue<int> q;

void BFSD(int s, int n, int* dis, int** G, int* ex) {
    int current;
        // Инициализация расстояний
    for (int i = 0; i < n; i++) {
        dis[i] = INT_MAX;
    }
    dis[s] = 0;

    q.push(s);

    while (!q.empty()) {
        current = q.front();
        q.pop();

        for (int i = 0; i < n; i++) {
            if (G[current][i] != 0) {
                // Новое расстояние через текущую вершину
                int new_dist = dis[current] + G[current][i];

                // Если нашли более короткий путь
                if (new_dist < dis[i]) {
                    dis[i] = new_dist;
                    q.push(i);
                }
            }
        }
    }
        // Находим эксцентриситет (максимальное расстояние от вершины s)
    ex[s] = 0;
    for (int i = 0; i < n; i++) {
        if (dis[i] != INT_MAX && dis[i] > ex[s] && i != s) {
            ex[s] = dis[i];
        }
    }
        // Если вершина изолирована (нет достижимых вершин кроме себя)
    bool has_reachable = false;
    for (int i = 0; i < n; i++) {
        if (i != s && dis[i] != INT_MAX) {
            has_reachable = true;
            break;
        }
    }
        if (!has_reachable && n > 1) {
        ex[s] = INT_MAX;
    }
}
int main() {
    int n;
    int R = INT_MAX, D = 0;
    srand(time(NULL));
    setlocale(LC_ALL, "Russian");
        // Корректность ввода количества вершин
    printf("Введите количество вершин для графа: ");
    while (1) {
        int result = scanf_s("%d", &n);
        if (result != 1 || n <= 0) {
            printf("Ошибка! Введите целое положительное число: ");
            while (getchar() != '\n');
        }
        else if (n > 100) {
            printf("Слишком большое число! Введите число не более 100: ");
            while (getchar() != '\n');
        }
        else {
            break;
        }
    }
        // Выделение памяти
    int** G = new int* [n];
    int* dis = new int[n];
    int* ex = new int[n];

    for (int i = 0; i < n; i++) {
        G[i] = new int[n];
        ex[i] = 0;
    }
        // Заполнение матрицы смежности (взвешенный граф)
    printf("\nГенерация взвешенного графа...\n");
    for (int i = 0; i < n; i++) {
        for (int j = i; j < n; j++) {
            if (i == j) {
                G[i][j] = 0;
            }
            else {
                // Вероятность создания ребра - 40%
                if (rand() % 100 < 40) {
                    G[i][j] = rand() % 9 + 1;  // Вес от 1 до 9
                }
                else {
                    G[i][j] = 0;
                }
                G[j][i] = G[i][j];
            }
        }
    }
        // Гарантируем связность графа
    for (int i = 1; i < n; i++) {
        bool connected = false;
        for (int j = 0; j < i; j++) {
            if (G[i][j] != 0) {
                connected = true;
                break;
            }
        }
        if (!connected) {
            int j = rand() % i;
            G[i][j] = rand() % 9 + 1;
            G[j][i] = G[i][j];
        }
    }
        // Вывод матрицы смежности
    printf("\nМатрица смежности взвешенного графа:\n");
    printf("     ");
    for (int j = 0; j < n; j++) {
        printf("%3d ", j);
    }
    printf("\n");

    for (int i = 0; i < n; i++) {
        printf("%3d: ", i);
        for (int j = 0; j < n; j++) {
            printf("%3d ", G[i][j]);
        }
        printf("\n");
    }
        // Вычисление расстояний и эксцентриситетов
    printf("\nВычисление расстояний и эксцентриситетов:\n");
    for (int i = 0; i < n; i++) {
        BFSD(i, n, dis, G, ex);

        printf("Вершина %d: расстояния = [", i);
        for (int j = 0; j < n; j++) {
            if (dis[j] == INT_MAX) {
                printf(" INF");
            }
            else {
                printf("%4d", dis[j]);
            }
            if (j < n - 1) printf(", ");
        }
        printf(" ]");

        if (ex[i] == INT_MAX) {
            printf("  эксцентриситет = INF\n");
        }
        else {
            printf("  эксцентриситет = %d\n", ex[i]);
        }
    }

    // Вывод эксцентриситетов и вычисление радиуса/диаметра
    printf("\nЭксцентриситеты вершин:\n");
    for (int i = 0; i < n; i++) {
        if (ex[i] == INT_MAX) {
            printf("Вершина %d: INF (изолирована)\n", i);
        }
        else {
            printf("Вершина %d: %d\n", i, ex[i]);
            if (ex[i] > D) D = ex[i];
            if (ex[i] < R) R = ex[i];
        }
    }
        printf("\n");
    if (R == INT_MAX || D == 0) {
        printf("Граф несвязный! Нельзя вычислить радиус и диаметр.\n");
    }
    else {
        printf("Диаметр графа = %d\n", D);
        printf("Радиус графа = %d\n", R);

        // Поиск центральных и периферийных вершин
        printf("\nЦентральные вершины (эксцентриситет = радиусу = %d):\n", R);
        for (int i = 0; i < n; i++) {
            if (ex[i] == R) {
                printf("%d ", i);
            }
        }

        printf("\n\nПериферийные вершины (эксцентриситет = диаметру = %d):\n", D);
        for (int i = 0; i < n; i++) {
            if (ex[i] == D) {
                printf("%d ", i);
            }
        }
        printf("\n");
    }
        // Проверка расстояния между случайными вершинами
    printf("\n--- Проверка расстояний ---\n");
    int v1 = rand() % n;
    int v2 = rand() % n;
    while (v2 == v1) v2 = rand() % n;

    BFSD(v1, n, dis, G, ex);

    if (dis[v2] == INT_MAX) {
        printf("Нет пути от вершины %d до вершины %d\n", v1, v2);
    }
    else {
        printf("Кратчайшее расстояние от вершины %d до вершины %d = %d\n", v1, v2, dis[v2]);
    }
        // Освобождение памяти
    for (int i = 0; i < n; i++) {
        delete[] G[i];
    }
    delete[] G;
    delete[] dis;
    delete[] ex;

    return 0;
}