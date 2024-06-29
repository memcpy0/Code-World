`EnumSet` 和 `EnumMap` 是 Java 标准库中专门针对枚举类型设计的集合类。

**EnumSet 是基于位向量实现的**，它将每个枚举值映射到一个二进制位上。由于**枚举类型的数量是固定的**，因此 EnumSet 可以通过位运算来高效地实现集合操作。同时，由于**枚举类型是不可变的**，所以 EnumSet 的元素也是不可变的，**这使得 EnumSet 可以保证线程安全**。

**EnumMap 是基于数组实现的**。与普通的数组不同的是，EnumMap 中的**每一个元素都是由枚举类型的元素作为下标索引的**。这种设计可以使得 EnumMap 的查找和更新操作的时间复杂度均为 O(1)，而且空间效率也比一般的 Map 实现更高。

java.util.EnumSet 是**只能包含特定枚举类**的对象 Set；java.util.EnumMap 是**键只能是特定枚举类的对象**的 Map。大家可以想一想为什么要单独设计它们呢？

了解哈希表的朋友就会知道，把一个元素存入哈希表的时候，需要计算这个元素的哈希值；而==一个枚举类的对象个数只有有限个，我们在编写枚举类时就已罗列好了枚举类的对象==，因此将枚举类的对象存入 Set 或者 Map 的时候，其实==不用计算哈希值==，就使用 ordinal() 方法返回的值就可以了。下面是 java.util.EnumMap; 的 put() 方法的代码：
```java
public V put(K key, V value) {
    typeCheck(key);

    int index = key.ordinal();
    Object oldValue = vals[index];
    vals[index] = maskNull(value);
    if (oldValue == null)
        size++;
    return unmaskNull(oldValue);
}
```
下面是使用例子：
```java
package cn.leetcode;
import java.util.EnumMap;
import java.util.Map;
import java.util.Set;

public class EnumMapExample {

    public static void main(String[] args) {
        EnumMap<LeetCodeProblemLevelEnum, Integer> enumMap = new EnumMap<>(LeetCodeProblemLevelEnum.class);
        enumMap.put(LeetCodeProblemLevelEnum.EASY, 3);
        enumMap.put(LeetCodeProblemLevelEnum.MEDIUM, 4);
        enumMap.put(LeetCodeProblemLevelEnum.HARD, 6);

        System.out.println(enumMap.size());
        Set<Map.Entry<LeetCodeProblemLevelEnum, Integer>> entries = enumMap.entrySet();
        for (Map.Entry<LeetCodeProblemLevelEnum, Integer> entry : entries) {
            System.out.println(entry.getKey().getName() + " 类型的问题在周赛中的得分：" + entry.getValue());
        }
    }
    
}
```
输出：
简单 类型的问题在周赛中的得分：3
中等 类型的问题在周赛中的得分：4
困难 类型的问题在周赛中的得分：6

下面是 使用的例子：
```java
package cn.leetcode;

import java.util.EnumSet;

class EnumSetExample {

    public static void main(String args[]) {
        EnumSet<LeetCodeProblemLevelEnum> mustGet = EnumSet.of(LeetCodeProblemLevelEnum.EASY, LeetCodeProblemLevelEnum.MEDIUM);
        System.out.println("必须掌握的问题等级：");
        for (LeetCodeProblemLevelEnum leetCodeProblemLevelEnum : mustGet) {
            System.out.println(leetCodeProblemLevelEnum.getName());
        }
    }
    
}
```
输出：
```
必须掌握的问题等级：
简单
中等
```

参考资料
https://docs.oracle.com/javase/8/docs/technotes/guides/language/enums.html
https://docs.oracle.com/javase/tutorial/reflect/special/enum.html

 