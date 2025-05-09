

Implement the `RandomizedSet` class:

-  `bool insert(int val)` Inserts an item `val` into the set if not present. Returns `true` if the item was not present, `false` otherwise.
 - `bool remove(int val)` Removes an item `val` from the set if present. Returns `true` if the item was present, `false` otherwise.
 - `int getRandom()` Returns a random element from the current set of elements (it's guaranteed that at least one element exists when this method is called). Each element must have the **same probability** of being returned.

**Follow up:** Could you implement the functions of the class with each function works in **average** `O(1)` time?

 

**Example 1:**

```clike
Input
["RandomizedSet", "insert", "remove", "insert", "getRandom", "remove", "insert", "getRandom"]
[[], [1], [2], [2], [], [1], [2], []]
Output
[null, true, false, true, 2, true, false, 2]


Explanation
RandomizedSet randomizedSet = new RandomizedSet();
randomizedSet.insert(1); // Inserts 1 to the set. Returns true as 1 was inserted successfully.
randomizedSet.remove(2); // Returns false as 2 does not exist in the set.
randomizedSet.insert(2); // Inserts 2 to the set, returns true. Set now contains [1,2].
randomizedSet.getRandom(); // getRandom() should return either 1 or 2 randomly.
randomizedSet.remove(1); // Removes 1 from the set, returns true. Set now contains [2].
randomizedSet.insert(2); // 2 was already in the set, so return false.
randomizedSet.getRandom(); // Since 2 is the only number in the set, getRandom() will always return 2.
```
 

**Constraints:**
-  <code>-2<sup>31</sup> <= val <= 2<sup>31</sup> - 1</code>
- At most <code>10<sup>5</sup></code> calls will be made to `insert`, `remove`, and `getRandom`.
- There will be **at least one** element in the data structure when `getRandom` is called.

题意：设计一个支持在**平均时间复杂度** `O(1)` 下，执行以下操作的数据结构。

- `insert(val)`：当元素 `val` 不存在时，向集合中插入该项。
 - `remove(val)`：元素 `val` 存在时，从集合中移除该项。
 - `getRandom`：随机返回现有集合中的一项。每个元素应该有相同的概率被返回。
 
 ---
### 解法 哈希表+数组
要在 $O(1)$ 的时间内插入和删除元素，非哈希表（哈希集合）莫属。

https://leetcode-cn.com/problems/insert-delete-getrandom-o1/comments/ 

https://leetcode-cn.com/problems/insert-delete-getrandom-o1-duplicates-allowed/
