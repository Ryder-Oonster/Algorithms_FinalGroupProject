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



int main() {
    hour = 18;
    srand(static_cast<unsigned int>(time(0)));
    //Create a world
    World USA;
    //Add all cities to world
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
    //Add roads/edges between cities
    USA.add_edge("Boston","Salem",26, 5);
    USA.add_edge("Boston","Burlington",24, 5);
    USA.add_edge("Salem","Burlington",30, 4);
    USA.add_edge("Boston","Newton",17, 5);
    USA.add_edge("Burlington","Newton",26, 4);
    USA.add_edge("Boston","Norwood",30, 5);
    USA.add_edge("Newton","Norwood",23, 4);
    USA.add_edge("Boston","Quincy",15, 5);
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


    //Test Case 1: Same source and destination
    /*vector<int> record = USA.dijk("Worcester","Worcester");
    if (record.size() != 1) {
        for (size_t i = 0; i < record.size(); i++) {
            cout << USA.cityNames[record[i]];
            if (i < record.size() - 1) cout << " -> ";
        }
    }
    else {
        cout<<"The source and destination cities are the same so a travel time can't be calculated\n";
    }
    */

    //Test Case 2: With Zero traffic we would expect time from Boston to Salem to be 26
    /*
    hour = 0;
    vector<int> record = USA.dijk("Boston","Salem");
    if (record.size() != 1) {
    for (size_t i = 0; i < record.size(); i++) {
    cout << USA.cityNames[record[i]];
    if (i < record.size() - 1) cout << " -> ";
    }
    }
    else {
    cout<<"The source and destination cities are the same so a travel time can't be calculated\n";
    }
    */




    //List all available cities in the world and prompts user for inputs
    string source, destination;
        cout<<"This program allows you to calculate the time, and best route to take between cities"<<endl;
        cout<<"Below is a list of cities that are included in this program:"<<endl;
        cout<<"Boston      Salem      Burlington    Newton     Norwood      Quincy"<<endl;
        cout<<"Lawrence    Lowell     Marlborough   Mansfield  Plymouth     Littleton"<<endl;
        cout<<"Leominster  Worcester  Providence    Orange     Greenfield   Springfield"<<endl;

        cout<<"Please enter the city that you are currently at (Exactly As Seen Above): ";
        cin>>source;
        cout<<"Please enter the city that is your destination (Exactly As Seen Above): ";
        cin>>destination;
        cout<<"Please Enter an Integer (0-24) Representing Time of Travel: ";
        cin>>hour;
        USA.updateTraffic();
    vector<int> record = USA.dijk(source,destination);

    //Output optimal path
    if (record.size() != 1) {
        for (size_t i = 0; i < record.size(); i++) {
            cout << USA.cityNames[record[i]];
            if (i < record.size() - 1) cout << " -> ";
        }
    }
    else {
        cout<<"The source and destination cities are the same so a travel time can't be calculated\n";
    }

    //Convert graph into a dot file that can be turned into a visualization
    USA.outputGraphToDotFile(record);



    return 0;

}