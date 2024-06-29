Describe how you could use a single array to implement three stacks.

Yout should implement `push(stackNum, value)` 、`pop(stackNum)`、`isEmpty(stackNum)`、`peek(stackNum)` methods, `stackNum` is the index of the stack, `value` is the value that pushed to the stack.

The constructor requires a `stackSize` parameter, which represents the size of each stack.

**Example1:**

```clike
Input: 
["TripleInOne", "push", "push", "pop", "pop", "pop", "isEmpty"]
[[1], [0, 1], [0, 2], [0], [0], [0], [0]]
Output: 
[null, null, null, 1, -1, -1, true]
Explanation: When the stack is empty, `pop, peek` return -1. When the stack is full, `push` does nothing.
```

**Example2:**

```clike
Input: 
["TripleInOne", "push", "push", "push", "pop", "pop", "pop", "peek"]
[[2], [0, 1], [0, 2], [0, 3], [0], [0], [0], [0]]
Output: 
[null, null, null, null, 2, 1, -1, -1]
```

 
