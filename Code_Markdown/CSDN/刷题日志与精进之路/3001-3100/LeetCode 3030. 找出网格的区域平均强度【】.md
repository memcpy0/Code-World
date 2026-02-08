> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

给你一个下标从 **0** 开始、大小为 `m x n` 的网格 `image` ，表示一个灰度图像，其中 `image[i][j]` 表示在范围 `[0..255]` 内的某个像素强度。另给你一个 **非负** 整数 `threshold` 。

如果 `image[a][b]` 和 `image[c][d]` 满足 `|a - c| + |b - d| == 1` ，则称这两个像素是 **相邻像素** 。

**区域** 是一个 `3 x 3` 的子网格，且满足区域中任意两个 **相邻** 像素之间，像素强度的 **绝对差** **小于或等于** `threshold` 。

**区域** 内的所有像素都认为属于该区域，而一个像素 **可以** 属于 **多个** 区域。

你需要计算一个下标从 **0** 开始、大小为 `m x n` 的网格 `result` ，其中 `result[i][j]` 是 `image[i][j]` 所属区域的 **平均** 强度，**向下取整** 到最接近的整数。如果 `image[i][j]` 属于多个区域，`result[i][j]` 是这些区域的 **“取整后的平均强度”** 的 **平均值**，也 **向下取整** 到最接近的整数。如果 `image[i][j]` 不属于任何区域，则 `result[i][j]` **等于** `image[i][j]` 。

返回网格 `result` 。

**示例 1：**

![](https://assets.leetcode.com/uploads/2023/12/21/example0corrected.png)

**输入：**image = [[5,6,7,10],[8,9,10,10],[11,12,13,10]], threshold = 3
**输出：**[[9,9,9,9],[9,9,9,9],[9,9,9,9]]
**解释：**图像中存在两个区域，如图片中的阴影区域所示。第一个区域的平均强度为 9 ，而第二个区域的平均强度为 9.67 ，向下取整为 9 。两个区域的平均强度为 (9 + 9) / 2 = 9 。由于所有像素都属于区域 1 、区域 2 或两者，因此 result 中每个像素的强度都为 9 。
注意，在计算多个区域的平均值时使用了向下取整的值，因此使用区域 2 的平均强度 9 来进行计算，而不是 9.67 。

**示例 2：**

![](https://assets.leetcode.com/uploads/2023/12/21/example1corrected.png)

**输入：**image = [[10,20,30],[15,25,35],[20,30,40],[25,35,45]], threshold = 12
**输出：**[[25,25,25],[27,27,27],[27,27,27],[30,30,30]]
**解释：**图像中存在两个区域，如图片中的阴影区域所示。第一个区域的平均强度为 25 ，而第二个区域的平均强度为 30 。两个区域的平均强度为 (25 + 30) / 2 = 27.5 ，向下取整为 27 。图像中第 0 行的所有像素属于区域 1 ，因此 result 中第 0 行的所有像素为 25 。同理，result 中第 3 行的所有像素为 30 。图像中第 1 行和第 2 行的像素属于区域 1 和区域 2 ，因此它们在 result 中的值为 27 。

**示例 3：**

**输入：**image = [[5,6,7],[8,9,10],[11,12,13]], threshold = 1
**输出：**[[5,6,7],[8,9,10],[11,12,13]]
**解释：**图像中不存在任何区域，因此对于所有像素，result[i][j] == image[i][j] 。

**提示：**

- `3 <= n, m <= 500`
- `0 <= image[i][j] <= 255`
- `0 <= threshold <= 255`

### 阅读理解（Python/Java/C++/Go）
遍历所有 3×33 \times 33×3 的子网格。
遍历网格内的所有左右相邻格子和上下相邻格子，如果存在差值超过 threshold\textit{threshold}threshold 的情况，则枚举下一个子网格。
如果合法，计算子网格的平均值 avg\textit{avg}avg，等于子网格的元素和除以 999 下取整。
更新子网格内的 result[i][j]\textit{result}[i][j]result[i][j]，由于需要计算平均值，我们先把 avg\textit{avg}avg 加到 result[i][j]\textit{result}[i][j]result[i][j] 中，同时用一个 cnt\textit{cnt}cnt 矩阵统计 (i,j)(i,j)(i,j) 在多少个合法子网格内。
最后返回答案。如果 cnt[i][j]=0\textit{cnt}[i][j]=0cnt[i][j]=0 则 result[i][j]=image[i][j]\textit{result}[i][j] = \textit{image}[i][j]result[i][j]=image[i][j]，否则 result[i][j]=⌊result[i][j]cnt[i][j]⌋\textit{result}[i][j] = \left\lfloor\dfrac{\textit{result}[i][j]}{\textit{cnt}[i][j]}\right\rfloorresult[i][j]=⌊ 
cnt[i][j]
result[i][j]
​
 ⌋。

```cpp
class Solution:
    def resultGrid(self, a: List[List[int]], threshold: int) -> List[List[int]]:
        m, n = len(a), len(a[0])
        result = [[0] * n for _ in range(m)]
        cnt = [[0] * n for _ in range(m)]
        for i in range(2, m):
            for j in range(2, n):
                # 检查左右相邻格子
                ok = True
                for row in a[i - 2: i + 1]:
                    if abs(row[j - 2] - row[j - 1]) > threshold or abs(row[j - 1] - row[j]) > threshold:
                        ok = False
                        break  # 不合法，下一个
                if not ok: continue

                # 检查上下相邻格子
                for y in range(j - 2, j + 1):
                    if abs(a[i - 2][y] - a[i - 1][y]) > threshold or abs(a[i - 1][y] - a[i][y]) > threshold:
                        ok = False
                        break  # 不合法，下一个
                if not ok: continue

                # 合法，计算 3x3 子网格的平均值
                avg = sum(a[x][y] for x in range(i - 2, i + 1) for y in range(j - 2, j + 1)) // 9

                # 更新 3x3 子网格内的 result
                for x in range(i - 2, i + 1):
                    for y in range(j - 2, j + 1):
                        result[x][y] += avg  # 先累加，最后再求平均值
                        cnt[x][y] += 1

        for i, row in enumerate(cnt):
            for j, c in enumerate(row):
                if c == 0:  # (i,j) 不属于任何子网格
                    result[i][j] = a[i][j]
                else:
                    result[i][j] //= c  # 求平均值
        return result

作者：灵茶山艾府
链接：https://leetcode.cn/problems/find-the-grid-of-region-average/solutions/2630945/yue-du-li-jie-pythonjavacgo-by-endlessch-pcnd/
来源：力扣（LeetCode）
著作权归作者所有。商业转载请联系作者获得授权，非商业转载请注明出处。

class Solution {
public:
    vector<vector<int>> resultGrid(vector<vector<int>> &a, int threshold) {
        int m = a.size(), n = a[0].size();
        vector<vector<int>> result(m, vector<int>(n));
        vector<vector<int>> cnt(m, vector<int>(n));
        for (int i = 2; i < m; i++) {
            for (int j = 2; j < n; j++) {
                // 检查左右相邻格子
                bool ok = true;
                for (int x = i - 2; x <= i; x++) {
                    if (abs(a[x][j - 2] - a[x][j - 1]) > threshold || abs(a[x][j - 1] - a[x][j]) > threshold) {
                        ok = false;
                        break; // 不合法，下一个
                    }
                }
                if (!ok) continue;

                // 检查上下相邻格子
                for (int y = j - 2; y <= j; y++) {
                    if (abs(a[i - 2][y] - a[i - 1][y]) > threshold || abs(a[i - 1][y] - a[i][y]) > threshold) {
                        ok = false;
                        break; // 不合法，下一个
                    }
                }
                if (!ok) continue;

                // 合法，计算 3x3 子网格的平均值
                int avg = 0;
                for (int x = i - 2; x <= i; x++) {
                    for (int y = j - 2; y <= j; y++) {
                        avg += a[x][y];
                    }
                }
                avg /= 9;

                // 更新 3x3 子网格内的 result
                for (int x = i - 2; x <= i; x++) {
                    for (int y = j - 2; y <= j; y++) {
                        result[x][y] += avg; // 先累加，最后再求平均值
                        cnt[x][y]++;
                    }
                }
            }
        }

        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                if (cnt[i][j] == 0) { // (i,j) 不属于任何子网格
                    result[i][j] = a[i][j];
                } else {
                    result[i][j] /= cnt[i][j]; // 求平均值
                }
            }
        }
        return result;
    }
};

作者：灵茶山艾府
链接：https://leetcode.cn/problems/find-the-grid-of-region-average/solutions/2630945/yue-du-li-jie-pythonjavacgo-by-endlessch-pcnd/
来源：力扣（LeetCode）
著作权归作者所有。商业转载请联系作者获得授权，非商业转载请注明出处。

func resultGrid(a [][]int, threshold int) [][]int {
	m, n := len(a), len(a[0])
	result := make([][]int, m)
	cnt := make([][]int, m)
	for i := range result {
		result[i] = make([]int, n)
		cnt[i] = make([]int, n)
	}
	for i := 2; i < m; i++ {
	next:
		for j := 2; j < n; j++ {
			// 检查左右相邻格子
			for _, row := range a[i-2 : i+1] {
				if abs(row[j-2]-row[j-1]) > threshold || abs(row[j-1]-row[j]) > threshold {
					continue next // 不合法，下一个
				}
			}

			// 检查上下相邻格子
			for y := j - 2; y <= j; y++ {
				if abs(a[i-2][y]-a[i-1][y]) > threshold || abs(a[i-1][y]-a[i][y]) > threshold {
					continue next // 不合法，下一个
				}
			}

			// 合法，计算 3x3 子网格的平均值
			avg := 0
			for x := i - 2; x <= i; x++ {
				for y := j - 2; y <= j; y++ {
					avg += a[x][y]
				}
			}
			avg /= 9

			// 更新 3x3 子网格内的 result
			for x := i - 2; x <= i; x++ {
				for y := j - 2; y <= j; y++ {
					result[x][y] += avg // 先累加，最后再求平均值
					cnt[x][y]++
				}
			}
		}
	}

	for i, row := range cnt {
		for j, c := range row {
			if c == 0 { // (i,j) 不属于任何子网格
				result[i][j] = a[i][j]
			} else {
				result[i][j] /= c // 求平均值
			}
		}
	}
	return result
}

func abs(x int) int { if x < 0 { return -x }; return x }

作者：灵茶山艾府
链接：https://leetcode.cn/problems/find-the-grid-of-region-average/solutions/2630945/yue-du-li-jie-pythonjavacgo-by-endlessch-pcnd/
来源：力扣（LeetCode）
著作权归作者所有。商业转载请联系作者获得授权，非商业转载请注明出处。

复杂度分析
时间复杂度：O(Amn)\mathcal{O}(Amn)O(Amn)，其中 mmm 和 nnn 分别为 a\textit{a}a 的行数和列数，A=9A=9A=9 表示子网格大小。
空间复杂度：O(mn)\mathcal{O}(mn)O(mn)。
思考题
如果额外输入两个数 www 和 hhh，把题目中 3×33\times 33×3 改成 w×hw\times hw×h，要怎么做？你能做到 O(mn)\mathcal{O}(mn)O(mn) 的时间复杂度吗？

欢迎在评论区分享你的思路/代码。

相关题目：2132. 用邮票贴满网格图，我的题解，包含了解决思考题需要掌握的算法。

作者：灵茶山艾府
链接：https://leetcode.cn/problems/find-the-grid-of-region-average/solutions/2630945/yue-du-li-jie-pythonjavacgo-by-endlessch-pcnd/
来源：力扣（LeetCode）
著作权归作者所有。商业转载请联系作者获得授权，非商业转载请注明出处。
```