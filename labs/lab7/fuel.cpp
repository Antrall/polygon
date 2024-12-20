#include <iostream>
#include <vector>
#include <queue>

int minRefuelStops(int target, int start_fuel, std::vector<std::vector<int>>& stations) {
    int stations_count = stations.size();
    int curr_fuel = start_fuel;
    std::priority_queue<int> queue;

    int i = 0;
    int result = 0;

    // начинаем ехать и едем, пока хватает топлива
    while(curr_fuel < target) {
        // по пути добавляем в очередь станции, мимо которых проехали
        while(i < stations_count && curr_fuel >= stations[i][0]) {
            queue.push(stations[i][1]);
            i++;
        }
        
        // если топливо кончилось, при этом даже не доехали до станции – то неудача
        if(queue.empty()) {
            return -1;
        }
        
        // если встали и нужна заправка – выбираем станцию из посещенных ранее, где больше всего топлива
        int new_fuel = queue.top();
        // удаляем из очереди, чтобы не учитывать в дальнешем пути
        queue.pop();
        
        // обновляем топливо и число остановок
        curr_fuel += new_fuel;
        result++;
    }
    return result;   
}