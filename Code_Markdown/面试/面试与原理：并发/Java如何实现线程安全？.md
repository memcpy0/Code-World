- 使用 Atomic 类，通过 CAS 保证操作不会在执行过程中被中断
- 使用 synchronized、volatile 进行加锁
- 需要对集合容器内部的方法进行加锁，以 map 为例，由于 hashMap 内部没有锁，所以它会导致线程不安全，而 而如果我们使用 hashTable，由于其中使用 synchronized 关键字进行了加锁，就可以保证线程安全。
- 使用 TLS 避免资源竞争，提供线程的副本进行同时访问和维护。
 