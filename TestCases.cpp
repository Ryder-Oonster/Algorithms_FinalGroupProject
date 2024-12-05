//Include Necessary Libraries For Our Project
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
#include <iomanip>

using namespace std;

//Initialize global variable hour which is used to hold time of travel
double hour;

//Function to calculate traffic multiplier based off of time of commute
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

//Struct that acts as a road, holds information about travel time and end of the road
struct edge {
    int end;
    int Basetime;
    int time;
    int variation;

    //Funtion to calculate travel time of road using traffic calculation and variation
    void ChangeTraffic() {
        time = Basetime * (1+trafficCalc()*variation*.002);
        //cout<<Basetime<<"    "<<time<<endl;
    }
};

//Class that holds city data. Has information on roads that connect to it and the city's name.
class City {
public:
    vector<edge> edges = {};
    string name;
    //Used to add an edge, or road, between cities
    void add_edge(int end, int time, int variation) {
        edge New;
        New.end = end;
        New.Basetime = time;
        New.variation = variation;
        New.ChangeTraffic();
        edges.push_back(New);
    }
};
//Class that holds vector of cities, and methods for routing and graph visulization
class World {
private:
    vector<City> cities;
    map<string, int> cities_dictionary;
    int city_count;

public:
    vector<string> cityNames;
    World() {
        city_count = 0;
    }
    //Method to add a city to the world
    void add_city(string name) {
        City New;
        New.name = name;
        cities.push_back(New);
        cityNames.push_back(name);
        cities_dictionary[name] = city_count;
        city_count += 1;
    }
    //Method to add a road/edge between two cities
    void add_edge(string start, string end, int time, int variation) {
        cities[cities_dictionary[start]].add_edge(cities_dictionary[end], time, variation);
        cities[cities_dictionary[end]].add_edge(cities_dictionary[start], time, variation);
    }
    //Method to find the city with lowest time to get to that hasn't been finalized
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
    //Method that runs dijkstras algorithm on the graph
    vector<int> dijk(string sourceS, string destinationS) {
        int source = cities_dictionary[sourceS];
        int destination = cities_dictionary[destinationS];

        if (source==destination) {
            cout << "Source and destination are the same." << endl;
            return {};
        }

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
            cout << left << setw(20) << cityNames[i] << setw(10) << times[i] << endl;
        }

        if (times[destination] == INT_MAX) {
            cout<<"There is no path from source to destination.\n";
            return {};
        }
        else {
            cout<<"The shortest past from source to destination has "<<times[destination]<<" time.\n";
        }

        vector<int> path;
        for (int position = destination; position != -1; position = parent[position] ) {
            path.push_back(position);
        }
        reverse(path.begin(), path.end());
        return path;
    }

    //Method creates DotFile that can be used with GraphViz to create a visual representation of the graph
    void outputGraphToDotFile(const vector<int>& path) {
        ofstream dotFile("graph.dot");
        dotFile << "graph DijkstraGraph {\n";

        // Define the edges in the graph
        for (size_t i = 0; i < cities.size(); ++i) {
            for (const auto& edge : cities[i].edges) {
                if(edge.end > i) {
                    dotFile << "  " << cityNames[i] << " -- " << cityNames[edge.end] << " [label=\"" << edge.time << "\"];\n";
                }
                }
        }

        // Highlight the optimal path
        dotFile << "\n  // Highlight the optimal path\n";
        for (size_t i = 1; i < path.size(); ++i) {
            dotFile << "  " << cityNames[path[i - 1]] << " -- " << cityNames[path[i]] << " [color=red, penwidth=3.0];\n";
        }

        dotFile << "}\n";
    }

    //Used to update the time of each road based on the time of commute
    void updateTraffic() {

        for (size_t i = 0; i < cities.size(); ++i) {
            for (size_t x = 0; x < cities[i].edges.size(); ++x) {
                //cout<<"City"<<i<<endl;
                cities[i].edges[x].ChangeTraffic();
            }
        }
    }


};


//This main is used purely for testing classes and methods
int main() {
    hour = 0;
    //Initialize world for testing
    World Test;
    Test.add_city("City1");
    Test.add_city("City2");
    Test.add_city("City3");
    Test.add_city("City5");

    Test.add_edge("City1", "City2", 10, 2);


    //Test Case 1: Test Dijkstras if there is no path from start to end
    cout<<"Test Case 1:\n";
    vector<int> path = Test.dijk("City1", "City3");
    if (path.size() != 1) {
        for (size_t i = 0; i < path.size(); i++) {
            cout << Test.cityNames[path[i]];
            if (i < path.size() - 1) cout << " -> ";
        }
    }
    //Test Case 2: Test Dijkstra's Algorithm with source and destination of at the same location
    cout<<"Test Case 2:\n";
    path = Test.dijk("City1", "City1");
    if (path.size() != 1) {
        for (size_t i = 0; i < path.size(); i++) {
            cout << Test.cityNames[path[i]];
            if (i < path.size() - 1) cout << " -> ";
        }
    }

    //Test Case 3: Testing Dykstra's with two different simple paths
    Test.add_city("City4");
    Test.add_edge("City1", "City3", 10, 5);
    Test.add_edge("City2", "City5", 20, 1);
    Test.add_edge("City3", "City4", 10, 5);
    Test.add_edge("City4", "City5", 5, 4);

    cout<<"Test Case 3 - We expect to see the route from City 1 to City 3 to City 4 to City 5, with a time of 25:\n";
    path = Test.dijk("City1", "City5");
    if (path.size() != 1) {
        for (size_t i = 0; i < path.size(); i++) {
            cout << Test.cityNames[path[i]];
            if (i < path.size() - 1) cout << " -> ";
        }
    }
    cout<<endl;


    //Test Case 4 - Testing traffic multiplier at different times testing traffic at peak times vs no traffic
    hour = 0;
    cout<<"The traffic scaler at midnight is: " <<trafficCalc()<<endl;
    hour = 7;
    cout<<"The traffic scaler at 7 AM is: " <<trafficCalc()<<endl;
    hour = 20;
    cout<<"The traffic scaler at 8 PM is: " <<trafficCalc()<<endl;

    //Test Case 5: Test same route as used in case 3 using traffic at 8 PM. The route should change.
    Test.updateTraffic();
    cout<<"Test Case 5 - We expect to see the route from City 1 to City 2 to City 5, with a time of 32:\n";
    path = Test.dijk("City1", "City5");
    if (path.size() != 1) {
        for (size_t i = 0; i < path.size(); i++) {
            cout << Test.cityNames[path[i]];
            if (i < path.size() - 1) cout << " -> ";
        }
    }
    cout<<endl;




    return 0;
}


