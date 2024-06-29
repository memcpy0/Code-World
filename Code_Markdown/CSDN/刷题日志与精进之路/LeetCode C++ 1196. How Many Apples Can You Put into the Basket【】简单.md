You have some apples, where `arr[i]` is the weight of the `i`-th apple.  You also have a basket that can carry up to `5000` units of weight.

Return the maximum number of apples you can put in the basket.

**Example 1:**

```haskell
Input: arr = [100,200,150,1000]
Output: 4
Explanation: All 4 apples can be carried by the basket since their sum of weights is 1450.
```

**Example 2:**

```haskell
Input: arr = [900,950,800,1000,700,800]
Output: 5
Explanation: The sum of weights of the 6 apples exceeds 5000 so we choose any 5 of them.
```

**Constraints:**
- `1 <= arr.length <= 10^3`
- `1 <= arr[i] <= 10^3`

题意：水果店正在促销，你打算买些苹果，`arr[i]` 表示第 `i` 个苹果的单位重量。你有一个购物袋，最多可以装 `5000` 单位重量的东西，算一算，最多可以往购物袋里装入多少苹果。
 
