> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202310091345950.png)
给定一个仅包含数字 `2-9` 的字符串，返回所有它能表示的字母组合。答案可以按 **任意顺序** 返回。给出数字到字母的映射如下（与电话按键相同）。注意 1 不对应任何字母。
<img src="https://img-blog.csdnimg.cn/20200827011421677.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70" width="30%">

**示例 1：**
```java
输入：digits = "23"
输出：["ad","ae","af","bd","be","bf","cd","ce","cf"]
```
**示例 2：**
```java
输入：digits = ""
输出：[]
```
**示例 3：**
```java
输入：digits = "2"
输出：["a","b","c"]
```
**提示：**
- `0 <= digits.length <= 4`
- `digits[i]` 是范围 `['2', '9']` 的一个数字。

---
方法一：回溯
首先使用哈希表存储每个数字对应的所有可能的字母，然后进行回溯操作。

回溯过程中维护一个字符串，表示已有的字母排列（如果未遍历完电话号码的所有数字，则已有的字母排列是不完整的）。该字符串初始为空。每次取电话号码的一位数字，从哈希表中获得该数字对应的所有可能的字母，并将其中的一个字母插入到已有的字母排列后面，然后继续处理电话号码的后一位数字，直到处理完电话号码中的所有数字，即得到一个完整的字母排列。然后进行回退操作，遍历其余的字母排列。

**回溯算法用于寻找所有的可行解，如果发现一个解不可行，则会舍弃不可行的解**。在这道题中，由于每个数字对应的每个字母都可能进入字母组合，因此**不存在不可行的解，直接穷举所有的解即可**。

 
思路1：就是这些数字字符对应的字母集的笛卡尔积。我们用简单的DFS+回溯即可解决。以 `digits = "23"` 为例，它对应的解空间树如下，从根结点到叶子结点的每一条路径都是一个字母组合：

![在这里插入图片描述](https://img-blog.csdnimg.cn/20200915174152211.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70#pic_center)
更加抽象一些的话：
- `digits` 是一个数字字符串；
- `s(digits)` 是 `digits` 所能代表的字母字符串；
- `letter(digits[i])` 是 `digits[i]` 对应的几个字母之一；
- `s(digits[0...n-1]) = letter(digits[0]) + s(digits[1...n-1]) = letter(digits[0]) + letter[digits[1]) + s(digits[2...n-1]) = ...`

先序实现的代码如下，**到达叶子结点时**将字符组合加入到字符串数组 `ans` 中：
```cpp
class Solution {
public:
    unordered_map<char, string> dict = {
        {'2', "abc"}, {'3', "def"}, {'4', "ghi"}, 
        {'5', "jkl"}, {'6', "mno"}, {'7', "pqrs"}, 
        {'8', "tuv"}, {'9', "wxyz"}
    };
    vector<string> ans;
    //s中保存了此时从digits[0...idx-1]翻译得到的一个字母字符串
    //寻找和digits[idx]匹配的字母,获得digits[0...idx]翻译的解
    void dfs(const string &s, const string &digits, int idx) {
        if (idx == digits.size()) {
            ans.push_back(s);
            return;
        }
        const string &t = dict[digits[idx]];
        for (char c : t) 
            dfs(s + c, digits, idx + 1);
    }

    vector<string> letterCombinations(string digits) {
        if (digits.empty()) return ans;
        dfs("", digits, 0); 
        return ans;
    }
};
```
复杂度分析

时间复杂度：O(3m×4n)O(3^m \times 4^n)O(3 
m
 ×4 
n
 )，其中 mmm 是输入中对应 333 个字母的数字个数（包括数字 222、333、444、555、666、888），nnn 是输入中对应 444 个字母的数字个数（包括数字 777、999），m+nm+nm+n 是输入数字的总个数。当输入包含 mmm 个对应 333 个字母的数字和 nnn 个对应 444 个字母的数字时，不同的字母组合一共有 3m×4n3^m \times 4^n3 
m
 ×4 
n
  种，需要遍历每一种字母组合。

空间复杂度：O(m+n)O(m+n)O(m+n)，其中 mmm 是输入中对应 333 个字母的数字个数，nnn 是输入中对应 444 个字母的数字个数，m+nm+nm+n 是输入数字的总个数。除了返回值以外，空间复杂度主要取决于哈希表以及回溯过程中的递归调用层数，哈希表的大小与输入无关，可以看成常数，递归调用层数最大为 m+nm+nm+n。

作者：力扣官方题解
链接：https://leetcode.cn/problems/letter-combinations-of-a-phone-number/solutions/388738/dian-hua-hao-ma-de-zi-mu-zu-he-by-leetcode-solutio/
来源：力扣（LeetCode）
著作权归作者所有。商业转载请联系作者获得授权，非商业转载请注明出处。

效率如下：
```cpp
执行用时：0 ms, 在所有 C++ 提交中击败了100.00% 的用户
内存消耗：6.5 MB, 在所有 C++ 提交中击败了96.46% 的用户
```
后序实现的代码如下，递归基准是：**递归到叶子结点时**返回对应的字符；递归体：在函数返回的所有字符串之前，添加当前 `digits[i]` 对应的字符。
```cpp
class Solution {
public:
    const string letterMap[10] = {" ", "", "abc", "def", "ghi", "jkl", "mno", "pqrs", "tuv", "wxyz"};  
    vector<string> letterCombinations(const string &digits, int idx) {
        vector<string> res;
        //递归边界条件(只有空串时可能用到)
        if (idx >= digits.size()) return res;
        char c = digits[idx];
        assert(c >= '0' && c <= '9' && c != '1');
        const string &letters = letterMap[c - '0'];
        //递归边界条件
        if (idx == digits.size() - 1) {
            for (char ch : letters)
                res.emplace_back(1, ch);
            return res; 
        }
        //递归体
        vector<string> postfix = letterCombinations(digits, idx + 1);
        for (char ch : letters) 
            for (const string &s : postfix) 
                res.push_back(string(1, ch) + s);
        return res;
    } 

    vector<string> letterCombinations(string digits) { 
        return letterCombinations(digits, 0);
    }
};
```
效率如下：
```cpp
执行用时：4 ms, 在所有 C++ 提交中击败了49.75% 的用户
内存消耗：6.8 MB, 在所有 C++ 提交中击败了28.13% 的用户
```

----
思路2：Python可以用列表推导，使用循环解决这道题。C++也可以用循环，只是需要在循环中修改 `ans` 数组。代码如下：
```cpp
class Solution {
public:
    unordered_map<char, string> dict = {
        {'2', "abc"}, {'3', "def"}, {'4', "ghi"}, 
        {'5', "jkl"}, {'6', "mno"}, {'7', "pqrs"}, 
        {'8', "tuv"}, {'9', "wxyz"}
    }; 
    vector<string> letterCombinations(string digits) {
        if (digits.empty()) return vector<string>(); 
        vector<string> ans(1, ""), tmp;
        int j = 0;
        for (char c : digits) { 
            tmp.clear();
            for (const string &pre : ans) { 
                const string &t = dict[c];
                for (char ch : t) 
                    tmp.push_back(pre + ch); 
            }
            ans = tmp;
        }
        return ans;
    }
};
```
效率较低：
```cpp
执行用时：4 ms, 在所有 C++ 提交中击败了48.75% 的用户
内存消耗：6.9 MB, 在所有 C++ 提交中击败了19.08% 的用户
```
