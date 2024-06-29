Given a positive integer `num` consisting only of digits 6 and 9.

Return the maximum number you can get by changing **at most** one digit (6 becomes 9, and 9 becomes 6).

 

**Example 1:**

```groovy
Input: num = 9669
Output: 9969
Explanation: 
Changing the first digit results in 6669.
Changing the second digit results in 9969.
Changing the third digit results in 9699.
Changing the fourth digit results in 9666. 
The maximum number is 9969.
```

**Example 2:**

```groovy
Input: num = 9996
Output: 9999
Explanation: Changing the last digit 6 to 9 results in the maximum number.
```

**Example 3:**

```groovy
Input: num = 9999
Output: 9999
Explanation: It is better not to apply any change.
```

 

**Constraints:**
- `1 <= num <= 10^4`
 - `num`'s digits are 6 or 9.

 题意：给出一个仅由数字 `6` 和 `9` 组成的正整数 `num` 。最多只能翻转一位数字，将 `6` 变成 `9`，或者把 `9` 变成 `6` 。返回可以得到的最大数字。 

---
### 解法1 转换为字符串
```cpp
class Solution {
public:
    int maximum69Number (int num) { 
       string t = to_string(num);
       for (char &c : t) {
           if (c == '6') {
               c = '9';
               break;
           }
       }
       return stoi(t);
    }
};
```
效率如下：
```cpp
执行用时：4 ms, 在所有 C++ 提交中击败了28.40% 的用户
内存消耗：6.2 MB, 在所有 C++ 提交中击败了6.19% 的用户
```
---
### 解法2 打表
由于数据范围有限，所以可以对所有情况分别返回对应结果：
```cpp
class Solution {
public:
    int maximum69Number (int num) { 
       switch (num) {
           case 6: return 9;
           case 9: return 9;

           case 66: return 96;
           case 69: return 99;
           case 96: return 99;
           case 99: return 99;

           case 666: return 966;
           case 669: return 969;
           case 696: return 996;
           case 699: return 999;
           case 966: return 996;
           case 969: return 999;
           case 996: return 999;
           case 999: return 999;

            case 6666: return 9666;
            case 6669: return 9669;
            case 6696: return 9696;
            case 6699: return 9699;
            case 6966: return 9966;
            case 6969: return 9969;
            case 6996: return 9996;
            case 6999: return 9999;
            case 9666: return 9966;
            case 9669: return 9969;
            case 9696: return 9996;
            case 9699: return 9999;
            case 9966: return 9996;
            case 9969: return 9999;
            case 9996: return 9999;
            case 9999: return 9999;
       }
       return 0;
    }
};
```
执行效率如下：
```cpp
执行用时：0 ms, 在所有 C++ 提交中击败了100.00% 的用户
内存消耗：6.3 MB, 在所有 C++ 提交中击败了5.11% 的用户
```
