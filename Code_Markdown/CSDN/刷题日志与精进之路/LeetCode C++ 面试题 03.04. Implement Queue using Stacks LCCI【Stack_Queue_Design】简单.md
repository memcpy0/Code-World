

Implement a MyQueue class which implements a queue using two stacks.
 

**Example:**

```swift
MyQueue queue = new MyQueue();

queue.push(1);
queue.push(2);
queue.peek();  // return 1
queue.pop();   // return 1
queue.empty(); // return false
```

 

**Notes:**
- You must use only standard operations of a stack -- which means only `push to top`, `peek/pop from top`, `size`, and `is empty` operations are valid.
 - Depending on your language, stack may not be supported natively. You may simulate a stack by using a list or deque (double-ended queue), as long as you use only standard operations of a stack.
 - You may assume that all operations are valid (for example, no pop or peek operations will be called on an empty queue).

 
题意：用两个栈实现一个队列类。

---
### 解法 一个输入栈+一个输出栈
这是最优的解法：
```cpp
class MyQueue {
public: 
    vector<int> in, out;
    MyQueue() { }
    
    /** Push element x to the back of queue. */
    void push(int x) {
        in.push_back(x);
    } 
    /** Removes the element from in front of queue and returns that element. */
    int pop() {
        if (out.empty()) {
            while (!in.empty()) {
                out.push_back(in.back());
                in.pop_back();
            }
        }
        int v = out.back();
        out.pop_back();
        return v;
    } 
    /** Get the front element. */
    int peek() {
        if (out.empty()) {
            while (!in.empty()) {
                out.push_back(in.back());
                in.pop_back();
            }            
        }
        return out.back();
    } 
    /** Returns whether the queue is empty. */
    bool empty() {
        return in.empty() && out.empty();
    }
};
```
执行效率见下：
```cpp
执行用时：0 ms, 在所有 C++ 提交中击败了100.00% 的用户
内存消耗：7.3 MB, 在所有 C++ 提交中击败了5.31% 的用户
```
