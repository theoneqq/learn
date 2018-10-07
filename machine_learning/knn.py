import numpy as np
from collections import Counter
import matplotlib.pyplot as plt
import sklearn
import pandas

def knn(train_set, train_class, predict, k = 3):
    #for train_set_row in train_set:
        #dst = sqrt(np.sum((train_set_row - predict) ** 2))
    distance = [np.sqrt(np.sum((train_set_row - predict) ** 2)) for train_set_row in train_set]#利用np对象对运算符类似重载的操作
    nearest = np.argsort(distance)#排序得到数据的索引
    nearest_k_class = [train_class[i] for i in nearest[:k]]
    votes = Counter(nearest_k_class) #统计数据的分类情况
    return votes.most_common(1)[0][0] #取出类别最多的数据信息

train_set = np.array([
    [3.393533211, 2.331273381],
    [3.110073483, 1.781539638],
    [1.343808831, 3.368360954],
    [3.582294042, 4.679179110],
    [2.280362439, 2.866990263],
    [7.423436942, 4.696522875],
    [5.745051997, 3.533989803],
    [9.172168622, 2.511101045],
    [7.792783481, 3.424088941],
    [7.939820817, 0.791637231]
    ])
train_class = np.array([0, 0, 0, 0, 0, 1, 1, 1, 1, 1])
predict = np.array([9, 9])

plt.scatter(train_set[train_class == 0, 0], train_set[train_class == 0, 1], color='green')#散点图
plt.scatter(train_set[train_class == 1, 0], train_set[train_class == 1, 1], color='black')
plt.scatter(predict[0], predict[1], color='red')
plt.show()

predict_class = knn(train_set, train_class, predict)
print('predict class:', predict_class)