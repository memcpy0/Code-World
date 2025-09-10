> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

一个 ATM 机器，存有 `5` 种面值的钞票：`20` ，`50` ，`100` ，`200` 和 `500` 美元。初始时，ATM 机是空的。用户可以用它存或者取任意数目的钱。

取款时，机器会优先取 **较大** 数额的钱。

- 比方说，你想取 `$300` ，并且机器里有 `2` 张 `$50` 的钞票，`1` 张 `$100` 的钞票和`1` 张 `$200` 的钞票，那么机器会取出 `$100` 和 `$200` 的钞票。
- 但是，如果你想取 `$600` ，机器里有 `3` 张 `$200` 的钞票和`1` 张 `$500` 的钞票，那么取款请求会被拒绝，因为机器会先取出 `$500` 的钞票，然后无法取出剩余的 `$100` 。注意，因为有 `$500` 钞票的存在，机器 **不能** 取 `$200` 的钞票。

请你实现 ATM 类：

- `ATM()` 初始化 ATM 对象。
- `void deposit(int[] banknotesCount)` 分别存入 `$20` ，`$50`，`$100`，`$200` 和 `$500` 钞票的数目。
- `int[] withdraw(int amount)` 返回一个长度为 `5` 的数组，分别表示 `$20` ，`$50`，`$100` ，`$200` 和 `$500` 钞票的数目，并且更新 ATM 机里取款后钞票的剩余数量。如果无法取出指定数额的钱，请返回 `[-1]` （这种情况下 **不** 取出任何钞票）。

**示例 1：**

**输入：**
["ATM", "deposit", "withdraw", "deposit", "withdraw", "withdraw"]
[[], [[0,0,1,2,1]], [600], [[0,1,0,1,1]], [600], [550]]
**输出：**
[null, null, [0,0,1,0,1], null, [-1], [0,1,0,0,1]]

**解释：**
ATM atm = new ATM();
atm.deposit([0,0,1,2,1]); // 存入 1 张 $100 ，2 张 $200 和 1 张 $500 的钞票。
atm.withdraw(600);        // 返回 [0,0,1,0,1] 。机器返回 1 张 $100 和 1 张 $500 的钞票。机器里剩余钞票的数量为 [0,0,0,2,0] 。
atm.deposit([0,1,0,1,1]); // 存入 1 张 $50 ，1 张 $200 和 1 张 $500 的钞票。
                          // 机器中剩余钞票数量为 [0,1,0,3,1] 。
atm.withdraw(600);        // 返回 [-1] 。机器会尝试取出 $500 的钞票，然后无法得到剩余的 $100 ，所以取款请求会被拒绝。
                          // 由于请求被拒绝，机器中钞票的数量不会发生改变。
atm.withdraw(550);        // 返回 [0,1,0,0,1] ，机器会返回 1 张 $50 的钞票和 1 张 $500 的钞票。

**提示：**

- `banknotesCount.length == 5`
- `0 <= banknotesCount[i] <= 10^9`
- `1 <= amount <= 10^9`
- **总共** 最多有 `5000` 次 `withdraw` 和 `deposit` 的调用。
- 函数 `withdraw` 和 `deposit` 至少各有 **一次** 调用。

```cpp
class ATM {
private:
    int money[5] = {20, 50, 100, 200, 500};
    vector<long long> cnt;
public:
    ATM() : cnt(5) {  }
    
    void deposit(vector<int> banknotesCount) {
        for (int i = 0; i < 5; ++i) 
            cnt[i] += banknotesCount[i];
    }
    
    vector<int> withdraw(int amount) {
        vector<int> ans(5);
        for (int i = 4; i >= 0; --i) {
            if (cnt[i] == 0) continue;
            long long t = cnt[i] * money[i];
            // cout << amount << ", "<< t << endl;
            if (t > amount) {
                int tcnt = amount / money[i];
                if (tcnt * money[i] == amount) {
                    ans[i] = tcnt;
                    cnt[i] -= tcnt;
                    return ans;
                } else if (tcnt * money[i] < amount) {
                    cnt[i] -= tcnt;
                    ans[i] = tcnt;
                    amount -= tcnt * money[i];
                }
            }
            else if (t == amount) {
                ans[i] = cnt[i];
                cnt[i] = 0;
                return ans;
            } else {
                ans[i] = cnt[i];
                cnt[i] = 0;
                amount -= t;
            }
        }
        if (amount) {
            for (int j = 0; j <= 4; ++j) cnt[j] += ans[j]; // 恢复
                return {-1};
        }
        return ans;
    }
};
```