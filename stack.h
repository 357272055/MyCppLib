#include <iostream>
using namespace std;


template<class T>class Stack;


template<class T>
class StackNode{
	friend class Stack<T>;
	private:
		T data;
		StackNode<T>* link;
	public:
		StackNode(const T& dat = 0, StackNode<T>* node = NULL):data(dat), link(node){}
};



template<class T>
class Stack{
	private:
		StackNode<T>* top;
		int count;
	public:
		Stack():count(0), top(NULL){}
		void push(const T&);
		bool pop(T&);
		bool getTop(T&);
		bool isEmpty(){return top == NULL;}
		int Count(){return count;}
};


template<class T>
void Stack<T>::push(const T& value){
	top = new StackNode<T>(value, top);
}


template<class T>
bool Stack<T>::pop(T& popElem){
	if(isEmpty())return false;
	popElem = top->data;
	StackNode<T>* tmp = top;
	top = top->link;
	delete tmp;
	return true;
}

template<class T>
bool Stack<T>::getTop(T& getTopValue){
	if(top == NULL)return false;
	getTopValue = top->data;
	return true;
}
