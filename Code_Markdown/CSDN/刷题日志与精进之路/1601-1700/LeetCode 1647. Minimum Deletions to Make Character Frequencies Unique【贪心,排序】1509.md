> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的[仓库](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。
 
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202309121337773.png)

A string `s` is called **good** if there are no two different characters in `s` that have the same **frequency**.

Given a string `s`, return _the **minimum** number of characters you need to delete to make_ `s` _**good**._

The **frequency** of a character in a string is the number of times it appears in the string. For example, in the string `"aab"`, the **frequency** of `'a'` is `2`, while the **frequency** of `'b'` is `1`.

**Example 1:**
```js
Input: s = "aab"
Output: 0
Explanation: `s` is already good.
```
**Example 2:**
```java
Input: s = "aaabbbcc"
Output: 2
Explanation: You can delete two 'b's resulting in the good string "aaabcc".
Another way it to delete one 'b' and one 'c' resulting in the good string "aaabbc".
```
**Example 3:**
```java
Input: s = "ceabaacb"
Output: 2
Explanation: You can delete both 'c's resulting in the good string "eabaab".
Note that we only care about characters that are still in the string at the end (i.e. frequency of 0 is ignored).
```
**Constraints:**
- `1 <= s.length <= 10^5`
- `s` contains only lowercase English letters.

---
### 解法 贪心+排序
==因为只能通过去掉字符来减小出现次数，因此出现次数较多的字符应当「尽可能保留」，以免和后面「撞车」==，这就是本题的贪心思路。

首先得到每个字符出现的次数，并从大到小排序。然后从前到后，遍历这个频数数组。设当前频数为 $cnt[i]\ (cnt[i]>0)$ ，并维护此前出现的「最小频数」（设为 $prev$ ）如果：
- $prev > cnt[i]$ ，则无需去除该字符
- 否则，如果 $prev \le cnt[i]$ ，说明**当前频数「过多」，需要去除一定数量的该字符**。因为要尽可能少去除，因此需要保留 ⁡$\max\{prev-1,0\}$ 个字符。
```cpp
class Solution {
public:
    int minDeletions(string s) {
        vector<int> cnt(26, 0);
        for (auto ch: s) {
            cnt[ch - 'a']++;
        }
        sort(cnt.begin(), cnt.end(), greater<int>());
        
        int ret = 0;
        int prev = cnt[0];
        for (int i = 1; i < 26 && cnt[i] > 0; i++) {
            if (prev <= cnt[i]) {
                prev = max(prev - 1, 0); // 要保留的字符数
                ret += (cnt[i] - prev);
            } else {
                prev = cnt[i];
            }
        }
        return ret;
    }
};
```
当然，还可使用 `Set` 进行去重。`HashSet` 中保存不同的数目，如果加进来的数目已经存在，就自减，减到 `HashSet` 中没有的数目。为什么不用排序？例如添加顺序为 $4\ 4\ 3\ 2\ 1$ 和 $3\ 2\ 1\ 4\ 4$ ， 第一种是把 $4\ 3\ 2\ 1$ 每个数都减 $1$ ，答案为 $4$ 。 第二种是直接把最后一个 $4$ 减到 $0$ ，答案也是 $4$ 。所以答案只需要在意去重自减时减少的个数，而不用在意顺序
```java
class Solution {
    public int minDeletions(String s) {
        int[] a = new int[26];
        char[] cs = s.toCharArray();
        for (char c : cs) a[c - 'a'] ++;// 统计字母个数

        Set<Integer> h = new HashSet<Integer>();
        int res = 0;
        for (int i : a) {
            if (i != 0) {               // 有数目才进行判断
                while (h.contains(i)) { // set已经包含就自减
                    i --;
                    res ++;
                }
                if (i != 0) h.add(i);   // 自减到0时，表示完全删除了某个字母，不能加入set中
            }
        }
        return res;
    }
}
```