Write a method to replace all spaces in a string with `'%20'` . You may assume that the string has sufficient space at the end to hold the additional characters,and that you are given the `"true"` length of the string. (Note: If implementing in `Java` , please use a character array so that you can perform this operation in place.)

**Example 1:**
```cpp
Input: "Mr John Smith ", 13
Output: "Mr%20John%20Smith"
Explanation: 
The missing numbers are [5,6,8,...], hence the third missing number is 8.
```

**Example 2:**
```cpp
Input: "               ", 5
Output: "%20%20%20%20%20"
```
**Note:**  `0 <= S.length <= 500000`




题意：URL转换。将规定长度 `length` 范围内的字符串中的空格全部替换为 `%20` 。

---
思路1：辅助字符串。代码如下：
```cpp
class Solution {
public:
    string replaceSpaces(string S, int length) {
        string ans; 
        for (int i = 0; i < length; ++i) {
            if (S[i] != ' ') ans.append(1, S[i]);
            else ans.append("%20");
        }
        return ans;
    }
};
```
效率很低：
```cpp
执行用时：124 ms, 在所有 C++ 提交中击败了9.00% 的用户
内存消耗：25.2 MB, 在所有 C++ 提交中击败了20.44% 的用户
```
---
思路2：原地工作。设 <code>1 <= i <= length</code>，`size` 是字符串 `S` URL化后的真实长度。一个在字符串算法中很常见的方法是：**我们编辑字符串，从尾部开始，然后往前修改字符串**。因为在字符串尾部有额外的 `buffer` ，这使得我们不用担心越界访问。在这个问题中使用这一做法，下面的算法将会扫描两遍。
- 第一遍：虽然原串中有足够的空间存储新增字符，但是我们不会使用所有空间。为此需要先计算URL化后的空间大小 `size` —— `length` 范围内空格的数量为 `spaceCount` ，所以 `size = length + spaceCount * 2` 。
- 之后，我们从 `S[length - i], i = 1`  开始往前扫描，如果是字符，则复制到 `S[size - i]` 处；否则用 `%20` 进行替换。


无论是C++还是Java，LeetCode上使用的都是字符串类，不过Java的字符串不变，**这样就需要返回该字符串的一个新拷贝，但是这也允许算法只进行一次扫描**，*具体做法参考思路1*。实用的、使用 `char[]` 的Java代码如下：
```java
class Solution {
    public void replaceSpaces(char[] str, int trueLength) {
        int spaceCount = 0;
        for (int i = 0; i < trueLength; ++i) 
            if (str[i] == ' ') ++spaceCount;
        int size = trueLength + spaceCount * 2, rear = size - 1; //URL后的真实长度
        if (size < str.length) str[size] = '\0'; //End Array
        for (i = trueLength - 1; i >= 0; --i) {
            if (str[i] == ' ') {
                str[rear--] = '0';
                str[rear--] = '2';
                str[rear--] = '%';
            } else str[rear--] = str[i];
        } 
    }
}
```
如果在LeetCode上用C++提交，代码如下。在原串上修改，不计算空格的数量：
```cpp
class Solution {
public:
    string replaceSpaces(string S, int length) {
        int rear = S.size() - 1;
        for (int i = length - 1; i >= 0; --i) {
            if (S[i] != ' ') S[rear--] = S[i];
            else {
                S[rear--] = '0';
                S[rear--] = '2';
                S[rear--] = '%';
            }
        }
        return S.substr(rear + 1);
    }
};
```
效率如下：
```cpp
执行用时：56 ms, 在所有 C++ 提交中击败了99.06% 的用户
内存消耗：21.6 MB, 在所有 C++ 提交中击败了78.21% 的用户
```

