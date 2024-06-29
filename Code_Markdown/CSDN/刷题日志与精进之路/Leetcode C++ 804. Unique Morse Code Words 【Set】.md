**International Morse Code** defines a standard encoding where **each letter** is mapped to **a series of dots and dashes**, as follows: "a" maps to ".-", "b" maps to "-...", "c" maps to "-.-.", and so on.

For convenience, the full table for the 26 letters of the English alphabet is given below:
```
[".-","-...","-.-.","-..",".","..-.","--.","....","..",".---","-.-",".-..","--","-.","---",".--.","--.-",".-.","...","-","..-","...-",".--","-..-","-.--","--.."]
```
Now, given a list of words, each word can be written as **a concatenation of the Morse code of each letter.** For example, "cba" can be written as "-.-..--...", (which is the concatenation "-.-." + "-..." + ".-"). We'll call such a concatenation, the transformation of a word.

Return **the number of different transformations** among all words we have.

Example:
```
Input: words = ["gin", "zen", "gig", "msg"]
Output: 2
```
Explanation: 
```
The transformation of each word is:
"gin" -> "--...-."
"zen" -> "--...-."
"gig" -> "--...--."
"msg" -> "--...--."

There are 2 different transformations, "--...-." and "--...--.".
```
Note:

-    The length of words will be at most $100$.
-    Each words[i] will have length in range $[1, 12]$.
-    words[i] will **only consist of lowercase letters**.

题意：摩斯密码，将每个字符对应一个点和横杆的组合。将一个字符串数组中的所有字符串转换为相应的摩斯密码，但是不同的词汇组成的摩斯密码可能是相同的。题目要求出一共有多少个不同的摩斯密码。

思路：很简单，先得到每个字符串的摩斯密码，然后添加进集合中，最后集合的大小，就是不同的摩斯密码的数量。我们需要一个集合类，C++就是`<set>`。


## C++代码
执行用时 :12 ms, 在所有 C++ 提交中击败了10.67% 的用户
内存消耗 :8.4 MB, 在所有 C++ 提交中击败了100.00%的用户
```cpp
class Solution {
public:
    int uniqueMorseRepresentations(vector<string>& words) {
        string morse[] = {".-","-...","-.-.","-..",".","..-.","--.","....",
        "..",".---","-.-",".-..","--","-.","---",".--.","--.-",".-.","...",
        "-","..-","...-",".--","-..-","-.--","--.."};
        set<string> st;
        for (int i = 0; i < words.size(); ++i) {
            string t;
            for (auto c : words[i]) 
                t += morse[c - 'a'];
            st.insert(t);
        }
        return st.size();
    }
};
```
改为无序集合，时间会提高许多：
执行用时 :4 ms, 在所有 C++ 提交中击败了91.85% 的用户
内存消耗 :8.6 MB, 在所有 C++ 提交中击败了100.00%的用户
```cpp
class Solution {
public:
    int uniqueMorseRepresentations(vector<string>& words) {
        string morse[] = {".-","-...","-.-.","-..",".","..-.","--.","....",
        "..",".---","-.-",".-..","--","-.","---",".--.","--.-",".-.","...",
        "-","..-","...-",".--","-..-","-.--","--.."};
        unordered_set<string> st;
        for (int i = 0; i < words.size(); ++i) {
            string t;
            for (auto c : words[i]) 
                t += morse[c - 'a'];
            st.insert(t);
        }
        return st.size();
    }
};
```

## Java代码
Java里面的比如说：`TreeSet`，当然，我们前面实现的那个简单的(有序)集合类也可以用，不过Java的`TreeSet`的功能比我们实现的要强大太多了，其一：Java的`TreeSet`底层实现是用的平衡二叉搜索树——红黑树，所以不会出现最差的情况；其二，`TreeSet`除了我们定义的`add, remove, contains, getSize, isEmpty`外还有更多的操作，利用其作为二叉搜索树的性质，`TreeSet`提供了许多有用的方法。

执行用时 :5 ms, 在所有 Java 提交中击败了14.78% 的用户
内存消耗 :37.6 MB, 在所有 Java 提交中击败了5.35%的用户
```java
class Solution {
    public int uniqueMorseRepresentations(String[] words) {
        String[] morse = {".-","-...","-.-.","-..",".","..-.","--.","....",
        "..",".---","-.-",".-..","--","-.","---",".--.","--.-",".-.","...",
        "-","..-","...-",".--","-..-","-.--","--.."}; 
        TreeSet<String> ts = new TreeSet<>();
        for (String s : words) {
            StringBuilder sb = new StringBuilder();
            for (int i = 0; i < s.length(); ++i) 
                sb.append(morse[s.charAt(i) - 'a']);
            ts.add(sb.toString());
        }
        return ts.size();
    }
}
```
不过我们知道，`TreeSet`要维持有序性，每次插入都需要 $O(logn)$ 的时间，但是Java还有无序集合，用哈希实现，插入的速度为 $O(1)$。

执行用时 :2 ms, 在所有 Java 提交中击败了99.78% 的用户
内存消耗 :37.2 MB, 在所有 Java 提交中击败了5.35%的用户
```java
class Solution {
    public int uniqueMorseRepresentations(String[] words) {
        String[] morse = {".-","-...","-.-.","-..",".","..-.","--.","....",
        "..",".---","-.-",".-..","--","-.","---",".--.","--.-",".-.","...",
        "-","..-","...-",".--","-..-","-.--","--.."}; 
        HashSet<String> hs = new HashSet<>();
        for (String s : words) {
            StringBuilder sb = new StringBuilder();
            for (int i = 0; i < s.length(); ++i) 
                sb.append(morse[s.charAt(i) - 'a']);
            hs.add(sb.toString());
        }
        return hs.size();
    }
}
```
