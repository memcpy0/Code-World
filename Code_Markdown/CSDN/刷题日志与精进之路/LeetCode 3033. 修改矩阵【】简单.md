> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

给你一个下标从 **0** 开始、大小为 `m x n` 的整数矩阵 `matrix` ，新建一个下标从 **0** 开始、名为 `answer` 的矩阵。使 `answer` 与 `matrix` 相等，接着将其中每个值为 `-1` 的元素替换为所在列的 **最大** 元素。

返回矩阵 `answer` 。

**例 1：**

![](https://assets.leetcode.com/uploads/2023/12/24/matrix1.png)

**输入：**matrix = [[1,2,-1],[4,-1,6],[7,8,9]]
**输出：**[[1,2,9],[4,8,6],[7,8,9]]
**解释：**上图显示了发生替换的元素（蓝色区域）。
- 将单元格 [1][1] 中的值替换为列 1 中的最大值 8 。
- 将单元格 [0][2] 中的值替换为列 2 中的最大值 9 。

**示例 2：**

![](https://assets.leetcode.com/uploads/2023/12/24/matrix2.png)

**输入：**matrix = [[3,-1],[5,2]]
**输出：**[[3,2],[5,2]]
**解释：**上图显示了发生替换的元素（蓝色区域）。

**提示：**

- `m == matrix.length`
- `n == matrix[i].length`
- `2 <= m, n <= 50`
- `-1 <= matrix[i][j] <= 100`
- 测试用例中生成的输入满足每列至少包含一个非负整数。

---
遍历每一列：先计算出列的最大值 mx\textit{mx}mx，再更新列中的 −1-1−1 为 mx\textit{mx}mx。
```cpp
class Solution:
    def modifiedMatrix(self, matrix: List[List[int]]) -> List[List[int]]:
        for j in range(len(matrix[0])):
            mx = max(row[j] for row in matrix)
            for row in matrix:
                if row[j] == -1:
                    row[j] = mx
        return matrix

作者：灵茶山艾府
链接：https://leetcode.cn/problems/modify-the-matrix/solutions/2637727/jian-ji-xie-fa-pythonjavacgo-by-endlessc-7ak3/
来源：力扣（LeetCode）
著作权归作者所有。商业转载请联系作者获得授权，非商业转载请注明出处。
class Solution {
    public int[][] modifiedMatrix(int[][] matrix) {
        for (int j = 0; j < matrix[0].length; j++) {
            int mx = 0;
            for (int[] row : matrix) {
                mx = Math.max(mx, row[j]);
            }
            for (int[] row : matrix) {
                if (row[j] == -1) {
                    row[j] = mx;
                }
            }
        }
        return matrix;
    }
}

作者：灵茶山艾府
链接：https://leetcode.cn/problems/modify-the-matrix/solutions/2637727/jian-ji-xie-fa-pythonjavacgo-by-endlessc-7ak3/
来源：力扣（LeetCode）
著作权归作者所有。商业转载请联系作者获得授权，非商业转载请注明出处。
class Solution {
public:
    vector<vector<int>> modifiedMatrix(vector<vector<int>> &matrix) {
        for (int j = 0; j < matrix[0].size(); j++) {
            int mx = 0;
            for (auto &row: matrix) {
                mx = max(mx, row[j]);
            }
            for (auto &row: matrix) {
                if (row[j] == -1) {
                    row[j] = mx;
                }
            }
        }
        return matrix;
    }
};

作者：灵茶山艾府
链接：https://leetcode.cn/problems/modify-the-matrix/solutions/2637727/jian-ji-xie-fa-pythonjavacgo-by-endlessc-7ak3/
来源：力扣（LeetCode）
著作权归作者所有。商业转载请联系作者获得授权，非商业转载请注明出处。
func modifiedMatrix(matrix [][]int) [][]int {
	for j := range matrix[0] {
		mx := 0
		for _, row := range matrix {
			mx = max(mx, row[j])
		}
		for _, row := range matrix {
			if row[j] == -1 {
				row[j] = mx
			}
		}
	}
	return matrix
}

作者：灵茶山艾府
链接：https://leetcode.cn/problems/modify-the-matrix/solutions/2637727/jian-ji-xie-fa-pythonjavacgo-by-endlessc-7ak3/
来源：力扣（LeetCode）
著作权归作者所有。商业转载请联系作者获得授权，非商业转载请注明出处。
```