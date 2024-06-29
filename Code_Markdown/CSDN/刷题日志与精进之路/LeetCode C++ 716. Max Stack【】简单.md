<p>Design a max stack data structure that supports the stack operations and supports finding the stack's maximum element.</p>

<p>Implement the <code>MaxStack</code> class:</p>

<ul>
	<li><code>MaxStack()</code> Initializes the stack object.</li>
	<li><code>void push(int x)</code> Pushes element <code>x</code> onto the stack.</li>
	<li><code>int pop()</code> Removes the element on top of the stack and returns it.</li>
	<li><code>int top()</code> Gets the element on the top of the stack without removing it.</li>
	<li><code>int peekMax()</code> Retrieves the maximum element in the stack without removing it.</li>
	<li><code>int popMax()</code> Retrieves the maximum element in the stack and removes it. If there is more than one maximum element, only remove the <strong>top-most</strong> one.</li>
</ul>

 
<p><strong>Example 1:</strong></p>

```haskell
Input
["MaxStack", "push", "push", "push", "top", "popMax", "top", "peekMax", "pop", "top"]
[[], [5], [1], [5], [], [], [], [], [], []]
Output
[null, null, null, null, 5, 5, 1, 5, 1, 5]

Explanation
MaxStack stk = new MaxStack();
stk.push(5);   // [5] the top of the stack and the maximum number is 5.
stk.push(1);   // [5, 1] the top of the stack is 1, but the maximum is 5.
stk.push(5);   // [5, 1, 5] the top of the stack is 5, which is also the maximum, because it is the top most one.
stk.top();     // return 5, [5, 1, 5] the stack did not change.
stk.popMax();  // return 5, [5, 1] the stack is changed now, and the top is different from the max.
stk.top();     // return 1, [5, 1] the stack did not change.
stk.peekMax(); // return 5, [5, 1] the stack did not change.
stk.pop();     // return 1, [5] the top of the stack and the max element is now 5.
stk.top();     // return 5, [5] the stack did not change.
```

 
 
<p><strong>Constraints:</strong></p>

<ul>
	<li><code>-10<sup>7</sup> &lt;= x &lt;= 10<sup>7</sup></code></li>
	<li>At most <code>10<sup>4</sup></code> calls will be made to <code>push</code>, <code>pop</code>, <code>top</code>, <code>peekMax</code>, and <code>popMax</code>.</li>
	<li>There will be <strong>at least one element</strong> in the stack when <code>pop</code>, <code>top</code>, <code>peekMax</code>, or <code>popMax</code> is called.</li>
</ul>
 
<strong>Follow up:</strong> Could you come up with a solution that supports <code>O(1)</code> for each <code>top</code> call and <code>O(logn)</code> for each other call?&nbsp;

题意：设计一个最大栈数据结构，既支持栈操作，又支持查找栈中最大元素。实现 `MaxStack` 类：
 - `MaxStack()` 初始化栈对象；
  - `void push(int x)` 将元素 `x` 压入栈中；
  - `int pop()` 移除栈顶元素并返回这个元素；
  - `int top()` 返回栈顶元素，无需移除；
   - `int peekMax()` 检索并返回栈中最大元素，无需移除；
-  `int popMax()` 检索并返回栈中最大元素，并将其移除。如果有多个最大元素，只要移除 **最靠近栈顶** 的那个。 
