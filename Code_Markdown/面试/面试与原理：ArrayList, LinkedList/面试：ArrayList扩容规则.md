**扩容规则**
1. **调用无参构造方法 `ArrayList()` 会在构造时，使用长度为零的数组**；无参是0，在添加第一个之后**初始容量变为10，只不过要在第一次add方法调用时才进行初始化**；这是Java8的方式，在添加元素时创建容量为10的数组，Java8以前是无参构造时创建容量为10的数组；JDK7是创建就初始化，JDK8是懒加载方式，
2. `ArrayList(int initialCapacity`) 会在构造时，使用指定容量的数组
3. `public ArrayList(Collection<? extends E> c)` 会在构造时，使用 `c` 的大小作为数组容量
4. **`add(Object o)` 首次扩容（大小不够时）为 10（初始容量为0时），（初始容量不为0）再次要扩容时为上次容量的 1.5 倍**（移位运算实现，`len + (len >>> 2)`
5. `addAll(Collection c)` 没有元素时，扩容为 `Math.max(10, 实际元素个数)` ，有元素或~~之前 `elementData != DEFAULTCAPACITY_EMPTY_ELEMENTDATA` ~~时为 `Math.max(原容量 1.5 倍, 实际元素个数=已有+新加)`
	![900](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303231748809.png)
其中第 4 点必须知道，其它几点视个人情况而定

**提示**
* 测试代码见 `day01.list.TestArrayList` ，这里不再列出
* 要**注意**的是，**示例中用反射方式来更直观地反映 ArrayList 的扩容特征**，但从 JDK 9 由于模块化的影响，对反射做了较多限制，需要在运行测试代码时添加 VM 参数 `--add-opens java.base/java.util=ALL-UNNAMED` 方能运行通过，后面的例子都有相同问题

> ***代码说明***
> * day01.list.TestArrayList#arrayListGrowRule 演示了 add(Object) 方法的扩容规则，输入参数 n 代表打印多少次扩容后的数组长度
