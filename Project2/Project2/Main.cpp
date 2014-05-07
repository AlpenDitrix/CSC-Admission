#include <iostream>
#include <assert.h>
#include <sstream>
using namespace std;

struct edge {
	int a;
	int b;
	inline bool operator<(edge e) {
		return a < e.a;
	}
	inline bool operator>(edge e) {
		return a > e.a;
	}
	inline void operator=(edge e){
		a = e.a;
		b = e.b;
	}
};

std::ostream& operator<<(std::ostream& os, const edge& e)
{
	os << '(' << e.a << ',' << e.b << ')';
	return os;
}

void sort();
void quickSort(edge *arr, int left, int right);
void count_dots_ownage_stupid();
void count_dots_ownage();


int N, M;
edge *edges;
int *dots;
void read(){
	cin >> N; edges = new edge[N];
	cin >> M; dots = new int[M];
	for (int i = 0; i < N; i++) {
		cin >> edges[i].a;
		cin >> edges[i].b;
	}
	for (int i = 0; i < M; i++){
		cin >> dots[i];
	}
}
int mainOld() {
	assert(1 == (1>0));
	read();
	//sort();
	count_dots_ownage_stupid();
	cin >> N;
	return 0;
}

void sort() {
	quickSort(edges, 0, N);
}

void quickSort(edge *arr, int left, int right) {
	int i = left, j = right;
	edge tmp;

	edge pivot = arr[(int)((left + right) / 2)];

	/* partition */
	while (i <= j) {
		while (arr[i] < pivot)
			i++;
		while (arr[j] > pivot)
			j--;
		if (i <= j) {
			tmp = arr[i];
			arr[i] = arr[j];
			arr[j] = tmp;
			i++;
			j--;
		}
	};

	/* recursion */
	if (left < j)
		quickSort(arr, left, j);
	if (i < right)
		quickSort(arr, i, right);

}

void count_dots_ownage_stupid() {
	for (int i = 0; i < M; i++){
		int count = 0;
		for (int j = 0; j < N; j++) {
			count += (edges[j].a <= dots[i] && dots[i] <= edges[j].b);
		}
		cout << count << ' ';
	}
	
}

void count_dots_ownage() {
	for (int i = 0; i < M; i++) {
		int dot = dots[i];
		int count = 0;
		int j = -1;
		while (edges[++j].a <= dot) {
			if (edges[j].b >= dot) { count++; }
		}
		cout << count << ' ';
	}
}