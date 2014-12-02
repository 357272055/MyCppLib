#include <iostream>
#include "queue.h"
using namespace std;

const int DefaultMaxVertexSize = 50;

template<class T, class E>
class Edge{
	public:
		int dest;
		E weight;
		Edge<T, E>* link;
		Edge():dest(-1), weight(0), link(NULL){}
		Edge(int dst, E wt):dest(dst), weight(wt), link(NULL){}
		bool operator != (Edge<T, E>& R)const{
			return (dest != R.dest);
		}
};


template<class T, class E>
class Vertex{
	public:
		T data;
		Edge<T, E>* path;
};


template<class T, class E>
class GraphLnk{
	private:
		Vertex<T, E>* NodeTable;
		int maxVertexSize;
		int numberOfEdges;
		int numberOfVertex;
	public:
		static const int INFINITY = 99999;
		GraphLnk(int sz = DefaultMaxVertexSize);
		~GraphLnk();
		bool isEmpty()const{
			return numberOfEdges == 0;
		}
		int VertexNum()const{return numberOfVertex;}
		int EdgeNum()const{return numberOfEdges;}
		T getValue(int v)const;
		E getWeight(int v1, int v2)const;
		int getFirstNeighbour(int v);
		int getNextNeighbour(int v, int w);
		bool insertVertex(const T& vertex);
		bool insertEdge(int v1, int v2, E weight);
		bool insertEdge(T t1, T t2, E weight);
		bool removeVertex(int v);
		bool removeEdge(int v1, int v2);
	//private:
		int getVertexPosition(T vertex){
			for(int i = 0;i < numberOfVertex;++i){
				if(NodeTable[i].data == vertex)
					return i;
			}
			return -1;
		}
};


template<class T, class E>
T GraphLnk<T, E>::getValue(int v)const{
	if(v < 0 || v >= numberOfVertex)return 0;
	return NodeTable[v].data;
}


template<class T, class E>
E GraphLnk<T, E>::getWeight(int v1, int v2)const{
	if(v1 >=0 && v2 >= 0 && v1 < numberOfVertex && v2 < numberOfVertex){
		Edge<T, E>* p = NodeTable[v1].path;
		while(p != NULL && p->dest != v2)p = p->link;
		if(p == NULL)return INFINITY;
		return p->weight;
	}
	return INFINITY;
}

template<class T, class E>
GraphLnk<T, E>::GraphLnk(int sz){
	maxVertexSize = sz;
	numberOfVertex = 0;
	numberOfEdges = 0;
	NodeTable = new Vertex<T, E>[maxVertexSize];
	if(!NodeTable)cerr << "no storage!\n";
	for(int i = 0;i < maxVertexSize;++i)
		NodeTable[i].path = NULL;
}

template<class T, class E>
GraphLnk<T, E>::~GraphLnk(){
	for(int i = 0;i < numberOfVertex;++i){
		Edge<T, E>* p = NodeTable[i].path;
		while(p != NULL){
			NodeTable[i].path = p->link;
			delete p;
			p = NodeTable[i].path;
		}
	}
	delete[] NodeTable;
}

//getFirstNeighbour
template<class T, class E>
int GraphLnk<T, E>::getFirstNeighbour(int v){
	if(v != -1){
		Edge<T, E>* p = NodeTable[v].path;
		if(p != NULL)
			return p->dest;
	}
	return -1;
}


//getNextNeighbour
template<class T, class E>
int GraphLnk<T, E>::getNextNeighbour(int v1, int v2){
	if(v1 != -1 && v2 != -1){
		Edge<T, E>* p = NodeTable[v1].path;
		while(p != NULL && p->dest != v2){
			p = p->link;
		}
		if(p != NULL && p->link != NULL)
			return p->link->dest;
	}
	return -1;
}



//insertVertex
template<class T, class E>
bool GraphLnk<T, E>::insertVertex(const T& vertex){
	if(numberOfVertex == maxVertexSize)return false;
	NodeTable[numberOfVertex++].data = vertex;
	return true;
}


//removeVertex
template<class T, class E>
bool GraphLnk<T, E>::removeVertex(int v){
	if(isEmpty() || v < 0 || v >= numberOfVertex)return false;

	Edge<T, E> *p, *s, *temp;
	int i, k;
	while(NodeTable[v].path != NULL){
		p = NodeTable[v].path;
		k = p->dest;
		s = NodeTable[k].path;
		temp = NULL;
		while(s != NULL && s->dest != v){
			temp = s;
			s = s->link;
		}
		if(s != NULL){
			if(temp != NULL)temp->link = s->link;
			else NodeTable[k].path = s->link;
			delete s;
		}
		NodeTable[v].path = p->link;
		delete p;
		--numberOfEdges;
	}
	--numberOfVertex;
	NodeTable[v].data = NodeTable[numberOfVertex].data;
	p = NodeTable[v].path = NodeTable[numberOfVertex].path;
	while(p != NULL){
		s = NodeTable[p->dest].path;
		while(s != NULL){
			if(s->dest == numberOfVertex){
				s->dest = v;
				break;
			}
			s = s->link;
		}
		p = p->link;
	}
	return true;
}


//insertEdge
template<class T, class E>
bool GraphLnk<T, E>::insertEdge(int v1, int v2, E wt){
	if(v1 >= 0 && v2 >= 0 && v1 < numberOfVertex && v2 < numberOfVertex && v1 != v2){
		Edge<T, E> *q, *p = NodeTable[v1].path;
		while(p != NULL && p->dest != v2)p = p->link;
		if(p != NULL)return false;
		p = new Edge<T, E>;
		q = new Edge<T, E>;
		p->dest = v2;
		p->weight = wt;
		p->link = NodeTable[v1].path;
		NodeTable[v1].path = p;

		q->dest = v1;
		q->weight = wt;
		q->link = NodeTable[v2].path;
		NodeTable[v2].path = q;
		++numberOfEdges;
		return true;
	}
	return false;
}

template<class T, class E>
bool GraphLnk<T, E>::insertEdge(T t1, T t2, E wt){
	if(t1 == t2)return false;
	int v1 = getVertexPosition(t1);
	int v2 = getVertexPosition(t2);
	return insertEdge(v1, v2, wt);
}

//removeEdge
template<class T, class E>
bool GraphLnk<T, E>::removeEdge(int v1, int v2){
	if(v1 >= 0 && v2 >= 0 && v1 < numberOfVertex && v2 < numberOfVertex && v1 != v2){
		Edge<T, E>*s, *p = NodeTable[v1].path;
		while(p != NULL && p->dest != v2){
			s = p;
			p = p->link;
		}
		if(p == NULL)return false;
		if(p == NodeTable[v1].path)
			NodeTable[v1].path = p->link;
		else
			s->link = p->link;
		delete p;

		p = NodeTable[v2].path;
		while(p != NULL && p->dest != v1){
			s = p;
			p = p->link;
		}
		if(p == NodeTable[v2].path)
			NodeTable[v2].path = p->link;
		else
			s->link = p->link;
		delete p;
		--numberOfEdges;
		return true;
	}
	return false;
}




//DFS, BFS


//DFS
template<class T, class E>
void DFS(GraphLnk<T, E>& G, int v, bool visited[]){
	cout << G.getValue(v) << ' ';
	visited[v] = true;
	int loc = G.getFirstNeighbour(v);
	while(loc != -1){
		if(visited[loc] == false)DFS(G, loc, visited);
		loc = G.getNextNeighbour(v, loc);
	}
}//insert dfs



template<class T, class E>
void DFS(GraphLnk<T, E>& G, const T& v){
	int numberOfVertex = G.VertexNum();
	bool* visited = new bool[numberOfVertex];
	int loc = 0;
	for(int i = 0;i < numberOfVertex;++i)visited[i] = false;
	loc = G.getVertexPosition(v);
	DFS(G, loc, visited);
	cout << endl;
	delete[] visited;
}//main dfs



//BFS


template<class T, class E>
void BFS(GraphLnk<T, E>& G, const T& v){
	int numberOfVertex = G.VertexNum();
	int w, loc = G.getVertexPosition(v);
	bool* visited = new bool[numberOfVertex];
	if(loc == -1){
		return;
	}
	for(int i = 0;i < numberOfVertex;++i)visited[i] = false;
	cout << G.getValue(loc) << ' ';
	visited[loc] = true;
	Queue<int> Q;
	Q.EnQueue(loc);
	while(!Q.isEmpty()){
		Q.DeQueue(loc);
		w = G.getFirstNeighbour(loc);
		while(w != -1){
			if(visited[w] == false){
				cout << G.getValue(w) << ' ';
				visited[w] = true;
				Q.EnQueue(w);
			}
			w = G.getNextNeighbour(loc, w);
		}
	}
	cout << endl;
	delete[] visited;
}



//
