#include <iostream>
#include <vector>
#include <queue>

int minRefuelStops(int target, int start_fuel, std::vector<std::vector<int>>& stations) { // + 4 + 4 + 24 байт
    int stations_count = stations.size(); // +4 байта
    int curr_fuel = start_fuel; // +4 байта
    std::priority_queue<int> queue; // +32 байта

    int i = 0; // +4 байта
    int result = 0; // +4 байта

    // начинаем ехать и едем, пока хватает топлива. O(N), N - число остановок, тк каждый раз цикл работает до следующей остановки
    while(curr_fuel < target) {
        // по пути добавляем в очередь станции, мимо которых проехали. O(M), M - колво станций, мимо которых проезжаем
        while(i < stations_count && curr_fuel >= stations[i][0]) {
            queue.push(stations[i][1]); // +4 байта. O(logK), K - колво элементов в очереди,  K <= M
            i++;
        }
        
        // если топливо кончилось, при этом даже не доехали до станции – то неудача
        if (queue.empty()) {
            return -1;
        }
        
        // если встали и нужна заправка – выбираем станцию из посещенных ранее, где больше всего топлива
        int new_fuel = queue.top(); // +4 байта
        // удаляем из очереди, чтобы не учитывать в дальнешем пути
        queue.pop(); // O(logK)
        
        // обновляем топливо и число остановок
        curr_fuel += new_fuel;
        result++;
    }
    return result;   
}
// Итог по временной сложности: до O(N * M), N - число остановок, M - число станций