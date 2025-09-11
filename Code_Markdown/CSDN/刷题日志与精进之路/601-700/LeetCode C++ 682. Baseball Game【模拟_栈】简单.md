
You are keeping score for a baseball game with strange rules. The game consists of several rounds, where the scores of past rounds may affect future rounds' scores.

At the beginning of the game, you start with an empty record. You are given a list of strings `ops`, where `ops[i]` is the <code>i<sup>th</sup></code> operation you must apply to the record and is one of the following:

1. An integer `x` - Record a new score of `x`.
 2. `"+"` - Record a new score that is the sum of the previous two scores. It is guaranteed there will always be two previous scores.
  3. `"D"` - Record a new score that is double the previous score. It is guaranteed there will always be a previous score.
  4. `"C"` - Invalidate the previous score, removing it from the record. It is guaranteed there will always be a previous score.

*Return the sum of all the scores on the record.*

 

**Example 1:**

```swift
Input: ops = ["5","2","C","D","+"]
Output: 30
Explanation:
"5" - Add 5 to the record, record is now [5].
"2" - Add 2 to the record, record is now [5, 2].
"C" - Invalidate and remove the previous score, record is now [5].
"D" - Add 2 * 5 = 10 to the record, record is now [5, 10].
"+" - Add 5 + 10 = 15 to the record, record is now [5, 10, 15].
The total sum is 5 + 10 + 15 = 30.
```

**Example 2:**

```swift
Input: ops = ["5","-2","4","C","D","9","+","+"]
Output: 27
Explanation:
"5" - Add 5 to the record, record is now [5].
"-2" - Add -2 to the record, record is now [5, -2].
"4" - Add 4 to the record, record is now [5, -2, 4].
"C" - Invalidate and remove the previous score, record is now [5, -2].
"D" - Add 2 * -2 = -4 to the record, record is now [5, -2, -4].
"9" - Add 9 to the record, record is now [5, -2, -4, 9].
"+" - Add -4 + 9 = 5 to the record, record is now [5, -2, -4, 9, 5].
"+" - Add 9 + 5 = 14 to the record, record is now [5, -2, -4, 9, 5, 14].
The total sum is 5 + -2 + -4 + 9 + 5 + 14 = 27.
```

**Example 3:**

```swift
Input: ops = ["1"]
Output: 1
```

 

**Constraints:**
- `1 <= ops.length <= 1000`
 - `ops[i]` is `"C"`, `"D"`, `"+"`, or a string representing an integer in the range <code>[-3 * 10<sup>4</sup>, 3 * 10<sup>4</sup>]</code>.
- For operation `"+"`, there will always be at least two previous scores on the record.
- For operations `"C"` and `"D"`, there will always be at least one previous score on the record.

题意：作为一场采特殊赛制棒球比赛的记录员，需要按照操作列表记录和改变得分，最后返回记录中所有得分的总和。

---
### 解法 模拟
简单题目，不用多说：
```cpp
class Solution {
public:
    int calPoints(vector<string>& ops) {
        vector<int> temp;
        int ans = 0, n = 0;
        for (string s : ops) {
            switch (s[0]) {
                case '+' : 
                    temp.push_back(temp[n - 1] + temp[n - 2]);
                    ++n;
                    break;
                case 'D':
                    temp.push_back(temp[n - 1] * 2);
                    ++n;
                    break;
                case 'C':
                    temp.pop_back();
                    --n;
                    break;
                default: 
                    temp.push_back(stoi(s));
                    ++n;
                    break;
            }
        }
        for (int i = 0; i < n; ++i) ans += temp[i];
        return ans;
    }
};
```
效率还行：
```cpp
执行用时：8 ms, 在所有 C++ 提交中击败了77.43% 的用户
内存消耗：8.5 MB, 在所有 C++ 提交中击败了16.16% 的用户
```
