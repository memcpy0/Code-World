@[toc]

---
# 1. AVL树概念
AVL树是带有平衡条件 `balance condition` 的二叉搜索树。平衡条件往往和高度有关，**每个结点都会在其结构中保存高度信息**。空树的高度定义为 `-1` 。

最简单的想法是要求根的左右子树有相同的高度，但是仅仅要求在根结点平衡是不够的；更激烈的想法是要求每个结点都必须有相同高度的左子树和右子树，不过这样就只有具有 $2^k - 1$ 个结点的理想平衡树满足这个条件，太严格以至于无法使用。

最终的定义是：一棵**AVL树**是其**每个结点的左子树和右子树的高度最多差 $1$** 的二叉搜索树。下图的左边是AVL树，右边则不是：
<img src="https://img-blog.csdnimg.cn/20200815004805702.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70" width="50%">

> 需要注意的是，**在高度为 $h$ 的AVL树中，最少结点数 $S(h) = S(h - 1) + S(h - 2) + 1$** 。对于 $h = 0,\ S(h) = 1;\ \ h = 1,\ S(h) = 2$ 。这一函数和斐波拉契数密切相关，由此可以推出AVL树的高度的界。

下图中展示的是一个高度为 $9$ 的最小AVL树，可以看到它具有这一高度的最少结点数 $143$ ：
 <img src="https://img-blog.csdnimg.cn/20200815005512499.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70" width="50%">
 
AVL的强大在于，所有的树操作都可以以时间 $O(\log N)$ 执行。不过，对AVL的插入和删除还是要花一番功夫。原因是：插入操作可能破坏AVL树的平衡特性，此时需要恢复平衡性质才能够认为这一步插入操作完成。插入以后，**只有那些从插入后到根结点的路径上的结点的平衡可能被改变**，因为只有它们的子树可能发生变化。我们需要**沿着这条路径从底向上直到根**更新平衡信息——如果我们发现一个结点破坏了AVL条件，就要通过**旋转** `rotation` 对其进行修正，使其重新平衡。

必须重新平衡的结点叫做 $\alpha$ ，由于任意结点最多只有两个儿子，因此高度不平衡时，$\alpha$ 的两棵子树高度差 $2$ ，有下面四种情况。分别用 `L` 和 `R` 的字母组合表示：
- 对 $\alpha$ 的左儿子的左子树进行一次插入：`LL` 型，当前结点的左儿子**太高了**，且左儿子的左子树**比较高**；
- 对 $\alpha$ 的左儿子的右子树进行一次插入：`LR` 型，当前结点的左儿子**太高了**，且左儿子的右子树**比较高**；
- 对 $\alpha$ 的右儿子的左子树进行一次插入：`RL` 型，当前结点的右儿子**太高了**，且右儿子的左子树**比较高**；
- 对 $\alpha$ 的右儿子的右子树进行一次插入：`RR` 型，当前结点的左儿子**太高了**，且左儿子的右子树**比较高**；

其中左子树太高指平衡因子 `>1` ，比较高指平衡因子 `>0` ；右子树太高指平衡因子 `<-1` ，比较高指平衡因子 `<0` 。


实际上只有两种情况，另外两种只是关于 $\alpha$ 的镜像对称。对**左-左/右-右**型，通过对树的一次**单旋转** `single rotation` 就可以完成调整；其他两种情况，需要通过更复杂的**双旋转** `double rotation` 来处理。

---
# 2. 单旋转
结点 $k_2$ 不满足AVL平衡性质， 图中的子树 `X` 在插入后长出了新的一层，这使得它比子树 `Z` 深出两层。而 `Y` 不可能与新的 `X` 处于同一层上，因为那样 $k_2$ 插入以前就失去平衡了，`Y` 也不可能与 `Z` 处于同一层上，因为那样 $k_1$ 就是在通向根的路径上破坏AVL平衡条件的第一个结点：
<img src="https://img-blog.csdnimg.cn/20200815142819418.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70" width="50%">
为了恢复平衡，需要把 `X` 上移一层，把 `Z` 下移一层，形成一棵等价的树——原树中 $k_2 > k_1$ ，于是新树中 $k_2$ 变成了 $k_1$ 的右儿子，`X, Z` 分别仍然是 $k_1$ 的左儿子和 $k_2$ 的右儿子，子树 `Y` 中包含原树中介于 $k_1$ 和 $k_2$ 之间的那些结点，可以把它放在新树中 $k_2$ 的左儿子的位置上。

改变一部分指针后，我们得到另一棵二叉搜索树，它是一棵AVL树，`X` 向上移动了一层，`Y` 停止原处，`Z` 下移一层。$k_1, k_2$ 不仅**满足AVL要求**，而且**子树都恰好处于同一高度上**；甚至，整棵树的新高度和**插入前原树**的高度相同，但子树 `X` 却长高了。

对称的情形如下：
<img src="https://img-blog.csdnimg.cn/20200815151630714.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70" width="50%">
 

总结成一句口诀：左旋提右左挂右，右旋提左右挂左。意思是：**左旋**就是把当前结点的**右子树**提升成为根结点，然后把右子树的**左子树**挂在当前结点的**右子树**上；**右旋**就是把当前结点的**左子树**提升成为根结点，然后把左子树的**右子树**挂在当前结点的**左子树**上。

---
下图中，将 `6` 插入左边原始的AVL树中后结点 `8` 不再平衡。于是在结点 `8` 和 `7` 之间做一次单旋转，得到右边的树：
<img src="https://img-blog.csdnimg.cn/20200815150410685.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70" width="50%">


更长的例子如下，向初始的空AVL树插入 `3, 2, 1` ，然后依次插入 `4, 5, 6, 7` 。插入 `1` 时AVL性质在根处被破坏。于是在根和左儿子之间进行单旋转以修正：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200815151724978.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
然后插入 `4` ，没有问题，但是插入 `5` 破坏了结点 `3` 处的AVL性质，通过单旋转将其修正：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200815160910300.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
插入 `6` ，在根结点产生一个平衡问题，它的左子树高度是 `0` 而右子树的高度是 `2` ，此时将在根处的 `2` 和 `4` 之间实施一次单旋转：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200815161229862.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
插入 `7` ，将导致另一次旋转：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200815161420764.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)



---
# 3. 双旋转
单旋转有一个问题，它对另外两种情形无效。如下图，子树 `Y` 太深，单旋转无法减低它的深度：
![在这里插入图片描述](https://img-blog.csdnimg.cn/2020081516212840.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
上图中，新的一项插入子树 `Y` 之中，这保证了它是非空的。因此可以假设它有一个根和两棵子树。于是，把整棵树看做四棵子树由三个结点相连。如下图：(左-右双旋转情形)
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200815162559419.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
为了重新平衡，就不能再让 $k_3$ 作为根，而在 $k_3,k_1$ 之间的旋转又解决不了问题。**唯一的选择就是把 $k_2$ 作为新的根**。于是需要 $k_1$ 是 $k_2$ 的左儿子，$k_3$ 是它的右儿子，从而得到最终的形态。可以看到，这棵新树的高度恢复到了插入前的水平，重新进入平衡状态。

类似的，右-左双旋转情形如下：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200815163340560.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
这两种情形的效果，都与先在 $\alpha$ 的儿子和孙子之间旋转、而后再在 $\alpha$ 和它的新儿子之间旋转的效果是相同的。

---
在前面的例子上，继续倒序插入 `10-16` ，然后插入 `8, 9` 。插入 `16` 很容易，它不破坏平衡性质；插入 `15` 则会引起结点 `7` 处的高度不平衡，需要通过一次右-左双旋转解决：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200815163837858.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
插入 `13` ，在根处不平衡，于是需要一次单旋转：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200815164256150.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
插入 `12` 也需要一个单旋转：
![在这里插入图片描述](https://img-blog.csdnimg.cn/2020081516440258.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
插入 `11` 和 `10` 各需要一次单旋转，
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200815164544336.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
最后插入 `9` 演示一次左-右双旋转：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200815164715675.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)

---
# 4. 具体实现
## (1) 结点信息和公共接口
结点中，我们真正需要的是**子树高度的差**，但是我们选择**在每个结点中存储高度信息**，同时也**不会正儿八经地计算当前结点和新根结点的平衡因子**。这样编写递归程序时会简明许多，只是需要**在旋转后更新结点的高度信息**。

当然，实际的实现方法有很多种，我们还可以在插入时**实时计算结点的高度和平衡因子**(左子树高度-右子树高度；根据**每个结点的左子树和右子树的高度最多差 $1$** 的要求可知，AVL所有结点的BF都只能等于 `-1,0,1` )，从而判断采取单旋转还是双旋转。这会慢一些，但是**不用在左旋和右旋函数中更新结点的高度信息**。

```cpp
//AVLTree Class
//PUBLIC:
//	void insert(x)              //插入x
//	void remove(x)				//删除x(没有实现) 
// 	bool contains(x)			//判断是否存在x
//	Comparable findMin()		//返回最小值 
//	Comparable findMax()		//返回最大值 
//	boolean empty()				//判断是否为空树 
//	void makeEmpty()			//删除所有项 
//	void printTree()			//打印中序序列

template<typename Comparable>
class AvlTree {
private:
	struct AvlNode {
		Comparable val;
		AvlNode   *left;
		AvlNode   *right;
		int       height; //可以不存储高度而是在插入的时候计算height和balance factor 
		AvlNode(const Comparable &v, AvlNode *lt = nullptr, AvlNode *rt = nullptr, int h = 0)
			: val(v), left(lt), right(rt), height(h) { }
	};
	AvlNode *root;

public:
	AvlTree() : root(nullptr) { }
	AvlTree(const AvlTree &rhs) : root(nullptr) { *this = rhs; }
	~AvlTree() { makeEmpty(); }
	
	//找到最小值
	const Comparable &findMin() const {
		if (empty()) throw underflow_error("Empty Tree");
		return findMin(root)->val;
	}
	
	//找到最大值
	const Comparable &findMax() const {
		if (empty()) throw underflow_error("Empty Tree");
		return findMax(root)->val;
	}
	
	//判断是否含有x
	bool contains(const Comparable &x) const {
		return contains(x, root);
	}
	
	//判断树是否为空
	bool empty() const {
		return root == nullptr;
	}
	
	//打印中序序列
	void printTree() const {
		if (empty()) cout << "Empty Tree" << endl;
		else printTree(root);
	}
	
	//清空树
	void makeEmpty() {
		makeEmpty(root);
	}
	
	//插入x到子树中,忽视重复值
	void insert(const Comparable &x) {
		insert(x, root);
	}
	
	//从树中移出x,如果不存在时就什么都不做
	void remove(const Comparable &x) {
        cout << "Sorry, remove unimplemented; " << x <<
                " still present" << endl;
    }
    
    //拷贝整棵树
	AvlTree &operator=(const AvlTree &rhs) {
		if (this != &rhs) {
			makeEmpty();
			root = clone(rhs.root);
		}
		return *this;
	}
};
```
## (2) 插入操作

## (3) 删除操作


上述的4种情形，看起来难，其实编程的细节相当简单。插入AVL树 `T` 的时候，把 `X` 新结点递归插入到 `T` 的相应子树 $T_{LR}$ 中；如果 $T_{LR}$ 的高度不变，那么插入完成；否则，在 `T` 中出现高度不平衡，就根据 `X` 和 `T` 以及 $T_{LR}$ 中的项做对应的单旋转或双旋转、更新这些高度、解决和树的其它部分的链接，从而完成插入。

