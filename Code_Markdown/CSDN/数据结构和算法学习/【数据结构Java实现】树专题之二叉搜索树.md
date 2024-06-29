@[toc]

从现在开始，进入树的阶段，按理说应该先介绍树的概念、存储，二叉树的概念和构建，二叉树和树的转换等等理论知识，不过这系列文章**主要是为了实现一些有用的数据结构**，因此直接进入二叉搜索树的范畴。
## 一、引子
为什么要用树结构？因为快。对于一棵二叉树，很多时候能够达到$O(logn)$的复杂度。这是很快的，很多时候是我们能够期待的最快的速度。

## 二、二叉搜索树Binary Search Tree基础
二叉搜索树，首先是一棵二叉树。二叉树和链表一样，也是一个动态数据结构，用引用连接起来。
```java
class Node {
	E e;
	Node left, right;
}
```
二叉树是**最常用**的一种树结构，对于一棵二叉树而言，每一个结点最多有两个孩子，左孩子和右孩子，**它们的顺序是有意义的**；每个结点最多只有一个父亲；根节点是唯一的。

对于二叉树而言，最重要的性质在于它天然的具有递归结构：
- 对于每个结点，它+左右子树，是一棵二叉树；
- 对于每个结点，它的左子树也是二叉树；
- 对于每个结点，它的右子树也是二叉树；

对于叶子结点，它的左右孩子为$NULL$，同上，为了满足递归性质，我们知道$NULL$也是一棵二叉树。和链表一样，**$NULL$是最基本最简单的二叉树结构**。

二叉搜索树，还是一棵搜索树。二叉搜索树的每个结点的值：
- 大于其左子树的所有结点的值；
- 小于其右子树的所有结点的值；
- **有重复值，则视这棵二叉搜索树的语义进行操作**。如果它实现的是集合，那么就直接返回；如果其允许重复，改变一下定义，把“等于”加到定义中，即把结点加到左边或右边。

为什么要这么定义二叉搜索树呢？如果我们要从其中查找某个数据，那么从根节点开始，与根节点的值比较，小于根的值，就递归往左边查；大于则递归往右边查。

是不是很有既视感？就像二分查找！不过二分查找最多用于静态查找；而构建出了一棵二叉搜索树，我们可以**实现动态的查找和插入、删除**。

但是我们可以存所有类型的元素吗？不可能，由于搜索树的定义，**我们存储的结点必须具有可比较性**！对于无序元素的集合，是不可能构成二叉搜索树的！

在Java中，为了体现可比较性，我们让Binary Search Tree存储的结点继承$Comparable<E>$。基本的实现如下：
```java 
public class BST<E extends Comparable<E>>{

	private class Node {
		public E e;
		public Node left, right;
		
		public Node(E e) {
			this.e = e;
			left = null;
			right = null;
		}
	}
	
	private Node root;
	private int size;
	
	public BST() {
		root = null;
		size = 0;
	}
	public int size() {
		return size;
	}
	public boolean isEmpty() {
		return size == 0;
	}  
} 
```

## 三、二叉搜索树添加元素
如果理解了二叉搜索树的定义，添加元素就很简单了。从根开始比较，如果小于根，就递归往左子树插入；否则递归往右子树插入。需要知道的是，这里实现的是**不包含重复元素的二叉搜索树**。而且下面**实现的是递归版本**，非递归实现可以放在以后。在这里，递归比非递归简单多了。

$还以为没有bug了，结果后面测试的时候发现了这个问题！$
```java
//向以node为根的二叉搜索树插入元素e
public void add(E e) { 
	//++size;
	root = add(root, e);
}
//私有版本, 内部调用
private Node add(Node root, E e) {
	if (root == null) {
	    //**注意, 必须把++size写在这里, 不能写在上面的add中, 因为不是每个e都会真正被插入树中！
		++size;
		return new Node(e);
	}
	if (e.compareTo(root.e) < 0)
		root.left = add(root.left, e);
	else if (e.compareTo(root.e) > 0)
		root.right = add(root.right, e);
	return root;
}
```

## 四、二叉搜索树查询元素
从二叉搜索树中查询元素，相当于树上的二分搜索，而且是添加元素的简单版本。
```java
//看以root为根的二叉搜索树是否含有e
public boolean contains(E e) {
	return contains(root, e);
}
private boolean contains(Node root, E e) {
	if (root == null) return false;
	
	if (e.equals(root.e)) return true;
	else if (e.compareTo(root.e) < 0) return contains(root.left, e);
	else return contains(root.right, e);
}
```
## 五、二叉(搜索)树的前序遍历(递归版)
先处理根节点，再处理左子树，再处理右子树。
```java
//二叉树的前序遍历
public void preOrder() {
	preOrder(root);
}
private void preOrder(Node root) {
	if (root == null) return;
	System.out.println(root.e);
	preOrder(root.left);
	preOrder(root.right);
}

@Override
public String toString() {
	StringBuilder res = new StringBuilder();
	generateBSTString(root, 0, res);
	return res.toString();
}
//生成一个描述二叉搜索树的字符串, 以root为根节点、深度为depth的描述二叉树的字符串 
private void generateBSTString(Node root, int depth, StringBuilder res) {
	if (root == null) {
		res.append(generateDepthString(depth) + "null\n");
		return;
	}
	res.append(generateDepthString(depth) + root.e + "\n");
	generateBSTString(root.left, depth + 1, res);
	generateBSTString(root.right, depth + 1, res);
}
//生成深度表示符--
private String generateDepthString(int depth) {
	StringBuilder res = new StringBuilder();
	for (int i = 0; i < depth; ++i)
		res.append("--"); 
	return res.toString();
}
	
public static void main(String[] args) {
	BST<Integer> bst = new BST<>();
	int[] nums = {5, 3, 6, 8, 4, 2};
	for (int num : nums)
		bst.add(num); 
	
	bst.preOrder();
	System.out.println();
	System.out.println(bst);
}
```
建立一棵测试的二叉树：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200315211521840.png)
## 六、二叉(搜索)树的后序遍历(递归版)
先处理左子树，再处理右子树，最后处理根节点。
```java
//二叉树的后序遍历
public void postOrder() {
	postOrder(root);
}
private void postOrder(Node root) {
	if (root == null) return;
	
	postOrder(root.left);
	postOrder(root.right);
	System.out.println(root.e);
} 
```
二叉树的后序遍历有很多应用：如释放二叉树的内存（先释放子树的内存，再释放根节点的内存）等。

后面写AVL树，**求高度和平衡因子的函数**，都是后序遍历形式的。

## 七、二叉(搜索)树的中序遍历(递归版)
先处理左子树，再处理根节点，最后处理右子树。
```java
//二叉树的中序遍历
public void inOrder() {
	inOrder(root);
}
private void inOrder(Node root) {
	if (root == null) return;
	
	inOrder(root.left);
	System.out.println(root.e);
	inOrder(root.right);
} 
```
对于二叉搜索树来说，中序遍历还有更多的意义——它**得到的序列是从小到大有序的**。

由于**插入的顺序**不同，对于同样的序列**可以有很多二叉搜索树的形态**——但它们的中序遍历序列是一样的。而且**这些二叉搜索树，可以由中序遍历序列，转换成一棵平衡的二叉搜索树——这个算法是分治思想的一个例子。**

到这里为止，我们学了二叉树的三种遍历形态，还有一个小小的知识点：**无论是哪一种遍历，都访问了每个结点三遍**。
## ★ 八、二叉(搜索)树的前序遍历(非递归)
这里只介绍二叉(搜索)树的前序遍历的非递归版本。**实际上，对于这几种遍历来说，非递归版本的写法比较复杂，而且应用也不多**。非递归前序遍历最好写，思想也最简单；非递归后序遍历的版本最麻烦。

可能用到的场景是，二叉树的层数太深，递归会崩溃。这时候用栈模拟，写非递归版本的遍历。

对于下面的二叉搜索树来说，前序遍历序列为$28,12,3,4,32,30,46,35$。我们用栈模拟一下：
<div align="center">
<img src="https://img-blog.csdnimg.cn/20200315214712208.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70" width = "40%"></img>

先把根节点$28$压入栈中，但没有访问$28$： 
<div align="center">
<img src="https://img-blog.csdnimg.cn/20200315215404271.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70" width = "50%"></img>

开始循环，弹出并访问$28$；再把$12,32$压入栈中，考虑到是先序遍历，先压$32$，然后是$12$。注意：这里的红色不代表访问，而是代表被压入或压入过栈中。
<div align="center">
<img src="https://img-blog.csdnimg.cn/20200315220001786.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70" width = "55%"></img>

弹出栈顶元素$12$，把其左子树结点$3$压入栈中：
<div align="center">
<img src="https://img-blog.csdnimg.cn/20200315220418923.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70" width = "55%"></img>

 弹出栈顶元素$3$，把其右子树结点$4$压入栈中：
<div align="center">
<img src="https://img-blog.csdnimg.cn/20200315220657925.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70" width = "58%"></img>

 弹出栈顶元素$4$，没有左右结点：
<div align="center">
<img src="https://img-blog.csdnimg.cn/20200315221142597.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70" width = "58%"></img>

 弹出栈顶元素$32$，把其右子树结点$46$和左子树结点$30$压入栈中：
<div align="center">
<img src="https://img-blog.csdnimg.cn/20200315222934271.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70" width = "58%"></img>

弹出栈顶元素$30$，没有结点入栈：
<div align="center">
<img src="https://img-blog.csdnimg.cn/2020031522322292.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70" width = "58%"></img>


 弹出栈顶元素$46$，把其左子树结点$35$压入栈中：
<div align="center">
<img src="https://img-blog.csdnimg.cn/20200315223458145.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70" width = "58%"></img>

最后弹出$35$，整棵树的前序遍历完成，得到最终的前序序列：
<div align="center">
<img src="https://img-blog.csdnimg.cn/20200315223630482.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70" width = "55%"></img></div>

这是前序非递归遍历的其中一种写法——**在每次弹出时访问元素**。还有别的写法——**先访问元素再压入栈中**。

下面就是根据上诉图片过程实现的代码：
```java
//非递归形式前序遍历
public void preOrderNR() { 
	LinkedStack<Node> stack = new LinkedStack<>();
	stack.push(root);
	
	while (!stack.isEmpty()) {
		Node cur = stack.pop();
		System.out.println(cur.e);
		if (cur.right != null) stack.push(cur.right); //右子树先进
		if (cur.left != null) stack.push(cur.left);
	}
}
...
public static void main(String[] args) {
	BST<Integer> bst = new BST<>();
	int[] nums = {5, 3, 6, 8, 4, 2};
	for (int num : nums)
		bst.add(num); 
	
	bst.preOrder();
	System.out.println();
	bst.preOrderNR(); //非递归前序遍历 
}
```
![在这里插入图片描述](https://img-blog.csdnimg.cn/2020031601095677.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
emmm....等等，非递归实现先序遍历的代码片段很眼熟。学了下面的层序遍历后，就会发现，这段代码**等价于非递归DFS**，**和BFS的代码片段除了使用的数据结构不同外，基本上写起来一模一样**。==因此，我觉得，这种写法比起其他非递归先序遍历的写法简单方便多了。==

另外，对于中序和后序的非递归实现，比较复杂，而且**用的不多，顶多作为算法和思维的训练，可能以后会写**。写法有如下两种：
- 经典教科书的写法；
- 完全模拟系统栈调用的方式。


## 九、二叉(搜索)树的层序遍历
二叉搜索树的层序遍历，或者说是二叉树的层序遍历，是**广度优先遍历BFS在树结构的体现**。遍历需要队列的帮助。**这个很简单**，就不放过程图，不多讲了。
<div align = "center">
<img src="https://img-blog.csdnimg.cn/20200313130640695.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70" width = "70%"></img>
</div>

这里的几个数据结构——LinkedStack和LoopQueue是前面实现的。

```java
//层序遍历
public void BFS() {
	LoopQueue<Node> queue = new LoopQueue<>();
	queue.enqueue(root);
	while (!queue.isEmpty()) {
		Node cur = queue.dequeue();
		System.out.println(cur.e);
		if (cur.left != null) queue.enqueue(cur.left);
		if (cur.right != null) queue.enqueue(cur.right);
	}
}
```
介绍完了这一点，树的DFS和BFS**几乎就完全讲完了**。
 
## 十、寻找和删除二分搜索树的最小最大元素
寻找二分搜索树中的**最小元素，就一路向左；最大元素，就一路向右**。可以写出递归和非递归版本。都很简单。

```java
//返回最小值
public E minimum() {
	if (size == 0)
		throw new IllegalArgumentException("BST is empty!");
	
	return minimum(root).e;
}
//返回以Node为根的二分搜索树的最小值所在的结点
private Node minimum(Node node) {
	if (node.left == null) return node;
	return minimum(node.left);
}
//返回最小值(非递归) 
public E minimumNR() {
	if (size == 0)
		throw new IllegalArgumentException("BST is empty!");
	Node ans = root;
	while (ans.left != null) ans = ans.left;
	return ans.e;
}

//返回最大值
public E maximum() {
	if (size == 0)
		throw new IllegalArgumentException("BST is empty!");
	
	return maximum(root).e;
}
//返回以Node为根的二分搜索树的最大值所在的结点
private Node maximum(Node node) {
	if (node.right == null) return node;
	return maximum(node.right);
}
//返回最大值(非递归) 
public E maximumNR() {
	if (size == 0)
		throw new IllegalArgumentException("BST is empty!");
	Node ans = root;
	while (ans.right != null) ans = ans.right;
	return ans.e;
}
```
删除最小值和最大值的代码及测试如下，当然，下面的测试很简陋，但是测试是必要的，这里的测试还帮我发现了前面的一个小$bug$。
```java
//删除二分搜索树的最小结点并返回其值
public E removeMin() {
	E ret = minimum(); //在这里面检查参数
	root = removeMin(root);
	return ret;
}
//删除以node为根的二分搜索树的最小结点
//返回删除结点后新的二分搜索子树的根
private Node removeMin(Node node) {
	if (node.left == null) {
		Node rightNode = node.right;
		node.right = null; 
		--size;
		return rightNode;
	}
	//删除左子树中的最小值并得到新的左子树
	node.left = removeMin(node.left); 
	return node;
}

//删除二分搜索树的最大结点并返回其值
public E removeMax() {
	E ret = maximum();
	root = removeMax(root);
	return ret;
}
//删除以node为根的二分搜索树的最大结点
//返回删除结点后新的二分搜索子树的根
private Node removeMax(Node node) {
	if (node.right == null) {
		Node leftNode = node.left; //最大值结点可能有左子树
		node.left = null;
		--size;
		return leftNode;
	}
	//删除右子树中的最大值并得到新的右子树		
	node.right = removeMax(node.right);
	return node;
}
  
public static void main(String[] args) { 
	BST<Integer> bst = new BST<>();
	Random random = new Random();
	int n = 1000;
	//test removeMin
	for (int i = 0; i < n; ++i)
		bst.add(random.nextInt(10000));
	Array<Integer> nums = new Array<>();
	while (!bst.isEmpty()) 
		nums.addLast(bst.removeMin());
	System.out.println(nums); //打印从小到大排序的数组
	for (int i = 1; i < nums.getSize(); ++i)
		if (nums.get(i - 1) > nums.get(i))
			throw new IllegalArgumentException("Error");
	System.out.println("removeMin test completed.");
	
	//test removeMax 
	for (int i = 0; i < n; ++i)
		bst.add(random.nextInt(10000));
	nums = new Array<>();
	while (!bst.isEmpty())
		nums.addLast(bst.removeMax());
	System.out.println(nums); //打印从大到小排序的数组
	for (int i = 1; i < nums.getSize(); ++i)
		if (nums.get(i - 1) < nums.get(i))
			throw new IllegalArgumentException("Error");
	System.out.println("removeMax test completed."); 
}
```
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200316120845669.png)

## 十一、删除二叉搜索树中的任意元素
鉴于二叉树有五种形态，其中可以删除的是：只有一个结点；只有左子树的结点；只有右子树的结点；左右子树都有的结点。

其中**叶子结点**和**只有左或右子树的结点**，在前面一小节已经写过了（当然，最大最小结点可能是叶子结点，或者只有一棵子树；但反过来就不一定成立）。这三种情况都很简单。**只有删除左右子树都有的结点比较困难**。

原因在于，既要保持左右子树，还要维持二叉搜索树的性质——根节点$\lt$右子树的所有结点，$\gt$左子树的所有结点。因此，在删除了那个结点后，不能简单的用左子树或右子树代替它的位置。

我们可以有两种替代方法：用二叉搜索树的**中序序列**中，要删除结点的**前驱或者后继结点**来替代它的位置。换句话说，就是**要删除结点的左子树的最大值，或者右子树的最小值**。

这里选择一个：右子树的最小值。我们可以用`minimum(delNode->right)`先得到它的最小值结点，用这个结果**替代**`delNode`，然后用`remove(delNode->right)`递归删除这个最小值，或者直接使用`Node removeMin()`函数来删除这个最小值。

<img alt = "替代" src="https://img-blog.csdnimg.cn/20200316122722157.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70"></img>


```java
//从二分搜索树中删除元素为e的结点
public void remove(E e) {
	root = remove(root, e);
}
//删除以node为根的二分搜索树中值为e的结点，递归算法
//返回删除结点后新的二分搜索树的根
private Node remove(Node node, E e) {
	if (node == null) return null;
	if (e.compareTo(node.e) < 0) {
		node.left = remove(node.left, e);
		return node;
	}
	else if (e.compareTo(node.e) < 0) {
		node.right = remove(node.right, e);
		return node;
	}
	else { //e == node.e
		if (node.left == null) { //待删除结点左子树为根的情况
			Node rightNode = node.right;
			node.right = null;
			--size;
			return rightNode;
		} 
		if (node.right == null) {
			Node leftNode = node.left;
			node.left = null;
			--size;
			return leftNode;
		}
		
		Node successor = minimum(node.right);
		successor.right = removeMin(node.right);
		successor.left = node.left;
		node.left = node.right = null;
		//这里不用写--size;因为已经在removeMin中减少了size
		return successor;
	}
}
```
## 最后：总结与更多
呼，好长啊。

到了总结了，虽然关于二叉树/二叉搜索树还有一些知识点没有讲，比如：
- 中序+后序或者先序+中序-->还原二叉树；
- 树<-->二叉树<-->森林的转变；
- 非递归版本的先序、中序、后序遍历的经典实现；
- 模拟系统栈的的先中后序遍历的实现；
- 二叉树Moris先中后遍历实现；
- 其他方法的非递归实现；
- 前驱和后继操作；
- $floor$和$ceil$操作：如找到$45$的$floor$就是找到比$45$小的最大元素，$ceil$就是找到比$45$大的最小元素，注意：$45$可以不在二叉搜索树中；
- 结点内维护$size$的二分搜索树：这样就很容易实现$rank$和$select$；
- $rank$和$select$操作：$rank$是给出某个元素的排名；$select$是给出排名对应的元素；
- 结点内维护$depth$的二分搜索树；
- 结点内维护$count$/支持重复元素的二分搜索树；
- 有重复元素结点的二分搜索树（另一种支持重复元素的二分搜索树实现）。
- ……

内容太多了！

下一章，就是二分搜索树的应用，用其实现集合和映射（也会用数组和链表实现）。
