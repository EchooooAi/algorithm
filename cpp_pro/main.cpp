#include<iostream>
#include<fstream>
#include<string>
#include<sstream>

using namespace std;


struct Items
{
    float weight, value,used_value;
    int id;

    bool operator < (const Items & a) const
    {
        return (value/weight) < (a.value/a.weight);
    }
    bool operator > (const Items & a) const
    {
//        cout<< value<<"/"<<weight<< ":"<<(value/weight) <<">?"<<(a.value/a.weight)<<endl;
//        cout<<((value/weight)  > (a.value/a.weight))<<endl;
        return (value/weight)  > (a.value/a.weight);
    }

};

void swap(Items * arr,int a,int b)
{
    Items temp =  arr[a];
    arr[a] = arr[b];
    arr[b] = temp;
}

int partition(Items *arr, int head, int tail ){
    Items pivot = arr[tail];
    //降序中:i记录比支点大的值（即维持i的左边的值比支点大,i指向第一个比支点小的数）
    // j右边记录值比支点  小
    int i = head, j ;
    for (j = head; j < tail; j++){
        if (arr[j] > pivot){
            swap(arr,i,j);
            i++;
        }
    }
//    cout<<"swap  "<<i<<" and  "<<tail<<endl;
    swap(arr,i,tail);
    return i;
}

void ranking(Items *arr,int head,int tail){
//    cout<<"head "<<head<<"  tail:"<<tail<<endl;
// if (head>=tail) return;
    if (head < tail){
        int q = partition(arr, head, tail);
//        cout<<"q: "<<q<<endl;
        //快速排序根据分区点q将问题分解为：q前区域，q后区域两个子问题
        ranking(arr, head, q-1); //注意这里是q-1
        ranking(arr, q+1, tail);
    }
}

float greedy (Items * items, float capacity)
{
    //W,V已经排好序
    float maxValue=0;
    int i;
    for(i=0;items[i].weight<capacity;i++) {
        items[i].used_value = items[i].value;
        maxValue += items[i].used_value ;
        capacity = capacity - items[i].weight;
    }
    items[i].used_value  = ( capacity/items[i].weight ) * items[i].value;
    maxValue += items[i].used_value ;
    return maxValue;
}



float capacity;//背包容量
int n;//物品个数
float MaxValue=0;//背包容量为C时最大价值
Items items[20];  //如果不提前指定，可以打开两次文件，第一次扫描确定n，或者在文件中输入n；或者像这样多分配的空间
void Run(){
    int i,k=0;
    FILE *ifp,*ofp;
    ifp=fopen("D:\\A-code\\algorithm\\cpp_pro/greedy_input.txt","r");


    fscanf(ifp,"%f\r\n",&capacity);  //实现记事本换行\r 回车符\n换行

    while(!feof(ifp)){

        fscanf(ifp,"%d\t%f\t%f\r\n",&items[k].id,&items[k].weight,&items[k].value);
        k++;
    }


    fclose(ifp);
    n=k;//单独给n赋值

    ranking(items,0, n-1);
    for(int ccc=0;ccc<n;ccc++){
        cout<<items[ccc].id<<endl;
        cout<<items[ccc].weight<<endl;
        cout<<items[ccc].value<<endl;
    }
    MaxValue = greedy(items,capacity);
    ;
    ofp=fopen("D:\\A-code\\algorithm\\cpp_pro/greedy_output.txt","wb");
    fprintf(ofp,"%f\r\n",MaxValue);
    for(i=0;i<n;i++)
    {
        fprintf(ofp,"%d\t%f\t%f\r\n",items[i].id,items[i].weight,items[i].used_value);
    }
    fclose(ofp);
}


int main(){
    Run();
    return 0;
}