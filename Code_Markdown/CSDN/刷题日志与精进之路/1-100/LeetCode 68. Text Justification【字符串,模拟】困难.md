> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

给定一个单词数组 `words` 和一个长度 `maxWidth` ，重新排版单词，使其成为每行恰好有 `maxWidth` 个字符，且左右两端对齐的文本。

你应该使用 “**贪心算法**” 来放置给定的单词；也就是说，尽可能多地往每行中放置单词。必要时可用空格 `' '` 填充，使得每行恰好有 _maxWidth_ 个字符。

要求尽可能均匀分配单词间的空格数量。如果某一行单词间的空格不能均匀分配，则左侧放置的空格数要多于右侧的空格数。

文本的最后一行应为左对齐，且单词之间不插入**额外的**空格。

**注意:**

- 单词是指由非空格字符组成的字符序列。
- 每个单词的长度大于 0，小于等于 _maxWidth_。
- 输入单词数组 `words` 至少包含一个单词。

**示例 1:**

**输入:** words = ["This", "is", "an", "example", "of", "text", "justification."], maxWidth = 16
**输出:**
[
   "This    is    an",
   "example  of text",
   "justification.  "
]

**示例 2:**

**输入:**words = ["What","must","be","acknowledgment","shall","be"], maxWidth = 16
**输出:**
[
  "What   must   be",
  "acknowledgment  ",
  "shall be        "
]
**解释:** 注意最后一行的格式应为 "shall be    " 而不是 "shall     be",
     因为最后一行应为左对齐，而不是左右两端对齐。       
     第二行同样为左对齐，这是因为这行只包含一个单词。

**示例 3:**

**输入:**words = ["Science","is","what","we","understand","well","enough","to","explain","to","a","computer.","Art","is","everything","else","we","do"]，maxWidth = 20
**输出:**
[
  "Science  is  what we",
  "understand      well",
  "enough to explain to",
  "a  computer.  Art is",
  "everything  else  we",
  "do                  "
]

**提示:**

- `1 <= words.length <= 300`
- `1 <= words[i].length <= 20`
- `words[i]` 由小写英文字母和符号组成
- `1 <= maxWidth <= 100`
- `words[i].length <= maxWidth`

---
方法一：模拟
根据题干描述的贪心算法，对于每一行，我们首先确定最多可以放置多少单词，这样可以得到该行的空格个数，从而确定该行单词之间的空格个数。

根据题目中填充空格的细节，我们分以下三种情况讨论：

当前行是最后一行：单词左对齐，且单词之间应只有一个空格，在行末填充剩余空格；
当前行不是最后一行，且只有一个单词：该单词左对齐，在行末填充空格；
当前行不是最后一行，且不只一个单词：设当前行单词数为 numWords\textit{numWords}numWords，空格数为 numSpaces\textit{numSpaces}numSpaces，我们需要将空格均匀分配在单词之间，则单词之间应至少有
avgSpaces=⌊numSpacesnumWords−1⌋\textit{avgSpaces}=\Big\lfloor\dfrac{\textit{numSpaces}}{\textit{numWords}-1}\Big\rfloor
avgSpaces=⌊ 
numWords−1
numSpaces
​
 ⌋
个空格，对于多出来的

extraSpaces=numSpaces mod (numWords−1)\textit{extraSpaces}=\textit{numSpaces}\bmod(\textit{numWords}-1)
extraSpaces=numSpacesmod(numWords−1)
个空格，应填在前 extraSpaces\textit{extraSpaces}extraSpaces 个单词之间。因此，前 extraSpaces\textit{extraSpaces}extraSpaces 个单词之间填充 avgSpaces+1\textit{avgSpaces}+1avgSpaces+1 个空格，其余单词之间填充 avgSpaces\textit{avgSpaces}avgSpaces 个空格。

Python3
C++
Java
C#
Golang
JavaScript
class Solution {
    // blank 返回长度为 n 的由空格组成的字符串
    string blank(int n) {
        return string(n, ' ');
    }

    // join 返回用 sep 拼接 [left, right) 范围内的 words 组成的字符串
    string join(vector<string> &words, int left, int right, string sep) {
        string s = words[left];
        for (int i = left + 1; i < right; ++i) {
            s += sep + words[i];
        }
        return s;
    }

public:
    vector<string> fullJustify(vector<string> &words, int maxWidth) {
        vector<string> ans;
        int right = 0, n = words.size();
        while (true) {
            int left = right; // 当前行的第一个单词在 words 的位置
            int sumLen = 0; // 统计这一行单词长度之和
            // 循环确定当前行可以放多少单词，注意单词之间应至少有一个空格
            while (right < n && sumLen + words[right].length() + right - left <= maxWidth) {
                sumLen += words[right++].length();
            }

            // 当前行是最后一行：单词左对齐，且单词之间应只有一个空格，在行末填充剩余空格
            if (right == n) {
                string s = join(words, left, n, " ");
                ans.emplace_back(s + blank(maxWidth - s.length()));
                return ans;
            }

            int numWords = right - left;
            int numSpaces = maxWidth - sumLen;

            // 当前行只有一个单词：该单词左对齐，在行末填充剩余空格
            if (numWords == 1) {
                ans.emplace_back(words[left] + blank(numSpaces));
                continue;
            }

            // 当前行不只一个单词
            int avgSpaces = numSpaces / (numWords - 1);
            int extraSpaces = numSpaces % (numWords - 1);
            string s1 = join(words, left, left + extraSpaces + 1, blank(avgSpaces + 1)); // 拼接额外加一个空格的单词
            string s2 = join(words, left + extraSpaces + 1, right, blank(avgSpaces)); // 拼接其余单词
            ans.emplace_back(s1 + blank(avgSpaces) + s2);
        }
    }
};
复杂度分析

时间复杂度：O(m)O(m)O(m)，其中 mmm 是数组 words\textit{words}words 中所有字符串的长度之和。

空间复杂度：O(m)O(m)O(m)。

作者：力扣官方题解
链接：https://leetcode.cn/problems/text-justification/solutions/986756/wen-ben-zuo-you-dui-qi-by-leetcode-solut-dyeg/


字符串大模拟，分情况讨论即可：

如果当前行只有一个单词，特殊处理为左对齐；
如果当前行为最后一行，特殊处理为左对齐；
其余为一般情况，分别计算「当前行单词总长度」、「当前行空格总长度」和「往下取整后的单位空格长度」，然后依次进行拼接。当空格无法均分时，每次往靠左的间隙多添加一个空格，直到剩余的空格能够被后面的间隙所均分。
代码：

Java
class Solution {
    public List<String> fullJustify(String[] words, int maxWidth) {
        List<String> ans = new ArrayList<>();
        int n = words.length;
        List<String> list = new ArrayList<>();
        for (int i = 0; i < n; ) {
            // list 装载当前行的所有 word
            list.clear();
            list.add(words[i]);
            int cur = words[i++].length();
            while (i < n && cur + 1 + words[i].length() <= maxWidth) {
                cur += 1 + words[i].length();
                list.add(words[i++]);
            }

            // 当前行为最后一行，特殊处理为左对齐
            if (i == n) {
                StringBuilder sb = new StringBuilder(list.get(0));
                for (int k = 1; k < list.size(); k++) {
                    sb.append(" ").append(list.get(k));
                }
                while (sb.length() < maxWidth) sb.append(" ");
                ans.add(sb.toString());
                break;
            }

            // 如果当前行只有一个 word，特殊处理为左对齐
            int cnt = list.size();
            if (cnt == 1) {
                String str = list.get(0);
                while (str.length() != maxWidth) str += " ";
                ans.add(str);
                continue;
            }

            /**
            * 其余为一般情况
            * wordWidth : 当前行单词总长度;
            * spaceWidth : 当前行空格总长度;
            * spaceItem : 往下取整后的单位空格长度
            */
            int wordWidth = cur - (cnt - 1);
            int spaceWidth = maxWidth - wordWidth;
            int spaceItemWidth = spaceWidth / (cnt - 1);
            String spaceItem = "";
            for (int k = 0; k < spaceItemWidth; k++) spaceItem += " ";
            StringBuilder sb = new StringBuilder();
            for (int k = 0, sum = 0; k < cnt; k++) {
                String item = list.get(k);
                sb.append(item);
                if (k == cnt - 1) break;
                sb.append(spaceItem);
                sum += spaceItemWidth;
                // 剩余的间隙数量（可填入空格的次数）
                int remain = cnt - k - 1 - 1;
                // 剩余间隙数量 * 最小单位空格长度 + 当前空格长度 < 单词总长度，则在当前间隙多补充一个空格
                if (remain * spaceItemWidth + sum < spaceWidth) {
                    sb.append(" ");
                    sum++;
                }
            }
            ans.add(sb.toString());
        }
        return ans;
    }
}
时间复杂度：会对 wordswordswords 做线性扫描，最坏情况下每个 words[i]words[i]words[i] 独占一行，此时所有字符串的长度为 n∗maxWidthn * maxWidthn∗maxWidth。复杂度为 O(n∗maxWidth)O(n * maxWidth)O(n∗maxWidth)
空间复杂度：最坏情况下每个 words[i]words[i]words[i] 独占一行，复杂度为 O(n∗maxWidth)O(n * maxWidth)O(n∗maxWidth)
其他「模拟」相关内容
意犹未尽？可以加练如下模拟题。

题目	题解	难度	推荐指数
1. 两数之和	LeetCode 题解链接	简单	🤩🤩🤩🤩🤩
2. 两数相加	LeetCode 题解链接	中等	🤩🤩🤩🤩🤩
5. 最长回文子串	LeetCode 题解链接	中等	🤩🤩🤩🤩🤩
6. Z 字形变换	LeetCode 题解链接	中等	🤩🤩🤩
7. 整数反转	LeetCode 题解链接	简单	🤩🤩🤩
8. 字符串转换整数 (atoi)	LeetCode 题解链接	中等	🤩🤩🤩
12. 整数转罗马数字	LeetCode 题解链接	中等	🤩🤩
13. 罗马数字转整数	LeetCode 题解链接	简单	🤩🤩
14. 最长公共前缀	LeetCode 题解链接	简单	🤩🤩🤩🤩
31. 下一个排列	LeetCode 题解链接	中等	🤩🤩🤩
38. 外观数列	LeetCode 题解链接	简单	🤩🤩
43. 字符串相乘	LeetCode 题解链接	中等	🤩🤩🤩🤩
59. 螺旋矩阵 II	LeetCode 题解链接	中等	🤩🤩🤩🤩
65. 有效数字	LeetCode 题解链接	困难	🤩🤩🤩
68. 文本左右对齐	LeetCode 题解链接	困难	🤩🤩🤩
73. 矩阵置零	LeetCode 题解链接	中等	🤩🤩🤩🤩
165. 比较版本号	LeetCode 题解链接	中等	🤩🤩🤩🤩
168. Excel表列名称	LeetCode 题解链接	简单	🤩🤩🤩
171. Excel表列序号	LeetCode 题解链接	简单	🤩🤩🤩
190. 颠倒二进制位	LeetCode 题解链接	简单	🤩🤩🤩
233. 数字 1 的个数	LeetCode 题解链接	困难	🤩🤩🤩🤩
263. 丑数	LeetCode 题解链接	简单	🤩🤩
345. 反转字符串中的元音字母	LeetCode 题解链接	简单	🤩🤩🤩
413. 等差数列划分	LeetCode 题解链接	中等	🤩🤩🤩🤩
443. 压缩字符串	LeetCode 题解链接	中等	🤩🤩🤩🤩
451. 根据字符出现频率排序	LeetCode 题解链接	中等	🤩🤩🤩🤩
457. 环形数组是否存在循环	LeetCode 题解链接	中等	🤩🤩🤩🤩
528. 按权重随机选择	LeetCode 题解链接	中等	🤩🤩🤩🤩
541. 反转字符串 II	LeetCode 题解链接	简单	🤩🤩🤩🤩🤩
551. 学生出勤记录 I	LeetCode 题解链接	简单	🤩🤩🤩
566. 重塑矩阵	LeetCode 题解链接	简单	🤩🤩🤩
645. 错误的集合	LeetCode 题解链接	简单	🤩🤩🤩
726. 原子的数量	LeetCode 题解链接	困难	🤩🤩🤩🤩
766. 托普利茨矩阵	LeetCode 题解链接	简单	🤩🤩🤩
867. 转置矩阵	LeetCode 题解链接	简单	🤩🤩🤩🤩
896. 单调数列	LeetCode 题解链接	简单	🤩🤩🤩🤩
1047. 删除字符串中的所有相邻重复项	LeetCode 题解链接	简单	🤩🤩🤩🤩
1104. 二叉树寻路	LeetCode 题解链接	中等	🤩🤩🤩
1480. 一维数组的动态和	LeetCode 题解链接	简单	🤩🤩🤩🤩🤩
1486. 数组异或操作	LeetCode 题解链接	简单	🤩🤩🤩
1583. 统计不开心的朋友	LeetCode 题解链接	中等	🤩🤩🤩🤩
1646. 获取生成数组中的最大值	LeetCode 题解链接	简单	🤩🤩🤩🤩
1720. 解码异或后的数组	LeetCode 题解链接	简单	🤩🤩🤩
1736. 替换隐藏数字得到的最晚时间	LeetCode 题解链接	简单	🤩🤩🤩🤩
1743. 从相邻元素对还原数组	LeetCode 题解链接	中等	🤩🤩🤩🤩
1748. 唯一元素的和	LeetCode 题解链接	简单	🤩🤩
1763. 最长的美好子字符串	LeetCode 题解链接	简单	🤩🤩🤩
1834. 单线程 CPU	LeetCode 题解链接	中等	🤩🤩🤩🤩
1893. 检查是否区域内所有整数都被覆盖	LeetCode 题解链接	简单	🤩🤩🤩🤩
面试题 10.02. 变位词组	LeetCode 题解链接	中等	🤩🤩🤩🤩

作者：宫水三叶
链接：https://leetcode.cn/problems/text-justification/solutions/987057/gong-shui-san-xie-zi-fu-chuan-mo-ni-by-a-s3v7/
来源：力扣（LeetCode）
著作权归作者所有。商业转载请联系作者获得授权，非商业转载请注明出处。