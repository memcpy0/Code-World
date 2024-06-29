

Implement a function to check if a linked list is a palindrome.

 

**Example 1:**

```clike
Input:  1->2
Output:  false 
```

**Example 2:**

```clike
Input:  1->2->2->1
Output:  true 
```

 

**Follow up:** Could you do it in `O(n)` time and `O(1)` space?

题意：编写一个函数，检查输入的链表是否是回文的。


---
### 解法1 辅助空间
使用额外的 $O(n)$ 空间，可以完成这一道题，做法类似回文串的判断：
```cpp
class Solution { 
public:
    bool isPalindrome(ListNode* head) { 
        if (head == nullptr || head->next == nullptr) return true;
        vector<int> temp;
        ListNode *ptr = head;
        while (ptr) {
            temp.push_back(ptr->val);
            ptr = ptr->next;
        }
        for (int i = 0, j = temp.size() - 1; i < j; ++i, --j) 
            if (temp[i] != temp[j])
                return false;
        return true;
    }
};
```
执行结果如下：
```cpp
执行用时：20 ms, 在所有 C++ 提交中击败了99.28% 的用户
内存消耗：14.4 MB, 在所有 C++ 提交中击败了15.26% 的用户
```
使用双端队列的做法如下：
```cpp
class Solution { 
public:
    bool isPalindrome(ListNode* head) { 
        if (head == nullptr || head->next == nullptr) return true;
        deque<int> temp;
        ListNode *ptr = head;
        while (ptr) {
            temp.push_back(ptr->val);
            ptr = ptr->next;
        } 
        while (!temp.empty()) {
            if (temp.front() != temp.back()) return false;
            if (temp.size() > 1) temp.pop_front();
            temp.pop_back(); //只有一个元素时就直接从后端弹出元素
        }
        return true;
    }
};
```
由于要弹出元素，效率比起**从两边向中间检测**稍微低一些：
```cpp
执行用时：28 ms, 在所有 C++ 提交中击败了77.47% 的用户
内存消耗：14.1 MB, 在所有 C++ 提交中击败了27.00% 的用户
```
---
### 解法2 递归
从链表尾部开始比较值，这种做法利用了递归自身的栈：
```cpp
class Solution {
private:
    ListNode *lo; 
    bool dfs(ListNode* head) {
        if (head == nullptr) return true;
        if (dfs(head->next) == false) return false;
        if (head->val != lo->val) return false;
        lo = lo->next;
        return true;
    }
public:
    bool isPalindrome(ListNode* head) {
        lo = head;
        return dfs(head); 
    }
};
```
运行效率稍微低一些：
```cpp
执行用时：32 ms, 在所有 C++ 提交中击败了49.03% 的用户
内存消耗：16 MB, 在所有 C++ 提交中击败了5.19% 的用户
```

---
### 解法3 常量空间
不使用额外空间，同时仍然保持 $O(n)$ 时间复杂度的做法是存在的。具体步骤如下：
- 快慢指针找到单链表的中点；
- 对后半部分的链表进行翻转；
- 双指针比较前后两部分链表；
- 再次翻转恢复中点后的链表；

快慢指针之后，如果链表长度为偶数，`lo` 指向后半部分链表的首结点，前后两段链表长度相等；如果链表长度为奇数，`lo` 指向的后段链表长度大 `1` ，比如 `1->2->1` ，`lo` 会指向 `2` 。此时可能有点疑虑——翻转 `lo` 指向的后段链表，需要区分这两种情况吗？

实际的回答是——不需要。如果是 `1->2->1` ，此时 `lo` 指向 `2` ，翻转后形成 `1->2<-1` ，前后段链表共用中间的 `2` ，`2` 指向 `null` ；如果是 `1->2->2->1` ，此时 `lo` 指向后面的 `2` ，翻转后形成 `1->2->2<-1` ，后面的 `2` 指向 `null` ，**因此比较时以后面的指针到达 `null` 为准**。之后是翻转回来的步骤，没有什么问题。

具体代码如下：
```cpp
class Solution { 
private:
    ListNode* reverseList(ListNode* head) { 
        if (head == nullptr || head->next == nullptr) return head;
        ListNode *pre = nullptr, *cur = head;
        while (cur) {
            ListNode *next = cur->next;
            cur->next = pre;
            pre = cur;
            cur = next;
        }
        return pre;
    }
public:
    bool isPalindrome(ListNode* head) { 
        if (head == nullptr || head->next == nullptr) return true;
        ListNode *lo = head, *hi = head;
        while (hi && hi->next) { //快慢指针找到中点
            hi = hi->next->next; 
            lo = lo->next;
        } //此时lo指向后半段链表的首结点
        ListNode *newHead = reverseList(lo); //翻转后半段链表,此时lo->next==nullptr
        ListNode *p = head, *q = newHead;
        while (q) { //以q到达nullptr为准
            if (p->val != q->val) return false;
            p = p->next;
            q = q->next;
        }
        reverseList(newHead); //恢复后半段链表, 函数返回lo指针
        return true;
    }
};
```
执行效率如下，由于多次翻转链表，时间效率稍微慢一点：
```cpp
执行用时：24 ms, 在所有 C++ 提交中击败了93.87% 的用户
内存消耗：13.8 MB, 在所有 C++ 提交中击败了43.54% 的用户
```
