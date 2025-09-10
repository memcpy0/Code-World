> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

卡车有两个油箱。给你两个整数，`mainTank` 表示主油箱中的燃料（以升为单位），`additionalTank` 表示副油箱中的燃料（以升为单位）。

该卡车每耗费 `1` 升燃料都可以行驶 `10` km。每当主油箱使用了 `5` 升燃料时，如果副油箱至少有 `1` 升燃料，则会将 `1` 升燃料从副油箱转移到主油箱。

返回卡车可以行驶的最大距离。

注意：从副油箱向主油箱注入燃料不是连续行为。这一事件会在每消耗 `5` 升燃料时突然且立即发生。

**示例 1：**
```java
输入：mainTank = 5, additionalTank = 10
输出：60
解释：
在用掉 5 升燃料后，主油箱中燃料还剩下 (5 - 5 + 1) = 1 升，行驶距离为 50km 。
在用掉剩下的 1 升燃料后，没有新的燃料注入到主油箱中，主油箱变为空。
总行驶距离为 60km 。
```
**示例 2：**
```java
输入：mainTank = 1, additionalTank = 2
输出：10
解释：
在用掉 1 升燃料后，主油箱变为空。
总行驶距离为 10km 。
```
**提示：**
- `1 <= mainTank, additionalTank <= 100`

---
### 相似题目
- [1518. 换水问题](https://leetcode.cn/problems/water-bottles/)
- [3100. 换水问题 II](https://leetcode.cn/problems/water-bottles-ii/)
### 解法1 模拟
按照每次消耗 $5$ 升燃料去模拟，直到 $\textit{mainTank}< 5$ ，退出循环。

别忘了最后还可以跑 $\textit{mainTank}\cdot 10$ 。
```cpp
class Solution {
public: 
    int distanceTraveled(int mainTank, int additionalTank) {
        int ans = 0;
        while (mainTank >= 5) {
            mainTank -= 5;
            if (additionalTank > 0) {
                --additionalTank;
                ++mainTank;
            }
            ans += 50;
        }
        return ans + mainTank * 10;
    }
};
```

```java
class Solution {
    public int distanceTraveled(int mainTank, int additionalTank) {
        int ans = 0;
        while (mainTank >= 5) {
            mainTank -= 5;
            ans += 50;
            if (additionalTank > 0) {
                additionalTank--;
                mainTank++;
            }
        }
        return ans + mainTank * 10;
    }
}
```

```python
class Solution:
    def distanceTraveled(self, mainTank: int, additionalTank: int) -> int:
        ans = 0
        while mainTank >= 5:
            mainTank -= 5
            ans += 50
            if additionalTank:
                additionalTank -= 1
                mainTank += 1
        return ans + mainTank * 10
```
复杂度分析：
- 时间复杂度：$\mathcal{O}(\textit{mainTank})$ 。
- 空间复杂度：$\mathcal{O}(1)$ 。

---
### 解法2 快速模拟：除法
如果 $\textit{mainTank}$ 有 $10^9$ ，那么方法一会超时。有没有更快的做法呢？把方法一的减法改成除法，统计 $-= 5$ 发生了 $t = \left\lfloor\dfrac{mainTank}{5}\right\rfloor$ 次。然后再一次性地把 $t$ 升燃料注入主油箱。注意 $t$ 不能超过 $\textit{additionalTank}$ 。

```java
class Solution {
    public int distanceTraveled(int mainTank, int additionalTank) {
        int ans = 0;
        while (mainTank >= 5) {
            int t = mainTank / 5;
            ans += t * 50;
            mainTank %= 5;
            t = Math.min(t, additionalTank);
            additionalTank -= t;
            mainTank += t;
        }
        return ans + mainTank * 10;
    }
}
```

```python
class Solution:
    def distanceTraveled(self, mainTank: int, additionalTank: int) -> int:
        ans = 0
        while mainTank >= 5:
            t = mainTank // 5
            ans += t * 50
            mainTank %= 5
            t = min(t, additionalTank)
            additionalTank -= t
            mainTank += t
        return ans + mainTank * 10
```

```cpp
class Solution {
public: 
    int distanceTraveled(int mainTank, int additionalTank) {
        int ans = 0;
        while (mainTank >= 5) {
            int t = mainTank / 5;
            mainTank %= 5;
            ans += t * 50;
            t = min(t, additionalTank);
            additionalTank -= t;
            mainTank += t;
        }
        return ans + mainTank * 10;
    }
};
```
复杂度分析：
- 时间复杂度：$\mathcal{O}(\log\textit{mainTank})$ 。每次循环 $\textit{mainTank}$ 至少减为原来的 $\dfrac{1}{4}$​ 。
- 空间复杂度：$\mathcal{O}(1)$ 。

---
### 解法3 数学
考虑从副油箱能得到多少燃料。

主油箱消耗 $5$ 升燃料可以从副油箱得到 $1$ 升，这 $1$ 升又加回主油箱，相当于**从主油箱的初始燃料 $\textit{mainTank}$ 中每消耗 $4$ 升，就可以从副油箱中得到一升**。所以看上去可以从副油箱得到
$$\min\left(\left\lfloor\dfrac{\textit{mainTank}}{4}\right\rfloor, \textit{additionalTank}\right)$$ 
升燃料。但这是不对的，例如 $\textit{mainTank}=8$ ，只能从副油箱得到 $1$ 升燃料。因为 8−5+1=48-5+1=48−5+1=4，此时无法再从副油箱中得到燃料。

将上式分子减一就可以修复这个问题，也就是
$$\min\left(\left\lfloor\dfrac{\textit{mainTank}-1}{4}\right\rfloor, \textit{additionalTank}\right)$$
对于 $\textit{mainTank}=8$ 只能算出 $1$ ，而对于 $\textit{mainTank}=9$ ，则可以恰好从副油箱得到 $2$ 升燃料（如果有的话）。

所以答案为
$$\left(\textit{mainTank} + \min\left(\left\lfloor\dfrac{\textit{mainTank}-1}{4}\right\rfloor, \textit{additionalTank}\right)\right)\cdot 10$$

```python
class Solution:
    def distanceTraveled(self, mainTank: int, additionalTank: int) -> int:
        return (mainTank + min((mainTank - 1) // 4, additionalTank)) * 10;
```

```cpp
class Solution {
public: 
    int distanceTraveled(int mainTank, int additionalTank) {
        return (mainTank + min((mainTank - 1) / 4, additionalTank)) * 10;
    }
};
```

```java
class Solution {
    public int distanceTraveled(int mainTank, int additionalTank) {
        return (mainTank + Math.min((mainTank - 1) / 4, additionalTank)) * 10;
    }
}
```
复杂度分析：
- 时间复杂度：$\mathcal{O}(1)$ 。
- 空间复杂度：$\mathcal{O}(1)$ 。