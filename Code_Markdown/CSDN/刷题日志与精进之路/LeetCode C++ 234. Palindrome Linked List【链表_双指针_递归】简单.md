Given a singly linked list, determine if it is a palindrome.

**Example 1:**

```swift
Input: 1->2
Output: false
```

**Example 2:**

```swift
Input: 1->2->2->1
Output: true
```

**Follow up:** Could you do it in $O(n)$ time and $O(1)$ space?


题意：判断一个单链表是否是回文链表。

---
### 解法1 复制链表值
一种容易想到的、$O(n)$ 时间和空间的解法是，将单链表的值复制到数组中，然后进行判断：
```cpp
class Solution { 
public:
    bool isPalindrome(ListNode* head) {
        if (head == nullptr || head->next == nullptr) return true;
        vector<int> temp;
        while (head) {
            temp.push_back(head->val);
            head = head->next;
        }
        for (int i = 0, j = temp.size() - 1; i < j; ++i, --j) 
            if (temp[i] != temp[j]) return false;
        return true;
    }
};
```
效率如下：
```cpp
执行用时：32 ms, 在所有 C++ 提交中击败了73.09% 的用户
内存消耗：14.7 MB, 在所有 C++ 提交中击败了11.08% 的用户
```

---

### 解法2 递归比较
使用一个指针从前往后遍历，再利用递归的返回过程从后往前回溯，比较两者的值是否相等：
```cpp
class Solution {
private:
    ListNode *cur;
    bool check(ListNode* head) {
        if (head == nullptr) return true;
        if (check(head->next) == false) return false;
        if (cur->val != head->val) return false;
        cur = cur->next;
        return true;
    } 
public:
    bool isPalindrome(ListNode* head) {
        if (head == nullptr || head->next == nullptr) return true;
        cur = head;
        return check(head);
    }
};
```
效率如下：
```cpp
执行用时：28 ms, 在所有 C++ 提交中击败了90.82% 的用户
内存消耗：16.3 MB, 在所有 C++ 提交中击败了5.02% 的用户
```

---
### 解法3 快慢指针+链表反转
使用快慢指针找到链表的中点，然后逆转后半部分的链表，接着双指针比较即可：
```cpp
class Solution {
public:
    bool isPalindrome(ListNode* head) {
        if (head == nullptr || head->next == nullptr) return true;
        ListNode *slow = head, *fast = head, *prev = nullptr; 
        while (fast && fast->next) { 
            slow = slow->next;
            fast = fast->next->next;
        }
        //如果链表长度为偶数,则slow指向后半部分链表的头部; 否则slow指向链表的中心结点
        while (slow) { //反转后半部分链表
            ListNode *temp = slow->next;
            slow->next = prev;
            prev = slow;
            slow = temp;
        }
        ///如果是奇数长度的链表如[1,0,2,0,1],反转后为[1,0,1,0,2],2不会被比较
        while (head && prev) { //双指针比较
            if (head->val != prev->val) return false; 
            head = head->next;
            prev = prev->next;
        }
        return true;
    }
};
```
时间为 $O(n)$ ，空间为 $O(1)$ ，效率比较高（尽管会有修改原链表的副作用）：
```cpp
执行用时：20 ms, 在所有 C++ 提交中击败了99.78% 的用户
内存消耗：13.9 MB, 在所有 C++ 提交中击败了45.99% 的用户
```
