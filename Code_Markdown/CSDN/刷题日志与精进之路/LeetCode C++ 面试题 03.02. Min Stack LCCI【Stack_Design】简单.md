How would you design a stack which, in addition to push and pop, has a function min which returns the minimum element? Push, pop and min should all operate in `O(1)` time.

**Example:**

```clike
MinStack minStack = new MinStack();
minStack.push(-2);
minStack.push(0);
minStack.push(-3);
minStack.getMin();   --> return -3.
minStack.pop();
minStack.top();      --> return 0.
minStack.getMin();   --> return -2.
```

题意：设计一个最小栈，`push, pop, min` 三个函数的时间复杂度均为 $O(1)$ 。

---
### 解法1 最小值数组
一个有意思的事情是，最小值不经常改变，只有当更小的元素被添加到栈中时它才会发生变化。因此一种方法是：使用一个单独的整型值 `minValue` 作为栈类型的成员变量，当更小的元素 `push` 进栈的时候，更新 `minValue` ；当 `minValue` 从栈中 `pop` 出去后，我们搜索整个栈，得到新的最小值。然而这样打破了题目限制，`pop` 无法做到 $O(1)$ 时间。

让我们举一个简单的例子：
```cpp
push(5);	//stack: 5, minArray = 5
push(6);	//stack: 5 6, minArray = 5 5
push(3);	//stack: 5 6 3, minArray = 5 5 3
push(7);	//stack: 5 6 3 7, minArray = 5 5 3 3
pop();		//pop 7, stack: 5 6 3, minArray = 5 5 3
pop();		//pop 3, stack: 5 6, minValue = 5 5
```
观察发现，弹出3之后栈回到之前的状态 `[5 6]` ，最小值也回到了之前的状态。这就是第二种方法，如果我们跟踪每个状态下的最小值，就可以很容易地知道当前需要的最小值，方法是**让每个节点记录自身进栈时的最小值**。要找到最小值，只需查看栈顶元素存储的最小值。 对应的Java代码如下：
```java
class NodeWithMin {
    public int value;
    public int min;
    public NodeWithMin(int v, int min) {
        value = v;
        this.min = min;
    }
}

class MinStack {
    private Stack<NodeWithMin> stack;
    /** initialize your data structure here. */
    public MinStack() {
        stack = new Stack<NodeWithMin>();
    }
    
    public void push(int x) {
        int newMin = Math.min(x, getMin());
        stack.add(new NodeWithMin(x, newMin));
    }
    
    public void pop() {
        stack.pop();
    }
    
    public int top() { //peek
        if (stack.isEmpty()) return Integer.MAX_VALUE; //Error value
        return stack.peek().value;
    }
    
    public int getMin() {
        if (stack.isEmpty()) return Integer.MAX_VALUE; //Error value
        return stack.peek().min;
    }
} 
```
使用了附带最小值的结点类，效率如下：
```java
执行用时：22 ms, 在所有 Java 提交中击败了21.16% 的用户
内存消耗：40 MB, 在所有 Java 提交中击败了99.01% 的用户
```
下面是用C++写的最小值数组/链表的实现：
```cpp
class MinStack {
public:
    /** initialize your data structure here. */
    list<int> valueStack, minList;
    MinStack() { }
    
    void push(int x) {
        valueStack.push_back(x);
        if (minList.empty()) minList.push_back(x);
        else minList.push_back(min(minList.back(), x));
    }
    
    void pop() {
        valueStack.pop_back();
        minList.pop_back();
    }
    
    int top() {
        return valueStack.back();
    }
    
    int getMin() {
        return minList.back();
    }
};
```
效率如下：
```cpp
执行用时：48 ms, 在所有 C++ 提交中击败了67.02% 的用户
内存消耗：16.2 MB, 在所有 C++ 提交中击败了5.33% 的用户
```

---
### 解法2 最小值栈
第二种方法的一个缺陷是，如果我们有一个很大的栈，那么记录每个元素对应的最小值，将会浪费大量的空间，为此进行改进。还是这个简单的例子：
```cpp
push(5);	//stack: 5, minValue = 5
push(6);	//stack: 5 6, minValue = 5 
push(3);	//stack: 5 6 3, minValue = 3
push(7);	//stack: 5 6 3 7, minValue = 3
pop();		//pop 7, stack: 5 6 3, minValue = 3
pop();		//pop 3, stack: 5 6, minValue = 5  
```
我们可以发现，**最小值的变化也遵循后进先出的规律**，因此可以**使用一个额外的栈存储最小值**。为什么这样做更加节省空间？假设有一个很大的栈，第一个进栈的元素是最小值。在第二种方法中，我们会记录这个数 `n` 次；在这种方法里，我们仅仅使用一个额外的最小值栈，它只存储了一个最小值元素。

具体的C++代码如下，其中关键点是，**要保留相等的最小值**：
```cpp
class MinStack {
public:
    /** initialize your data structure here. */
    list<int> valueStack, minStack;
    MinStack() { }
    
    void push(int x) {
        valueStack.push_back(x);
        if (x <= getMin()) minStack.push_back(x); //允许相等的最小值
    }
    
    void pop() { //弹出一个值时判断它是否进入最小值栈
        if (valueStack.back() == getMin()) minStack.pop_back(); 
        valueStack.pop_back();
    }
    
    int top() {
        return valueStack.back();
    }
    
    int getMin() {
        if (minStack.empty()) return INT_MAX;
        return minStack.back();
    }
};
```
效率喜人：
```cpp
执行用时：40 ms, 在所有 C++ 提交中击败了95.28% 的用户
内存消耗：15.6 MB, 在所有 C++ 提交中击败了5.09% 的用户
```
Java代码的实现如下：
```java
class MinStack {
    private Stack<Integer> valueStack;
    private Stack<Integer> minStack;
    /** initialize your data structure here. */
    public MinStack() {
        valueStack = new Stack<Integer>();
        minStack = new Stack<Integer>();
    }
    
    public void push(int x) {
        valueStack.push(x);
        if (x <= getMin()) minStack.push(x);
    }
    
    public void pop() {
        int value = valueStack.pop();
        if (value == getMin()) minStack.pop();
    }
    
    public int top() { //peek
        if (valueStack.isEmpty()) return Integer.MAX_VALUE; //Error value
        return valueStack.peek();
    }
    
    public int getMin() {
        if (minStack.isEmpty()) return Integer.MAX_VALUE; //Error value
        return minStack.peek();
    }
}
```
效率如下：
```java
执行用时：22 ms, 在所有 Java 提交中击败了22.28% 的用户
内存消耗：40.2 MB, 在所有 Java 提交中击败了93.98% 的用户
```
