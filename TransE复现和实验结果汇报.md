## TransE复现和实验结果汇报

#### 1. 伪代码以及代码流程

![](./_image/2024-11-22/838e322172dcc304d9c2f54bcb83017d.png)
代码流程为:
1：初始化： 对于关系按照1的初始化方式初始化即可
2：这里进行了L2范数归一化，也就是除以自身的L2范数
3：同理，也对实体进行了初始化，但是这里没有除以自身的L2范数
4：训练的循环过程中：
5：首先对实体进行了L2范数归一化
6：取一个batch的样本，这里Sbatch代表的是正样本，也就是正确的三元组
7： 初始化三元组对，应该就是创造一个用于储存的列表
8，9，10：这里的意思应该是根据Sbatch的正样本替换头实体或者尾实体构造负样本，然后把对应的正样本三元组和负样本三元组放到一起，组成Tbatch
11：完成正负样本的提取
12：根据梯度下降更新向量
13：结束循环
其中选择L2范数，梯度下降中求导为：
![](./_image/2024-11-22/QQ_1732278348976.png)
#### 部分C++代码展示
![](./_image/2024-11-22/QQ_1732278551224.png)
随机提取正样本，然后随机选择实体进行代替，得到负样本，然后将正负样本一起加入Tbatch中，进行对应的求偏导和梯度下降。
![](./_image/2024-11-22/QQ_1732278676193.png)![](./_image/2024-11-22/QQ_1732278687187.png)
梯度下降的实现，目标是让正样本的loss越小，负样本的loss越大。
![](./_image/2024-11-22/QQ_1732278807578.png)
不断训练，让loss收敛，最后得到一个映射每个实体的向量空间
#### 测试
用训练好的模型做Link prediction，预测三元组中缺失的部分，例如给定关系和尾实体，预测头实体。其本质上都是预测向量。
其中预测的指标为Mean Rank和Hit@10，分为头实体预测和尾实体预测两种。训练集也分为Raw和Filter
![](./_image/2024-11-22/QQ_1732350581282.png)
![](./_image/2024-11-22/QQ_1732350599706.png)
首先进行一次测试，预测三元组的头和尾
![](./_image/2024-11-22/QQ_1732341412703.png)
将得分排名低于1000的三元组标记为正样本，加入到训练集中重新训练，然后再进行一次测试
![](./_image/2024-11-22/QQ_1732346689631.png)
将得分排名低于100的三元组标记为正样本，加入到训练集中
![](./_image/2024-11-22/QQ_1732350315481.png)
可以发现训练完之后，测试的效果越来越好
