#include <iostream>
#include <vector>
#include <list>
#include <map>
#include <fstream>
#include <sstream>
#include <cmath>
#include <algorithm>
#include <chrono>

using namespace std;
using Node = pair<double, double>;


// структура графа
struct Graph {
    // сами вершины задаём парами lon-lat, граф задаём списком смежности
    map<Node, list<pair<Node, double>>> nodes_list;

    // добавление ребра в общий список
    void add_edge(double lon_parent, double lat_parent, double lon_child, double lat_child, double weight) {
        nodes_list[{lon_parent, lat_parent}].push_back({{lon_child, lat_child}, weight});
    }

    // нахождение точки, ближайшей к заданным координатам. Алгоритм как в гугл таблице
    Node find_closest_node(double lon, double lat) {
        Node node_founded;
        double min_distance = pow(10, 9);

        // Перебираем все вершины, считаем для них евклидово расстояние до заданной точки
        for (const auto &node: nodes_list) {
            double distance = sqrt(pow(node.first.first - lon, 2) + pow(node.first.second - lat, 2));
            // При необходимости обновляем данные о ближайшей вершине
            if (distance < min_distance) {
                min_distance = distance;
                node_founded = node.first;
            }
        }

        return node_founded;
    }
    
    // получение графа из файла
    void get_file_data(const string &filename) {
        ifstream file(filename);
        string line;
        
        while (getline(file, line)) {
            stringstream line_stream(line);
            string node_info;
            getline(line_stream, node_info, ':');
 
            stringstream node_stream(node_info);
            string node_coords;

            getline(node_stream, node_coords, ',');
            double lon1 = stod(node_coords);
            getline(node_stream, node_coords);
            double lat1 = stod(node_coords);

            string edges_info;
            while (getline(line_stream, edges_info, ';')) {
                stringstream edge_stream(edges_info);
                string child_info;
 
                getline(edge_stream, child_info, ',');
                double lon2 = stod(child_info);

                getline(edge_stream, child_info, ',');
                double lat2 = stod(child_info);

                getline(edge_stream, child_info, ',');
                double weight = stod(child_info);
                
                // деляем граф неориентированным, иначе он получается не полностью связным
                add_edge(lon1, lat1, lon2, lat2, weight);
                add_edge(lon2, lat2, lon1, lat1, weight);
            }
        }
        file.close();
    }

    // вывод пути между вершинами в графе
    void print_path(vector<Node> path) {
        if (path.empty()) {
            cout << "The path was not found." << endl;
            return;
        }

        cout << "Your path: ";
        for (int i = 0; i < path.size() - 1; i++) {
            cout << "(" << path[i].first << ";" << path[i].second << "), ";
        }
        cout << "(" << path[path.size()-1].first << ";" << path[path.size()-1].second << ")" << endl;
    }
};
