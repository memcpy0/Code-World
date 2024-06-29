> 本文属于「算法学习」系列文章的汇总目录。之前的「数据结构和算法设计」系列着重于基础的数据结构和算法设计课程的学习，与之不同的是，这一系列主要用来记录大学课程范围之外的**高级算法学习、优化与应用的全过程**，同时也将归纳总结出简洁明了的算法模板，以便记忆和运用。在本系列学习文章中，为了透彻理解算法和代码，本人参考了诸多博客、教程、文档、书籍等资料，由于精力有限，恕不能一一列出，这里只列示重要资料的不完全参考列表：
> - 算法竞赛进阶指南，李煜东，河南电子音像出版社，[GitHub Tedukuri社区](https://github.com/lydrainbowcat/tedukuri)以及[个人题解文章汇总目录](https://memcpy0.blog.csdn.net/article/details/121280121)
> 
> <b></b>
> 为了方便在PC上运行调试、分享代码，我还建立了相关的[仓库](https://github.com/memcpy0/Algorithm-Templates)。在这一仓库中，你可以看到算法文章、模板代码、应用题目等等。由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[算法学习系列文章目录](https://memcpy0.blog.csdn.net/article/details/117125688)一文以作备忘。

@[toc]

---
**Treap**是一种平衡二叉查找树，也是比AVL树简单的一种树，它像跳跃表一样**使用随机数，并对任意的输入都给出 $\text{O(logN)}$ 期望时间的性能**——它能动态维护一个有序表，支持在 $O(\log n)$ 的时间内，完成查找/插入/删除一个元素、查找第 $k$ 大元素等任务。

它的查找时间等同于非平衡二叉查找树，插入时间只比递归非平衡二叉树查找的实现稍慢，删除更是慢得多，但仍然是 $\text{O(logN)}$ 的期望时间。


# 1. Treap树介绍
Treap很简单，不用画图就可以描述：
- 树中每个结点存储数据、一个左指针、一个右指针，以及一个优先级，**优先级在建立结点时随机指定**；
- Treap的**键值满足二叉查找树的所有性质**，或者说Treap就是二叉查找树；
- Treap的**结点优先级满足堆序性**，任意结点的优先级必须至少和它父亲的优先级一样大，根结点的优先级最小。

每一项都有不同的优先级的不同项的集合只能有一棵Treap树表示。结点的声明很简单，其中标记结点 `nullnode` 的优先级为 $\infin$ 。

---
# 2. Treap树代码
Treap的类接口、构造函数和部分简单函数：
```cpp
/* Treap Class
PUBLIC:
void insert(x)		    插入x 
void remove(x)			删除x 
bool contains(x)		如果x存在返回true 
Comparable findMin()	返回最小值 
Comparable findMax()	返回最大值
bool empty()			判断是否为空 
void makeEmpty() 		删除所有项 
void printTree()		有序打印整棵树
ERRORS:
Throws underflow_error as warranted 
*/
template <typename Comparable>
class Treap {
public:
	//默认构造函数 
	Treap() {
		nullNode = new TreapNode;
		nullNode->left = nullNode->right = nullNode;
		nullNode->priority = INT_MAX;
		root = nullNode;
		//修改随机数的范围,其他结点随机到的优先级[1,INT_MAX-1]
		uniform_int_distribution<unsigned>::param_type param{1, INT_MAX - 1}; 
		u.param(param); 
	}
	//复制构造函数 
	Treap(const Treap &rhs) : Treap() {
		*this = rhs;							//使用赋值运算符函数复制rhs 
	}
	//析构函数 
	~Treap() { 
		makeEmpty();
		delete nullNode;
	}
	//赋值运算符函数 
	Treap& operator=(const Treap &rhs) {		//赋值运算符函数 
		if (this != &rhs) {
			makeEmpty();
			root = clone(rhs.root);				//最终使用clone进行复制 
		}
		return *this;
	}
	//判断是否为空树 
	bool empty() const {
		return root == nullNode;
	}
	//打印中序序列 
	void printTree() const {
		if (empty()) cout << "<Empty Tree>" << endl;
		else printTree(root); 					//使用私有方法输出树的序列 
	}
	//调用私有方法清空整棵树 
	void makeEmpty() {
		makeEmpty(root);
	}
//.........................................................................
private: 
	struct TreapNode {
		Comparable data;  						//数据
		TreapNode *left;						//左指针 
		TreapNode *right;						//右指针 
		unsigned int priority;					//优先级
		
		TreapNode() : left(nullptr), right(nullptr), priority(INT_MAX) { }
		TreapNode(const Comparable &d, TreapNode *lt, TreapNode *rt, unsigned int pr)
			: data(d), left(lt), right(rt), priority(pr)
			{ } 
	};
	TreapNode *root;
	TreapNode *nullNode;  
	default_random_engine e;              //u(e)随机数生成器  
	uniform_int_distribution<unsigned> u; //在构造函数中改变范围为min:1 max:2147483646
	
	//清空一棵树
	void makeEmpty(TreapNode *&t) {
		if (t != nullNode) {
			makeEmpty(t->left);
			makeEmpty(t->right);
			delete t;
		}
		t = nullNode;
	}
	//中序递归打印Treap
	void printTree(TreapNode *t) const {
		if (t != nullNode) {
			printTree(t->left);
			cout << t->data << endl;
			printTree(t->right);
		}
	}
	//复制一棵树 
	TreapNode *clone(TreapNode *t) const {
		if (t == t->left) // Cannot test against nullNode!!!
			return nullNode;
		else 
			return new TreapNode(t->data, clone(t->left), clone(t->right), t->priority);
	}
	
	//LL型,右旋
	void rotateWithLeftChild(TreapNode *&k2) {
		TreapNode *k1 = k2->left;
		k2->left = k1->right;
		k1->right = k2;
		k2 = k1; 
	}
	//RR型,左旋
	void rotateWithRightChild(TreapNode *&k1) {
		TreapNode *k2 = k1->right;
		k1->right = k2->left;
		k2->left = k1;
		k1 = k2;
	}
};
```
---

## (1) 插入操作
Treap树的插入操作比AVL树简单一些，如果插入值 `x` 小于当前结点的键值时往左子树插入；如果大于当前结点的键值则往右子树插入。如果相等则不插入，因为我们的实现**假设没有重复元素**，如果存在重复元的话，`remove` 就麻烦许多。

当前结点为标记结点 `nullNode` 时，表示来到空树处，此时将 `x` 作为树叶结点加入，**使用随机数生成器随机给它生成一个优先级**，同时该叶子结点的 `left = right = nullNode` 。

结点生成和插入结束后递归返回，判断是否满足堆序性：
- 对于插入到左子树的情况，需要判断当前结点和其左子树的优先级，如果**当前结点的优先级大于左子树的优先级**，需要对当前结点进行一次**右旋**；
- 对于插入到右子树的情况，需要判断当前结点和其右子树的优先级，如果**当前结点的优先级大于右子树的优先级**，需要对当前结点进行一次**左旋**；

这样沿着插入路径向上不断判断和旋转，**直到优先级满足堆序为止**。虽然还是需要旋转，但是没有了AVL树的**平衡因子、双旋转**等，稍微简单一些。代码如下，包括公共接口和私有方法：
```cpp
//调用私有方法插入x 
void insert(const Comparable &x) {
	insert(x, root);
}

/**递归方法 
 * 内部方法,插入x进入Treap的子树 
 * x是要插入的数据 
 * t是树/子树的根结点
 * 设置子树新的根
 * u(e)是一个随机数生成器 
 */
void insert(const Comparable &x, TreapNode *&t) {
	if (t == nullNode) 
		t = new TreapNode(x, nullNode, nullNode, u(e)); //随机赋值优先级 
	else if (x < t->data) {
		insert(x, t->left);
		if (t->left->priority < t->priority) //t的优先级>左子树优先级 
			rotateWithLeftChild(t);			 //右旋with t的左子树
	} 
	else if (t->data < x) {
		insert(x, t->right);
		if (t->right->priority < t->priority)//t的优先级>右子树优先级 
			rotateWithRightChild(t);         //左旋with t的右子树
	}
	//else duplicate;   //do nothing 
} 
```
## (2) 删除操作
删除操作要麻烦一点：
- 首先，根据键值的大小往左右子树递归寻找要删除的项。
- 在找到后，通过把它的优先级(象征性)增加到 $\infin$ ，并沿着**低优先级儿子**的路径向下旋转。需要注意的是，如果 `x` 的结点逻辑上是叶子结点时，它仍有 `nullNode` 作为左右子树，按照代码， `x` 的结点会与右子树被左旋，旋转后 `t` 为 `nullNode` ，而要被删除的项成为 `t` 的左子树。
- 随后，如果新的当前子树根结点 `t` 不是 `nullNode` 标记结点的话，就递归往新的子树删除 `x` ；
- 否则，直接删除 `t->left` ，就可以把 `x` 所在的结点删除，然后记得 `t->left = nullNode;` 。

代码的公共接口和私有方法如下：
```cpp
//调用私有方法移除x 
void remove(const Comparable &x) {
	remove(x, root);
}

/**递归方法 
 * 内部方法,从子树中删除x 
 * x是要删除的数据 
 * t是树/子树的根结点
 * 设置子树新的根 
 */
void remove(const Comparable &x, TreapNode *&t) {
	if (t == nullNode)    return;
	if (x < t->data) 	  remove(x, t->left); 
	else if (t->data < x) remove(x, t->right);
	else { //Match data found
		//将优先级更小的子树旋转到子树根结点 
		if (t->left->priority < t->right->priority) 
			rotateWithLeftChild(t);  //右旋 
		else 
			rotateWithRightChild(t); //左旋 
		//在新的子树中递归删除x 
		if (t != nullNode) 
			remove(x, t);
		//如果t已经是nullNode,直接删除 
		else {
			delete t->left;
			t->left = nullNode;
		}
	} 
}
```
---
## (3) 找最大值、最小值
在最困难的插入、删除操作完成后，我们看一下其他的简单操作，比如寻找最大值和最小值：
```cpp
//找到最小值 
const Comparable &findMin() const { 
	if (empty()) 
		throw underflow_error("Empty Tree");
	TreapNode *ptr = root;
	while (ptr->left != nullNode) 
		ptr = ptr->left;
	return ptr->data;
}
//找到最大值 
const Comparable &findMax() const {
	if (empty()) 
		throw underflow_error("Empty Tree");
	TreapNode *ptr = root;
	while (ptr->right != nullNode) 
		ptr = ptr->right;
	return ptr->data;
}
```
## (4) 判断是否包含某值
`contains()` 方法如下：
```cpp
//判断是否包含x 
bool contains(const Comparable &x) const {
	TreapNode *current = root;
	nullNode->data = x;
	while (true) {
		if (x < current->data)
			current = current->left;
		else if (current->data < x)
			current = current->right;
		else
			return current != nullNode;
	}
}
```
至此，基本的Treap已经实现完成，它比AVL容易许多，因为我们不必担心调整 `priority` 成员，而且也不必向AVL树一样及时更新结点的信息。

---
# 3. 数据测试
代码如下：
```cpp
//Test Treap 
int main() {
    Treap<int> t;
    int NUMS = 200000;
	const int GAP = 37; 
	cout << "检查...(没有多余的输出意味着成功)" << endl; 
	//插入数据1-199999
	vector<int> vi; 
	for (int i = GAP; i != 0; i = (i + GAP) % NUMS)	 
		t.insert(i);
	//删除奇数 
	for (int i = 1; i < NUMS; i += 2) 
		t.remove(i);
		 
	if (NUMS < 40)
		t.printTree();
	//如果最小值不为2或者最大值不为NUMS-2
	//最小值不会是0或者1,最大值不会是NUMS-1和NUMS 
	if (t.findMin() != 2 || t.findMax() != NUMS - 2)
		cout << "FindMin or FindMax error!" << endl;
	//不包含偶数,说明出错 
	for (int i = 2; i < NUMS; i += 2)
		if (!t.contains(i))
			cout << "Find error1!" << endl;
	//包含奇数,说明出错 
	for (int i = 1; i < NUMS; i += 2)
		if (t.contains(i))
			cout << "Find error2!" << endl;
	//赋值运算符函数、clone函数 
	Treap<int> t2;
	t2 = t;
	//不包含偶数,说明出错 
	for (int i = 2; i < NUMS; i += 2)
		if (!t2.contains(i))
			cout << "Find error1!" << endl;
	//包含奇数,说明出错 
	for (int i = 1; i < NUMS; i += 2)
		if (t2.contains(i))
			cout << "Find error2!" << endl;	 
	cout << "测试完成" << endl;
    return 0;
}
```
运行如下：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200812220713147.png#pic_center)


---

[【PAT】2021年秋季PAT甲级题解 Sorted Cartesian Tree (30 分)](https://memcpy0.blog.csdn.net/article/details/121863753)
