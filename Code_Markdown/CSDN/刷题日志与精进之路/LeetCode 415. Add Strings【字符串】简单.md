> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

给定两个字符串形式的非负整数 `num1` 和`num2` ，计算它们的和并同样以字符串形式返回。

你不能使用任何內建的用于处理大整数的库（比如 `BigInteger`）， 也不能直接将输入的字符串转换为整数形式。

**示例 1：**
```java
输入：num1 = "11", num2 = "123"
输出："134"
```
**示例 2：**
```java
输入：num1 = "456", num2 = "77"
输出："533"
```
**示例 3：**
```java
输入：num1 = "0", num2 = "0"
输出："0"
```
**提示：**
- `1 <= num1.length, num2.length <= 10^4`
- `num1` 和`num2` 都只包含数字 `0-9`
- `num1` 和`num2` 都不包含任何前导零

---
### 解法 字符串
本题只需要对两个大整数模拟「竖式加法」的过程。竖式加法就是平常学习生活中用的对两个整数相加的方法，回想一下纸上对两个整数相加的操作，是不是如下图将相同数位对齐，从低到高逐位相加，如果当前位和超过 $10$ ，则向高位进一位？因此只要将这个过程用代码写出来即可。
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202307172245458.png)
具体实现也不复杂，我们定义两个指针 $i$ 和 $j$ 分别指向 $\textit{num}_1$ 和 $\textit{num}_2$ 的末尾，即最低位，同时定义一个变量 $\textit{add}$ 维护当前是否有进位，然后从末尾到开头逐位相加即可。可能会想两个数字位数不同怎么处理，这里统一**在指针当前下标处于负数时返回 $0$** ，等价于==对「位数较短的数字」进行了补零操作==，这样就可以除去两个数字位数不同情况的处理，具体可以看下面的代码。
```cpp
class Solution {
public:
    string addStrings(string num1, string num2) {
        string ans;
        int i = num1.size() - 1, j = num2.size() - 1, c = 0;
        while (i >= 0 || j >= 0 || c != 0) {
            if (i >= 0) c += (num1[i--] - '0');
            if (j >= 0) c += (num2[j--] - '0'); 
            ans += char(c % 10 + '0'); 
            c /= 10;
        }  
        reverse(ans.begin(), ans.end());
        return ans;
    }
};
```

```java
class Solution {
    public String addStrings(String num1, String num2) {
        int c = 0;
        int i = num1.length() - 1, j = num2.length() - 1;
        var sb = new StringBuilder();
        while (i >= 0 || j >= 0 || c != 0) {
            c += i >= 0 ? (num1.charAt(i--) - '0') : 0;
            c += j >= 0 ? (num2.charAt(j--) - '0') : 0;
            sb.append(c % 10);
            c /= 10;
        }
        return sb.reverse().toString();
    }
}
```
复杂度分析：
- 时间复杂度：$O(\max(\textit{len}_1,\textit{len}_2))$ ，其中 $\textit{len}_1=\textit{num}_1.\text{length}$ ，$\textit{len}_2=\textit{num}_2.\text{length}$ 。竖式加法的次数取决于较大数的位数。
- 空间复杂度：$O(1)$ 。除答案外只需要常数空间存放若干变量。在Java解法中使用到了 $StringBuilder$ ，故Java解法的空间复杂度为 $O(n)$ 。

