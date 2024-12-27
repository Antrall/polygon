#include "graph.cpp"
#include <queue>

/** Алгоритм Дейкстры для поиска min расстояния от вершины до вершины **/
pair<double, vector<Node>> dijkstra(Graph &graph, Node start_node, Node finish_node) {
    // список вершин и рёбер графа
    map<Node, list<pair<Node, double>>> nodes_list = graph.nodes_list;

    // приоритетная очередь
    priority_queue<pair<double, Node>, vector<pair<double, Node>>, greater<>> pq;
    pq.push({0.0, start_node});

    // посещённые вершины
    map<Node, bool> used;
    // родительские вершины
    map<Node, Node> parents;
    parents[start_node] = start_node;

    // расстояния от start_node до других вершин, на старте задаём 0 от стартовой до себя, а до остальных – inf
    map<Node, double> distance;
    for (const auto& node_data : nodes_list) {
        distance[node_data.first] = numeric_limits<double>::infinity();
    }
    distance[start_node] = 0.0;

    // основной цикл алгоритма
    while (!pq.empty()) {
        // берём данные о вершине из приоритетной очереди, саму вершину оттуда удаляем
        double curr_dist = pq.top().first;
        Node curr_node = pq.top().second;
        pq.pop();

        // если текущая – это целевая, собираем путь до неё
        if (curr_node == finish_node) {
            vector<Node> way;

            // перебираем список родителей, и для каждой вершины берём предыдущую
            for (Node way_node = finish_node; way_node != start_node; way_node = parents[way_node]) {
                way.push_back(way_node);
            }

            // в конце добавляем стартовую и переворачиваем, чтобы путь был от стартовой к целевой
            way.push_back(start_node);
            reverse(way.begin(), way.end());
            // возвращаем расстояние от start_node до curr_node (finish_node) и сам путь
            return {distance[curr_node], way};
        }

        // если текущая уже была просмотрена, пропускаем, иначе отмечаем как посещённую и перебираем соседей
        if (used[curr_node]) continue;
        used[curr_node] = true;

        for (const auto& child_data : nodes_list[curr_node]) {
            // получаем данные о каждом соседе
            const Node child = child_data.first;
            double weight = child_data.second;

            // считаем новое расстояние как расстояние до текущей + расстояние от неё до соседа
            double new_dist = curr_dist + weight;
            // если новое расстояние оптимальнее, обновляем данные о соседе
            if (new_dist < distance[child]) {
                distance[child] = new_dist;
                parents[child] = curr_node;
                // тк расстояние до соседа обновилось, добавляем в очередь для просмотра
                pq.push({new_dist, child});
            }
        }
    }
    // если целевой вершины достичь нельзя, возвращаем inf и пустой путь
    return {numeric_limits<double>::infinity(), {}};
}


// функция для вывода теста
void run_dijkstra(Graph &graph, Node start, Node finish) {
    auto start_time = chrono::high_resolution_clock::now();
    auto result = dijkstra(graph, start, finish);
    auto end_time = chrono::high_resolution_clock::now();
 
    double duration = chrono::duration_cast<chrono::milliseconds>(end_time - start_time).count() / 1000.0;
    cout << "Dijkstra result: " << result.first << endl;
    cout << "Dijkstra path length (edges in path): " << result.second.size() - 1 << endl;
    //graph.print_path(result.second);
    cout << "Dijkstra time: " << duration << " seconds" << endl;
}


int main() {
    Graph mini_graph;
    mini_graph.get_file_data("tst.txt");
    Node start = mini_graph.find_closest_node(0,0);

    // Тест для двух вершин в маленьком графе
    Node end1 = mini_graph.find_closest_node(1,4);
    run_dijkstra(mini_graph, start, end1);
    cout << "\n";

    // Тест для тех вершин, между которыми пути нет
    Node end2 = mini_graph.find_closest_node(1,16);
    run_dijkstra(mini_graph, start, end2);
    cout << "\n";
    
    // Тест на графе Петербурга
    Graph graph;
    graph.get_file_data("spb_graph.txt");
    Node my_node = graph.find_closest_node(30.455366, 59.900638);
    Node itmo_node = graph.find_closest_node(30.309514, 59.956184);
    run_dijkstra(graph, my_node, itmo_node);

    return 0;
}