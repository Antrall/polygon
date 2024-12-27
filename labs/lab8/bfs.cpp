#include "graph.cpp"
#include <queue>


/** Алгоритм поиска в ширину **/
pair<double, vector<Node>> bfs(Graph &graph, Node st_node, Node fin_node) {
    // список вершин и рёбер графа
    map<Node, list<pair<Node, double>>> nodes_list = graph.nodes_list;
    // очередь - вершины и их расстояния
    queue<pair<Node, double>> queue;
    queue.push({st_node, 0.0});
    // посещённые вершины
    map<Node, bool> used;
    used[st_node] = true;
    // родительские вершины
    map<Node, Node> parents;
    parents[st_node] = st_node;
    // расстояния до вершин
    map<Node, double> distance;
    distance[st_node] = 0.0;

    // основной цикл поиска
    while (!queue.empty()) {
        // берём данные о текущей вершине из очереди, саму её из очереди удаляем
        Node curr_node = queue.front().first;
        double curr_dist = queue.front().second;
        queue.pop();
        
        // если текущая вершина - целевая, то строим путь до неё
        if (curr_node == fin_node) {
            vector<Node> way;

            // перебираем список родителей, и для каждой вершины берём предыдущую
            for (Node way_node = fin_node; way_node != st_node; way_node = parents[way_node]) {
                way.push_back(way_node);
            }

            // в конце добавляем стартовую и переворачиваем, чтобы путь был от стартовой к целевой
            way.push_back(st_node);
            reverse(way.begin(), way.end());
            // возвращаем длину пути и сам путь
            return {curr_dist, way};
        }

        // если текущая вершина - не целевая, смотрим всех на её соседей
        for (const auto& child_data : nodes_list[curr_node]) {
            Node child = child_data.first;
            double weight = child_data.second;

            // если сосед ещё не просмотрен, обновляем данные о нём
            if (!used[child]) {
                used[child] = true;
                distance[child] = weight + curr_dist;
                parents[child] = curr_node;
                // добавляем соседа в очередь
                queue.push({child, weight + curr_dist});
            }
        }
    }
    // если пути между вершинами нет, возвращаем inf и пустой путь
    return {numeric_limits<double>::infinity(), {}};
}


// функция для вывода теста
void run_bfs(Graph &graph, Node start, Node finish) {
    auto start_time = chrono::high_resolution_clock::now();
    auto result = bfs(graph, start, finish);
    auto end_time = chrono::high_resolution_clock::now();
 
    double duration = chrono::duration_cast<chrono::milliseconds>(end_time - start_time).count() / 1000.0;
    cout << "BFS result: " << result.first << endl;
    cout << "BFS path length (edges in path): " << result.second.size() - 1 << endl;
    //graph.print_path(result.second);
    cout << "BFS time: " << duration << " seconds" << endl;
}


int main() {
    Graph mini_graph;
    mini_graph.get_file_data("tst.txt");
    Node start = mini_graph.find_closest_node(0,0);

    // Тест для двух вершин в маленьком графе
    Node end1 = mini_graph.find_closest_node(1,4);
    run_bfs(mini_graph, start, end1);
    cout << "\n";

    // Тест для тех вершин, между которыми пути нет
    Node end2 = mini_graph.find_closest_node(1,16);
    run_bfs(mini_graph, start, end2);
    cout << "\n";
    
    // Тест на графе Петербурга
    Graph graph;
    graph.get_file_data("spb_graph.txt");
    Node my_node = graph.find_closest_node(30.455366, 59.900638);
    Node itmo_node = graph.find_closest_node(30.309514, 59.956184);
    run_bfs(graph, my_node, itmo_node);

    return 0;
}