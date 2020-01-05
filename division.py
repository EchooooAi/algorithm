#include<stdio.h>
#define N 100
import numpy as np
# table = np.zeros((0, 0), dtype=np.int)

# table = [[]*8]*8  #注意这个方法不对，外层列表是对内层列表的引用，而非值的复制！！


def scheduled(i,size):
    #i规划表格起始位置/第几个人, size表示解决的大小/n
    global table
    if (size == 1):
        table[i][0] = i+1
        return
    scheduled(i,size//2)  #上半部分
    scheduled(i+size//2,size//2) #下半部分
    # 成功规划出第0,1列
    i_move = i
    while(i_move - i < size//2 ): #右上的循环,以起始位置i开始作对角线，即行需要加上i，列需要减去i
        for upper_right_idx in range(size//2,size):
            table[i_move][upper_right_idx] = table[i + upper_right_idx][i_move-i] #下移，左移
        i_move += 1
    while(i_move - i >=size//2 and i_move - i <size): #右下的循环，可以简单看成左上的位移，移动size//2个位置
        for lower_right_idx in range(size//2,size): #2,3
            table[i_move][lower_right_idx] = table[i_move-size//2][lower_right_idx-size//2] #右下的表格是上移size//2，左移size//2
        i_move += 1


if __name__ == "__main__":
    val = input("输入比赛人数n：")
    try:
        val = int(val)
        table = np.zeros((val,val), dtype=np.int)
        scheduled(0,val)
        print(table)
        print(table.shape)
        np.savetxt('D:/A-code/algorithm/222017321062069_循环赛.txt', table, delimiter='\t', fmt='%d')   # X is an array
        print("saved")
    except:
        print("请输入数字")
