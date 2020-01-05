
#include<iostream>
#include<queue>
#include <mshtmlc.h>
using namespace std;




struct Node {
    float weight,value,bound_value;
    int level,nodeRecord[20]={0};
    friend bool operator< (Node a, Node b)//一元运算符重载，应该是top里用，注释掉就报错
    {
        return a.bound_value < b.bound_value;//小于结点就是小于结点里value值得意思
    }
};

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
static float maxValue=0; //最大价值，当前使用重量，以及价值
static float capacity;//背包容量
static int item_n;//物品个数
static priority_queue<Node> prique;


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

void ranking(Items *arr,int head,int tail, bool by_id=False, bool reversed=False){

    if (head < tail){
        int q = partition(arr, head, tail,by_id,reversed);
        //快速排序根据分区点q将问题分解为：q前区域，q后区域两个子问题
        ranking(arr, head, q-1,by_id,reversed); //注意这里是q-1
        ranking(arr, q+1, tail,by_id,reversed);
    }
}


//计算上界 items;depth
float bound(Items items[],float cleft,float bound_value,int d) {
//    float cleft = capacity - cw;//剩余容量
//    float bound_value = cv;
    //以物品单位重量价值递减序装入物品，w[i]已排好
    //注意索引和level/depth的关系

    for(d;(items[d].weight<=cleft && d<item_n ) ;d++) {
        bound_value += items[d-1].value;
        cleft -= items[d-1].weight;
    }
    bound_value +=  ( cleft/items[d-1].weight > 1 ? 1: cleft/items[d-1].weight ) * items[d-1].value;  //防止越界

    return bound_value;
}

void addQueue(Items items[],int weight, int value, int level, int used, int temp[])//入栈函数
{
    // 下一层形成新的节点
    Node node;
    node.weight = weight;
    node.value = value;
    node.level = level;
    node.bound_value = bound(items,capacity-weight,value,level + 1);
//    cout<<"level"<<level<<endl;
//    cout<<"value"<<value<<endl;
//    cout<<"bound_value"<< node.bound_value<<endl;
    for (int j = 0; j < level-1; j++)
        node.nodeRecord[j] = temp[j];//记录新节点的record
    node.nodeRecord[level-1] = used;//是否使用最新的一层，第一层对应第一个物品，索引为0

    //判断是否是叶子结点
    if (level == item_n)
    {
        if (value > maxValue)
        {
            //大于当前最优值，更新数值
            maxValue = value;
            for (int j = 0; j < item_n; j++) {
                items[j].used = node.nodeRecord[j];//更新record
            }
        }
        return;
    }
    else
    {
        //不是叶子结点就更新队列
        prique.push(node);
    }
    return;
}

//最优队列，分支限界
//优先队列： 排的是走到当前节点处累计状态下的物品价值
//已经按性价比从大到小排好顺序，
// 1）尽量拿性价比高的物品，
// 2）如果贪心算法求得的理想值>当前最优值，可以考虑
void bbKnapsack(int depth, Items items []) {
    //初始化空节点
    Node currentNS; //current_node_state
    currentNS.weight = 0;
    currentNS.value = 0;
    currentNS.level = 0;
    currentNS.bound_value = 0;
    currentNS.nodeRecord[currentNS.level] = 0;//currentNS.level其实就是0

    //空节点进入队列
    prique.push(currentNS);

    //循环开始
    do
    {
        if (currentNS.weight + items[currentNS.level].weight <= capacity){
            //进左子树 判断是超重
            int used = 1;
            addQueue(items, currentNS.weight + items[currentNS.level].weight, currentNS.value + items[currentNS.level].value, currentNS.level + 1, used, currentNS.nodeRecord);
        }
        if (bound(items,capacity-currentNS.weight,currentNS.value,currentNS.level + 1) > maxValue) {
            //进右子树 估计上界值是否大于当前最优值
            int used = 0;
            addQueue(items,currentNS.weight, currentNS.value, currentNS.level + 1, used, currentNS.nodeRecord);//进入右子树
        }
        currentNS = prique.top(); //获取队列里最大的当前节点状态
        if(currentNS.bound_value <= maxValue){
            //如果当前的取得队列最大值上界仍然小于最大值，程序结束
            return;
        }
        prique.pop();//出栈
    } while (!prique.empty());//循环至队列为空

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
    item_n=k;//单独给n赋值

    ranking(items,0, item_n-1);
    bbKnapsack(0,items);
    ranking(items,0,item_n-1,True,True);

    for(int ccc=0;ccc<item_n;ccc++) {
//        items[ccc].used = record[ccc];
        cout << items[ccc].used << endl;
//        cout << items[ccc].weight << endl;
//        cout << items[ccc].value << endl;
    }
    ofp=fopen("D:\\A-code\\algorithm\\cpp_pro/bbknapsack_output.txt","wb");
    fprintf(ofp,"%f\r\n",maxValue);
    for(i=0;i<item_n;i++)
    {
        fprintf(ofp,"%d\t%d\t%f\r\n",items[i].id,items[i].used,items[i].value);
    }
    fclose(ofp);
}


int main(){
    Run();
    return 0;
}