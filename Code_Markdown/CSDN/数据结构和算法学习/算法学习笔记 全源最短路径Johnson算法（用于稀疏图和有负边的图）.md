@[toc]

> 参考内容：
> 算法导论 第三版 25.3 用于稀疏图的Johnson算法



Johnson算法可在 $O(V^2 \log V +VE)$ 的时间内，找到所有结点对之间的最短路径。对于稀疏图（边比较少）来说，**Johnson算法**的渐进表现要优于 `asymptotically faster` **重复平方法** `repeated squaring of matrices ` 和**Floyd-Warshall算法**。==Johnson算法要么返回「一个包含所有结点对的最短路径权重的矩阵」，要么报告「输入图包含一个权重为负值的环路」== `The algorithm either returns a matrix of shortest-path weights for all pairs of vertices or reports that the input graph contains a negative-weight cycle` 。此外，Johnson算法在运行中，需要使用Dijkstra算法和Bellman-Ford算法，作为自己的子程序。

Johnson算法使用的技术，称为**重新赋予权重** `reweighting` 。该技术的工作原理如下：
1. 如果图 $G = (V, E)$ 中所有的边权重 $w$ 都为非负值，就可以通过「对每个结点运行一次Dijkstra算法」来找到所有结点对之间的最短路径。如果使用斐波那契堆最小优先队列，该算法的运行时间为 $O(V^2 \log V+VE)$ 。
2. 如果图 $G$ 包含权重为负值的边（负边），但没有权重为负值的环路（负环），那么只要计算出一组新的非负权重值，然后使用同样的方法即可（第一步）`simply compute a new set of nonnegative edge weights that allows us to use the same method` 。**新赋予的权重（函数）$\hat{w}$ 必须满足下面两个重要性质**：
	- 对于所有结点对 $u, v \in V$ ，一条==路径 $p$ 是在使用权重函数 $w$ 时、从结点 $u$ 到结点 $v$ 的一条最短路径==，当且仅当 ==$p$ 是在使用权重函数 $\hat{w}$ 时、从 $u$ 到 $v$ 的一条最短路径==（简记为**最短路径不变性**）。
	- 对于所有的边 $(u, v)$ ，新权重 $\hat{w} (u, v)$ 为非负值。

正如将要看到的，我们可以对图 $G$ 进行预处理，并在 $O(VE)$ 的时间内计算出 $\hat{w}$ 。

# 1. 重新赋予权重来维持最短路径
下面的引理描述的是，**我们可以很容易地对边的权重进行重新赋值、以满足上面的两个条件**。使用 $\delta$ 表示**从权重函数 $w$ 所导出的最短路径权重**，$\hat{\delta}$ 表示**从权重函数 $\hat{w}$ 所导出的最短路径权重**。

**引理25.1**（**重新赋予权重并不改变最短路径**、也不改变负环）给定带权重的有向图 $G = (V, E)$ ，其权重函数为 $w: E\to \R$ 。同时，设 $h: V\to \R$ 为任意函数，该函数将结点映射到实数上。对于每条边 $(u, v) \in E$ ，定义：$$\tag{25.9} \hat{w}(u, v) = w(u, v) + h(u) - h(v)$$

设 $p = \langle v_0, v_1, \dots, v_k\rangle$ 为从结点 $v_0$ 到结点 $v_k$ 的任意一条路径，那么 ==$p$ 是在使用权重函数 $w$ 时、从结点 $v_0$ 到结点 $v_k$ 的一条最短路径==，当且仅当 ==$p$ 是在使用权重函数 $\hat{w}$ 时、从结点 $v_0$ 到结点 $v_k$ 的一条最短路径==，即 `That is` $w(p) = \delta(v_0, v_k)$ 当且仅当 $\hat{w}(p) = \hat{\delta}(v_0, v_k)$ 。而且，==图 $G$ 在使用权重函数 $w$ 时、包含一个权重为负值的环路==，当且仅当 ==$G$ 在使用权重函数 $\hat{w}$ 时、也包含一个权重为负值的环路==。

**证明** 首先来证明：$$\tag{25.10} \hat{w}(p) = w(p) + h(v_0) - h(v_k)$$ 我们有：
$$\hat{w}(p) = \sum^k_{i=1} \hat{w}(v_{i-1}, v_i) \\
= \sum^k_{i=1} \bigg( w(v_{i-1}, v_i) + h(v_{i-1}) - h(v_i)\bigg) \\
=\bigg( \sum^k_{i=1} w(v_{i-1}, v_i) \bigg) + h(v_0) - h(v_k) \quad (因为裂项相消和) \\
= w(p) + h(v_0) - h(v_k)
$$ 因此，对于结点 $v_0$ 到结点 $v_k$ 的任意路径 $p$ ，我们有 $\hat{w}(p) = w(p) + h(v_0) - h(v_k)$ 。==因为 $h(v_0), h(v_k)$ 不依赖于任何具体的路径，如果结点 $v_0$ 到结点 $v_k$ 的一条路径、在使用权重函数 $w$ 时、比另一条路径短，则其在使用权重函数 $\hat{w}$ 时也比另一条短==。因此，$w(p) = \delta(v_0, v_k)$ 当且仅当 $\hat{w}(p) = \hat{\delta}(v_0, v_k)$ 。

最后我们证明，图 $G$ 在使用权重函数 $w$ 时、包含一个权重为负值的环路，当且仅当 $G$ 在使用权重函数 $\hat{w}$ 时、也包含一个权重为负值的环路。考虑任意环路 $c = \langle v_0, v_1, \dots, v_k\rangle$ ，其中 $v_0 = v_k$ 。根据式 $(25.10)$ ，我们有 $\hat{w}(c) = w(c) + h(v_0) - h(v_k) = w(c)$ 。因此，==环路 $c$ 在使用权重函数 $w$ 时为负值、当且仅当其在使用权重函数 $\hat{w}$ 时也为负值==。

# 2. 通过重新赋值来生成非负权重
我们的下一个目标是**确保第二个属性保持成立**，即对于所有的边 $(u, v) \in E$ ，$\hat{w}(u, v)$ 为非负值。

给定带权重的有向图 $G= (V, E)$ ，其权重函数为 $w: E\to \R$ 。我们制作一幅新图 $G' = (V', E')$ ，这里 $V' = V\cup \{ s\}$ ，$s$ 是一个新结点，$s \notin V$ ，$E' = E\cup \{  (s, v) \mid v \in V\}$ 。还对权重函数 $w$ 进行扩展，使得对于所有的结点 $v \in V,\ w(s, v) = 0$ 。注意，==因为结点 $s$ 没有进入的边，除了以 $s$ 为源结点的最短路径外，图 $G'$ 中没有其他包含结点 $s$ 的最短路径==。而且，**图 $G'$ 不包含权重为负值的环路、当且仅当图 $G$ 不包含权重为负值的环路**（因为图 $G'$ 的负环还是图 $G$ 的负环，权值也一样）。图25-6(a)描述的是对应图 $G$ 的图 $G'$ 。

现在**假定图 $G$ 和图 $G'$ 都不包含权重为负值的环路**（实际操作中，通过Bellman-Ford算法排除含有负环的图，排除后才重新赋值来生成非负权重）。让我们定义，对于所有的结点 $v \in V'$ ，$h(v) =\delta(s, v)$ 。==根据三角不等式（引理 $24.10$ ），对于所有的边 $(u, v) \in E'$ ，有 $h(v) \le h(u) + w(u, v)$== 。因此，==如果我们根据式 $25.9$ 来定义新的权重 $\hat{w}$ ，则有 $\hat{w}(u, v) = w(u, v) + h(u) - h(v) \ge 0$== 。至此，我们满足了第二条性质。图25-6(b)描述的是对图25-6(a)的图进行权重重新赋值后的图 $G'$ 。
![在这里插入图片描述](https://img-blog.csdnimg.cn/4af32ad4eb8e4d5380ce38deadc927c5.png)

# 3. 计算所有结点对之间的最短路径
Johnson算法在执行过程中，需要使用Bellman-Ford算法和Dijkstra算法作为子程序，计算所有结点对之间的最短路径。该算法假定所有的边都保存在邻接链表里，其==返回的则是一个 $|V| \times |V|$ 的矩阵 $D = d_{ij}$（其中 $d_{ij} = \delta(i, j)$ ），或者报告输入图包含一个权重为负值的环路==。对于「所有结点对最短路径算法」来说，通常假定结点的编号为 $1\sim |V|$ 。

下面的代码执行的就是前面讨论的操作。
1. 先生成图 $G'$ ，接着在图 $G'$ 上运行Bellman-Ford算法，使用权重函数 $w$ 和源结点 $s$ 。如果图 $G'$（也因此图 $G$ ）包含一条权重为负值的环路，算法将报告这个问题。
2. 其后的算法，假定图 $G'$ 不包含权重为负值的环路。
3. 随后，将 $h(v)$ 的值设置为「由Bellman-Ford算法所计算出来的最短路径权重 $\delta(s, v)$」。
4. 接着，计算新的权重 $\hat{w}$ 。
5. 然后，对每一对结点 $u, v \in V$ ，算法用for循环、通过调用Dijkstra算法来计算最短路径权重 $\hat{\delta}(u, v)$ 。之后将根据式 $25.10$ 所计算出来的最短路径权重 $\delta(u, v)$ 保存在矩阵元素 $d_{uv}$ 中。
6. 最后，返回构造完毕的矩阵 $D$ 。

注意，代码中用 $w', \delta'$ 代替 $\hat{w}, \hat{\delta}$ ：
```cpp
JOHNSON(G, w)
compute G', where G'.V = G.V ∪ {s},
	G'.E = G.E ∪ {(s, v) | v ∈ G.V}, and
	w(s, v) = 0 for all v ∈ G.V
if (BELLMAN-FORD(G, w, s) == false)
	printf("the input graph contains a negative-weight cycle")
else {
	for each vertex v ∈ G'.V {
		set h(v) to the value of δ(s, v)
			computed by the Bellman-Ford algorithm
	}
	for each edge(u, v) ∈ G'.E 
		w' = w(u, v) + h(u) - h(v)
	let D = (duv) be a new n × n matrix
	for earch vertex u ∈ G.V {
		run DIJKSTRA(G, w', u) to compute δ'(u, v) for all v ∈ G.V
		for each vertex v ∈ G.V
			duv = δ'(u, v) + h(v) - h(u)
	}
	return D
}
```
如果使用斐波那契堆来实现Dijkstra算法里面的最小优先队列，则Johnson算法的运行时间为 $O(V^2 \log V+VE)$ 。使用更简单的二叉最小堆实现，则运行时间为 $O(VE\log V)$ 。在稀疏图的情况下，该时间仍然比Floyd-Warshall算法的时间表现要好。
