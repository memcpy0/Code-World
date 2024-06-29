<div class="content__1Y2H"><div class="notranslate"><p>In a array <code>A</code> of size <code>2N</code>, there are <code>N+1</code> unique elements, and exactly one of these elements is repeated <code>N</code> times.</p> 
<p>Return the element repeated <code>N</code> times.</p>

<div>
<p><strong>Example 1:</strong></p>

```cpp
Input: [1,2,3,3]
Output: 3 
```

<div>
<p><strong>Example 2:</strong></p>

```cpp
Input: [2,1,2,5,3,2]
Output: 2
```

<div>
<p><strong>Example 3:</strong></p>

```cpp
Input: [5,1,5,2,5,3,5,4]
Output: 5
```

<p><strong>Note:</strong></p>

<ul>
	<li><code>4 &lt;= A.length &lt;= 10000</code></li>
	<li><code>0 &lt;= A[i] &lt; 10000</code></li>
	<li><code>A.length</code> is even</li>
</ul>
</div>
</div>
</div>
</div></div>

题意：给出一个 `2N` 的数组，其中有 `N + 1` 个不同的元素，某个元素重复出现了 `N` 次。返回这个元素。

--- 
### 思路1：哈希表
使用布尔数组，如果发现有一个元素再次出现 ，就返回：
```cpp
class Solution {
public:
    int repeatedNTimes(vector<int>& A) { 
        int ans;
        vector<bool> appear(10010);
        for (const int &v : A) {
            if (appear[v]) {
                ans = v;
                break;
            } 
            appear[v] = true;
        }
        return ans;
    }
};
```
效率如下：
```cpp
执行用时：96 ms, 在所有 C++ 提交中击败了40.46% 的用户
内存消耗：24.5 MB, 在所有 C++ 提交中击败了23.85% 的用户
```
---
### 思路2：排列讨论
对于一个长度为 `2N` 的数组，其中有 `N` 个数字相等，则相当于在 `N + 1` 个间隔中放入 `N` 个相同的数字，于是只有两种情况：
- 排列中要么所有相同的数都不相邻；
- 要么必定存在相邻且相等的情形。

比如有三个数排成一圈，要往排列里插入三个相同的数，相同的数相隔的**最小距离要么是** `1` (相邻)，要么是 `2` (间隔一个数字)：
```cpp
_a_b_c_
^ ^ ^ ^
//更多的数
_a_b_c_d_
```
三个数字有点多了，看看 `2` 个相同的数的所有情况，此时 `A.length = 4` ：
```cpp
1 2 3 3
1 3 2 3
1 3 3 2
2 1 3 3
2 3 1 3
2 3 3 1
3 1 2 3 ×
3 1 3 2
3 2 1 3 ×
3 2 3 1
3 3 1 2
3 3 2 1  
```
发现有点问题，即在 `2` 个相同的数时，存在特殊情形。弥补一下，也比较一下距离为 `3` 的数值：
```cpp
class Solution {
public:
    int repeatedNTimes(vector<int>& A) { 
        int n = A.size();
        for (int i = 0; i < n; ++i) {
            if (i < 1) continue;
            if (A[i] == A[i - 1]) return A[i];
            if (i < 2) continue; 
            if (A[i] == A[i - 2]) return A[i];
            if (i < 3) continue; 
            if (A[i] == A[i - 3]) return A[i];
        } 
        return 0;
    }
};
```
提交后效率微有上升，不过分支太多，可能影响分支预测：
```cpp
执行用时：92 ms, 在所有 C++ 提交中击败了42.74% 的用户
内存消耗：24.3 MB, 在所有 C++ 提交中击败了54.34% 的用户
```
为此，**把数组看作一个环，则距离为 `2` 以内的数一定会出现重复值**，例如：
```js
x x a b (环状下，同 axxb, abxx, xabx 等情况相同，距离为1）
a x b x (环状下，同 xaxb 相同，距离为2）
```
得出的代码如下：
```cpp
class Solution {
public:
    int repeatedNTimes(vector<int>& A) { 
        int n = A.size(); 
        for (int i = 0; i < n; ++i)
            if (A[i] == A[(i - 1 + n) % n] || A[i] == A[(i - 2 + n) % n])
                return A[i]; 
        return 0;
    }
};
```
效率如下：
```cpp
执行用时：64 ms, 在所有 C++ 提交中击败了86.63% 的用户
内存消耗：24.2 MB, 在所有 C++ 提交中击败了69.51% 的用户
```
顺带一提，如果用Java提交的话，emmm：
```java
class Solution { 
    public int repeatedNTimes(int[] A) { 
        int n = A.length; 
        for (int i = 0; i < n; ++i)
            if (A[i] == A[(i - 1 + n) % n] || A[i] == A[(i - 2 + n) % n])
                return A[i]; 
        return 0;
    }
}
```
效率瞬间爆涨，让人有点不真实的感觉：
```cpp
执行用时：0 ms, 在所有 Java 提交中击败了100.00% 的用户
内存消耗：40.3 MB, 在所有 Java 提交中击败了5.09% 的用户
```
