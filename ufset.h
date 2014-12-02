#include <iostream>
using namespace std;

class UFSets{
	private:
		static const int DefaultUFSetSize = 20;
		int* parent;
		int size;
	public:
		UFSets(int sz = DefaultUFSetSize);
		~UFSets(){delete[] parent;}
		int Find(int);
		void Union(int, int);
		UFSets& operator = (UFSets&);
};


UFSets::UFSets(int sz):size(sz){
	parent = new int[size];
	for(int i = 0;i < size;++i)
		parent[i] = -1;
}


int UFSets::Find(int x){
	while(parent[x] >= 0)x = parent[x];
	return x;
}


void UFSets::Union(int root1, int root2){
	parent[root1] += parent[root2];
	parent[root2] = root1;
}
