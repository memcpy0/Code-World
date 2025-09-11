

Given `head` which is a reference node to a singly-linked list. The value of each node in the linked list is either 0 or 1. The linked list holds the binary representation of a number.

Return *the decimal value* of the number in the linked list.

 

**Example 1:**
![在这里插入图片描述](https://img-blog.csdnimg.cn/20201111230152199.png#pic_center)

```swift
Input: head = [1,0,1]
Output: 5
Explanation: (101) in base 2 = (5) in base 10
```

**Example 2:**

```swift
Input: head = [0]
Output: 0
```

**Example 3:**

```swift
Input: head = [1]
Output: 1
```

**Example 4:**

```swift
Input: head = [1,0,0,1,0,0,1,1,1,0,0,0,0,0,0]
Output: 18880
```

**Example 5:**

```swift
Input: head = [0,0]
Output: 0
```

 

**Constraints:**
- The Linked List is not empty.
 - Number of nodes will not exceed `30`.
 - Each node's value is either `0` or `1`.

题意：给出一个单链表的引用结点 `head` 。链表中每个结点的值不是 `0` 就是 `1` 。已知此链表是一个整数数字的二进制表示，返回该链表所表示数字的十进制值。

---
### 解法 简单遍历
```cpp
class Solution {
public:
    int getDecimalValue(ListNode* head) {
        int ans = 0;
        for (; head; head = head->next) ans = ans * 2 + head->val;
        return ans;
    }
};
```
效率见下：
```cpp
执行用时：4 ms, 在所有 C++ 提交中击败了49.74% 的用户
内存消耗：8.4 MB, 在所有 C++ 提交中击败了12.03% 的用户
```

 
