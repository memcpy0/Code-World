### equals判断相等，hashcode也相等吗？
分情况，equals没重写，默认就是判断的hashcode，相等；**如果equals重写了，hashcode也要重写**，否则，会出现不相等。java规定，两个对象相等，hashcode也要相等。
### hashcode相等，对象相等吗
不一定，哈希冲突