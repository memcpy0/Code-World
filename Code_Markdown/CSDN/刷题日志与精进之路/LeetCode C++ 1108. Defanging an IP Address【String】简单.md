

Given a valid (IPv4) IP `address`, return a defanged version of that IP address.

A defanged IP address replaces every period `"."` with `"[.]"`.

 

**Example 1:**

```swift
Input: address = "1.1.1.1"
Output: "1[.]1[.]1[.]1"
```

**Example 2:**

```swift
Input: address = "255.100.50.0"
Output: "255[.]100[.]50[.]0"
```

 

**Constraints:** The given `address` is a valid IPv4 address.

题意：给出一个有效的IPv4地址 `address`，返回这个IP地址的无效化版本。

---
### 解法 辅助空间
```cpp
class Solution {
public:
    string defangIPaddr(string address) {
        string ans;
        int n = address.size();
        for (int i = 0; i < n; ++i) {
            if (address[i] == '.') ans.append("[.]");
            else ans.push_back(address[i]);
        }
        return ans;
    }
};
```
时间和空间都为 $O(n)$ ，运行效率如下：
```cpp
执行用时：0 ms, 在所有 C++ 提交中击败了100.00% 的用户
内存消耗：6.2 MB, 在所有 C++ 提交中击败了9.25% 的用户
```
 
