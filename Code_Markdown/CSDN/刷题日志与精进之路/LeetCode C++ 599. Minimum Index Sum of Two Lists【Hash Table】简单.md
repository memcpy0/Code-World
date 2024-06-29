Suppose Andy and Doris want to choose a restaurant for dinner, and they both have a list of favorite restaurants represented by strings.

You need to help them find out their **common interest** with the **least list index sum**. If there is a choice tie between answers, output all of them with no order requirement. You could assume there always exists an answer.

 

**Example 1:**

```swift
Input: list1 = ["Shogun","Tapioca Express","Burger King","KFC"], list2 = ["Piatti","The Grill at Torrey Pines","Hungry Hunter Steakhouse","Shogun"]
Output: ["Shogun"]
Explanation: The only restaurant they both like is "Shogun".
```

**Example 2:**

```swift
Input: list1 = ["Shogun","Tapioca Express","Burger King","KFC"], list2 = ["KFC","Shogun","Burger King"]
Output: ["Shogun"]
Explanation: The restaurant they both like and have the least index sum is "Shogun" with index sum 1 (0+1).
```

**Example 3:**

```swift
Input: list1 = ["Shogun","Tapioca Express","Burger King","KFC"], list2 = ["KFC","Burger King","Tapioca Express","Shogun"]
Output: ["KFC","Burger King","Tapioca Express","Shogun"]
```

**Example 4:**

```swift
Input: list1 = ["Shogun","Tapioca Express","Burger King","KFC"], list2 = ["KNN","KFC","Burger King","Tapioca Express","Shogun"]
Output: ["KFC","Burger King","Tapioca Express","Shogun"]
```

**Example 5:**

```swift
Input: list1 = ["KFC"], list2 = ["KFC"]
Output: ["KFC"]
```

 

**Constraints:**
- `1 <= list1.length, list2.length <= 1000`
- `1 <= list1[i].length, list2[i].length <= 30`
- `list1[i]` and `list2[i]` consist of spaces `' '` and English letters.
- All the stings of `list1` are **unique**.
-  All the stings of `list2` are **unique**.

题意：假设Andy和Doris想在晚餐时选择一家餐厅，并且他们都有一个表示最喜爱餐厅的列表，每个餐厅的名字用字符串表示。帮助他们用**最少的索引和**找出他们**共同喜爱的餐厅**。 如果答案不止一个，则输出所有答案并且不考虑顺序。可以假设总是存在一个答案。

----
### 解法 哈希集合
这一道题可以看作求两个列表的交集，然后从中寻找具有**最小索引和**的一个或多个餐厅。具体代码如下：
```cpp
class Solution {
public:
    vector<string> findRestaurant(vector<string>& list1, vector<string>& list2) {
        int n = list1.size(), m = list2.size(), minIndexSum = n + m - 2;
        vector<string> ans;
        unordered_map<string, int> rec1;
        for (int i = 0; i < n; ++i) rec1[list1[i]] = i;
        for (int i = 0; i < m; ++i) {
            if (rec1.find(list2[i]) != rec1.end()) { //相同的兴趣
                int idxSum = rec1[list2[i]] + i;
                if (idxSum == minIndexSum) { //等于则添加到ans中,必须在判读<之前
                    ans.push_back(list2[i]);
                } else if (idxSum < minIndexSum) { //小于minIndexSum则更新
                    ans.clear();
                    ans.push_back(list2[i]);
                    minIndexSum = idxSum;
                }
            }
        }
        return ans;
    }
};
```
提交后效率如下：
```cpp
执行用时：136 ms, 在所有 C++ 提交中击败了94.70% 的用户
内存消耗：36.6 MB, 在所有 C++ 提交中击败了19.06% 的用户
```
