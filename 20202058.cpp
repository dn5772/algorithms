#include <iostream>
using namespace std;

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
	P_queue(int pq_capacity = 10));
	~P_queue();

	void push(const T& a);
	T pop();
	bool isEmpty();
	int size();
};

template <typename T>
P_queue<T>::P_queue(int pq_capacity) : capacity(pq_capacity) {
	data = new T[capacity];
	size = 0;
}

template <typename T>
P_queue<T>::~P_queue(){
	delete data[];
}

template <typename T>
void P_queue<T>::push(const T& a){

}

template <typename T>
T P_queue<T>::pop(){

}

template <typename T>
inline bool P_queue<T>::isEmpty(){
	return size == 0;
}

template <typename T>
inline int P_queue<T>::size(){return size;}

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

int N, K, maxprofit = 0;
item items[21];

void insertion_ (item arr[]){
	for (int i=2; i<=N; i++){
		float key;
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

	while (! Q.IsEmpty())
	{
		v = Q.Front();
		Q.Pop();

		u.level = v.level + 1;

		u.profit = v.profit + items[u.level].p;
		u.weight = v.weight + items[u.level].w;
		cout << "1 ^" << u.profit << endl;
		if ((u.weight <= K)&&(u.profit > maxprofit)) {maxprofit = u.profit;}

		if (bound(u) > (double)maxprofit) {
			Q.Push(u);
		}

		u.weight = v.weight;
		u.profit = v.profit;

		if (bound(u)> (double)maxprofit) {
			Q.Push(u);
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
		// quick_ (0, N-1, arr);

	}

	cout << maxprofit << endl;

	return 0;
}