> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

Given an integer, write an algorithm to convert it to hexadecimal. For negative integer, two’s complement method is used.

**Note:**
1. All letters in hexadecimal (`a-f`) must be in lowercase.
2. The hexadecimal string must not contain extra leading `0` s. If the number is zero, it is represented by a single zero character `'0'` ; otherwise, the first character in the hexadecimal string will not be the zero character.
3. The given number is guaranteed to fit within the range of a 32-bit signed integer.
4. You **must not use any method provided by the library** which converts/formats the number to hex directly.

**Example 1:**

```swift
Input:
26

Output:
"1a"
```
**Example 2:**
```swift
Input:
-1

Output:
"ffffffff"
```

题意：给定一个整数，编写一个算法将这个数转换为十六进制数。

---
### 解法 位操作
本题相当简单。只要不断使用 `num & 0xf` 获取 `num` 二进制表示的低 `4` 位，并映射为 `1` 位十六进制数字，然后 `num >>= 4` 右移四位，如此重复直到 `num` 为零为止。

然而比较隐秘的问题是，C++的 `>>` 是算术右移，对正整数右移左边补 `0` ，对负整数右移左边补 `1`，因此**有符号整数的右移运算**不能保证 `num` 最终为零，需要注意 `num` 为负整数的情况：
```cpp
//C++ version
class Solution {
private:
    char hex[17] = "0123456789abcdef";
public:
    string toHex(int num) {
        string ans;
        for (int i = 0; i < 8; ++i) {
            ans.push_back(hex[num & 0xf]);
            num >>= 4;
            if (num == 0) break; //正整数右移最终令num==0,负整数依赖32位处理
        }
        reverse(ans.begin(), ans.end());
        return ans;
    }
};
//执行用时：0 ms, 在所有 C++ 提交中击败了100.00% 的用户
//内存消耗：6 MB, 在所有 C++ 提交中击败了43.53% 的用户
```
或者将有符号整数 `num` 转换为无符号整数类型 `uint32_t` ，就不用依靠 `for` 循环右移 `8` 次了：
```cpp
//C++ version
class Solution {
private:
    char hex[17] = "0123456789abcdef";
public:
    string toHex(int num) {
        string ans;
        uint32_t n = static_cast<uint32_t>(num);
        do {
            ans.push_back(hex[n & 0xf]);
            n >>= 4;
        } while (n);
        reverse(ans.begin(), ans.end());
        return ans;
    }
};
//执行用时：0 ms, 在所有 C++ 提交中击败了100.00% 的用户
//内存消耗：5.9 MB, 在所有 C++ 提交中击败了43.11% 的用户
```

