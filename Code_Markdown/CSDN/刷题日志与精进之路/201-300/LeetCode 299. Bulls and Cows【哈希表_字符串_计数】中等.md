> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的[仓库](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。


<p>You are playing the <strong><a href="https://en.wikipedia.org/wiki/Bulls_and_Cows">Bulls and Cows</a></strong> game with your friend.</p>

<p>You write down a secret number and ask your friend to guess what the number is. When your friend makes a guess, you provide a hint with the following info:</p>

<ul>
	<li>The number of "bulls", which are digits in the guess that are in the correct position.</li>
	<li>The number of "cows", which are digits in the guess that are in your secret number but are located in the wrong position. Specifically, the non-bull digits in the guess that could be rearranged such that they become bulls.</li>
</ul>

<p>Given the secret number <code>secret</code> and your friend's guess <code>guess</code>, return <em>the hint for your friend's guess</em>.</p>

<p>The hint should be formatted as <code>"xAyB"</code>, where <code>x</code> is the number of bulls and <code>y</code> is the number of cows. Note that both <code>secret</code> and <code>guess</code> may contain duplicate digits.</p>

 
<p><strong>Example 1:</strong></p>

<pre><strong>Input:</strong> secret = "1807", guess = "7810"
<strong>Output:</strong> "1A3B"
<strong>Explanation:</strong> Bulls are connected with a '|' and cows are underlined:
"1807"
  |
"<u>7</u>8<u>10</u>"</pre>

<p><strong>Example 2:</strong></p>

<pre><strong>Input:</strong> secret = "1123", guess = "0111"
<strong>Output:</strong> "1A1B"
<strong>Explanation:</strong> Bulls are connected with a '|' and cows are underlined:
"1123"        "1123"
  |      or     |
"01<u>1</u>1"        "011<u>1</u>"
Note that only one of the two unmatched 1s is counted as a cow since the non-bull digits can only be rearranged to allow one 1 to be a bull.
</pre>

<p><strong>Example 3:</strong></p>

<pre><strong>Input:</strong> secret = "1", guess = "0"
<strong>Output:</strong> "0A0B"
</pre>

<p><strong>Example 4:</strong></p>

<pre><strong>Input:</strong> secret = "1", guess = "1"
<strong>Output:</strong> "1A0B"
</pre>

 <strong>Constraints:</strong> 
<ul>
	<li><code>1 &lt;= secret.length, guess.length &lt;= 1000</code></li>
	<li><code>secret.length == guess.length</code></li>
	<li><code>secret</code> and <code>guess</code> consist of digits only.</li>
</ul>

题意：你在和朋友一起玩 <a href="https://baike.baidu.com/item/%E7%8C%9C%E6%95%B0%E5%AD%97/83200?fromtitle=Bulls+and+Cows&amp;fromid=12003488&amp;fr=aladdin">猜数字（Bulls and Cows）</a>游戏，该游戏规则为——写出一个秘密数字，并请朋友猜这个数字是多少。朋友每猜测一次，你就会给他一个包含下述信息的提示：
<ul>
	<li>猜测数字中有多少位属于数字和确切位置都猜对了（称为 "Bulls", 公牛），</li>
	<li>有多少位属于数字猜对了但是位置不对（称为 "Cows", 奶牛）。也就是说，这次猜测中有多少位非公牛数字可以通过重新排列转换成公牛数字。</li>
</ul>

给你一个秘密数字&nbsp;<code>secret</code> 和朋友猜测的数字&nbsp;<code>guess</code> ，请你返回对朋友这次猜测的提示。提示的格式为 <code>"xAyB"</code> ，<code>x</code> 是公牛个数， <code>y</code> 是奶牛个数，<code>A</code> 表示公牛，<code>B</code>&nbsp;表示奶牛。 

---
### 解法 哈希表+计数
根据题意，我们可以对 `secret` 和 `guess` 的诸位进行逐个比较，统计公牛个数 `a` 和奶牛个数 `b` 。具体来说，对 `secret` 和 `guess` 中字符相同的位置，令 `a` 自增一；对字符不同的位置，使用「哈希表」分别统计该字符的词频。某个数字 `x` 在 `secret` 和 `guess` 词频中的较小值，即为该数字对应的奶牛数量，所有数字 `[0, 9]` 对应的奶牛数量的总和即为 `b` 。最后构建字符串 `aAbB` 返回即可。

这一做法的时间复杂度为 $O(n)$ ，空间复杂度为 $O(C)$ ，即字符集大小 $C= 10$ 。
```cpp
//C++ version
class Solution {
public:
    string getHint(string secret, string guess) {
        string ans;
        int a = 0, b = 0, scnt[10] = {0}, gcnt[10] = {0};
        for (int i = 0, n = secret.size(); i < n; ++i) {
            if (secret[i] == guess[i]) ++a;
            else ++scnt[secret[i] - '0'], ++gcnt[guess[i] - '0'];
        }
        for (int i = 0; i < 10; ++i) b += min(scnt[i], gcnt[i]);
        return to_string(a) + "A" + to_string(b) + "B";
    }
};
//执行用时：4 ms, 在所有 C++ 提交中击败了79.40% 的用户
//内存消耗：6.4 MB, 在所有 C++ 提交中击败了62.63% 的用户
```
虽说可以一个循环搞定，不过过多的分支可能降低算法效率。
```cpp
//C++ version
class Solution {
public:
    string getHint(string secret, string guess) {
        string ans;
        int a = 0, b = 0, cnt[10] = {0};
        for (int i = 0, n = secret.size(); i < n; ++i) {
            if (secret[i] == guess[i]) ++a;
            else {
                if (cnt[guess[i] - '0']-- > 0) ++b;
                if (cnt[secret[i] - '0']++ < 0) ++b;
            }
        }
        return to_string(a) + "A" + to_string(b) + "B";
    }
};
//执行用时：4 ms, 在所有 C++ 提交中击败了79.40% 的用户
//内存消耗：6.2 MB, 在所有 C++ 提交中击败了99.28% 的用户
```
