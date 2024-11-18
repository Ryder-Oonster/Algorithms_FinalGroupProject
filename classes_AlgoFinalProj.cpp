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

    USA.add_city("Boston");
    USA.add_city("Salem");
    USA.add_city("Burlington");
    USA.add_city("Newton");
    USA.add_city("Norwood");
    USA.add_city("Quincy");
    USA.add_city("Lawrence");
    USA.add_city("Lowell");
    USA.add_city("Marlborough");
    USA.add_city("Mansfield");
    USA.add_city("Plymouth");
    USA.add_city("Littleton");
    USA.add_city("Leominster");
    USA.add_city("Worcester");
    USA.add_city("Providence");
    USA.add_city("Orange");
    USA.add_city("Greenfield");
    USA.add_city("Springfield");


    USA.add_edge("Boston","Salem",5);
    USA.add_edge("Boston","Burlington",5);
    USA.add_edge("Salem","Burlington",4);
    USA.add_edge("Boston","Newton",5);
    USA.add_edge("Burlington","Newton",4);
    USA.add_edge("Boston","Norwood",5);
    USA.add_edge("Newton","Norwood",4);
    USA.add_edge("Boston","Quincy",5);
    USA.add_edge("Norwood","Quincy",4);
    USA.add_edge("Salem","Lawrence",3);
    USA.add_edge("Burlington","Lawrence",4);
    USA.add_edge("Lawrence","Lowell",3);
    USA.add_edge("Burlington","Lowell",4);
    USA.add_edge("Newton","Marlborough",4);
    USA.add_edge("Lowell","Marlborough",3);
    USA.add_edge("Norwood","Mansfield",4);
    USA.add_edge("Marlborough","Mansfield",3);
    USA.add_edge("Quincy","Plymouth",3);
    USA.add_edge("Mansfield","Plymouth",3);
    USA.add_edge("Lowell","Littleton",3);
    USA.add_edge("Marlborough","Littleton",3);
    USA.add_edge("Littleton","Leominster",3);
    USA.add_edge("Leominster","Worcester",2);
    USA.add_edge("Marlborough","Worcester",3);
    USA.add_edge("Worcester","Providence",3);
    USA.add_edge("Mansfield","Providence",4);
    USA.add_edge("Plymouth","Providence",3);
    USA.add_edge("Leominster","Orange",2);
    USA.add_edge("Orange","Greenfield",2);
    USA.add_edge("Orange","Springfield",1);
    USA.add_edge("Greenfield","Springfield",3);
    USA.add_edge("Worcester","Springfield",3);

    USA.dijk(0);

    return 0;
}
