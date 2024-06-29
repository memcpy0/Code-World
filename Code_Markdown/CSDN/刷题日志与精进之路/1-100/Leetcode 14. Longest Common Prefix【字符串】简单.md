
> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202310091343500.png)
编写一个函数来查找字符串数组中的最长公共前缀。

如果不存在公共前缀，返回空字符串 `""`。

**示例 1：**
```java
输入：strs = ["flower","flow","flight"]
输出："fl"
```
**示例 2：**
```java
输入：strs = ["dog","racecar","car"]
输出：""
解释：输入不存在公共前缀。
```
**提示：**
- `1 <= strs.length <= 200`
- `0 <= strs[i].length <= 200`
- `strs[i]` 仅由小写英文字母组成

---
用 LCP(S1…Sn)\textit{LCP}(S_1 \ldots S_n)LCP(S 
1
​
 …S 
n
​
 ) 表示字符串 S1…SnS_1 \ldots S_nS 
1
​
 …S 
n
​
  的最长公共前缀。

可以得到以下结论：

LCP(S1…Sn)=LCP(LCP(LCP(S1,S2),S3),…Sn) \textit{LCP}(S_1 \ldots S_n) = \textit{LCP}(\textit{LCP}(\textit{LCP}(S_1, S_2),S_3),\ldots S_n)
LCP(S 
1
​
 …S 
n
​
 )=LCP(LCP(LCP(S 
1
​
 ,S 
2
​
 ),S 
3
​
 ),…S 
n
​
 )
基于该结论，可以得到一种查找字符串数组中的最长公共前缀的简单方法。依次遍历字符串数组中的每个字符串，对于每个遍历到的字符串，更新最长公共前缀，当遍历完所有的字符串以后，即可得到字符串数组中的最长公共前缀。
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202307022156029.png)



如果在尚未遍历完所有的字符串时，最长公共前缀已经是空串，则最长公共前缀一定是空串，因此不需要继续遍历剩下的字符串，直接返回空串即可。
 

方法二：水平比较，其实就是第一个字符串和第二个字符串比较得出最长公共前缀flow，然后结果flow和第3个字符串比较，得到最长公共前缀fl。也就是全部字符串的LCP=max(...(...max(max(strs[0], strs[1]), strs[2])...)...)。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20191122195416933.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
执行用时 :4 ms, 在所有 cpp 提交中击败了95.58% 的用户
内存消耗 :8.7 MB, 在所有 cpp 提交中击败了89.62%的用户
```cpp
class Solution {
public:
    string longestCommonPrefix(vector<string>& strs) {
        if (strs.empty()) return string();
        string ans = strs[0]; // 先设第一个字符串为LCP
        for (int i = 1; i < strs.size(); i++) {
            int minLen = min(ans.size(), strs[i].size()), j = 0;
            for (; j < minLen; j++) {
                if (ans[j] != strs[i][j]) break;
            }
            ans = strs[i].substr(0, j);
        } 
        return ans;
    }
};
```
复杂度分析

时间复杂度：O(mn)O(mn)O(mn)，其中 mmm 是字符串数组中的字符串的平均长度，nnn 是字符串的数量。最坏情况下，字符串数组中的每个字符串的每个字符都会被比较一次。

空间复杂度：O(1)O(1)O(1)。使用的额外空间复杂度为常数。

### 纵向扫描
方法一：垂直比较。先求出最短的字符串长度，然后以这个长度为限，一列一列的比较，O(minLen * n)的时间复杂度。

纵向扫描时，从前往后遍历所有字符串的每一列，比较相同列上的字符是否相同，如果相同则继续对下一列进行比较，如果不相同则当前列不再属于公共前缀，当前列之前的部分为最长公共前缀。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20191122194141540.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
```cpp
class Solution {
public:
    string longestCommonPrefix(vector<string>& strs) {
        if (strs.empty()) return "";
        string ans = "";
        int minLen = 0x7fffffff;
        for (auto &s : strs)  
            if (s.size() < minLen) 
                minLen = s.size(); 
        for (int i = 0; i < minLen; i++) { 
            for (auto &s : strs) {
                if (s[i] != strs[0][i]) return ans;  // 垂直比较的字符
            } 
            ans += strs[0][i];
        }
        return ans;
    }
};
```

复杂度分析

时间复杂度：O(mn)O(mn)O(mn)，其中 mmm 是字符串数组中的字符串的平均长度，nnn 是字符串的数量。最坏情况下，字符串数组中的每个字符串的每个字符都会被比较一次。

空间复杂度：O(1)O(1)O(1)。使用的额外空间复杂度为常数。
 

---
方法三：分治
注意到 LCP\textit{LCP}LCP 的计算满足结合律，有以下结论：

LCP(S1…Sn)=LCP(LCP(S1…Sk),LCP(Sk+1…Sn)) \textit{LCP}(S_1 \ldots S_n) = \textit{LCP}(\textit{LCP}(S_1 \ldots S_k), \textit{LCP} (S_{k+1} \ldots S_n))
LCP(S 
1
​
 …S 
n
​
 )=LCP(LCP(S 
1
​
 …S 
k
​
 ),LCP(S 
k+1
​
 …S 
n
​
 ))
其中 LCP(S1…Sn)\textit{LCP}(S_1 \ldots S_n)LCP(S 
1
​
 …S 
n
​
 ) 是字符串 S1…SnS_1 \ldots S_nS 
1
​
 …S 
n
​
  的最长公共前缀，1<k<n1 < k < n1<k<n。

基于上述结论，可以使用分治法得到字符串数组中的最长公共前缀。对于问题 LCP(Si⋯Sj)\textit{LCP}(S_i\cdots S_j)LCP(S 
i
​
 ⋯S 
j
​
 )，可以分解成两个子问题 LCP(Si…Smid)\textit{LCP}(S_i \ldots S_{mid})LCP(S 
i
​
 …S 
mid
​
 ) 与 LCP(Smid+1…Sj)\textit{LCP}(S_{mid+1} \ldots S_j)LCP(S 
mid+1
​
 …S 
j
​
 )，其中 mid=i+j2mid=\frac{i+j}{2}mid= 
2
i+j
​
 。对两个子问题分别求解，然后对两个子问题的解计算最长公共前缀，即为原问题的解。



Java
C++
Python3
Golang
class Solution {
public:
    string longestCommonPrefix(vector<string>& strs) {
        if (!strs.size()) {
            return "";
        }
        else {
            return longestCommonPrefix(strs, 0, strs.size() - 1);
        }
    }

    string longestCommonPrefix(const vector<string>& strs, int start, int end) {
        if (start == end) {
            return strs[start];
        }
        else {
            int mid = (start + end) / 2;
            string lcpLeft = longestCommonPrefix(strs, start, mid);
            string lcpRight = longestCommonPrefix(strs, mid + 1, end);
            return commonPrefix(lcpLeft, lcpRight);
        }
    }

    string commonPrefix(const string& lcpLeft, const string& lcpRight) {
        int minLength = min(lcpLeft.size(), lcpRight.size());
        for (int i = 0; i < minLength; ++i) {
            if (lcpLeft[i] != lcpRight[i]) {
                return lcpLeft.substr(0, i);
            }
        }
        return lcpLeft.substr(0, minLength);
    }
};
复杂度分析

时间复杂度：O(mn)O(mn)O(mn)，其中 mmm 是字符串数组中的字符串的平均长度，nnn 是字符串的数量。时间复杂度的递推式是 T(n)=2⋅T(n2)+O(m)T(n)=2 \cdot T(\frac{n}{2})+O(m)T(n)=2⋅T( 
2
n
​
 )+O(m)，通过计算可得 T(n)=O(mn)T(n)=O(mn)T(n)=O(mn)。

空间复杂度：O(mlog⁡n)O(m \log n)O(mlogn)，其中 mmm 是字符串数组中的字符串的平均长度，nnn 是字符串的数量。空间复杂度主要取决于递归调用的层数，层数最大为 log⁡n\log nlogn，每层需要 mmm 的空间存储返回结果。

方法四：二分查找
显然，最长公共前缀的长度不会超过字符串数组中的最短字符串的长度。用 minLength\textit{minLength}minLength 表示字符串数组中的最短字符串的长度，则可以在 [0,minLength][0,\textit{minLength}][0,minLength] 的范围内通过二分查找得到最长公共前缀的长度。每次取查找范围的中间值 mid\textit{mid}mid，判断每个字符串的长度为 mid\textit{mid}mid 的前缀是否相同，如果相同则最长公共前缀的长度一定大于或等于 mid\textit{mid}mid，如果不相同则最长公共前缀的长度一定小于 mid\textit{mid}mid，通过上述方式将查找范围缩小一半，直到得到最长公共前缀的长度。

class Solution {
public:
    string longestCommonPrefix(vector<string>& strs) {
        if (!strs.size()) {
            return "";
        }
        int minLength = min_element(strs.begin(), strs.end(), [](const string& s, const string& t) {return s.size() < t.size();})->size();
        int low = 0, high = minLength;
        while (low < high) {
            int mid = (high - low + 1) / 2 + low;
            if (isCommonPrefix(strs, mid)) {
                low = mid;
            }
            else {
                high = mid - 1;
            }
        }
        return strs[0].substr(0, low);
    }

    bool isCommonPrefix(const vector<string>& strs, int length) {
        string str0 = strs[0].substr(0, length);
        int count = strs.size();
        for (int i = 1; i < count; ++i) {
            string str = strs[i];
            for (int j = 0; j < length; ++j) {
                if (str0[j] != str[j]) {
                    return false;
                }
            }
        }
        return true;
    }
};
复杂度分析

时间复杂度：O(mnlog⁡m)O(mn \log m)O(mnlogm)，其中 mmm 是字符串数组中的字符串的最小长度，nnn 是字符串的数量。二分查找的迭代执行次数是 O(log⁡m)O(\log m)O(logm)，每次迭代最多需要比较 mnmnmn 个字符，因此总时间复杂度是 O(mnlog⁡m)O(mn \log m)O(mnlogm)。

空间复杂度：O(1)O(1)O(1)。使用的额外空间复杂度为常数。

 