Design your implementation of the circular queue. The circular queue is a linear data structure in which the operations are performed based on `FIFO` (First In First Out) principle and the last position is connected back to the first position to make a circle. It is also called "Ring Buffer".

One of the benefits of the circular queue is that we can make use of the spaces in front of the queue. In a normal queue, once the queue becomes full, we cannot insert the next element even if there is a space in front of the queue. But using the circular queue, we can use the space to store new values.

Your implementation should support following operations:
- `MyCircularQueue(k)` : Constructor, set the size of the queue to be `k` .
- `Front` : Get the front item from the queue. If the queue is empty, return `-1` .
- `Rear` : Get the last item from the queue. If the queue is empty, return `-1` .
- `enQueue(value)` : Insert an element into the circular queue. Return `true` if the operation is successful.
- `deQueue()` : Delete an element from the circular queue. Return `true` if the operation is successful.
- `isEmpty()` : Checks whether the circular queue is empty or not.
- `isFull()` : Checks whether the circular queue is full or not.

 

**Example:**

```swift
MyCircularQueue circularQueue = new MyCircularQueue(3); // set the size to be 3
circularQueue.enQueue(1);  // return true
circularQueue.enQueue(2);  // return true
circularQueue.enQueue(3);  // return true
circularQueue.enQueue(4);  // return false, the queue is full
circularQueue.Rear();  // return 3
circularQueue.isFull();  // return true
circularQueue.deQueue();  // return true
circularQueue.enQueue(4);  // return true
circularQueue.Rear();  // return 4
```

 

**Note:**
- All values will be in the range of `[0, 1000]` .
- The number of operations will be in the range of `[1, 1000]` .
- Please do not use the built-in Queue library.

题意：设计循环队列实现。 循环队列是一种线性数据结构，其操作表现基于 `FIFO`（先进先出）原则并且队尾被连接在队首之后以形成一个循环。也被称为"环形缓冲器"。循环队列的一个好处是可以利用这个队列之前用过的空间。在一个普通队列里，一旦一个队列"满了"，就不能插入下一个元素，即使在队列前面仍有空间。但是使用循环队列，就能使用这些空间去存储新的值。
 
---
### 思路
感觉LeetCode上的设计题都可以替代平常的数据结构实现课程的学习了。代码如下：
```cpp
class MyCircularQueue {
private:
    int *queue;
    int capacity;
    int size;
    int front, rear; //[front, rear]
public:
    /** Initialize your data structure here. Set the size of the queue to be k. */
    MyCircularQueue(int k) {
        queue = new int[capacity = k];
        size = 0;
        front = 0, rear = -1;
    }
    
    /** Insert an element into the circular queue. Return true if the operation is successful. */
    bool enQueue(int value) {
        if (isFull()) return false;
        ++size;
        rear = (rear + 1) % capacity;
        queue[rear] = value;
        return true;
    }
    
    /** Delete an element from the circular queue. Return true if the operation is successful. */
    bool deQueue() {
        if (isEmpty()) return false;
        --size;
        front = (front + 1) % capacity;
        return true;
    }
    
    /** Get the front item from the queue. */
    int Front() {
        if (isEmpty()) return -1;
        return queue[front];
    }
    
    /** Get the last item from the queue. */
    int Rear() {
        if (isEmpty()) return -1;
        return queue[rear];
    }
    
    /** Checks whether the circular queue is empty or not. */
    bool isEmpty() {
        return size == 0;
    }
    
    /** Checks whether the circular queue is full or not. */
    bool isFull() {
        return size == capacity;
    }
}; 
```
