Given a string `s` , you are allowed to convert it to a palindrome by adding characters in front of it. Find and return the shortest palindrome you can find by performing this transformation.

**Example 1:**
```cpp
Input: "aacecaaa"
Output: "aaacecaaa"
```
**Example 2:**
```cpp
Input: "abcd"
Output: "dcbabcd" 
```

题意：给定一个字符串 `s` ，通过在字符串前面添加字符将其转换为回文串。找到并返回可以用这种方式转换的**最短回文串**。

---
思路1：使用Manacher算法。这一题的实质是**查找在必须包含第一个字符的情况下，最长的回文子串是什么**。<font color="red">**把之后不是最长回文子串的部分逆序过来，就是应该添加在前面的部分**。</font>最后，将这一部分添加在原字符串前面，返回即可。

如 `"123321abcd"` ，必须包含第一个字符的情况下，最长回文子串是 `"123321"` ，之后不是最长回文子串的部分是 `"abcd"` ，把它逆序过来 `"dcba"` 添加到前面即可得到：`dcba123321abcd` 。

修改Manacher算法：从左到右计算回文半径时，关注回文半径最左即将到达的位置 `i - pArr[i]` ，一旦发现其 `< 0` ，说明**这一回文串包含第一个字符**，记录这些回文子串半径的最大值为 `maxContainsBegin` 。于是原串包含第一个字符的最长回文子串的长度是 `maxContainsBegin - 1` 。要添加在前面的部分的长度为 `s.size() - maxContainsBegin + 1` 。代码如下：
```cpp
class Solution {
public:
    string shortestPalindrome(string s) {
        if (s.size() <= 1) return s;
        int len = s.size() * 2 + 1, idx = 0;
        string str;
        for (int i = 0; i < len; ++i)
            str.push_back(i & 1 ? s[idx++] : '#');
        int *pArr = new int[len];   //记录以i位置为中心的最大回文半径
        int pR = -1;                //记录之前遍历的所有回文半径中,最右即将到达的下一个位置
        int index = -1;             //记录最近一次pR更新时,那个回文中心的位置
        int maxContainsBegin = -1;  //记录必须包含首个字符时最长回文子串的半径
        for (int i = 0; i < len; ++i) {
            pArr[i] = pR > i ? min(pArr[2 * index - i], pR - i) : 1;
            while (i + pArr[i] < len && i - pArr[i] >= 0) {
                if (str[i + pArr[i]] == str[i - pArr[i]]) 
                    ++pArr[i];
                else break;
            }
            if (i + pArr[i] > pR) {
                pR = i + pArr[i];
                index = i;
            } 
            if (i - pArr[i] < 0) maxContainsBegin = max(maxContainsBegin, pArr[i]);
        } 
        int ansLen = s.size() - maxContainsBegin + 1;   //应该添加的字符串的长度
        if (ansLen <= 0) return s;                      //不需要添加时,直接返回原串
        string ans(ansLen, ' ');
        for (int i = 0; i < ansLen; ++i)
            ans[i] = s[s.size() - i - 1];
        return ans + s; 
    }
};
```
效率如下：
```cpp
执行用时：8 ms, 在所有 C++ 提交中击败了52.10% 的用户
内存消耗：7.9 MB, 在所有 C++ 提交中击败了59.01% 的用户
```
---
思路2：不用Manacher算法，而是**中心扩展法的复杂变形**。以前写的中心扩展法是 `count(s, i, i); count(s, i, i + 1);` 这种，这里针对题意进行了优化。代码如下：
```cpp
class Solution {
public:
	string shortestPalindrome(string s) {
		if (s.size() <= 1) return s;
		//maxContainsBegin记录包含第一个字符的最长回文子串结束位置
		int maxContainsBegin = 0, slen = s.size();
		for (int i = 0; i < slen; ++i) {
			//起始位置(中心扩展法)
			int begin = i;
			//找到连续相等字符的起始和结束位置(作为中心扩展法的整体中心)
			while (i + 1 < slen && s[i] == s[i + 1])
				++i;
			//结束位置(中心扩展法)
			int end = i;
			//判断是否已经不可能构成回文子串了
			//需要从0开始,以begin到end为回文中心构建回文子串
			//所以当begin+end大于数组长度时,可认定无法构成回文字符串,
			//其实遍历差不多只会到length/2的位置,发现无法构成时就会退出循环
			if (begin + end > slen - 1) 
				break;
			//检查以[begin,end]为中心、包含首位字符的子串是否是回文子串,是的话更新maxContainsBegin
			if (check(s, begin, end)) 
				maxContainsBegin = begin + end; //此回文子串的长度end+begin
		}
		//截取后续字符串,翻转拼接到字符串前面
		string str = s.substr(maxContainsBegin + 1);
		reverse(str.begin(), str.end());
		return str + s;
	}
	bool check(const string &s, int begin, int end) {
		//可以从begin~end往两头扩展,同时从两头往中间比较,更快
		for (int left = 0, right = end + begin; left <= begin && end <= right; ) {
			if (s[left] != s[right] || s[begin] != s[end]) 
				return false;
			++left;
			--right;
			--begin;
			++end;
		}
		return true;
	}
};
```
这一方法的优点：不像Manacher一样使用大量额外的空间；利用**必须包含首位字符**的特点，判断可能的回文子串长度是否超过 `s` 的长度以便及时退出，优化回文子串的判断等。最后效率如下，大概在 `0ms~4ms` 之间：
```cpp
执行用时：0 ms, 在所有 C++ 提交中击败了100.00% 的用户
内存消耗：7.3 MB, 在所有 C++ 提交中击败了98.50% 的用户
```
---
思路3：KMP算法。**需要在逆序的 `s` 中搜索模式串 `s`** ，得到 `s` 中从左到右的最长回文子串的长度，再取后面非回文串的逆序添加到字符串 `s` 的前面。例如：`s = "aabbaad", reversed_s = "daabbaa"` ，于是在 `reversed_s[1:]` 位置处 `reversed_s` 和 `s[0:6)` 形成部分匹配，即 `aabbaa` 是 `s` 中从左到右的最长回文子串，于是取最后的 `d` (的逆序)添加到字符串 `s` 的前面作为答案。
 
> 扩展一下：如果要添加到后面形成最短回文串，**应该在 `s` 中搜索逆序的 `s`** ，得到 `s` 中从右到左最长回文子串的长度，再取前面的非回文串的逆序加到后面即可。

具体代码如下：
```cpp
class Solution {
public:
	string shortestPalindrome(string s) {
		if (s.size() <= 1) return s;
		//计算kmp算法中的next数组(模式串s)
		vector<int> next(s.size());
		next[0] = -1, next[1] = 0;
		int pos = 2, cn = 0;
		while (pos < s.size()) {
			if (s[pos - 1] == s[cn]) next[pos++] = ++cn;
			else if (cn > 0) cn = next[cn];
			else next[pos++] = cn;
		}
		//kmp算法在倒序的s中寻找s, j就是s中包含首位字符的最长回文串的长度,下标i对应逆序s,下标j对应模式串s
		string reversed_s(s.rbegin(), s.rend());
		int i = 0, j = 0;
		while (i < s.size() && j < s.size()) {
			if (reversed_s[i] == s[j]) ++i, ++j;
			else if (next[j] != -1) j = next[j];
			else ++i;
		}
		string add = s.substr(j);
		reverse(add.begin(), add.end());
		return add + s;
	}
};
```
效率如下，和Manacher算法差不多：
```cpp
执行用时：8 ms, 在所有 C++ 提交中击败了52.10% 的用户
内存消耗：7.8 MB, 在所有 C++ 提交中击败了62.87% 的用户
```
复习另一个版本的KMP：
```cpp
class Solution {
public:
	string shortestPalindrome(string s) {
		if (s.size() <= 1) return s;
		//计算kmp算法中的next数组(模式串s)
		vector<int> next(s.size());
		next[0] = 0, next[1] = 0;
		for (int i = 1; i < s.size() - 1; ++i) {
			int j = next[i];
			while (j && s[i] != s[j]) j = next[j];
			next[i + 1] = (s[i] == s[j] ? j + 1 : 0);
		}
		//kmp算法在倒序的s中寻找s, j + 1就是s中包含首位字符的最长回文串的长度,下标i对应逆序s,下标j对应模式串s
		string reversed_s(s.rbegin(), s.rend());
		int j = 0;
		for (int i = 0; i < s.size(); ++i) {
			while (j && reversed_s[i] != s[j]) j = next[j];
			if (reversed_s[i] == s[j]) ++j;
		}
		string add = s.substr(j);
		reverse(add.begin(), add.end());
		return add + s;
	}
}; 
```
这样写提交时，得到了下面的结果，差不多：
```cpp
执行用时：8 ms, 在所有 C++ 提交中击败了52.10% 的用户
内存消耗：7.7 MB, 在所有 C++ 提交中击败了65.82% 的用户
```
