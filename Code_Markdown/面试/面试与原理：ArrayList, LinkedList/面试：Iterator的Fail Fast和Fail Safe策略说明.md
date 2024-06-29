**要求**：掌握什么是 Fail-Fast、什么是 Fail-Safe

**Iterator是遍历集合的**，有两种策略：
- Fail-Fast：一旦发现遍历的同时其他人来修改，则**立即抛出异常、尽快失败**，不允许其他人修改
- Fail-Safe：发现遍历的同时其他人来修改，应当能有应对策略，例如牺牲一致性来让整个遍历运行完成，**遍历的同时可以修改，原理是读写分离**——牺牲一致性，就是指遍历的拷贝后的数组
* **ArrayList 是 fail-fast 的典型代表，遍历的同时不能修改，尽快失败**
* CopyOnWriteArrayList 是 fail-safe 的典型代表（**线程安全的集合**）

断点还可以加条件：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303231806628.png)

**提示**
* 测试代码见 `day01.list.FailFastVsFailSafe`，这里不再列出

---
源码 expectedModCount = modCount（**外面List已经记录的修改次数**），创建时记录集合修改了多少次。

==expectedModCount是在开始遍历前记录的「外面List已经修改的次数），如果遍历过程中其他人修改了List，则外面的modCount会++，和迭代器中记录的expectedModCount不同（在next方法中检查是否不同）==

多版本并发控制：调用checkForComodification
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303231834752.png)
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303231833012.png)

调试：
![1000](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303231836204.png)

---
**CopyOnWriteArrayList**：
![1000](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303231839404.png)

创建迭代器时，记录一个数组到snapshot，![1000](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303231839430.png)

调试中用调试器给它**加一个E元素**（迭代器中的是四个元素，即**遍历的是旧数组**；列表中的五个元素）：
![1000](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303231841601.png)

**每次调用使用synchronized了的add方法，会复制一份原来的数组并扩容加1，新加的元素加在复制出的新数组**（Copy On Write，在写时拷贝），添加结束后会setArray将复制出的新数组替换**旧快照数组**：
![600](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303231842083.png)

Vector在添加前也会对比是否一致，不一致则throw new ConcurrentModificationException();
vector源码看到modCount，是fail-fast