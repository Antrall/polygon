#include "graph.cpp"
#include <stack>


/** Алгоритм поиска в глубину без рекурсии через стек **/
pair<double, vector<Node>> dfs(Graph &graph, Node start_node, Node finish_node) {
    // общий списочек
    map<Node, list<pair<Node, double>>> nodes_list = graph.nodes_list;
    // стек для хранения вершин и расстояний
    stack<pair<Node, double>> stack;
    stack.push({start_node, 0.0});
    // посещённые вершины
    map<Node, bool> used;
    used[start_node] = true;
    // родительские вершины
    map<Node, Node> parents;
    parents[start_node] = start_node;
    // расстояния до вершин
    map<Node, double> distance;
    distance[start_node] = 0.0;
    
    // основной цикл поиска
    while (!stack.empty()) {
        // берём данные о вершине из стека, саму вершину из стека удаляем
        Node curr_node = stack.top().first;
        double curr_dist = stack.top().second;
        stack.pop();

        // если текущая == целевая, собираем путь до неё
        if (curr_node == finish_node) {
            vector<Node> way;

            // перебираем список родителей, и для каждой вершины берём предыдущую
            for (Node way_node = finish_node; way_node != start_node; way_node = parents[way_node]) {
                way.push_back(way_node);
            }

            // в конце добавляем стартовую и переворачиваем, чтобы путь был от стартовой к целевой
            way.push_back(start_node);
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
                // добавляем соседа в очереди
                stack.push({child, weight + curr_dist});
            }
        }
    }
    // если пути между вершинами нет, возвращаем inf и пустой путь
    return {numeric_limits<double>::infinity(), {}};
}


// функция для вывода теста
void run_dfs(Graph &graph, Node start, Node finish) {
    auto start_time = chrono::high_resolution_clock::now();
    auto result = dfs(graph, start, finish);
    auto end_time = chrono::high_resolution_clock::now();
 
    double duration = chrono::duration_cast<chrono::milliseconds>(end_time - start_time).count() / 1000.0;
    cout << "DFS result: " << result.first << endl;
    cout << "DFS path length (edges in path): " << result.second.size() - 1 << endl;
    //graph.print_path(result.second);
    cout << "DFS time: " << duration << " seconds" << endl;
}


int main() {
    Graph mini_graph;
    mini_graph.get_file_data("tst.txt");
    Node start = mini_graph.find_closest_node(0,0);

    // Тест для двух вершин в маленьком графе
    Node end1 = mini_graph.find_closest_node(1,4);
    run_dfs(mini_graph, start, end1);
    cout << "\n";

    // Тест для тех вершин, между которыми пути нет
    Node end2 = mini_graph.find_closest_node(1,16);
    run_dfs(mini_graph, start, end2);
    cout << "\n";
    
    // Тест на графе Петербурга
    Graph graph;
    graph.get_file_data("spb_graph.txt");
    Node my_node = graph.find_closest_node(30.455366, 59.900638);
    Node itmo_node = graph.find_closest_node(30.309514, 59.956184);
    run_dfs(graph, my_node, itmo_node);

    return 0;
}