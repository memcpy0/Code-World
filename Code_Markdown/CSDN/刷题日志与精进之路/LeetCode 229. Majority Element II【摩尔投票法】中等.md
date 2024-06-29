> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

给定一个大小为 _n_ 的整数数组，找出其中所有出现超过 `⌊ n/3 ⌋` 次的元素。

**示例 1：**
```js
输入：nums = [3,2,3]
输出：[3]
```
**示例 2：**
```js
输入：nums = [1]
输出：[1]
```
**示例 3：**
```js
输入：nums = [1,2]
输出：[1,2]
```
**提示：**
- `1 <= nums.length <= 5 * 10^4`
- `-10^9 <= nums[i] <= 10^9`

**进阶：** 尝试设计时间复杂度为 O(n)、空间复杂度为 O(1)的算法解决此问题。

---
题目集合：
- [169. 多数元素](https://leetcode.cn/problems/majority-element/)
- [面试题 17.10. Find Majority Element LCCI](https://leetcode.cn/problems/find-majority-element-lcci/)
- [229. 多数元素 II](https://leetcode.cn/problems/majority-element-ii/)
- 1150. Check If a Number Is Majority Element in a Sorted Array
- [1157. 子数组中占绝大多数的元素](https://leetcode.cn/problems/online-majority-element-in-subarray/)：困难

### 解法 摩尔投票法
在之前的169和面试题17.10中，我们使用「摩尔投票」在 $O(1)$ 空间复杂度内找到了出现次数超过一半的元素，即出现次数大于 $n / 2$ 的数。并使用了例子进行辅助证明。有兴趣的可以参考论文的证明过程，论文地址：[MJRTYA Fast Majority Vote Algorithm](https://leetcode.cn/link/?target=https%3A%2F%2Fwww.cs.ou.edu%2F~rlpage%2Fdmtools%2Fmjrty.pdf)

对于本题，我们需要统计出现次数超过 $n / 3$ 的数。**不失一般性的将其拓展为「统计出现次数超过 $n / k$ 的数」**。

可以证明，**出现次数超过 $n / k$ 的数最多只有 $k - 1$ 个**。否则必然违背「数总共只有 $n$ 个」或者「当前统计的是出现次数超过 $n / k$ 的数」的前提条件。当明确了符合要求的数的数量之后，我们可以使用有限变量来代表这 $k - 1$ 个候选数及其出现次数。

然后使用「摩尔投票」的标准做法，在遍历数组时同时 $check$ 这 $k - 1$ 个数，假设当前遍历到的元素为 $x$ ：
- 如果 $x$ 本身是候选者的话，则对其出现次数加一；
- 如果 $x$ 本身不是候选者，检查是否有候选者的出现次数为 $0$ ：
    - 若有，则让 $x$ 代替其成为候选者，并记录出现次数为 $1$ ；
    - 若无，则**让所有候选者的出现次数减一**。
- 当处理完整个数组后，**这 $k - 1$ 个数可能会被填满，但不一定都是符合出现次数超过 $n / k$ 要求的**。需要进行二次遍历，来确定候选者是否符合要求，将符合要求的数加到答案。

上述做法正确性的关键是：==若存在出现次数超过 $n / k$ 的数，最后必然会成为这 $k - 1$ 个候选者之一==。我们可以通过「反证法」来进行证明：若出现次数超过 $n / k$ 的数 $x$ 最终没有成为候选者。有两种可能会导致这个结果：
- 数值 $x$ 从来没成为过候选者：如果 $x$ 从来没成为过候选者，那么在遍历 $x$ 的过程中，必然有 $k-1$ 个候选者被减了超过 $n/k$ 次，假设当前 $x$ 出现次数为 $C$ ，已知 $C > n / k$（前提条件），此时总个数为
$$(k−1)∗C+C=C∗k$$
    再根据 $C > n / k$ ，可知 $C * k > n$ ，而我们总共就只有 $n$ 个数，因此该情况恒不成立。
- 数值 $x$ 成为过候选者，但被逐出替换了：同理，被逐出替换，说明发生了对 $x$ 出现次数减一的动作（减到 $0$ ），每次减一操作，意味着有其余的 $k - 2$ 个候选者的出现次数也发生了减一动作，加上本身被遍历到的当前数 $num[i]$ ，共有 $k-1$ 个数字和 $x$ 被一同统计。 因此，根据我们摩尔投票的处理过程，如果 $x$ 成为过候选者，并被逐出替换，那么同样能够推导出我们存在超过 $n$ 个数。
- 综上，**如果存在出现次数超过 $n/k$ 的数，其必然会成为 $k-1$ 个候选者之一**。

一个对摩尔投票法非常形象的比喻是：**多方混战**。我们把这道题比作一场三方混战，战斗结果一定只有最多两个阵营幸存，其他阵营被歼灭。数组中的数字即代表某士兵所在的阵营。

我们维护两个潜在幸存阵营 $A$ 和 $B$ 。遍历数组：
- 如果遇到了属于 $A$ 或者属于 $B$ 的士兵，则把士兵加入 $A$ 或 $B$ 队伍中，该队伍人数加一。继续遍历。
- 如果遇到了一个士兵既不属于 $A$ 阵营，也不属于 $B$ 阵营，这时有两种情况：
    1. $A$ 阵营和 $B$ 阵营都还有活着的士兵，那么进行一次厮杀，参与厮杀的三个士兵全部阵亡：$A$ 阵营的一个士兵阵亡，$B$ 阵营的一个士兵阵亡，这个不知道从哪个阵营来的士兵也阵亡。继续遍历。
    2. $A$ 阵营或 $B$ 阵营已经没有士兵了。这个阵营暂时消失了。那么把当前遍历到的新士兵算作新的潜在幸存阵营，这个新阵营只有他一个人。继续遍历。
- 大战结束，最后 $A$ 和 $B$ 阵营就是初始人数最多的阵营。判断一下 $A,B$ 的人数是否超过所有人数的三分之一即可。

```cpp
class Solution {
public:
    vector<int> majorityElement(vector<int> & nums, int k){
        vector<int> cand(k-1, INT_MIN);
        vector<int> cnt(k-1, 0);
        for(int i = 0; i < nums.size(); i++){
            bool oldcand = false;
            for(int j = 0; j < k-1; j++){
                if(cand[j] == nums[i]){
                    cnt[j]++;
                    oldcand = true;
                    break;
                }
            }
            if(!oldcand){
                bool newcand = false;
                for(int j = 0; j < k-1; j++){
                    if(cnt[j] == 0){
                        cand[j] = nums[i];
                        cnt[j] = 1;
                        newcand = true;
                        break;
                    }
                }
                if(!newcand){
                    for(int j = 0; j < k-1; j++){
                        cnt[j]--;
                    }
                }
            }
        }
        vector<int> res; 
        int target = nums.size() / k;
        for(int i = 0; i < k-1; i++){
            int count = 0;
            for(int j = 0; j < nums.size(); j++){
                if(cand[i] == nums[j]){
                    count++;
                }
            }
            if(count > target){
                res.push_back(cand[i]); 
            }
        }
        return res;
    }
    vector<int> majorityElement(vector<int>& nums) {
        return majorityElement(nums, 3);    
    }
};
```
复杂度分析：
- 时间复杂度：$O(n)$
- 空间复杂度：$O(1)$