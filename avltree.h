#include "stack.h"

template<class T>class AVLTree;


template<class T>
class AVLNode{
	friend class AVLTree<T>;
	private:
		int bf;
		T key;
		AVLNode* left;
		AVLNode* right;
	public:
		AVLNode(T k = 0, AVLNode<T>* l = NULL, AVLNode<T>* r = NULL):key(k), left(l), right(r){}
};


template<class T>
class AVLTree{
	private:
		int count;
		int printCount;
	public:
		AVLNode<T>* root;
		AVLTree():root(NULL), printCount(0), count(0){}
		AVLTree(const T&);
		bool Insert(const T&);
		bool Remove(const T&);
		AVLNode<T>* Search(const T&);
		int Count(){return count;}
		friend ostream& operator << (ostream& os, const AVLTree<T>*& tree);
		void Print(AVLNode<T>*);
		void print(){Print(root);cout << endl;printCount = 0;}
	private:
		bool Insert(AVLNode<T>*&, const T&);
		bool Remove(AVLNode<T>*&, const T&);
		AVLNode<T>* Search(const T&, AVLNode<T>*);
		void RotateL(AVLNode<T>*&);
		void RotateR(AVLNode<T>*&);
		void RotateLR(AVLNode<T>*&);
		void RotateRL(AVLNode<T>*&);
};

template<class T>
AVLNode<T>* AVLTree<T>::Search(const T& value){
	AVLNode<T>* returnValue = Search(value, root);
	if(!returnValue)
		cout << value << " not exist!\n";
	else
		cout << "search success!\n";
	return returnValue;
}

template<class T>
AVLNode<T>* AVLTree<T>::Search(const T& value, AVLNode<T>* startNode){
	AVLNode<T>* returnValue = NULL;
	if(startNode != NULL){
		if(value == startNode->key){
			cout << "Find!\n";
			return startNode;
		}
		else if(value < startNode->key)
			returnValue = Search(value, startNode->left);
		else if(value > startNode->key)
			returnValue = Search(value, startNode->right);
	}
	return returnValue;
}


template<class T>
void AVLTree<T>::RotateL(AVLNode<T>*& ptr){
	AVLNode<T>* subL = ptr;
	ptr = subL->right;
	subL->right = ptr->left;
	ptr->left = subL;
	ptr->bf = subL->bf = 0;
}


template<class T>
void AVLTree<T>::RotateR(AVLNode<T>*& ptr){
	AVLNode<T>* subR = ptr;
	ptr = subR->left;
	subR->left = ptr->right;
	ptr->right = subR;
	ptr->bf = subR->bf = 0;
}


template<class T>
void AVLTree<T>::RotateLR(AVLNode<T>*& ptr){
	AVLNode<T> *subR = ptr, *subL = subR->left;
	ptr = subL->right;
	subL->right = ptr->left;
	ptr->left = subL;
	if(ptr->bf <= 0)subL->bf = 0;
	else subL->bf = -1;
	subR->left = ptr->right;
	ptr->right = subR;
	if(ptr->bf == -1)subR->bf = 1;
	else subR->bf = 0;
	ptr->bf = 0;
}

template<class T>
void AVLTree<T>::RotateRL(AVLNode<T>*& ptr){
	AVLNode<T> *subL = ptr, *subR = subL->right;
	ptr = subR->left;
	subR->left = ptr->right;
	ptr->right = subR;
	if(ptr->bf >= 0)subR->bf = 0;
	else subR->bf = 1;

	subL->right = ptr->left;
	ptr->left = subL;
	if(ptr->bf == 1)subL->bf = -1;
	else subL->bf = 0;
	ptr->bf = 0;
}


template<class T>
AVLTree<T>::AVLTree(const T& refValue):root(NULL), printCount(0), count(0){
	T k = 0;
	cout << "Enter a line of data, end with(" << refValue << ")\n";
	cin >> k;
	while(k != refValue){
		Insert(root, k);
		cin >> k;
	}
}


template<class T>
bool AVLTree<T>::Insert(const T& value){
	return Insert(root, value);
}

/*
template<class T>
bool AVLTree<T>::Insert(AVLNode<T>*& ptr, const T& value){
	bool isSuccess = false;
	if(ptr == NULL){
		ptr = new AVLNode<T>(value);
		++count;
		if(ptr == NULL){
			//RAM FULL!
			--count;
		}else{
			isSuccess = true;
		}
	}else if(ptr->key == value){
		cerr << "The input key is already in " << value << endl;
		isSuccess = false;
	}else if(value < ptr->key){
		isSuccess = Insert(ptr->left, value);
	}else{
		isSuccess = Insert(ptr->right, value);
	}
	return isSuccess;
}
*/


template<class T>
bool AVLTree<T>::Insert(AVLNode<T>*& ptr, const T& value){
	AVLNode<T>* pr = NULL, *p = ptr, *q;
	int d;
	Stack<AVLNode<T>* >stack;
	while(p != NULL){
		if(p->key == value){
			cout << value << " is already in\n";
			return false;
		}
		pr = p;
		stack.push(pr);
		if(value < p->key)p = p->left;
		else p = p->right;
	}
	++count;
	p = new AVLNode<T>(value);
	if(pr == NULL){
		ptr = p;
		return true;
	}
	if(value < pr->key)pr->left = p;
	else pr->right = p;
	while(!stack.isEmpty()){
		stack.pop(pr);
		if(p == pr->left)--(pr->bf);
		else ++(pr->bf);
		if(pr->bf == 0)break;//first situation
		if(pr->bf * pr->bf == 1)
			p = pr;
		else{
			d = (pr->bf < 0)? -1: 1;
			if(p->bf == d){
				if(d == -1)RotateR(pr);
				else RotateL(pr);
			}else{
				if(d == -1)RotateLR(pr);
				else RotateRL(pr);
			}
			break;
		}
	}
	if(stack.isEmpty())ptr = pr;
	else{
		stack.getTop(q);
		if(q->key > pr->key)q->left = pr;
		else q->right = pr;
	}
	return true;
}

template<class T>
bool AVLTree<T>::Remove(const T& value){
	return Remove(root, value);
}


template<class T>
bool AVLTree<T>::Remove(AVLNode<T>*& ptr, const T& value){
	bool isSuccess = false;
	AVLNode<T>* tmp;
	if(ptr != NULL){
		if(value < ptr->key)
			isSuccess = Remove(ptr->left, value);
		else if(value > ptr->key)
			isSuccess = Remove(ptr->right, value);
		else if(ptr->left != NULL && ptr->right != NULL){
			tmp = ptr->right;
			while(tmp->left != NULL)tmp = tmp->left;
			ptr->key = tmp->key;
			isSuccess = Remove(ptr->right, ptr->key);
		}else{
			tmp = ptr;
			if(ptr->left == NULL)ptr = ptr->right;
			else ptr = ptr->left;
			delete tmp;
			--count;
			isSuccess = true;
		}
	}
	return isSuccess;
}


template<class T>
void AVLTree<T>::Print(AVLNode<T>* ptr){
	if(ptr != NULL){
		Print(ptr->left);
		if(printCount < count - 1){
			cout << ptr->key << "->";
			++printCount;
		}else{
			cout << ptr->key;
			++printCount;
		}
		Print(ptr->right);
	}
}

template<class T>
ostream& operator << (ostream& os, const AVLTree<T>*& tree){
	tree->printCount = 0;
	cout << "operator << \n";
	tree->Print(tree->root);
	return os;
}
