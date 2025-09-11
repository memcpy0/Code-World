<p>You are given a phone number as a string <code>number</code>. <code>number</code> consists of digits, spaces <code>' '</code>, and/or dashes <code>'-'</code>.</p>

<p>You would like to reformat the phone number in a certain manner. Firstly, <strong>remove</strong> all spaces and dashes. Then, <strong>group</strong> the digits from left to right into blocks of length 3 <strong>until</strong> there are 4 or fewer digits. The final digits are then grouped as follows:</p>

<ul>
	<li>2 digits: A single block of length 2.</li>
	<li>3 digits: A single block of length 3.</li>
	<li>4 digits: Two blocks of length 2 each.</li>
</ul>

<p>The blocks are then joined by dashes. Notice that the reformatting process should <strong>never</strong> produce any blocks of length 1 and produce <strong>at most</strong> two blocks of length 2.</p>

<p>Return <em>the phone number after formatting.</em></p>

 
<p><strong>Example 1:</strong></p>
 

```cpp
Input: number = "1-23-45 6"
Output: "123-456"
Explanation: The digits are "123456".
Step 1: There are more than 4 digits, so group the next 3 digits. The 1st block is "123".
Step 2: There are 3 digits remaining, so put them in a single block of length 3. The 2nd block is "456".
Joining the blocks gives "123-456".
```

 
<p><strong>Example 2:</strong></p>

```cpp
Input: number = "123 4-567"
Output: "123-45-67"
Explanation: The digits are "1234567".
Step 1: There are more than 4 digits, so group the next 3 digits. The 1st block is "123".
Step 2: There are 4 digits left, so split them into two blocks of length 2. The blocks are "45" and "67".
Joining the blocks gives "123-45-67".
```

 
<p><strong>Example 3:</strong></p>

```cpp
Input: number = "123 4-5678"
Output: "123-456-78"
Explanation: The digits are "12345678".
Step 1: The 1st block is "123".
Step 2: The 2nd block is "456".
Step 3: There are 2 digits left, so put them in a single block of length 2. The 3rd block is "78".
Joining the blocks gives "123-456-78".
```

 
<p><strong>Example 4:</strong></p>

```cpp
Input: number = "12"
Output: "12" 
```

<p><strong>Example 5:</strong></p>

```cpp
Input: number = "--17-5 229 35-39475 "
Output: "175-229-353-94-75" 
```

<p><strong>Constraints:</strong></p>

<ul>
	<li><code>2 &lt;= number.length &lt;= 100</code></li>
	<li><code>number</code> consists of digits&nbsp;and the characters <code>'-'</code> and <code>' '</code>.</li>
	<li>There are at least <strong>two</strong> digits in <code>number</code>.</li>
</ul>




题意：给出一个字符串形式的电话号码 <code>number</code> 。<code>number</code> 由数字、空格 <code>' '</code>、和破折号 <code>'-'</code> 组成。 

<p>按下述方式重新格式化电话号码。</p>
<ul>
	<li>首先，<strong>删除</strong> 所有的空格和破折号。</li>
	<li>其次，将数组从左到右 <strong>每 3 个一组</strong> 分块，<strong>直到 </strong>剩下 4 个或更少数字。剩下的数字将按下述规定再分块：
	<ul>
		<li>2 个数字：分为单个含 2 个数字的块。</li>
		<li>3 个数字：分为单个含 3 个数字的块。</li>
		<li>4 个数字：分为两个分别含 2 个数字的块。</li>
	</ul>
	</li>
</ul>

<p>最后用破折号将这些块连接起来。注意，重新格式化过程中 <strong>不应该</strong> 生成仅含 1 个数字的块，并且 <strong>最多</strong> 生成两个含 2 个数字的块。返回格式化后的电话号码。</p>

---
### 解法 顺序遍历
先删除掉所有的空格和破折号 `spaces and dashes` ，然后遍历整个字符串，根据剩下的数字字符的长度，决定使用哪一条规则：
```cpp
class Solution {
public:
    string reformatNumber(string number) {
        string s, ans;
        for (const char &c : number) if (isdigit(c)) s.push_back(c);
        int n = s.size();
        for (int i = 0; i < n; ) {
            if (n - i > 4) { 
                ans += s.substr(i, 3) + "-";
                i += 3;
            } else if (n - i == 4) { //剩下的分为两个块
                ans += s.substr(i, 2) + "-" + s.substr(i + 2);
                break;
            } else if (n - i >= 2) { //分为一个块
                ans += s.substr(i); 
                break; 
            }
        }
        return ans;
    }
};
```
提交后效率如下：
```cpp
执行用时：0 ms, 在所有 C++ 提交中击败了100.00% 的用户
内存消耗：6.3 MB, 在所有 C++ 提交中击败了85.81% 的用户
```
