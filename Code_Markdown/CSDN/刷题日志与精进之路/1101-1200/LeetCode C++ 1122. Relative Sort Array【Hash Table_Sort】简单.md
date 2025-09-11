Given two arrays `arr1` and `arr2` , the elements of `arr2` are distinct, and all elements in `arr2` are also in `arr1` .

Sort the elements of `arr1` such that the relative ordering of items in `arr1` are the same as in `arr2` .  Elements that don't appear in `arr2` should be placed at the end of `arr1` in ascending order.

**Example 1:**
```
Input: arr1 = [2,3,1,3,2,4,6,7,9,2,19], arr2 = [2,1,4,3,9,6]
Output: [2,2,2,1,4,3,3,9,6,7,19]
```

**Constraints:**
-  `arr1.length, arr2.length <= 1000`
- `0 <= arr1[i], arr2[i] <= 1000`
-  Each `arr2[i]` is distinct.
-  Each `arr2[i]` is in `arr1` .


题意：相对排序，将 `arr1` 中的元素按照 `arr2` 中该元素的相对顺序排序，`arr1` 中没有出现在 `arr2` 的元素按照从小到大排序放到 `arr1` 的末尾。`arr2` 的元素是无重复的，且都存在于 `arr1` 中。元素范围小于等于 `1000` 。

---
### 解法 哈希表
因为数据范围小，所以可以开一个 `1005` 大小的哈希表，对 `arr1` 计数排序。接着开一个 `ans` 数组，按照 `arr2` 的元素顺序，将相应的元素按其出现次数加入 `ans` 中。最后从 `0` 到 `1000` ，将哈希表中还没有放进 `ans` 的元素顺序加入 `ans` 尾部 。最后返回 `ans` 。 代码如下：
```c
class Solution {
public:
    vector<int> relativeSortArray(vector<int>& arr1, vector<int>& arr2) {
        int hashTable[1005] = {0};
        for (const int v : arr1) ++hashTable[v];
        vector<int> ans;
        for (const int v : arr2) //按照arr2中元素的相对顺序
            while (hashTable[v]) {
                ans.push_back(v);
                --hashTable[v];
            }
        for (int i = 0; i <= 1000; ++i)
            while (hashTable[i]--)
                ans.push_back(i);
        return ans;
    }
};
```
效率如下：
```cpp
执行用时：4 ms, 在所有 C++ 提交中击败了88.70% 的用户
内存消耗：7.6 MB, 在所有 C++ 提交中击败了84.22% 的用户
```

> **20201114 Update** 打卡写的代码，重用之前的 `arr1` 数组：
> ```cpp
> class Solution { public:
>     vector<int> relativeSortArray(vector<int>& arr1, vector<int>& arr2) {
>         int cnt[1010] = {0}, k = 0; //计算arr1中每个数出现的次数
>         for (const int &v : arr1) ++cnt[v];
>         for (const int &v : arr2) {
>             int size = cnt[v];
>             for (int i = 0; i < size; ++i)
>                 arr1[k++] = v;
>             cnt[v] = 0; //已经使用这些值
>         }
>         for (int i = 0; i <= 1000; ++i)
>             for (int j = 0; j < cnt[i]; ++j)
>                 arr1[k++] = i;
>         return arr1;
>     } 
>  }; 
>  ```

用Java的话：
```java
class Solution {
    public int[] relativeSortArray(int[] arr1, int[] arr2) {
        int[] hash = new int[1005];
        for (int v : arr1) ++hash[v];
        int[] ans = new int[arr1.length];
        int k = 0;
        for (int v : arr2) 
            while (hash[v] > 0) {
                ans[k++] = v;
                --hash[v];
            }
        for (int i = 0; i <= 1000; ++i) 
            while (hash[i]-- > 0)
                ans[k++] = i;
        return ans;
    }
}
```
效率如下……所以这是语言歧视吧？是作弊吧？
```java
执行用时：0 ms, 在所有 Java 提交中击败了100.00% 的用户
内存消耗：39.6 MB, 在所有 Java 提交中击败了43.72% 的用户
```
