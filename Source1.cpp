#include "Header1.h"
#include <iostream> 
#include "TXLib.h"
#include <vector>
#include <fstream>
#include <stack>

using namespace std;

void choosing_a_map();
void creating_a_map();
void generation(string name_map);

void menu() {
	txCreateWindow(1920, 1080);
	char x;
	cout << "1 - choosing a map, 2 - creating a map: ";
	cin >> x;
	if (x == '1') choosing_a_map();
	else if (x == '2') creating_a_map();

}

void choosing_a_map() {
	string name_map;
	cout << "enter name a map...";
	cin >> name_map;
	generation(name_map);


}

void setka()
{
	txSetColor(RGB(150, 150, 150), 1);

	for (int i = 0; i < 160 + 1; i++)
	{
		txLine(i * 12, 0, i * 12, 90 * 12);
	}
	for (int i = 0; i < 90 + 1; i++)
	{
		txLine(0, i * 12, 160 * 12, i * 12);
	}
}

void Draw(int(&P)[90][160])
{
	txSetFillColour(RGB(224, 112, 16));
	txSetColor(RGB(224, 112, 16), 1);
	for (int i = 0; i < 90; i++) {
		for (int j = 0; j < 160; j++) {

			if (P[i][j] == 1)
			{
				txSetFillColour(RGB(250, 200, 16));
				txSetColor(RGB(224, 112, 16), 1);
				txRectangle(j * 12, i * 12, j * 12 + 12, i * 12 + 12);
			}
			else if (P[i][j] == 4)
			{
				txSetFillColour(RGB(0, 200, 16));
				txSetColor(RGB(0, 112, 16), 3);
				txRectangle(j * 12, i * 12, j * 12 + 12, i * 12 + 12);
			}
			else if (P[i][j] == 5)
			{
				txSetFillColour(RGB(250,50,100));
				txSetColor(RGB(250, 100, 150), 4);
				txRectangle(j * 12, i * 12, j * 12 + 12, i * 12 + 12);
			}
			else if (P[i][j] == 3)
			{
				txSetFillColour(RGB(0, 112, 16));
				txSetColor(RGB(0, 112, 16), 1);
				txCircle(j * 12 + 12 / 2, i * 12 + 12 / 2, (12 / 2) - 2);
			}

		}
	}
	setka();

}

void read_map_for_creating(vector<int[90][160]>&P,int quaniti_floor)
{
	string line;
	int k = 0;
	ifstream in("maps\\creating_map.txt");
	
	if (in.is_open())
	{
		while (getline(in, line))
		{
			for (char x : line) {
				P[(k / 14400)%quaniti_floor][(k / 160) % 90][k % 160] = int(x-'0'); k++;
				//cout <<int(x-'0') << ' ' << x << endl;
			}
		}
	}
	in.close();
}

void creating_a_map() {

	const int M = 90, N = 160, k = 12;
	bool deactivated = 0;
	
	int XM, YM, floor_number = 0, quantity_floor;
	
	cout << "LKM-wall;\nPKM-delete;\nWHEEL-exit;\nPKM+LKM-people;\nUP-next floor;\n2X WHEEL-stairs\n\nhow many floors?";
	cin >> quantity_floor;
	vector<int[90][160]>P(quantity_floor); read_map_for_creating(P,quantity_floor);
	//system("cls"); 

	txSetFillColour(RGB(240, 240, 240));
	txClear();
	
	while (deactivated == 0)
	{
		Draw(P[floor_number]);
		while (txMouseButtons() == 0)
		{
			Sleep(5);
			if (txGetAsyncKeyState(VK_SPACE) == TRUE)
			{
				deactivated = 1;
				break;
			}
			else if (txGetAsyncKeyState(VK_UP) == TRUE)
			{
				floor_number = (floor_number + 1) % quantity_floor;
				Sleep(500);
				txSetFillColour(RGB(240, 240, 240));
				txClear();
				setka();
				
				break;
			}
			
		}
		XM = ceil(txMouseX() / 12) - 1; YM = ceil(txMouseY() / 12) - 1;
		if (txMouseButtons() == 1)//wall
		{
			P[floor_number][YM][XM] = 1;
		}
		else if (txMouseButtons() == 2)//delete
		{
			P[floor_number][YM][XM] = 0;
			txSetFillColour(RGB(240, 240, 240));
			txSetColor(RGB(240, 240, 240), 1);
			txRectangle(XM * 12, YM * 12, XM * 12 + 12, YM * 12 + 12);

		}
		else if (txMouseButtons() == 4)
		{	
			if (P[floor_number][YM][XM] == 4 or P[floor_number][YM][XM] == 5) {
				for (int i = 0; i < quantity_floor; i++) {
					P[i][YM][XM] = 5;//stairs
				}
			}
			else {
				P[floor_number][YM][XM] = 4;//exits
				Sleep(200);
			}
		}
		else if (txMouseButtons() == 3)//people
		{
			P[floor_number][YM][XM] = 3;
		}
	}
	ofstream out;
	out.open("maps\\creating_map.txt");
	//out << quantity_floor;
	for (int fl = 0; fl < quantity_floor; fl++) {
		for (int i = 0; i < M; i++)
		{
			for (int j = 0; j < N; j++)
			{
				
				if (out.is_open())
				{
					out << P[fl][i][j];
				}
				
			}
		}
		
	}
	out.close();
	txSetColor(RGB(224, 10, 16), 1);

	txSelectFont("Comic Sans MS", 90);
	txTextOut(800,100, "File has been written");


}

pair<int,int> read_map_for_generation(string name_map, vector<bool[90][160]>& wall, vector<vector<pair<int, int>>>& exit, vector<int[90][160]>& people1, vector<pair<int, int>>& stairs)
{
	string line;
	int k = 0,id=1,count_stairs=0;
	name_map = "creating_map.txt";
	ifstream in("maps\\"+name_map);

	if (in.is_open())
	{
		while (getline(in, line))
		{
			for (char x : line) {
				
				
				if (x == '1')wall[(k / 14400) % 4][(k / 160) % 90][k % 160] = 1;
				if (x == '3') {
					people1[(k / 14400) % 4][(k / 160) % 90][k % 160] = id; id++;
				}
				else if (x == '4') {
					exit[(k / 14400) % 4].push_back(make_pair(((k / 160) % 90), (k % 160)));
				}
					
				else if (x == '5' and k<=14400) { stairs.push_back(make_pair((k / 160) % 90, k % 160)); count_stairs++; }
				k++;
			}
		}
	}
	in.close();
	return make_pair(id,count_stairs);
}

string search_path(bool(&wall)[90][160],int ip,int jp,int ie,int je) 
{
	string chernovik = "";
	bool nalichie;
	int  weight[90][160], distance_from_the_start, distance_from_the_finish, checked[90][160],
		i_new = ip, j_new = jp, min_weight = 160 * 90, kakar = 0, zell[90][160], Path[90][160];
	for (int i = 0; i < 90; i++)
	{
		for (int j = 0; j < 160; j++)
		{
			Path[i][j] = 0;
			checked[i][j] = 0;
			weight[i][j] = 160 * 90 * 10;
			zell[i][j] = 0;
		}
	}
	while (i_new != ie or j_new != je) {

		if ((wall[i_new][j_new + 1] == 0) and (checked[i_new][j_new + 1] == 0)) {
			distance_from_the_finish = abs(ie - (i_new)) + abs(je - (j_new + 1));//!!!!!
			if (weight[i_new][j_new + 1] >= (zell[i_new][j_new] + 1 + distance_from_the_finish)) {
				weight[i_new][j_new + 1] = zell[i_new][j_new] + 1 + distance_from_the_finish;
				Path[i_new][j_new + 1] = 2; zell[i_new][j_new + 1] = zell[i_new][j_new] + 1;
			}
		}
		if ((wall[i_new + 1][j_new] == 0) and (checked[i_new + 1][j_new] == 0)) {
			distance_from_the_finish = abs(ie - (i_new + 1)) + abs(je - (j_new));
			if (weight[i_new + 1][j_new] >= (zell[i_new][j_new] + 1 + distance_from_the_finish)) {
				weight[i_new + 1][j_new] = zell[i_new][j_new] + 1 + distance_from_the_finish;
				Path[i_new + 1][j_new] = 3; zell[i_new + 1][j_new] = zell[i_new][j_new] + 1;
			}
		}
		if ((wall[i_new][j_new - 1] == 0) and (checked[i_new][j_new - 1] == 0)) {
			distance_from_the_finish = abs(ie - (i_new)) + abs(je - (j_new - 1));
			if (weight[i_new][j_new - 1] >= zell[i_new][j_new] + 1 + distance_from_the_finish) {
				weight[i_new][j_new - 1] = zell[i_new][j_new] + 1 + distance_from_the_finish;
				Path[i_new][j_new - 1] = 4; zell[i_new][j_new - 1] = zell[i_new][j_new] + 1;
			}
		}
		if ((wall[i_new - 1][j_new] == 0) and (checked[i_new - 1][j_new] == 0)) {
			distance_from_the_finish = abs(ie - (i_new - 1)) + abs(je - (j_new));
			if (weight[i_new - 1][j_new] >= zell[i_new][j_new] + 1 + distance_from_the_finish) {
				weight[i_new - 1][j_new] = zell[i_new][j_new] + 1 + distance_from_the_finish;
				Path[i_new - 1][j_new] = 1; zell[i_new - 1][j_new] = zell[i_new][j_new] + 1;
			}
		}
		nalichie = false;
		min_weight = 160 * 90 * 20;
		checked[i_new][j_new] = 1;
		distance_from_the_start = 160 + 90;
		kakar++;

		for (int i1 = 0; i1 < 90; i1++)
		{
			for (int j1 = 0; j1 < 160; j1++)
			{
				if ((weight[i1][j1] <= min_weight) and (weight[i1][j1] != (160 * 90 * 10))
					and (checked[i1][j1] == 0) and (wall[i1][j1] == 0))
				{

					i_new = i1;
					j_new = j1;
					min_weight = weight[i1][j1];
					nalichie = true;
					distance_from_the_start = abs(ip - (i1)) + abs(jp - (j1 - 1));

				}
			}
		}

		if (nalichie == false) break;
	}
	i_new = ie; j_new = je;

	while (i_new != ip or j_new != jp) {
		if (nalichie == false) {
			chernovik = "1";
			break;
		}
		if (Path[i_new][j_new] == 3) {
			i_new--;
			chernovik += "3";
		}
		else if (Path[i_new][j_new] == 2) {
			j_new--;
			chernovik += "2";
		}
		else if (Path[i_new][j_new] == 1) {
			i_new++;
			chernovik += "1";
		}
		else if (Path[i_new][j_new] == 4) {
			j_new++;
			chernovik += "4";
		}
	}
	reverse(chernovik.begin(), chernovik.end());
	return chernovik;
}

string building_a_path(int floor_number,int ip,int jp, vector<bool[90][160]>&wall, vector<pair<int, int>>&exits, vector<pair<int, int>>& stairs) {
	
	
	if (floor_number == 0) {
		string kost="", p="";
		int s = 1000000;
		
		for (pair<int, int>exit : exits) {
			kost = search_path(wall[0], ip, jp, exit.first, exit.second);
			if (kost.size() < s) {
				p = kost;
				s = kost.size();
			}
		}
		return p;
	}
	else {
		string p1="", p2="", kost="";
		int s = 1000000,id_stairs=0;
		for (int i = 0; i < stairs.size();i++) {
			kost = search_path(wall[floor_number], ip, jp, stairs[i].first, stairs[i].second);
			if (kost.size() < s) {
				p1 = kost;
				s = kost.size();
				id_stairs = i;
			}
		}
		s = 1000000;
		for (pair<int, int>exit : exits) {
			kost = search_path(wall[0], stairs[id_stairs].first, stairs[id_stairs].second, exit.first, exit.second);
			if (kost.size() < s) {
				p2 = kost;
				s = kost.size();
			}
		}
		
		return p1 +"5" + to_string(id_stairs) + p2;
	}
}

void draw_map(int(&P)[90][160], bool(&wall)[90][160], vector<pair<int, int>> &exit, vector<pair<int, int>>&stairs)
{
	const int k = 12;
	txSetFillColour(RGB(224, 112, 16));
	txSetColor(RGB(224, 112, 16), 1);
	for (int i = 0; i < 90; i++) {
		for (int j = 0; j < 160; j++) {

			if (P[i][j] != 0)//people
			{
				txSetFillColour(RGB(0, 213, 0));
				txSetColor(RGB(0, 112, 0), 3);
				txCircle(j * k + k / 2, i * k + k / 2, (k / 2) - 2);
			}
			if (wall[i][j] == 1)//wall
			{
				txSetFillColour(RGB(45, 35, 40));
				txSetColor(RGB(45, 35, 40), 5);
				txRectangle(j * k, i * k, j * k + k, i * k + k);
			}

		}

	}
	txSetFillColour(RGB(200, 255, 200));
	txSetColor(RGB(100, 255, 100), 5);
	for (pair<int,int>ex:exit) {//exit

		txRectangle(ex.second * k, ex.first * k, ex.second * k + k, ex.first * k + k);

	}
	txSetFillColour(RGB(200,200 , 255));
	txSetColor(RGB(100,100 ,255 ), 5);
	for (pair<int, int>st : stairs) {//stairs

		txRectangle(st.second * k, st.first * k, st.second * k + k, st.first * k + k);

	}

}

void Clear(int(&P1)[90][160], int(&P2)[90][160])
{
	const int k = 12;
	
	for (int i = 0; i < 90; i++)
	{
		for (int j = 0; j < 160; j++) {//clear_people
			if (P1[i][j] != 0 and P2[i][j] == 0)
			{
				txSetFillColour(RGB(200, 200, 200));
				txSetColor(RGB(200, 200, 200), 4);
				txCircle(j * k + k / 2, i * k + k / 2, (k / 2) - 2);
			}
			else if (P2[i][j] != 0) {//people
				txSetFillColour(RGB(0, 213, 0));
				txSetColor(RGB(0, 112, 0), 3);
				txCircle(j * k + k / 2, i * k + k / 2, (k / 2) - 2);
			}
		}
	}
}

bool step(vector<int[90][160]>&people1, vector<int[90][160]>& people2, vector<vector<pair<int, int>>> &exit, vector<pair<int, int>>& stairs, vector<string> &path, vector<vector<pair<int, int>>> &people_on_stairs,const int floor) {
	
	int size_p;
	bool empty = 1;
	for (int pos = 0; pos < people_on_stairs.size();pos++) {//id and counter
		size_p = people_on_stairs[pos].size();
		for (int p = 0; p < size_p; p++) {
			
			if (people_on_stairs[pos][p].second == 0 ) {
				if (people1[0][stairs[pos].first][stairs[pos].second] == 0 and people2[0][stairs[pos].first][stairs[pos].second] == 0) {
					people2[0][stairs[pos].first][stairs[pos].second] = people_on_stairs[pos][p].first;
					people_on_stairs[pos].erase(people_on_stairs[pos].begin() + p); size_p--;
					empty = 0;
				}
			}
			else {
				people_on_stairs[pos][p].second--;
			}
		}
	}

	for (int f = 0; f < floor; f++) {
		for (int i = 0; i < 90; i++) {
			for (int j = 0; j < 160; j++) {
				if (people1[f][i][j] != 0) {
					if (path[people1[f][i][j]].substr(0, 1) == "") {
						people2[f][i][j] = 0; empty = 0;
					}
					else if (path[people1[f][i][j]].substr(0, 1) == "3" and people1[f][i + 1][j] == 0 and people2[f][i + 1][j] == 0) {
						people2[f][i][j] = 0; people2[f][i + 1][j] = people1[f][i][j];//up
						path[people1[f][i][j]].erase(0, 1); empty = 0;
					}
					else if (path[people1[f][i][j]].substr(0, 1) == "4" and people1[f][i][j - 1] == 0 and people2[f][i][j - 1] == 0) {
						people2[f][i][j] = 0; people2[f][i][j - 1] = people1[f][i][j];//left
						path[people1[f][i][j]].erase(0, 1); empty = 0;
					}
					else if (path[people1[f][i][j]].substr(0, 1) == "1" and people1[f][i - 1][j] == 0 and people2[f][i - 1][j] == 0) {
						people2[f][i][j] = 0; people2[f][i - 1][j] = people1[f][i][j];//down
						path[people1[f][i][j]].erase(0, 1); empty = 0;
					}
					else if (path[people1[f][i][j]].substr(0, 1) == "2" and people1[f][i][j + 1] == 0 and people2[f][i][j + 1] == 0) {
						people2[f][i][j] = 0; people2[f][i][j + 1] = people1[f][i][j];//right
						path[people1[f][i][j]].erase(0, 1); empty = 0;
					}
					else if (path[people1[f][i][j]].substr(0, 1) == "5") {
						path[people1[f][i][j]].erase(0, 1);
						people2[f][i][j] = 0; people_on_stairs[stoi(path[people1[f][i][j]].substr(0, 1))].push_back(make_pair(people1[f][i][j], f*14));//stairs
						path[people1[f][i][j]].erase(0, 1);//to stair
						empty = 0;
					}
				}
			}
		}
	}
	return empty;
}

void copy_information(vector<int[90][160]>& x, vector<int[90][160]>& y,const int floor) {
	for (int f = 0; f < floor; f++) {
		for (int i = 0; i < 90; i++) {
			for (int j = 0; j < 160; j++) {
				y[f][i][j] = x[f][i][j];
			}
		}
	}
}

void generation(string name_map) {

	const int floor = 4;
	bool deactivated = 0;
	vector<bool[90][160]>wall(floor);
	vector<int[90][160]>people1(floor), people2(floor);

	vector<vector<pair<int, int>>>exit(floor);
	vector<pair<int, int>>stairs;
	int floor_number = 0,count_step=0;
	double time = 0;
	
	
	pair<int, int> inf = read_map_for_generation(name_map, wall, exit, people1, stairs);

	vector<string>path(inf.first);
	vector<vector<pair<int,int>>>people_on_stairs(inf.second);

	copy_information(people1, people2, floor);

	txSetColor(RGB(255, 255, 255), 1);
	txClear();
	setka();
	system("cls");

	for (int f = 0; f < floor; f++) {
		for (int i = 0; i < 90; i++) {
			for (int j = 0; j < 160; j++) {
				if (people1[f][i][j] != 0)path[people1[f][i][j]]=building_a_path(f,i,j,wall,exit[0],stairs);
			}
		}
	}
	
	draw_map(people1[floor_number], wall[floor_number], exit[floor_number], stairs);
	while (deactivated==0)
	{
		if (txGetAsyncKeyState(VK_SPACE) == TRUE)
		{
			deactivated = 1;
			break;
		}
		if (txGetAsyncKeyState(VK_UP) == TRUE)
		{
			floor_number = (floor_number + 1) % floor;
			Sleep(30);
			txSetFillColor(RGB(255, 255, 255));
			txClear();
			setka();
			draw_map(people1[floor_number], wall[floor_number], exit[floor_number], stairs);
		}
		Sleep(100);
		if (step(people1, people2, exit, stairs, path, people_on_stairs, floor))break;
		count_step++;
		Clear(people1[floor_number], people2[floor_number]);
		copy_information(people2,people1,floor);
		
	}
	txSetColor(RGB(255, 0, 50),5);
	txSelectFont("Comic Sans MS", 90);
	txDrawText((1920 / 2) - 200, 100, (1920 / 2) + 200,200, "Complete");
	
	txSleep(300);
	txSetFillColor(RGB(50, 50, 50));
	txClear();
	time = count_step * 0.7;
	cout << "Count step: " << count_step << "\nCount people: " << inf.first << "\nTime: " <<time;
}