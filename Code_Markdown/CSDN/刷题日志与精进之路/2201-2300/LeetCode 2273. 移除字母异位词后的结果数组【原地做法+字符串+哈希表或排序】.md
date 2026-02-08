> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

给你一个下标从 **0** 开始的字符串 `words` ，其中 `words[i]` 由小写英文字符组成。

在一步操作中，需要选出任一下标 `i` ，从 `words` 中 **删除** `words[i]` 。其中下标 `i` 需要同时满足下述两个条件：
1. `0 < i < words.length`
2. `words[i - 1]` 和 `words[i]` 是 **字母异位词** 。

只要可以选出满足条件的下标，就一直执行这个操作。

在执行所有操作后，返回 `words` 。可以证明，按任意顺序为每步操作选择下标都会得到相同的结果。

**字母异位词** 是由重新排列源单词的字母得到的一个新单词，所有源单词中的字母通常恰好只用一次。例如，`"dacb"` 是 `"abdc"` 的一个字母异位词。

**示例 1：**
```c
输入：words = ["abba","baba","bbaa","cd","cd"]
输出：["abba","cd"]
解释：
获取结果数组的方法之一是执行下述步骤：
- 由于 words[2] = "bbaa" 和 words[1] = "baba" 是字母异位词，选择下标 2 并删除 words[2] 。
  现在 words = ["abba","baba","cd","cd"] 。
- 由于 words[1] = "baba" 和 words[0] = "abba" 是字母异位词，选择下标 1 并删除 words[1] 。
  现在 words = ["abba","cd","cd"] 。
- 由于 words[2] = "cd" 和 words[1] = "cd" 是字母异位词，选择下标 2 并删除 words[2] 。
  现在 words = ["abba","cd"] 。
无法再执行任何操作，所以 ["abba","cd"] 是最终答案。
```
**示例 2：**
```c
输入：words = ["a","b","c","d","e"]
输出：["a","b","c","d","e"]
解释：
words 中不存在互为字母异位词的两个相邻字符串，所以无需执行任何操作。
```
**提示：**
- `1 <= words.length <= 100`
- `1 <= words[i].length <= 10`
- `words[i]` 由小写英文字母组成
---
### 方法 原地做法+字符串+哈希表/排序
类似[26. 删除有序数组中的重复项](https://leetcode.cn/problems/remove-duplicates-from-sorted-array/)，连续相同的字母异位词，只保留其中最左边的。

示例 1 的 $abba,baba,bbaa$ 互为字母异位词，保留最左边的 $abba$ ；$cd,cd$ 互为字母异位词，保留最左边的 $cd$ 。

根据题意，如果两个字符串每种字母的出现次数相同，就是一对字母异位词。可以用**哈希表**统计字母出现次数，也可以**把字符串排序**，两个排序后相同的字符串是一对字母异位词。其余逻辑同 26 题。
```cpp
class Solution {
public:
    vector<string> removeAnagrams(vector<string>& words) {
        string base = "";
        int k = 0;
        for (auto& word : words) {
            string s = word;
            ranges::sort(s);
            if (s != base) {
                base = move(s);
                words[k++] = word; // 保留 word
            }
        }
        words.resize(k);
        return words;
    }
};
```
```java
class Solution {
    public List<String> removeAnagrams(String[] words) {
        char[] base = words[0].toCharArray();
        Arrays.sort(base);
        int k = 1;
        for (int i = 1; i < words.length; i++) {
            char[] s = words[i].toCharArray();
            Arrays.sort(s);
            if (!Arrays.equals(s, base)) {
                base = s;
                words[k++] = words[i]; // 保留 words[i]
            }
        }
        return Arrays.asList(Arrays.copyOf(words, k));
    }
}
```
```rust
impl Solution {
    pub fn remove_anagrams(mut words: Vec<String>) -> Vec<String> {
        let mut base = vec![];
        let mut k = 0;
        for i in 0..words.len() {
            let mut s = words[i].as_bytes().to_vec();
            s.sort_unstable();
            if s != base {
                base = s;
                words[k] = words[i].clone(); // 保留 words[i]
                k += 1;
            }
        }
        words.truncate(k);
        words
    }
}
```
```go
func removeAnagrams(words []string) []string {
	var base []byte
	k := 0
	for _, word := range words {
		s := []byte(word)
		slices.Sort(s)
		if !bytes.Equal(s, base) {
			base = s
			words[k] = word // 保留 word
			k++
		}
	}
	return words[:k]
}
```
```python
class Solution:
    def removeAnagrams(self, words: List[str]) -> List[str]:
        k = 1
        for s, t in pairwise(words):
            if sorted(s) != sorted(t):
                words[k] = t
                k += 1
        del words[k:]
        return words
# 写法二
class Solution:
    def removeAnagrams(self, words: List[str]) -> List[str]:
        k = 1
        for s, t in pairwise(words):
            if Counter(s) != Counter(t):
                words[k] = t
                k += 1
        del words[k:]
        return words
```
```c
int cmp(const void* a, const void* b) {
    return (*(char*)a - *(char*)b);
}

char ** removeAnagrams(char ** words, int wordsSize, int* returnSize){
    char* base = strdup(words[0]);
    qsort(base, strlen(base), sizeof(char), cmp);
    int k = 1;
    for (int i = 1; i < wordsSize; i++) {
        char* s = strdup(words[i]);
        qsort(s, strlen(s), sizeof(char), cmp);
        if (strcmp(s, base)) {
            free(base);
            base = s;
            words[k++] = words[i]; // 保留 words[i]
        } else {
            free(s);
        }
    }
    free(base);
    *returnSize = k;
    return words;
}
```
```js
var removeAnagrams = function(words) {
    let base = '';
    let k = 0;
    for (const word of words) {
        const s = word.split('').sort().join('');
        if (s !== base) {
            base = s;
            words[k++] = word; // 保留 word
        }
    }
    words.length = k;
    return words;
};
```
复杂度分析：
- 时间复杂度：$O(nL)$ 或 $O(nL \log L)$ ，其中 $n$ 是 $words$ 的长度，$L\le 10$ 是 $words[i]$ 的最大长度。
- 空间复杂度：$O(L)$ 。