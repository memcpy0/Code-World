@[toc]
### 进制转换的原理
从R进制到10进制的方法很简单，但是从10进制到R进制，**往往采用辗转相除法**。原理在于：

设Ｒ进制数为W，对应的十进制的数为Y，有如下多项式关系：
- X~n~*R^n^ + X~n-1~R^n-1^ + ... + X~2~R^2^ + X~1~R^1^ + X~0~R^0^ = Y (从左到右是R进制转换为10进制，按位乘权相加；而从右到左是10进制转换为R进制)
- 同时，R进制形式的W = X~n~X~n-1~X~n-2~...X~2~X~1~X~0~
- 因此，我们要求出W，就是要求出【 X~n~，X~n-1~，X~n-2，~...X~2~，X~1~，X~0~】这些未知数的值。
- 那么我们这样做：
- 等式两边同时模R，比如说R=2，2^3^x1=8，那么模2就会等于0，而2^0^x1=1，模2=1。R^0^=1，因此有X~0~ = Y % R。
- 接着求X~1~，按照上一步的方法，需要先排除X~0~的影响，因此我们将等式两边同时除以R，在R进制中X~i~必然小于R【X~i~范围是`[0, R-1]`】，因此X~0~R^0^／R＝０，即有X~n~*R^n－１^ + X~n-1~R^n-２^ + ... + X~2~R^１^ + X~1~R^０^＝Y / R。然后两边同上一步取模即可。
- ...
- 最后一步，X~n~*R^0^ = Y / R^n-1^，同上取模即可得X~n~。 
- 因为每一步得到的是X~0~、X~1~、...X~n~的顺序，所以需要倒着排序，求出W。
- 这就是10进制转R进制的方法。

### 168. Excel Sheet Column Title 【简单】
Given a positive integer, return its corresponding column title as appear in an Excel sheet.

For example:

    1 -> A
    2 -> B
    3 -> C
    ...
    26 -> Z
    27 -> AA
    28 -> AB 
    ...

Example 1:
```
Input: 1
Output: "A"
```
Example 2:
```
Input: 28
Output: "AB"
```
Example 3:
```
Input: 701
Output: "ZY"
```
题意：给出一个正整数，返回它出现在Excel表中的对应列号，1等价于A，2等价于B，依次类推，26等价于Z。

思路：这道题和进制转换很类似，其实也就是10进制转为26进制的变形。按照上面说的，有以下的模型：

设26进制数为W，对应的十进制的数为Y，有如下多项式关系：(注意：这里的范围实际上不是0-25，而是1-26，所以表示成如下形式，本来应该A-Z对应0-25的...)
- (X~n~+1)26^n^ + (X~n-1~+1)26^n-1^ + ... + (X~2~+1)26^2^ + (X~1~+1)26^1^ + (X~0~+1)26^0^ = Y  
- 那么我们这样做：
- 等式两边同时模26，有X~0~+1 = Y % 26，则X~0~=Y % 26 - 1...是行不通的！原因在于X~i~+1可能为26。
- 因此，我们在等式两边减1，有(X~n~+1)26^n^ + (X~n-1~+1)26^n-1^ + ... + (X~2~+1)26^2^ + (X~1~+1)26^1^ + X~0~ = Y - 1。然后同时模26，有X~0~ = （Y - 1) % 26。用’A‘+X~0~可以得到相对应的字符。
- 接着求X~1~，需要先排除X~0~的影响，因此我们将上面的等式两边同时除以26，X~i~必然小于26【X~i~范围是`[0, 25]`】，因此X~0~／R＝０，即有(X~n~+1)26^n^ + (X~n-1~+1)26^n-1^ + ... + (X~2~+1)26^2^ + (X~1~+1)26^1^  = (Y - 1)  / 26。然后两边同上一步取模即可。
- ...

写成程序如下：

`执行用时 :0 ms, 在所有 cpp 提交中击败了100.00% 的用户`
`内存消耗 :8.2 MB, 在所有 cpp 提交中击败了7.34%的用户`
```cpp
class Solution {
public:
    string convertToTitle(int n) {
        string ans;
        while (n) {
            ans += ('A' + (n - 1) % 26);
            n = (n - 1) / 26;
        } 
        reverse(ans.begin(), ans.end());
        return ans;
    }
};
```
### 171. Excel Sheet Column Number【简单】
Given a column title as appear in an Excel sheet, return its corresponding column number.

For example:

    A -> 1
    B -> 2
    C -> 3
    ...
    Z -> 26
    AA -> 27
    AB -> 28 
    ...

Example 1:
```
Input: "A"
Output: 1
```
Example 2:
```
Input: "AB"
Output: 28
```
Example 3:
```
Input: "ZY"
Output: 701
```
题意：给出字符串，返回对应的列数值。从26进制转换为10进制。

方法：是上面的题的反向，每一位权重为26：

```cpp
class Solution {
public:
    int titleToNumber(string s) {
        int ans = 0;
        for (const auto i : s) {
            ans = ans * 26 + (i - 'A' + 1);
        }
        return ans;
    }
};
```
