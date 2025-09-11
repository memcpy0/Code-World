Given an array `A` of strings made only from lowercase letters, return a list of all characters that **show up in all strings** within the list (**including duplicates**).  For example, if a character occurs `3` times in all strings but not `4` times, you need to include that character **three times** in the final answer.

You may return the answer in any order.

 

Example 1:
```swift
Input: ["bella","label","roller"]
Output: ["e","l","l"]
```
Example 2:
```swift
Input: ["cool","lock","cook"]
Output: ["c","o"]
```
Note:
- `1 <= A.length <= 100`
- `1 <= A[i].length <= 100`
- `A[i][j]` is a lowercase letter

题意：求出多个字符串的字符交集，允许重复字符。

思路：以第一个例子为例，我们知道第一个字符串 `bella` 的字符数量列表为：
```swift
b 1
e 1
l 2
a 1
```
第二个字符串 `label` 的字符数量列表为：
```swift
l 2
a 1
b 1
e 1
```
第三个字符串 `roller` 的字符数量列表为：
```swift
r 2
o 1
l 2
e 1
```
这三个求交集后的结果为：
```swift
e 1
l 2
```
我们可以用哈希表+计数索引来表示**字符-数量**之间的关系，考虑到效率直接用数组即可。这个题目基本是 `350. Intersection of Two Arrays II` 的升级版，只是不是两个数组而是多个字符数组。

代码中用到了一个不太常用的 `string` 构造函数：
```cpp
class Solution {
public:
    vector<string> commonChars(vector<string>& A) {
        vector<string> ans;
        int cnt[26] = {0};
        for (char c : A[0]) ++cnt[c - 'a'];
        for (int i = 1; i < A.size(); ++i) {
            int tmp[26] = {0};
            for (char c : A[i]) ++tmp[c - 'a'];
            for (int j = 0; j < 26; ++j)
                cnt[j] = min(cnt[j], tmp[j]);  //每两个字符串之间求出字符交集数量
        }
        for (int i = 0; i < 26; ++i) {
            if (cnt[i]) {
                for (int j = 0; j < cnt[i]; ++j) {
                    ans.push_back(string(1, 'a' + i));
                }
            }
        }
        return ans;
    }
};
```
效率：
```
执行用时：16 ms, 在所有 C++ 提交中击败了61.26% 的用户
内存消耗：9.1 MB, 在所有 C++ 提交中击败了100.00% 的用户
```
