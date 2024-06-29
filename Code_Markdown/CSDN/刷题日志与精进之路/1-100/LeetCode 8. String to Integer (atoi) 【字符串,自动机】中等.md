> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202310091338176.png)

<div class="_1l1MA" data-track-load="description_content"><p>请你来实现一个&nbsp;<code>myAtoi(string s)</code>&nbsp;函数，使其能将字符串转换成一个 32 位有符号整数（类似 C/C++ 中的 <code>atoi</code> 函数）。</p>

<p>函数&nbsp;<code>myAtoi(string s)</code> 的算法如下：</p>

<ol>
	<li>读入字符串并丢弃无用的前导空格</li>
	<li>检查下一个字符（假设还未到字符末尾）为正还是负号，读取该字符（如果有）。 确定最终结果是负数还是正数。 如果两者都不存在，则假定结果为正。</li>
	<li>读入下一个字符，直到到达下一个非数字字符或到达输入的结尾。字符串的其余部分将被忽略。</li>
	<li>将前面步骤读入的这些数字转换为整数（即，"123" -&gt; 123， "0032" -&gt; 32）。如果没有读入数字，则整数为 <code>0</code> 。必要时更改符号（从步骤 2 开始）。</li>
	<li>如果整数数超过 32 位有符号整数范围 <code>[−2<sup>31</sup>,&nbsp; 2<sup>31&nbsp;</sup>− 1]</code> ，需要截断这个整数，使其保持在这个范围内。具体来说，小于 <code>−2<sup>31</sup></code> 的整数应该被固定为 <code>−2<sup>31</sup></code> ，大于 <code>2<sup>31&nbsp;</sup>− 1</code> 的整数应该被固定为 <code>2<sup>31&nbsp;</sup>− 1</code> 。</li>
	<li>返回整数作为最终结果。</li>
</ol>

<strong>注意：</strong> 
<ul>
	<li>本题中的空白字符只包括空格字符 <code>' '</code> 。</li>
	<li>除前导空格或数字后的其余字符串外，<strong>请勿忽略</strong> 任何其他字符。</li>
</ul>
 <br>
 
<strong>示例&nbsp;1：</strong> 
<pre><strong>输入：</strong>s = "42"
<strong>输出：</strong>42
<strong>解释：</strong>加粗的字符串为已经读入的字符，插入符号是当前读取的字符。
第 1 步："42"（当前没有读入字符，因为没有前导空格）
         ^
第 2 步："42"（当前没有读入字符，因为这里不存在 '-' 或者 '+'）
         ^
第 3 步："<u>42</u>"（读入 "42"）
           ^
解析得到整数 42 。
由于 "42" 在范围 [-2<sup>31</sup>, 2<sup>31</sup> - 1] 内，最终结果为 42 。</pre>
 <br>
<strong>示例&nbsp;2：</strong>
<pre><strong>输入：</strong>s = "   -42"
<strong>输出：</strong>-42
<strong>解释：</strong>
第 1 步："<u><strong>   </strong></u>-42"（读入前导空格，但忽视掉）
            ^
第 2 步："   <u><strong>-</strong></u>42"（读入 '-' 字符，所以结果应该是负数）
             ^
第 3 步："   <u><strong>-42</strong></u>"（读入 "42"）
               ^
解析得到整数 -42 。
由于 "-42" 在范围 [-2<sup>31</sup>, 2<sup>31</sup> - 1] 内，最终结果为 -42 。
</pre>
 <br>
<strong>示例&nbsp;3：</strong>

<pre><strong>输入：</strong>s = "4193 with words"
<strong>输出：</strong>4193
<strong>解释：</strong>
第 1 步："4193 with words"（当前没有读入字符，因为没有前导空格）
         ^
第 2 步："4193 with words"（当前没有读入字符，因为这里不存在 '-' 或者 '+'）
         ^
第 3 步："<u>4193</u> with words"（读入 "4193"；由于下一个字符不是一个数字，所以读入停止）
             ^
解析得到整数 4193 。
由于 "4193" 在范围 [-2<sup>31</sup>, 2<sup>31</sup> - 1] 内，最终结果为 4193 。
</pre>
 <br>
<strong>提示：</strong>
<ul>
	<li><code>0 &lt;= s.length &lt;= 200</code></li>
	<li><code>s</code> 由英文字母（大写和小写）、数字（<code>0-9</code>）、<code>' '</code>、<code>'+'</code>、<code>'-'</code> 和 <code>'.'</code> 组成</li>
</ul>
</div>

---
根据问题描述，我们来判断并且描述对应的解题方法。对于这道题目，很明显是字符串的转化问题。**需要明确转化规则，尽量根据转化规则编写对应的子函数**。

这里要进行模式识别，一旦涉及整数的运算，需要注意溢出。本题可能产生溢出的步骤在于推入、乘以 101010 操作和累加操作都可能造成溢出。==对于溢出的处理方式，通常可以转换为 INT_MAX 的逆操作==。
- 比如判断某数乘以 101010 是否会溢出，那么就把该数和 INT_MAX 除以 10 进行比较。??? 第7题传入的是数字，所以可以这样做？
- 比如判断某数+？是否会溢出，那么就把该数和 INT_MAX - ？进行比较。




方法一：自动机


字符串处理的题目往往涉及复杂的流程以及条件情况，如果直接上手写程序，一不小心就会写出极其臃肿的代码。

因此，为了有条理地分析每个输入字符的处理方法，我们可以使用自动机这个概念：

我们的程序在每个时刻有一个状态 s，每次从序列中输入一个字符 c，并根据字符 c 转移到下一个状态 s'。这样，我们只需要建立一个覆盖所有情况的从 s 与 c 映射到 s' 的表格即可解决题目中的问题。

算法

本题可以建立如下图所示的自动机：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202307021746593.png)
我们也可以用下面的表格来表示这个自动机：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202307021746185.png)

' '	+/-	number	other
start	start	signed	in_number	end
signed	end	end	in_number	end
in_number	end	end	in_number	end
end	end	end	end	end
接下来编程部分就非常简单了：我们只需要把上面这个状态转换表抄进代码即可。

另外自动机也需要记录当前已经输入的数字，只要在 s' 为 in_number 时，更新我们输入的数字，即可最终得到输入的数字。

 
class Automaton {
    string state = "start";
    unordered_map<string, vector<string>> table = {
        {"start", {"start", "signed", "in_number", "end"}},
        {"signed", {"end", "end", "in_number", "end"}},
        {"in_number", {"end", "end", "in_number", "end"}},
        {"end", {"end", "end", "end", "end"}}
    };

    int get_col(char c) {
        if (isspace(c)) return 0;
        if (c == '+' or c == '-') return 1;
        if (isdigit(c)) return 2;
        return 3;
    }
public:
    int sign = 1;
    long long ans = 0;

    void get(char c) {
        state = table[state][get_col(c)];
        if (state == "in_number") {
            ans = ans * 10 + c - '0';
            ans = sign == 1 ? min(ans, (long long)INT_MAX) : min(ans, -(long long)INT_MIN);
        }
        else if (state == "signed")
            sign = c == '+' ? 1 : -1;
    }
};

class Solution {
public:
    int myAtoi(string str) {
        Automaton automaton;
        for (char c : str)
            automaton.get(c);
        return automaton.sign * automaton.ans;
    }
};
复杂度分析

时间复杂度：O(n)O(n)O(n)，其中 nnn 为字符串的长度。我们只需要依次处理所有的字符，处理每个字符需要的时间为 O(1)O(1)O(1)。

空间复杂度：O(1)O(1)O(1)。自动机的状态只需要常数空间存储。

准确地说，这是确定有限状态机（deterministic finite automaton, DFA）。其实这题已经不算是容易写“出臃肿代码”的了。考虑清楚边界（主要是溢出处理）和输入种类（+, -, 0-9以及其他），大概在20行内完成代码不难。说实话LC官方题解里很少见给出标准DFA解法的，点个赞。另外看到评论区乌烟瘴气的，真让人唏嘘（没有一点基本的敬畏心、浮躁功利、认为题目边界太复杂导致自己不能AC，那是何其自负）。给两个更加需要DFA的题目吧：[utf-8-validation](https://leetcode-cn.com/problems/utf-8-validation/) ( [附dfa解法](https://leetcode-cn.com/problems/utf-8-validation/solution/java-dfa-by-zdxiq125/) ) 以及 [valid-number](https://leetcode-cn.com/problems/valid-number/)。 顺便贴一下普通解法（那种臃肿的、易错的、可能会被test cases虐到骂娘的；但如果骂娘了，本质还是基本功不扎实）。

```java
class Solution {
    public int myAtoi(String str) {
        str = str.trim();
        if (str.length() == 0) return 0;
        if (!Character.isDigit(str.charAt(0))
            && str.charAt(0) != '-' && str.charAt(0) != '+')
            return 0;
        long ans = 0L;
        boolean neg = str.charAt(0) == '-';
        int i = !Character.isDigit(str.charAt(0)) ? 1 : 0;
        while (i < str.length() && Character.isDigit(str.charAt(i))) {
            ans = ans * 10 + (str.charAt(i++) - '0');
            if (!neg && ans > Integer.MAX_VALUE) {
                ans = Integer.MAX_VALUE;
                break;
            }
            if (neg && ans > 1L + Integer.MAX_VALUE) {
                ans = 1L + Integer.MAX_VALUE;
                break;
            }
        }
        return neg ? (int) -ans : (int) ans;
    }
}
```

----2020.6.9 update------

评论区有一点很有趣，大家对于能不能使用`long`颇有争议。很多时候大家对于“脑筋急转弯式”的题目是有天然抵抗的，因为背离了算法的核心：比如，不允许使用“if-else”，“for循环”去实现某个题目，[面试题64. 求1+2+…+n](https://leetcode-cn.com/problems/qiu-12n-lcof)。这其实可以理解，作为闲时趣味可以，但作为专业问题，“脑筋急转弯”总让人觉得脱离实际。

但是具体到这题上，有很多人又陷入了“脑筋急转弯”式的问题解读思维上。且不说世界上根本就不存在只能存`int`不能存`long`的硬件平台（当然，硬件指令和存取速度可能会随总线宽度而变），这题真的是想在考察边界处理下，再给你加一道“脑筋急转弯”式的声明吗去禁用`long`吗？所以我个人倾向于，题目只是在给我们要实现的`atoi`确立一个合理的取值范围罢了。当然，每个人可以由不同的解读，不想用`long`就多一些溢出判断就可以了（当然，实际写的时候，溢出问题一般需要“反向思考”，还需仔细）。但，这真的是本题的核心吗？

```java
class Solution {
    public int myAtoi(String str) {
        str = str.trim();
        if (str.length() == 0) return 0;
        if (!Character.isDigit(str.charAt(0))
            && str.charAt(0) != '-' && str.charAt(0) != '+')
            return 0;
        int ans = 0;
        boolean neg = str.charAt(0) == '-';
        int i = !Character.isDigit(str.charAt(0)) ? 1 : 0;
        while (i < str.length() && Character.isDigit(str.charAt(i))) {
            int tmp = ((neg ? Integer.MIN_VALUE : Integer.MIN_VALUE + 1) + (str.charAt(i) - '0')) / 10;
            if (tmp > ans) {
                return neg ? Integer.MIN_VALUE : Integer.MAX_VALUE;
            }
            ans = ans * 10 - (str.charAt(i++) - '0');
        }
        return neg ? ans : -ans;
    }
}
```
还是有必要明确一下。DFA的重点在于定义两件事：1. 状态，2. 输入。本题中，一个字符（char）就是一个输入，当我们遍历字符串时，每遇到一个字符，状态都会随之转移。状态则需要你去仔细枚举——开始的空白算是一个状态、而当游标已经在数字中间就又是另一个状态了。这题之所以暴力解决相对容易，是因为`trim()`和判断符号位已经可以干掉两个状态（start和signed）了；而一旦到达end，你就可以直接退出循环了，那么实际上我们真正要处理的就只有题解中的in_number态（不断接收数字就好）。