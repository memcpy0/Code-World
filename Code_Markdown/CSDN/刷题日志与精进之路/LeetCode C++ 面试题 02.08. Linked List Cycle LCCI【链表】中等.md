Given a circular linked list, implement an algorithm that returns the node at the beginning of the loop.

Circular linked list: A (corrupt) linked list in which a node's next pointer points to an earlier node, so as to make a loop in the linked list.

**Example 1:**
```clike
Input: head = [3,2,0,-4], pos = 1
Output: tail connects to node index 1
```
**Example 2:**
```clike
Input: head = [1,2], pos = 0
Output: tail connects to node index 0
```
**Example 3:**
```clike
Input: head = [1], pos = -1
Output: no cycle
```

 

