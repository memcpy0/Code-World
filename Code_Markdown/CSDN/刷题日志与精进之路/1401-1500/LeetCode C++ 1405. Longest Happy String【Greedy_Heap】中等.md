<p>A string is called <em>happy</em> if it does&nbsp;not have any of the strings <code>'aaa'</code>, <code>'bbb'</code>&nbsp;or <code>'ccc'</code>&nbsp;as a substring.</p>

<p>Given three integers <code>a</code>, <code>b</code> and <code>c</code>, return <strong>any</strong> string <code>s</code>,&nbsp;which satisfies following conditions:</p>

<ul>
	<li><code>s</code> is <em>happy&nbsp;</em>and longest possible.</li>
	<li><code>s</code> contains <strong>at most</strong> <code>a</code>&nbsp;occurrences of the letter&nbsp;<code>'a'</code>, <strong>at most</strong> <code>b</code>&nbsp;occurrences of the letter <code>'b'</code> and <strong>at most</strong> <code>c</code> occurrences of the letter <code>'c'</code>.</li>
	<li><code>s&nbsp;</code>will only contain <code>'a'</code>, <code>'b'</code> and <code>'c'</code>&nbsp;letters.</li>
</ul>

<p>If there is no such string <code>s</code>&nbsp;return the empty string <code>""</code>.</p>

 
<p><strong>Example 1:</strong></p>

```swift
Input: a = 1, b = 1, c = 7
Output: "ccaccbcc"
Explanation: "ccbccacc" would also be a correct answer.
```

 

<p><strong>Example 2:</strong></p>

```swift
Input: a = 2, b = 2, c = 1
Output: "aabbc"
```

<p><strong>Example 3:</strong></p>

```swift
Input: a = 7, b = 1, c = 0
Output: "aabaa"
Explanation: It's the only correct answer in this case. 
```
 
<p><strong>Constraints:</strong></p>

<ul>
	<li><code>0 &lt;= a, b, c &lt;= 100</code></li>
	<li><code>a + b + c &gt; 0</code></li>
</ul>


题意：如果字符串中不含有任何 <code>'aaa'</code>，<code>'bbb'</code> 或 <code>'ccc'</code> 这样的字符串作为子串，那么该字符串就是一个「快乐字符串」。

<p>给出三个整数 <code>a</code>，<code>b</code> ，<code>c</code>，返回<strong>任意一个</strong>满足下列全部条件的字符串 <code>s</code>：</p>

<ul>
	<li><code>s</code> 是一个尽可能长的快乐字符串。</li>
	<li><code>s</code> 中 <strong>最多</strong> 有 <code>a</code> 个字母 <code>'a'</code>、<code>b</code>&nbsp;个字母 <code>'b'</code>、<code>c</code> 个字母 <code>'c'</code> 。</li>
	<li><code>s </code>中只含有 <code>'a'</code>、<code>'b'</code> 、<code>'c'</code> 三种字母。</li>
</ul>

<p>如果不存在这样的字符串 <code>s</code> ，请返回一个空字符串 <code>""</code>。</p>

---
### 解法 贪心
```cpp
class Solution { 
public:
    string longestDiverseString(int a, int b, int c) {
        struct cmp {
            bool operator()(const pair<int, char>& a, const pair<int, char>& b) const {
                return a.first < b.first; //数量大的字符位于堆顶
            }
        };
        priority_queue<pair<int, char>, vector<pair<int, char>>, cmp> pq;
        if (a) pq.push(make_pair(a, 'a'));
        if (b) pq.push(make_pair(b, 'b'));
        if (c) pq.push(make_pair(c, 'c')); //当字符数量不是0时进入堆中
        string ans;
        char prev = 'd', beforePrev = 'd'; //作为前两个字符的标识
        while (!pq.empty()) {
            pair<int, char> t = pq.top(); pq.pop();
            if (t.second == prev && t.second == beforePrev) { //避免同样的字母连续三个
                if (pq.empty()) break; //退出
                pair<int, char> a = pq.top(); pq.pop();
                pq.push(t); 
                t = a; //更换字母,和原来的t.second以及前面两个字母一定不同
            }
            ans += t.second; //拿出数量最多的字母,添加一个到字符串末端
            t.first--;       //字母数量减1
            beforePrev = prev;
            prev = t.second;
            if (t.first > 0) pq.push(t); //字符数量仍然不为0
        }
        return ans;
    } //贪心：每次都选择剩下数量最多的字母添加
};
```
运行效率如下：
```cpp
执行用时：4 ms, 在所有 C++ 提交中击败了53.72% 的用户
内存消耗：6.3 MB, 在所有 C++ 提交中击败了70.17% 的用户
```
