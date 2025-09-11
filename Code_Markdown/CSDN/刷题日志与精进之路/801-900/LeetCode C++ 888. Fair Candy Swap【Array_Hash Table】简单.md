Alice and Bob have candy bars of different sizes: `A[i]` is the size of the `i`-th bar of candy that Alice has, and `B[j]` is the size of the `j`-th bar of candy that Bob has.

Since they are friends, they would like to exchange one candy bar each so that after the exchange, they both have the same total amount of candy.  *(The total amount of candy a person has is the sum of the sizes of candy bars they have.)*

Return an integer array `ans` where `ans[0]` is the size of the candy bar that Alice **must** exchange, and `ans[1]` is the size of the candy bar that Bob **must** exchange.

If there are multiple answers, you may return any one of them.  It is **guaranteed** an answer exists.

 

**Example 1:**

```swift
Input: A = [1,1], B = [2,2]
Output: [1,2]
```

**Example 2:**

```swift
Input: A = [1,2], B = [2,3]
Output: [1,2]
```

**Example 3:**

```swift
Input: A = [2], B = [1,3]
Output: [2,3]
```

**Example 4:**

```swift
Input: A = [1,2,5], B = [2,4]
Output: [5,4]
```

 

**Note:**
- `1 <= A.length <= 10000`
 - `1 <= B.length <= 10000`
- `1 <= A[i] <= 100000`
 - `1 <= B[i] <= 100000`
 -  It is guaranteed that Alice and Bob have different total amounts of candy.
  -  It is guaranteed there exists an answer.

题意：爱丽丝和鲍勃有不同大小的糖果棒：`A[i]` 是爱丽丝拥有的第 `i` 块糖的大小，`B[j]` 是鲍勃拥有的第 `j` 块糖的大小。他们想交换一个糖果棒，这样交换后，他们都有相同的糖果大小总量。返回一个整数数组 `ans`，其中 `ans[0]` 是爱丽丝必须交换的糖果棒的大小，`ans[1]` 是鲍勃必须交换的糖果棒的大小。如果有多个答案，可以返回其中任何一个。保证答案存在。


---
### 解法 哈希表
先计算出爱丽丝和鲍勃各自拥有的糖果大小总量 `suma, sumb` ，交换后拥有相同的糖果大小总量 `average = (suma + sumb) / 2` 。对于鲍勃拥有的每个糖果 `B[i]` ，如果鲍勃要交换出 `B[i]` ，则剩下 `sumb - B[i]` 个糖果。要让交换成立，则爱丽丝必须有某个大小为 `average - sumb - b[i]` 的糖果，用以交换过来：
```cpp
class Solution {
public:
    vector<int> fairCandySwap(vector<int>& A, vector<int>& B) {
        int n = A.size(), m = B.size(), suma = 0, sumb = 0;
        unordered_set<int> recA;
        for (const int &v : A) { 
            recA.insert(v);
            suma += v;
        }
        for (const int &v : B) sumb += v;
        int average = (suma + sumb) / 2;
        vector<int> ans;
        for (int i = 0; i < m; ++i) { //选择了B[i]用来交换
            int temp = average - (sumb - B[i]); //需要交换过来的正数值 
            if (recA.find(temp) != recA.end())
                return {temp, B[i]};
        }
        return {0, 0};
    }
};
```
运行效率如下：
```cpp
执行用时：216 ms, 在所有 C++ 提交中击败了78.53% 的用户
内存消耗：46.3 MB, 在所有 C++ 提交中击败了54.08% 的用户
```
