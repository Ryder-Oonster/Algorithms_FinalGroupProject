#include <iostream>
#include <vector>
#include <string>

using namespace std;

struct edge{
    int start;
    int end;
    int time;
};


class City{
private:
    vector<edge> edges = {};
public:
    string name;
    void add_edge(int end, int time){
        edge* New = new edge;
        New->end = end;
        New->time = time;
        edges.push_back(*New);
    }
};

class map{
    private:
        vector<City> cities;
        int city_count;
    public:
        map(){
            city_count = 0;
        }
        void add_city(string name){
            City* New = new City;
            New->name = name;
            cities.push_back(*New);
            city_count+=1;
        }
        void add_edge(int start, int end, int time){
            cities[start].add_edge(end, time);
            cities[end].add_edge(start, time);
        }
};


int main(){
    map USA;
    USA.add_city("Boston");
    USA.add_city("Chicago");



} 