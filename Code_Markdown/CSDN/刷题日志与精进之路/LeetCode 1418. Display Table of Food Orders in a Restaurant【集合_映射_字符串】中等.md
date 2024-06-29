<p>Given&nbsp;the array <code>orders</code>, which represents the orders that customers have done in a restaurant. More specifically&nbsp;<code>orders[i]=[customerName<sub>i</sub>,tableNumber<sub>i</sub>,foodItem<sub>i</sub>]</code> where <code>customerName<sub>i</sub></code> is the name of the customer, <code>tableNumber<sub>i</sub></code>&nbsp;is the table customer sit at, and <code>foodItem<sub>i</sub></code>&nbsp;is the item customer orders.</p>

<p><em>Return the restaurant's “<strong>display table</strong>”</em>. The “<strong>display table</strong>” is a table whose row entries denote how many of each food item each table ordered. The first column is the table number and the remaining columns correspond to each food item in alphabetical order. The first row should be a header whose first column is “Table”, followed by the names of the food items. Note that the customer names are not part of the table. Additionally, the rows should be sorted in numerically increasing order.</p>

 
<p><strong>Example 1:</strong></p>

```bash
Input: orders = [["David","3","Ceviche"],["Corina","10","Beef Burrito"],["David","3","Fried Chicken"],["Carla","5","Water"],["Carla","5","Ceviche"],["Rous","3","Ceviche"]]
Output: [["Table","Beef Burrito","Ceviche","Fried Chicken","Water"],["3","0","2","1","0"],["5","0","1","0","1"],["10","1","0","0","0"]] 
Explanation:
The displaying table looks like:
Table,Beef Burrito,Ceviche,Fried Chicken,Water
3    ,0           ,2      ,1            ,0
5    ,0           ,1      ,0            ,1
10   ,1           ,0      ,0            ,0
For the table 3: David orders "Ceviche" and "Fried Chicken", and Rous orders "Ceviche".
For the table 5: Carla orders "Water" and "Ceviche".
For the table 10: Corina orders "Beef Burrito". 
```
 
<p><strong>Example 2:</strong></p>

```bash
Input: orders = [["James","12","Fried Chicken"],["Ratesh","12","Fried Chicken"],["Amadeus","12","Fried Chicken"],["Adam","1","Canadian Waffles"],["Brianna","1","Canadian Waffles"]]
Output: [["Table","Canadian Waffles","Fried Chicken"],["1","2","0"],["12","0","3"]] 
Explanation: 
For the table 1: Adam and Brianna order "Canadian Waffles".
For the table 12: James, Ratesh and Amadeus order "Fried Chicken".
```

<p><strong>Example 3:</strong></p>

```bash
Input: orders = [["Laura","2","Bean Burrito"],["Jhon","2","Beef Burrito"],["Melissa","2","Soda"]]
Output: [["Table","Bean Burrito","Beef Burrito","Soda"],["2","1","1","1"]]  
```

 
<p><strong>Constraints:</strong></p>

<ul>
	<li><code>1 &lt;=&nbsp;orders.length &lt;= 5 * 10^4</code></li>
	<li><code>orders[i].length == 3</code></li>
	<li><code>1 &lt;= customerName<sub>i</sub>.length, foodItem<sub>i</sub>.length &lt;= 20</code></li>
	<li><code>customerName<sub>i</sub></code> and <code>foodItem<sub>i</sub></code> consist of lowercase and uppercase English letters and the space character.</li>
	<li><code>tableNumber<sub>i</sub>&nbsp;</code>is a valid integer between <code>1</code> and <code>500</code>.</li>
</ul>


题意：给你一个数组 <code>orders</code>，表示客户在餐厅中完成的订单，确切地说， <code>orders[i]=[customerName<sub>i</sub>,tableNumber<sub>i</sub>,foodItem<sub>i</sub>]</code> ，其中 <code>customerName<sub>i</sub></code> 是客户的姓名，<code>tableNumber<sub>i</sub></code> 是客户所在餐桌的桌号，而 <code>foodItem<sub>i</sub></code> 是客户点的餐品名称。 
<p>请你返回该餐厅的 <strong>点菜展示表</strong><em> 。</em>在这张表中，表中第一行为标题，其第一列为餐桌桌号 “Table” ，后面每一列都是按字母顺序排列的餐品名称。接下来每一行中的项则表示每张餐桌订购的相应餐品数量，第一列应当填对应的桌号，后面依次填写下单的餐品数量。客户姓名不是点菜展示表的一部分。此外，表中的数据行应该按餐桌桌号升序排列。 

---
### 解法 寻找合适的数据结构
本题最关键的地方在于用什么数据结构实现。我们注意到，结果表是要求出所有二元对 `(tableNumber, foodItem)` 出现的频数，有点类似于搜索引擎中的正排索引表，此外表中数据行应按照 `tableNumber` 升序排列，数据列应按照 `foodItem` 升序排列。于是可以使用双重哈希表记录 `(tableNumber, foodItem)` 的频数，然后构造结果表。

首先使用 `unordered_map<int, unordered_map<string, int>>` 双重无序哈希映射：
```cpp
class Solution {
public:
    vector<vector<string>> displayTable(vector<vector<string>>& orders) {
        vector<vector<string>> ans;
        set<int> tables;
        set<string> foods;
        unordered_map<int, unordered_map<string, int>> rec;
        for (const vector<string>& order : orders) {
            int tableNum = stoi(order[1]);
            tables.insert(tableNum); //桌号
            foods.insert(order[2]); //食品名称
            ++rec[tableNum][order[2]]; //tableNum:foodItemNumber
        }
        vector<string> firstRow;
        firstRow.push_back("Table");
        for (const string& food : foods) firstRow.push_back(food);
        ans.push_back(firstRow);
        for (const int& table : tables) {
            vector<string> row; //每一行
            row.push_back(to_string(table)); //餐桌名
            for (int i = 1, n = firstRow.size(); i < n; ++i) //每种餐品的数量
                row.push_back(to_string(rec[table][firstRow[i]])); 
            ans.push_back(row);
        }
        return ans;
    }
};
```
运行效率如下：
```cpp
执行用时：212 ms, 在所有 C++ 提交中击败了60.48% 的用户
内存消耗：95.6 MB, 在所有 C++ 提交中击败了21.93% 的用户
```
或者可以使用 `map<int, unordered_map<string, int>>` 有序映射嵌套无序哈希映射：
```cpp
class Solution {
public:
    vector<vector<string>> displayTable(vector<vector<string>>& orders) {
        vector<vector<string>> ans;
        set<string> foods;
        map<int, unordered_map<string, int>> rec;
        for (const vector<string>& order : orders) {
            foods.insert(order[2]); //食品名称
            ++rec[stoi(order[1])][order[2]]; //tableNumber:foodItemNumber
        }
        vector<string> firstRow;
        firstRow.push_back("Table");
        for (const string& food : foods) firstRow.push_back(food);
        ans.push_back(firstRow);
        for (auto p = rec.begin(); p != rec.end(); ++p) {
            vector<string> row; //每一行
            row.push_back(to_string(p->first)); //餐桌名
            for (int i = 1, n = firstRow.size(); i < n; ++i) //每种餐品的数量
                row.push_back(to_string(p->second[firstRow[i]])); 
            ans.push_back(row);
        }
        return ans;
    }
};
```
运行效率如下：
```cpp
执行用时：180 ms, 在所有 C++ 提交中击败了72.79% 的用户
内存消耗：95.6 MB, 在所有 C++ 提交中击败了20.58% 的用户
```
