import numpy as np

def kapsack(V,W,capacity=5):
    n_item = len(V)
    M = np.zeros((n_item+1,capacity+1))
    for k in range (1,n_item+1):
        for cc in range(1,capacity+1):
            if(W[k-1]>cc): #current_capacity 当前规划的背包容量; k-1表示索引对应的物品应该左移一位：涉及到V,W,item_checked
                M[k,cc] = M[k-1,cc]
            else: ## cc-weight_currrent > 0 当前背包容量可以容纳当前考虑的物品
                add = M[k-1,cc-W[k-1]] + V[k-1] #cc-W[k]表明考虑k-1个物品时的容量
                no_add = M[k-1,cc]
                if add>no_add:
                    M[k][cc] = add
                else:
                    M[k][cc] = no_add
    'Item checked:\n',
    #反推过程
    M_value = M[n_item,capacity]
    cc_pos = capacity
    k_pos = n_item-1 #比较上一个物品
    item_checked = np.zeros(n_item)
    while(k_pos>=0 ):
        if M_value != M[k_pos,cc_pos]: #加入了当前考虑的物品
            item_checked[k_pos] = 1  #已经左移一位
            cc_pos -= W[k_pos]  #上一个物品所处得背包容量位置
            M_value -= V[k_pos]

        k_pos -= 1 #考虑上一个物品
    return[M,item_checked]

if __name__ == "__main__":
    # W_array = np.array([2,1,3,2])
    # V_array = np.array([12,10,20,15])
    with open('D:/A-code/algorithm/knapsack_input.txt', 'r') as f:
        capacity = f.readline()
    (W_array, V_array) = np.loadtxt('D:/A-code/algorithm/knapsack_input.txt', dtype=int, skiprows=1, comments='#', delimiter='\t', usecols=(1, 2), unpack=True)
    M_table, item_checked = kapsack(V_array,W_array,capacity=int(capacity))
    print('M_table:\n',M_table)
    print('Item checked:\n',item_checked)
    output = np.concatenate([np.arange(1,len(V_array)+1).reshape(-1,1),item_checked.reshape(-1,1),V_array.reshape(-1,1)],axis=1)
    print('output:\n',output)
    with open('knapsack_output.txt', 'w') as f:
        f.write('%i\n#ID\tChoice\tValue\n'%(np.max(M_table)))
        np.savetxt(f, output, delimiter='\t',fmt='%i')