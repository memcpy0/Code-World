

The Game of Master Mind is played as follows:

The computer has four slots, and each slot will contain a ball that is red (R). yellow (Y). green (G) or blue (B). For example, the computer might have RGGB (Slot #1 is red, Slots #2 and #3 are green, Slot #4 is blue).

You, the user, are trying to guess the solution. You might, for example, guess YRGB.

When you guess the correct color for the correct slot, you get a "hit:' If you guess a color that exists but is in the wrong slot, you get a "pseudo-hit:' Note that a slot that is a hit can never count as a pseudo-hit.

For example, if the actual solution is RGBY and you guess GGRR, you have one hit and one pseudo-hit. Write a method that, given a guess and a solution, returns the number of hits and pseudo-hits.

Given a sequence of colors `solution`, and a `guess`, write a method that return the number of hits and pseudo-hit `answer`, where `answer[0]` is the number of hits and `answer[1]` is the number of pseudo-hit.

**Example:**

```swift
Input:  solution="RGBY",guess="GGRR"
Output:  [1,1]
Explanation:  hit once, pseudo-hit once.
```

**Note:**
- `len(solution) = len(guess) = 4`
- There are only `"R"`,`"G"`,`"B"`,`"Y"` in `solution` and `guess`.


题意：一个名为珠玑妙算的游戏——计算机有4个槽，每个槽放一个球，颜色可能是红色（R）、黄色（Y）、绿色（G）或蓝色（B）。用户需要猜出颜色组合，要是猜对某个槽的颜色，则算一次“猜中”；要是只猜对颜色但槽位猜错了，则算一次“伪猜中”。注意，“猜中”不能算入“伪猜中”。给定一种颜色组合`solution` 和一个猜测 `guess`，编写一个方法，返回猜中和伪猜中的次数 `answer`，其中 `answer[0]` 为猜中的次数，`answer[1]` 为伪猜中的次数。
 
----
### 解法 使用字符频率数组
计算**猜中次数**很简单，关键在于如何计算**伪猜中次数**。我们需要创建一个具有每个元素发生频率的数组，然后计算 `solution, guess` 中四种元素的出现次数，得到**可能猜中的最大次数**。如对 `solution = "RGBY", guess = "GGRR"` ，其中 `'R'` 在 `solution` 中出现了一次、在 `guess` 中出现了两次，`'G'` 在 `solution` 中出现了一次，在 `guess` 中出现了两次，于是**可能猜中的最大次数**等于 `2` 。而后可知，**伪猜中次数=可能猜中的最大次数-猜中次数**。 具体代码如下：
```cpp
class Solution {
public:
    vector<int> masterMind(string solution, string guess) {
        vector<int> ans(2, 0); //[0, 0]
        if (solution.empty()) return ans;
        for (int i = 0; i < 4; ++i) if (solution[i] == guess[i]) ++ans[0]; //猜中
        int sset[26] = {0}, gset[26] = {0};
        for (const char &c : solution) ++sset[c - 'A'];
        for (const char &c : guess) ++gset[c - 'A'];
        ans[1] += min(sset['R' - 'A'], gset['R' - 'A']);
        ans[1] += min(sset['G' - 'A'], gset['G' - 'A']);
        ans[1] += min(sset['B' - 'A'], gset['B' - 'A']);
        ans[1] += min(sset['Y' - 'A'], gset['Y' - 'A']);
        ans[1] -= ans[0]; //全部可能的命中次数-正确命中次数=伪命中次数
        return ans;
    }
};
```
效率如下：
```cpp
执行用时：0 ms, 在所有 C++ 提交中击败了100.00% 的用户
内存消耗：6.2 MB, 在所有 C++ 提交中击败了54.68% 的用户
```
