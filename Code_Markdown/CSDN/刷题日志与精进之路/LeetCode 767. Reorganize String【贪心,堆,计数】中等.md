> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

给定一个字符串 `s` ，检查是否能重新排布其中的字母，使得两相邻的字符不同。

返回 _`s` 的任意可能的重新排列。若不可行，返回空字符串 `""`_ 。

**示例 1:**
```java
输入: s = "aab"
输出: "aba"
```
**示例 2:**
```java
输入: s = "aaab"
输出: ""
```
**提示:**
-   `1 <= s.length <= 500`
-   `s` 只包含小写字母

---
### 分析
本题是典型的使用贪心思想的题。重构字符串时，需要根据「每个字母在字符串中出现的次数」处理每个字母放置的位置。
- ==如果出现次数最多的字母可以在重新排布之后不相邻，则可以重新排布字母使得相邻的字母都不相同==。
- 如果出现次数最多的字母过多，则无法重新排布字母使得相邻的字母都不相同。

假设字符串的长度为 $n$ ，如果可以重新排布成「相邻字母都不相同的字符串」，每个字母最多出现多少次？
- 当 $n$ 是偶数时，有 $\dfrac{n}{2}$ 个偶数下标、$\dfrac{n}{2}$ 个奇数下标，因此每个字母的出现次数都不能超过 $\dfrac{n}{2}$ 次，否则出现次数最多的字母一定会相邻。
- 当 $n$ 是奇数时，有 $\dfrac{n +1 }{2}$ 个偶数下标、$\dfrac{n}{2}$ 个奇数下标，因此每个字母的出现次数都不能超过 $\dfrac{n+1}{2}$ 次，否则出现次数最多的字母一定会相邻。
- 由于 $n$ 是偶数时，$\dfrac{n}{2} = \Big\lfloor \dfrac{n+1}{2} \Big\rfloor$ ，因此可以合并 $n$ 是偶数与 $n$ 是奇数的情况：==如果可以重新排布成相邻的字母都不相同的字符串，每个字母最多出现 $\Big\lfloor \dfrac{n+1}{2} \Big\rfloor$  次==。

因此，首先遍历字符串并统计每个字母的出现次数，**如果存在一个字母的出现次数大于 $\Big\lfloor \dfrac{n+1}{2} \Big\rfloor$ ，则无法重新排布字母使得相邻的字母都不相同，返回空字符串**。如果所有字母的出现次数都不超过 $\Big\lfloor \dfrac{n+1}{2} \Big\rfloor$ ，则考虑如何重新排布字母。

以下提供两种使用贪心的方法，分别基于最大堆和计数。

---
### 解法1 基于最大堆的贪心
维护最大堆存储字母，**堆顶元素为出现次数最多的字母**。首先统计每个字母的出现次数，然后「将出现过的字母及其次数」加入最大堆。

当最大堆的元素个数大于 $1$ 时，每次从最大堆取出两个字母，拼接到重构的字符串，然后将两个字母的出现次数分别减 $1$ ，并将「剩余出现次数大于 $0$ 的字母」重新加入最大堆。==由于最大堆中的元素都是不同的，因此取出的两个字母一定也是不同的，将两个不同的字母拼接到重构的字符串，可以确保相邻的字母都不相同==。
> 这里其实有问题，每次从队列中取出 $2$ 个字符，题目中**没有保证总是按照特定顺序取**。 比如字符串有 $3$ 个 $a$ 、$3$ 个 $b$ 。万一第一次取到了 $a,b$ ，但第二次取到了 $b,a$ 呢？因为他们只是按照频次排序，没有按字母顺序排序。
> <b></b>
> 评论区的回答是：照Java 8的PriorityQueue现行实现，如果一个新元素和堆顶的元素相等，这个新元素不会被sift up到堆顶，那么下一次取出相等元素一定是最早放进去的那个元素，所以当前实现下，这是正确的。但堆本来就不应该存在这个假定（例如JDK 8的文档就写了 *ties are broken arbitrarily* ），所以有可能 *library* 换了实现，这个Case就 *fail* 了。
> <b></b> 
> 解决方案有两个：
> 1. 将取出的两个字符和已经放入结果的最后一个字符比较，先放不同的那个字符。
> 2. ==重载堆的排序，出现次数相同时，按字典序排列==。

如果最大堆变成空，则已经完成字符串的重构。如果最大堆剩下 $1$ 个元素，则取出最后一个字母，拼接到重构的字符串。

对于长度为 $n$ 的字符串，共有 $\Big\lfloor \dfrac{n}{2} \Big\rfloor$ 次每次从最大堆取出两个字母的操作，当 $n$ 是奇数时，还有一次从最大堆取出一个字母的操作，因此重构的字符串的长度一定是 $n$ 。

==当 $n$ 是奇数时，是否可能出现重构的字符串的最后两个字母相同的情况==？如果最后一个字母在整个字符串中至少出现了 $2$ 次，则在最后一次从最大堆取出两个字母时（此时该字母次数为 $2$ ，另一个字母次数为 $1$ ），**该字母会先被选出**，因此不会成为重构的字符串的倒数第二个字母，也不可能出现重构的字符串最后两个字母相同的情况。

因此，在重构字符串可行的情况下，基于最大堆的贪心可以确保得到正确答案。
```java
class Solution {
    public String reorganizeString(String s) {
        if (s.length() < 2) return s;
        int[] counts = new int[26];
        int maxCount = 0;
        int length = s.length();
        for (int i = 0; i < length; ++i) {
            char c = s.charAt(i);
            ++counts[c - 'a'];
            maxCount = Math.max(maxCount, counts[c - 'a']);
        }
        if (maxCount > (length + 1) / 2) return ""; // 超过范围则不可能重构字符串
        var q = new PriorityQueue<Character>(new Comparator<Character>() {
            public int compare(Character letter1, Character letter2) { // 出现次数相同时，按字典序排列
                if (counts[letter1 - 'a'] == counts[letter2 - 'a']) return letter1 - letter2;
                return counts[letter2 - 'a'] - counts[letter1 - 'a']; // 否则次数大的排在前面
            }
        });
        for (char c = 'a'; c <= 'z'; ++c) if (counts[c - 'a'] > 0) q.offer(c);
        StringBuffer sb = new StringBuffer();
        while (q.size() > 1) {
            char letter1 = q.poll();
            char letter2 = q.poll();
            sb.append(letter1);
            sb.append(letter2);
            int idx1 = letter1 - 'a', idx2 = letter2 - 'a';
            --counts[idx1];
            --counts[idx2];
            if (counts[idx1] > 0) q.offer(letter1);
            if (counts[idx2] > 0) q.offer(letter2);
        }
        if (q.size() > 0) sb.append(q.poll());
        return sb.toString();
    }
}
```
复杂度分析：
- 时间复杂度：$O(n\log |\sum| + |\sum|)$ ，其中 $n$ 是字符串的长度，$|\sum|$ 是字符集，本题中字符集为所有小写字母，$|\sum| = 26$ 。
	- 遍历字符串并统计每个字母的出现次数，时间复杂度为 $O(n)$ ；
	- 将每个字母加入最大堆，字母个数最多为 $|\sum|$ ，这里设真正出现的小写字母数量为 $| \sum' |$ ，那么时间复杂度为 $O(|\sum|)$ 加上 $O(| \sum' |\log |\sum'|)$ 或 $O(|\sum'|)$ ，前者是遍历次数数组的时间复杂度，后者是将每个字母依次加入最大堆、或是直接使用一次堆的初始化操作。
	- 重构字符串需要对最大堆进行取出元素和添加元素的操作，取出元素和添加元素的次数都不会超过 $n$ 次，每次操作的时间复杂度为 $O(\log |\sum'|)$ ，因此总复杂度为 $O(n\log |\sum'|)$ 。由于真正出现的小写字母数量为 $| \sum' |$ 一定 $\le$ 字符串的长度 $n$ ，因此上面的时间复杂度 $O(n)$ ，$O(| \sum' |\log |\sum'|)$ 或 $O(|\sum'|)$ 均在渐近意义上小于 $O(n \log | \sum' |)$ ，只需要保留 $O(|\sum|)$ 。
	- 由于 $|\sum' | \le | \sum|$ ，为了不引入额外符号，可以将时间复杂度 $O(n \log | \sum' |)$ 写成 $O(n\log | \sum|)$ 。总时间复杂度为 $O(n \log | \sum | + | \sum |)$ 。
- 空间复杂度：$O(| \sum| + n)$ 。计算字符集的长度和最终答案字符串所需的空间。
 
---
### 解法2 基于计数的贪心
本题的思路是**交替放置最常见的字符**。先统计每个字符出现的次数，然后根据每个字母的出现次数重构字符串。

我之前的做法，需要排序：出现次数最多的字符先分隔排列,接着排列次多的。
```cpp
class Solution {
public:
    string reorganizeString(string s) {
        vector<pair<char, int>> cnt;
        for (int i = 0; i < 26; ++i) cnt.push_back({'a' + i, 0});
        for (const char &c : s) ++cnt[c - 'a'].second; //统计次数
        sort(cnt.begin(), cnt.end(), [&](const pair<char, int>& a, const pair<char, int>& b) {
            return a.second > b.second; //从大到小排序
        }); 
        int k = 0, n = s.size();
        if (cnt[0].second > (n + 1) / 2) return "";  //判断冲突
        string str(n, ' ');
        for (int i = 0; i < 26; ++i) {
            for (int j = 0; j < cnt[i].second; ++j) {
                str[k] = cnt[i].first;
                //if (k > 0 && str[k] == str[k - 1]) return ""; //判断冲突
                //if (k < n - 1 && str[k] == str[k + 1]) return "";
                k += 2; 
                if (k >= n) { 
                    if (n & 1) k = k % n; //奇数长度时,直接模n,从下个空处开始放置
                    else k = (k + 1) % n; //偶数长度时,需要+1,再模n
                } 
            }
        }
        return str;
    }
};
```
执行效率如下：
```cpp
执行用时：0 ms, 在所有 C++ 提交中击败了100.00% 的用户
内存消耗：6.6 MB, 在所有 C++ 提交中击败了19.71% 的用户
```
但还有不用排序的做法。==当 $n$ 是奇数且出现最多的字母的出现次数是 $\dfrac{n+1}{2}$ 时，出现次数最多的字母必须全部放置在偶数下标，否则一定会出现相邻的字母相同的情况==。其余情况下，每个字母放置在偶数下标**或者**奇数下标都是**可行的**。

维护偶数下标 $evenIndex$ 和奇数下标 $oddIndex$ ，初始值分别为 $0$ 和 $1$ 。遍历每个字母，**根据每个字母的出现次数判断字母应该放置在偶数下标还是奇数下标**。

**首先考虑是否可以放置在奇数下标**。根据上述分析可知，==只要字母的出现次数不超过字符串长度的一半（即出现次数小于或等于 $\Big\lfloor \dfrac{n}{2} \Big\rfloor$​ ，就可以放置在奇数下标==；**只有当字母的出现次数超过字符串长度的一半时，才必须放置在偶数下标**。而字母的出现次数超过字符串的长度的一半，只可能发生在 $n$ 是奇数的情况下，且**最多只有一个字母的出现次数会超过字符串长度的一半**。

因此通过如下操作在重构的字符串中放置字母：
- 如果字母的出现次数 $> 0$ 且 $\le \lfloor \dfrac{n}{2} \rfloor$ ，且 $oddIndex$ 没有超过数组下标范围，则将字母放置在 $oddIndex$ ，然后将 $oddIndex$ 的值加 $2$ ；
- **如果字母的出现次数 $> \lfloor \dfrac{n}{2} \rfloor$ ，或 $oddIndex$ 超过数组下标范围**，则将字母放置在 $evenIndex$ ，然后将 $evenIndex$ 的值加 $2$ 
- 如果一个字母出现了多次，则重复上述操作，直到该字母全部放置完毕。

那么上述做法**是否可以确保相邻的字母都不相同**？考虑以下三种情况。
- 如果 $n$ 是奇数且存在一个字母的出现次数为 $\dfrac{n+1}{2}$ ，则该字母全部被放置在偶数下标，**其余的 $\dfrac{n-1}{2}$​个字母都被放置在奇数下标**，因此**相邻的字母一定不相同**。
- 不存在一个字母的出现次数为 $\dfrac{n+1}{2}$ ，则按理来说，同一个字母可全部被放置在奇数下标、或全部被放置在偶数下标，则该字母不可能在相邻的下标出现。
- 当然，如果同个字母先被放置在奇数下标、直到奇数下标超过数组下标范围，然后被放置在偶数下标，**由于该字母的出现次数不会超过 $\lfloor \dfrac{n}{2} \rfloor$ ，因此该字母的最小奇数下标与最大偶数下标之差不小于 $3$ ，不可能在相邻的下标出现**。证明如下：
	- 当 $n$ 是偶数时，如果该字母的出现次数为 $\dfrac{n}{2}$ ，包括 $p$ 个奇数下标和 $q$ 个偶数下标，满足 $p + q = \dfrac{n}{2}$ ，最小奇数下标是 $n - 2p + 1$ ，最大偶数下标是 $2(q - 1)$ ，最小奇数下标与最大偶数下标的差是
	 $$\begin{align} 
    & (n−2p+1)−2(q−1)\\
     &= n−2p+1−2q+2\\
     &= n−2(p+q)+3\\
     &= n−2×\dfrac{n}{2} + 3\\
     &= n - n + 3 = 3\end{align}$$ 
	- 当 $n$ 是奇数时，如果该字母的出现次数为 $\dfrac{n-1}{2}$ ，包括 $p$ 个奇数下标和 $q$ 个偶数下标，满足 $p + q = \dfrac{n-1}{2}$ ，最小奇数下标是 $n - 2p$ ，最大偶数下标是 $2(q - 1)$ ，最小奇数下标与最大偶数下标的差是
	 $$\begin{align} 
    & (n−2p)−2(q−1)\\
     &= n−2p−2q+2\\
     &= n−2(p+q)+2\\
     &= n−2×\dfrac{n-1}{2} + 2\\
     &= n - (n-1) + 2 = 3\end{align}$$ 
因此，在重构字符串可行的情况下，基于计数的贪心可以确保相邻的字母都不相同，得到正确答案。
```java
class Solution {
    public String reorganizeString(String s) {
        if (s.length() < 2) return s;
        int[] counts = new int[26];
        int maxCount = 0;
        int length = s.length();
        for (int i = 0; i < length; i++) {
            char c = s.charAt(i);
            counts[c - 'a']++;
            maxCount = Math.max(maxCount, counts[c - 'a']);
        }
        if (maxCount > (length + 1) / 2) return "";
        char[] ans = new char[length];
        int eventIndex = 0, oddIndex = 1;
        int halfLength = length / 2;
        for (int i = 0; i < 26; ++i) {
            char c = (char)('a' + i);
            while (counts[i] > 0 && counts[i] <= halfLength && oddIndex < length) { // 从奇数下标开始放置
                ans[oddIndex] = c;
                --counts[i];
                oddIndex += 2;
            }
            while (counts[i] > 0) { // 可能是奇数下标放完了,现在放置在偶数下标; 或counts[i]>halfLength
                ans[eventIndex] = c;
                --counts[i];
                eventIndex += 2;
            }
        }
        return new String(ans);
    }
}
```
复杂度分析：
- 时间复杂度：$O(n+|\Sigma|)$ ，其中 $n$ 是字符串的长度，$\Sigma$ 是字符集，在本题中字符集为所有小写字母，$|\Sigma|=26$ 。遍历字符串并统计每个字母的出现次数，时间复杂度是 $O(n)$ 。重构字符串需要进行 $n$ 次放置字母的操作，并遍历每个字母得到出现次数，时间复杂度是 $O(n+|\Sigma|)$ 。 总时间复杂度是 $O(n+|\Sigma|)$ 。
- 空间复杂度：$O(|\Sigma|+n)$ ，其中 $n$ 是字符串的长度，$\Sigma$ 是字符集，在本题中字符集为所有小写字母，$|\Sigma|=26$ 。空间复杂度主要取决于统计每个字母出现次数的空间。这里计算存储最终答案字符串需要的空间。



 