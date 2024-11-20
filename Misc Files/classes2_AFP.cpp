#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <limits>

using namespace std;

struct Edge {
    int end;
    int time;
};

class City {
public:
    vector<Edge> edges;
    string name;
    void addEdge(int end, int time) {
        edges.push_back({end, time});
    }
};

class Map {
private:
    vector<City> cities;
public:
    void addCity(const string& name) {
        City newCity;
        newCity.name = name;
        cities.push_back(newCity);
    }

    void addEdge(int start, int end, int time) {
        if (start < cities.size() && end < cities.size()) {
            cities[start].addEdge(end, time);
            cities[end].addEdge(start, time); // Bidirectional edge
        }
    }

    void dijkstra(int source) {
        int cityCount = cities.size();
        vector<int> times(cityCount, numeric_limits<int>::max());
        times[source] = 0;

        using Pair = pair<int, int>; // Pair for (time, cityIndex)
        priority_queue<Pair, vector<Pair>, greater<>> minHeap;
        minHeap.push({0, source});

        vector<bool> finalized(cityCount, false);

        while (!minHeap.empty()) {
            int currentCity = minHeap.top().second;
            int currentTime = minHeap.top().first;
            minHeap.pop();

            if (finalized[currentCity]) continue;
            finalized[currentCity] = true;

            for (const auto& edge : cities[currentCity].edges) {
                int nextCity = edge.end;
                int newTime = currentTime + edge.time;

                if (!finalized[nextCity] && newTime < times[nextCity]) {
                    times[nextCity] = newTime;
                    minHeap.push({newTime, nextCity});
                }
            }
        }

        // Output the shortest times
        cout << "City\tMinimum Time from Source\n";
        for (int i = 0; i < cityCount; i++) {
            cout << cities[i].name << "\t\t" << times[i] << endl;
        }
    }
};

int main() {
    Map USA;
    USA.addCity("Boston");   // Index 0
    USA.addCity("Chicago");  // Index 1
    USA.addCity("Portland"); // Index 2

    USA.addEdge(0, 1, 15);
    USA.addEdge(1, 2, 20);

    USA.dijkstra(0); // Find shortest paths from Boston (index 0)

    return 0;
}
