> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。
 
You are given a string `s` of **even length** consisting of digits from `0` to `9`, and two integers `a` and `b`.

You can apply either of the following two operations any number of times and in any order on `s`:

-   Add `a` to all odd indices of `s` **(0-indexed)**. Digits post `9` are cycled back to `0`. For example, if `s = "3456"` and `a = 5`, `s` becomes `"3951"`.
-   Rotate `s` to the right by `b` positions. For example, if `s = "3456"` and `b = 1`, `s` becomes `"6345"`.

Return _the **lexicographically smallest** string you can obtain by applying the above operations any number of times on_ `s`.

A string `a` is lexicographically smaller than a string `b` (of the same length) if in the first position where `a` and `b` differ, string `a` has a letter that appears earlier in the alphabet than the corresponding letter in `b`. For example, `"0158"` is lexicographically smaller than `"0190"` because the first position they differ is at the third letter, and `'5'` comes before `'9'`.

**Example 1:**
```java
Input: s = "5525", a = 9, b = 2
Output: "2050"
Explanation: We can apply the following operations:
Start:  "5525"
Rotate: "2555"
Add:    "2454"
Add:    "2353"
Rotate: "5323"
Add:    "5222"
Add:    "5121"
Rotate: "2151"
​​​​​​​Add:    "2050"​​​​​​​​​​​​
There is no way to obtain a string that is lexicographically smaller then "2050".
```
**Example 2:**
```java
Input: s = "74", a = 5, b = 1
Output: "24"
Explanation: We can apply the following operations:
Start:  "74"
Rotate: "47"
​​​​​​​Add:    "42"
​​​​​​​Rotate: "24"​​​​​​​​​​​​
There is no way to obtain a string that is lexicographically smaller then "24".
```
**Example 3:**
```java
Input: s = "0011", a = 4, b = 2
Output: "0011"
Explanation: There are no sequence of operations that will give us a lexicographically smaller string than "0011".
```
**Constraints:**
-   `2 <= s.length <= 100`
-   `s.length` is even.
-   `s` consists of digits from `0` to `9` only.
-   `1 <= a <= 9`
-   `1 <= b <= s.length - 1`

题意：给出一个字符串 `s` 以及两个整数 `a` 和 `b` 。其中，字符串 `s` 的长度为偶数，且仅由数字 `0` 到 `9` 组成。可在 `s` 上按任意顺序、多次执行下面两个操作之一：
-   累加：将  `a` 加到 `s` 中所有下标为奇数的元素上（**下标从 0 开始**）。数字一旦超过 `9` 就会变成 `0`，如此循环往复。例如，`s = "3456"` 且 `a = 5`，则执行此操作后 `s` 变成 `"3951"`。
-   轮转：将 `s` 向右轮转 `b` 位。例如，`s = "3456"` 且 `b = 1`，则执行此操作后 `s` 变成 `"6345"`。

返回在 `s` 上执行上述操作任意次后，可以得到的 **字典序最小** 的字符串。

---
### 解法1 BFS+哈希集合
使用 `unordered_set` 而非 `unordered_map` 就通过了：
```cpp
class Solution {
public:
    string findLexSmallestString(string s, int a, int b) {
        unordered_set<string> rec;
        queue<string> q;
        q.push(s);
        string ans = s;
        rec.emplace(s);
        while (!q.empty()) {
            string v = q.front(); q.pop();
            if (v < ans) ans = v;
            string v1 = v.substr(v.size() - b) + v.substr(0, v.size() - b);
            if (!rec.count(v1)) {
                q.emplace(v1);
                rec.emplace(v1);
            }
            for (int i = 1; i < v.size(); i += 2) {
                int val = (v[i] - '0' + a) % 10;
                v[i] = val + '0';
            }
            if (!rec.count(v)) {
                q.push(v);
                rec.emplace(v);
            }
        }
        return ans;
    }
};
```

---
### 解法2 枚举
题目有两种操作，以下操作可以进行无限次，问可以得到的字典序最小的字符串：
- 累加，将 $s$ 的奇数位元素加上 $a$ ，超过 $9$ 则回到 $0$ 继续加。
- 轮转，将 $s$ 向右轮转 $b$ 位。

> 事实上，这两种操作有一种统一性：
> - 向右轮转 $b$ 位，相当于将原本第 $i$ 位的元素移动到第 $i + b$ 位上，即 $i + b$ ，和对奇数位元素加上 $a$ 差不多。
> - 轮转会产生循环，累加（并取余）也会产生循环。==对于累加操作，数字最多累加 $10$ 次，就会回到原来的状态；对于轮转操作，字符串最多轮转 $n$ 次，也会回到原来的状态==。
> - $s$ 的长度是偶数，则 $(i + b)$ 需要对偶数取余；$[0, 9]$ 的个数是偶数，对 $10$ 取余也是对偶数取余。
> - 即轮转是横轴上的“累加”，累加是纵轴（指数字大小）上的“轮转”。

由于条件中 $s$ 的长度 $n$ 是偶数，因此**如果 $b$ 是偶数，那么无论轮转多少次，我们都只能对 $s$ 中奇数位的元素做累加操作**（奇数加偶数还是奇数，偶数加偶数还是偶数）。但**如果 $b$ 是奇数的话，通过轮转可以让原来偶数位变成奇数位**，我们**可以给 $s$ 中奇数位和偶数位的元素都做加法，并且可以做不同的次数**。轮转操作最多产生 $n$ 种状态，如果轮转位数 $b$ 为偶数，累加操作只会对奇数位数字产生影响，因此总共产生 $n \times 10$ 种状态；如果轮转位数 $b$ 为奇数，累加操作既会对奇数位数字产生影响，也会对偶数位数字产生影响，因此总共产生 $n \times 10 \times 10$ 种状态。

还可看出，先对奇数位任意次累加、再进行任意次轮转得到的字符串，先进行任意次轮转、再进行任意次累加也能得到，即==做累加操作的次数和做轮转操作的次数是互相独立的，做轮转的次数并不会影响到能否对数位进行累加==。因此我们可以**先枚举做轮转的次数，再枚举做累加的次数**（即**先完成轮转，再完成累加**），从而找到字典序最小的答案。更具体的，我们按照如下步骤进行枚举：
1. 枚举做 $b$ 位轮转的次数，然后令 $t$ 为每次 $s$ 轮转后的字符串。由于**轮转最终会产生循环，且至多轮转 $n$ 次**（$b$ 为奇数时最多才能到 $n$ 次），因此我们用一个数组 $vis$ 、来记录每个位置是否被轮转过。==如果遇到之前轮转过的位置，则枚举结束==。
2. 对于每个 $t$ ，枚举对 $t$ 的奇数位做累加 $a$ 操作的次数 $j$ ，再枚举对 $t$ 的偶数位做累加操作的次数 $k$ 。这里==因为元素值范围是 $[0,9]$ ，因此我们做累加操作的次数上限是 $9$ ，第 $0$ 次不对数位进行操作，再多势必会产生循环==。特殊的，如果 $b$ 是偶数，则 $k$ 的上限是 $0$（即只能对奇数位进行操作），否则是 $9$ 。
 
```cpp
class Solution {
public:
    string findLexSmallestString(string s, int a, int b) {
        int n = s.size();
        vector<int> vis(n);
        string res = s;
        // 将 s 延长一倍，方便截取轮转后的字符串 t
        s = s + s;
        for (int i = 0; vis[i] == 0; i = (i + b) % n) { // <= n
            vis[i] = 1;
            for (int j = 0; j < 10; j++) { // <= 10
                int k_limit = b % 2 == 0 ? 0 : 9;
                for (int k = 0; k <= k_limit; k++) { // <= 10
                    // 每次进行累加之前，重新截取 t
                    string t = s.substr(i, n);
                    for (int p = 1; p < n; p += 2) {
                        t[p] = '0' + (t[p] - '0' + j * a) % 10;
                    }
                    for (int p = 0; p < n; p += 2) {
                        t[p] = '0' + (t[p] - '0' + k * a) % 10;
                    }
                    res = min(res, t);
                }
            }
        }
        return res;
    }
};
```
枚举轮转次数过程中，我们依次考虑了如下位置：$$0 \times b \pmod n,~1 \times b \pmod n,~2 \times b \pmod n,\ \cdots,\ x \times b\pmod n$$ 。用一个表达式来计算最终到达的位置：
$$xb−yn=z$$
其中 $x$ 是轮转次数，$y$ 是取模过程减去 $n$ 的次数，而 $z$ 是最终到达的位置。
> **对于不全为零的任意整数 $a, b$ ，记 $g=gcd(a,b)$ 为 $a,b$ 的最大公约数，则对于任意整数 $x, y$ 都满足 $ax + by$ 是 $g$ 的倍数**。

根据[[LeetCode 1250. Check If It Is a Good Array【数论】中等]]中用过的裴蜀定理可知，**$z$ 一定是 $gcd(b,n)$ 的倍数**，因此我们只需枚举 $[0,n)$ 中 $gcd(b, n)$ 的倍数即可，从而==改善轮转次数的枚举==。
```cpp
class Solution {
public:
    string findLexSmallestString(string s, int a, int b) {
        int n = s.size();
        string ans = s;
        // 将s延长一倍,方便截取轮转后的字符串t; 而非临时对其进行截取和拼凑
        s = s + s;
        int g = gcd(b, n);
        for (int i = 0; i < n; i += g) { // 模拟轮转b位,最多不超过n次
            for (int j = 0; j <= 9; ++j) { // 模拟对字符串奇数位累加,最多不超过10次
                int kLimit = (b & 1) ? 9 : 0; // b是奇数则可以轮转后操作原本的偶数位; 否则只能操作奇数位
                for (int k = 0; k <= kLimit; ++k) {
                    // 每次进行累加前,重新截取t
                    string t = s.substr(i, n);
                    for (int p = 1; p < n; p += 2)
                        t[p] = '0' + (t[p] - '0' + j * a) % 10;
                    for (int p = 0; p < n; p += 2)
                        t[p] = '0' + (t[p] - '0' + k * a) % 10;
                    ans = min(ans, t);  
                }
            }
        }
        return ans;
    }
};
```
枚举累加次数的过程中，我们的目的是**让字符串的字典序更小**，由于==奇偶位两组互相独立，**组内累加次数相同**==，因此我们只需考虑 $t[0]$ 和 $t[1]$ 即可——==先找到使得 $t[1]$ 最小的累加次数，对奇数位进行累加==。==如果 $b$ 是奇数，还要找到让 $t[0]$ 最小的累加次数==，对偶数位进行累加。
```cpp
class Solution {
public:
    string findLexSmallestString(string s, int a, int b) {
        int n = s.size();
        string ans = s;
        // 将s延长一倍,方便截取轮转后的字符串t; 而非临时对其进行截取和拼凑
        s = s + s;
        int g = gcd(b, n);
        auto findMin = [&](string &t, int start) {
            int minVal = 10, times = 0;
            for (int i = 0; i <= 9; ++i) {
                int added = ((t[start] - '0') + i * a) % 10;
                if (added < minVal) {
                    minVal = added;
                    times = i;
                }
            }
            for (int i = start; i < n; i += 2) // 组内累加次数相同
                t[i] = '0' + ((t[i] - '0') + times * a) % 10;
        };
        for (int i = 0; i < n; i += g) {
            string t = s.substr(i, n);
            findMin(t, 1);
            if (b % 2) findMin(t, 0); // b是偶数
            ans = min(ans, t);
        }
        return ans;
    }
};
```
复杂度分析：
- 时间复杂度：$O(n^2d^2)$ ，其中 $n$ 是 $s$ 的长度，$d$ 是枚举累加次数的上限，本题中等于 $10$ 。==优化枚举轮转次数后，算法复杂度常数改善==，在最坏情况下仍然为 $O(n^2d^2)$ 。==优化枚举累加次数==后，时间复杂度降低为 $O(n^2d)$ 。
- 空间复杂度：$O(n)$ ，其中 $n$ 是 $s$ 的长度。

事实上，我们没有必要暴力求最小累加值，可以直接算出来，这样复杂度可以优化到 $O(n^2)$ 。方法也很简单。枚举累加次数过程中，以 $x$ 为起始点，我们依次考虑如下累加值：$$0 \times a \pmod {10},\ 1 \times a \pmod {10},~2 \times a \pmod {10},\ \cdots,\ (k-1)\times a\pmod {10}$$ 用一个表达式来计算最终的累加值：
$$ka−10y=z$$
其中 $k$ 是轮转次数，$y$ 是取模过程减去 $10$ 的次数，而 $z$ 是最终的累加值。同样根据裴蜀定理，$z$ 一定是 $gcd(a, 10)$ 的倍数。我们只需不断给 $x$ 累加 $gcd(a, 10)$ 就可枚举整个数列。
> **对于不全为零的任意整数 $a, b$ ，记 $g=gcd(a,b)$ 为 $a,b$ 的最大公约数，则对于任意整数 $x, y$ 都满足 $ax + by$ 是 $g$ 的倍数**。

因此，==数字 $x$ 经过累加 $a$ （并对 $10$ 取模）的任意次操作，能得到的所有结果构成公差为 $gcd(a,10)$ 的等差数列==，即为 $x+k\times gcd(a,10) \pmod {10}$ 。**$x$ 是整个等差数列其中一个数，但不一定是最小的非负值**，最小非负值是 $x\mod gcd(a,10)$ 。

为了**避免多次的取子串 `substr` 操作**，同时减少「反复增加字符串中字符值的操作」，这里使用 `RotationStringPosition` 表示「轮转后以 `s[pos]` 开头的字符串、对其奇数位加 `d1` 、偶数位加 `d0` 」。对比字符串大小时，使用自定义的 `compare` 函数，`s` 作为 `string_view` 传入。
```cpp
class Solution {
public:
	struct RotationStringPosition {
		unsigned pos, d0, d1; // 轮转后以s[pos]开头的字符串,对其奇数位加d1,偶数位加d0
	};
	static unsigned add(unsigned a, unsigned b, unsigned m) { // 即(a+b)%m,但不用取模% 
		const unsigned c = a + b;
		return c < m ? c : c - m;
	} 
	// s1<s2,reurn true
	static bool compare(string_view s, RotationStringPosition s1, RotationStringPosition s2) {
		const unsigned d1[2] = {s1.d0, s1.d1};
		const unsigned d2[2] = {s2.d0, s2.d1};
		unsigned p1 = s1.pos, p2 = s2.pos;
		const unsigned n = s.size();
		for (unsigned i = 0; i < n; ++i) { // 对比完整个字符串长度就结束
			const unsigned c1 = add(s[p1] - '0', d1[i % 2], 10); // 轮转字符串s1的第p1位字符
			const unsigned c2 = add(s[p2] - '0', d2[i % 2], 10); // 轮转字符串s2的第p2位字符
			if (c1 != c2) return c1 < c2;
			if (++p1 == n) p1 = 0;
			if (++p2 == n) p2 = 0;
		}
		return false;
	}  
	string findLexSmallestString(string s, int a, int b) {
		const int n = s.size(), ga = gcd(a, 10), gb = gcd(b, n);
        RotationStringPosition ans = {0, 0, 0};
        cout << ga << endl;
		for (unsigned i = 0; i < n; i += gb) { // 最小值是x mod gcd(a,10),但这里的d0,d1是对s[i]-'0'=x的增值!
			// 在轮转后以s[pos]开头的字符串s中,直接计算使字符串最小的最小组内累加值
			const unsigned d0 = (gb & 1) ? (10 - (s[i] - '0') / ga * ga) : 0; // gb是奇数,和b是奇数效果相同; 对偶数位进行操作的最小累加值
			const unsigned d1 = 10 - (s[add(i, 1, n)] - '0') / ga * ga; // 对奇数位进行操作的最小累加值
            // x+d=x%ga,d=x%ga-x<=0,d+=10不影响结果
            // 10-x+x%ga=10-(x-x%ga)=10-(x/ga*ga)
			if (compare(s, {i, d0, d1}, ans)) ans = {i, d0, d1};	
		}
		const auto ptr = s.data();
		rotate(ptr, ptr + ans.pos, ptr + n); // 下面的轮转和累加操作不能颠倒!
		const unsigned d[2] = {ans.d0, ans.d1}; 
		for (unsigned i = 0; i < n; ++i)
			s[i] = add(s[i] - '0', d[i % 2], 10) + '0';
		return s;
	}  
};
```
这一代码已经能打败90%的代码：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303210222621.png)
另外，还可以换个思路，**枚举累加值**，然后用变种的最小表示法**计算最小轮换数**，复杂度 $O(nd^2)$ 。
```cpp
class Solution {
public:
	struct RotationStringPosition {
		unsigned pos, d0, d1; // 轮转后以s[pos]开头的字符串,对其奇数位加d1,偶数位加d0
	};
	static unsigned add(unsigned a, unsigned b, unsigned m) { // 即(a+b)%m,但不用取模% 
		const unsigned c = a + b;
		return c < m ? c : c - m;
	} 
	// s1<s2,reurn true
	static bool compare(string_view s, RotationStringPosition s1, RotationStringPosition s2) {
		const unsigned d1[2] = {s1.d0, s1.d1};
		const unsigned d2[2] = {s2.d0, s2.d1};
		unsigned p1 = s1.pos, p2 = s2.pos;
		const unsigned n = s.size();
		for (unsigned i = 0; i < n; ++i) { // 对比完整个字符串长度就结束
			const unsigned c1 = add(s[p1] - '0', d1[p1 % 2], 10); // 先累加,再轮转字符串s1,比较第p1位字符
			const unsigned c2 = add(s[p2] - '0', d2[p2 % 2], 10); // 先累加,再轮转字符串s2,比较第p2位字符
			if (c1 != c2) return c1 < c2;
			if (++p1 == n) p1 = 0;
			if (++p2 == n) p2 = 0;
		}
		return false;
	}  
	static unsigned minRotate(string_view s, unsigned d0, unsigned d1, unsigned gb) {
        const unsigned d[2] = {d0, d1};
        unsigned i = 0, j = gb, k = 0;
        const unsigned n = s.size(); // 两次轮转中比较第k个字符+累加值
        while (i < n && j < n && k < n) { // 开始比较
            const unsigned p1 = add(i, k, n); // 上次轮转字符串的第k个字符
            const unsigned p2 = add(j, k, n); // 本次轮转字符串的第k个字符
            const unsigned c1 = add(s[p1] - '0', d[p1 % 2], 10); // 先累加
            const unsigned c2 = add(s[p2] - '0', d[p2 % 2], 10); 
            if (c1 == c2) ++k; // 两次轮转的第k个字符+累加值后相等,继续比较下个字符
            else {
	            if (c1 < c2) { // 这次轮转后反而变大
		            j += (k / gb + 1) * gb; // 跳到后面的轮转次数
	            } else i += (k / gb + 1) * gb;
	            if (i == j) j += gb; // 错开
	            k = 0; // 两个字符不等,则下面的轮转又要从头比起
            }
        }
        return min(i, j);
	} 
    string findLexSmallestString(string s, int a, int b) {
	    const int n = s.size(), ga = gcd(a, 10), gb = gcd(b, n);
	    const unsigned na = (gb & 1) ? 10 : ga; // 累加值的限界
	    RotationStringPosition ans = {0, 0, 0};
	    for (unsigned i = 0; i < na; i += ga) { // 枚举对偶数位可能的累加值
		    for (unsigned j = 0; j < 10; j += ga) { // 枚举对奇数位可能的累加值
			    const unsigned p = minRotate(s, i, j, gb); // 求出最小轮转次数
			    if (compare(s, {p, i, j}, ans))
				    ans = {p, i, j};
		    }
	    } 
        const unsigned d[2] = {ans.d0, ans.d1}; // 下面的轮转和累加操作不能颠倒!
        for (unsigned i = 0; i < n; ++i)
            s[i] = add(s[i] - '0', d[i % 2], 10) + '0';
        const auto ptr = s.data();
        rotate(ptr, ptr + ans.pos, ptr + n);
        return s;
    }
};
```