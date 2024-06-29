Key能否为null，作为key的对象有什么要求？TreeMap，HashTable的key都不能为null
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303232219030.png)

更准确地说：算出hash是为了最终得出一个索引，而计算hash的方式得尽量分布均匀
hashCode相同->不一定equals，equals->一定hashCode相同。
### 6）key 的设计
**key 的设计要求**
1. **HashMap 的 key 可以为 null**，但 Map 的其他实现则不然
2. **作为 key 的对象，必须实现 hashCode 和 equals，并且 key 的内容不能修改（不可变）**
3. **key 的 hashCode 应该有良好的散列性**

如果 key 可变，例如修改了 age 会导致再次查询时查询不到
```java
public class HashMapMutableKey {
    public static void main(String[] args) {
        HashMap<Student, Object> map = new HashMap<>();
        Student stu = new Student("张三", 18);
        map.put(stu, new Object());

        System.out.println(map.get(stu));

        stu.age = 19;
        System.out.println(map.get(stu));
    }

    static class Student {
        String name;
        int age;

        public Student(String name, int age) {
            this.name = name;
            this.age = age;
        }

        public String getName() {
            return name;
        }

        public void setName(String name) {
            this.name = name;
        }

        public int getAge() {
            return age;
        }

        public void setAge(int age) {
            this.age = age;
        }

        @Override
        public boolean equals(Object o) {
            if (this == o) return true;
            if (o == null || getClass() != o.getClass()) return false;
            Student student = (Student) o;
            return age == student.age && Objects.equals(name, student.name);
        }

        @Override
        public int hashCode() {
            return Objects.hash(name, age);
        }
    }
}
```
**String 对象的 hashCode() 设计**
* 目标是**达到较为均匀的散列效果**，每个字符串的 hashCode 足够独特
* 字符串中的每个字符都可以表现为一个数字，称为 $S_i$，其中 i 的范围是 0 ~ n - 1 
* 散列公式为： $S_0∗31^{(n-1)}+ S_1∗31^{(n-2)}+ … S_i ∗ 31^{(n-1-i)}+ …S_{(n-1)}∗31^0$
* **31 代入公式的散列特性较好**，并且 **31 * h 可以被优化**（计算性能好）为 
  * 即 $32 ∗h -h$
  * 即 $2^5  ∗h -h$
  * 即 $h≪5  -h$
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303232224443.png)

 