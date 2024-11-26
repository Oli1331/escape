#pragma once
#include "Source1.cpp" 

void menu();

void choosing_a_map();

void creating_a_map();

void Draw(int(&P)[90][160]);

void setka();

void read_map_for_creating(vector<int[90][160]>& P);

void generation(string name_map);

pair<int, int> read_map_for_generation(string name_map, vector<bool[90][160]>& wall, vector<vector<pair<int, int>>>& exit, vector<int[90][160]>& people1, vector<pair<int, int>>& stairs);

string search_path(bool(&wall)[90][160], int ip, int jp, int ie, int je);

string building_a_path(int floor_number, int ip, int jp, vector<bool[90][160]>& wall, vector<pair<int, int>>& exits, vector<pair<int, int>>& stairs);

void draw_map(int(&P)[90][160], bool(&wall)[90][160], vector<pair<int, int>>& exit, vector<pair<int, int>>& stairs);

bool step(vector<int[90][160]>& people1, vector<int[90][160]>& people2, vector<vector<pair<int, int>>>& exit, vector<pair<int, int>>& stairs, vector<string>& path, vector < vector<pair<int, int>>>& people_on_stairs, const int floor);

void Clear(int(&P1)[90][160], int(&P2)[90][160]);

void generation(string name_map);
