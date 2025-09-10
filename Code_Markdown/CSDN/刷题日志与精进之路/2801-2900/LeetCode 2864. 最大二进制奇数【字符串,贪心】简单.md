> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

给你一个 **二进制** 字符串 `s` ，其中至少包含一个 `'1'` 。

你必须按某种方式 **重新排列** 字符串中的位，使得到的二进制数字是可以由该组合生成的 **最大二进制奇数** 。

以字符串形式，表示并返回可以由给定组合生成的最大二进制奇数。

**注意** 返回的结果字符串 **可以** 含前导零。

**示例 1：**
```js
输入：s = "010"
输出："001"
解释：因为字符串 s 中仅有一个 '1' ，其必须出现在最后一位上。所以答案是 "001" 。
```
**示例 2：**
```js
输入：s = "0101"
输出："1001"
解释：其中一个 '1' 必须出现在最后一位上。而由剩下的数字可以生产的最大数字是 "100" 。所以答案是 "1001" 。
```
**提示：**
- `1 <= s.length <= 100`
- `s` 仅由 `'0'` 和 `'1'` 组成
- `s` 中至少包含一个 `'1'`

---
### 解法 贪心
把一个 $1$ 放末尾，其余全部放在开头。
```cpp
class Solution {
public:
    string maximumOddBinaryNumber(string s) {
        int cnt1 = count(s.begin(), s.end(), '1');
        return string(cnt1 - 1, '1') + string(s.length() - cnt1, '0') + '1';
    }
};
```

```python
class Solution:
    def maximumOddBinaryNumber(self, s: str) -> str:
        cnt1 = s.count('1')
        return '1' * (cnt1 - 1) + '0' * (len(s) - cnt1) + '1'
```

```javascript
/**
 * @param {string} s
 * @return {string}
 */
var maximumOddBinaryNumber = function(s) {
    let cnt1 = _.sumBy(s, c => c === '1')
    return '1'.repeat(cnt1 - 1) + '0'.repeat(s.length - cnt1) + '1';
};
```

```go
func maximumOddBinaryNumber(s string) string {
    cnt1 := strings.Count(s, "1")
    return strings.Repeat("1", cnt1 - 1) + strings.Repeat("0", len(s) - cnt1) + "1"
}
```

```java
class Solution {
    public String maximumOddBinaryNumber(String s) {
        int cnt1 = (int) s.chars().filter(c -> c == '1').count();
        return "1".repeat(cnt1 - 1) + "0".repeat(s.length() - cnt1) + "1";
    }
}
```

```rust
impl Solution {
    pub fn maximum_odd_binary_number(s: String) -> String {
        let cnt1 = s.chars().filter( |&c| c == '1').count();
        "1".repeat(cnt1 - 1) + &*"0".repeat(s.len() - cnt1) + "1"
    }
}
```
复杂度分析：
- 时间复杂度：$\mathcal{O}(n)$ ，其中 $n$ 为 $s$ 的长度。
- 空间复杂度：$\mathcal{O}(1)$ 。返回值不计入。
