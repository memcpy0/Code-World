> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

给你一个字符串 `croakOfFrogs`，它表示不同青蛙发出的蛙鸣声（字符串 `"croak"` ）的组合。由于同一时间可以有多只青蛙呱呱作响，所以 `croakOfFrogs` 中会混合多个 `"croak"` _。_

请你返回模拟字符串中所有蛙鸣所需不同青蛙的最少数目。

要想发出蛙鸣 "croak"，青蛙必须 **依序** 输出 `'c', 'r', 'o', 'a', 'k'` 这 5 个字母。如果没有输出全部五个字母，那么它就不会发出声音。如果字符串 `croakOfFrogs` 不是由若干有效的 "croak" 字符混合而成，请返回 `-1` 。

<p><strong>示例 1：</strong></p>
<pre><strong>输入：</strong>croakOfFrogs = "croakcroak"
<strong>输出：</strong>1 
<strong>解释：</strong>一只青蛙 "呱呱" 两次
</pre>
<p><strong>示例 2：</strong></p><pre><strong>输入：</strong>croakOfFrogs = "crcoakroak"
<strong>输出：</strong>2 
<strong>解释：</strong>最少需要两只青蛙，"呱呱" 声用黑体标注
第一只青蛙 "<strong>cr</strong>c<strong>oak</strong>roak"
第二只青蛙 "cr<strong>c</strong>oak<strong>roak</strong>"
</pre>
<p><strong>示例 3：</strong></p><pre><strong>输入：</strong>croakOfFrogs = "croakcrook"
<strong>输出：</strong>-1
<strong>解释：</strong>给出的字符串不是 "croak" 的有效组合。
</pre>

**提示：**
- `1 <= croakOfFrogs.length <= 10^5`
- 字符串中的字符只有 `'c'`, `'r'`, `'o'`, `'a'` 或者 `'k'`

---
### 解法 模拟
这道题和括号匹配等题目有点像，但很不相同，因为可能所有青蛙输出的五个字母都不连续，也就不太适合用栈来求解。
> 后来想想说不定可以用栈，先用下面的做法判断一下字符串是否由「依次输出的 `'c','r','o','a','k'` 」五个字符组成，然后只关注叫声的开始和结尾 `'c', 'k'` ，即分别作为左括号和右括号，使用一个栈进行记录，**求最深的括号嵌套深度**。

由题目已知，一只青蛙要发出蛙鸣，必须依次发出这五个字符 `'c','r','o','a','k'` 。我们把每个字符看作一个状态，**一只青蛙必须经过这五个状态，最终达到 `'k'` 结束状态，才能发出蛙鸣**。也就是说，**每种状态（字符）都是从前一种状态（字符）转移过来**，例如一只青蛙发声 `'o'` 时、必须先发声 `'r'` ，即从发声 `'r'` 的青蛙转移过来。如果前一种状态不存在青蛙，则无法转移，发声失败。特别的，如果当前状态为 `'c'` ，其前一种状态为**未开始蛙鸣**，和**蛙鸣结束**一样，都处于 `'k'` ，且无法从前一种状态转移过来时，说明要新增一只青蛙来蛙鸣。

设 $status[i]$ 分别为处于 `'c','r','o','a','k'` 五种状态中第 $i$ 种状态的青蛙数目，$ans$ 为所需的青蛙总数。
- 如果当前状态为 `'c'` ：
	- 前一种状态 `'k'` 对应的青蛙数目 $status[4]$ 不为 $0$ ，说明有青蛙蛙鸣结束，可以继续发出新的蛙鸣，令 $status[4]$ 减一，$status[0]$ 加一。
	- 否则，说明要新增一只青蛙，发出新的蛙鸣，令 $ans$ 和 $status[0]$ 各加一。
- 如果处于其他四种状态，则不能新增青蛙：
	- 前一种状态对应的青蛙数目 $status[i - 1]$ 不为 $0$ ，说明可以转移过来，令 $status[i - 1]$ 减一，$status[i]$ 加一。
	- 否则，说明前一种状态的青蛙不够用，直接失败。
- 最后，遍历完整个字符串，如果 $ans$ 等于 $status[4]$（即**所有青蛙最终都必须回到 `'k'` 结束状态**），则返回 $ans$ ，否则返回 $-1$ 。

```c
int minNumberOfFrogs(char* croakOfFrogs){
    int status[5] = {0}, ans = 0;
    for (char *q = croakOfFrogs; *q; ++q) {
        int c = *q == 'c' ? 0 : (*q == 'r' ? 1 : (*q == 'o' ? 2 : (*q == 'a' ? 3 : 4)));
        if (c == 0) { // 说明可能需要一只新青蛙从头开始蛙鸣
            if (status[4]) // 已经有青蛙蛙鸣结束
                --status[4], ++status[0]; // 让这只青蛙开始新的蛙鸣
            else ++ans, ++status[0]; // 不够用了,增加一只新青蛙开始新的蛙鸣
        } else { // 如果不是开头,则不能增加青蛙,不够用就直接失败
            if (status[c - 1])
                --status[c - 1], ++status[c]; 
            else return -1;
        }
    }
    return status[4] == ans ? ans : -1; // 最终所有青蛙必须回到k状态
}
```
进一步简化代码：
```c
int minNumberOfFrogs(char* croakOfFrogs){
    int status[5] = {0}, ans = 0;
    for (char *q = croakOfFrogs; *q; ++q) {
        int c = *q == 'c' ? 0 : (*q == 'r' ? 1 : (*q == 'o' ? 2 : (*q == 'a' ? 3 : 4)));
        c = (c + 4) % 5;
        if (status[c]) --status[c];
        else if (c == 4) ++ans;
        else return -1;
        ++status[(c + 1) % 5];
    }
    return status[4] == ans ? ans : -1; // 最终所有青蛙必须回到k状态
}
```
复杂度分析：
- 时间复杂度：$\mathcal{O}(n)$ ，其中 $n$ 为 $croak$ 的长度。
- 空间复杂度：$\mathcal{O}(|\Sigma|)$ ，其中 $|\Sigma|$  为字符集合的大小。