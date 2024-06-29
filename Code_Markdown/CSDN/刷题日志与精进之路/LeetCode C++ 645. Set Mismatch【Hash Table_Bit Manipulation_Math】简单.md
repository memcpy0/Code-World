 The set `S` originally contains numbers from `1` to `n` . But unfortunately, due to the data error, one of the numbers in the set got duplicated to another number in the set, which results in repetition of one number and loss of another number.

Given an array nums representing the data status of this set after the error. Your task is to **firstly** find the number occurs twice and **then** find the number that is missing. Return them in the form of an array.

**Example 1:**

```swift
Input: nums = [1,2,2,4]
Output: [2,3]
```

**Note:**
-  The given array size will in the range `[2, 10000]` .
 -   The given array's numbers won't have any order.

题意：集合 `S` 包含从 `1` 到 `n` 的整数。因为数据错误，导致集合里面某一个元素复制成了另外一个元素的值，导致集合丢失了一个整数并且有一个元素重复。

给定一个数组 `nums` 代表了集合 `S` 发生错误后的结果。首先寻找到重复出现的整数，再找到丢失的整数，将它们以数组的形式返回。

---
### 思路1 哈希表
暴力的 $O(n^2)$ 算法就不说了。这里用位图求出重复的整数，然后遍历一遍布尔数组，得到丢失的整数：
```cpp
class Solution {
public:
    vector<int> findErrorNums(vector<int>& nums) {
        bitset<10010> bst;
        int repeatNum, missNum;
        for (const int &v : nums) {
            if (bst[v]) repeatNum = v;
            else bst[v] = true;
        }
        for (int i = 1; i <= 10000; ++i) {
            if (bst[i] == false) {
                missNum = i;
                break;
            }
        }
        return vector<int>{repeatNum, missNum};
    }
};
```
 
优化一下，求出了重复的整数后，可以直接得到丢失的整数：
```cpp
class Solution {
public:
    vector<int> findErrorNums(vector<int>& nums) {
        bitset<10010> bst;
        int n = nums.size(), sum = (1 + n) * n / 2, repeatNum, trueSum = 0;
        for (const int &v : nums) {
            if (bst[v]) repeatNum = v;
            else bst[v] = true;
            trueSum += v;
        } //sum = (1 + n) * n / 2; trueSum包含重复的整数
        return vector<int>{repeatNum, sum - trueSum + repeatNum};
    }
};
```
执行效率如下：
```cpp
执行用时：68 ms, 在所有 C++ 提交中击败了60.46% 的用户
内存消耗：20 MB, 在所有 C++ 提交中击败了56.46% 的用户
```
---
### 思路2 负数标记法
已知 `nums[]` 中所有数字都是正整数，且处于 `1 ~ n` 之间。遍历 `nums[]` 中的所有数字，根据数字 `i` 找到 `nums[|i|]` ，如果是第一次访问 `nums[|i|}` ，将其反转为负数；如果是第二次访问，会发现已经是负数。因此，可以根据访问一个数字时，是否已经是负数找到重复数字。

然后，对于所有出现过的数字，其对应索引处的数字都会是负数，只有缺少数字 `j` 对应的索引处仍然是正数。对应的Java代码如下：
```cpp
class Solution {
    public int[] findErrorNums(int[] nums) {
        int[] ans = new int[2];
        for (int i = 0; i < nums.length; ++i) {
            int absv = Math.abs(nums[i]);
            if (nums[absv - 1] > 0) nums[absv - 1] *= -1; //负数标记
            else ans[0] = absv; //对应位置已经是负数,即为重复出现的数
        }
        for (int i = 0; i < nums.length; ++i) {
            if (nums[i] > 0) ans[1] = i + 1; //仍然大于0,即为未出现的数
            else nums[i] *= -1; //恢复原状
        } 
        return ans;
    }
}
```
执行结果如下所示：
```cpp
执行用时：4 ms, 在所有 Java 提交中击败了51.86% 的用户
内存消耗：39.9 MB, 在所有 Java 提交中击败了85.42% 的用户
```

或者，**对发现是重复数字的 `nums[i]` 外的所有 `nums[i]` 异或、同时再对 `1 ~ n` 异或**，得到的也是缺少数字。C++代码如下：
```cpp
class Solution {
public:
    vector<int> findErrorNums(vector<int>& nums) {
        int n = nums.size(), repeatNum = 0, missNum = 0;
        for (int i = 0; i < n; ++i) {
            int absValue = abs(nums[i]);
            if (nums[absValue - 1] > 0) {
                nums[absValue - 1] = -nums[absValue - 1];
                missNum ^= absValue;
            }
            else repeatNum = absValue;
            missNum ^= i + 1;
        }
        return vector<int>{repeatNum, missNum};
    }
};
```
提交后的效率如下：
```cpp
执行用时：84 ms, 在所有 C++ 提交中击败了42.17% 的用户
内存消耗：19.7 MB, 在所有 C++ 提交中击败了94.47% 的用户
```

---
### 思路3 异或运算
将 `nums[i]` 中的所有数字都与 `1 ~ n` 的每个数字做异或运算，得到的结果为 `x ^ y` ，`x, y` 分别表示重复数字和缺失数字。在异或结果中，值为 `1` 的置位比特表示 `x, y` 在该位置的值不同，因此设 `x ^ y` 最右边的置位比特代表的值为 `rightmostbit` 。

根据 `rightmostbit` 将 `1 ~ n` 的所有元素分成两部分、将 `nums[]` 中的元素也分成两部分，一部分的该位为 `1` ，一部分的该位为 `0` 。分别异或就可以得到 `x, y` 。最后还需遍历 `nums[]` ，才能确定哪个是重复数字、哪个是缺失数字。代码如下：
```cpp
class Solution {
public:
    vector<int> findErrorNums(vector<int>& nums) {
        int xxory = 0, n = nums.size();
        for (int i = 0; i < n; ++i)
            xxory ^= nums[i] ^ (i + 1);
        //设重复的数字为x,缺少的数字为y,x!=y,则x^x^x=x,其他的数字全部被抵消,结果为x^y
        //x^y的二进制表示肯定有个1,说明x,y在这一位上一个是0,一个是1
        //把原数组和1~n中间符合这一位=1的全部找出来做异或运算,得出的结果必然是x,y中这一位=1的数
        //再和x^y做异或运算就得出另一个数
        int rightmostSetBitValue = xxory & ~(xxory - 1), x = 0; //xxory & (-xxory)
        for (int i = 0; i < n; ++i) {
            if (nums[i] & rightmostSetBitValue) x ^= nums[i];
            if ((i + 1) & rightmostSetBitValue) x ^= (i + 1);
        }
        for (int i = 0; i < n; ++i) 
            if (nums[i] == x) return vector<int>{x, xxory ^ x}; //x是重复数字
        return vector<int>{xxory ^ x, x};
    }
}; 
```
