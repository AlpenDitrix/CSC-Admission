#include <vector>
#include <deque>
#include <iostream>

using namespace std;

struct topology_sorted{
	int amount;
	int *vertices;
	int iterator;
	inline void operator=(int size) {
		amount = size;
		vertices = new int[size];
		iterator = amount-1;
	}
	inline void operator+(int vertice){
		vertices[iterator--] = vertice;
	}
};
std::ostream& operator<<(std::ostream &o, topology_sorted sort) {
	for (int i = 0; i < sort.amount; i++) {
		o << sort.vertices[i] <<' ';
	}
	return o;
}
int V; //vertices amount
int E; //edges amount
vector<int> *connected; //graph connectivity vector
vector<int> *transposed; //transposed connectivity graph
bool *visited; //graph coloring
int *start_timing; //vertices sorted in order of time when they're visited first time
int *finish_timing; //vertices sorted in order of time when they was left
int timing = 0; //current "visitor" time
topology_sorted topology_sorting; //currently made topology sorting
int shortest_distance(); //returns shortest distance between two vertices or -1 if they have no connection path

//main methods
bool check_connectivity(); //checks if vertice 'from' connected to 'to'
int connected_components(); //count amount of connected components
bool check_cycle(); //checks if graph has cycle
void do_topology_sorting(); //returns vertice numbers sorted in topology sotring order
int stongly_connected_components(); //counts amount of strong connected components

//supporting methods
void cout_True_False(bool b);
bool is_connected(int from, int to);
void explore_v(int from);
bool explore_start_end(int from);
void explore_v_end(int from);
void explore_transposed_v(int from);
void explore_v_topol(int from);

//common data initialization
void init(const bool ordered){
	cin >> V >> E;
	connected = new vector<int>[V];

	for (int i = 0; i < E; i++) {
		int a, b;
		cin >> a >> b;
		connected[a - 1].push_back(b - 1);
		if (ordered) continue;
		else connected[b - 1].push_back(a - 1);
	}
}

int main() {
	const bool ORDERED = true;
	init(ORDERED);
	//choose your destiny:
	/*cout_True_False(check_connectivity());*/
	/*cout << connected_components();*/
	/*cout_True_False(check_cycle());*/
	/*do_topology_sorting();
	cout << topology_sorting;*/
	/*cout << stongly_connected_components();*/
	/*cout << shortest_distance();*/
	//cin >> V;
}

void cout_True_False(bool b){
	cout << (b ? "True" : "False");
}

/*==========================================*/
bool check_connectivity() {
	//depending init
	int u, v;
	cin >> u >> v;
	u--; v--;
	visited = new bool[V];
	for (int i = 0; i < V; i++) visited[i] = false;

	//dfs
	return is_connected(u, v);
}
bool is_connected(int from, int to) {
	visited[from] = true;
	for (unsigned int i = 0; i < connected[from].size(); i++) {
		int vertice = connected[from][i];
		if (vertice != to) {	
			if (!visited[vertice] && is_connected(vertice, to)) return true;
		} else return true;
	}
	return false;
}
/*==========================================*/
int connected_components() {
	//ddepending init
	visited = new bool[V];
	for (int i = 0; i < V; i++) visited[i] = false;
		
	//dfs for every component
	int components = 0;
	for (int i = 0; i < V; i++) {
		if (!visited[i]) {
			explore_v(i);
			components++;
		}
	}
	return components;
}
void explore_v(int from) {
	visited[from] = true;
	for (unsigned int i = 0; i < connected[from].size(); i++) {
		int vertice = connected[from][i];
		if (!visited[vertice]) explore_v(vertice);
	}
}
/*==========================================*/
bool check_cycle() {
	start_timing = new int[V];
	finish_timing = new int[V];
	for (int i = 0; i < V; i++) start_timing[i] = 0;
	for (int i = 0; i < V; i++) finish_timing[i] = 0;

	//dfs with saving time of visits
	for (int i = 0; i < V; i++) {
		if (!start_timing[i] && explore_start_end(i)) return true;
	}
	return false;
}
bool explore_start_end(int from) {
	start_timing[from]= ++timing;
	for (unsigned int i = 0; i < connected[from].size(); i++) {
		int vertice = connected[from][i];
		if (start_timing[vertice] == 0) { // if not visited
			if (explore_start_end(vertice))
				return true;
		} else if (finish_timing[vertice]== 0) return true; // if already visited, but not finished
	}
	finish_timing[from]= ++timing;
	return false;
}
/*==========================================*/
void do_topology_sorting() {
	topology_sorting = V;
	visited = new bool[V];
	for (int i = 0; i < V; i++) visited[i] = false;


	for (int i = 0; i < V; i++) {
		if (!visited[i]) explore_v_topol(i);
	}
}
void explore_v_topol(int from)
{
	visited[from] = true;
	for (unsigned int i = 0; i < connected[from].size(); i++) {
		int vertice = connected[from][i];
		if (!visited[vertice]) explore_v_topol(vertice);
	}
	topology_sorting + (from + 1);
}
/*==========================================*/
int stongly_connected_components(){
	//depending init
	finish_timing = new int[V];
	for (int i = 0; i < V; i++) finish_timing[i] = 0;
	visited = new bool[V];
	for (int i = 0; i < V; i++) visited[i] = false;
	transposed = new vector<int>[V];
	for (int from = 0; from < V; from++){
		for (unsigned int j = 0; j < connected[from].size(); j++){
			transposed[connected[from][j]].push_back(from);
		}

	}

	//dfs
	for (int i = 0; i < V; i++) {
		if (!visited[i]) explore_v_end(i);

	}
	for (int i = 0; i < V; i++) visited[i] = false;
		//transposed dfs
	int strong_count = 0;
	for (timing--; timing >= 0; timing--) {
		int vertice = finish_timing[timing];
		if (!visited[vertice]){
			explore_transposed_v(vertice);
			strong_count++;
		}
	}
	return strong_count;
}
void explore_v_end(int from){
	visited[from] = true;
	for (unsigned int i = 0; i < connected[from].size(); i++) {
		int vertice = connected[from][i];
		if (!visited[vertice]) explore_v_end(vertice);
	}
	finish_timing[timing++] = from;
}
void explore_transposed_v(int from) {
	visited[from] = true;
	for (unsigned int i = 0; i < transposed[from].size(); i++) {
		int vertice = transposed[from][i];
		if (!visited[vertice]) explore_transposed_v(vertice);
	}
}
/*==========================================*/
int shortest_distance() {
	int u, v;
	cin >> u >> v;
	u--; v--;
	int *dist = new int[V];
	for (int i = 0; i < V; i++) dist[i] = -1;
	dist[u] = 0;

	deque<int> curr_level;
	curr_level.push_back(u);
	while (!curr_level.empty()) {
		int curr = curr_level.front();
		curr_level.pop_front();
		for (unsigned int i = 0; i < connected[curr].size(); i++){
			int vertice = connected[curr][i];
			if(vertice!=v){if (dist[vertice] == -1) {

				curr_level.push_back(vertice);
				dist[vertice] = dist[curr] + 1;
			}
			}
			else return dist[curr] + 1;
		}
	}
	return dist[v];//-1
}