

In the classic problem of the Towers of Hanoi, you have 3 towers and N disks of different sizes which can slide onto any tower. The puzzle starts with disks sorted in ascending order of size from top to bottom (i.e., each disk sits on top of an even larger one). You have the following constraints:

(1) Only one disk can be moved at a time.
(2) A disk is slid off the top of one tower onto another tower.
(3) A disk cannot be placed on top of a smaller disk.

Write a program to move the disks from the first tower to the last using stacks.

**Example1:**

```swift
Input: A = [2, 1, 0], B = [], C = []
Output: C = [2, 1, 0]
```

**Example2:**

```swift
Input: A = [1, 0], B = [], C = []
Output: C = [1, 0]
```

**Note:** `A.length <= 14`

题意：在经典汉诺塔问题中，有 `3` 根柱子及 `N` 个不同大小的穿孔圆盘，盘子可以滑入任意一根柱子。一开始，所有盘子自上而下按升序依次套在第一根柱子上(即每一个盘子只能放在更大的盘子上面)。移动圆盘时受到以下限制:
- 每次只能移动一个盘子;
- 盘子只能从柱子顶端滑出移到下一根柱子;
- 盘子只能叠在比它大的盘子上。

编写程序，将所有盘子从第一根柱子移到最后一根柱子。

---
### 解法1 取巧
```cpp
class Solution { 
public:
    void hanota(vector<int>& A, vector<int>& B, vector<int>& C) {
        C = A; 
    }
};
```
效率如下：
```cpp
执行用时：0 ms, 在所有 C++ 提交中击败了100.00% 的用户
内存消耗：6.8 MB, 在所有 C++ 提交中击败了40.13% 的用户
```
---
### 解法2 递归
这道题的递归解法不知道做过多少次了：
```cpp
class Solution {
private:
    void move(int n, vector<int>& A, vector<int>& B, vector<int>& C) {
        if (n < 2) { //递归结束情形,如果A柱上只剩一个盘子,那么直接移动到C柱即可
            C.push_back(A.back());
            A.pop_back();
            return;
        }
        move(n - 1, A, C, B);  //将A中N-1个盘子经过C移动到中间柱子B
        C.push_back(A.back()); //此时A柱子剩下那个盘子是N个盘子中最大的,将它移动到C柱上
        A.pop_back();
        move(n - 1, B, A, C);  //中间柱子B中剩下的N-1个盘子经过A移动到C
    }
public:
    void hanota(vector<int>& A, vector<int>& B, vector<int>& C) { 
        int n = A.size(); //所有的盘子数量
        move(n, A, B, C); 
    }
};
```
运行效率如下：
```cpp
执行用时：4 ms, 在所有 C++ 提交中击败了47.39% 的用户
内存消耗：7.5 MB, 在所有 C++ 提交中击败了7.65% 的用户
```
---
### 解法3 迭代
将上述递归中序遍历的代码转换为迭代形式，用栈模仿函数递归调用的过程：
```cpp
class Solution { 
    using A3 = array<vector<int>*, 3>;  //存储3个指向vector<int>*的指针的数组
public:
    void hanota(vector<int>& A, vector<int>& B, vector<int>& C) {
        int n = A.size();
        if (n == 0) return;
        stack<pair<int, A3>> st; //存储函数递归调用需要的参数
        st.push(make_pair(n, A3{&A, &B, &C}));
        while (!st.empty()) {
            auto t = st.top(); st.pop();
            const auto &sp = t.second;
            if (t.first == 1) { //访问子树根节点
                sp[2]->push_back(sp[0]->back());
                sp[0]->pop_back();
            } else { //要和函数递归调用的顺序相反
                st.push(make_pair(t.first - 1, A3{sp[1], sp[0], sp[2]}));
                st.push(make_pair(1, A3{sp[0], sp[1], sp[2]}));
                st.push(make_pair(t.first - 1, A3{sp[0], sp[2], sp[1]}));
            }
        }
    }
};
```
或者使用教科书的经典代码模板：
```cpp
class Solution { 
private:
    using A3 = array<vector<int>*, 3>;  //存储3个指向vector<int>*的指针的数组
public:
    void hanota(vector<int>& A, vector<int>& B, vector<int>& C) {
        int n = A.size();
        if (n == 0) return;
        stack<pair<int, A3>> st;
        pair<int, A3> root = make_pair(n, A3{&A, &B, &C}); 
        while (root.first > 0 || !st.empty()) {
            while (root.first > 0) {
                st.push(root);
                const auto &sp = root.second;
                root = make_pair(root.first - 1, A3{sp[0], sp[2], sp[1]}); //往左孩子状态走
            }
            root = st.top(); st.pop();
            const auto &sp = root.second;
            sp[2]->push_back(sp[0]->back());
            sp[0]->pop_back();
            root = make_pair(root.first - 1, A3{sp[1], sp[0], sp[2]}); //往右孩子状态走
        }
    }
}; 
```
提交后运行结果如下：
```cpp
执行用时：12 ms, 在所有 C++ 提交中击败了47.39% 的用户
内存消耗：6.8 MB, 在所有 C++ 提交中击败了37.07% 的用户
```
