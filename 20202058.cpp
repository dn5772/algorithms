#include <iostream>
using namespace std;

int cnt = 0, cnt_p = 0;

template <typename T> 
class Queue {
  private :

  T* queue;
  int front, rear, capacity;

  public :

  Queue(int queueCapacity = 10);
  bool IsEmpty () const;
  T& Front() const;
  T& Rear() const;
  void Push(const T& item);
  void Pop();
};

template <typename T>
Queue<T>::Queue(int queueCapacity) : capacity(queueCapacity) {
  if (capacity < 1) 
    throw "Queue capacity must be > 0"; 
  
  queue = new T[capacity];
  front = rear = 0;
}

template <typename T>
inline bool Queue<T>::IsEmpty() const { 
  return front == rear; 
}

template <typename T>
inline T& Queue<T>::Front() const {
  if (IsEmpty())
    throw "Queue is empty. No front element"; 
  return queue[(front + 1) % capacity];
}

template <typename T>
inline T& Queue<T>::Rear() const {
  if(IsEmpty())
    throw "Queue is empty. No rear element"; 
  return queue[rear];
}

template <typename T>
void Queue<T>::Push(const T& x) 
{
  if ((rear+1) % capacity == front) {
    T* newQueue = new T[2 * capacity];
    int start = (front + 1) % capacity; 
    if (start < 2)
      copy(queue + start, queue + start + capacity - 1,newQueue);

    else{
      copy(queue + start, queue + capacity, newQueue); 
      copy(queue, queue + rear + 1, newQueue + capacity - start);
    }
    front = 2 * capacity - 1; 
    rear = capacity - 2; 
    capacity *= 2; 
    delete[]queue;
    queue = newQueue;
  }
  cnt_p++;
  rear = (rear + 1) % capacity; 
  queue[rear] = x;
}

template <typename T> 
void Queue<T>::Pop() {
  if (IsEmpty()) 
    throw "Queue is empty. Cannot delete."; 
  front = (front + 1) % capacity;
  queue[front].~T();
}

template <typename T>
class P_queue{
	private :

	T* data;
	int size;
	int capacity;

	public :
	P_queue(int pq_capacity = 10);
	~P_queue();

	void push(const T& a);
	T pop();
	bool isEmpty();
};

template <typename T>
P_queue<T>::P_queue(int pq_capacity) : capacity(pq_capacity) {
	data = new T[capacity];
	size = 0;
}

template <typename T>
P_queue<T>::~P_queue(){
	delete[] data;
}

template <typename T>
void P_queue<T>::push(const T& a){
	if (size == capacity){
		T* newPQ = new T[2*capacity];
		for (int i=0; i < size; i++){
			newPQ[i] = data[i];
		}
		delete[] data;
		data = newPQ;
		capacity *= 2;
	}

	data[size] = a;

	int parent = (size - 1) / 2;
	int child = size;

	while ((parent >= 0) && (data[child] > data[parent]))
	{
		T tmp = data[parent];
		data[parent] = data[child];
		data[child] = tmp;

		child = parent;
		parent = (parent - 1) / 2;
	}
	cnt_p++;
	size++;
}

template <typename T>
T P_queue<T>::pop(){
	T pop_data = data[0];
	data[0] = data[--size];
	
	int parent = 0;
	int child = 1;

	while (child < size)
	{
		if (data[child+1] > data[child]){child++;}

		if (data[parent] >= data[child]){break;}
		else {
		T tmp = data[parent];
		data[parent] = data[child];
		data[child] = tmp;
		}

		parent = child;
		child = child * 2 + 1; 
	}

	return pop_data;
}

template <typename T>
inline bool P_queue<T>::isEmpty(){
	return size == 0;
}


////////////////////////////////////////////////////////

class Node{
	private :

	public :
	Node();
	~Node();

	int level;
	int profit;
	int weight;
	double bound;

	bool operator> (Node& no);
	bool operator>= (Node& no);

};

Node::Node(){
	level = 0;
	profit = 0;
	weight = 0;
	bound = 0;
}

Node::~Node(){}

struct item{
	int p;
	int w;
	double e;
};

bool Node::operator> (Node& no){
	if (this->bound > no.bound) {return true;}
	else {return false;}
}

bool Node::operator>= (Node& no){
	if (this->bound < no.bound) {return false;}
	else {return true;}
}

int N, K, maxprofit = 0;
item items[21];

void insertion_ (item arr[]){
	for (int i=2; i<=N; i++){
		double key;
		int j=0;
		item key_i = arr[i];
		key = arr[i].e;

		for (j=i-1; j>0; j--){
			if (arr[j].e < key){
				arr[j+1] = arr[j];
			}else {
				break;
			}
		}
		arr[j+1] = key_i;
	}
}

bool promising (int i, int profit, int weight){
	int j, k;
	int totweight;
	double bound;

	if (weight >= K) return false;

	else {
		j = i+1;
		bound = profit;
		totweight = weight;

		while ((j<=N) && (totweight + items[j].w <= K))
		{
			totweight = totweight + items[j].w;
			bound = bound + items[j].p;
			j++;
		}

		k = j;

		if (k <= N){
			bound = bound + (K-totweight)*items[k].p/items[k].w;
		}
		return bound > maxprofit;
	}
}

double bound(Node u) {
	int j, k;
	int totweight;
	double result;

	if (u.weight >= K){return 0;}

	else {
		result = u.profit;
		j = u.level + 1;
		totweight = u.weight;

		while ((j<=N)&&(totweight + items[j].w <= K)){
			totweight = totweight + items[j].w;
			result = result + items[j].p;
			j++;
		}

		k = j;
		if (k<=N) {result = result + (K - totweight)*items[k].p/items[k].w;}

		return result;
	}
}

void knapsack_1 (int i, int profit, int weight){
	cnt++;
	if ((weight <= K) &&(profit > maxprofit)){maxprofit = profit;}

	if (promising(i, profit, weight)){
		knapsack_1(i+1, profit+items[i+1].p, weight+items[i+1].w);
		knapsack_1(i+1, profit, weight);	
	}
}

void knapsack_2 (void){
	Queue <Node> Q;
	Node u, v;

	Q.Push(v);
	cnt++;

	while (! Q.IsEmpty())
	{
		v = Q.Front();
		Q.Pop();

		u.level = v.level + 1;

		u.profit = v.profit + items[u.level].p;
		u.weight = v.weight + items[u.level].w;

		if ((u.weight <= K)&&(u.profit > maxprofit)) {maxprofit = u.profit;}
		cnt++;
		if (bound(u) > (double)maxprofit) {
			Q.Push(u);
		}

		u.weight = v.weight;
		u.profit = v.profit;
		cnt++;
		if (bound(u)> (double)maxprofit) {
			Q.Push(u);
		}
	}
}

void knapsack_3 (void){
	P_queue<Node> PQ;
	Node u, v;

	v.bound = bound(v);
	cnt++;
	PQ.push(v);

	while (!PQ.isEmpty())
	{
		v = PQ.pop();

		if (v.bound > maxprofit){
			u.level = v.level + 1;

			u.profit = v.profit + items[u.level].p;
			u.weight = v.weight + items[u.level].w;

			if ((u.weight <= K)&&(u.profit > maxprofit)) {maxprofit = u.profit;}

			u.bound = bound(u);
			cnt++;
			if (bound(u) > (double)maxprofit) {
				PQ.push(u);
			}

			u.weight = v.weight;
			u.profit = v.profit;
			u.bound = bound(u);
			cnt++;
			if (bound(u)> (double)maxprofit) {
				PQ.push(u);
			}
		}
	}
}

int main (int argc, char* argv[]){

	cin >> N >> K;

	for (int i=1; i<=N; i++){
		cin >> items[i].w >> items[i].p;
		items[i].e = (double)items[i].p / items[i].w;
	}

	insertion_(items);

	if (!strcmp(argv[1], "backtracking")){
		knapsack_1(0, 0, 0);
	}

	else if (!strcmp(argv[1], "breadth")){
		knapsack_2();
	}

	else if (!strcmp(argv[1], "best")){
		knapsack_3();

	}

	cout << maxprofit << "\n cnt : "<<cnt<<"\n push : "<<cnt_p<< endl;

	return 0;
}