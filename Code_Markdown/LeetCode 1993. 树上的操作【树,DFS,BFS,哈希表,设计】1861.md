> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的[仓库](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

给你一棵 `n` 个节点的树，编号从 `0` 到 `n - 1` ，以父节点数组 `parent` 的形式给出，其中 `parent[i]` 是第 `i` 个节点的父节点。树的根节点为 `0` 号节点，所以 `parent[0] = -1` ，因为它没有父节点。你想要设计一个数据结构实现树里面对节点的加锁，解锁和升级操作。

数据结构需要支持如下函数：
- **Lock：** 指定用户给指定节点 **上锁** ，上锁后其他用户将无法给同一节点上锁。只有当节点处于未上锁的状态下，才能进行上锁操作。
- **Unlock：** 指定用户给指定节点 **解锁** ，只有当指定节点当前正被指定用户锁住时，才能执行该解锁操作。
- **Upgrade：** 指定用户给指定节点 **上锁** ，并且将该节点的所有子孙节点 **解锁** 。只有如下 3 个条件 **全部** 满足时才能执行升级操作：
    - 指定节点当前状态为未上锁。
    - 指定节点至少有一个上锁状态的子孙节点（可以是 **任意** 用户上锁的）。
    - 指定节点没有任何上锁的祖先节点。

请你实现 `LockingTree` 类：
- `LockingTree(int[] parent)` 用父节点数组初始化数据结构。
- `lock(int num, int user)` 如果 id 为 `user` 的用户可以给节点 `num` 上锁，那么返回 `true` ，否则返回 `false` 。如果可以执行此操作，节点 `num` 会被 id 为 `user` 的用户 **上锁** 。
- `unlock(int num, int user)` 如果 id 为 `user` 的用户可以给节点 `num` 解锁，那么返回 `true` ，否则返回 `false` 。如果可以执行此操作，节点 `num` 变为 **未上锁** 状态。
- `upgrade(int num, int user)` 如果 id 为 `user` 的用户可以给节点 `num` 升级，那么返回 `true` ，否则返回 `false` 。如果可以执行此操作，节点 `num` 会被 **升级** 。

**示例 1：**
![](https://assets.leetcode.com/uploads/2021/07/29/untitled.png)
```js
输入：
["LockingTree", "lock", "unlock", "unlock", "lock", "upgrade", "lock"]
[[[-1, 0, 0, 1, 1, 2, 2]], [2, 2], [2, 3], [2, 2], [4, 5], [0, 1], [0, 1]]
输出：
[null, true, false, true, true, true, false]
解释：
LockingTree lockingTree = new LockingTree([-1, 0, 0, 1, 1, 2, 2]);
lockingTree.lock(2, 2);    // 返回 true ，因为节点 2 未上锁。
                           // 节点 2 被用户 2 上锁。
lockingTree.unlock(2, 3);  // 返回 false ，因为用户 3 无法解锁被用户 2 上锁的节点。
lockingTree.unlock(2, 2);  // 返回 true ，因为节点 2 之前被用户 2 上锁。
                           // 节点 2 现在变为未上锁状态。
lockingTree.lock(4, 5);    // 返回 true ，因为节点 4 未上锁。
                           // 节点 4 被用户 5 上锁。
lockingTree.upgrade(0, 1); // 返回 true ，因为节点 0 未上锁且至少有一个被上锁的子孙节点（节点 4）。
                           // 节点 0 被用户 1 上锁，节点 4 变为未上锁。
lockingTree.lock(0, 1);    // 返回 false ，因为节点 0 已经被上锁了。
```
**提示：**
- `n == parent.length`
- `2 <= n <= 2000`
- 对于 `i != 0` ，满足 `0 <= parent[i] <= n - 1`
- `parent[0] == -1`
- `0 <= num <= n - 1`
- `1 <= user <= 10^4`
- `parent` 表示一棵合法的树。
- `lock` ，`unlock` 和 `upgrade` 的调用 **总共** 不超过 `2000` 次。

---
### 解法 设计+模拟
按照题目要求，依次实现各个函数即可：
- $Lock$ ：可以用一个数组变量 $lockNodeUser$ 记录给各个节点上锁的用户，$lockNodeUser[num]$ 即表示给节点 $num$ 上锁的用户。当 $lockNodeUser[num]=−1$ 时，即表示 节点 $num$ 未被上锁，通过给 $lockNodeUser[num]$ 赋值实现上锁。
- $Unlock$ ：通过比较变量 $lockNodeUser[num]$ 和 $user$ 是否先等来判断当前节点是否可以解锁，通过赋值来解锁。
- $Upgrade$ ：实现较为复杂，首先需要判断三个条件是否同时成立，如果是，还需要给指定节点上锁并且给它的所有子孙节点解锁。三个条件中：
    - **指定节点当前状态为未上锁**：通过变量 $lockNodeUser$ 来判断。
    - **指定节点没有任何上锁的祖先节点**：需要依次遍历当前节点的父亲节点，通过变量 $lockNodeUser$ 和 $parent$ 来判断。具体代码中，我们利用一个函数 $hasLockedAncestor$ 来实现这一判断。
    - **指定节点至少有一个上锁状态的子孙节点**：我们将这一判断放到第三步来进行，==使得它可以和「**给它的所有子孙节点解锁**」同时实现==。
    - 三个状态的判断，我们用「短路与」来连接，当只有前两步都为真，才会进行第三步。当第三步也为真，那么我们就需要进行「给它的所有子孙节点解锁」这一步；==当第三步为假，就说明指定节点没有上锁的子孙节点，那么我们仍可以进行「给它的所有子孙节点解锁」这一步，并不影响树的状态==。我们定义一个递归函数 $checkAndUnlockDescendant$  来实现这一步，返回一个布尔值表示**当前节点是否有上锁的子孙节点**（**也包括自己**），同时**将所有的子孙节点（也包括自己）解锁**。遍历子孙节点时，我们提前构建一个变量 $children$ ，表示当前节点的孩子节点，这一步可以在初始化时完成。
    - 最后，**如果这三个条件与的结果为真，将当前节点上锁**。

```cpp
class LockingTree {
private:
    vector<int> parent;
    vector<int> lockNodeUser;
    vector<vector<int>> children;
public:
    LockingTree(vector<int>& parent) {
        int n = parent.size();
        this->parent = parent; // 用于遍历祖先
        this->lockNodeUser = vector<int>(n, -1);

        this->children = vector<vector<int>>(n); // 邻接表实现多叉树,用于遍历子孙
        for (int i = 0; i < n; ++i)
            if (parent[i] != -1) 
                children[parent[i]].emplace_back(i);
    }
    
    bool lock(int num, int user) {
        if (lockNodeUser[num] == -1) {
            lockNodeUser[num] = user;
            return true;
        }
        return false;
    }
    
    bool unlock(int num, int user) {
        if (lockNodeUser[num] == user) {
            lockNodeUser[num] = -1;
            return true;
        }
        return false;
    }
    
    bool upgrade(int num, int user) {
        bool flag = lockNodeUser[num] == -1  // 当前节点没有锁 
            && !hasLockedAncestor(num)       // 没有上锁的祖先(包括自己)
            && checkAndUnlockDescendant(num); // 有上锁的子孙节点(包括自己)就解锁;没有也解锁
        if (flag) lockNodeUser[num] = user;
        return flag;
    }

    bool hasLockedAncestor(int num) {
        num = parent[num];
        while (num != -1) {
            if (lockNodeUser[num] != -1) // 存在上锁的祖先
                return true;
            num = parent[num];
        }
        return false;
    }

    bool checkAndUnlockDescendant(int num) {
        bool res = lockNodeUser[num] != -1;
        lockNodeUser[num] = -1;
        for (int child : children[num]) 
            res |= checkAndUnlockDescendant(child); // 递归检查和解锁
        return res;
    }
};
```
复杂度分析：
- 时间复杂度：初始化：构建 $children$ 消耗 $O(n)$ ，$Lock$ 和 $Unlock$ 都消耗 $O(1)$ ，$Upgrade$ 消耗 $O(n)$ 。
- 空间复杂度：初始化消耗 $O(n)$ ，$Lock$ 和 $Unlock$ 都消耗 $O(1)$ ，$Upgrade$ 消耗 $O(n)$。

