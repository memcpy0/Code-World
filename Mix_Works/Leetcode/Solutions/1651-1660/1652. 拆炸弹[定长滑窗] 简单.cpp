/*
** Problem: https://leetcode.cn/problems/defuse-the-bomb/
** Article: https://memcpy0.blog.csdn.net/article/details/138468454
** Author: memcpy0
*/
```python
class Solution:
    def decrypt(self, code: List[int], k: int) -> List[int]:
        n = len(code)
        ans = [0] * n
        r = k + 1 if k > 0 else n
        k = abs(k)
        s = sum(code[r - k: r]) # ans[0], [r-k, r)
        for i in range(n):
            ans[i] = s
            s += code[r % n] - code[(r - k) % n] # +移入,-移出
            r += 1
        return ans
```

```java
class Solution {
    public int[] decrypt(int[] code, int k) {
        int n = code.length;
        int[] ans = new int[n];
        int r = k > 0 ? k + 1 : n;
        k = Math.abs(k);
        int s = 0;
        for (int i = r - k; i < r; ++i) s += code[i]; // ans[0]
        for (int i = 0; i < n; ++i) {
            ans[i] = s;
            s += code[r % n] - code[(r - k) % n];
            ++r;
        }
        return ans;
    }
}
```

```cpp
class Solution {
public:
    vector<int> decrypt(vector<int>& code, int k) {
        int n = code.size();
        vector<int> ans(n);
        int r = k > 0 ? k + 1 : n;
        k = abs(k);
        int s = reduce(code.begin() + r - k, code.begin() + r); // ans[0]
        for (int i = 0; i < n; ++i) {
            ans[i] = s;
            s += code[r % n] - code[(r - k) % n];
            ++r;
        }
        return ans;
    }
};
```