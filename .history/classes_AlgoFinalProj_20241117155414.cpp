#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <climits> // For INT_MAX
#include <cstdlib> // For rand()
#include <ctime>   // For srand()

using namespace std;

struct edge {
    int end;
    int Basetime;
    int time;
    int variation = 4;

    void ChangeTraffic() {
        time = Basetime + rand() % variation + 1;
    }
};

class City {
public:
    vector<edge> edges = {};
    string name;
    void add_edge(int end, int time) {
        edge New;
        New.end = end;
        New.Basetime = time;
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

    void add_edge(string start, string end, int time) {
        cities[cities_dictionary[start]].add_edge(cities_dictionary[end], time);
        cities[cities_dictionary[end]].add_edge(cities_dictionary[start], time);
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

    void dijk(int source) {
        vector<int> finalized(city_count, 0);
        vector<int> times(city_count, INT_MAX);
        times[source] = 0;

        int current;
        for (int i = 0; i < city_count; i++) {
            current = findMin(finalized, times);
            if (current == -1) break; // All reachable nodes are finalized
            finalized[current] = 1;

            for (size_t x = 0; x < cities[current].edges.size(); x++) {
                if (!finalized[cities[current].edges[x].end]) {
                    times[cities[current].edges[x].end] = min(times[cities[current].edges[x].end], times[current] + cities[current].edges[x].time);
                }
            }
        }

        for (int i = 0; i < city_count; i++)
            cout << i << " \t\t\t\t" << times[i] << endl;
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

    USA.add_edge("Boston", "Chicago", 15);
    USA.add_edge("Chicago", "Portland", 8);
    USA.add_edge("Boston", "Portland", 4);
    USA.add_edge("Chicago", "Keene", 10);
    USA.add_edge("Cambridge", "Portland", 3);
    USA.dijk(0);

    return 0;
}
