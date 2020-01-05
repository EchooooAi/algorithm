
#include<iostream>
#include<queue>
using namespace std;

int track[20];//记录取不取的轨迹数组

struct treenode {
    int weight;
    int value;
    int level;
    int flag[20];//4为n+1手动改，记录轨迹
    friend bool operator< (treenode a, treenode b)//一元运算符重载，应该是top里用，注释掉就报错
    {
        return a.value < b.value;//小于结点就是小于结点里value值得意思
    }
};

priority_queue<treenode> prique;

void enPriQueue(int weight, int value, int level, int flag, int n, int* bestValue, int temp[])//入栈函数
{
    treenode node;
    node.weight = weight;
    node.value = value;
    node.level = level;
    for (int j = 0; j < level; j++)
        node.flag[j] = temp[j];//把自己父节点的flag转移到自己创建的新的flag数组里
    node.flag[level] = flag;//自己结点的flag取1还是0，通过传入的flag识别
    if (level == n)//判断是否是叶子结点
    {
        if (value > *bestValue)
        {
            for (int j = 1; j < n + 1; j++)
                track[j] = node.flag[j];//更新track数组，把叶子结点flag数组里记录取不取的值传入track数组中
            *bestValue = value;
        }
        return;
    }
    else
    {
        prique.push(node);//不是叶子结点就入栈
    }
    return;
}

int prioritybbnap(int w[], int v[], int c, int n, int* bestValue)
{
    treenode liveNode;
    liveNode.weight = 0;
    liveNode.value = 0;
    liveNode.level = 0;
    liveNode.flag[liveNode.level] = 0;//liveNode.level其实就是0
    prique.push(liveNode);//把A入栈
    do
    {
        if (liveNode.weight + w[liveNode.level] <= c)//是否能进左子树
        {
            enPriQueue(liveNode.weight + w[liveNode.level], liveNode.value + v[liveNode.level], liveNode.level + 1, 1, n, bestValue, liveNode.flag);
        }
        enPriQueue(liveNode.weight, liveNode.value, liveNode.level + 1, 0, n, bestValue, liveNode.flag);//进入右子树
        liveNode = prique.top();//把队列里value最大的赋值给liveNode
        prique.pop();//出栈
    } while (!prique.empty());//空就停止循环
    return 0;
}

void IO() {
    int w[20], v[20], c, n, bestValue = 0, i, k = 0, num[20];
    FILE *ifp, *ofp;
    ifp = fopen("input.txt", "r");
    fscanf(ifp, "%d\r\n", &c);// /r/n实现记事本换行/r回车符/n换行
    while (!feof(ifp)) {
        fscanf(ifp, "%d\t%d\t%d\r\n", &num, &w[k], &v[k]);
        k++;
    }
    fclose(ifp);
    n = k;//单独给n赋值
    printf("文件数据已成功读取...\n");
    prioritybbnap(w, v, c, n, &bestValue);
    ofp = fopen("output.txt", "wb");
    fprintf(ofp, "%d\r\n", bestValue);
    for (i = 0; i<n; i++)
    {
        fprintf(ofp, "%d\t%d\t%d\r\n", (i + 1), track[i+1], v[i]);
    }
    fclose(ofp);
    printf("结果已输出到本目录文件\"output.txt\"中...\n");
}

int main()
{
    IO();
    system("pause");
    return 0;
}