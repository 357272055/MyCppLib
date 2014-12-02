#include <iostream>
using namespace std;

template<class T>
class Queue;
template<class T>
class MinQueue;

template<class T>
class QueueNode{
	friend class Queue<T>;
	friend class MinQueue<T>;
	private:
		T data;
		QueueNode<T>* link;
		QueueNode(const T& dat):data(dat), link(NULL){}
};

template<class T>
class Queue{
	protected:
		int queueSize;
		QueueNode<T>* head;
		QueueNode<T>* tail;
	public:
		Queue():queueSize(0), head(NULL), tail(NULL){}
		~Queue();
		bool EnQueue(const T&);
		bool DeQueue(T&);
		bool isEmpty(){return queueSize == 0;}
		template<class X>
		friend ostream& operator << (ostream& os, Queue<X>& Q){
			QueueNode<T>* p = Q.head;
			if(Q.isEmpty())return os;
			while(p != NULL){
				os << p->data << ' ';
				p = p->link;
			}
			return os;
		}
};


template<class T>
class MinQueue:public Queue<T>{
	public:
		MinQueue(){Queue<T>();}
		bool EnQueue(const T&);
	private:
		void adjust();
};


template<class T>
Queue<T>::~Queue(){
	if(isEmpty())return;
	QueueNode<T>* p = head;
	while(p != NULL){
		head = head->link;
		delete p;
		p = head;
	}
	return;
}


template<class T>
bool Queue<T>::EnQueue(const T& data){
	QueueNode<T>* p = new QueueNode<T>(data);
	if(!p)return false;
	if(queueSize == 0){
		head = p;
		tail = p;
	}else{
		tail->link = p;
		tail = p;
	}
	++queueSize;
	return true;
}


template<class T>
void MinQueue<T>::adjust(){
	int qSize = this->queueSize;
	if(qSize <= 1)return;
	T& dat = this->tail->data;
	QueueNode<T>* p = this->head;
	while(p != this->tail){
		if(p->data > dat){
			T tmp = p->data;
			this->tail->data = tmp;
			p->data = dat;
			dat = tmp;
		}
		p = p->link;
	}
}


template<class T>
bool MinQueue<T>::EnQueue(const T& data){
	QueueNode<T>* p = new QueueNode<T>(data);
	if(!p)return false;
	if(this->queueSize == 0){
		this->head = p;
		this->tail = p;
	}else{
		QueueNode<T>* pre, *cur = this->head;
		pre = cur;
		while(cur != NULL){
			if(cur->data > data){
				if(cur == this->head){
					p->link = cur;
					this->head = p;
				}else{
					pre->link = p;
					p->link = cur;
				}
				break;
			}
			pre = cur;
			cur = cur->link;
		}
		if(cur == NULL){
			this->tail->link = p;
			this->tail = p;
		}
	}
	++this->queueSize;
	return true;
}

template<class T>
bool Queue<T>::DeQueue(T& queueHead){
	if(isEmpty()){
		return false;
	}else{
		QueueNode<T>* p = head;
		queueHead = p->data;
		head = head->link;
		--queueSize;
		if(isEmpty())tail = head;
		delete p;
	}
	return true;
}
