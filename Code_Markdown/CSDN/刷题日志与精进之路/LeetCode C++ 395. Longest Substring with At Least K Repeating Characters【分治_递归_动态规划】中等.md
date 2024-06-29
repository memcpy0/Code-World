 Find the length of the longest substring `T` of a given string (consists of lowercase letters only) such that every character in `T` appears no less than `k` times.

**Example 1:** 
```swift
Input:
s = "aaabb", k = 3

Output:
3

The longest substring is "aaa", as 'a' is repeated 3 times.
```

**Example 2:** 
```swift
Input:
s = "ababbc", k = 2

Output:
5


The longest substring is "ababb", as 'a' is repeated 2 times and 'b' is repeated 3 times.
```
题意：找到给定字符串（由小写字符组成）中的最长子串 `T` ， 要求 `T` 中的每一字符出现次数都不少于 `k` 。输出 `T` 的长度。

---
### 解法1 分治
对于一个字符串来说，要求某个子串中的每个字符在子串中最少出现 `k` 次，那么如果某些字母的出现次数在整个字符串中都少于 `k` 次，这些字母就一定不会出现在子串中，且会将整个字符串分割成几段。几段中可能含有最长的符合规则的子串。

由于题意要求的字符出现次数是在子串中，而我们现在得到的是整个字符串的出现次数，与题意不符。为此，我们可以**以每个小段作为整串**来递归检查各个小段。同样，如果某些字母在串中出现的次数小于 `k` ，就会将子串进一步分割下去。

整个算法的具体流程如下，我们先检查一些边界情况：
1. 如果 `k <= 1` ，直接返回整个字符串的长度；
2. 如果字符串 `s` 为空或者长度 `< k` ，则返回零；
3. 然后以整个字符串为目标，计算每一字符的出现次数：
	- 如果所有字符的出现次数都 `>= k` ，则整个字符串符合要求，返回其长度即可；
	- 如果某一字符 `s[i]` 出现次数 `< k` ，则整个字符串不符合要求；于是递归检查 `s[0 : i)` 这一段；而后越过其他不符合要求的字符，到达 `s[j]` ，递归检查 `s[j : ]` 这一段。
	- 比较这两段的返回值，取最大的那个子串长度。

举例如下：
```cpp
Input: "aacbbbdc", k = 2
1. 处理"aacbbbdc", d在串中只出现了一次, 不会存在于目标子串中, 然后递归处理区间 [0, 5], [7, 7]
2. 处理"aacbbb", c在串中只出现了一次, 不会存在于目标子串中, 然后递归处理区间 [0, 1], [3, 5]
3. 处理"aa", 满足所有字符出现次数>=2, return 2
4. 处理"bbb", 满足所有字符出现次数>=2, return 3
5. return max(2, 3) = 3
6. 处理"c", 长度小于k, 且c只出现了一次, 不满足题意, 不继续递归, return 0
7. return max(3, 0) = 3 
```
具体代码如下：
```cpp
class Solution {
public:
    int longestSubstring(string s, int k) {
        if (k <= 1) return s.size(); //最长子串T就是S, 其中每一字符的出现次数都>=k
        if (s.empty() || s.size() < k) return 0; 
        vector<int> hash(128, 0);
        for (const char &c : s) ++hash[c];
        int i = 0;
        while (i < s.size() && hash[s[i]] >= k) ++i; //判断每个字符的出现次数是否>=k
        if (i == s.size()) return s.size(); //整个字符串中每个字符的出现次数都>=k

        int l = longestSubstring(s.substr(0, i), k);
        while (i < s.size() && hash[s[i]] < k) ++i;
        int r = longestSubstring(s.substr(i), k);

        return max(l, r);
    }
};
```
效率很高：
```cpp
执行用时：0 ms, 在所有 C++ 提交中击败了100.00% 的用户
内存消耗：7 MB, 在所有 C++ 提交中击败了13.59% 的用户
```
---
### 解法2 枚举+二进制压缩
在解题时，**依次判断每个子串是否满足条件**。使用哈希表或者数组存储时，频繁的判断会导致超时（尤其是还用了函数调用时）。一个做法是使用一个数来存储判断结果，初始化为 `0` ，每一位代表一个字母——`0` 代表这个字母出现次数大于 `k` ，`1` 则反之。最后，如果这个数为 `0` ，那么则符合条件，否则不符合。
```cpp
class Solution {
public:
    int longestSubstring(string s, int k) {
        int ans = 0, n = s.size();
        for (int i = 0; i <= n - k; ++i) {
            int appears[26] = {0};
            int point = 0; //用二进制数记录每一位(每个字母)是否满足条件
            int maxTail = i;
            for (int j = i; j < n; ++j) {
                int c = s[j] - 'a';
                ++appears[c]; //出现次数加1
                if (appears[c] < k) //出现次数小于k
                    point = point | (1 << c); //对第c位设置为1
                else 
                    point = point & (~(1 << c)); //对第c位设置为0
                if (point == 0) { //如果point==0,则代表子串s[i:j]中所有的字母都符合规则
                    ans = max(ans, j - i + 1);
                    maxTail = j;
                }
            }
            //i = maxTail;
            //不加这句的话也不会超时
        }
        return ans;
    }
    
};
```
效率如下，比较低：
```cpp
执行用时：152 ms, 在所有 C++ 提交中击败了5.64% 的用户
内存消耗：6.6 MB, 在所有 C++ 提交中击败了71.78% 的用户
```
如果取消 `i = maxTail` 的注释，效率如下：
```cpp
执行用时：100 ms, 在所有 C++ 提交中击败了6.31% 的用户
内存消耗：6.6 MB, 在所有 C++ 提交中击败了58.89% 的用户
```
