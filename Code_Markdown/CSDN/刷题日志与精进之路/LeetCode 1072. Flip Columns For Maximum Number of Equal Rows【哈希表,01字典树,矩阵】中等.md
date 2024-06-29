> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

给定 `m x n` 矩阵 `matrix` 。

你可以从中选出任意数量的列并翻转其上的 **每个** 单元格。（即翻转后，单元格的值从 `0` 变成 `1`，或者从 `1` 变为 `0` 。）

返回 _经过一些翻转后，行与行之间所有值都相等的最大行数_ 。

**示例 1：**
```java
输入：matrix = [[0,1],[1,1]]
输出：1
解释：不进行翻转，有 1 行所有值都相等。
```
**示例 2：**
```java
输入：matrix = [[0,1],[1,0]]
输出：2
解释：翻转第一列的值之后，这两行都由相等的值组成。
```
**示例 3：**
```java
输入：matrix = [[0,0,0],[0,0,1],[1,1,0]]
输出：2
解释：翻转前两列的值之后，后两行由相等的值组成。
```
**提示：**
-   `m == matrix.length`
-   `n == matrix[i].length`
-   `1 <= m, n <= 300`
-   `matrix[i][j] == 0` 或 `1`

---
### 解法1 并查集
从答案出发倒着思考，关注最后全为 $0$ 或者全为 $1$ 的行，倒数第二步是什么样的？假如倒数第二步是翻转最后一列，$000$ 就变成 $001$ ，$111$ 变成 $110$ 。

从这个例子可以发现，**对于相同的行，或者「互补」的行，一定存在一种翻转方式，可以使这些行最终全为 $0$ 或者全为 $1$** 。 或者反过来说，==对于两行，如果它们经过（某些）列旋转之后、该行元素全部相同，那么变换之前有两种情况，这两行所有列的元素==：
- 要么全部相同，如两行都是 $011$ ，翻转第一列就都变为 $111$ ；
- 要么全部相反（互补），如两行分别是 $011, 100$ ，翻转第一列就分别变为 $111, 000$ 。

从图论的角度看，根据此规则，我们可以判断两行（相同或互补）是否可以属于一个**集合**或**连通块**；用并查集找到最大的连通分量即可。
```java
class Solution {
    private int find(int[] parent, int p) {
        return parent[p] != p ? parent[p] = find(parent, parent[p]) : parent[p]; 
    }
    private void union(int[] parent, int p, int q) {
        parent[find(parent, p)] = find(parent, q);
    }
    public int maxEqualRowsAfterFlips(int[][] matrix) {
        int[] parent = new int[matrix.length];
        for (int i = 0; i < parent.length; ++i) parent[i] = i;
        for (int i = 0; i < matrix.length; i++) {
            for (int j = i + 1; j < matrix.length; j++) {
                boolean flag = true;
                for (int k = 0; k < matrix[0].length; k++) {
                    int v = Math.abs(matrix[i][0] - matrix[j][0]);
                    if (Math.abs(matrix[i][k] - matrix[j][k]) != v) {
                        flag = false;
                        break;
                    }
                }
                if (flag) union(parent, i, j);
            }
        }
        int[] cnts = new int[matrix.length];
        for (int i = 0; i < parent.length; i++) {
            find(parent, i);
            cnts[parent[i]]++;
        }
        int ans = 0;
        for (int cnt : cnts) ans = Math.max(ans, cnt); 
        return ans;
    }
}
```
但这个方法的时间复杂度很高，究其原因是因为**所有行必须两两比对才能判断他们是否相同或互补、才能归并到同一个集合中**，其实可以根据行自身的性质就可判断。

---
### 解法2 哈希表 
实际上，我们不需要建图/并查集，而是**用哈希表统计这些行**。==为了统计互补的行，可以把一行全部翻转==。例如示例3，把最后一行翻转得到 $001$（变成互补的行），发现与第二行是一样的，所以答案等于 $2$ 。

具体到代码，不同语言有着不同的实现方式：
- Python 直接转成 tuple 放入哈希表中。
- C++ 和 Java 转成字符串。

```java
class Solution {
    public int maxEqualRowsAfterFlips(int[][] matrix) {
        int ans = 0;
        var rec = new HashMap<String, Integer>();
        for (int i = 0; i < matrix.length; ++i) {
            var key = new StringBuilder();
            var revKey = new StringBuilder();
            // 对key中的0列取反,则key全为1
            // key对应的revKey(相应位置上的值都相反),则相关1列都变为0
            for (int j = 0; j < matrix[i].length; ++j) {
                key.append((char)('0' + matrix[i][j]));
                revKey.append(key.charAt(j) == '0' ? '1' : '0');
            }
            String s = key.toString();
            rec.put(s, rec.getOrDefault(s, 0) + 1);            
            ans = Math.max(ans, rec.get(s) + rec.getOrDefault(revKey.toString(), 0));
        }
        return ans;
    }
}
```
==上述写法会存储每行的字符串形式，还会把每行都翻转，并查询哈希表中存储的对应字符串的数量==——实际上没有必要，我们可继续优化。比如说 $001, 110$ 或 $110, 001$ ，我们翻转 $110$ ，将其中每个元素和开头的 $1$ 异或，得到 $001$ ，并在哈希表中增加 $001$ 的数量。

也就是说，我们**只存储 $0$ 开头的字符串及其数量，对 $1$ 开头的字符串先进行翻转、再来存储**。
```java
class Solution {
    public int maxEqualRowsAfterFlips(int[][] matrix) {
        int ans = 0, n = matrix[0].length;
        var cnt = new HashMap<String, Integer>();
        for (var row : matrix) {
            var r = new char[n];
            for (int j = 0; j < n; ++j)
                r[j] = (char)(row[j] ^ row[0]); // 翻转第一个数为1的行
            int c = cnt.merge(new String(r), 1, Integer::sum);
            ans = Math.max(ans, c);
        }
        return ans;
    }
}
```
复杂度分析：
- 时间复杂度：$O(mn)$  
- 空间复杂度：$O(mn)$ 

---
### 解法3 01字典树/二叉树判断
如果在某些列翻转后，两行最终都能转化成每行元素都相同，那么变换之前这两行的元素要么全部相异，要么全部相同。而且，**将这两行的每列元素分别与该行第一列元素相比，得出的比较结果相同**。

比如 `matrix = [[0,0,1],[0,0,1],[1,1,0]]` ，我们将每行的每列元素与每行第一列元素相比，值分别是：
- 第一行：相同，相同，相反 
- 第二行：相同，相同，相反 
- 第三行：相同，相同，相反 
- 故第一、二、三行都属于一个集合。

不难想到，我们可用01字典树（也是一棵二叉树）来表示，根结点为虚结点，不存储值，每行的每列元素与每行第一列元素相比，相同则走左节点，相反则为右节点，**叶子节点存储所有规则相同的行的数量**。

我们只需要遍历 `matrix` 中的每行，不断将每行的每列元素与第一列比较，根据比较结果，要么创建对应的左/右结点，要么直接访问对应的左/右结点。
```java
class Solution { 
    private class Node {
        Node left;
        Node right;
        int cnt;
        public Node() { cnt = 0; }
    }
    public int maxEqualRowsAfterFlips(int[][] matrix) {
        int ans = 0;
        Node root = new Node();
        for (int[] row : matrix) {
            Node e = root;
            for (int i : row) {
                if (i != row[0]) {
                    if (e.right == null) e.right = new Node();
                    e = e.right;
                } else {
                    if (e.left == null) e.left = new Node();
                    e = e.left;
                }
            }
            ++e.cnt;
            ans = Math.max(ans, e.cnt);
        }
        return ans;
    }
}
```
复杂度分析：
- 时间复杂度：$O(mn)$ ，只需遍历一次 `matrix` 即可。
- 空间复杂度：$O(mn)$ 
