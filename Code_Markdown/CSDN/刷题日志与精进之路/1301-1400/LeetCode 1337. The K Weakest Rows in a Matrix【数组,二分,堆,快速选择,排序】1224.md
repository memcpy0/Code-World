> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的[仓库](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

给你一个大小为 `m * n` 的矩阵 `mat`，矩阵由若干军人和平民组成，分别用 1 和 0 表示。

请你返回矩阵中战斗力最弱的 `k` 行的索引，按从最弱到最强排序。

如果第 _**i**_ 行的军人数量少于第 _**j**_ 行，或者两行军人数量相同但 _**i**_ 小于 _**j**_，那么我们认为第 _**i**_ 行的战斗力比第 _**j**_ 行弱。

军人 **总是** 排在一行中的靠前位置，也就是说 1 总是出现在 0 之前。

**示例 1：**
```js
输入：mat = 
[[1,1,0,0,0],
 [1,1,1,1,0],
 [1,0,0,0,0],
 [1,1,0,0,0],
 [1,1,1,1,1]], 
k = 3
输出：[2,0,3]
解释：
每行中的军人数目：
行 0 -> 2 
行 1 -> 4 
行 2 -> 1 
行 3 -> 2 
行 4 -> 5 
从最弱到最强对这些行排序后得到 [2,0,3,1,4]
```
**示例 2：**
```js
输入：mat = 
[[1,0,0,0],
 [1,1,1,1],
 [1,0,0,0],
 [1,0,0,0]], 
k = 2
输出：[0,2]
解释： 
每行中的军人数目：
行 0 -> 1 
行 1 -> 4 
行 2 -> 1 
行 3 -> 1 
从最弱到最强对这些行排序后得到 [0,2,3,1]
```
**提示：**
- `m == mat.length`
- `n == mat[i].length`
- `2 <= n, m <= 100`
- `1 <= k <= m`
- `matrix[i][j]` 不是 $0$ 就是 $1$

---
由于本题中的矩阵行数 $m$ 和列数 $n$ 均不超过 $100$ ，数据规模较小，因此我们可以设计出一些时间复杂度较高的方法，例如直接对整个矩阵进行一次遍历，计算出每一行的战斗力，再进行排序并返回最弱的 $k$  行的索引。
### 解法 二分查找 + 堆
题目描述中有一条重要的保证：**军人总是排在一行中的靠前位置**，也就是说 $1$  总是出现在 $0$ 之前。因此，我们可以通过二分查找的方法，找出一行中最后的那个 $1$ 的位置。如果其位置为 $pos$ ，那么这一行 $1$  的个数就为 $pos+1$ 。特别地，如果这一行没有 $1$ ，那么令 $pos=-1$ 。

**当我们得到每一行的战斗力后，我们可以将它们全部放入一个小根堆中**，并不断地取出堆顶的元素 $k$  次，这样我们就得到了最弱的 $k$  行的索引。

需要注意的是，如果我们依次将每一行的战斗力以及索引（因为如果战斗力相同，索引较小的行更弱，所以**我们需要在小根堆中存放战斗力和索引的二元组）放入小根堆中，那么这样做的时间复杂度是 $O(m\log m)$  的**。一种更好的方法是**使用这 $m$ 个战斗力值直接初始化一个小根堆，时间复杂度为 $O(m)$** 。可参考《算法导论》的 6.3 节了解该过程时间复杂度的证明方法。
```cpp
class Solution {
public:
    vector<int> kWeakestRows(vector<vector<int>>& mat, int k) {
        int m = mat.size(), n = mat[0].size();
        vector<pair<int, int>> power;
        for (int i = 0; i < m; ++i) {
            int l = 0, r = n - 1, pos = -1;
            while (l <= r) {
                int mid = (l + r) / 2;
                if (mat[i][mid] == 0) {
                    r = mid - 1;
                }
                else {
                    pos = mid;
                    l = mid + 1;
                }
            }
            power.emplace_back(pos + 1, i);
        }
        priority_queue q(greater<pair<int, int>>(), move(power));
        vector<int> ans;
        for (int i = 0; i < k; ++i) {
            ans.push_back(q.top().second);
            q.pop();
        }
        return ans;
    }
};
```
复杂度分析：
- 时间复杂度：$O(m\log n+k\log m)$ ：我们需要 $O(m\log n)$  的时间对每一行进行二分查找。我们需要 $O(m)$  的时间建立小根堆。我们需要 $O(k\log m)$  的时间从堆中取出 $k$ 个最小的元素。
- 空间复杂度：$O(m)$ ，即为堆需要使用的空间。

---
### 解法 二分查找 + 快速选择
我们也可以通过快速选择算法，**在平均 $O(m)$  的时间内不计顺序地内找出 $k$  个最小的元素**，==再使用排序算法在 $O(k\log k)$  的时间对这 $k$  个最小的元素进行升序排序，就可以得到最终的答案==。参考「剑指 Offer 40. 最小的k个数」题解的方法三或者「215. 数组中的第K个最大元素」的题解方法一了解快速选择算法：
```cpp
template<typename T>
class Helper {
    static int partition(vector<T>& nums, int l, int r) {
        T pivot = nums[r];
        int i = l - 1;
        for (int j = l; j <= r - 1; ++j) {
            if (nums[j] <= pivot) {
                i = i + 1;
                swap(nums[i], nums[j]);
            }
        }
        swap(nums[i + 1], nums[r]);
        return i + 1;
    }

    // 基于随机的划分
    static int randomized_partition(vector<T>& nums, int l, int r) {
        int i = rand() % (r - l + 1) + l;
        swap(nums[r], nums[i]);
        return partition(nums, l, r);
    }

    static void randomized_selected(vector<T>& arr, int l, int r, int k) {
        if (l >= r) {
            return;
        }
        int pos = randomized_partition(arr, l, r);
        int num = pos - l + 1;
        if (k == num) {
            return;
        } else if (k < num) {
            randomized_selected(arr, l, pos - 1, k);
        } else {
            randomized_selected(arr, pos + 1, r, k - num);
        }
    }

public:
    static vector<T> getLeastNumbers(vector<T>& arr, int k) {
        srand((unsigned)time(NULL));
        randomized_selected(arr, 0, (int)arr.size() - 1, k);
        vector<T> vec;
        for (int i = 0; i < k; ++i) {
            vec.push_back(arr[i]);
        }
        return vec;
    }
};

class Solution {
public:
    vector<int> kWeakestRows(vector<vector<int>>& mat, int k) {
        int m = mat.size(), n = mat[0].size();
        vector<pair<int, int>> power;
        for (int i = 0; i < m; ++i) {
            int l = 0, r = n - 1, pos = -1;
            while (l <= r) {
                int mid = (l + r) / 2;
                if (mat[i][mid] == 0) {
                    r = mid - 1;
                }
                else {
                    pos = mid;
                    l = mid + 1;
                }
            }
            power.emplace_back(pos + 1, i);
        }

        vector<pair<int, int>> minimum = Helper<pair<int, int>>::getLeastNumbers(power, k);
        sort(minimum.begin(), minimum.begin() + k);
        vector<int> ans;
        for (int i = 0; i < k; ++i) {
            ans.push_back(minimum[i].second);
        }
        return ans;
    }
};
```
复杂度分析
- 时间复杂度：$O(m\log n + k\log k)$ ：我们需要 $O(m\log n)$  的时间对每一行进行二分查找。我们需要 $O(m)$  的时间完成快速选择算法。我们需要 $O(k\log k)$  的时间对这 $k$  个最小的元素进行升序排序。
- 空间复杂度：$O(m)$ ，即为快速选择算法中的数组需要使用的空间。