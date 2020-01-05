#include<iostream>
#include<fstream>
#include<string>
#include<sstream>


using namespace std;


struct Items
{
    float weight, value;
    int id;

    bool operator < (const item & a) const
    {
        return (value/weight) < (a.value/a.weight);
    }
    bool operator > (const item & a) const
    {
        return (value/weight)  > (a.value/a.weight);
    }

};

void swap(Items * arr,int a,int b)
{
    float temp = arr[a];
    arr[a] = arr[b];
    arr[b] = temp;
}

int partition(Items *arr, int head, int tail ){
    int pivot = arr[tail];
    //降序中:i记录比支点大的值（即维持i的左边的值比支点大）
    // j右边记录值比支点  小
    int i = head, j ;
    for (int j = head; j < tail-1; j++){
        if (arr[j] > pivot){
            swap(arr,i,j);
            i++;
        }
    }
    swap(arr,i,tail);
    return i;
}

void rank(Items *arr,int head,int tail){
    if (head < tail){
        int q = partition(arr, head, tail);
        //快速排序根据分区点q将问题分解为：q前区域，q后区域两个子问题
        rank(arr, head, q);
        rank(arr, q+1, tail);
    }
    return 0;
}

float greedy (Items * items,int capacity)
{
    //W,V已经排好序
    float used_value[10]={0};
    float maxValue=0;
    int i;
    for(i=0;items[i].weight<capacity;i++) {
        used_value[i] = items[i].value;
        maxValue += items[i].value;
        capacity = capacity - items[i].weight;
    }
        used_value[i]=( (float)capacity/items[i].weight ) * items[i].value;
        maxValue+=used_value[i];
        return maxValue;
}


int v[50];//价值
int w[50];//重量
int capacity;//背包容量
int n;//物品个数

float MaxValue=0;//背包容量为C时最大价值

Items items[3];

void Run(){
    int i,k=0,num[20];
    FILE *ifp,*ofp;
    ifp=fopen("greedy_input.txt","r");
    fscanf(ifp,"%d\r\n",&capacity);  //实现记事本换行\r 回车符\n换行
    while(!feof(ifp)){
        fscanf(ifp,"%d\t%d\t%d\r\n",&items[k].id,&items[k].weight,&items[k].value);
        k++;
    }
    fclose(ifp);
    n=k;//单独给n赋值

    rank(items,0,n);
    MaxValue = greedy(items,capacity);
    DProcessing();

    ofp=fopen("greedy_output.txt","wb");
    fprintf(ofp,"%d\r\n",MaxValue);
    for(i=1;i<=n;i++)
    {
        fprintf(ofp,"%d\t%d\t%d\t%d\r\n",i,selected[i],v[i],w[i]);
    }
    fclose(ofp);
}


int main(){
    Run();
    return 0;
}