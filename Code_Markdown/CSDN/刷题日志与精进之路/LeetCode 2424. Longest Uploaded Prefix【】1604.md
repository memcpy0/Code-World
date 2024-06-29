> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

给你一个 `n` 个视频的上传序列，每个视频编号为 `1` 到 `n` 之间的 **不同** 数字，你需要依次将这些视频上传到服务器。请你实现一个数据结构，在上传的过程中计算 **最长上传前缀** 。

如果 **闭区间** `1` 到 `i` 之间的视频全部都已经被上传到服务器，那么我们称 `i` 是上传前缀。最长上传前缀指的是符合定义的 `i` 中的 **最大值** 。  
  
请你实现 `LUPrefix` 类：

- `LUPrefix(int n)` 初始化一个 `n` 个视频的流对象。
- `void upload(int video)` 上传 `video` 到服务器。
- `int longest()` 返回上述定义的 **最长上传前缀** 的长度。

**示例 1：**

**输入：**
["LUPrefix", "upload", "longest", "upload", "longest", "upload", "longest"]
[[4], [3], [], [1], [], [2], []]
**输出：**
[null, null, 0, null, 1, null, 3]

**解释：**
LUPrefix server = new LUPrefix(4);   // 初始化 4个视频的上传流
server.upload(3);                    // 上传视频 3 。
server.longest();                    // 由于视频 1 还没有被上传，最长上传前缀是 0 。
server.upload(1);                    // 上传视频 1 。
server.longest();                    // 前缀 [1] 是最长上传前缀，所以我们返回 1 。
server.upload(2);                    // 上传视频 2 。
server.longest();                    // 前缀 [1,2,3] 是最长上传前缀，所以我们返回 3 。

**提示：**
- `1 <= n <= 10^5`
- `1 <= video <= 10^5`
- `video` 中所有值 **互不相同** 。
- `upload` 和 `longest` **总调用** 次数至多不超过 `2 * 10^5` 次。
- 至少会调用 `longest` 一次。

```cpp
class LUPrefix {
private:
    vector<int> tree;
    int n;
    int lo = 0, hi = 0;
    void add(int x, int v) {
        for (int i = x; i <= n; i += (i & -i)) 
            tree[i] += v;
    }
    int sum(int x) {
        int ans = 0;
        for (int i = x; i; i -= (i & -i))
            ans += tree[i];
        return ans;
    }
public:
    LUPrefix(int n) {
        this->n = n;
        tree.resize(1e5 + 10);
    }
    
    void upload(int video) {
        hi = max(video, hi);
        add(video, 1);
    }
    
    int longest() {
        int l = lo, r = hi;
        while (l < r) {
            int mid = (l + r + 1) >> 1;
            int t = sum(mid);
            // cout << mid << " " << t << endl;
            if (t >= mid) l = mid;
            else r = mid - 1;
        }
        // cout << "sdf" << l <<endl;
        return l;
    }
};
```