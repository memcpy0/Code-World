Design a stack that supports push, pop, top, and retrieving the minimum element in constant time.

-   `push(x)` -- Push element x onto stack.
-    `pop()` -- Removes the element on top of the stack.
-    `top()` -- Get the top element.
-    `getMin()` -- Retrieve the minimum element in the stack.

 

**Example 1**:
```cpp
Input
["MinStack","push","push","push","getMin","pop","top","getMin"]
[[],[-2],[0],[-3],[],[],[],[]]

Output
[null,null,null,null,-3,null,0,-2]

Explanation:
MinStack minStack = new MinStack();
minStack.push(-2);
minStack.push(0);
minStack.push(-3);
minStack.getMin(); // return -3
minStack.pop();
minStack.top();    // return 0
minStack.getMin(); // return -2
```



 

**Constraints**:

-    Methods `pop`, `top` and `getMin` operations will always be called on **non-empty** stacks.

题意：设计一个支持获取最小值的栈，

思路：可以用数组或链表，从最底层搭建；这里我使用的是STL的 `stack` 。

除了存储数值的栈外，还定义一个存储最小值的栈——如果新进入的值**小于等于**栈顶元素，则入栈，此时栈顶元素为当前的最小值；`getMin` 总是取最小栈的栈顶元素；如果元素出栈，发现它是最小栈的栈顶元素，则最小栈也要出栈一次。

代码：
```cpp
class MinStack {
public:
    /** initialize your data structure here. */
    stack<int> minVal, st;
    MinStack() {  }
    
    void push(int x) {
        if (minVal.empty()) minVal.push(x);
        else if (x <= minVal.top()) minVal.push(x);
        st.push(x);
    }
    
    void pop() {
        if (!st.empty()) {
            if (minVal.top() == st.top()) minVal.pop(); 
            st.pop();
        }
    }
    
    int top() { return st.top(); }
    
    int getMin() { return minVal.top(); }
};

/**
 * Your MinStack object will be instantiated and called as such:
 * MinStack* obj = new MinStack();
 * obj->push(x);
 * obj->pop();
 * int param_3 = obj->top();
 * int param_4 = obj->getMin();
 */
```
效率：
```
执行用时：40 ms, 在所有 C++ 提交中击败了89.09% 的用户
内存消耗：14.5 MB, 在所有 C++ 提交中击败了100.00% 的用户
```
