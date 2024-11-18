#include <algorithm>
#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <climits> // For INT_MAX
#include <cstdlib> // For rand()
#include <ctime>   // For srand()
#include <math.h>
#include <fstream> // For .dot graph file generation

using namespace std;
double hour;
double trafficCalc() {
    if (hour >=5.5 && hour <=10.5) {
     return (0.277778*(pow(hour, 3))-10.2976*(pow(hour,2))+108.948*hour-325.238);
    }
    else if (hour >= 12.5 && hour <=20.5) {
        return 0.353535*(pow(hour,3))-23.5119*(pow(hour,2))+482.682*hour-3045.95;
    }
    else {
        return 0;
    }
}


struct edge {
    int end;
    int Basetime;
    int time;
    int variation;
    void ChangeTraffic() {
        time = Basetime + (1+trafficCalc()*variation*1);
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


    void outputGraphToDotFile(const vector<int>& path) {
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


    USA.add_edge("Boston","Salem",32, 5);
    USA.add_edge("Boston","Burlington",20, 5);
    USA.add_edge("Salem","Burlington",25, 4);
    USA.add_edge("Boston","Newton",19, 5);
    USA.add_edge("Burlington","Newton",22, 4);
    USA.add_edge("Boston","Norwood",29, 5);
    USA.add_edge("Newton","Norwood",20, 4);
    USA.add_edge("Boston","Quincy",20, 5);
    USA.add_edge("Norwood","Quincy",20, 4);
    USA.add_edge("Salem","Lawrence",40, 3);
    USA.add_edge("Burlington","Lawrence",24, 4);
    USA.add_edge("Lawrence","Lowell",19, 3);
    USA.add_edge("Burlington","Lowell",17, 4);
    USA.add_edge("Newton","Marlborough",30,4);
    USA.add_edge("Lowell","Marlborough",30,3);
    USA.add_edge("Norwood","Mansfield",18,4);
    USA.add_edge("Marlborough","Mansfield",36,3);
    USA.add_edge("Quincy","Plymouth",32,3);
    USA.add_edge("Mansfield","Plymouth",42,3);
    USA.add_edge("Lowell","Littleton",17,3);
    USA.add_edge("Marlborough","Littleton",20,3);
    USA.add_edge("Littleton","Leominster",19,3);
    USA.add_edge("Leominster","Worcester",26,2);
    USA.add_edge("Marlborough","Worcester",22,3);
    USA.add_edge("Worcester","Providence",45,3);
    USA.add_edge("Mansfield","Providence",27,4);
    USA.add_edge("Plymouth","Providence",57,3);
    USA.add_edge("Leominster","Orange",37,2);
    USA.add_edge("Orange","Greenfield",26,2);
    USA.add_edge("Orange","Springfield",59,1);
    USA.add_edge("Greenfield","Springfield",38,3);
    USA.add_edge("Worcester","Springfield",54,3);
    vector<int> record = USA.dijk(0,3);

    for (size_t i = 0; i < record.size(); i++) {
        cout << record[i];
        if (i < record.size() - 1) cout << " -> ";
    }

    hour = 11;
    cout<<endl<<trafficCalc();

    //Outup graph


    return 0;
}