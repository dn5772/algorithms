#include<stdio.h>
#include<string.h>
#include<stdbool.h>
#include<stdlib.h>

int maxprofit;
int n, k, w[21], v[21];
double x[21];

struct node {
   int level, profit, weight;
   float bound;
};

typedef struct Queue {
   struct node buf[110000];
   int qsize;
   int front;
   int rear;
}Queue;

typedef struct PriorityQueue {
   struct node heap[10000];
   int count;
}PriorityQueue;

void nodeChange(struct node* a, struct node* b) {
   struct node temp = *a;
   *a = *b;
   *b = temp;
}

void InitPQueue(PriorityQueue* PQ) {
   PQ->count = 0;
}

void InsertPQ(PriorityQueue* root, struct node data) {
   root->heap[root->count] = data;
   printf("w:%d p:%d x:%f\n", data.weight, data.profit, data.bound);
   int now = root->count;
   int parent = (root->count - 1) / 2;
   while ((parent >= 0) && (root->heap[now].bound > root->heap[parent].bound)) {
      nodeChange(&root->heap[now], &root->heap[parent]);
      now = parent;
      parent = (parent - 1) / 2;
   }
   root->count++;
}

struct node RemovePQ(PriorityQueue* root) {
   struct node res = root->heap[0];
   root->count--;
   root->heap[0] = root->heap[root->count];
   int now = 0, leftChild = 1, rightChild = 2;
   int target = now;
   while (leftChild < root->count) {
      if (root->heap[leftChild].bound < root->heap[leftChild+1].bound) leftChild++;
      if (root->heap[now].bound >= root->heap[leftChild].bound) break;
      else {
         nodeChange(&root->heap[now], &root->heap[leftChild]);
         now = leftChild;
      }
      leftChild = now * 2 + 1;
      rightChild = now * 2 + 2;
   }
   return res;
}

bool empty(PriorityQueue* PQ) {
   return PQ->count == 0;
}

int SizeofQueue(Queue* q) {
   return sizeof(q->buf) / sizeof(struct node);
}

void InitQueue(Queue* q) {
   q->rear = 0;
   q->front = 0;
}

bool IsFull(Queue* q) {
   return ((q->rear + 1) % q->qsize == q->front);
}

bool IsEmpty(Queue* q) {
   return (q->front == q->rear);
}

void Enqueue(Queue* q, struct node data) {
   q->rear = (q->rear + 1);
   q->buf[q->rear] = data;
}

struct node Dequeue(Queue* q) {
   q->front = q->front + 1;
   return q->buf[q->front];
}

void insertion(int s, int e) {
   int tmpw, tmpv;
   double key;
   int i, j;
   for (i = s + 1; i <= e; i++) {
      key = x[i];
      tmpw = w[i];
      tmpv = v[i];
      for (j = i - 1; j > 0 && x[j] < key; j--) {
         x[j + 1] = x[j];
         w[j + 1] = w[j];
         v[j + 1] = v[j];
      }
      x[j + 1] = key;
      w[j + 1] = tmpw;
      v[j + 1] = tmpv;
   }
}

bool promising(int i, int profit, int weight) {
   int j, l;
   int totweight;
   float bound;
   if (weight >= k) {
      return false;
   }
   else {
      j = i + 1;
      bound = (float)profit;
      totweight = weight;
      while ((j <= n) && (totweight + w[j] <= k)) {
         totweight = totweight + w[j];
         bound = bound + v[j];
         j++;
      }
      l = j;
      if (l <= n)
         bound = bound + (k - totweight) * (float)v[l] / (float)w[l];

      return bound > maxprofit;
   }
}

void backtracking(int i, int profit, int weight) {
   if (weight <= k && maxprofit < profit) {
      maxprofit = profit;
   }

   if (promising(i, profit, weight)) {
      backtracking(i + 1, profit + v[i + 1], weight + w[i + 1]);
      backtracking(i + 1, profit, weight);
   }
}

float bound(struct node u) {
   int l, j, totweight;
   float result;
   if (u.weight >= k) {
      return 0;
   }
   else {
      result = u.profit;
      j = u.level + 1;
      totweight = u.weight;
      while ((j <= n) && (totweight + w[j] <= k)) {
         totweight = totweight + w[j];
         result = result + v[j];
         j++;
      }
      l = j;
      if (l <= n)
         result = result + (k - totweight) * v[l] / w[l];
      return result;
   }
}

void breadth_first() {
   Queue* queue = malloc(sizeof(Queue));
   struct node u, o;
   InitQueue(queue);
   o.level = 0;
   o.profit = 0;
   o.weight = 0;
   maxprofit = 0;
   Enqueue(queue, o);
   while (!IsEmpty(queue)) {
      o = Dequeue(queue);
      u.level = o.level + 1;
      u.profit = o.profit + v[u.level];
      u.weight = o.weight + w[u.level];
      if ((u.weight <= k) && (u.profit > maxprofit)) maxprofit = u.profit;
      if (bound(u) > maxprofit) Enqueue(queue, u);
      u.weight = o.weight;
      u.profit = o.profit;
      if (bound(u) > maxprofit) Enqueue(queue, u);
   }
}

void bestfirst() {
   PriorityQueue* PQ = malloc(sizeof(PriorityQueue));
   struct node u, o;
   InitPQueue(PQ);
   o.level = 0;
   o.profit = 0;
   o.weight = 0;
   o.bound = bound(o);
   maxprofit = 0;
   InsertPQ(PQ, o);
   while (!empty(PQ)) {
      o = RemovePQ(PQ);
      if (o.bound > maxprofit) {
         u.level = o.level + 1;
         u.profit = o.profit + v[u.level];
         u.weight = o.weight + w[u.level];
         if ((u.weight <= k) && (u.profit > maxprofit))
            maxprofit = u.profit;
         u.bound = bound(u);
         if (bound(u) > (double)maxprofit)
            {InsertPQ(PQ, u);}
         u.weight = o.weight;
         u.profit = o.profit;
         u.bound = bound(u);
         if (bound(u) > (double)maxprofit) {
            InsertPQ(PQ, u);
         }
      }
   }
}

int main(int argc, char* argv[]) {

   scanf("%d %d", &n, &k);

   for (int i = 1; i <= n; i++) {
      scanf("%d %d", &w[i], &v[i]);
      x[i] = (double)v[i] / (double)w[i];
   }

   insertion(1, n);

   int profit = 0, weight = 0;

   if (strcmp(argv[1], "backtracking") == 0) {
      backtracking(0, profit, weight);
   }
   else if (strcmp(argv[1], "breadth") == 0) {
      breadth_first();
   }
   else if (strcmp(argv[1], "best") == 0) {
      bestfirst();
   }

   printf("%d\n", maxprofit);

}