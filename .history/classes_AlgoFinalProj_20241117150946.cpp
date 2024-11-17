#include <iostream>
#include <vector>
#include <string>
#include <map>

using namespace std;

struct edge{
    int end;
    int Basetime;
    int time;
    int variation = 4;

    void ChangeTraffic(){
        time = Basetime + rand() % variation + 1;
    }
};

class City{
public:
    vector<edge> edges = {};
    string name;
    void add_edge(int end, int time){
        edge* New = new edge;
        New->end = end;
        New->Basetime = time;
        New->ChangeTraffic();
        edges.push_back(*New);
    }
};

class World{
    private:
        vector<City> cities;
        map<string, int> cities_dictionary;
        int city_count;
    public:
        World(){
            city_count = 0;
        }
        void add_city(string name){
            City* New = new City;
            New->name = name;
            cities.push_back(*New);
            cities_dictionary[name] = city_count;
            city_count+=1;
        }

        void add_edge(string start, string end, int time) {
            cities[cities_dictionary[start]].add_edge(cities_dictionary[end], time);
            cities[cities_dictionary[end]].add_edge(cities_dictionary[start], time);
        }

        int findMin(int finalized[], int times[]){
            int minIndex;
            int minTime = INT_MAX;
            for (int i = 0; i<city_count; i++){
                if (!finalized[i]&&times[i]<minTime){
                    minTime = times[i];
                    minIndex = i;
                }
            }
            return minIndex;
        }
        void dijk (int source){

            int finalized[city_count];
            for (int i = 0; i<city_count; i++){
                finalized[i]=0;
            }
            int times[city_count];
            for (int i = 0; i<city_count; i++){
                times[i]=INT_MAX;
            }
            times[source] = 0;

            int current;
            for(int i = 0; i<city_count; i++){
                current = findMin(finalized,times);
                finalized[current] = 1;

                for(int x = 0; x<cities[current].edges.size();x++){
                    if(!finalized[cities[current].edges[x].end]){
                        times[cities[current].edges[x].end] = min(times[cities[current].edges[x].end], times[current]+cities[current].edges[x].time);
                    }
                }
            }

        for (int i = 0; i < city_count; i++)
             cout << i << " \t\t\t\t" << times[i] << endl;
            
        }
        

};


int main(){
    srand(time(0)); 
    World USA;
    USA.add_city("Boston"); //0
    USA.add_city("Chicago"); //1
    USA.add_city("Portland"); //2
    USA.add_city("Keene"); //3
    USA.add_city("Cambridge"); //4

 
    USA.add_edge("Boston", "Chicago", 15);
    USA.add_edge("Chicago", "Portland", 8);
    USA.add_edge("Boston", "Portland", 4);
    USA.add_edge("Chicago", "Keene", 10);
    USA.add_edge("Cambridge", "Portland", 3);
    USA.dijk(0);
    
} 