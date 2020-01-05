#include<iostream>
#include<string>
#include <mshtmlc.h>


using namespace std;
static float maxValue=0,cw,cv;//最大价值，当前使用重量，以及价值
static float capacity;//背包容量
int n;//物品个数
int record[20]={0};

struct Items
{
    float weight, value;
    int id,used;

    bool operator < (const Items & a) const
    {
        return (value/weight) < (a.value/a.weight);
    }
    bool operator > (const Items & a) const
    {
        return (value/weight)  > (a.value/a.weight);
    }

};

void swap(Items * arr,int a,int b)
{
    Items temp =  arr[a];
    arr[a] = arr[b];
    arr[b] = temp;
}

int partition(Items *arr, int head, int tail,bool by_id, bool reversed){
    Items pivot = arr[tail];
    //降序中:i记录比支点大的值（即维持i的左边的值比支点大,i指向第一个比支点小的数）
    // j右边记录值比支点  小
    int i = head, j ;
    for (j = head; j < tail; j++){
        if(by_id){ //判断排序依据
            if(reversed){ //判断是否是升序
                if (arr[j].id < pivot.id){//升序
                    swap(arr,i,j);
                    i++;
                }
            }else{//降序
                if (arr[j].id > pivot.id){
                    swap(arr,i,j);
                    i++;
                }
            }
        }else{ //性价比
            if(reversed){ //判断是否是升序
                if (arr[j] < pivot){//升序
                    swap(arr,i,j);
                    i++;
                }
            }else{//降序
                if (arr[j] > pivot){
                    swap(arr,i,j);
                    i++;
                }
            }
        }
    }
    swap(arr,i,tail);
    return i;
}

void ranking(Items *arr,int head,int tail, bool by_id=False,bool reversed=False){

    if (head < tail){
        int q = partition(arr, head, tail,by_id,reversed);
        //快速排序根据分区点q将问题分解为：q前区域，q后区域两个子问题
        ranking(arr, head, q-1,by_id,reversed); //注意这里是q-1
        ranking(arr, q+1, tail,by_id,reversed);
    }
}


//计算上界 items;depth
float bound(Items items[],int d) {
    float cleft = capacity - cw;//剩余容量
    float bound_value = cv;
    //以物品单位重量价值递减序装入物品，w[i]已排好序
    for(d;(items[d].weight<=cleft && d<n ) ;d++) {
        bound_value += items[d].value;
        cleft -= items[d].weight;
    }
    bound_value +=  ( cleft/items[d].weight ) * items[d].value;

    return bound_value;
}



void backtrack(int depth, Items items []) {//回溯算法
    //叶子结点返回条件
    if (depth >= n) {//到达叶结点
        if(cv>maxValue){
            maxValue = cv;
            for(int i=0;i<n;i++){
                items[i].used = record[i];
            }
        }
        return;
    }
    //没有达到叶子节点
    if (cw + items[depth].weight <= capacity) {//进入左子树
        cw += items[depth].weight;
        cv += items[depth].value;
        record[depth]=1;
        backtrack(depth+1,items);
        //退出左子树搜索
        cw -= items[depth].weight ;
        cv -= items[depth].value;
    }
    if (bound(items, depth+1) > maxValue) {//进入右子树
        record[depth]=0;
        backtrack(depth+1,items);
    }
}


void Run(){
    Items items[20];
    int i,k=0;
    FILE *ifp,*ofp;
    ifp=fopen("D:\\A-code\\algorithm\\cpp_pro/backtrack_input.txt","r");


    fscanf(ifp,"%f\r\n",&capacity);  //实现记事本换行\r 回车符\n换行

    while(!feof(ifp)){

        fscanf(ifp,"%d\t%f\t%f\r\n",&items[k].id,&items[k].weight,&items[k].value);
        k++;
    }


    fclose(ifp);
    n=k;//单独给n赋值

    ranking(items,0, n-1);
    backtrack(0,items);
    ranking(items,0,n-1,True,True);

    for(int ccc=0;ccc<n;ccc++) {
//        items[ccc].used = record[ccc];
//        cout<<record[ccc]<<endl;
        cout << items[ccc].used << endl;
//        cout << items[ccc].weight << endl;
//        cout << items[ccc].value << endl;
    }
    ofp=fopen("D:\\A-code\\algorithm\\cpp_pro/backtrack_output.txt","wb");
    fprintf(ofp,"%f\r\n",maxValue);
    for(i=0;i<n;i++)
    {
        fprintf(ofp,"%d\t%d\t%f\r\n",items[i].id,items[i].used,items[i].value);
    }
    fclose(ofp);
}


int main(){
    Run();
    return 0;
}