Under the grammar given below, strings can represent a set of lowercase words. Let `R(expr)` denote the set of words the expression represents.

The grammar can best be understood through simple examples:
-   Single letters represent a singleton set containing that word.
    -   `R("a") = {"a"}`
    -   `R("w") = {"w"}`
-   When we take a comma-delimited list of two or more expressions, we take the union of possibilities.
    -   `R("{a,b,c}") = {"a","b","c"}`
    -   `R("{{a,b},{b,c}}") = {"a","b","c"}` (notice the final set only contains each word at most once)
-   When we concatenate two expressions, we take the set of possible concatenations between two words where the first word comes from the first expression and the second word comes from the second expression.
    -   `R("{a,b}{c,d}") = {"ac","ad","bc","bd"}`
    -   `R("a{b,c}{d,e}f{g,h}") = {"abdfg", "abdfh", "abefg", "abefh", "acdfg", "acdfh", "acefg", "acefh"}`

Formally, the three rules for our grammar:
-   For every lowercase letter `x`, we have `R(x) = {x}`.
-   For expressions `e1, e2, ... , ek` with `k >= 2`, we have `R({e1, e2, ...}) = R(e1) ∪ R(e2) ∪ ...`
-   For expressions `e1` and `e2`, we have `R(e1 + e2) = {a + b for (a, b) in R(e1) × R(e2)}`, where `+` denotes concatenation, and `×` denotes the cartesian product.

Given an expression representing a set of words under the given grammar, return _the sorted list of words that the expression represents_.

**Example 1:**
```java
Input: expression = "{a,b}{c,{d,e}}"
Output: ["ac","ad","ae","bc","bd","be"]
```
**Example 2:**
```java
Input: expression = "{{a,z},a{b,c},{ab,z}}"
Output: ["a","ab","ac","z"]
Explanation: Each distinct word is written only once in the final answer.
```
**Constraints:**
-   `1 <= expression.length <= 60`
-   `expression[i]` consists of `'{'`, `'}'`, `','`or lowercase English letters.
-   The given `expression` represents a set of words based on the grammar given in the description.

题意：如果熟悉 Shell 编程，那么一定了解过==花括号展开，它可以用来生成任意字符串==。花括号展开的表达式可以看作一个由 **花括号**、**逗号** 和 **小写英文字母** 组成的字符串，定义下面几条语法规则：
-   如果只给出单一的元素 `x`，那么表达式表示的字符串就只有 `"x"`。`R(x) = {x}`
    -   例如，表达式 `"a"` 表示字符串 `"a"`。
    -   而表达式 `"w"` 就表示字符串 `"w"`。
-   当两个或多个表达式并列，以逗号分隔，我们取这些表达式中元素的并集。`R({e_1,e_2,...}) = R(e_1) ∪ R(e_2) ∪ ...`
    -   例如，表达式 `"{a,b,c}"` 表示字符串 `"a","b","c"`。
    -   而表达式 `"{{a,b},{b,c}}"` 也可以表示字符串 `"a","b","c"`。
-   要是两个或多个表达式相接，中间没有隔开时，我们从这些表达式中各取一个元素依次连接形成字符串。`R(e_1 + e_2) = {a + b for (a, b) in R(e_1) × R(e_2)}`
    -   例如，表达式 `"{a,b}{c,d}"` 表示字符串 `"ac","ad","bc","bd"`。
-   表达式之间允许嵌套，单一元素与表达式的连接也是允许的。
    -   例如，表达式 `"a{b,c,d}"` 表示字符串 `"ab","ac","ad"​​​​​​`。
    -   例如，表达式 `"a{b,c}{d,e}f{g,h}"` 可以表示字符串 `"abdfg", "abdfh", "abefg", "abefh", "acdfg", "acdfh", "acefg", "acefh"`。

给出表示基于给定语法规则的表达式 `expression`，返回它所表示的所有字符串组成的**有序列表**。

---
### 解法1 BFS
BFS模板题，定义队列，初识时加入整个表达式 `expression` 。开始遍历，出队当前字符串，定义 `left` 和 `right` ，找到成对的 `{` 和 `}` 索引位置，如果出现多层嵌套，如 `a{b{c,d}}` ，则**优先处理最内层的 `{` 和 `}`** ，这里的最内层 `{}` 是第一个 `}` 出现之前的 `{}` 。==如果最终字符串没有 `{` 位置，则直接将当前字符串加入**结果集**==。

之后根据 `{` 和 `}` 将字符串分割为 3 部分：
- `left` 之前的字符串；
- `[left,right]` 之间的字符串数组，分别将其中的字符串作为新的中间部分，与左右两边拼接，将其入队，进行下一轮处理；
- `right` 之后的字符串；

```java
class Solution {
    public List<String> braceExpansionII(String expression) {
        Deque<String> q = new ArrayDeque<>();       
        Set<String> ret = new HashSet<>(); // 去重
        q.offer(expression);
        while (!q.isEmpty()) {
            String expr = q.poll();
            int left = -1, right = 0;
            while (right < expr.length() && expr.charAt(right) != '}') {
                if (expr.charAt(right) == '{') {
                    left = right; // left指向第一个}之前的那个{
                }
                ++right;
            }
            if (left == -1) { // 说明没有{,即没有{},只有单一元素
                ret.add(expr);
                continue;
            }
            String l = expr.substring(0, left);
            String[] words = expr.substring(left + 1, right).split(","); // 去除选择的{}字符,并以','分割
            String r = expr.substring(right + 1);
            for (String word : words) {
                q.offer(new StringBuilder(l).append(word).append(r).toString());
            }
        } 
        List<String> list = new ArrayList<>(ret);
        Collections.sort(list);
        return list;
    }
}
```

---
### 解法2 递归解析
好像编译原理啊……**表达式可以拆分为多个子表达式，以逗号分隔或者直接相接**。我们应当先按照逗号分割成多个子表达式进行求解，然后再对所有结果求并集。这样做的原因是求积的优先级高于求并集的优先级。

用 `expr` 表示一个任意一种表达式，用 `term` 表示一个最外层没有逗号分割的表达式，那么 `expr` 可以按照如下规则分解：
```java
expr -> term | term,expr
expr -> term | term+expr
```
其中的 `|` 表示或者，即 `expr` 可以分解为前者，也可以分解为后者。再来看 `term` ，`term` 可以由**小写英文字母**或者**花括号包括的表达式**直接相接组成，我们用 `item` 来表示每一个相接单元，那么 `term` 可以按照如下规则分解：
```java
term -> item | item term
term -> item | item*term
```
`item` 可以进一步分解为**小写英文字母 `letter`** 或者**花括号包括的表达式**，它的分解如下：
```java
item -> letter | {expr}
```
在代码中，我们编写三个函数，分别负责以上三种规则的分解：
- `expr` 函数，**不断的调用 `term` ，并与其结果进行合并**。如果==匹配到表达式末尾或者当前字符不是逗号时，则返回==。
- `term` 函数，**不断的调用 `item` ，并与其结果求积**。如果==匹配到表达式末尾或者当前字符不是小写字母，并且也不是左括号时，则返回==。
- `item` 函数，根据当前字符是不是左括号来求解。如果是左括号，则调用 `expr` ，返回结果；否则**构造一个只包含当前字符的字符串集合，返回结果**。

以下示意图以 `{a,b}{c,{d,e}}` 为例（`(a+b)*(c+(d+e))`，展示了表达式递归拆解、以及回溯的全过程—— `{ac,ad,ae,bc,bd,be}` 。
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303081634492.png)
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303081634220.png)
在代码实现过程中有以下细节：维护一个指针来遍历整个表达式，或者将表达式和当前下标以引用的方式传递给被调函数。因最终答案需要去重，可先用集合来求解中间结果，最后转换成已排序的列表作为答案。
```cpp
class Solution {
    string expression;
    int idx;

    // item -> letter | { expr }
    set<string> item() {
        set<string> ret;
        if (expression[idx] == '{') {
            idx++;
            ret = expr();
        } else {
            ret = {string(1, expression[idx])};
        }
        idx++;
        return move(ret);
    }

    // term -> item | item term
    set<string> term() {
        // 初始化空集合，与之后的求解结果求笛卡尔积
        set<string> ret = {""};
        // item 的开头是 { 或小写字母，只有符合时才继续匹配
        while (idx < expression.size() && (expression[idx] == '{' || isalpha(expression[idx]))) {
            auto sub = item();
            set<string> tmp;
            for (auto &left : ret) {
                for (auto &right : sub) {
                    tmp.insert(left + right);
                }
            }
            ret = move(tmp);
        }
        return move(ret);
    }

    // expr -> term | term, expr
    set<string> expr() {
        set<string> ret;
        while (true) {
            // 与 term() 求解结果求并集
            ret.merge(term());
            // 如果匹配到逗号则继续，否则结束匹配
            if (idx < expression.size() && expression[idx] == ',') {
                idx++;
                continue;
            } else {
                break;
            }
        }
        return move(ret);
    }

public:
    vector<string> braceExpansionII(string expression) {
        this->expression = expression;
        this->idx = 0;
        auto ret = expr();
        return {ret.begin(), ret.end()};
    }
};
```
复杂度分析
- 时间复杂度：$O(n\log n)$ ，其中 $n$ 是 $expression$ 的长度。整个 $expression$ 只会遍历一次，时间复杂度为 $O(n)$ ，集合合并以及求积运算的时间复杂度为 $O(n\log n)$ 。
- 空间复杂度：$O(n)$ 。递归过程所需的栈空间为 $O(n)$ ，以及存放中间答案的空间复杂度为 $O(n)$ ，因此总的空间复杂度为 $O(n)$ 。

---
### 解法3 栈（中缀转后缀）
如果把题目中的表达式并列关系（`,`）看做是求和，把相接看做是求积，那么求解整个表达式的过程可以类比于求解中缀表达式的过程，例如：`{a,b}{c,{d,e}}` 可以看做是 `{a+b}×{c+{d+e}}` 。

与求解中缀表达式一样，在遍历表达式的过程中我们需要==用到两个栈，一个用来存放运算符（即加号和乘号，以及左大括号），另一个用来存运算对象==（即集合）。本题中有一个特殊情况需要处理，就是**乘号需要我们自己来添加**，我们按照当前字符的种类来判断前面是否需要添加乘号：
- 如果**当前字符是 `{` ，并且前面是 `}` 或者小写英文字母**时，需要添加乘号运算。
- 如果**当前字符是小写字母，并且前面是 `}` 或者是小写英文字母**时，需要添加乘号运算。
- 如果当前字符是 `,` ，则前面一定不需要添加乘号运算。
- 如果当前字符是 `}` ，则前面一定不需要添加乘号运算。
- 因此，只有当前字符是 `{` 或者小写字母时，才需要考虑是否在前面添加乘号。

接下来我们分析运算优先级的问题，在本题中只涉及加法和乘法两种运算。==如果一个表达式同时有并列和相接，那我们应该先计算相接的结果，再计算并列的结果==。因此，乘法的优先级要大于加法。

至此，可按照如下流程来计算表达式的值：
1. 遇到 `,` ，则先判断运算符栈顶是否是乘号，如果是乘号则需要先计算乘法，直到栈顶不是乘号为止，再将加号放入运算符栈中。
2. 遇到 `{` ，则先判断是否需要添加乘号，再将 `{` 放入运算符栈。
3. 遇到 `}` ，则不断地弹出运算符栈顶，并进行相应的计算，直到栈顶为左括号为止。
4. 遇到小写字母，则先判断是否需要添加乘号，再构造一个只包含当前小写字母的字符串集合，放入集合栈中。
5. 按照上述流程遍历完一次之后，由于题目给定的表达式中最外层可能没有大括号，例如 `{a,b}{c,{d,e}}` ，因此运算符栈中可能依然有元素，我们需要依次将他们弹出并进行计算。最终，集合栈栈顶元素即为答案。

下面展示了以 `{a,b}{c,{d,e}}` 为例求解的全过程：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303081641899.gif)
class Solution {
public:
    vector<string> braceExpansionII(string expression) {
        vector<char> op;
        vector<set<string>> stk;

        // 弹出栈顶运算符，并进行计算
        auto ope = [&]() {
            int l = stk.size() - 2, r = stk.size() - 1;
            if (op.back() == '+') {
                stk[l].merge(stk[r]);
            } else {
                set<string> tmp;
                for (auto &left : stk[l]) {
                    for (auto &right : stk[r]) {
                        tmp.insert(left + right);
                    }
                }
                stk[l] = move(tmp);
            }
            op.pop_back();
            stk.pop_back();
        };

        for (int i = 0; i < expression.size(); i++) {
            if (expression[i] == ',') {
                // 不断地弹出栈顶运算符，直到栈为空或者栈顶不为乘号
                while (op.size() && op.back() == '*') {
                    ope();
                }
                op.push_back('+');
            } else if (expression[i] == '{') {
                // 首先判断是否需要添加乘号，再将 { 添加到运算符栈中
                if (i > 0 && (expression[i - 1] == '}' || isalpha(expression[i - 1]))) {
                    op.push_back('*');
                }
                op.push_back('{');
            } else if (expression[i] == '}') {
                // 不断地弹出栈顶运算符，直到栈顶为 {
                while (op.size() && op.back() != '{') {
                    ope();
                }
                op.pop_back();
            } else {
                // 首先判断是否需要添加乘号，再将新构造的集合添加到集合栈中
                if (i > 0 && (expression[i - 1] == '}' || isalpha(expression[i - 1]))) {
                    op.push_back('*');
                }
                stk.push_back({string(1, expression[i])});
            }
        }
        
        while (op.size()) {
            ope();
        }
        return {stk.back().begin(), stk.back().end()};
    }
};
复杂度分析

时间复杂度：O(nlog⁡n)O(n\log n)O(nlogn)，其中 nnn 是 expression\textit{expression}expression 的长度。整个 expression\textit{expression}expression 只会遍历一次，时间复杂度为 O(n)O(n)O(n)，集合合并以及求积运算的时间复杂度为 O(nlog⁡n)O(n\log n)O(nlogn)，因此总的时间复杂度为 O(nlog⁡n)O(n \log n)O(nlogn)。

空间复杂度：O(n)O(n)O(n)。过程中用到了两个栈，他们都满足在任意时刻元素个数不超过 O(n)O(n)O(n)，包含 nnn 个元素的集合的时间复杂度为 O(n)O(n)O(n)，因此总的空间复杂度为 O(n)O(n)O(n)。

方法一：前缀树+桶排序+dfs+位运算+链表+哈希+延迟处理
思路
递归处理思路详解请查看官解。

本题解中以前缀树保存递归处理结果，将字符串的构造延迟到调用get方法时处理。

以前缀树保存结果，提供了一定的去重功能，且最后提取答案时可以用桶排序替代字符串排序。

每个前缀树节点中用一个mark标识该节点存在哪些子节点：(mark & 1 << c) == 1代表该节点存在对应字符'a' + c的子节点。遍历前缀树时，可以根据节点的mark值快速定位其子节点。（但子节点较密集时，性能不如直接遍历子节点容器）

结果
时间 3 ms
Code
class Solution extends java.util.AbstractList<String> {

    /** HashMap负载因子 */
    private static final float LOAD_FACTOR = 16f / 21;
    /** 如果 int x 二进制有且仅有一位为1，则 MAP[(x - 1) % MAP.length] 为 x 的1位后尾随的0的位数 */
    private static final int[] MAP =
        {0, 1, 5, 2, 22, 6, 12, 3, 10, 23, 25, 7, 18, 13, 0, 4, 21, 11, 9, 24, 17, 0, 20, 8, 16, 19, 15, 14, 0};
    /** 字符缓存数组 */
    private static final char[] BUFFER = new char[60];

    /** 解析前：expression.length()；解析后：答案数量 */
    private int size;
    /** 前缀树根节点 */
    private final Node root = new Node();
    /** 前缀树遍历路径终点 */
    private Path path;
    /** 遍历指针 */
    private int pos;

    public List<String> braceExpansionII(String expression) {
        size = expression.length();
        expression.getChars(0, size, BUFFER, 0);
        path = new Path(root);
        Node[] nodes = resolve(new Node[] {root});
        pos = 0;
        size = nodes.length;
        // 标记答案
        for (Node node : nodes) {
            node.isAns = true;
        }
        return this;
    }

    @Override
    public String get(int index) {
        do {
            while (path.node.mark == 0) {
                // 本节点遍历完成，回溯
                --pos;
                path = path.prev;
            }
            index = path.node.mark & -path.node.mark;
            path.node.mark ^= index;
            index = MAP[(index - 1) % MAP.length];
            BUFFER[pos++] = (char)('a' + index);
            path = path.setNext(index);
        } while (!path.node.isAns);
        return new String(BUFFER, 0, pos);
    }

    @Override
    public int size() {
        return size;
    }

    /**
     * dfs解析生成前缀树
     * 
     * @return 答案节点
     */
    private Node[] resolve(Node[] parents) {
        Set<Node> retSet = null;
        Node[] nodes = parents;
        while (pos < size) {
            char c = BUFFER[pos++];
            if (c > 'z') {
                if (c == '}') {
                    break;
                }

                nodes = resolve(nodes);
            } else if (c == ',') {
                if (retSet == null) {
                    // ','后面还有至少一个并列表达式，这里分配两倍空间
                    retSet = new HashSet<>(nodes.length * 21 + 7 >>> 3, LOAD_FACTOR);
                }
                for (Node node : nodes) {
                    retSet.add(node);
                }
                // 重置状态
                nodes = parents;
            } else {
                // 英文字母，将每个前缀向下扩展一层
                Node[] arr;
                if (nodes == parents) {
                    // 不能直接修改parents
                    arr = parents;
                    nodes = new Node[parents.length];
                } else {
                    arr = nodes;
                }
                for (int i = 0; i < arr.length; i++) {
                    nodes[i] = arr[i].mark(c - 'a');
                }
            }
        }
        if (retSet == null) {
            return nodes;
        }

        for (Node node : nodes) {
            retSet.remove(node);
        }
        if (retSet.isEmpty()) {
            return nodes;
        }

        parents = retSet.toArray(new Node[retSet.size() + nodes.length]);
        System.arraycopy(nodes, 0, parents, retSet.size(), nodes.length);
        return parents;
    }

    /** 前缀树节点 */
    private static class Node {

        /** 是否为答案 */
        private boolean isAns;
        /** 标记 */
        private int mark;
        /** 子节点 */
        private Node[] children;

        /**
         * 标记并创建子节点
         * 
         * @return 子节点
         */
        private Node mark(int i) {
            if ((mark & 1 << i) != 0) {
                return children[i];
            }

            mark |= 1 << i;
            if (children == null) {
                children = new Node[26];
            }
            return children[i] = new Node();
        }
    }

    /** 路径点（链表节点） */
    private static class Path {

        /** 当前节点 */
        private Node node;
        /** 上个路径点 */
        private Path prev;
        /** 下个路径点 */
        private Path next;

        public Path() {}

        public Path(Node node) {
            this.node = node;
        }

        public Path setNext(int i) {
            if (next == null) {
                next = new Path();
                next.prev = this;
            }
            next.node = node.children[i];
            return next;
        }
    }
}
方法二：进一步延迟处理
思路
可以直接把前缀树的构造也延迟到读取答案时进行。

结果
时间 0 ms
Code
class Solution extends java.util.AbstractList<String> {

    /** HashMap负载因子 */
    private static final float LOAD_FACTOR = 16f / 21;
    /** 如果 int x 二进制有且仅有一位为1，则 MAP[(x - 1) % MAP.length] 为 x 的1位后尾随的0的位数 */
    private static final int[] MAP =
        {0, 1, 5, 2, 22, 6, 12, 3, 10, 23, 25, 7, 18, 13, 0, 4, 21, 11, 9, 24, 17, 0, 20, 8, 16, 19, 15, 14, 0};
    /** 字符缓存数组 */
    private static final char[] BUFFER = new char[60];

    /** 解析前：expression.length()；解析后：答案数量 */
    private int size;
    /** 前缀树根节点 */
    private final Node root = new Node();
    /** 前缀树遍历路径终点 */
    private Path path;
    /** 遍历指针 */
    private int pos;

    public List<String> braceExpansionII(String expression) {
        size = expression.length();
        expression.getChars(0, size, BUFFER, 0);
        return this;
    }

    @Override
    public String get(int index) {
        do {
            while (path.node.mark == 0) {
                // 本节点遍历完成，回溯
                --pos;
                path = path.prev;
            }
            index = path.node.mark & -path.node.mark;
            path.node.mark ^= index;
            index = MAP[(index - 1) % MAP.length];
            BUFFER[pos++] = (char)('a' + index);
            path = path.setNext(index);
        } while (!path.node.isAns);
        return new String(BUFFER, 0, pos);
    }

    @Override
    public int size() {
        if (path == null) {
            path = new Path(root);
            Node[] nodes = resolve(new Node[] {root});
            pos = 0;
            size = nodes.length;
            // 标记答案
            for (Node node : nodes) {
                node.isAns = true;
            }
        }
        return size;
    }

    /**
     * dfs解析生成前缀树
     * 
     * @return 答案节点
     */
    private Node[] resolve(Node[] parents) {
        Set<Node> retSet = null;
        Node[] nodes = parents;
        while (pos < size) {
            char c = BUFFER[pos++];
            if (c > 'z') {
                if (c == '}') {
                    break;
                }

                nodes = resolve(nodes);
            } else if (c == ',') {
                if (retSet == null) {
                    // ','后面还有至少一个并列表达式，这里分配两倍空间
                    retSet = new HashSet<>(nodes.length * 21 + 7 >>> 3, LOAD_FACTOR);
                }
                for (Node node : nodes) {
                    retSet.add(node);
                }
                // 重置状态
                nodes = parents;
            } else {
                // 英文字母，将每个前缀向下扩展一层
                Node[] arr;
                if (nodes == parents) {
                    // 不能直接修改parents
                    arr = parents;
                    nodes = new Node[parents.length];
                } else {
                    arr = nodes;
                }
                for (int i = 0; i < arr.length; i++) {
                    nodes[i] = arr[i].mark(c - 'a');
                }
            }
        }
        if (retSet == null) {
            return nodes;
        }

        for (Node node : nodes) {
            retSet.remove(node);
        }
        if (retSet.isEmpty()) {
            return nodes;
        }

        parents = retSet.toArray(new Node[retSet.size() + nodes.length]);
        System.arraycopy(nodes, 0, parents, retSet.size(), nodes.length);
        return parents;
    }

    /** 前缀树节点 */
    private static class Node {

        /** 是否为答案 */
        private boolean isAns;
        /** 标记 */
        private int mark;
        /** 子节点 */
        private Node[] children;

        /**
         * 标记并创建子节点
         * 
         * @return 子节点
         */
        private Node mark(int i) {
            if ((mark & 1 << i) != 0) {
                return children[i];
            }

            mark |= 1 << i;
            if (children == null) {
                children = new Node[26];
            }
            return children[i] = new Node();
        }
    }

    /** 路径点（链表节点） */
    private static class Path {

        /** 当前节点 */
        private Node node;
        /** 上个路径点 */
        private Path prev;
        /** 下个路径点 */
        private Path next;

        public Path() {}

        public Path(Node node) {
            this.node = node;
        }

        public Path setNext(int i) {
            if (next == null) {
                next = new Path();
                next.prev = this;
            }
            next.node = node.children[i];
            return next;
        }
    }
}