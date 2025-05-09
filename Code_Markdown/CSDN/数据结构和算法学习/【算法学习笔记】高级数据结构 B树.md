> 参考算法导论第18章 B树
> ![在这里插入图片描述](https://img-blog.csdnimg.cn/7ce4602493164bfab13feabe4d66276d.png)

# 1. B树概述
**B树是为「磁盘或其他直接存取的辅助存储设备」而设计的一种平衡搜索树**。B树类似于红黑树（算导第13章），但它们在降低磁盘I/O操作数 `minimizing disk I/O operations` 方面要更好一些。许多数据库系统使用B树或B树的变种来存储信息。

B树与红黑树的不同之处，在于B树的结点可以有很多孩子，从数个到数千个。也就是说，一个B树的**分支因子** `branching factor` 可以很大，尽管它通常依赖于所使用的磁盘单元的特性 `characteristics of the disk unit used` 。
- B树类似于红黑树，就是指每棵含有 $n$ 个结点的B树的高度为 $O(\log n)$ 。
- 然而，一棵B树的严格高度可能比一棵红黑树的高度要小许多，这是因为它的分支因子，也就是表示高度的对数的底数可以非常大。

因此，我们也可以使用B树在时间 $O(\log n)$ 内完成一些动态集合的操作 `dynamic-set operations` 。

B树以一种自然的方式推广了二叉搜索树，图18-1给出了一棵简单的B树。**如果B树的一个内部结点 $x$ 包含 $x.n$ 个关键字，那么结点 $x$ 就有 $x.n+1$ 个孩子**。结点 $x$ 中的关键字就是分隔点 `dividing points` ，它把「结点 $x$ 中所处理的关键字的范围」分隔为 $x.n+1$ 个子域，每个子域都由 $x$ 的一个孩子处理。**当在一棵B树中查找一个关键字时，基于对存储在结点 $x$ 中的 $x.n$ 个关键字的比较，做出一个 $(x.n+1)$ 路的选择**。此外，**$B$ 树中所有叶结点处在树中相同的深度，叶结点的结构与内部结点的结构不同**，（算导18.1节）之后将讨论这些差别。
![在这里插入图片描述](https://img-blog.csdnimg.cn/bf1e25c816f24b01a01fabe45ece0927.png)

（算导18.1节）给出B树的精确定义，并证明了：**B树的高度仅随「它所包含的结点数」按对数增长**。（算导18.2节）介绍如何在B树中查找和插入一个关键字，（算导18.3节）讨论删除操作。

## 辅存上的数据结构
然而，在开始之前需要弄清楚，为什么「针对磁盘设计的数据结构」不同于「针对随机访问的主存设计的数据结构」。

计算机系统利用各种技术来提供存储能力。一个计算机系统的**主存** `primary memory or main memory` 通常由硅存储芯片组成。这种技术每位的存储代价一般要比磁存储技术（如磁带或磁盘）高不止一个数量级。许多计算机系统还有基于磁盘的**辅存** `secondary storage` ；这种辅存的容量通常要比主存的容量高出至少两个数量级。

图18-2是一个典型的磁盘驱动器。驱动器由一个或多个**盘片** `platter` 组成，它们以一个固定的速度绕着一个共同的**主轴** `spindle` 旋转。每个盘的表面覆盖着一层可磁化的物质。驱动器通过**磁臂** `arm` 末尾的**磁头** `head` 来读/写盘片，磁臂可以将磁头向主轴移近或移远。当一个给定的磁头处于静止时，它下面经过的磁盘表面称为一个**磁道** `track` 。多个盘片增加的仅仅是磁盘驱动器的容量、而不影响性能。
![在这里插入图片描述](https://img-blog.csdnimg.cn/167dd608f80d4d2a9354d37e161a8195.png)
尽管磁盘比主存便宜、并且具有更多的容量，但是它们比主存慢很多，因为它们有机械运动的部分。**磁盘有两个机械运动的部分：盘片旋转和磁臂移动**。
> 算导中说：在写本书时，商用磁盘的旋转速度是 $5400 \sim 15000$ 转/分钟 `revolutions per minute, RPM` 。通常看到的 $15 000\mathrm{RPM}$ 的速度是用于服务器级的驱动器上，$7200\mathrm{RPM}$ 的速度用于台式机的驱动器上，$5400\mathrm{RPM}$ 的速度用于笔记本的驱动器上。尽管 $7200\mathrm{RPM}$ 看上去很快，但是旋转一圈需要 $8.33ms$ ，比硅存储的常见存取时间 $50ns$ 要高出 $5$ 个数量级。也就是说，如果不得不等待一个磁盘旋转完整的一圈，让一个特定的项到达读/写磁头下方，在这个时间内，我们可能存取主存超过 $100 000$ 次。平均来说，只需等待半圈，但硅存储存取时间和磁盘存储存取时间的差距仍然是巨大的。移动磁臂也要耗费时间。在写本书时，商用磁盘的平均存取时间在 $8 \sim 11ms$ 范围内。 
> 此外，固态硬盘虽然比机械磁盘驱动器快，但它们每GB的成本更高，并且比机械磁盘驱动器的容量更小。

**为了摊还机械移动所花费的等待时间，磁盘会一次存取多个数据项而不是一个**。信息被分为一系列相等大小的、在磁道 `tracks` 内连续出现的位**页面** `page` ，并且每个磁盘读或写一个或多个完整的页面。对于一个典型的磁盘来说，一页的长度可能为 $2^{11} \sim 2^{14}$ 字节。==一旦读/写磁头正确定位，并且盘片已经旋转到所要页面的开头位置，对磁盘的读或写就完全电子化了（除了磁盘的旋转外），磁盘能够快速地读或写大量的数据==。

通常，定位到一页信息并将其从磁盘里读出的时间、要比对读出信息进行检查的时间要长得多。因此，这里将对运行时间的两个主要组成部分分别加以考虑：
- 磁盘存取次数 `the number of disk accesses` 。
- CPU（计算）时间 `the CPU (computing) time` 。

**我们使用「需要读出或写入磁盘的信息的页数」来衡量磁盘存取次数**。注意到，磁盘存取时间并不是常量——它依赖于当前磁道与所需磁道之间的距离、以及磁盘的初始旋转状态。但是，**我们仍然使用「读或写的页数」作为磁盘存取总时间的主要近似值**。
 
在一个典型B树应用中，所要处理的数据量非常大，以至于所有数据无法一次装入主存。==B树算法将所需页面从磁盘复制到主存，然后将修改过的页面写回磁盘。在任何时刻，B树算法都只需在主存中保持一定数量的页面==。因此，主存的大小并不限制被处理的B树的大小 `the size of main memory does not limit the size of B-trees that can be handled` 。

用以下的伪代码来对磁盘操作进行建模。设 $x$ 为指向一个对象的指针。**如果该对象正在主存中，那么可以像平常一样引用该对象的各个属性**：如 $x.key$ 。然而，**如果 $x$ 所指向的对象驻留在磁盘上，那么在引用它的属性之前，必须先执行 `DISK-READ(x)` ，将该对象读入主存中**（假设如果 $x$ 已经在主存中，那么 `DISK-READ(x)` 不需要磁盘存取，即它是个空操作）。类似地，**操作 `DISK-WRITE(x)` 用来保存「对对象 $x$ 的属性所做的任何修改」**。即，一个对象操作的典型模式如下：
```cpp
x is a pointer to some object
DISK-READ(x)
operations that access and/or modify the attributes of x
DISK-WRITE(x) // omitted if no attributes of x were changed
other operations that access but do not modify attributes of x
```
**在任何时刻，这个系统可以在主存中只保持有限的页数**。我们应假定，系统将「不再被使用的页」从主存中换出 `the system flushes from main memory pages no longer in use` ；后面的B树算法会忽略这个问题。
 
由于在大多数系统中，**一个B树算法的运行时间主要由它所执行的 `DISK-READ` 和 `DISK-WRITE` 操作的次数决定**，所以我们希望这些操作能够读或写尽可能多的信息。因此，**一个B树的结点通常和一个完整磁盘页一样大，并且磁盘页的大小限制了一个B树结点可以含有的孩子个数** `a B-tree node is usually as large as a whole disk page, and this size limits the number of children a B-tree node can have` 。

对存储在磁盘上的一棵大的B树 `a large B-tree stored on a disk` ，通常看到**分支因子**在 $50 \sim 2 000$ 之间，**具体取决于一个关键字相对于一页的大小** `the size of a key relative to the size of a page` 。 一个大的分支因子可以大大降低树的高度、以及查找任何一个关键字所需的磁盘存取次数。图18-3显示的是一棵分支因子为 $1001$ 、高度为 $2$ 的B树，它可以存储超过 $10$ 亿个关键字。不过，**由于根结点可以持久地保存在主存中，所以在这棵树中查找某个关键字至多只需两次磁盘存取**。
![在这里插入图片描述](https://img-blog.csdnimg.cn/138147e25c7b4512b4751d54d9a3673e.png)

---
# 2. B树的定义
为简单起见我们假定，就像二叉搜索树和红黑树中一样，任何和关键字相联系的**卫星数据** `satellite information` 将与关键字一样存放在同一个结点中。实际上可能只是为每个关键字存放一个指针，指向存放该关键字的卫星数据的磁盘页。这里的伪代码都隐含地假设：==当一个关键字从一个结点移动到另一个结点时，无论是与关键字相联系的卫星数据、还是指向卫星数据的指针，都会随着关键字一起移动==。一个常见的B树变种、称为**B^+^树，它把所有的卫星数据都存储在叶结点中，内部结点只存放关键字和孩子指针，因此最大化了内部结点的分支因子**。

一棵B树 $T$ 是具有以下性质的有根树（根为 $T.root$ ）：
1. 每个结点 $x$ 有下面属性：
	- ==$x.n$ ，当前存储在结点 $x$ 中的关键字个数==；
	- ==$x.n$ 个关键字本身 $x.key_1,\ x.key_2,\ \dots,\ x.key_{x.n}$ ，以非降序存放==，使得 $x.key_1 \le x.key_2 \le \dots \le x.key_{x.n}$ 。  
	- ==$x.leaf$ ，一个布尔值==，如果 $x$ 是叶结点则为 `TRUE` ；如果 $x$ 是内部结点则为 `FALSE` 。
2. 每个**内部结点** $x$ 还包含 ==$x.n+1$ 个指向其孩子的指针== $x.c_1,\ x.c_2,\ \dots,\ x.c_{x.n+1}$ 。叶结点没有孩子，所以它们的 $c_i$ 属性没有定义。
3. **关键字 $x.key_i$ 对存储在各子树中的关键字范围加以分割**：如果 $k_i$ 为任意一个存储「在以 $x.c_i$ 为根的子树」中的关键字，那么 $$k_1 \le x.key_1 \le k_2 \le x.key_2 \le \dots \le x.key_{x.n} \le k_{x.n+1}$$
4. **每个叶结点具有相同的深度**，即树的高度 $h$ 。
5. **每个结点能包含的关键字个数有上界和下界**，我们用一个被称为B树的**最小度数** `minimum degree` 的固定整数 $t \ge 2$ 来表示这些界：
	a. **除了根结点以外的每个结点，必须至少有 $t - 1$ 个关键字**。因此，除了根结点以外的每个内部结点至少有 $t$ 个孩子。**如果树非空，根结点至少有一个关键字**（即有两个孩子）。
	b. **每个结点至多可包含 $2t-1$ 个关键字**。因此，一个内部结点至多可有 $2t$ 个孩子。当一个结点恰好有 $2t-1$ 个关键字时，称该结点是**满的** `full` 。

	显然，$t = 2$ 时的B树是最简单的。每个内部结点有 $1$ 个、$2$ 个或 $3$ 个关键字，即有 $2$ 个、$3$ 个或 $4$ 个孩子，即一棵**2-3-4树**。然而在实际中，$t$ 的值越大，B树的高度就越小。

## B树的高度
**B树上大部分的操作所需的磁盘存取次数，与B树的高度是成正比的**。现在来分析B树最坏情况下的高度。我们会发现，B树的高度与最小度数 $t$ 成反比、与关键字个数 $n$ 成正比。

**定理18.1** 如果 $n \ge 1$ ，那么对任意一棵包含 **$n$ 个关键字**、高度为 $h$ 、**最小度数** $t \ge 2$ 的B树 $T$ ，有：$$h \le \log_t \dfrac{n+1}{2}$$ 

**证明**：**B树 $T$ 的根至少包含一个关键字，而且所有其他的结点至少包含 $t - 1$ 个关键字**。因此，高度为 $h$ 的B树 $T$ 在深度 $1$ 时至少包含 $2$ 个结点，在深度 $2$ 至少包含 $2t$ 个结点，在深度 $3$ 至少包含 $2t^2$ 个结点，等等，直到深度 $h$ 至少有 $2t^{h-1}$ 个结点。图18-4给出了 $h = 3$ 时的一棵树。因此，关键字的个数 $n$ 满足不等式：$$n \ge 1 + (t - 1) \sum^h_{i = 1} 2t^{i - 1} = 1 + 2(t - 1) ( \dfrac{ t^h - 1}{t - 1}) = 2t^h - 1$$ 由简单的代数变换，可以得到 $t^h \le (n + 1) / 2$ 。两边取以 $t$ 为底的对数就证明了定理。
![在这里插入图片描述](https://img-blog.csdnimg.cn/c81484f6891e4ec4907ec66297040b9b.png)
与红黑树对比，这里我们看到了B树的能力。尽管二者的高度都以 $O(\log n)$ 的速度增长（注意 $t$ 是个常数），但对B树来说，对数的底可以大很多倍。因此，对大多数树的操作来说，在B树中要检查的结点数相较于红黑树，要少大约 $\log t$ 的因子。**由于我们通常需要访问磁盘、来检查一棵树中的任意一个结点（意即检查每个结点都需要一次磁盘访问），所以B树避免了大量的磁盘访问**。

---
# 3. B树上的基本操作
本节将给出 `B-TREE-SEARCH, B-TREE-CREATE, B-TREE-INSERT` 操作的细节。在这些过程中，我们采用两个约定：
- **B树的根结点始终在主存中**，这样无需对根做 `DISK-READ` 操作；然而，当根结点被改变后，需要对根结点做一次 `DISK-WRITE` 操作。
- 任何被当做参数的结点在被传递之前，都要对它们先做一次 `DISK-READ` 操作。

我们给出的过程都是单程 `one-pass` 算法，即它们从树的根开始向下，没有任何返回向上的过程。
## 3.1 搜索B树
搜索一棵B树和搜索一棵二叉搜索树很相似，只是在每个结点所做的不是两路分支选择，而是根据结点的孩子数做多路分支选择。更严格地说，对每个内部结点 $x$ ，做的是一个 $(x.n+1)$ 路的分支选择（ $t - 1\le x.n \le 2t - 1$ ） 。

`B-TREE-SERACH` 是定义在二叉搜索树上的 `TREE-SEARCH` 过程的一个直接推广。它的输入是一个指向某子树根结点 $x$ 的指针，以及要在该子树中搜索的一个关键字 $k$ 。因此，顶层调用的形式为 `B-TREE-SEARCH(T.root, k)` 。**如果 $k$ 在B树中，那么 `B-TREE-SEARCH` 返回的是由结点 $y$ 和使得 $y.key_i = k$ 的下标 $i$ 组成的有序对 $(y, i)$** ；否则，过程返回 `NULL` 。
```cpp
B-TREE-SEARCH(x, k)
	i = 1
	while i <= x.n and k > x.key[i]
		i = i + 1
	if i <= x.n and k == x.key[i]
		return (x, i)
	else if x.leaf
		return NULL
	else DISK-READ(x.c[i])
		return B-TREE-SEARCH(x.c[i], k)
```
函数体内利用一个线性搜索过程，前三行找出最小下标 $i$ 、使得 $k \le x.key_i$ 。若找不到，则置 $i$ 为 $x.n+1$ 。第四至五行检查是否已经找到该关键字，如果找到则返回；否则，第六至九行结束这次不成功查找（如果 $x$ 是叶结点），或者在对孩子结点执行必要的 `DISK-READ` 后，递归搜索 $x$ 的相应子树。

图18-1显示了 `B-TREE-SEARCH` 的操作过程，浅阴影的结点是在搜索关键字 $R$ 的过程中被检查的结点。

就像二叉搜索树的 `TREE-SEARCH` 过程一样，在递归过程中所遇到的结点、构成了一条从树根向下的简单路径。因此，**由 `B-TREE-SEARCH` 过程访问的磁盘页面数（一个B树的结点通常和一个完整磁盘页一样大）为 $O(h) = O(\log_t n)$ ，其中 $h$ 为B树的高，$n$ 为B树中所含关键字个数**。由于 $x.n < 2t$ ，所以第二至三行的while循环在每个结点中花费的时间为 $O(t)$ ，==总的CPU时间为 $O(th) = O(t\log_t n)$== 。

## 创建一棵空的B树
为构造一棵B树 $T$ ，先用 `B-TREE-CREATE` 来创建一个空的根结点，然后调用 `B-TREE-INSERT` 来添加新的关键字。这些过程都要用到一个辅助过程 `ALLOCATE-NODE` ，它在 $O(1)$ 时间内为一个新结点分配一个磁盘页。我们可以假定，**由 `ALLOCATE-NODE` 所创建的结点并不需要 `DISK-READ` ，因为磁盘上还没有关于该结点的有用信息**。
```cpp
B-TREE-CREATE(T)
	x = ALLOCATE-NODE()
	x.leaf = true
	x.n = 0
	DISK-WRITE(x)
	T.root = x
```
`B-TREE-CREATE` 需要 $O(1)$ 次的磁盘操作和 $O(1)$ 的CPU时间。

## 向B树中插入一个关键字
B树中插入一个关键字，要比二叉搜索树中插入一个关键字复杂得多。像二叉搜索树中一样，**要查找插入新关键字的叶结点的位置**。然而，在B树中不能简单地创建一个新的叶结点，然后将其插入，因为**这样得到的树将不再是合法的B树**（叶结点的高度不同）。相反，我们是==将新的关键字插入一个已经存在的叶结点上==。由于不能将关键字插入一个满的叶结点，故引入一个操作，==将一个满的结点 $y$（有 $2t -1$ 个关键字）按其**中间关键字** `median key` $y.key_t$ **分裂** `split` 为两个各含 $t - 1$ 个关键字的结点，中间关键字被提升到 $y$ 的父结点，以标识两棵新树的划分点==。但是==如果 $y$ 的父结点也是满的，就必须在插入新的关键字之前将其分裂，最终满结点的分裂会沿着树向上传播==。

与一棵二叉搜索树一样，可以在「从树根到叶子这个单程向下过程」中、将一个新的关键字插入B树中。为了做到这一点，我们并不是等到找出「插入过程中实际要分裂的满结点」时才做分裂。相反，==当沿着树向下查找新的关键字所属位置时，就分裂沿途遇到的每个满结点（包括叶结点本身）。因此，每当要分裂一个满结点 $y$ 时，就能确保它的父结点不是满的==。
 
## 分裂B树中的结点
过程 `B-TREE-SPLIT-CHILD` 的输入是一个**非满**的内部结点 $x$（假定在主存中），和一个使 $x.c_i$（也假定在主存中）为「$x$ 的**满**子结点」的下标 $i$ 。**该过程把这个子结点分裂成两个，并调整 $x$ ，使其包含多出来的孩子**。要分裂一个满的根，首先要让根成为一个新的空根结点的孩子，这样才能使用 `B-TREE-SPLIT-CHILD` 。树的高度因此增加 $1$ ；**分裂是树长高的唯一途径**。

图18-5显示了这个过程。满结点 $y = x.c_i$ 按照其中间关键字 $S$ 进行分裂，$S$ 被提升到 $y$ 的父结点 $x$ 。$y$ 中的「那些大于中间关键字的关键字」都置于一个新的结点 $z$ 中，它成为 $x$ 的一个新的孩子。
![](https://img-blog.csdnimg.cn/d034ddffdf43470488e0431c99cde2e1.png)
```cpp
B-TREE-SPLIT-CHILD(x, i)
	z = ALLOCATE-NODE()
	y = x.c[i] 			// y为x的满子结点
	z.leaf = y.leaf
	z.n = t - 1
	for j = 1 to t - 1 	// 复制t-1个关键字到z
		z.key[j] = y.key[j + t]
	if not y.leaf 		// y不是叶子结点时
		for j = 1 to t 	// 复制后面的t个指针到z
			z.c[j] = y.c[j + t]
	y.n = t - 1 		// 调整y的关键字个数
	for j = x.n + 1 downto i + 1
		x.c[j + 1] = x.c[j]
	x.c[i + 1] = z		// 插入结点z到x
	for j = x.n downto i
		x.key[j + 1] = x.key[j]
	x.key[i] = y.key[t] // 结点z的关键字都>=x.key[i]
	x.n = x.n + 1
	DISK-WRITE(y)
	DISK-WRITE(z)
	DISK-WRITE(x)
```
![在这里插入图片描述](https://img-blog.csdnimg.cn/e83f6ddc8ae64f299273cdff539e5ce1.png)

`B-TREE-SPLIT-CHILD` 以直接的“剪贴”方式工作。这里 $x$ 是被分裂的结点，$y$ 是 $x$ 的第 $i$ 个孩子（见第2行）。开始时，结点 $y$ 有 $2t$ 个孩子（$2t - 1$ 个关键字），在分裂后减少至 $t$ 个孩子（$t - 1$ 个关键字）。结点 $z$ 取走 $y$ 的 $t$ 个最大的孩子（$t - 1$ 个关键字），并且 $z$ 成为 $x$ 的新孩子，它在 $x$ 的孩子表中仅位于 $y$ 之后。$y$ 的中间关键字上升到 $x$ 中，成为分隔 $y$ 和 $z$ 的关键字。

过程体中第一至九行创建结点 $z$ ，并将 $y$ 的 $t - 1$ 个关键字、以及相应的 $t$ 个孩子给它。第十行调整 $y$ 的关键字个数。最后，**第 $11\sim 17$ 行将 $z$ 插入为 $x$ 的一个孩子，并提升 $y$ 的中间关键字到 $x$ 来分隔 $y, z$ ，然后调整 $x$ 的关键字个数**。第 $18\sim 20$ 行写出所有修改过的磁盘页面。

`B-TREE-SPLIT-CHILD` 占用的CPU时间为 $\Theta(t)$ ，是由第 $5\sim 6$ 行和第 $8 \sim 9$ 行的循环引起的（其他循环执行 $O(t)$ 次迭代），这个过程执行 $O(1)$ 次磁盘操作。

## 以沿树单程下行方式向B树插入关键字
在一棵高度为 $h$ 的B树 $T$ 中，以沿树单程下行方式插入一个关键字 $k$ 的操作，需要 $O(h)$ 次磁盘存取，所需要的CPU时间为 $O(th) = O(t \log_t n)$ 。过程 `B-TREE-INSERT` 利用 `B-TREE-SPLIT-CHILD` 来保证**递归始终不会降至一个满结点上** `the recursion never descends to a full node` 。
```cpp
B-TREE-INSERT(T, k)
	r = T.root
	if r.n == 2t - 1			// 根结点满 
		s = ALLOCATE-NODE()
		T.root = s				// 新的空根结点
		s.leaf = false
		s.n = 0					// 关键字为0个
		s.c[1] = r				// 只有一个孩子(原根结点)
		B-TREE-SPLIT-CHILD(s, 1)
		B-TREE-INSERT-NONFULL(s, k)
	else B-TREE-INSERT-NONFULL(r, k)
```
![在这里插入图片描述](https://img-blog.csdnimg.cn/6ab00fea99c14611962903560fdcf849.png)

第三至九行处理了根结点 $r$ 为满的情况：原来的根结点被分裂，一个新的结点 $s$（有两个孩子）成为根。**对根进行分裂是增加B树高度的唯一途径**。图18-6说明了这种情况。与二叉搜索树不同，B树高度的增加发生在顶部、而非底部。过程通过调用 `B-TREE-INSERT-NONFULL` 完成将关键字 $k$ 插入「以非满的根结点为根的树」中。`B-TREE-INSERT-NONFULL` 在需要时沿树向下递归，在必要时通过调用 `B-TREE-SPLIT-CHILD` 来保证：**任何时刻它所递归处理的结点都是非满的**。
![在这里插入图片描述](https://img-blog.csdnimg.cn/39ae7a4e90e341909734c397c2239099.png)
辅助的递归过程 `B-TREE-INSERT-NONFULL` 将关键字插入结点 $x$ ，要求假定：**在调用该过程时 $x$ 是非满的**。操作 `B-TREE-INSERT` 和递归操作 `B-TREE-INSERT-NONFULL` 保证了这个假设成立。
```cpp
B-TREE-INSERT-NONFULL(x, k)
	i = x.n
	if x.leaf // x是叶结点
		while i >= 1 and k < x.key[i]
			x.key[i + 1] = x.key[i]
			i = i - 1
		x.key[i + 1] = k // 将关键字k插入叶结点x
		x.n = x.n + 1
		DISK-WRITE(x)
	else // x不是叶结点
		while i >= 1 and k < x.key[i]
			i = i - 1
		i = i + 1
		DISK-READ(x.c[i]) // 从磁盘读入新的磁盘页/结点
		if x.c[i].n == 2t - 1 // x.c[i]为满子结点
			B-TREE-SPLIT-CHILD(x, i)
			if k > x.key[i]
				i = i + 1
		B-TREE-INSERT-NONFULL(x.c[i], k) // 递归向非满根结点的子树插入k
```

![在这里插入图片描述](https://img-blog.csdnimg.cn/e7d5adeea8bb46b694b5a6fc59be1137.png)

过程 `B-TREE-INSERT-NONFULL` 的工作方式如下。图18-7说明了向B树中插入关键字的各种情况。
- 第 $3 \sim 8$ 行处理 $x$ 是叶结点的情况，将关键字 $k$ 插入 $x$ 。
- 如果 $x$ 不是叶结点，则必须将 $k$ 插入「以内部结点 $x$ 为根的**子树**」中适当的叶结点内。这种情况下，第 $9 \sim 11$ 行决定向 $x$ 的哪个子结点递归下降。
- 第 $13$ 行检查是否递归降至了一个满子结点上，若是，则第 $14$ 行用 `B-TREE-SPLIT-CHILD` 将该子结点分裂为两个非满的孩子，第 $15 \sim 16$ 行确定向「两个孩子中的哪一个」下降是正确的（注意，在第 $16$ 行中 $i = i +1$ 后无需做 `DISK-READ(x.c[i])` ，因为这种情况下递归会降至一个刚刚由 `B-TREE-SPLIT-CHILD` 创建的子结点上）。
- 第 $13 \sim 16$ 行的真正作用就是，**保证该程序始终不会降至一个满结点上**，然后第 $17$ 行递归地将 $k$ 插入合适的子树中。
![在这里插入图片描述](https://img-blog.csdnimg.cn/b559eaf3ba1a4d399adfd9bc05f7d32d.png)

对一棵高度为 $h$ 的B树来说，`B-TREE-INSERT` 要做 $O(h)$ 次磁盘存取，因为在每次调用 `B-TREE-INSERT-NONFULL` 之间，只做了 $O(1)$ 次 `DISK-READ` 和 `DISK-WRITE` 操作。所占用的总CPU时间为 $O(th) = O(t \log_t n)$ 。因为 `B-TREE-INSERT-NONFULL` 是尾递归的，所以我们也可以用一个while循环来实现它，从而说明了：**在任何时刻，需要留在主存中的页面数为 $O(1)$** `the number of pages that need to be in main memory at any time is O(1)` 。


---
# 4. 从B树中删除关键字
B树上的删除操作与插入操作类似，只是略微复杂一些，因为可以从任意一个结点中删除一个关键字、而不仅仅是叶结点，而且**当从一个内部结点删除一个关键字时，还要重新安排这个结点的孩子**。与插入操作一样，必须防止因删除操作而导致树的结构违反B树性质——==就像必须保证一个结点不会因为插入而变得太大一样，必须保证一个结点不会在删除期间变得太小（根结点除外，因为它允许有比最少关键字数 $t - 1$ 还少的关键字个数）==。一个简单插入算法，如果插入关键字的路径上结点满，可能需要向上回溯；与此类似，一个简单删除算法，当一个「处于要删除关键字的路径上的结点（非根）」有最少的关键字个数时，也可能需要向上回溯。

过程 `B-TREE-DELETE` 从「以 $x$ 为根的子树」中删除关键字 $k$ 。我们设计的这个过程必须保证，**无论何时结点 $x$ 递归调用自身时，$x$ 中关键字个数至少为最小度数 $t$** 。注意到，这个要求比「通常B树中的最少关键字个数」多一个以上，使得**有时在递归下降至子结点之前，需要把一个关键字移到子结点中** `a key may have to be moved into a child node before recursion descends to that child` 。这个加强的条件允许，在一趟下降过程中，就可以将一个关键字从树中删除，无需任何“向上回溯”（有一个例外，后面会解释）。

对下面的B树上删除操作的规定，应当这样理解：==如果根结点 $x$ 成为一个不含任何关键字的内部结点（这种情况可能出现在图18-8中的情况 $2c$ 和情况 $3b$ 中），那么 $x$ 就要被删除，$x$ 的唯一孩子 $x.c_1$ 成为树的新根==，从而**树的高度降低 $1$ ，同时也维持树根必须包含至少一个关键字的性质（除非树是空的）**。现在简要地介绍删除操作是如何工作的，而不是给出其伪代码。图18-8描述了从B树中删除关键字的各种情况。
1. 如果**关键字 $k$ 在结点 $x$ 中，并且 $x$ 是叶结点**，则从 $x$ 中删除 $k$ 。
2. 如果**关键字 $k$ 在结点 $x$ 中，并且 $x$ 是内部结点**，则做以下操作：
	a. 如果结点 $x$ 中**前于 $k$ 的子结点 $y$ 至少包含 $t$ 个关键字**，则找出 $k$ 在以 $y$ 为根的子树中的前驱 $k'$ 。递归地删除 $k'$ ，并**在 $x$ 中用 $k'$ 代替 $k$**（找到 $k'$ 并删除它，可在沿树下降的单程中完成）。
	b. 对称地，如果 $y$ 有少于 $t$ 个关键字，则检查结点 $x$ 中**后于 $k$ 的子结点 $z$ 。如果 $z$ 至少有 $t$ 个关键字**，则找出 $k$ 在以 $z$ 为根的子树中的后继 $k'$ 。递归地删除 $k'$ ，并**在 $x$ 中用 $k'$ 代替 $k$**（找到 $k'$ 并删除它，可在沿树下降的单程中完成）。
	c. 否则，**如果 $y$ 和 $z$ 都只含有 $t - 1$ 个关键字**，则==将 $k$ 和 $z$ 的全部合并进 $y$ ，这样 $x$ 就失去了 $k$ 和指向 $z$ 的指针==，并且 $y$ 现在包含 $2t - 1$ 个关键字（并且也包含关键字 $k$ ）。然后**释放 $z$ 、并递归地从 $y$ 中删除 $k$** 。
![在这里插入图片描述](https://img-blog.csdnimg.cn/eb001631453d442cb2dffdbf5fee26d0.png)
3. 如果**关键字 $k$ 当前不在内部结点 $x$ 中**，则确定必包含 $k$ 的子树的根 $x.c_i$（如果 $k$ 确实在树中）。==如果 $x.c_i$ 只有 $t - 1$ 个关键字，必须执行步骤 $3a$ 或 $3b$ 来保证：降至一个至少包含 $t$ 个关键字的结点==。然后，通过对 $x$ 的某个合适的子结点进行递归而结束。
 	a. **如果 $x.c_i$ 只含有 $t - 1$ 个关键字，但是它的一个相邻的兄弟至少包含 $t$ 个关键字**，则将 $x$ 的某一个关键字降至 $x.c_i$ 中，将 $x.c_i$ 的相邻左兄弟或右兄弟的一个关键字升至 $x$ ，将该兄弟中相应的孩子指针移到 $x.c_i$ 中，这样就使得 $x.c_i$ 增加了一个额外的关键字。
 	b. **如果 $x.c_i$ 以及 $x.c_i$ 的所有相邻兄弟都只包含 $t - 1$ 个关键字**，则将 $x.c_i$ 与一个兄弟合并，即==将 $x$ 的一个关键字移到新合并的结点，使之成为该结点的中间关键字==。
![在这里插入图片描述](https://img-blog.csdnimg.cn/c1405717b4834f538bdd49027fc7ba3b.png)


由于一棵B树中的大部分关键字都在叶结点中，我们可以预期**在实际中，删除操作最经常用于从叶结点中删除关键字**。这样 `B-TREE-DELETE` 过程只要沿树下降一趟即可，不需要向上回溯。==然而，当要删除某个内部结点的关键字时，该过程也要沿树下降一趟，但可能还要返回到「从中删除了关键字的那个结点」，以用「该关键字的前驱或后继」来取代被删除的关键字（情况 $2a$ 和情况 $2b$ ）== `When deleting a key in an internal node, however, the procedure makes a downward pass through the tree but may have to return to the node from which the key was deleted to replace the key with its predecessor or successor (cases 2a and 2b)` 。

尽管这个过程看起来很复杂，但对一棵高度为 $h$ 的B树，它只需要 $O(h)$ 次磁盘操作，因为在递归调用该过程之间，仅需 $O(1)$ 次对 `DISK-READ` 和 `DISK-WRITE` 的调用。所需CPU时间为 $O(th) = O(t \log_t n)$ 。
