<p>You are given an array <code>items</code>, where each <code>items[i] = [type<sub>i</sub>, color<sub>i</sub>, name<sub>i</sub>]</code> describes the type, color, and name of the <code>i<sup>th</sup></code> item. You are also given a rule represented by two strings, <code>ruleKey</code> and <code>ruleValue</code>.</p>

<p>The <code>i<sup>th</sup></code> item is said to match the rule if <strong>one</strong> of the following is true:</p>

<ul>
	<li><code>ruleKey == "type"</code> and <code>ruleValue == type<sub>i</sub></code>.</li>
	<li><code>ruleKey == "color"</code> and <code>ruleValue == color<sub>i</sub></code>.</li>
	<li><code>ruleKey == "name"</code> and <code>ruleValue == name<sub>i</sub></code>.</li>
</ul>

<p>Return <em>the number of items that match the given rule</em>.</p>
 
<p><strong>Example 1:</strong></p>

```clike
Input: items = [["phone","blue","pixel"],["computer","silver","lenovo"],["phone","gold","iphone"]], ruleKey = "color", ruleValue = "silver"
Output: 1
Explanation: There is only one item matching the given rule, which is ["computer","silver","lenovo"].  
```

<p><strong>Example 2:</strong></p>

```clike
Input: items = [["phone","blue","pixel"],["computer","silver","phone"],["phone","gold","iphone"]], ruleKey = "type", ruleValue = "phone"
Output: 2
Explanation: There are only two items matching the given rule, which are ["phone","blue","pixel"] and ["phone","gold","iphone"]. Note that the item ["computer","silver","phone"] does not match.  
```

 
<p><strong>Constraints:</strong></p>

<ul>
	<li><code>1 &lt;= items.length &lt;= 10<sup>4</sup></code></li>
	<li><code>1 &lt;= type<sub>i</sub>.length, color<sub>i</sub>.length, name<sub>i</sub>.length, ruleValue.length &lt;= 10</code></li>
	<li><code>ruleKey</code> is equal to either <code>"type"</code>, <code>"color"</code>, or <code>"name"</code>.</li>
	<li>All strings consist only of lowercase letters.</li>
</ul>



题意：给你一个数组 `items` ，其中 `items[i] = [typei, colori, namei]` ，描述第 `i` 件物品的类型、颜色以及名称。另给你一条由两个字符串 `ruleKey` 和 `ruleValue` 表示的检索规则。如果第 `i` 件物品能满足下述条件之一，则认为该物品与给定的检索规则 **匹配** ： 
- `ruleKey == "type"` 且 `ruleValue == typei` 。
 - `ruleKey == "color"` 且 `ruleValue == colori` 。
 -  `ruleKey == "name"` 且 `ruleValue == namei` 。

统计并返回 **匹配检索规则的物品数量** 。

---
### 解法 直接遍历
```cpp
class Solution {
public:
    int countMatches(vector<vector<string>>& items, string ruleKey, string ruleValue) {
        int cnt = 0, i = (ruleKey == "type" ? 0 : (ruleKey == "color" ? 1 : 2));
        for (const vector<string>& item : items) if (item[i] == ruleValue) ++cnt;
        return cnt;
    }
};
```
运行效率如下：
```cpp
执行用时：88 ms, 在所有 C++ 提交中击败了60.86% 的用户
内存消耗：30.2 MB, 在所有 C++ 提交中击败了50.84% 的用户
```
使用STL的 `count_if` ：
```cpp
class Solution {
public:
    int countMatches(vector<vector<string>>& items, string ruleKey, string ruleValue) {
        int i = (ruleKey == "type" ? 0 : (ruleKey == "color" ? 1 : 2));
        return count_if(items.begin(), items.end(), [&](const vector<string>& item) { return item[i] == ruleValue; });  
    }
};
```
运行效率如下：
```cpp
执行用时：84 ms, 在所有 C++ 提交中击败了75.16% 的用户
内存消耗：30.2 MB, 在所有 C++ 提交中击败了39.01% 的用户
```
