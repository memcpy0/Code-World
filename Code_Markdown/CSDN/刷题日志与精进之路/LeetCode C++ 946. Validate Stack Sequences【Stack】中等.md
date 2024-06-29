<p>Given two sequences <code>pushed</code> and <code>popped</code>&nbsp;<strong>with distinct values</strong>,&nbsp;return <code>true</code> if and only if this could have been the result of a sequence of push and pop operations on an initially empty stack.</p>

 
<div>
<p><strong>Example 1:</strong></p>

```swift
Input: pushed = [1,2,3,4,5], popped = [4,5,3,2,1]
Output: true
Explanation: We might do the following sequence:
push(1), push(2), push(3), push(4), pop() -> 4,
push(5), pop() -> 5, pop() -> 3, pop() -> 2, pop() -> 1 
```

 
<p><strong>Example 2:</strong></p>

```swift
Input: pushed = [1,2,3,4,5], popped = [4,3,5,1,2]
Output: false
Explanation: 1 cannot be popped before 2. 
```

  
<p><strong>Constraints:</strong></p>

<ul>
	<li><code>0 &lt;= pushed.length == popped.length &lt;= 1000</code></li>
	<li><code>0 &lt;= pushed[i], popped[i] &lt; 1000</code></li>
	<li><code>pushed</code> is a permutation of <code>popped</code>.</li>
	<li><code>pushed</code> and <code>popped</code> have distinct values.</li>
</ul>

题意：给定 `pushed` 和 `popped` 两个序列，**每个序列中的值都不重复**，只有当它们可能是在最初空栈上进行的推入 `push` 和弹出 `pop` 操作序列的结果时，返回 `true`；否则，返回 `false` 。

---
### 解法 栈
```cpp
class Solution {
public:
    bool validateStackSequences(vector<int>& pushed, vector<int>& popped) {
        stack<int> st;
        int size = pushed.size(), j = 0;
        for (int i = 0; i < size; ++i) {
            st.push(pushed[i]); //无需判断j<size, 因为j>=size时栈一定为空
            while (!st.empty() && st.top() == popped[j]) { ++j; st.pop(); } 
        }
        return st.empty(); //等价于j>=size
    }
};
```
提交后运行效率如下：
```cpp
执行用时：24 ms, 在所有 C++ 提交中击败了35.86% 的用户
内存消耗：15.3 MB, 在所有 C++ 提交中击败了52.14% 的用户
```
