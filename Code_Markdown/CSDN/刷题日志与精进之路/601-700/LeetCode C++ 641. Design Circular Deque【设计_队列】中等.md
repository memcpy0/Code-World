Design your implementation of the circular double-ended queue (`deque`). Your implementation should support following operations:
- `MyCircularDeque(k)` : Constructor, set the size of the deque to be `k` .
- `insertFront()` : Adds an item at the front of Deque. Return `true` if the operation is successful.
- `insertLast()` : Adds an item at the rear of Deque. Return `true` if the operation is successful.
- `deleteFront()` : Deletes an item from the front of Deque. Return `true` if the operation is successful.
- `deleteLast()` : Deletes an item from the rear of Deque. Return `true` if the operation is successful.
- `getFront()` : Gets the front item from the Deque. If the deque is empty, return `-1` .
- `getRear()` : Gets the last item from Deque. If the deque is empty, return `-1` .
- `isEmpty()` : Checks whether Deque is empty or not. 
- `isFull()` : Checks whether Deque is full or not.

 

**Example:**

```swift
MyCircularDeque circularDeque = new MycircularDeque(3); // set the size to be 3
circularDeque.insertLast(1);			// return true
circularDeque.insertLast(2);			// return true
circularDeque.insertFront(3);			// return true
circularDeque.insertFront(4);			// return false, the queue is full
circularDeque.getRear();  			// return 2
circularDeque.isFull();				// return true
circularDeque.deleteLast();			// return true
circularDeque.insertFront(4);			// return true
circularDeque.getFront();			// return 4
```
**Note:**
- All values will be in the range of `[0, 1000]` .
 - The number of operations will be in the range of `[1, 1000]` .
- Please do not use the built-in Deque library.

题意：设计实现循环双端队列。

---
### 思路
实现循环双端队列时，注意下面的 `(front, rear]` 是一个左开右闭区间。代码如下：
```cpp
class MyCircularDeque {
private:
    int *deque;
    int capacity;
    int front, rear;
    int size;
public:
    /** Initialize your data structure here. Set the size of the deque to be k. */
    MyCircularDeque(int k) {
        deque = new int[capacity = k];
        front = 0, rear = 0; //(,]
        size = 0;
    }
    
    /** Adds an item at the front of Deque. Return true if the operation is successful. */
    bool insertFront(int value) {
        if (isFull()) return false;
        ++size;
        deque[front] = value;
        front = (front - 1 + capacity) % capacity;
        return true;
    }
    
    /** Adds an item at the rear of Deque. Return true if the operation is successful. */
    bool insertLast(int value) {
        if (isFull()) return false;
        ++size;
        rear = (rear + 1) % capacity;
        deque[rear] = value;
        return true;
    }
    
    /** Deletes an item from the front of Deque. Return true if the operation is successful. */
    bool deleteFront() {
        if (isEmpty()) return false;
        --size;
        front = (front + 1) % capacity;
        return true;
    }
    
    /** Deletes an item from the rear of Deque. Return true if the operation is successful. */
    bool deleteLast() {
        if (isEmpty()) return false;
        --size;
        rear = (rear - 1 + capacity) % capacity;
        return true;
    }
    
    /** Get the front item from the deque. */
    int getFront() {
        if (isEmpty()) return -1; 
        return deque[(front + 1) % capacity];
    }
    
    /** Get the last item from the deque. */
    int getRear() {
        if (isEmpty()) return -1;
        return deque[rear];
    }
    
    /** Checks whether the circular deque is empty or not. */
    bool isEmpty() {
        return size == 0;
    }
    
    /** Checks whether the circular deque is full or not. */
    bool isFull() {
        return size == capacity;
    }
}; 
```
感觉这些设计题都蛮有意思的，以后可以多做一下。
