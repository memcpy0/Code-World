Given a string, write a function to check if it is **a permutation of a palin­drome**. A palindrome is a word or phrase that is the same forwards and backwards. A permutation is a rearrangement of letters. The palindrome does not need to be limited to just dictionary words.

 
**Example1:**
```cpp
Input: "tactcoa"
Output: true（permutations: "tacocat"、"atcocta", etc.）
```

题意：判断一个字符串是否是某个回文串的排列。

---

> 这个问题有助于弄清楚**字符串是一个回文串的排列**究竟意味着什么。一个回文串是前后相同的字符串，因此要确定一个字符串是否是一个回文串的排列，我们需要知道它是否可以被写成前后相同。
> 
> 怎样才能以同样的方式前后书写一组字符？我们需要几乎所有的字符都是偶数个，这样一个字符的一半数量可以在一边，另一半数量在另一边。最多只能有一个字符（中间字符）的数量是奇数。例如，我们知道 `tactcoapapa` 是一个回文的排列，因为它有两个 `t` 、四个 `a` 、两个 `c` 、两个 `p` 和一个 `o` 。其中，`o` 是这些字符可能形成的所有回文串的中心。
> 
> 更准确的说，如果是回文串的某个排列，一个偶数长度的字符串中，所有字符的出现数量都必须是偶数的（不能有一个出现次数为奇数的字符）；一个奇数长度的字符串中，必须有且只有一个字符出现奇数次（不能都出现偶数次）。总的来说，**作为一个回文的某个排列，一个字符串最多只能有一个字符出现奇数次。**

---
### 解法1 哈希表计数
对于一个字符串 `s` ，如果它是某个回文串的一个排列，这意味着什么呢？回文串意味着**除了回文中心的一个字符出现次数可以是奇数次或者偶数次外，其他所有字符出现的次数都必须是偶数次**。也就意味着，字符串 `s` 中出现次数为奇数次的字符，要么只有一个，要么没有。

实现这一算法的代码如下，用一个哈希表记录每个字符出现多少次，然后迭代哈希表，判断是否不超过一个字符出现奇数次:
```cpp
class Solution {
public:
    bool canPermutePalindrome(string s) {
        unordered_map<char, int> record;
        for (const char &ch : s) ++record[ch];
        int odd = 0; //计算出现奇数次的字符的个数
        for (const auto &it : record)
            if (it.second & 1) ++odd;
        return odd <= 1 ? true : false;
    }
};
```
效率如下：
```cpp
执行用时：4 ms, 在所有 C++ 提交中击败了38.43% 的用户
内存消耗：6.3 MB, 在所有 C++ 提交中击败了22.75% 的用户
```
---
### 解法2 哈希集合抵销
我们 用Java代码实现另一种思路。就像电灯经过偶数次的反复开和关，最后会恢复到初始状态一样。我们利用 `set` 来存储每种字符（不允许重复，所以每种字符最多存一个），初始时 `set` 中不存在任何字符，对应的状态都是**关**。如果 `set` 中不包含该种字符，就能够往 `set` 中添加该种字符，添加进去，对应的状态变为**开**；如果已经包含，则删除该种字符，对应的状态变为**关**。由于作为回文串的某个排列的字符串中，奇数次数的字符最多只有一个，所以最后 `set` 中的字符个数不会超过 `1` ，其余出现偶数次数的字符最终都不会存在于 `set` 之中。
```java
class Solution {
    public boolean canPermutePalindrome(String phrase) {
        if (phrase == null) return false;
        char[] chars = phrase.toCharArray();
        Set<Character> set = new HashSet<>();
        for (char c : chars) {
            if (set.contains(c)) set.remove(c);
            else set.add(c);
        }
        return set.size() <= 1;
    } 
}
```
最后提交的效率如下：
```java
执行用时：1 ms, 在所有 Java 提交中击败了69.26% 的用户
内存消耗：36.4 MB, 在所有 Java 提交中击败了87.74% 的用户
```

---
### 解法3 整数数组计数
书上给出的Java代码如下，虽然漂亮但却是错误的，因为回文不会限定在字典的单词之中，可能出现如 `/, \, +` 这些字符，而书中代码只开了 `26` 大小的计数数组，并将这些特殊字符都映射到 `-1` 上……
```cpp
class Solution {
    public boolean canPermutePalindrome(String phrase) {
        int[] table = buildCharFrequencyTable(phrase);
        return checkMaxOneOdd(table);
    }
    //Count hou many times each character appears.
    int[] buildCharFrequencyTable(String phrase) {
        int[] table = new int[Character.getNumericValue('z') 
                            - Character.getNumericValue('a') + 1];
        for (char c : phrase.toCharArray()) {
            int x = getCharNumber(c);
            if (x != -1) ++table[x];
        }
        return table;
    }
    //Map each character to a number. a->0, b->1,etc.
    //This is case insensitive. Non-letter characters map to -1.
    private int getCharNumber(Character c) {
        int a = Character.getNumericValue('a');
        int z = Character.getNumericValue('z');
        int val = Character.getNumericValue(c);
        if (a <= val && val <= z) return val - a;
        return -1;
    }
    //Check that no more than one character has add odd count.
    private boolean checkMaxOneOdd(int[] table) {
        boolean foundOdd = false;
        for (int count : table) {
            if (count % 2 == 1) {
                if (foundOdd) return false;
                foundOdd = true;
            }
        }
        return true;
    }
}
```
不过我们注意到，LeetCode上只用到了 `0~127` 的ASCII码字符，于是修改一下上述代码，得到：
```java
class Solution {
    public boolean canPermutePalindrome(String phrase) {
        if (phrase == null) return false;
        int[] table = new int[128];
        for (char c : phrase.toCharArray()) ++table[c];
        boolean foundOdd = false;
        for (int count : table) {
            if (count % 2 == 1) {
                if (foundOdd) return false;
                foundOdd = true;
            }
        }
        return true;
    } 
}
```
得到的效率如下：
```java
执行用时：0 ms, 在所有 Java 提交中击败了100.00% 的用户
内存消耗：36.7 MB, 在所有 Java 提交中击败了35.33% 的用户
```

在时间上无法继续优化，因为任何算法都必须查看整个字符串。但可以做一些较小的增量改进。因为这是一个相对简单的问题，做法是**边走边检查**，而不是在最后检查奇数。一旦到了最后，我们有了答案。可以看到，这是前面的集合抵消法的变体：
```java
class Solution {
    public boolean canPermutePalindrome(String phrase) {
        if (phrase == null) return false;
        int[] table = new int[128];
        int countOdd = 0;
        for (char c : phrase.toCharArray()) {
            ++table[c];
            if (table[c] % 2 == 1) ++countOdd;
            else --countOdd;
        }
        return countOdd <= 1;
    } 
}
```
最重要的是要非常清楚，**上述做法不一定是更好的**，它们的大O时间相同，甚至这里可能会稍微慢一点。虽然消除了对哈希表的最终迭代，但是现在必须为字符串中的每个字符运行额外的几行代码。当然，这里的效率看不出来：
```java
执行用时：0 ms, 在所有 Java 提交中击败了100.00% 的用户
内存消耗：36.6 MB, 在所有 Java 提交中击败了60.10% 的用户
```

---
### 解法4 位图计数(压缩空间) 
还是那个比喻——考虑一下打开/关闭灯光（最初是关闭的），如果灯在关闭状态结束，我们不会知道翻转了多少次，但知道是偶数。同样的，**我们不必知道每个字符的出现次数，而只需要知道它的出现次数是奇数还是偶数**。因此可以使用翻转比特位的形式实现上述代码。下面是C++的代码：
```cpp
class Solution {
public:
    bool canPermutePalindrome(string s) {
        bitset<128> bst; //16个字节
        for (const char &c : s) bst.flip(c);
        return bst.count() <= 1; //判断置位的数量是否小于等于1
    }
};
```
效率很高了：
```cpp
执行用时：0 ms, 在所有 C++ 提交中击败了100.00% 的用户
内存消耗：6 MB, 在所有 C++ 提交中击败了92.82% 的用户
```
对应的，书中给出的Java代码如下，虽然写得漂亮，但同样存在错误——它只使用了一个整型数字（因为觉得大小不敏感的字母有 `26` 个；实际上需要16个字节），看到一个字符，就将其映射到整型的 `0 ~ 26` 位，翻转该位的比特值 `flip/toggle the bit` 。最后判断整型中是否只有一个置位比特（`n & (n - 1) == 0`）或者为 `0` ：
```java
class Solution {
    public boolean canPermutePalindrome(String phrase) {
        if (phrase == null) return false;
        int bitVector = createBitVector(phrase);
        return bitVector == 0 || checkExactlyOneBitSet(bitVector);
    } 
    //Create a bit vector for the string. For each letter with value i, toggle the ith bit.
    private int createBitVector(String phrase) {
        int bitVector = 0;
        for (char c : phrase.toCharArray()) //翻转对应位的比特
            bitVector = toggle(bitVector, c);
        return bitVector;
    }
    //Toggle the ith bit in the integer.
    int toggle(int bitVector, int index) {
        if (index < 0) return bitVector;
        int mask = 1 << index;
        //或者换成:
        //bitVector ^= mask;
        if ((bitVector & mask) == 0) bitVector |= mask; //置为1
        else bitVector &= ~mask; //置为0
        return bitVector;
    }
    //Check that exactly one bit is set by subtracting one from the integer
    //and ANDing it with the original integer
    boolean checkExactlyOneBitSet(int bitVector) {
        return (bitVector & (bitVector - 1)) == 0;
    }
}
```
---
另外在评论区发现了一个错误的算法，也提醒一下自己：
```cpp
class Solution {
public:
    bool canPermutePalindrome(string s) {
        int bitSet[4] = {0}; //0~127位对应位翻转
        for (const char &c : s) bitSet[c / 32] ^= (1 << (c % 32)); 
        for (int i = 0; i < 4; ++i) 
            if (bitSet[i] - (bitSet[i] & -bitSet[i]) != 0) //判断多于一个1
                return false;
        return true; 
    }
};
```

