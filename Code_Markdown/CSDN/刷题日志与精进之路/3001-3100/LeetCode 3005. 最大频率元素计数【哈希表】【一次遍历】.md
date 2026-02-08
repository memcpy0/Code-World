> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

给你一个由 **正整数** 组成的数组 `nums` 。

返回数组 `nums` 中所有具有 **最大** 频率的元素的 **总频率** 。

元素的 **频率** 是指该元素在数组中出现的次数。

**示例 1：**
```c
输入：nums = [1,2,2,3,1,4]
输出：4
解释：元素 1 和 2 的频率为 2 ，是数组中的最大频率。
因此具有最大频率的元素在数组中的数量是 4 。
```
**示例 2：**
```c
输入：nums = [1,2,3,4,5]
输出：5
解释：数组中的所有元素的频率都为 1 ，是最大频率。
因此具有最大频率的元素在数组中的数量是 5 。
```
**提示：**
- `1 <= nums.length <= 100`
- `1 <= nums[i] <= 100`
---
### 方法 一次遍历
遍历 $nums$ ，同时用哈希表统计每个元素的出现次数，并维护出现次数的最大值 $maxCnt$ ：
- 如果出现次数 $c \gt maxCnt$ ，那么更新 $maxCnt = c$ ，答案 $ans = c$ ；
- 如果出现次数 $c = maxCnt$ ，那么答案增加 $c$ 。
```cpp
class Solution {
public:
    int maxFrequencyElements(vector<int>& nums) {
        unordered_map<int, int> cnt;
        int ans = 0, max_cnt = 0;
        for (int x : nums) {
            int c = ++cnt[x];
            if (c > max_cnt) {
                ans = max_cnt = c;
            } else if (c == max_cnt) {
                ans += c;
            }
        }
        return ans;
    }
};
```
```java
class Solution {
    public int maxFrequencyElements(int[] nums) {
        Map<Integer, Integer> cnt = new HashMap<>(); // 更快的写法见【Java 数组】
        int maxCnt = 0;
        int ans = 0;
        for (int x : nums) {
            int c = cnt.merge(x, 1, Integer::sum); // c = ++cnt[x]
            if (c > maxCnt) {
                ans = maxCnt = c;
            } else if (c == maxCnt) {
                ans += c;
            }
        }
        return ans;
    }
}
// Java数组
class Solution {
    public int maxFrequencyElements(int[] nums) {
        int mx = 0;
        for (int x : nums) {
            mx = Math.max(mx, x);
        }
        
        int[] cnt = new int[mx + 1];
        int maxCnt = 0;
        int ans = 0;
        for (int x : nums) {
            int c = ++cnt[x];
            if (c > maxCnt) {
                ans = maxCnt = c;
            } else if (c == maxCnt) {
                ans += c;
            }
        }
        return ans;
    }
}
```
```rust
use std::collections::HashMap;

impl Solution {
    pub fn max_frequency_elements(nums: Vec<i32>) -> i32 {
        let mut cnt = HashMap::new();
        let mut max_cnt = 0;
        let mut ans = 0;
        for x in nums {
            let e = cnt.entry(x).or_insert(0);
            *e += 1;
            let c = *e;
            if c > max_cnt {
                max_cnt = c;
                ans = c;
            } else if c == max_cnt {
                ans += c;
            }
        }
        ans
    }
}
```
```python
class Solution:
    def maxFrequencyElements(self, nums: List[int]) -> int:
        cnt = defaultdict(int)
        ans = max_cnt = 0
        for x in nums:
            cnt[x] += 1
            c = cnt[x]
            if c > max_cnt:
                ans = max_cnt = c
            elif c == max_cnt:
                ans += c
        return ans
```
```go
func maxFrequencyElements(nums []int) (ans int) {
	cnt := map[int]int{}
	maxCnt := 0
	for _, x := range nums {
		cnt[x]++
		c := cnt[x]
		if c > maxCnt {
			maxCnt = c
			ans = c
		} else if c == maxCnt {
			ans += c
		}
	}
	return
}
```
```c
#define MAX(a, b) ((b) > (a) ? (b) : (a))

int maxFrequencyElements(int* nums, int numsSize) {
    int mx = 0; // 直接初始化 mx = 100 可以做到一次遍历
    for (int i = 0; i < numsSize; i++) {
        mx = MAX(mx, nums[i]);
    }

    int* cnt = calloc(mx + 1, sizeof(int));
    int max_cnt = 0;
    int ans = 0;

    for (int i = 0; i < numsSize; i++) {
        int c = ++cnt[nums[i]];
        if (c > max_cnt) {
            ans = c;
            max_cnt = c;
        } else if (c == max_cnt) {
            ans += c;
        }
    }

    free(cnt);
    return ans;
}
```
```js
var maxFrequencyElements = function(nums) {
    const cnt = new Map();
    let ans = 0, maxCnt = 0;
    for (const x of nums) {
        const c = (cnt.get(x) ?? 0) + 1;
        cnt.set(x, c);
        if (c > maxCnt) {
            ans = maxCnt = c;
        } else if (c === maxCnt) {
            ans += c;
        }
    }
    return ans;
};
```
复杂度分析：
- 时间复杂度：$O(n)$ ，其中 $n$ 是 $nums$ 的长度。
- 空间复杂度：$O(n)$ 。