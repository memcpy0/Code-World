> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202310091351754.png)

给定一个字符串 `s` 和一个字符串数组 `words`**。** `words` 中所有字符串 **长度相同**。

 `s` 中的 **串联子串** 是指一个包含  `words` 中所有字符串以任意顺序排列连接起来的子串。
- 例如，如果 `words = ["ab","cd","ef"]`， 那么 `"abcdef"`， `"abefcd"`，`"cdabef"`， `"cdefab"`，`"efabcd"`， 和 `"efcdab"` 都是串联子串。 `"acdbef"` 不是串联子串，因为他不是任何 `words` 排列的连接。

返回所有串联字串在 `s` 中的开始索引。你可以以 **任意顺序** 返回答案。

**示例 1：**
```java
输入：s = "barfoothefoobarman", words = ["foo","bar"]
输出：`[0,9]`
解释：因为 words.length == 2 同时 words[i].length == 3，连接的子字符串的长度必须为 6。
子串 "barfoo" 开始位置是 0。它是 words 中以 ["bar","foo"] 顺序排列的连接。
子串 "foobar" 开始位置是 9。它是 words 中以 ["foo","bar"] 顺序排列的连接。
输出顺序无关紧要。返回 [9,0] 也是可以的。
```

**示例 2：**

**输入：**s = "wordgoodgoodgoodbestword", words = ["word","good","best","word"]
`**输出：**[]`
**解释：**因为 words.length == 4 并且 words[i].length == 4，所以串联子串的长度必须为 16。
s 中没有子串长度为 16 并且等于 words 的任何顺序排列的连接。
所以我们返回一个空数组。

**示例 3：**

**输入：**s = "barfoofoobarthefoobarman", words = ["bar","foo","the"]
**输出：**[6,9,12]
**解释：**因为 words.length == 3 并且 words[i].length == 3，所以串联子串的长度必须为 9。
子串 "foobarthe" 开始位置是 6。它是 words 中以 ["foo","bar","the"] 顺序排列的连接。
子串 "barthefoo" 开始位置是 9。它是 words 中以 ["bar","the","foo"] 顺序排列的连接。
子串 "thefoobar" 开始位置是 12。它是 words 中以 ["the","foo","bar"] 顺序排列的连接。

**提示：**

- `1 <= s.length <= 104`
- `1 <= words.length <= 5000`
- `1 <= words[i].length <= 30`
- `words[i]` 和 `s` 由小写英文字母组成

---
### 方法一：滑动窗口
思路

此题是「438. 找到字符串中所有字母异位词」的进阶版。不同的是第 438 题的元素是字母，而此题的元素是单词。可以用类似「438. 找到字符串中所有字母异位词的官方题解」的方法二的滑动窗口来解这题。

记 words\textit{words}words 的长度为 mmm，words\textit{words}words 中每个单词的长度为 nnn，sss 的长度为 ls\textit{ls}ls。首先需要将 sss 划分为单词组，每个单词的大小均为 nnn （首尾除外）。这样的划分方法有 nnn 种，即先删去前 iii （i=0∼n−1i = 0 \sim n-1i=0∼n−1）个字母后，将剩下的字母进行划分，如果末尾有不到 nnn 个字母也删去。对这 nnn 种划分得到的单词数组分别使用滑动窗口对 words\textit{words}words 进行类似于「字母异位词」的搜寻。

划分成单词组后，一个窗口包含 sss 中前 mmm 个单词，用一个哈希表 differ\textit{differ}differ 表示窗口中单词频次和 words\textit{words}words 中单词频次之差。初始化 differ\textit{differ}differ 时，出现在窗口中的单词，每出现一次，相应的值增加 111，出现在 words\textit{words}words 中的单词，每出现一次，相应的值减少 111。然后将窗口右移，右侧会加入一个单词，左侧会移出一个单词，并对 differ\textit{differ}differ 做相应的更新。窗口移动时，若出现 differ\textit{differ}differ 中值不为 000 的键的数量为 000，则表示这个窗口中的单词频次和 words\textit{words}words 中单词频次相同，窗口的左端点是一个待求的起始位置。划分的方法有 nnn 种，做 nnn 次滑动窗口后，即可找到所有的起始位置。

```dart
class Solution {
    public List<Integer> findSubstring(String s, String[] words) {
        List<Integer> res = new ArrayList<>();
        // 所有单词的个数
        int num = words.length;
        // 每个单词的长度（是相同的）
        int wordLen = words[0].length();
        // 字符串长度
        int stringLen = s.length();

        for (int i = 0; i < wordLen; i++) {
            // 遍历的长度超过了整个字符串的长度，退出循环
            if (i + num * wordLen > stringLen) {
                break;
            }
            // differ表示窗口中的单词频次和words中的单词频次之差
            Map<String, Integer> differ = new HashMap<>();
            // 初始化窗口，窗口长度为num * wordLen,依次计算窗口里每个切分的单词的频次
            for (int j = 0; j < num; j++) {
                String word = s.substring(i + j * wordLen, i + (j + 1) * wordLen);
                differ.put(word, differ.getOrDefault(word, 0) + 1);
            }
            // 遍历words中的word，对窗口里每个单词计算差值
            for (String word : words) {
                differ.put(word, differ.getOrDefault(word, 0) - 1);
                // 差值为0时，移除掉这个word
                if (differ.get(word) == 0) {
                    differ.remove(word);
                }
            }
            // 开始滑动窗口
            for (int start = i; start < stringLen - num * wordLen + 1; start += wordLen) {
                if (start != i) {
                    // 右边的单词滑进来
                    String word = s.substring(start + (num - 1) * wordLen, start + num * wordLen);
                    differ.put(word, differ.getOrDefault(word, 0) + 1);
                    if (differ.get(word) == 0) {
                        differ.remove(word);
                    }
                    // 左边的单词滑出去
                    word = s.substring(start - wordLen, start);
                    differ.put(word, differ.getOrDefault(word, 0) - 1);
                    if (differ.get(word) == 0) {
                        differ.remove(word);
                    }
                    word = s.substring(start - wordLen, start);
                }
                // 窗口匹配的单词数等于words中对应的单词数
                if (differ.isEmpty()) {
                    res.add(start);
                }
            }
        }
        return res;
    }
}
```
复杂度分析

时间复杂度：O(ls×n)O(\textit{ls} \times n)O(ls×n)，其中 ls\textit{ls}ls 是输入 sss 的长度，nnn 是 words\textit{words}words 中每个单词的长度。需要做 nnn 次滑动窗口，每次需要遍历一次 sss。

空间复杂度：O(m×n)O(m \times n)O(m×n)，其中 mmm 是 words\textit{words}words 的单词数，nnn 是 words\textit{words}words 中每个单词的长度。每次滑动窗口时，需要用一个哈希表保存单词频次。

朴素哈希表
令 n 为字符串 s 的长度，m 为数组 words 的长度（单词的个数），w 为单个单词的长度。

由于 words 里面每个单词长度固定，而我们要找的字符串只能恰好包含所有的单词，因此我们要找的目标子串的长度为 m×wm \times wm×w。

那么一个直观的思路是：

使用哈希表 map 记录 words 中每个单词的出现次数
枚举 s 中的每个字符作为起点，往后取得长度为 m×wm \times wm×w 的子串 sub
使用哈希表 cur 统计 sub 每个单词的出现次数（每隔 w 长度作为一个单词）
比较 cur 和 map 是否相同
注意：在步骤 333 中，如果发现 sub 中包含了 words 没有出现的单词，可以直接剪枝。

剪枝处使用了带标签的 continue 语句直接回到外层循环进行。

代码：

Java
class Solution {
    public List<Integer> findSubstring(String s, String[] words) {
        int n = s.length(), m = words.length, w = words[0].length();
        Map<String, Integer> map = new HashMap<>();
        for (String word : words) map.put(word, map.getOrDefault(word, 0) + 1);
        List<Integer> ans = new ArrayList<>();
        out:for (int i = 0; i + m * w <= n; i++) {
            Map<String, Integer> cur = new HashMap<>();
            String sub = s.substring(i, i + m * w);
            for (int j = 0; j < sub.length(); j += w) {
                String item = sub.substring(j, j + w);
                if (!map.containsKey(item)) continue out;
                cur.put(item, cur.getOrDefault(item, 0) + 1);
            }
            if (cur.equals(map)) ans.add(i);
        }
        return ans;
    }

}
时间复杂度：将 words 中的单词存入哈希表，复杂度为 O(m)O(m)O(m)；然后第一层循环枚举 s 中的每个字符作为起点，复杂度为 O(n)O(n)O(n)；在循环中将 sub 划分为 m 个单词进行统计，枚举了 m - 1 个下标，复杂度为 O(m)O(m)O(m)；每个字符串的长度为 w。整体复杂度为 O(n×m×w)O(n \times m \times w)O(n×m×w)
空间复杂度：O(m×w)O(m \times w)O(m×w)
滑动窗口 & 哈希表
事实上，我们可以优化这个枚举起点的过程。

我们可以将起点根据 当前下标与单词长度的取余结果 进行分类，这样我们就不用频繁的建立新的哈希表和进行单词统计。

代码：

Java
class Solution {
    public List<Integer> findSubstring(String s, String[] words) {
        int n = s.length(), m = words.length, w = words[0].length();
        // 统计 words 中「每个目标单词」的出现次数
        Map<String, Integer> map = new HashMap<>();
        for (String word : words) map.put(word, map.getOrDefault(word, 0) + 1);
        List<Integer> ans = new ArrayList<>();
        for (int i = 0; i < w; i++) {
            // 构建一个当前子串对应 map，统计当前子串中「每个目标单词」的出现次数
            Map<String, Integer> curMap = new HashMap<>();
            // 滑动窗口的大小固定是 m * w
            // 每次将下一个单词添加进 cur，上一个单词移出 cur
            for (int j = i; j + w <= n; j += w) {   
                String cur = s.substring(j, j + w);
                if (j >= i + (m * w)) {
                    int idx = j - m * w;
                    String prev = s.substring(idx, idx + w);
                    if (curMap.get(prev) == 1) curMap.remove(prev);    
                    else curMap.put(prev, curMap.get(prev) - 1);
                }
                curMap.put(cur, curMap.getOrDefault(cur, 0) + 1);
                // 如果当前子串对应 map 和 words 中对应的 map 相同，说明当前子串包含了「所有的目标单词」，将起始下标假如结果集
                if (map.containsKey(cur) && curMap.get(cur).equals(map.get(cur)) && curMap.equals(map)) {
                    ans.add(j - (m - 1) * w);
                }
            }
        }
        return ans;
    }
}
时间复杂度：将 words 中的单词存入哈希表，复杂度为 O(m)O(m)O(m)；然后枚举了取余的结果，复杂度为 O(w)O(w)O(w)；每次循环最多处理 n 长度的字符串，复杂度为 O(n)O(n)O(n)。整体复杂度为 O(m+w×n)O(m + w \times n)O(m+w×n)
空间复杂度：O(m×w)O(m \times w)O(m×w)

作者：宫水三叶
链接：https://leetcode.cn/problems/substring-with-concatenation-of-all-words/solutions/575605/shua-chuan-lc-po-su-ha-xi-biao-jie-fa-hu-ml3x/
来源：力扣（LeetCode）
著作权归作者所有。商业转载请联系作者获得授权，非商业转载请注明出处。

写在前面
原版题解在 这里，本次只针对「解法二」做了代码优化。

朴素哈希表
令 n 为字符串 s 的长度，m 为数组 words 的长度（单词的个数），w 为单个单词的长度。

由于 words 里面每个单词长度固定，而我们要找的字符串只能恰好包含所有的单词，因此我们要找的目标子串的长度为 m×wm \times wm×w。

那么一个直观的思路是：

使用哈希表 map 记录 words 中每个单词的出现次数
枚举 s 中的每个字符作为起点，往后取得长度为 m×wm \times wm×w 的子串 sub
使用哈希表 cur 统计 sub 每个单词的出现次数（每隔 w 长度作为一个单词）
比较 cur 和 map 是否相同
注意：在步骤 333 中，如果发现 sub 中包含了 words 没有出现的单词，可以直接剪枝。

剪枝处使用了带标签的 continue 语句直接回到外层循环进行。

代码：

Java
class Solution {
    public List<Integer> findSubstring(String s, String[] words) {
        int n = s.length(), m = words.length, w = words[0].length();
        Map<String, Integer> map = new HashMap<>();
        for (String word : words) map.put(word, map.getOrDefault(word, 0) + 1);
        List<Integer> ans = new ArrayList<>();
        out:for (int i = 0; i + m * w <= n; i++) {
            Map<String, Integer> cur = new HashMap<>();
            String sub = s.substring(i, i + m * w);
            for (int j = 0; j < sub.length(); j += w) {
                String item = sub.substring(j, j + w);
                if (!map.containsKey(item)) continue out;
                cur.put(item, cur.getOrDefault(item, 0) + 1);
            }
            if (cur.equals(map)) ans.add(i);
        }
        return ans;
    }
}
时间复杂度：将 words 中的单词存入哈希表，复杂度为 O(m)O(m)O(m)（由于字符串长度固定且不超过 303030，假定所有哈希操作均为 O(1)O(1)O(1) 的）；然后第一层循环枚举 s 中的每个字符作为起点，复杂度为 O(n)O(n)O(n)；在循环中将 sub 划分为 m 个单词进行统计，枚举了 m - 1 个下标，复杂度为 O(m)O(m)O(m)；每个字符串的长度为 w。整体复杂度为 O(n×m×w)O(n \times m \times w)O(n×m×w)
空间复杂度：O(m×w)O(m \times w)O(m×w)
滑动窗口 + 哈希表
事实上，我们可以优化这个枚举起点的过程。

我们可以将起点根据 当前下标与单词长度的取余结果 进行分类，这样我们就不用频繁的建立新的哈希表和进行单词统计。

对于取余结果相同的位置，我们可以运用「滑动窗口」来进行词频统计，复杂度为 O(n)O(n)O(n)，当处理完所有余数情况时，代表我们枚举完了所有的起点。

代码：

Java
class Solution {
    public List<Integer> findSubstring(String s, String[] words) {
        int n = s.length(), m = words.length, w = words[0].length();
        // 统计 words 中「每个目标单词」的出现次数
        Map<String, Integer> map = new HashMap<>();
        for (String str : words) map.put(str, map.getOrDefault(str, 0) + 1);
        List<Integer> ans = new ArrayList<>();
        for (int i = 0; i < w; i++) {
            // 构建一个当前子串对应的哈希表，统计当前子串中「每个目标单词」的出现次数
            Map<String, Integer> temp = new HashMap<>();
            // 滑动窗口的大小固定是 m * w，每次将下一个单词添加进 temp，上一个单词移出 temp
            for (int j = i; j + w <= n; j += w) {   
                String cur = s.substring(j, j + w);
                temp.put(cur, temp.getOrDefault(cur, 0) + 1);
                if (j >= i + (m * w)) {
                    int idx = j - m * w;
                    String prev = s.substring(idx, idx + w);
                    if (temp.get(prev) == 1) temp.remove(prev);
                    else temp.put(prev, temp.get(prev) - 1);
                    if (!temp.getOrDefault(prev, 0).equals(map.getOrDefault(prev, 0))) continue;
                }
                if (!temp.getOrDefault(cur, 0).equals(map.getOrDefault(cur, 0))) continue;
                // 上面两个 continue 可以减少 map 之间的 equals 操作
                if (temp.equals(map)) ans.add(j - (m - 1) * w);
            }
        }
        return ans;
    }
}
时间复杂度：将 words 中的单词存入哈希表，复杂度为 O(m)O(m)O(m)（由于字符串长度固定且不超过 303030，假定所有哈希操作均为 O(1)O(1)O(1) 的）；然后枚举了取余的结果，复杂度为 O(w)O(w)O(w)；每次循环最多处理 n 长度的字符串，复杂度为 O(n)O(n)O(n)。整体复杂度为 O(m+w×n)O(m + w \times n)O(m+w×n)
空间复杂度：O(m×w)O(m \times w)O(m×w)


作者：宫水三叶
链接：https://leetcode.cn/problems/substring-with-concatenation-of-all-words/solutions/1619194/by-ac_oier-enjd/
来源：力扣（LeetCode）
著作权归作者所有。商业转载请联系作者获得授权，非商业转载请注明出处。

此题是「438. 找到字符串中所有字母异位词」的进阶版。不同的是第 438 题的元素是字母，而此题的元素是单词。可以用类似「438. 找到字符串中所有字母异位词的官方题解」的方法二的滑动窗口来解这题。

记 words\textit{words}words 的长度为 mmm，words\textit{words}words 中每个单词的长度为 nnn，sss 的长度为 ls\textit{ls}ls。首先需要将 sss 划分为单词组，每个单词的大小均为 nnn （首尾除外）。这样的划分方法有 nnn 种，即先删去前 iii （i=0∼n−1i = 0 \sim n-1i=0∼n−1）个字母后，将剩下的字母进行划分，如果末尾有不到 nnn 个字母也删去。对这 nnn 种划分得到的单词数组分别使用滑动窗口对 words\textit{words}words 进行类似于「字母异位词」的搜寻。

划分成单词组后，一个窗口包含 sss 中前 mmm 个单词，用一个哈希表 differ\textit{differ}differ 表示窗口中单词频次和 words\textit{words}words 中单词频次之差。初始化 differ\textit{differ}differ 时，出现在窗口中的单词，每出现一次，相应的值增加 111，出现在 words\textit{words}words 中的单词，每出现一次，相应的值减少 111。然后将窗口右移，右侧会加入一个单词，左侧会移出一个单词，并对 differ\textit{differ}differ 做相应的更新。窗口移动时，若出现 differ\textit{differ}differ 中值不为 000 的键的数量为 000，则表示这个窗口中的单词频次和 words\textit{words}words 中单词频次相同，窗口的左端点是一个待求的起始位置。划分的方法有 nnn 种，做 nnn 次滑动窗口后，即可找到所有的起始位置。

作者：力扣官方题解
链接：https://leetcode.cn/problems/substring-with-concatenation-of-all-words/solutions/1616997/chuan-lian-suo-you-dan-ci-de-zi-chuan-by-244a/
来源：力扣（LeetCode）
著作权归作者所有。商业转载请联系作者获得授权，非商业转载请注明出处。

下面这种写法会在：出错
![Uploading file...472np]()

```java
class Solution {
    public List<Integer> findSubstring(String s, String[] words) {
        int n = s.length(), m = words.length, w = words[0].length();
        var map = new HashMap<String, Integer>();
        for (String word : words) map.put(word, map.getOrDefault(word, 0) + 1);
        var ans = new ArrayList<Integer>();
        for (int i = 0; i < w; ++i) {
            var tmp = new HashMap<String, Integer>();
            for (int j = i; j + w <= n; j += w) {
                String cur = s.substring(j, j + w);
                tmp.put(cur, tmp.getOrDefault(cur, 0) + 1);
                if (j >= i + (m * w)) {
                    int idx = j - m * w;
                    String prev = s.substring(idx, idx + w);
                    int cnt = tmp.get(prev);
                    if (cnt == 1) tmp.remove(prev);
                    else tmp.put(prev, cnt - 1);
                    if (tmp.getOrDefault(prev, 0) != map.getOrDefault(prev, 0)) continue;
                }
                if (map.getOrDefault(cur, 0) != tmp.getOrDefault(cur, 0)) continue;
                if (map.equals(tmp)) ans.add(j - (m - 1) * w);
            }
        }
        return ans;
    }
}
```