> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

给你两个字符串数组 `positive_feedback` 和 `negative_feedback` ，分别包含表示正面的和负面的词汇。**不会** 有单词同时是正面的和负面的。

一开始，每位学生分数为 `0` 。每个正面的单词会给学生的分数 **加** `3` 分，每个负面的词会给学生的分数 **减**  `1` 分。

给你 `n` 个学生的评语，用一个下标从 **0** 开始的字符串数组 `report` 和一个下标从 **0** 开始的整数数组 `student_id` 表示，其中 `student_id[i]` 表示这名学生的 ID ，这名学生的评语是 `report[i]` 。每名学生的 ID **互不相同**。

给你一个整数 `k` ，请你返回按照得分 **从高到低** 最顶尖的 `k` 名学生。如果有多名学生分数相同，ID 越小排名越前。

**示例 1：**

**输入：**positive_feedback = ["smart","brilliant","studious"], negative_feedback = ["not"], report = ["this student is studious","the student is smart"], student_id = [1,2], k = 2
**输出：**[1,2]
**解释：**
两名学生都有 1 个正面词汇，都得到 3 分，学生 1 的 ID 更小所以排名更前。

**示例 2：**

**输入：**positive_feedback = ["smart","brilliant","studious"], negative_feedback = ["not"], report = ["this student is not studious","the student is smart"], student_id = [1,2], k = 2
**输出：**[2,1]
**解释：**
- ID 为 1 的学生有 1 个正面词汇和 1 个负面词汇，所以得分为 3-1=2 分。
- ID 为 2 的学生有 1 个正面词汇，得分为 3 分。
学生 2 分数更高，所以返回 [2,1] 。

**提示：**

- `1 <= positive_feedback.length, negative_feedback.length <= 10^4`
- `1 <= positive_feedback[i].length, negative_feedback[j].length <= 100`
- `positive_feedback[i]` 和 `negative_feedback[j]` 都只包含小写英文字母。
- `positive_feedback` 和 `negative_feedback` 中不会有相同单词。
- `n == report.length == student_id.length`
- `1 <= n <= 10^4`
- `report[i]` 只包含小写英文字母和空格 `' '` 。
- `report[i]` 中连续单词之间有单个空格隔开。
- `1 <= report[i].length <= 100`
- `1 <= student_id[i] <= 10^9`
- `student_id[i]` 的值 **互不相同** 。
- `1 <= k <= n`
```java
class Solution {
    public List<Integer> topStudents(String[] positive_feedback, String[] negative_feedback, String[] report, int[] student_id, int k) {
        var pos = new HashSet<String>();
        for (String p : positive_feedback) pos.add(p);
        var neg = new HashSet<String>();
        for (String n : negative_feedback) neg.add(n);
        var rec = new HashMap<Integer, Integer>();
        int idx = 0;
        for (String r : report) {
            String[] splitted = r.split(" ");
            int grade = 0;
            for (String s : splitted) {
                if (pos.contains(s)) grade += 3;
                else if (neg.contains(s)) --grade;
            }
            // System.out.println(student_id[idx] + " " + grade);
            rec.put(student_id[idx++], grade);
        }
        Integer[] students = new Integer[student_id.length];
        for (int i = 0; i < student_id.length; ++i) students[i] = student_id[i];
        Arrays.sort(students, new Comparator<Integer>() {
            @Override
            public int compare(Integer a, Integer b) {
                int ga = rec.get(a), gb = rec.get(b);
                // System.out.println(ga + ":" + gb);
                if (ga != gb) return gb - ga;
                return a - b;
            }
        });
        List<Integer> ans = new ArrayList<Integer>();
        for (int i = 0; i < k; ++i) ans.add(students[i]);
        return ans;
    }
}
```

把 feedback\textit{feedback}feedback 及其分数存到哈希表 score\textit{score}score 中，对每个 reporti\textit{report}_ireport 
i
​
 ，按照空格分割，然后用 score\textit{score}score 计算分数之和。

最后按照题目规则排序，取前 kkk 个 studentId\textit{studentId}studentId 为答案。

Python3
Go
class Solution:
    def topStudents(self, positive_feedback: List[str], negative_feedback: List[str], report: List[str], student_id: List[int], k: int) -> List[int]:
        score = defaultdict(int)
        for w in positive_feedback: score[w] = 3
        for w in negative_feedback: score[w] = -1
        a = sorted((-sum(score[w] for w in r.split()), i) for r, i in zip(report, student_id))
        return [i for _, i in a[:k]]
复杂度分析
时间复杂度：O((p+q+n)L+nlog⁡n)O((p+q+n)L+n\log n)O((p+q+n)L+nlogn)，其中 ppp 为 positiveFeedback\textit{positiveFeedback}positiveFeedback 的长度，qqq 为 negativeFeedback\textit{negativeFeedback}negativeFeedback 的长度，nnn 为 report\textit{report}report 的长度，LLL 为字符串的平均长度。
空间复杂度：O((p+q)L+n)O((p+q)L+n)O((p+q)L+n)。

作者：灵茶山艾府
链接：https://leetcode.cn/problems/reward-top-k-students/solutions/2031819/mo-ni-by-endlesscheng-kqzb/
来源：力扣（LeetCode）
著作权归作者所有。商业转载请联系作者获得授权，非商业转载请注明出处。