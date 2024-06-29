@[toc]

如果我们作为中·华·有·为的HR，想要给那些年龄在20到35岁且年薪要求在10K~20K的技术人员发送面试邀请……这类问题就叫做**二维范围查询** `two-dimensional range query` 。

如果只有一维的情况，我们可以通过递归遍历预先构造的二叉查找树来解决。但是对于二维或者更高维的情况，就需要新的数据结构。

---
# 1. K-D树概念
二维查找树 `two-dimensional search tree` 具有简单的性质：**奇数层**的分支按照第一个键进行，**偶数层**的分支按照第二个键进行；根是任意选取的**奇数层**。对于K维的情况，同样的做法依然成立，通过每层的键进行。下图就是一棵2-d树：
<img src="https://img-blog.csdnimg.cn/20200815172740823.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70" width="54%">

---
# 2. 具体实现
**结点的基本项假设是一个两个元素的数组**。另外，我们允许重复元的出现，并总把它们放在右分支。

## (1) 结点信息
```cpp
template <typename Comparable>
class KdTree {
private:
	struct KdNode {
		vector<Comparable> data;
		KdNode			  *left;
		KdNode			  *right;
		KdNode(const vector<Comparable> &item) : data(item), left(nullptr), right(nullptr) {}	
	};
	KdNode *root;
};
```
## (2) 插入操作
2-d树的插入操作是二叉查找树插入操作的简单扩展，**沿着树下行的时候，我们保留当前的层数信息**，如果是奇数层，就对第0个元素进行键值比较；是偶数层，就对第1个元素进行键值比较。

私有方法和公共接口如下：
```cpp
//插入的公共接口
void insert(const vector<Comparable> &x) {
	insert(x, root, 0);
}
//私有方法, 进行插入
void insert(const vector<Comparable> &x, KdNode *&t, int level) {
	if (t == nullptr) 
		t = new KdNode(x);
	else if (x[level] < t->data[level]) //比较该层的奇数1/偶数0处键值 
		insert(x, t->left, 1 - level);
	else 
		insert(x, t->right, 1 - level);
}
```
很容易就能发现，随机构造的2-d树和随机二叉查找树具有同样的结构化性质：高度平均为 $O(\log N)$ ，但是最坏是 $O(N)$ 。

另外，**不存在已知的树旋转方案保证一棵平衡的2-d树**，基于树旋转的方法在2-d树中行不通。最好的方法是**定期重构子树来保持平衡**。而且，也不存在超越懒惰删除方法的删除算法。

## (3) 范围查询操作
2-d树上可以进行的查询有几种：精确的匹配或者基于两个键之一的匹配(部分匹配查询 `partial match query`) 。这两种都是(**正交**)**范围匹配** `range query` 的特殊情形。

因此我们只完成正交范围查询的函数——它将给出**第一个键在一个特顶的值集合之间**且**第二个键在另一个特定的值集合之间**的所有项。范围查询通过一次递归的树遍历就可以完成：
```cpp
/**
 * 打印满足下列条件的项 
 * low[0] <= x[0] <= high[0] && low[1] <= x[1] <= high[1]	
 */
void printRange(const vector<Comparable> &low, const vector<Comparable> &high) const {
	printRange(low, high, root, 0);
}

//打印范围 
void printRange(const vector<Comparable> &low, const vector<Comparable> &high,
				KdNode *t, int level) const {
	if (t != nullptr) {
		if (low[0] <= t->data[0] && high[0] >= t->data[0] &&
			low[1] <= t->data[1] && high[1] >= t->data[1])
			cout << "(" << t->data[0] << ","
				 		<< t->data[1] << ")" << endl;
		if (low[level] <= t->data[level])
			printRange(low, high, t->left, 1 - level);
		if (high[level] >= t->data[level])
			printRange(low, high, t->right, 1 - level);
	}
}
```
如果想找到特定的项，可以让两个键值的 `low` 都等于 `high` 并且等于要查找的项的对应键值；如果想执行部分匹配查询，让这次匹配**涉及不到的键**的值范围为 $-\infin \to \infin$ ，而其余的范围，则设置低点和高点等于匹配中**涉及到的键**的值。

---
# 3. 数据测试
```cpp
#include <bits/stdc++.h>
using namespace std;

int main() {
	KdTree<int> t;
	cout << "开始测试!" << endl;
	for (int i = 300; i < 370; ++i) {
	 	vector<int> it(2);
	 	it[0] = i;
	 	it[1] = 2500 - i;
	 	t.insert(it);
	}
	vector<int> low(2), high(2);
	low[0] = 70; low[1] = 2186;
	high[0] = 1000; high[1] = 2200; 

	t.printRange(low, high);
    return 0;
}
```
运行如下：
<img src="https://img-blog.csdnimg.cn/20200815180244949.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70" width="45%">

