 <div class="notranslate"><p>The <strong>numeric value</strong> of a <strong>lowercase character</strong> is defined as its position <code>(1-indexed)</code> in the alphabet, so the numeric value of <code>a</code> is <code>1</code>, the numeric value of <code>b</code> is <code>2</code>, the numeric value of <code>c</code> is <code>3</code>, and so on.</p>

<p>The <strong>numeric value</strong> of a <strong>string</strong> consisting of lowercase characters is defined as the sum of its characters' numeric values. For example, the numeric value of the string <code>"abe"</code> is equal to <code>1 + 2 + 5 = 8</code>.</p>

<p>You are given two integers <code>n</code> and <code>k</code>. Return <em>the <strong>lexicographically smallest string</strong> with <strong>length</strong> equal to <code>n</code> and <strong>numeric value</strong> equal to <code>k</code>.</em></p>

<p>Note that a string <code>x</code> is lexicographically smaller than string <code>y</code> if <code>x</code> comes before <code>y</code> in dictionary order, that is, either <code>x</code> is a prefix of <code>y</code>, or if <code>i</code> is the first position such that <code>x[i] != y[i]</code>, then <code>x[i]</code> comes before <code>y[i]</code> in alphabetic order.</p>

**Example 1:**
```swift
Input: n = 3, k = 27
Output: "aay"
Explanation: The numeric value of the string is 1 + 1 + 25 = 27, and it is the smallest string with such a value and length equal to 3.
```

**Example 2:**
```swift
Input: n = 5, k = 73
Output: "aaszz"
```

 

**Constraints:** 
<ul>
	<li><code>1 &lt;= n &lt;= 10<sup>5</sup></code></li>
	<li><code>n &lt;= k &lt;= 26 * n</code></li>
</ul>
</div>



题意：<strong>小写字符</strong>的<strong>数值</strong>是它在字母表中的位置（从 <code>1</code> 开始）。字符串由若干小写字符组成，<strong>字符串的数值</strong>为各字符的数值之和。

<p>给定两个整数 <code>n</code> 和 <code>k</code> 。返回<strong>长度</strong>等于 <code>n</code> 且<strong>数值</strong>等于 <code>k</code> 的<strong>字典序最小</strong>的字符串。</p>

<p>如果字符串 <code>x</code> 在字典排序中位于 <code>y</code> 之前，就认为 <code>x</code> 字典序比 <code>y</code> 小，有以下两种情况：</p>

<ul>
	<li><code>x</code> 是 <code>y</code> 的一个前缀；</li>
	<li>如果 <code>i</code> 是&nbsp;<code>x[i] != y[i]</code> 的第一个位置，且 <code>x[i]</code>&nbsp;在字母表中的位置比&nbsp;<code>y[i]</code>&nbsp;靠前。</li>
</ul>

  
---
### 解法1 DFS+剪枝
由数据范围可知，如果对字符串的每个字符的每种可能都尝试一次，绝对会超时。不过采用剪枝剪去不必要的情况，可以通过这一题：
```cpp
class Solution {
private:
    string ans;
    bool dfs(int s, int e, int k) {
        if (s >= e) return k == 0; //n个字符时是否k为0
        if (k > (e - s) * 26) return false; //剪枝,如果剩下所有字符都选择最大的'z'也不能成功,直接返回
        for (int i = 1; i <= 26; ++i) { //i <= k <= (e - s) * 26
            ans.push_back(i + 'a' - 1);
            if (dfs(s + 1, e, k - i)) return true;
            ans.pop_back();
        }
        return false;
    }
public:
    string getSmallestString(int n, int k) { 
        dfs(0, n, k);
        return ans;
    }
};
```
执行效率如下，由于提交人数不多，因此不太准确：
```cpp
执行用时：632 ms, 在所有 C++ 提交中击败了100.00% 的用户
内存消耗：37.5 MB, 在所有 C++ 提交中击败了100.00% 的用户
```
---
### 解法2 贪心构造
这种做法是上述DFS+剪枝的迭代版，枚举字符串的每个字符的每种可能，加以剪枝：
```cpp
char str[101000];
class Solution {
public:
    string getSmallestString(int n, int k) { 
        for (int i = 0; i < n; ++i) {
            for (int j = 1; j <= 26; ++j) {
                if (k - j >= n - i - 1 && k - j <= (n - i - 1) * 26) {
                    str[i] = j + 'a' - 1;
                    k -= j;
                    break;
                }
            }
        }
        str[n] = 0;
        string ans = str;
        return ans;
    }
};
```

超时是因为当n太大时，计算z的位数的过程中如果是按位遍历，计算次数太多。其实一步就能完成，（k - n）/ 25。

首先假设所有位都为‘a’，然后从后往前不断地补‘z’，直到k为0
贪心从后往前取优先取大的
```swift
class Solution {
    public String getSmallestString(int n, int k) {       
        StringBuilder sb=new StringBuilder();
        String str="abcdefghijklmnopqrstuvwxyz";
        k-=n;
        for(int i=0;i<n;i++){
            int num=Math.min(k,25);
            sb.append(str.charAt(num));
            k-=num;
        }
        return sb.reverse().toString();
    }
}
```

