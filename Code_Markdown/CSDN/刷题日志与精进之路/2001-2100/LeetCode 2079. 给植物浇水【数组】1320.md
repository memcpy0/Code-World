> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

你打算用一个水罐给花园里的 `n` 株植物浇水。植物排成一行，从左到右进行标记，编号从 `0` 到 `n - 1` 。其中，第 `i` 株植物的位置是 `x = i` 。`x = -1` 处有一条河，你可以在那里重新灌满你的水罐。

每一株植物都需要浇特定量的水。你将会按下面描述的方式完成浇水：
- 按从左到右的顺序给植物浇水。
- 在给当前植物浇完水之后，如果你没有足够的水 **完全** 浇灌下一株植物，那么你就需要返回河边重新装满水罐。
- 你 **不能** 提前重新灌满水罐。

最初，你在河边（也就是，`x = -1`），在 x 轴上每移动 **一个单位** 都需要 **一步** 。

给你一个下标从 **0** 开始的整数数组 `plants` ，数组由 `n` 个整数组成。其中，`plants[i]` 为第 `i` 株植物需要的水量。另有一个整数 `capacity` 表示水罐的容量，返回浇灌所有植物需要的 **步数** 。

**示例 1：**
```java
输入：plants = [2,2,3,3], capacity = 5
输出：14
解释：从河边开始，此时水罐是装满的：
- 走到植物 0 (1 步) ，浇水。水罐中还有 3 单位的水。
- 走到植物 1 (1 步) ，浇水。水罐中还有 1 单位的水。
- 由于不能完全浇灌植物 2 ，回到河边取水 (2 步)。
- 走到植物 2 (3 步) ，浇水。水罐中还有 2 单位的水。
- 由于不能完全浇灌植物 3 ，回到河边取水 (3 步)。
- 走到植物 3 (4 步) ，浇水。
需要的步数是 = 1 + 1 + 2 + 3 + 3 + 4 = 14 。
```
**示例 2：**
```java
输入：plants = [1,1,1,4,2,3], capacity = 4
输出：30
解释：从河边开始，此时水罐是装满的：
- 走到植物 0，1，2 (3 步) ，浇水。回到河边取水 (3 步)。
- 走到植物 3 (4 步) ，浇水。回到河边取水 (4 步)。
- 走到植物 4 (5 步) ，浇水。回到河边取水 (5 步)。
- 走到植物 5 (6 步) ，浇水。
需要的步数是 = 3 + 3 + 4 + 4 + 5 + 5 + 6 = 30 。
```
**示例 3：**
```java
输入：plants = [7,7,7,7,7,7,7], capacity = 8
输出：49
解释：每次浇水都需要重新灌满水罐。
需要的步数是 = 1 + 1 + 2 + 2 + 3 + 3 + 4 + 4 + 5 + 5 + 6 + 6 + 7 = 49 。
```
**提示：**
- `n == plants.length`
- `1 <= n <= 1000`
- `1 <= plants[i] <= 10^6`
- `max(plants[i]) <= capacity <= 10^9`

---
### 解法 遍历数组
由于需要浇灌所有植物，至少要走 $n$ 步，故初始化答案 $ans=n$ 。
1. 初始化水罐中的水量 $water=capacity$ 。
2. 从左到右遍历 $plants$ 。如果 $water<plants[i]$ ，那么我们在上一个位置 $x=i−1$ 就可以返回 $x=−1$ 重新灌满水，再返回到 $x=i−1$ ，需要额外走 $2i$ 步，加入答案，把 $water$ 重置为 $\textit{capacity}$ ，然后浇水，把 $water$ 减少 $plants[i]$ 。如果水量足够，就直接浇水。
3. 返回答案。

```cpp
class Solution {
public:
    int wateringPlants(vector<int>& plants, int capacity) {
        int n = plants.size();
        int ans = n;
        for (int i = 0, water = capacity; i < n; ++i) {
            if (water < plants[i]) {
                ans += 2 * i;
                water = capacity;
            }
            water -= plants[i];
        }
        return ans;
    }
};
```

```python
class Solution:
    def wateringPlants(self, plants: List[int], capacity: int) -> int:
        ans = len(plants)
        water = capacity
        for i, need in enumerate(plants):
            if water < need:
                ans += i * 2
                water = capacity
            water -= need
        return ans
```

```java
class Solution {
    public int wateringPlants(int[] plants, int capacity) {
        int n = plants.length;
        int ans = n;
        int water = capacity;
        for (int i = 0; i < n; ++i) {
            if (water < plants[i]) {
                ans += i * 2;
                water = capacity;
            }
            water -= plants[i];
        }
        return ans;
    }
}
```
复杂度分析：
- 时间复杂度：$\mathcal{O}(n)$ ，其中 $n$ 为 $plants$ 的长度。
- 空间复杂度：$\mathcal{O}(1)$ 。

> 思考题：如果额外输入一个 $\textit{position}$ 数组，表示在 $position[i]$ 处你可以重新灌满水，要怎么做？
