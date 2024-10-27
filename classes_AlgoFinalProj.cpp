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
    string name;
    vector<edge> edges = {};
public:
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
        void add_city(string name){
            City* New = new City;
            
        }


};


int main(){




} 