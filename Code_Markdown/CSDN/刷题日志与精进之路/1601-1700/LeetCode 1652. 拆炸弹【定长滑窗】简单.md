> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

你有一个炸弹需要拆除，时间紧迫！你的情报员会给你一个长度为 `n` 的 **循环** 数组 `code` 以及一个密钥 `k` 。

为了获得正确的密码，你需要替换掉每一个数字。所有数字会 **同时** 被替换。
- 如果 `k > 0` ，将第 `i` 个数字用 **接下来** `k` 个数字之和替换。
- 如果 `k < 0` ，将第 `i` 个数字用 **之前** `k` 个数字之和替换。
- 如果 `k == 0` ，将第 `i` 个数字用 `0` 替换。

由于 `code` 是循环的， `code[n-1]` 下一个元素是 `code[0]` ，且 `code[0]` 前一个元素是 `code[n-1]` 。

给你 **循环** 数组 `code` 和整数密钥 `k` ，请你返回解密后的结果来拆除炸弹！

**示例 1：**
```java
输入：code = [5,7,1,4], k = 3
输出：[12,10,16,13]
解释：每个数字都被接下来 3 个数字之和替换。解密后的密码为 [7+1+4, 1+4+5, 4+5+7, 5+7+1]。注意到数组是循环连接的。
```
**示例 2：**
```java
输入：code = [1,2,3,4], k = 0
输出：[0,0,0,0]
解释：当 k 为 0 时，所有数字都被 0 替换。
```
**示例 3：**
```java
输入：code = [2,4,9,3], k = -2
输出：[12,5,6,13]
解释：解密后的密码为 [3+9, 2+3, 4+2, 9+4] 。注意到数组是循环连接的。如果 k 是负数，那么和为 **之前** 的数字。
```
**提示：**
- `n == code.length`
- `1 <= n <= 100`
- `1 <= code[i] <= 100`
- `-(n - 1) <= k <= n - 1`

---
### 解法 统一两种情况+定长滑窗
如果 $k>0$ ，例如 $\textit{code}=[3,1,4,1,5,9],\ k=3$ ：
- 计算 $ans[0]$ ，即子数组 $[1,4,1]$ 的元素和 $1+4+1=6$ 。
- 计算 $\textit{ans}[1]$ ，即子数组 $[4,1,5]$ 的元素和，我们可以在 $[1,4,1]$ 的基础上，增加 $\textit{code}[4]=5$ ，减少 $\textit{code}[1]=1$ ，得到 $6+5-1=10$ 。
- 计算 $\textit{ans}[2]$ ，即子数组 $[1,5,9]$ 的元素和，我们可以在 $[4,1,5]$ 的基础上，增加 $\textit{code}[5]=9$ ，减少 $\textit{code}[2]=4$ ，得到 $10+9-4=15$ 。
- 计算 $\textit{ans}[3]$ ，即子数组 $[5,9,3]$ 的元素和，我们可以在 $[1,5,9]$ 的基础上，增加 $\textit{code}[6\bmod 6]=\textit{code}[0] = 3$ ，减少 $\textit{code}[3]=1$ ，得到 $15+3-1=17$ 。
- 计算 $\textit{ans}[4]$ ，即子数组 $[9,3,1]$ 的元素和，我们可以在 $[5,9,3]$ 的基础上，增加 $\textit{code}[7\bmod 6]=\textit{code}[1] = 1$ ，减少 $\textit{code}[4]=5$ ，得到 $17+1-5=13$ 。
- 计算 $\textit{ans}[5]$ ，即子数组 $[3,1,4]$ 的元素和，我们可以在 $[9,3,1]$ 的基础上，增加 $\textit{code}[8\bmod 6]=\textit{code}[2] = 4$ ，减少 $\textit{code}[5]=9$ ，得到 $13+4-9=8$ 。

对于 $k < 0$ 的情况也同理。注意无论 $k>0$ 还是 $k<0$ ，窗口都是在向右移动的，所以确定好第一个窗口的位置，就可以把 $k>0$ 和 $k<0$ 两种情况合并起来了。
- $k>0$ ，第一个窗口的的下标范围为 $[1,k+1)$ 。
- $k<0$ ，第一个窗口的的下标范围为 $[n-|k|,n)$ 。

在窗口向右滑动时，设移入窗口的元素下标为 $r\bmod n$ ，则移出窗口的元素下标为 $(r-|k|)\bmod n$ 。

代码实现时，$k=0$ 的特判可以省略。
```python
class Solution:
    def decrypt(self, code: List[int], k: int) -> List[int]:
        n = len(code)
        ans = [0] * n
        r = k + 1 if k > 0 else n
        k = abs(k)
        s = sum(code[r - k: r]) # ans[0], [r-k, r)
        for i in range(n):
            ans[i] = s
            s += code[r % n] - code[(r - k) % n] # +移入,-移出
            r += 1
        return ans
```

```java
class Solution {
    public int[] decrypt(int[] code, int k) {
        int n = code.length;
        int[] ans = new int[n];
        int r = k > 0 ? k + 1 : n;
        k = Math.abs(k);
        int s = 0;
        for (int i = r - k; i < r; ++i) s += code[i]; // ans[0]
        for (int i = 0; i < n; ++i) {
            ans[i] = s;
            s += code[r % n] - code[(r - k) % n];
            ++r;
        }
        return ans;
    }
}
```

```cpp
class Solution {
public:
    vector<int> decrypt(vector<int>& code, int k) {
        int n = code.size();
        vector<int> ans(n);
        int r = k > 0 ? k + 1 : n;
        k = abs(k);
        int s = reduce(code.begin() + r - k, code.begin() + r); // ans[0]
        for (int i = 0; i < n; ++i) {
            ans[i] = s;
            s += code[r % n] - code[(r - k) % n];
            ++r;
        }
        return ans;
    }
};
```
复杂度分析：
- 时间复杂度：$\mathcal{O}(n)$ ，其中 $n$ 为 $\textit{code}$ 的长度。
- 空间复杂度：$\mathcal{O}(1)$ 。返回值不计入。Python 忽略切片的空间。

