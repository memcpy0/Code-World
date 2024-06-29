<p>You are given an <code>m x n</code> matrix of characters <code>box</code> representing a side-view of a box. Each cell of the box is one of the following:</p>

<ul>
	<li>A stone <code>'#'</code></li>
	<li>A stationary obstacle <code>'*'</code></li>
	<li>Empty <code>'.'</code></li>
</ul>

<p>The box is rotated <strong>90 degrees clockwise</strong>, causing some of the stones to fall due to gravity. Each stone falls down until it lands on an obstacle, another stone, or the bottom of the box. Gravity <strong>does not</strong> affect the obstacles' positions, and the inertia from the box's rotation <strong>does not </strong>affect the stones' horizontal positions.</p>

<p>It is <strong>guaranteed</strong> that each stone in <code>box</code> rests on an obstacle, another stone, or the bottom of the box.</p>

<p>Return <em>an </em><code>n x m</code><em> matrix representing the box after the rotation described above</em>.</p>
 
<p><strong>Example 1:</strong></p>

<p><img style="width: 300px; height: 150px;" src="https://assets.leetcode.com/uploads/2021/04/08/rotatingtheboxleetcodewithstones.png" alt=""></p>

```clike
Input: box = [["#",".","#"]]
Output: [["."],
         ["#"],
         ["#"]] 
```

<p><strong>Example 2:</strong></p>

<p><img style="width: 375px; height: 195px;" src="https://assets.leetcode.com/uploads/2021/04/08/rotatingtheboxleetcode2withstones.png" alt=""></p>

```clike
Input: box = [["#",".","*","."],
              ["#","#","*","."]]
Output: [["#","."],
         ["#","#"],
         ["*","*"],
         [".","."]]
```

 
<p><strong>Example 3:</strong></p>

<p><img style="width: 400px; height: 218px;" src="https://assets.leetcode.com/uploads/2021/04/08/rotatingtheboxleetcode3withstone.png" alt=""></p>

```clike
Input: box = [["#","#","*",".","*","."],
              ["#","#","#","*",".","."],
              ["#","#","#",".","#","."]]
Output: [[".","#","#"],
         [".","#","#"],
         ["#","#","*"],
         ["#","*","."],
         ["#",".","*"],
         ["#",".","."]]
```

<p><strong>Constraints:</strong></p>

<ul>
	<li><code>m == box.length</code></li>
	<li><code>n == box[i].length</code></li>
	<li><code>1 &lt;= m, n &lt;= 500</code></li>
	<li><code>box[i][j]</code> is either <code>'#'</code>, <code>'*'</code>, or <code>'.'</code>.</li>
</ul>

题意：给你一个&nbsp;<code>m x n</code>&nbsp;的字符矩阵&nbsp;<code>box</code>&nbsp;，它表示一个箱子的侧视图。箱子的每一个格子可能为： 
<ul>
	<li><code>'#'</code>&nbsp;表示石头</li>
	<li><code>'*'</code>&nbsp;表示固定的障碍物</li>
	<li><code>'.'</code>&nbsp;表示空位置</li>
</ul>

<p>这个箱子被 <strong>顺时针旋转 90 度</strong>&nbsp;，由于重力原因，部分石头的位置会发生改变。每个石头会垂直掉落，直到它遇到障碍物，另一个石头或者箱子的底部。重力 <strong>不会</strong>&nbsp;影响障碍物的位置，同时箱子旋转不会产生<strong>惯性</strong>&nbsp;，也就是说石头的水平位置不会发生改变。</p>

<p>题目保证初始时&nbsp;<code>box</code>&nbsp;中的石头要么在一个障碍物上，要么在另一个石头上，要么在箱子的底部。请你返回一个<em>&nbsp;</em><code>n x m</code>的矩阵，表示按照上述旋转后，箱子内的结果。</p>

---
### 解法1 暴力模拟
当我们把盒子顺时针旋转后，原来的每行变成了新的每列，由于石头只受到重力的作用垂直掉落，因此**每一列互不影响**。我们依次模拟计算每一行的结果，即得到新的每一列的结果。注意，**由于重力向下，应当从右往左遍历原来的每行**。
```cpp
class Solution {
public:
    vector<vector<char>> rotateTheBox(vector<vector<char>>& box) {
        int m = box.size(), n = box[0].size();
        for (int i = 0; i < m; ++i) {
            for (int j = n - 1; j >= 0; --j) {
                if (box[i][j] == '#') { //如果石头下是底部或者障碍物或者石头,位置不变
                    if (j == n - 1 || box[i][j + 1] == '*' || box[i][j + 1] == '#') continue;
                    box[i][j] = '.'; //否则变成空位置
                    for (int k = j; k < n; ++k) {
                        if (k == n - 1 || box[i][k + 1] == '*' || box[i][k + 1] == '#') {
                            box[i][k] = '#'; 
                            break;
                        }
                    }
                }
            }
        }
        vector<vector<char>> ans(n, vector<char>(m)); //矩阵顺时针旋转90度
        for (int i = m - 1; i >= 0; --i) for (int j = 0; j < n; ++j) ans[j][m - i - 1] = box[i][j];
        return ans;
    }
};
```
运行效率如下：
```cpp
执行用时：364 ms, 在所有 C++ 提交中击败了18.55% 的用户
内存消耗：51.3 MB, 在所有 C++ 提交中击败了48.77% 的用户
```
---
### 解法2 队列维护空位
如果从右往左对每一块需要垂直下落的石头，我们都向右搜索合适的位置，不免做了重复的操作，我们完全可以批量将石头放到合适的空位置。

具体做法是，我们从右往左遍历时，记录每一个空位；遇到障碍物时，清空目前记录的空位，因为**障碍物右侧的空位都是没有用的**；遇到石头（且空位队列非空）时，取出我们当前记录的第一个空位，将石头放进去，此时**石头的位置也变成了空位，需要加入队列**。不难发现，这是一个先进先出的操作顺序，所以需要用到队列。

这样处理原来的每一行只需要 $O(n)$ 复杂度，总体只需要 $O(mn)$ 的时间复杂度。队列需要用到的空间为 $O(n)$ 。
```cpp
class Solution {
public:
    vector<vector<char>> rotateTheBox(vector<vector<char>>& box) {
        int m = box.size(), n = box[0].size();
        for (int i = 0; i < m; ++i) {
            deque<int> q;
            for (int j = n - 1; j >= 0; --j) { 
                if (box[i][j] == '.') q.push_back(j); //将空位的位置放入队列
                else if (box[i][j] == '*') q.clear(); //清空队列
                else if (box[i][j] == '#' && !q.empty()) { 
                    swap(box[i][j], box[i][q.front()]); 
                    q.pop_front(); 
                    q.push_back(j); //此时石头的位置也变成了空位
                }
            }
        }
        vector<vector<char>> ans(n, vector<char>(m));
        for (int i = m - 1; i >= 0; --i) for (int j = 0; j < n; ++j) ans[j][m - i - 1] = box[i][j];
        return ans;
    }
};
```
运行效率显著提高：
```cpp
执行用时：280 ms, 在所有 C++ 提交中击败了46.67% 的用户
内存消耗：55.5 MB, 在所有 C++ 提交中击败了16.10% 的用户
```
---
### 解法3 双指针
仔细想想，我们甚至可以不使用队列！由于每一行或者说新的每一列都是**独立的**，我们分别处理每一行，将石头向右移动到底部、障碍物上或者其他石头上——不是很类似**移动零**这样将某种元素移动到数组末尾的题目吗？因此通过认真考虑，可以得出一个双指针的解法。

这种方法也可以从解法2中思考得出。在解法2中，遍历完某个位置 `j` 之后，如果队列不为空（没有遇到障碍物，位置 `j` 是石头或者空位），则**队尾一定是 `j` 位置，且该位置一定是空位、或者原本是石头但下落后变成空位；队列中的位置一定连续**。 因此我们只需要维护队首对应的位置，整个队列隐式存在。 
```cpp
class Solution {
public:
    vector<vector<char>> rotateTheBox(vector<vector<char>>& box) {
        int m = box.size(), n = box[0].size();
        for (int i = 0; i < m; ++i) { 
            int pos = n - 1; //队首对应的位置
            for (int j = n - 1; j >= 0; --j) { 
                if (box[i][j] == '*') pos = j - 1; //遇到障碍物,清空队列,队首位置变化
                else if (box[i][j] == '#' ) { 
                    swap(box[i][j], box[i][pos]);
                    --pos;
                }
            }
        }
        vector<vector<char>> ans(n, vector<char>(m));
        for (int i = m - 1; i >= 0; --i) for (int j = 0; j < n; ++j) ans[j][m - i - 1] = box[i][j];
        return ans;
    }
};
```
运行效率如下：
```cpp
执行用时：256 ms, 在所有 C++ 提交中击败了84.60% 的用户
内存消耗：51.2 MB, 在所有 C++ 提交中击败了71.30% 的用户
```
