Suppose you have a long flowerbed in which some of the plots are planted and some are not. However, flowers cannot be planted in adjacent plots - they would compete for water and both would die.

Given a flowerbed (represented as an array containing 0 and 1, where 0 means empty and 1 means not empty), and a number **n**, return if **n** new flowers can be planted in it without violating the no-adjacent-flowers rule.

**Example 1:**

```swift
Input: flowerbed = [1,0,0,0,1], n = 1
Output: True
```

**Example 2:**

```swift
Input: flowerbed = [1,0,0,0,1], n = 2
Output: False
```

**Note:**
- The input array **won't violate** no-adjacent-flowers rule.
- The input array size is in the range of `[1, 20000]` .
 - **n** is a non-negative integer which won't exceed the input array size.

题意：假设有一个很长的花坛，一部分地块种植了花（数值为 `1`），另一部分却没有（数值为 `0`），花卉不能种植在相邻的地块上。给出一个数 `n` ，能否在不打破种植规则的情况下种入 `n` 朵花？

---
### 解法 消除边界情况
在 `flowerbed` 数组左右两侧各添加一个 `0` ，好处是不用考虑边界条件。任意为 `0` 的位置处，只要相邻两侧均为 `0` ，就可以栽上一棵花。
```cpp
class Solution {
public:
    bool canPlaceFlowers(vector<int>& flowerbed, int n) {
        vector<int> t;
        t.push_back(0);
        for (const int &v : flowerbed) t.push_back(v);
        t.push_back(0);
        int len = flowerbed.size() + 2, flowers = 0;
        for (int i = 1; i < len - 1; ++i) {
            if (t[i - 1] == 0 && t[i] == 0 && t[i + 1] == 0) {
                t[i] = 1;
                ++flowers;
            }
        }
        return flowers >= n;
    }
};
```
运行效率如下，比较慢：
```cpp
执行用时：44 ms, 在所有 C++ 提交中击败了36.61% 的用户
内存消耗：20.8 MB, 在所有 C++ 提交中击败了5.07% 的用户
```
对应的Java代码如下：
```java
class Solution {
    public boolean canPlaceFlowers(int[] flowerbed, int n) {
        int len = flowerbed.length, flowers = 0;
        int[] arr = new int[len + 2];
        arr[0] = 0;
        for (int i = 0; i < len; ++i) arr[i + 1] = flowerbed[i];
        arr[len + 1] = 0;
        for (int i = 1; i <= len; ++i) {
            if (arr[i - 1] == 0 && arr[i] == 0 && arr[i + 1] == 0) {
                ++flowers; //能种入的花数量+1
                arr[i] = 1; 
            }
        } 
        return flowers >= n;
    }
}
```
运行效率如下：
```java
执行用时：1 ms, 在所有 Java 提交中击败了100.00% 的用户
内存消耗：39.4 MB, 在所有 Java 提交中击败了96.66% 的用户
```
当然，不进行 `0` 添加而是考虑边界情况，也是能写的，不过比较麻烦，以后再写。
