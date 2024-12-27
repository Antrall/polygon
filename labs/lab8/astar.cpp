#include "graph.cpp"
#include <queue>


/** Подсчёт эвристики как евклидового расстояния между вершинами **/
double heuristic(Node node1, Node node2) {
    return sqrt(pow(node1.first - node2.first, 2) + pow(node1.second - node2.second, 2));
}


/** Алгоритм A* для поиска min расстояния от вершины до вершины **/
pair<double, vector<Node>> astar(Graph &graph, Node start_node, Node finish_node) {
    // список вершин и рёбер графа
    map<Node, list<pair<Node, double>>> nodes_list = graph.nodes_list;

    // gscore и hscore, hscore[n] = gscore[n] + heuristic
    map<Node, double> gscore;
    map<Node, double> hscore;
    for (const auto& node_data : nodes_list) {
        gscore[node_data.first] = numeric_limits<double>::infinity();
        hscore[node_data.first] = numeric_limits<double>::infinity();
    }
    gscore[start_node] = 0.0;
    hscore[start_node] = heuristic(start_node, finish_node);

    // приоритетная очередь: хранится hscore + node, приоритет смотрим по hscore
    priority_queue<pair<double, Node>, vector<pair<double, Node>>, greater<>> pq;
    pq.push({hscore[start_node], start_node});

    // родительские вершины
    map<Node, Node> parents;
    parents[start_node] = start_node;

    // основной цикл А*
    while (!pq.empty()) {
        // берём данные о вершине из приоритетной очереди, саму вершину оттуда удаляем
        double curr_hscore = pq.top().first;
        Node curr_node = pq.top().second;
        pq.pop();

        // если текущая – это целевая, строим путь до неё
        if (curr_node == finish_node) {
            vector<Node> way;

            // перебираем список родителей, и для каждой вершины берём предыдущую
            for (Node way_node = finish_node; way_node != start_node; way_node = parents[way_node]) {
                way.push_back(way_node);
            }

            // в конце добавляем стартовую и переворачиваем, чтобы путь был от стартовой к целевой
            way.push_back(start_node);
            reverse(way.begin(), way.end());
            // возвращаем расстояние от start_node до finish_node (соответствующий gscore) и сам путь 
            return {gscore[curr_node], way};
        }

        // если текущая – ещё не целевая, просматриваем её соседей
        for (const auto& child_data : nodes_list[curr_node]) {
            const Node child = child_data.first;
            double weight = child_data.second;

            // считаем новый gscore
            double new_gscore = gscore[curr_node] + weight;
            // если он оптимальнее текущего, обновляем данные о соседе
            if (new_gscore < gscore[child]) {
                gscore[child] = new_gscore;
                hscore[child] = new_gscore + heuristic(child, finish_node);
                parents[child] = curr_node;
                // тк gscore обновился, добавляем соседа в очередь для просмотра
                pq.push({hscore[child], child});
            }
        }
    }
    // если пути между вершинами не нашлось, возвращаем inf и пустой путь
    return {numeric_limits<double>::infinity(), {}};
}


// функция для вывода теста
void run_astar(Graph &graph, Node start, Node finish) {
    auto start_time = chrono::high_resolution_clock::now();
    auto result = astar(graph, start, finish);
    auto end_time = chrono::high_resolution_clock::now();
 
    double duration = chrono::duration_cast<chrono::milliseconds>(end_time - start_time).count() / 1000.0;
    cout << "A* result: " << result.first << endl;
    cout << "A* path length (edges in path): " << result.second.size() - 1 << endl;
    //graph.print_path(result.second);
    cout << "A* time: " << duration << " seconds" << endl;
}


int main() {
    Graph mini_graph;
    mini_graph.get_file_data("tst.txt");
    Node start = mini_graph.find_closest_node(0,0);

    // Тест для двух вершин в маленьком графе
    Node end1 = mini_graph.find_closest_node(1,4);
    run_astar(mini_graph, start, end1);
    cout << "\n";

    // Тест для тех вершин, между которыми пути нет
    Node end2 = mini_graph.find_closest_node(1,16);
    run_astar(mini_graph, start, end2);
    cout << "\n";
    
    // Тест на графе Петербурга
    Graph graph;
    graph.get_file_data("spb_graph.txt");
    Node my_node = graph.find_closest_node(30.455366, 59.900638);
    Node itmo_node = graph.find_closest_node(30.309514, 59.956184);
    run_astar(graph, my_node, itmo_node);

    return 0;
}