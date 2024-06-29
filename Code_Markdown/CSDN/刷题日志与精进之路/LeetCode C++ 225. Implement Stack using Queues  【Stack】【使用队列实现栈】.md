Implement the following operations of a stack using queues.
```cpp
push(x) -- Push element x onto stack.
pop() -- Removes the element on top of the stack.
top() -- Get the top element.
empty() -- Return whether the stack is empty.
```
Example:
```cpp
MyStack stack = new MyStack();

stack.push(1);
stack.push(2);  
stack.top();   // returns 2
stack.pop();   // returns 2
stack.empty(); // returns false
```
Notes:

-    You must use only standard operations of a queue -- which means only push to back, peek/pop from front, size, and is empty operations are valid.
-    Depending on your language, queue may not be supported natively. You may simulate a queue by using a list or deque (double-ended queue), as long as you use only standard operations of a queue.
-    You may assume that all operations are valid (for example, no pop or top operations will be called on an empty stack).

题意：使用Queues来模拟栈的各种方法，如push、pop(这里返回队首值)、peek(得到队首值)、empty等方法。而且最好只使用queue的标准操作，在C++中是`push/pop/front/size/empty`。

思路：使用Queues，这里的栈是复数！但是也可以从头到尾只使用一个栈。

 
### 解决方法一：改造输出方法pop和top+两个临时队列；优化到不需要临时队列
pop和top两个方法中，都使用临时队列。**如果要得到栈顶值，就将原来的队列中的值全部弹出，并依次push进临时队列中，其中最后一个元素被记录而不被放进临时队列，这就是栈的栈顶元素**，实现了pop方法。 而top方法中记录最后一个元素，同时也将其放进临时队列。然后全部弹出返回原来的队列就可以了。
- 执行用时 :4 ms, 在所有 C++ 提交中击败了66.24% 的用户
内存消耗 :9.5 MB, 在所有 C++ 提交中击败了5.43%的用户
```cpp 
class MyStack {
public:
    /** Initialize your data structure here. */
    queue<int> q;
    MyStack() { }
    
    /** Push element x onto stack. */
    void push(int x) { 
        q.push(x);
    }
    
    /** Removes the element on top of the stack and returns that element. */
    int pop() {
        queue<int> temp;
        while (q.size() > 1) {
            temp.push(q.front());
            q.pop();
        }
        int data = q.front(); q.pop(); //去除最后一个元素
        while (!temp.empty()) { //还原原本的元素
            q.push(temp.front());
            temp.pop();
        }
        return data;
    }
    
    /** Get the top element. */
    int top() { 
        queue<int> temp;
        while (q.size() > 1) {
            temp.push(q.front());
            q.pop();
        }
        int data = q.front(); //得到栈顶元素
        temp.push(data); //不删除栈顶元素
        q.pop(); 
        while (!temp.empty()) {
            q.push(temp.front());
            temp.pop();
        }
        return data;
    }
    
    /** Returns whether the stack is empty. */
    bool empty() {
        return q.empty();
    }
};
```
但是C++的queue提供了一个**直接得到队列尾元素的方法back()**，可以直接得到栈顶，简化了代码。当然这儿使用back()是`犯规`的。
```cpp
//执行用时 :4 ms, 在所有 C++ 提交中击败了66.24% 的用户
//内存消耗 :9.5 MB, 在所有 C++ 提交中击败了5.43%的用户
class MyStack {
public:
    /** Initialize your data structure here. */
    queue<int> q;
    MyStack() { }
    
    /** Push element x onto stack. */
    void push(int x) { 
        q.push(x);
    }
    
    /** Removes the element on top of the stack and returns that element. */
    int pop() {
        queue<int> temp; 
        int data = q.back(); //C++的方法
        while (!q.empty()) {
            if (q.front() != data) 
                temp.push(q.front());
            q.pop();
        } 
        while (!temp.empty()) { //还原原来的元素
            q.push(temp.front());
            temp.pop();
        }
        return data;
    }
    
    /** Get the top element. */
    int top() {
        return q.back(); //C++的方法 
    }
    
    /** Returns whether the stack is empty. */
    bool empty() {
        return q.empty();
    }
};
```
不过这里我们可以优化，不像栈只能一端进出，我们在弹出原来的队列元素时可以直接加到队列queue的后面，即**边删除边添加**。这样就可以**抛弃临时队列**了。
```cpp
class MyStack {
public:
    /** Initialize your data structure here. */
    queue<int> q;
    MyStack() { }
    
    /** Push element x onto stack. */
    void push(int x) { 
        q.push(x);
    }
    
    /** Removes the element on top of the stack and returns that element. */
    int pop() { 
        int size = q.size();
        while (size > 1) {
            q.push(q.front()); //边删除边添加
            q.pop();
            --size;
        }
        int data = q.front(); q.pop(); //不添加栈顶元素
        return data;
    }
    
    /** Get the top element. */
    int top() {
        int size = q.size();
        while (size > 1) {
            q.push(q.front());
            q.pop();
            --size;
        }
        int data = q.front(); q.pop();
        q.push(data);
        return data;
    }
    
    /** Returns whether the stack is empty. */
    bool empty() {
        return q.empty();
    }
};
```


### 解法二：改造输入方法push+一个临时队列；★ 优化到不需要临时队列
**仅仅**在push方法中使用临时队列，**每次要push一个元素时，就将先前的元素全部弹出放到临时队列中，然后将新的元素放到队首，再把其他的元素放在新元素的后面**，这样就可以用原来的队列实现后进先出了。从这里可以看出，**我们总得在一方面下功夫，要么是输入，要么是输出**。
- 执行用时 :0 ms, 在所有 C++ 提交中击败了100.00% 的用户
内存消耗 :9.4 MB, 在所有 C++ 提交中击败了5.43%的用户
```cpp
class MyStack {
public:
    /** Initialize your data structure here. */
    queue<int> q;
    MyStack() { }
    
    /** Push element x onto stack. */
    void push(int x) { //在输入上, 将放在栈顶的x放进队首
        queue<int> temp;
        while (!q.empty()) {
            temp.push(q.front());
            q.pop();
        }
        q.push(x);
        while (!temp.empty()) {
            q.push(temp.front());
            temp.pop();
        }
    }
    
    /** Removes the element on top of the stack and returns that element. */
    int pop() {
        int data = q.front(); //得到栈顶元素
        q.pop();
        return data;
    }
    
    /** Get the top element. */
    int top() {
        return q.front();
    }
    
    /** Returns whether the stack is empty. */
    bool empty() {
        return q.empty();
    }
};

/**
 * Your MyStack object will be instantiated and called as such:
 * MyStack* obj = new MyStack();
 * obj->push(x);
 * int param_2 = obj->pop();
 * int param_3 = obj->top();
 * bool param_4 = obj->empty();
 */
 ```
★ 运用上面的思想，可以不用临时队列，直接使用`q`就可以。也是==本题最优雅最巧妙的解法==。只针对push做特殊处理，其他函数直接返回即可。每次**先**`push`一个新元素到队尾，**再**把队列中的其他元素重新排到新元素的后面：
```
1 : a
2 : a b -> b a
3 : b a c -> c b a
4 : c b a d -> d c b a
```
```cpp
class MyStack {
public:
    /** Initialize your data structure here. */
    queue<int> q;
    MyStack() { }
    
    /** Push element x onto stack. */
    void push(int x) {  
        q.push(x); //把x放在队尾
        int size = q.size();
        while (size > 1) { //调整使得x位于队首
            q.push(q.front());
            q.pop();
            --size;
        }
    }
    
    /** Removes the element on top of the stack and returns that element. */
    int pop() {  
        int data = q.front(); q.pop();
        return data;
    }
    
    /** Get the top element. */
    int top() { 
        return q.front();
    }
    
    /** Returns whether the stack is empty. */
    bool empty() {
        return q.empty();
    }
};
```
