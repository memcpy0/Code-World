用两个栈实现一个队列。队列的声明如下，请实现它的两个函数 `appendTail ` 和 `deleteHead` ，分别完成在队列尾部插入整数和在队列头部删除整数的功能。(若队列中没有元素，`deleteHead` 操作返回 `-1` )

示例 1：
```cpp
输入：
["CQueue","appendTail","deleteHead","deleteHead"]
[[],[3],[],[]]
输出：[null,null,3,-1]
```

示例 2：

```cpp
输入：
["CQueue","deleteHead","appendTail","appendTail","deleteHead","deleteHead"]
[[],[],[5],[2],[],[]]
输出：[null,-1,null,null,5,2]
```

提示：
-    `1 <= values <= 10000`
-    最多会对 `appendTail、deleteHead` 进行 `10000` 次调用


题意：使用栈实现队列的队尾插入和队首删除功能。

思路1：**做过的题目**；使用一个栈，插入时直接进入栈顶；删除时需要把全部元素弹出并插入到临时栈中，此时要删除的元素在临时栈栈顶，删除即可；然后重新插入回原栈中。

代码1：

```cpp
class CQueue {
public:
    stack<int> st;
    CQueue() {}
    
    void appendTail(int value) {
        st.push(value);
    }
    
    int deleteHead() {
        if (st.empty()) return -1;
        stack<int> temp;
        while (!st.empty()) {
            temp.push(st.top());
            st.pop();
        } 
        int ans = temp.top(); temp.pop();
        while (!temp.empty()) {
            st.push(temp.top());
            temp.pop();
        }
        return ans;
    }
};
/**
 * Your CQueue object will be instantiated and called as such:
 * CQueue* obj = new CQueue();
 * obj->appendTail(value);
 * int param_2 = obj->deleteHead();
 */
```

效率：耗费空间少，但是时间慢，因为在删除元素时浪费了太多时间。
```
执行用时：1300 ms, 在所有 C++ 提交中击败了5.89% 的用户
内存消耗：212 MB, 在所有 C++ 提交中击败了100.00% 的用户
```
---

思路2：使用 `input` 和 `output` 两个栈。删除元素时仅需删除 `output` 栈顶元素即可。

代码2：
```cpp
class CQueue {
public:
    stack<int> input, output;
    CQueue() {}
    
    void appendTail(int value) {
        input.push(value);
    }
    
    int deleteHead() {
        if (input.empty() && output.empty()) return -1;
        if (output.empty()) {
            while (!input.empty()) {
                output.push(input.top());
                input.pop();
            }
        }
        int ans = output.top(); output.pop();
        return ans;
    }
};

/**
 * Your CQueue object will be instantiated and called as such:
 * CQueue* obj = new CQueue();
 * obj->appendTail(value);
 * int param_2 = obj->deleteHead();
 */
```
效率：
```
执行用时：728 ms, 在所有 C++ 提交中击败了55.06% 的用户
内存消耗：103.6 MB, 在所有 C++ 提交中击败了100.00% 的用户
```
