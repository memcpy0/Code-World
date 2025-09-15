> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

小红和小明在玩一个字符串元音游戏。

给你一个字符串 `s`，小红和小明将轮流参与游戏，小红 **先** 开始：
- 在小红的回合，她必须移除 `s` 中包含 **奇数** 个元音的任意 **非空** 子字符串。
- 在小明的回合，他必须移除 `s` 中包含 **偶数** 个元音的任意 **非空** 子字符串。

第一个无法在其回合内进行移除操作的玩家输掉游戏。假设小红和小明都采取 **最优策略** 。

如果小红赢得游戏，返回 `true`，否则返回 `false`。

英文元音字母包括：`a`, `e`, `i`, `o`, 和 `u`。

**示例 1：**
```c
输入： s = "leetcoder"
输出： true
解释：  
小红可以执行如下移除操作来赢得游戏：
- 小红先手，她可以移除加下划线的子字符串 s = "leetcoder"，其中包含 3 个元音。结果字符串为 s = "der" 。
- 小明接着，他可以移除加下划线的子字符串 s = "der" ，其中包含 0 个元音。结果字符串为 s = "er" 。
- 小红再次操作，她可以移除整个字符串 s = "er" ，其中包含 1 个元音。
- 又轮到小明，由于字符串为空，无法执行移除操作，因此小红赢得游戏。
```
**示例 2：**
```c
输入： s = "bbcd"
输出： false
解释：  
小红在她的第一回合无法执行移除操作，因此小红输掉了游戏。
```
**提示：**
- `1 <= s.length <= 10^5`
- `s` 仅由小写英文字母组成。

### 方法 脑筋急转弯
分类讨论：
- 如果 $s$ 不包含任何元音，小红输；
- 如果 $s$ 包含奇数个元音，小红可以直接把整个 $s$ 移除，小红赢；
- 如果 $s$ 包含正偶数个元音，由于**偶数减奇数等于奇数**，小红移除任何包含奇数个元音的子串后，剩下元音个数仍然为奇数，由于**奇数减偶数还是奇数**，所以无论小明如何操作，仍然会剩下奇数个元音，此时小红可以直接把整个 $s$ 移除，小红赢。

所以，只要 $s$ 包含至少一个元音，就返回 $true$ ，否则返回 $false$ 。
```java
class Solution {
    public boolean doesAliceWin(String s) {
        for (char c : s.toCharArray()) {
            if (c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u') {
                return true;
            }
        }
        return false;
    }
}
```
```cpp
class Solution {
public:
    bool doesAliceWin(string s) {
        return ranges::any_of(s, [](char c) {
            return c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u';
        });
    }
};
```
```rust
impl Solution {
    pub fn does_alice_win(s: String) -> bool {
        s.bytes().any(|c| "aeiou".contains(c as char))
    }
}
```
```python
class Solution:
    def doesAliceWin(self, s: str) -> bool:
        return any(c in s for c in "aeiou")
        
class Solution:
    def doesAliceWin(self, s: str) -> bool:
        return any(c in "aeiou" for c in s)
```
```c
bool doesAliceWin(char* s) {
    for (int i = 0; s[i]; i++) {
        char c = s[i];
        if (c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u') {
            return true;
        }
    }
    return false;
}
```
```go
func doesAliceWin(s string) bool {
	return strings.ContainsAny(s, "aeiou")
}
```
```javascript
var doesAliceWin = function(s) {
    return /[aeiou]/.test(s);
};
```
复杂度分析：
- 时间复杂度：$O(n)$ ，其中 $n$ 是 $s$ 的长度。
- 空间复杂度：$O(1)$ 。

### 思考题
如果小明先手呢？
答：
- 如果 $s$ 不包含任何元音，小明赢；
- 如果 $s$ 包含偶数个元音，小明赢；
- 如果 $s$ 包含奇数个元音，小明取走偶数个以后，还是奇数个元音，因此小红赢。