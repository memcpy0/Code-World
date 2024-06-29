Given two strings representing two complex numbers.

You need to return a string representing their multiplication. Note $i^2 = -1$ according to the definition.

Example 1:

```cpp
Input: "1+1i", "1+1i"
Output: "0+2i"
Explanation: (1 + i) * (1 + i) = 1 + i2 + 2 * i = 2i, and you need convert it to the form of 0+2i.
```

Example 2:

```cpp
Input: "1+-1i", "1+-1i"
Output: "0+-2i"
Explanation: (1 - i) * (1 - i) = 1 + i2 - 2 * i = -2i, and you need convert it to the form of 0+-2i.
```

Note:

-    The input strings will `not have extra blank`.
-   The input strings will be given in the form of `a+bi`, where the integer `a` and `b` will both belong to the range of `[-100, 100]`. And **the output should be also in this form**.

题意：给出用字符串表示的复数，然后进行复数乘法。

思路：先找到 `+` 的位置，然后对前后的两个数字单独抽取出来转为整型数，进行算术运算。

代码：
```cpp
class Solution {
public:
    string complexNumberMultiply(string a, string b) {
        size_t aAddPos = a.find('+'), bAddPos = b.find('+'), asize = a.size(), bsize = b.size();
        int xr = stoi(a.substr(0, aAddPos)), xi = stoi(a.substr(aAddPos + 1, asize - 1 - aAddPos));
        int yr = stoi(b.substr(0, bAddPos)), yi = stoi(b.substr(bAddPos + 1, bsize - 1 - bAddPos));

        int real = xr * yr - xi * yi, imag = xr * yi + yr * xi;
        string ans = to_string(real) + "+" + to_string(imag) + "i";
        return ans;
    }
};
```
