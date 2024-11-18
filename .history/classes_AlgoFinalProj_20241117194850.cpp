#include <algorithm>
#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <climits> // For INT_MAX
#include <cstdlib> // For rand()
#include <ctime>   // For srand()
#include <fstream> // For .dot graph file generation

using namespace std;

struct edge {
    int end;
    int Basetime;
    int time;
    int variation;
    void ChangeTraffic() {
        time = Basetime + (rand() % variation) + 1;
    }
};

class City {
public:
    vector<edge> edges = {};
    string name;
    void add_edge(int end, int time, int variation) {
        edge New;
        New.end = end;
        New.Basetime = time;
        New.variation = variation;
        New.ChangeTraffic();
        edges.push_back(New);
    }
};

class World {
private:
    vector<City> cities;
    map<string, int> cities_dictionary;
    int city_count;

public:
    World() {
        city_count = 0;
    }

    void add_city(string name) {
        City New;
        New.name = name;
        cities.push_back(New);
        cities_dictionary[name] = city_count;
        city_count += 1;
    }

    void add_edge(string start, string end, int time, int variation) {
        cities[cities_dictionary[start]].add_edge(cities_dictionary[end], time, variation);
        cities[cities_dictionary[end]].add_edge(cities_dictionary[start], time, variation);
    }

    int findMin(const vector<int>& finalized, const vector<int>& times) {
        int minIndex = -1;
        int minTime = INT_MAX;
        for (int i = 0; i < city_count; i++) {
            if (!finalized[i] && times[i] < minTime) {
                minTime = times[i];
                minIndex = i;
            }
        }
        return minIndex;
    }

    vector<int> dijk(int source, int destination) {
        vector<int> finalized(city_count, 0);
        vector<int> times(city_count, INT_MAX);
        vector<int> parent(city_count, -1);

        times[source] = 0;

        int current;
        for (int i = 0; i < city_count; i++) {
            current = findMin(finalized, times);
            if (current == -1) break; // All reachable nodes are finalized
            finalized[current] = 1;

            for (size_t x = 0; x < cities[current].edges.size(); x++) {
                if (!finalized[cities[current].edges[x].end] && times[cities[current].edges[x].end] > times[current] + cities[current].edges[x].time) {
                    times[cities[current].edges[x].end] = times[current] + cities[current].edges[x].time;
                    parent[cities[current].edges[x].end] = current;
                }
            }
        }

        for (int i = 0; i < city_count; i++) {
            cout << i << " \t\t\t\t" << times[i] << endl;
        }
        vector<int> path;
        for (int postition = destination; postition != -1; postition = parent[postition] ) {
            path.push_back(postition);
        }
        reverse(path.begin(), path.end());
        return path;
    }

    // Add this function to output the graph as a .dot file
void outputGraphToDotFile(const vector<City>& cities, const vector<int>& path) {
    std::ofstream dotFile("graph.dot");
    dotFile << "graph DijkstraGraph {\n";

    // Define the edges in the graph
    for (size_t i = 0; i < cities.size(); ++i) {
        for (const auto& edge : cities[i].edges) {
            dotFile << "  " << i << " -- " << edge.end << " [label=\"" << edge.time << "\"];\n";
        }
    }

    // Highlight the optimal path
    dotFile << "\n  // Highlight the optimal path\n";
    for (size_t i = 1; i < path.size(); ++i) {
        dotFile << "  " << path[i - 1] << " -- " << path[i] << " [color=red, penwidth=2.0];\n";
    }

    dotFile << "}\n";
}


};

int main() {
    srand(static_cast<unsigned int>(time(0)));
    World USA;
    USA.add_city("Boston");   // 0
    USA.add_city("Chicago");  // 1
    USA.add_city("Portland"); // 2
    USA.add_city("Keene");    // 3
    USA.add_city("Cambridge");// 4

    USA.add_edge("Boston", "Chicago", 30, 8);
    USA.add_edge("Boston", "Chicago", 40, 1);
    USA.add_edge("Chicago", "Portland", 28,1);
    USA.add_edge("Boston", "Portland", 4,1);
    USA.add_edge("Chicago", "Keene", 10,1);
    USA.add_edge("Cambridge", "Portland", 3,1);
    vector<int> record = USA.dijk(0,3);

    for (size_t i = 0; i < record.size(); i++) {
        cout << record[i];
        if (i < record.size() - 1) cout << " -> ";
    }

    return 0;
}
