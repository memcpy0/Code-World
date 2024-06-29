A conveyor belt has packages that must be shipped from one port to another within `D` days.

The ith package on the conveyor belt has a weight of `weights[i]`. Each day, we load the ship with packages on the conveyor belt (in the order given by `weights`). We may not load more weight than the maximum weight capacity of the ship.

Return **the least weight capacity** of the ship that will result in all the packages on the conveyor belt being shipped within D days.
 
**Example 1:** 
```clike
Input: weights = [1,2,3,4,5,6,7,8,9,10], D = 5
Output: 15
Explanation: A ship capacity of 15 is the minimum to ship all the packages in 5 days like this:
1st day: 1, 2, 3, 4, 5
2nd day: 6, 7
3rd day: 8
4th day: 9
5th day: 10

Note that the cargo must be shipped in the order given, so using a ship of capacity 14 and splitting the packages into parts like (2, 3, 4, 5), (1, 6, 7), (8), (9), (10) is not allowed.
```
**Example 2:**

```clike
Input: weights = [3,2,2,4,1,4], D = 3
Output: 6
Explanation: A ship capacity of 6 is the minimum to ship all the packages in 3 days like this:
1st day: 3, 2
2nd day: 2, 4
3rd day: 1, 4
```

**Example 3:**

```clike
Input: weights = [1,2,3,1,1], D = 4
Output: 3
Explanation:
1st day: 1
2nd day: 2
3rd day: 3
4th day: 1, 1
```
**Constraints:**
- `1 <= D <= weights.length <= 5 * 1e4`
- `1 <= weights[i] <= 500`

题意：传送带上的包裹必须在 `D` 天内从一个港口运送到另一个港口。传送带上的第 `i` 个包裹的重量为 `weights[i]` 。每一天，我们都会按**给出重量的顺序**往传送带上装载包裹。我们装载的重量不会超过船的最大运载重量。返回能在 `D` 天内将传送带上的所有包裹送达的船的最低运载能力。

---
### 解法 二分
船的最大运载重量越大，运输天数越少，于是问题变成选择尽可能小的船的最大运载重量（运输天数变多），同时满足能在 `D` 天里将所有包裹送达。做法是，二分选择船的最大运载重量，再顺序检测能否送达，能送达，说明当前船的最大运载重量可能是答案；不能送达，说明需要提升船的最大运载重量。一个特例是，如果某个包裹的重量 `weights[i]` 大于船的最大运载重量 `capacity` ，那么就永远无法送达，必须增加运载重量（常数优化：令 `capacity = weights[i]`）。
```cpp
class Solution {
public:
    int shipWithinDays(vector<int>& weights, int D) {
        if (weights.empty()) return 0; 
        int inf = 0x3f3f3f3f, lo = 0, hi = accumulate(weights.begin(), weights.end(), 0); 
        while (lo < hi) {
            int capacity = lo + (hi - lo) / 2; //船的最大运载能力 
            int days = 0, cursum = 0;
            for (const int &w : weights) {
                if (w > capacity) { days = inf; lo = w; break; } //当前这个包裹超过船的最大运载,无法运输
                cursum += w; //船上添加这个包裹
                if (cursum > capacity) { //如果此时的船超载
                    ++days; //记录新的天数
                    cursum = w; //这个包裹留到下一天运输
                }
            }
            if (days == inf) continue;
            ++days; //此时cursum必然非零,需要运输最后一部分包裹
            if (days <= D) hi = capacity;
            else lo = capacity + 1;
        }
        return lo;
    }
};
```
运行结果如下：
```cpp
执行用时：36 ms, 在所有 C++ 提交中击败了98.02% 的用户
内存消耗：25.4 MB, 在所有 C++ 提交中击败了76.94% 的用户
```
简单写成如下形式：
```cpp
class Solution {
public:
    int shipWithinDays(vector<int>& weights, int D) {
        if (weights.empty()) return 0; 
        int lo = *max_element(weights.begin(), weights.end()), hi = accumulate(weights.begin(), weights.end(), 0); 
        while (lo < hi) {
            int capacity = lo + (hi - lo) / 2; //船的最大运载能力 
            int days = 1, cursum = 0; //days初始化为1
            for (const int &w : weights) {  
                if (cursum + w > capacity) { //如果加上新包裹,船超载
                    ++days; //记录新的天数
                    cursum = 0; 
                }
                cursum += w; //船上添加这个包裹
            }  
            if (days <= D) hi = capacity;
            else lo = capacity + 1;
        }
        return lo;
    }
};
```
