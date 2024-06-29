> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

给定一个只包含数字的字符串 `s` ，用以表示一个 IP 地址，返回所有可能从 `s` 获得的 **有效 IP 地址** 。你可以按任何顺序返回答案。

**有效 IP 地址** 正好由四个整数（每个整数位于 0 到 255 之间组成，且不能含有前导 `0`），整数之间用 `'.'` 分隔。

例如："0.1.2.201" 和 "192.168.1.1" 是 **有效** IP 地址，但是 "0.011.255.245"、"192.168.1.312" 和 "192.168@1.1" 是 **无效** IP 地址。

**示例 1：**
```java
输入：s = "25525511135"
输出：["255.255.11.135","255.255.111.35"]
```
**示例 2：**
```java
输入：s = "0000"
输出：["0.0.0.0"]
```
**示例 3：**
```java
输入：s = "1111"
输出：["1.1.1.1"]
```
**示例 4：**
```java
输入：s = "010010"
输出：["0.10.0.10","0.100.1.0"]
```
**示例 5：**
```java
输入：s = "10203040"
输出：["10.20.30.40","102.0.30.40","10.203.0.40"]
```
**提示：**
-   `0 <= s.length <= 3000`
-   `s` 仅由数字组成

注意：本题与主站 93 题相同：[https://leetcode-cn.com/problems/restore-ip-addresses/](https://leetcode-cn.com/problems/restore-ip-addresses/)

---
### 解法1 DFS+回溯
首先明确一下合法的IP地址的条件：
1. 一个IP地址有四个分段；
2. 每个分段表示的数值小于等于 $255$
3. 分段除了表示为 $0$ 的情况以外，其他情况第一个数字不能为 $0$

所以判断分段合法性的代码如下：
```cpp
stoi(sa) <= 255 && (sa == "0" || seg[0] != '0')
```
下面的代码，从当前位置的字符开始，往后依次查看长度为 $1,2,3$ 的字符串，判断它们是否合法，合法则加入IP分段数组，并继续递归。递归回溯后，从IP分段数组中弹出当前分段即可。直到扫描完整个字符串为止，我们判断IP分段数组中是否只有4个分段，是则将分段以 `.` 连接起来，加入答案中。

为了提升算法的效率，我们可以做一下剪枝：
1. $s$ 如果是合法IP地址，则长度最大只能到 $12$ 、最小必须 $\ge 4$ ，题目范围 `0 <= s.length <= 3000` 这么大，可能是来迷惑人的吧……
2. 递归过程中，**如果当前IP分段数组的长度超过了 $4$ ，则直接返回**，说明之前有分段太小了。
```cpp
class Solution {
private:
    vector<string> ans; 
    void dfs(const string &s, int idx, vector<string>& addrs) {
        if (addrs.size() > 4) return; // 分段超过4个则剪枝
        if (idx >= s.size()) {
            if (addrs.size() == 4) 
                ans.push_back(addrs[0] + "." + addrs[1] + "." + addrs[2] + "." + addrs[3]);
            return;
        }
        for (int len = 1; len <= 3; ++len) {
            if (idx + len > s.size()) break;
            string sa = s.substr(idx, len); 
            if (stoi(sa) > 255 || (sa != "0" && sa[0] == '0')) break;  
            addrs.push_back(sa);
            dfs(s, idx + len, addrs);
            addrs.pop_back();
        }
    }
public:
    vector<string> restoreIpAddresses(string s) {
        if (s.size() < 4 || s.size() > 12) return {};
        vector<string> temp;
        dfs(s, 0, temp);
        return ans;
    }
};
```
复杂度分析：我们用 $\text{SEG\_COUNT} = 4$ 表示IP地址的段数。
- 时间复杂度：$O(3^\text{SEG\_COUNT} \times n)$ 。由于IP地址的每一段的位数不会超过 $3$ ，因此在递归的每一层，我们最多只会深入到下一层的 $3$ 种情况。由于 $\text{SEG\_COUNT} = 4$ ，对应着递归的最大层数，所以递归本身的时间复杂度为 $O(3^\text{SEG\_COUNT})$ 。如果我们复原出了一种满足题目要求的IP地址，那么需要 $O(|s|) = O(n)$ 的时间将其加入答案数组中，因此总时间复杂度为 $O(3^\text{SEG\_COUNT} \times n)$ 。
- 空间复杂度：$O(n)$ 。$n$ 为 $s$ 的长度，但不超过合法IP地址的最大长度。这里不考虑存储答案数组的空间。

---
### 解法2 四重循环暴力求解
写的很爽，代码很优雅：
```java
class Solution { 
    public List<String> restoreIpAddresses(String s) {
        List<String> ret = new ArrayList<>();
        StringBuilder ip = new StringBuilder();
        for (int a = 1; a < 4; ++a) {
            for (int b = 1; b < 4; ++b) {
                for (int c = 1; c < 4; ++c) {
                    for (int d = 1; d < 4; ++d) {
                        if (a + b + c + d == s.length()) {
                            int n1 = Integer.parseInt(s.substring(0, a)); // "010" -> 10(int)
                            int n2 = Integer.parseInt(s.substring(a, a + b));
                            int n3 = Integer.parseInt(s.substring(a + b, a + b + c));
                            int n4 = Integer.parseInt(s.substring(a + b + c));
                            if (n1 <= 255 && n2 <= 255 && n3 <= 255 && n4 <= 255) {
                                ip.append(n1).append('.').append(n2).append('.').append(n3).append('.').append(n4);
                                if (ip.length() == s.length() + 3) ret.add(ip.toString()); // 有前导零的会被过滤掉
                                ip.delete(0, ip.length());
                            }
                        }
                    }
                }
            }
        }
        return ret;
    }
}
```
复杂度分析：
- 时间复杂度：$O(3^4)$
- 空间复杂度：$O(n)$