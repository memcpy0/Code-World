> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

设计一个支持下述操作的食物评分系统：
- **修改** 系统中列出的某种食物的评分。
- 返回系统中某一类烹饪方式下评分最高的食物。

实现 `FoodRatings` 类：
- `FoodRatings(String[] foods, String[] cuisines, int[] ratings)` 初始化系统。食物由 `foods`、`cuisines` 和 `ratings` 描述，长度均为 `n` 。
    - `foods[i]` 是第 `i` 种食物的名字。
    - `cuisines[i]` 是第 `i` 种食物的烹饪方式。
    - `ratings[i]` 是第 `i` 种食物的最初评分。
- `void changeRating(String food, int newRating)` 修改名字为 `food` 的食物的评分。
- `String highestRated(String cuisine)` 返回指定烹饪方式 `cuisine` 下评分最高的食物的名字。如果存在并列，返回 **字典序较小** 的名字。

注意，字符串 `x` 的字典序比字符串 `y` 更小的前提是：`x` 在字典中出现的位置在 `y` 之前，也就是说，要么 `x` 是 `y` 的前缀，或者在满足 `x[i] != y[i]` 的第一个位置 `i` 处，`x[i]` 在字母表中出现的位置在 `y[i]` 之前。

**示例：**
**输入**
```java
["FoodRatings", "highestRated", "highestRated", "changeRating", "highestRated", "changeRating", "highestRated"]
[[["kimchi", "miso", "sushi", "moussaka", "ramen", "bulgogi"], ["korean", "japanese", "japanese", "greek", "japanese", "korean"], [9, 12, 8, 15, 14, 7]], ["korean"], ["japanese"], ["sushi", 16], ["japanese"], ["ramen", 16], ["japanese"]]
```
**输出**
```java
[null, "kimchi", "ramen", null, "sushi", null, "ramen"]
```
**解释**
```java
FoodRatings foodRatings = new FoodRatings(["kimchi", "miso", "sushi", "moussaka", "ramen", "bulgogi"], ["korean", "japanese", "japanese", "greek", "japanese", "korean"], [9, 12, 8, 15, 14, 7]);
foodRatings.highestRated("korean"); // 返回 "kimchi"
                                    // "kimchi" 是分数最高的韩式料理，评分为 9 。
foodRatings.highestRated("japanese"); // 返回 "ramen"
                                      // "ramen" 是分数最高的日式料理，评分为 14 。
foodRatings.changeRating("sushi", 16); // "sushi" 现在评分变更为 16 。
foodRatings.highestRated("japanese"); // 返回 "sushi"
                                      // "sushi" 是分数最高的日式料理，评分为 16 。
foodRatings.changeRating("ramen", 16); // "ramen" 现在评分变更为 16 。
foodRatings.highestRated("japanese"); // 返回 "ramen"
                                      // "sushi" 和 "ramen" 的评分都是 16 。
                                      // 但是，"ramen" 的字典序比 "sushi" 更小。
```
**提示：**
- `1 <= n <= 2 * 10^4`
- `n == foods.length == cuisines.length == ratings.length`
- `1 <= foods[i].length, cuisines[i].length <= 10`
- `foods[i]`、`cuisines[i]` 由小写英文字母组成
- `1 <= ratings[i] <= 108`
- `foods` 中的所有字符串 **互不相同**
- 在对 `changeRating` 的所有调用中，`food` 是系统中食物的名字。
- 在对 `highestRated` 的所有调用中，`cuisine` 是系统中 **至少一种** 食物的烹饪方式。
- 最多调用 `changeRating` 和 `highestRated` **总计** `2 * 10^4` 次

---
### 解法1 平衡树
我们可以用一个**哈希表** $fs$ 记录每个食物名称、对应的食物评分和烹饪方式，另一个**哈希表套平衡树** $cs$ 记录每个烹饪方式、对应的食物评分和食物名称集合。

对于 `changeRating` 操作，先从 `cs[fs[food].cuisine]` 中删除旧数据，然后将 `newRating` 和 `food` 记录到 $cs$ 和 $fs$ 中：
```cpp
// cpp
class FoodRatings {
private:
    unordered_map<string, pair<int, string>> fs;
    unordered_map<string, set<pair<int, string>>> cs;
public:
    FoodRatings(vector<string>& foods, vector<string>& cuisines, vector<int>& ratings) {
        for (int i = 0, n = foods.size(); i < n; ++i) {
            auto &f = foods[i], &c = cuisines[i];
            int r = ratings[i];
            fs[f] = {r, c};
            cs[c].emplace(-r, f);
        }
    }
    
    void changeRating(string food, int newRating) {
        auto &[r, c] = fs[food];
        auto &s = cs[c];
        s.erase({-r, food}); // 移除旧数据
        s.emplace(-newRating, food); // 添加新数据
        r = newRating;
    }
    
    string highestRated(string cuisine) {
        return cs[cuisine].begin()->second;
    }
};
// java
class FoodRatings {
    Map<String, Pair<Integer, String>> fs = new HashMap<>();
    Map<String, TreeSet<Pair<Integer, String>>> cs = new HashMap<>();

    public FoodRatings(String[] foods, String[] cuisines, int[] ratings) {
        for (var i = 0; i < foods.length; ++i) {
            String f = foods[i], c = cuisines[i];
            var r = ratings[i];
            fs.put(f, new Pair<>(r, c));
            cs.computeIfAbsent(c, k -> new TreeSet<>((a, b) -> 
                !Objects.equals(a.getKey(), b.getKey()) ? 
                    b.getKey() - a.getKey() : // 逆序 
                    a.getValue().compareTo(b.getValue())
                )).add(new Pair<>(r, f));
        }
    }
    
    public void changeRating(String food, int newRating) {
        var e = fs.get(food);
        var s = cs.get(e.getValue());
        s.remove(new Pair<>(e.getKey(), food)); // 移除旧数据
        s.add(new Pair<>(newRating, food)); // 添加新数据
        fs.put(food, new Pair<>(newRating, e.getValue()));
    }
    
    public String highestRated(String cuisine) {
        return cs.get(cuisine).first().getValue();
    }
}
// python
from sortedcontainers import SortedSet
class FoodRatings:
    def __init__(self, foods: List[str], cuisines: List[str], ratings: List[int]):
        self.fs = {}
        self.cs = defaultdict(SortedSet)
        for f, c, r in zip(foods, cuisines, ratings):
            self.fs[f] = [r, c]
            self.cs[c].add((-r, f))

    def changeRating(self, food: str, newRating: int) -> None:
        r, c = self.fs[food]
        s = self.cs[c]
        s.remove((-r, food)) # 移除旧数据
        s.add((-newRating, food)) # 添加新数据
        self.fs[food][0] = newRating

    def highestRated(self, cuisine: str) -> str:
        return self.cs[cuisine][0][1]
// go

```
---
### 解法2 懒删除堆
另一种做法是用堆：
- 对于 `changeRating` 操作，直接往 $cs$ 中记录，不做任何删除操作；
- 对于 $highestRated$ 操作，==查看堆顶的食物评分是否等于其实际值，若不相同则意味着对应的元素已被替换成了其他值==，堆顶存的是个垃圾数据，直接弹出堆顶；否则堆顶就是答案。

```cpp
// cpp
class FoodRatings {
private:
    unordered_map<string, pair<int, string>> fs;
    unordered_map<string, 
        priority_queue<pair<int, string>, vector<pair<int, string>>, greater<>>> cs;
public:
    FoodRatings(vector<string>& foods, vector<string>& cuisines, vector<int>& ratings) {
        for (int i = 0, n = foods.size(); i < n; ++i) {
            auto &f = foods[i], &c = cuisines[i];
            int r = ratings[i];
            fs[f] = {r, c};
            cs[c].emplace(-r, f);
        }
    }
    
    void changeRating(string food, int newRating) {
        auto &[r, c] = fs[food];
        cs[c].emplace(-newRating, food); // 直接添加新数据，后面 highestRated 再删除旧的
        r = newRating;
    }
    
    string highestRated(string cuisine) {
        auto &q = cs[cuisine];
        while (-q.top().first != fs[q.top().second].first) // 堆顶的食物评分不等于其实际值
            q.pop();
        return q.top().second;
    }
};
// java
class FoodRatings {
    Map<String, Pair<Integer, String>> fs = new HashMap<>();
    Map<String, Queue<Pair<Integer, String>>> cs = new HashMap<>();

    public FoodRatings(String[] foods, String[] cuisines, int[] ratings) {
        for (var i = 0; i < foods.length; ++i) {
            String f = foods[i], c = cuisines[i];
            var r = ratings[i];
            fs.put(f, new Pair<>(r, c));
            cs.computeIfAbsent(c, k -> new PriorityQueue<>((a, b) -> 
                !Objects.equals(a.getKey(), b.getKey()) ? 
                    b.getKey() - a.getKey() : // 逆序 
                    a.getValue().compareTo(b.getValue())
                )).add(new Pair<>(r, f));
        }
    }
    
    public void changeRating(String food, int newRating) {
        var c = fs.get(food).getValue();
        cs.get(c).offer(new Pair<>(newRating, food)); // 直接添加新数据，后面 highestRated 再删除旧的
        fs.put(food, new Pair<>(newRating, c)); // 记录新评分
    }
    
    public String highestRated(String cuisine) {
        var q = cs.get(cuisine);
        while (!Objects.equals(q.peek().getKey(), fs.get(q.peek().getValue()).getKey()))
            q.poll();
        return q.peek().getValue();
    }
}
// python
class FoodRatings:
    def __init__(self, foods: List[str], cuisines: List[str], ratings: List[int]):
        self.fs = {}
        self.cs = defaultdict(list)
        for f, c, r in zip(foods, cuisines, ratings):
            self.fs[f] = [r, c]
            heappush(self.cs[c], (-r, f))

    def changeRating(self, food: str, newRating: int) -> None:
        f = self.fs[food]
        heappush(self.cs[f[1]], (-newRating, food)) # 直接添加新数据,后面highestRated再删除旧的
        f[0] = newRating

    def highestRated(self, cuisine: str) -> str:
        h = self.cs[cuisine]
        while -h[0][0] != self.fs[h[0][1]][0]: # 堆顶的食物评分!=实际值
            heappop(h)
        return h[0][1]
```