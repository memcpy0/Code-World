> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

在一个由 `m` 个用户组成的社交网络里，我们获取到一些用户之间的好友关系。两个用户之间可以相互沟通的条件是他们都掌握同一门语言。

给你一个整数 `n` ，数组 `languages` 和数组 `friendships` ，它们的含义如下：
- 总共有 `n` 种语言，编号从 `1` 到 `n` 。
- `languages[i]` 是第 `i` 位用户掌握的语言集合。
- `friendships[i] = [u​​​​​​i​​​, v​​​​​​i]` 表示 `u​​​​​​​​​​​i`​​​​​ 和 `vi` 为好友关系。

你可以选择 **一门** 语言并教会一些用户，使得所有好友之间都可以相互沟通。请返回你 **最少** 需要教会多少名用户。

请注意，好友关系没有传递性，也就是说如果 `x` 和 `y` 是好友，且 `y` 和 `z` 是好友， `x` 和 `z` 不一定是好友。

**示例 1：**
```c
输入：n = 2, languages = [[1],[2],[1,2]], friendships = [[1,2],[1,3],[2,3]]
输出：1
解释：你可以选择教用户 1 第二门语言，也可以选择教用户 2 第一门语言。
```
**示例 2：**
```c
输入：n = 3, languages = [[2],[1,3],[1,2],[3]], friendships = [[1,4],[1,2],[3,4],[2,3]]
输出：2
解释：教用户 1 和用户 3 第三门语言，需要教 2 名用户。
```
**提示：**
- `2 <= n <= 500`
- `languages.length == m`
- `1 <= m <= 500`
- `1 <= languages[i].length <= n`
- `1 <= languages[i][j] <= n`
- `1 <= u​​​​​​i < v​​​​​​i <= languages.length`
- `1 <= friendships.length <= 500`
- 所有的好友关系 `(u​​​​​i, v​​​​​​i)` 都是唯一的。
- `languages[i]` 中包含的值互不相同。
---
### 方法一 暴力枚举
题目说，好友关系没有传递性，所以只需关心 $friendships$ 中的每对好友能否沟通，也就是**两人是否都会同一门语言**。

题目说，我们只能选一门语言并教会一些用户。那么**枚举**选择的语言是 $k = 1, 2, 3, \dots, n$ 。

对于不会语言 $k$ 的用户，是否一定要教他语言 $k$ 呢？不一定，比如某人和他的好友都会英语，那就不需要教他们外语。如果我们发现一对好友没有共同语言，就需要教其中不会语言 $k$ 的人。

具体来说：
1. 枚举选择的语言是 $k = 1, 2, 3, \dots, n$ 。
2. 遍历 $friendships$ 。
3. 如果发现一对好友没有共同语言，那么标记其中不会语言 $k$ 的用户。
	- 代码实现时，可以把 $languages[i]$ 转换为哈希集合或布尔数组，从而快速求出两个集合是否有交集。
4. 遍历结束后，被标记的用户个数，即为需要学习语言 $k$ 的用户数，更新答案的最小值。可以用集合来对标记的用户去重。
```java
class Solution {
    public int minimumTeachings(int n, int[][] languages, int[][] friendships) {
        int m = languages.length;
        boolean[][] learned = new boolean[m][n + 1];
        for (int i = 0; i < m; i++) {
            for (int x : languages[i]) {
                learned[i][x] = true;
            }
        }

        List<int[]> todoList = new ArrayList<>();
        next:
        for (int[] f : friendships) {
            int u = f[0] - 1, v = f[1] - 1; // 减一，下标从 0 开始
            for (int x : languages[u]) {
                if (learned[v][x]) { // 两人可以相互沟通，无需学习语言
                    continue next;
                }
            }
            todoList.add(f);
        }

        int ans = m;
        for (int k = 1; k <= n; k++) { // 枚举需要教的语言 k
            Set<Integer> set = new HashSet<>();
            for (int[] f : todoList) {
                int u = f[0] - 1, v = f[1] - 1;
                if (!learned[u][k]) { // u 需要学习语言 k
                    set.add(u);
                }
                if (!learned[v][k]) { // v 需要学习语言 k
                    set.add(v);
                }
            }
            ans = Math.min(ans, set.size()); // set.size() 是需要学习语言 k 的人数
        }
        return ans;
    }
}
```
```cpp
class Solution {
public:
    int minimumTeachings(int n, vector<vector<int>>& languages, vector<vector<int>>& friendships) {
        int m = languages.size();
        vector learned(m, vector<int8_t>(n + 1));
        for (int i = 0; i < m; i++) {
            for (int x : languages[i]) {
                learned[i][x] = true;
            }
        }

        vector<pair<int, int>> todo_list;
        for (auto& f : friendships) {
            int u = f[0] - 1, v = f[1] - 1; // 减一，下标从 0 开始
            bool ok = false;
            for (int x : languages[u]) {
                if (learned[v][x]) { // 两人可以相互沟通，无需学习语言
                    ok = true;
                    break;
                }
            }
            if (!ok) {
                todo_list.emplace_back(u, v);
            }
        }

        int ans = m;
        for (int k = 1; k <= n; k++) { // 枚举需要教的语言 k
            unordered_set<int> st;
            for (auto& [u, v] : todo_list) {
                if (!learned[u][k]) { // u 需要学习语言 k
                    st.insert(u);
                }
                if (!learned[v][k]) { // v 需要学习语言 k
                    st.insert(v);
                }
            }
            ans = min(ans, (int) st.size()); // st.size() 是需要学习语言 k 的人数
        }
        return ans;
    }
};
```
```rust
use std::collections::HashSet;

impl Solution {
    pub fn minimum_teachings(n: i32, languages: Vec<Vec<i32>>, friendships: Vec<Vec<i32>>) -> i32 {
        let m = languages.len();
        let un = n as usize + 1;
        let mut learned = vec![ vec![false; un] ; m]; // m个人, n种语言
        for i in 0..m {
            for x in &languages[i] { // 隐式地调用languages[i].iter()
                learned[i][*x as usize] = true;
            }
        }

        let mut todo_list = Vec::new(); // 找出无法相互沟通的好友对
        for f in &friendships {
            let u = (f[0] - 1) as usize;
            let v = (f[1] - 1) as usize; // 减去1，下标从0开始
            let mut ok = false;
            for x in &languages[u] {
                if learned[v][*x as usize] { // 两人可以相互沟通，无需学习语言
                    ok = true;
                    break;
                }
            }
            if !ok {
                todo_list.push([u, v]);
            }
        }

        let mut ans = m as i32;
        for i in 1..n+1 { // 枚举需要教的语言i
            let mut st = HashSet::new();
            for f in &todo_list {
                let u = f[0];
                let v = f[1];
                if !learned[u][i as usize] { // u需要学语言i
                    st.insert(u);   // 避免重复计数学过语言i的人
                }
                if !learned[v][i as usize] { // v需要学语言i
                    st.insert(v);
                }
            }
            ans = ans.min(st.len() as i32);
        }
        ans
    }
}
```
```python
class Solution:
    def minimumTeachings(self, n: int, languages: List[List[int]], friendships: List[List[int]]) -> int:
        learned = list(map(set, languages))

        todo_list = []
        for u, v in friendships:
            # u 和 v 减一，下标从 0 开始
            if learned[u - 1].isdisjoint(learned[v - 1]):  # 无交集
                todo_list.append((u - 1, v - 1))

        ans = inf
        for k in range(1, n + 1):  # 枚举需要教的语言 k
            st = set()
            for u, v in todo_list:
                if k not in learned[u]:  # u 需要学习语言 k
                    st.add(u)
                if k not in learned[v]:  # v 需要学习语言 k
                    st.add(v)
            ans = min(ans, len(st))  # len(st) 是需要学习语言 k 的人数
        return ans
# 写法二
class Solution:
    def minimumTeachings(self, n: int, languages: List[List[int]], friendships: List[List[int]]) -> int:
        learned = list(map(set, languages))
        todo_list = [(u - 1, v - 1) for u, v in friendships if learned[u - 1].isdisjoint(learned[v - 1])]

        ans = inf
        for k in range(1, n + 1):  # 枚举需要教的语言 k
            st = set()
            for u, v in todo_list:
                if k not in learned[u]:  # u 需要学习语言 k
                    st.add(u)
                if k not in learned[v]:  # v 需要学习语言 k
                    st.add(v)
            ans = min(ans, len(st))  # len(st) 是需要学习语言 k 的人数
        return ans
```
```go
func minimumTeachings(n int, languages [][]int, friendships [][]int) int {
	m := len(languages)
	learned := make([][]bool, m)
	for i, list := range languages {
		learned[i] = make([]bool, n+1)
		for _, x := range list {
			learned[i][x] = true
		}
	}

	todoList := [][2]int{}
next:
	for _, f := range friendships {
		u, v := f[0]-1, f[1]-1 // 减一，下标从 0 开始
		for _, x := range languages[u] {
			if learned[v][x] { // 两人可以相互沟通，无需学习语言
				continue next
			}
		}
		todoList = append(todoList, [2]int{u, v})
	}

	ans := m
	for k := 1; k <= n; k++ { // 枚举需要教的语言 k
		set := map[int]struct{}{}
		for _, f := range todoList {
			u, v := f[0], f[1]
			if !learned[u][k] { // u 需要学习语言 k
				set[u] = struct{}{}
			}
			if !learned[v][k] { // v 需要学习语言 k
				set[v] = struct{}{}
			}
		}
		ans = min(ans, len(set)) // len(set) 是需要学习语言 k 的人数
	}
	return ans
}
```
复杂度分析：
- 时间复杂度：$O(nm + nf)$ ，$n$ 是语言种数，$m$ 是 $languages$ 长度，$f$ 是 $friendships$ 的长度。
- 空间复杂度：$O(nm + f)$ 。
---
### 方法二 贪心+计数
比如有 $10$ 个人无法沟通，其中说中文的人最多，有 $8$ 个，那么只需教剩下的 $10 - 8 = 2$ 个人会中文（少数服从多数）。

所以统计两类信息：
1. 有多少人无法沟通，记作 $total$ ；
2. 对于无法沟通的人，遍历这个人的语言列表，把语言 $x$ 的出现次数加一，即 $cnt[x]$ 加一。

最多有 $\max(cnt)$ 个人会说通一种语言，其余的 $total - \max(cnt)$ 个人需要学会这么语言。
```java
class Solution {
    public int minimumTeachings(int n, int[][] languages, int[][] friendships) {
        int m = languages.length;
        boolean[][] learned = new boolean[m][n + 1]; // 记录每个人学的语言
        for (int i = 0; i < m; i++) {
            for (int x : languages[i]) {
                learned[i][x] = true;
            }
        }

        boolean[] vis = new boolean[m];
        int[] cnt = new int[n + 1];

        next:
        for (int[] f : friendships) {
            int u = f[0] - 1, v = f[1] - 1; // 下标从 0 开始
            for (int x : languages[u]) {
                if (learned[v][x]) { // 两人可以相互沟通，无需学习语言
                    continue next;
                }
            }
            add(u, languages, vis, cnt);
            add(v, languages, vis, cnt);
        }

        int maxCnt = 0;
        for (int c : cnt) {
            maxCnt = Math.max(maxCnt, c);
        }

        return total - maxCnt;
    }

    private int total = 0;

    private int add(int u, int[][] languages, boolean[] vis, int[] cnt) {
        if (vis[u]) {
            return total;
        }
        vis[u] = true; // 避免重复统计
        total++;
        for (int x : languages[u]) {
            cnt[x]++;
        }
        return total;
    }
}
```
```cpp
class Solution {
public:
    int minimumTeachings(int n, vector<vector<int>>& languages, vector<vector<int>>& friendships) {
        int m = languages.size();
        vector learned(m, vector<int8_t>(n + 1));
        for (int i = 0; i < m; i++) {
            for (int x : languages[i]) {
                learned[i][x] = true;
            }
        }

        int total = 0;
        vector<int8_t> vis(m);
        vector<int> cnt(n + 1);

        auto add = [&](int u) -> void {
            if (vis[u]) {
                return;
            }
            vis[u] = true; // 避免重复统计
            total++;
            for (int x : languages[u]) {
                cnt[x]++;
            }
        };

        for (auto& f : friendships) {
            int u = f[0] - 1, v = f[1] - 1; // 下标从 0 开始
            bool ok = false;
            for (int x : languages[u]) {
                if (learned[v][x]) { // 两人可以相互沟通，无需学习语言
                    ok = true;
                    break;
                }
            }
            if (!ok) {
                add(u);
                add(v);
            }
        }

        return total - ranges::max(cnt);
    }
};
```
```rust
use std::collections::HashSet;

impl Solution {
    pub fn minimum_teachings(n: i32, languages: Vec<Vec<i32>>, friendships: Vec<Vec<i32>>) -> i32 {
        let m = languages.len();
        let un = n as usize + 1;
        let mut learned = vec![vec![false; un]; m]; // m个人, n种语言
        for i in 0..m {
            for x in &languages[i] { // 隐式地调用languages[i].iter()
                learned[i][*x as usize] = true;
            }
        }

        let mut total = 0; // 有多少人无法沟通
        let mut cnt = vec![0; un]; // 计算每种语言最多有多少人会
        let mut vis = vec![false; m]; // 避免重复统计某个人会的语言

        for f in &friendships {
            let u = (f[0] - 1) as usize;
            let v = (f[1] - 1) as usize;
            let mut ok = false;
            for x in &languages[u] {
                if learned[v][*x as usize] { // 两个人可以相互沟通，无需学习语言
                    ok = true;
                    break;
                }
            }
            if !ok {
                if !vis[u] {
                    vis[u] = true;
                    total += 1;
                    for x in &languages[u] {
                        cnt[*x as usize] += 1;
                    }
                }
                if !vis[v] {
                    vis[v] = true;
                    total += 1;
                    for x in &languages[v] {
                        cnt[*x as usize] += 1;
                    }
                }
            }
        }
        total - cnt.iter().max().unwrap_or(&0) // 减去最多会的那种语言的人数
    }

}
```
```python
class Solution:
    def minimumTeachings(self, n: int, languages: List[List[int]], friendships: List[List[int]]) -> int:
        learned = list(map(set, languages))

        todo = set()  # 需要学语言的人
        for u, v in friendships:
            # u 和 v 减一，下标从 0 开始
            if learned[u - 1].isdisjoint(learned[v - 1]):  # 无交集
                todo.add(u - 1)
                todo.add(v - 1)

        cnt = [0] * (n + 1)
        for u in todo:
            for x in languages[u]:
                cnt[x] += 1

        return len(todo) - max(cnt)
# 写法二
class Solution:
    def minimumTeachings(self, n: int, languages: List[List[int]], friendships: List[List[int]]) -> int:
        learned = list(map(set, languages))

        total = 0
        vis = [False] * len(languages)
        cnt = [0] * (n + 1)

        def add(u: int) -> None:
            if vis[u]:
                return
            vis[u] = True  # 避免重复统计
            nonlocal total
            total += 1
            for x in languages[u]:
                cnt[x] += 1

        for u, v in friendships:
            # u 和 v 减一，下标从 0 开始
            if learned[u - 1].isdisjoint(learned[v - 1]):  # 无交集
                add(u - 1)
                add(v - 1)

        return total - max(cnt)
```
```go
func minimumTeachings(n int, languages [][]int, friendships [][]int) int {
	m := len(languages)
	learned := make([][]bool, m)
	for i, list := range languages {
		learned[i] = make([]bool, n+1)
		for _, x := range list {
			learned[i][x] = true
		}
	}

	total := 0
	vis := make([]bool, m)
	cnt := make([]int, n+1)
	add := func(u int) {
		if vis[u] {
			return
		}
		vis[u] = true // 避免重复统计
		total++
		for _, x := range languages[u] {
			cnt[x]++
		}
	}

next:
	for _, f := range friendships {
		u, v := f[0]-1, f[1]-1 // 减一，下标从 0 开始
		for _, x := range languages[u] {
			if learned[v][x] { // 两人可以相互沟通，无需学习语言
				continue next
			}
		}
		add(u)
		add(v)
	}

	return total - slices.Max(cnt)
}
```
复杂度分析：
- 时间复杂度：$O(nm + nf)$ ，$n$ 是语言种数，$m$ 是 $languages$ 长度，$f$ 是 $friendships$ 的长度。
- 空间复杂度：$O(nm)$ 。

> 注意：对于C++，可以用 `bitset` 优化，快速判断两个语言列表是否有交集。