#include<stdio.h>
#include<string.h> 
#include<stdbool.h>

int maxprofit = 0;
int numbest;
int bestset[10];
int include[10];
int N;
int s[3][6];
int V,W;
int p[5];
int w[5];
int n=4;
void knapsack (int i, int profit, int weight);
bool promising(int i, int profit, int weight) {
   int j, k; 
   int totweight; 
   float bound;
   if (weight >= W) 
    return false;

   else {                     
      j = i+1;
   bound = profit; 
   totweight = weight;
   while ((j <= n) && (totweight +w[j] <= W)) {  
      totweight = totweight + w[j]; 
      bound = bound + p[j];
      j++;
   }
   k=j;        
   if (k <= n)  
      bound = bound +(W-totweight)*p[k]/w[k];
   return bound>maxprofit;
}


}
int main(int argc, char *argv[]) { 

    int sum_p=0;
    int sum_w=0;
    for(int i = 0; i < 4; i++) {
        for(int j = 0; j < 2; j++) {
            scanf("%d ", &s[i][j]);// input W[i][j] : i -> j weight
        }
    } 
    V=s[0][0];
    W=s[0][1];
    printf("0번 %d \n", s[3][0]);
    printf("%d %d ",V,W);
    printf("1번 %d \n", s[3][0]);
    p[0]=s[1][0];
    printf("3번 %d \n", s[3][0]);
    p[1]=s[2][0];
    printf("4번 %d \n", s[3][0]);
    p[2]=s[3][0];
    printf("5번 %d \n", s[3][0]);
    p[3]=s[4][0];
    printf("6번 %d \n", s[3][0]);
    printf("7번 %d \n", p[2]);
    for(int j = 0; j < 4; j++) {
        printf("%d \n", s[j+1][0]);
        }
    for(int j = 1; j < 5; j++) {
            w[j-1]=s[j][1];
            printf("%d \n", s[j][1]);
        }
    knapsack(0,p[0],w[0]);
    for(int j=1;j<=n;j++){
        if(bestset[j]==1)
        {
            sum_p+=p[j];
            //sum_w+=w[j];
            //printf("%d %d\n",p[j],w[j]);
        }
        
    }
    printf("가치 합산: %d",sum_p);
} 

void knapsack (int i, int profit, int weight) {
   if (weight <= W && profit > maxprofit) { 
      maxprofit = profit;
      numbest = i; 
        for(int j=1;j<4;j++)
      bestset[i] = include[i]; 
   }
   if (promising(i, profit, weight)) {
      include[i+1] = 1; // Include w[i+1]
      knapsack(i+1, profit+p[i+1], weight+w[i+1]); //p,w
      include[i+1] = 0; // Not include w[i+1]
      knapsack(i+1, profit, weight);
   }
}