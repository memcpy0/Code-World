There are three types of edits that can be performed on strings: insert a character, remove a character, or replace a character. Given two strings, write a function to check if they are one edit (or zero edits) away.

**Example 1:**
```swift
Input: 
first = "pale"
second = "ple"
Output: True
```
**Example 2:**
```swift
Input: 
first = "pales"
second = "pal"
Output: False
```
题意：有三种字符串编辑操作：插入一个字符、删除一个字符、替换一个字符。给定两个字符串，编写一个函数判定它们是否只需要一次（或零次）编辑。

----
### 解法 分析操作的意义
有一种蛮力算法可以解决这一题——通过**测试每个字符的删除**（并进行比较）、**测试每个字符的替换**（并进行比较），然后**测试每个可能字符的插入**（并进行比较），来检查所有可能的字符串。当然，这太慢了，所以不必费心去实现它。

这是一个问题，我们需要思考这些操作的意义。两个字符串彼此之间的区别不过是一次插入、替换、删除，这意味着什么？
- 替换 `Replacement` ：考虑两个字符串，例如 `bale` 和 `pale` ，它们是一个替换。是的，这意味着可以替换 `bale` 中的一个字符使其变为 `pale` 。但更确切地说，**这意味着它们只在一个地方不同。**
- 插入 `Insertion` ：`apple` 和 `aple` 是一个插入。这意味着，**如果比较字符串，它们将是相同的，除了字符串中某个位置的一次移位。**
- 移除 `Removal` ：`apple` 和 `aple` 也是一次移除，因为移除只是插入的反向操作——`apple` 移除得到 `aple` ，`aple` 插入得到 `apple` 。

现在可以开始实现这个算法了，把**插入和删除的检查**合并到一个步骤中，并单独检查替换。注意，**字符串的长度将指示需要检查字符串的插入或者删除，抑或替换**：替换要求等长并且只有一个位置的字符不同；插入要求 `s1` 的长度加一等于 `s2` ，删除要求 `s1` 的长度减一等于 `s2` ，如果找到了一处不同后指向长字符串的指针后移一位，剩余的字符全部相同，则两个字符串是插入和删除的关系（长字符串删除得到短字符串，短字符串插入得到长字符串）。Java代码如下：
```cpp
class Solution {
    public boolean oneEditAway(String first, String second) {
        if (first.length() == second.length()) 
            return oneEditReplace(first, second);
        else if (first.length() + 1 == second.length()) 
            return oneEditInsert(first, second);
        else if (first.length() - 1 == second.length())
            return oneEditInsert(second, first);
        return false;
    }
    private boolean oneEditReplace(String s1, String s2) {
        boolean foundDifference = false;
        for (int i = 0; i < s1.length(); ++i) {
            if (s1.charAt(i) != s2.charAt(i)) {
                if (foundDifference) return false; 
                foundDifference = true;
            }
        }
        return true;
    }
    //Check if you can insert a character into s1 to make s2.
    private boolean oneEditInsert(String s1, String s2) {
        int index1 = 0, index2 = 0;
        while (index1 < s1.length() && index2 < s2.length()) {
            if (s1.charAt(index1) != s2.charAt(index2)) {
                if (index1 != index2) return false; //第二次s1[index1] != s2[index2]时,index1 != index2
                ++index2; //第一次s1[index1] != s2[index2]时,index1 == index2
            } else {
                ++index1;
                ++index2;
            }
        }
        return true;
    }
}
```
效率如下：
```java
执行用时：2 ms, 在所有 Java 提交中击败了99.62% 的用户
内存消耗：38.9 MB, 在所有 Java 提交中击败了38.76% 的用户
```
这个算法的时间复杂度为 $O(n)$ ，其中 $n$ 是短字符串的长度。为什么不是长字符串呢？因为如果两个字符串等长或者多一个字符、少一个字符，此时使用哪个字符串的长度定义时间复杂度是无关紧要的；如果字符串的长度差别非常显著，则此算法将以 $O(1)$ 的时间终止，一个很长的字符串不会显著延长运行时间，除非两个字符串都很长。

在这里可能会注意到 `oneEditReplace` 与 `oneEditInsert` 的代码非常相似，两个方法遵循相似的逻辑：**比较每个字符并确保字符串只相差一个**。处理这种差异的方法各不相同。方法 `oneEditReplace` 只标记差异，而 `oneEditlnsert` 将指向较长字符串的指针后移一位。因此可以把它们合并起来，用同样的方法处理这两个问题：
- `s1` 是短字符串，`s2` 是长字符串（或者两者的长度相等），`index1, index2` 分别指向 `s1, s2` ，`foundDifference = false` 作为标志变量。如果第一次发现差异，就用 `foundDifference = true` 标记差异，如果第二次发现差异，就返回 `false` ；
- 如果字符相同，短字符串指针 `index1` 后移一位。第一次发现差异时，如果两个字符串等长，说明是替换，需要后移 `index1` 短字符串指针；否则就是插入或者删除的判断，此时 `index1` 短字符串指针不动。最后无论如何，都将长字符串指针 `index2` 后移一位。
- 效果是：字符相同时，两个指针都后移一位；不同时视区别移动或者不移动短字符串指针，总是移动长字符串指针。

```java
class Solution {
    public boolean oneEditAway(String first, String second) {
        //Length check.
        if (Math.abs(first.length() - second.length()) > 1) 
            return false;
        //Get shorter and longer string.
        String s1 = first.length() < second.length() ? first : second; //shorter
        String s2 = first.length() < second.length() ? second : first; //longer
        int index1 = 0, index2 = 0;
        boolean foundDifference = false;
        while (index1 < s1.length() && index2 < s2.length()) {
            if (s1.charAt(index1) != s2.charAt(index2)) {
                //Ensure that this is the first difference found.
                if (foundDifference) return false;
                foundDifference = true;
                //On insert or remove, don't move shorter pointer
                if (s1.length() == s2.length()) ++index1; //On replace, move shorter pointer
            } else ++index1; //If matching, move shorter pointer
            ++index2; //Always move pointer for longer string
        }
        return true;
    } 
}
```
效果如下，和上一版代码一样的时间复杂度。可能会认为第一种方法更好，因为它更清晰、更具可读性、更容易遵循。也可能认为第二种更好，因为它更紧凑，并且不会重复代码（即便于维护）。不一定要选择一方，可以和面试官讨论权衡。
```java
执行用时：2 ms, 在所有 Java 提交中击败了99.62% 的用户
内存消耗：38.6 MB, 在所有 Java 提交中击败了83.46% 的用户
```
