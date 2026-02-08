> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202310091713840.png)
DP 的内循环可以用最小堆，里面放的元素是：某节点的最小的次数和该节点对应的能够到达的最远的路径。如果堆顶的元素不能到达当前点，就弹出，直到第一个能够到达的。

解题思路
这道题是典型的贪心算法，通过局部最优解得到全局最优解。以下两种方法都是使用贪心算法实现，只是贪心的策略不同。

方法一：反向查找出发位置
我们的目标是到达数组的最后一个位置，因此我们可以考虑最后一步跳跃前所在的位置，该位置通过跳跃能够到达最后一个位置。

如果有多个位置通过跳跃都能够到达最后一个位置，那么我们应该如何进行选择呢？直观上来看，我们可以「贪心」地选择距离最后一个位置最远的那个位置，也就是对应下标最小的那个位置。因此，我们可以从左到右遍历数组，选择第一个满足要求的位置。

找到最后一步跳跃前所在的位置之后，我们继续贪心地寻找倒数第二步跳跃前所在的位置，以此类推，直到找到数组的开始位置。

使用这种方法编写的 C++ 和 Python 代码会超出时间限制，因此我们只给出 Java 和 Go 代码。

Java
Golang
class Solution {
    public int jump(int[] nums) {
        int position = nums.length - 1;
        int steps = 0;
        while (position > 0) {
            for (int i = 0; i < position; i++) {
                if (i + nums[i] >= position) {
                    position = i;
                    steps++;
                    break;
                }
            }
        }
        return steps;
    }
}
复杂度分析

时间复杂度：O(n2)O(n^2)O(n 
2
 )，其中 nnn 是数组长度。有两层嵌套循环，在最坏的情况下，例如数组中的所有元素都是 111，position 需要遍历数组中的每个位置，对于 position 的每个值都有一次循环。

空间复杂度：O(1)O(1)O(1)。

方法二：正向查找可到达的最大位置
方法一虽然直观，但是时间复杂度比较高，有没有办法降低时间复杂度呢？

如果我们「贪心」地进行正向查找，每次找到可到达的最远位置，就可以在线性时间内得到最少的跳跃次数。

例如，对于数组 [2,3,1,2,4,2,3]，初始位置是下标 0，从下标 0 出发，最远可到达下标 2。下标 0 可到达的位置中，下标 1 的值是 3，从下标 1 出发可以达到更远的位置，因此第一步到达下标 1。

从下标 1 出发，最远可到达下标 4。下标 1 可到达的位置中，下标 4 的值是 4 ，从下标 4 出发可以达到更远的位置，因此第二步到达下标 4。

![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202307061504079.png)





在具体的实现中，我们维护当前能够到达的最大下标位置，记为边界。我们从左到右遍历数组，到达边界时，更新边界并将跳跃次数增加 1。

在遍历数组时，我们不访问最后一个元素，这是因为在访问最后一个元素之前，我们的边界一定大于等于最后一个位置，否则就无法跳到最后一个位置了。如果访问最后一个元素，在边界正好为最后一个位置的情况下，我们会增加一次「不必要的跳跃次数」，因此我们不必访问最后一个元素。

Java
C++
Python3
Golang
class Solution {
    public int jump(int[] nums) {
        int length = nums.length;
        int end = 0;
        int maxPosition = 0; 
        int steps = 0;
        for (int i = 0; i < length - 1; i++) {
            maxPosition = Math.max(maxPosition, i + nums[i]); 
            if (i == end) {
                end = maxPosition;
                steps++;
            }
        }
        return steps;
    }
}
复杂度分析

时间复杂度：O(n)O(n)O(n)，其中 nnn 是数组长度。

空间复杂度：O(1)O(1)O(1)。

下一篇题解
【跳跃游戏 II】别想那么多，就挨着跳吧 II
评论 (345)

排序:最热

预览
评论

Terry
发布于 黑龙江
2021.01.19
懂和不懂我觉得就是差这几行注释： 思想就一句话：每次在上次能跳到的范围（end）内选择一个能跳的最远的位置（也就是能跳到max_far位置的点）作为下次的起跳点 ！

class Solution {
public:
    int jump(vector<int>& nums) 
    {
        int max_far = 0;// 目前能跳到的最远位置
        int step = 0;   // 跳跃次数
        int end = 0;    // 上次跳跃可达范围右边界（下次的最右起跳点）
        for (int i = 0; i < nums.size() - 1; i++)
        {
            max_far = std::max(max_far, i + nums[i]);
            // 到达上次跳跃能到达的右边界了
            if (i == end)
            {
                end = max_far;  // 目前能跳到的最远位置变成了下次起跳位置的有边界
                step++;         // 进入下一次跳跃
            }
        }
        return step;
    }
};
537
展示 35 条回复
回复

LeetCode
发布于 未知归属地
2020.05.04
"每天起床第一句，先去看看力扣题。自从有了打卡题，每天变得更努力。写完看看扣友圈，题解视频有魔力。打卡，我要打卡，我要编程万人敌。《燃烧我的力扣题》"——选自扣友语录
力扣新青年，既是代码小能手，更是段子手，还可以是灵魂歌手。
祝各位身怀绝技、才华横溢的扣友们青年节快乐！在力扣每日一题，每一天都正青春。

692
展示 24 条回复
回复

字母、

L3
发布于 广东
2022.11.14
dp专栏的题目用dp做执行用时超越5% 。。

62
展示 5 条回复
回复

重刹进入一号弯
发布于 山西
2021.06.29
代码和解释可以说毫无关系，太差了

310
展示 2 条回复
回复

Jacob
L1
发布于 湖南
2023.03.14
这里放个dp做法，大多数人都是用的贪心，所以只超过了%5的用户（）

（所以说为什么要在dp题单里放一个正解为贪心的题。。）

做法：先预处理出每个元素能够有哪些前驱，之后从前往后dp，注意求的是最小值

class Solution {
public:
    int jump(vector<int>& nums) {
        int n = nums.size();
        int mx = *max_element(nums.begin(), nums.end());
        vector<vector<int>> pre(n + mx + 10);
        for(int i = 0; i < n; ++i){
            for(int j = i; j <= i + nums[i]; ++j){
                pre[j].push_back(i);
            }
        }
        vector<int> dp(n);
        for(int i = 0; i < n; ++i) dp[i] = 2e4;
        dp[0] = 0;
        for(int i = 1; i < n; ++i){
            for(auto v : pre[i]){
                dp[i] = min(dp[v] + 1, dp[i]);
            }
        }
        return dp[n - 1];
    }
};
5
回复

fengcc_cn
发布于 浙江
2022.02.07
个人感觉方法二的代码换种方式更好理解，end 维护的是当前这一跳能达到的最右位置，若要超过该位置必须要进行一次跳跃，因此需将跳跃次数加1，并更新这次跳跃能到达的最右位置

43
展示 6 条回复
回复

星辰耀乌月落
发布于 北京
2021.08.17
我是煞笔

67
展示 3 条回复
回复

是江江啦啦啦啊
L1
发布于 广西
2022.08.08
我觉得按照官方题解的方法2，代码应该是这个样子。

int jump(vector<int>& nums) {
    int n = nums.size();
    int jumpNum = 0;
    for(int i = 0; i < n - 1; ) {
        int cover = i + nums[i];// 当前能跳到的最远地方
        if(cover >= n - 1) return jumpNum + 1;// 如果能够调到最后一个元素，则直接跳
        // 否则，筛选下一个跳跃元素
        int maxIndex = 0;
        for(int j = i + 1; j <= i + nums[i]; j++) {
            if(j + nums[j] > cover) { // 覆盖范围更广，则更新cover和对应下标
                cover = j + nums[j];
                maxIndex = j;
            }
        }      
        i = maxIndex;// 选择能使得cover最大的下标，跳到该数字上
        jumpNum++;
    }
    return jumpNum;
}
展开全部
19
展示 6 条回复
回复

WRJ90901
发布于 湖北
(编辑过)
2023.04.19
从「起点」开始考虑比较好理解。

由于起点是一定要经过的，因此，我们从起点开始跳，并初始化为第一步 step = 1，同时我们能得到从起点可以到达的最远距离，我们将其设置为第一步的边界 end = nums[0]；

接下来，贪心的直觉告诉我们：「我们不管起跳的方案是什么，我们只要每次贪心地能跳到最远即可」，因此，我们从起点的右边开始，也就是从第二个点开始，从左到右遍历每个能到达的地点，只要当前的点没到边界 end，说明我们还在第一步能够到达的范围内，步数还是原来的步数 1，同时，在到达第一步的边界 end 之前，我们贪心地更新第二个点到边界 end 之间每一个点能够到达的最远距离，这之间的点能够到达的最远距离在未来就会成为第二步能够到达的最远距离，也就是将来第二步的边界，因为「第二步的起跳点一定是从第二个点到起点的边界 end 之间的」，我们用 nextMaxEnd 表示；

OK，一旦遍历的当前点来到了第一步的边界 end，说明第一步能够到达的最远距离已经走到了，必须跳第二步了 (注意这里并不是真正的起跳点，只是已经到达了第一步能到达的最远距离而已，无论如何你都必须跳出第二步了，我们只是贪心地每次都到最远距离了才肯跳出下一步，但一定要记住，这并不是真正的起跳方案)，此时就把边界 end 更新为 nextMaxEnd 表示第二步能够到达的最远距离，也就是第二步的边界，并且 step++，表示你无论如何都得跳出第二步了；

以此类推，不断地在到达当前步的边界 end 前更新下一步能够到达的最远距离nextMaxEnd 作为下一步的边界，更新途中一旦到达当前步的边界 end，就表示当前步已经贪心地走完了，必须跳下一步了，step++，并且下一步能够到达的最远边界我们已经在之前比较出来了，直接更新即可：end = nextMaxEnd。

到这里，其实就可以明白为什么不用遍历终点了，因为边界 end 有可能是终点，那 end 一旦到了终点其实也就到了，但是我们的代码逻辑是到达 end 就得步数加一，其实这是没必要的。

下面的代码和官方的有一些不同，我是先初始化了第一步，比较好理解。

class Solution {
    public int jump(int[] nums) {
        if(nums.length == 1) return 0;

        // 下面三行代码初始化第一步
        int step = 1;       // 第一步
        int end = nums[0];  // 第一步的边界，也就是第一步能到达的最远距离
        int nextMaxEnd = 0; // 下一步能到达的最远边界 (只要当前步到达它的边界 end，就更新为 nextMaxEnd 作为下一步的边界 end)

        for(int i = 1; i < nums.length-1; i++) {
            nextMaxEnd = Math.max(nextMaxEnd, nums[i]+i);
            if(i == end) {
                end = nextMaxEnd;
                step++;
            }
        }

        return step;
    }
}

作者：力扣官方题解
链接：https://leetcode.cn/problems/jump-game-ii/solutions/230241/tiao-yue-you-xi-ii-by-leetcode-solution/
来源：力扣（LeetCode）
著作权归作者所有。商业转载请联系作者获得授权，非商业转载请注明出处。
                                     ```cpp
class Solution {
public:
    int jump(vector<int>& nums) {
        int n = nums.size();
        int mx = *max_element(nums.begin(), nums.end());
        vector<vector<int>> pre(n + mx + 10);
        for(int i = 0; i < n; ++i){
            for(int j = i; j <= i + nums[i]; ++j){
                pre[j].push_back(i);
            }
        }
        vector<int> dp(n);
        for(int i = 0; i < n; ++i) dp[i] = 2e4;
        dp[0] = 0;
        for(int i = 1; i < n; ++i){
            for(auto v : pre[i]){
                dp[i] = min(dp[v] + 1, dp[i]);
            }
        }
        return dp[n - 1];
    }
};
```