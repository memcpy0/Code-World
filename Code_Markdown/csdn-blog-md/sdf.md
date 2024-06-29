# 
请你给一个停车场设计一个停车系统。停车场总共有三种不同大小的车位：大，中和小，每种尺寸分别有固定数目的车位。 
<p>请你实现&nbsp;<code>ParkingSystem</code>&nbsp;类：</p>

<ul>
	<li><code>ParkingSystem(int big, int medium, int small)</code>&nbsp;初始化&nbsp;<code>ParkingSystem</code>&nbsp;类，三个参数分别对应每种停车位的数目。</li>
	<li><code>bool addCar(int carType)</code>&nbsp;检车是否有&nbsp;<code>carType</code>&nbsp;对应的停车位。&nbsp;<code>carType</code>&nbsp;有三种类型：大，中，小，分别用数字&nbsp;<code>1</code>，&nbsp;<code>2</code>&nbsp;和&nbsp;<code>3</code>&nbsp;表示。<strong>一辆车只能停在</strong>&nbsp;<strong>&nbsp;</strong><code>carType</code>&nbsp;对应尺寸的停车位中。如果没有空车位，请返回&nbsp;<code>false</code>&nbsp;，否则将该车停入车位并返回&nbsp;<code>true</code>&nbsp;。</li>
</ul> 

<p><strong>示例 1：</strong></p>

```swift
输入：
["ParkingSystem", "addCar", "addCar", "addCar", "addCar"]
[[1, 1, 0], [1], [2], [3], [1]]
输出： 
[null, true, true, false, false]

解释：
ParkingSystem parkingSystem = new ParkingSystem(1, 1, 0);
parkingSystem.addCar(1); // 返回 true ，因为有 1 个空的大车位
parkingSystem.addCar(2); // 返回 true ，因为有 1 个空的中车位
parkingSystem.addCar(3); // 返回 false ，因为没有空的小车位
parkingSystem.addCar(1); // 返回 false ，因为没有空的大车位，唯一一个大车位已经被占据了
```
 
<p><strong>提示：</strong></p> 
<ul>
	<li><code>0 &lt;= big, medium, small &lt;= 1000</code></li>
	<li><code>carType</code>&nbsp;取值为&nbsp;<code>1</code>，&nbsp;<code>2</code>&nbsp;或&nbsp;<code>3</code></li>
	<li>最多会调用&nbsp;<code>addCar</code>&nbsp;函数&nbsp;<code>1000</code>&nbsp;次</li>
</ul>

代码如下：
```cpp
class ParkingSystem {
public:
    int B, M, S;
    ParkingSystem(int big, int medium, int small) {
        B = big;
        M = medium;
        S = small;
    }
    
    bool addCar(int carType) {
        if (carType == 1 && B > 0) {
            --B;
            return true;
        } else if (carType == 2 && M > 0) {
            --M;
            return true;
        } else if (carType == 3 && S > 0) {
            --S;
            return true;
        }
        return false;
    }
};

/**
 * Your ParkingSystem object will be instantiated and called as such:
 * ParkingSystem* obj = new ParkingSystem(big, medium, small);
 * bool param_1 = obj->addCar(carType);
 */
```

---
# 警告一小时内使用相同员工卡大于等于三次的人

              <p>力扣公司的员工都使用员工卡来开办公室的门。每当一个员工使用一次他的员工卡，安保系统会记录下员工的名字和使用时间。如果一个员工在一小时时间内使用员工卡的次数大于等于三次，这个系统会自动发布一个 <strong>警告</strong>&nbsp;。</p>

<p>给你字符串数组&nbsp;<code>keyName</code>&nbsp;和&nbsp;<code>keyTime</code> ，期中&nbsp;<code>[keyName[i], keyTime[i]]</code>&nbsp;对应一个人的名字和他在&nbsp;<strong>某一天</strong> 内使用员工卡的时间。</p>

<p>使用时间的格式是 <strong>24小时制</strong>&nbsp;，形如<strong>&nbsp;"HH:MM"</strong>&nbsp;，比方说&nbsp;<code>"23:51"</code> 和&nbsp;<code>"09:49"</code>&nbsp;。</p>

<p>请你返回去重后的收到系统警告的员工名字，将它们按 <strong>字典序</strong><strong>升序&nbsp;</strong>排序后返回。</p>

<p>请注意&nbsp;<code>"10:00"</code> - <code>"11:00"</code>&nbsp;视为一个小时时间范围内，而&nbsp;<code>"23:51"</code> - <code>"00:10"</code>&nbsp;不被视为一小时内，因为系统记录的是某一天内的使用情况。</p>

<p>&nbsp;</p>

<p><strong>示例 1：</strong></p>

<pre><strong>输入：</strong>keyName = ["daniel","daniel","daniel","luis","luis","luis","luis"], keyTime = ["10:00","10:40","11:00","09:00","11:00","13:00","15:00"]
<strong>输出：</strong>["daniel"]
<strong>解释：</strong>"daniel" 在一小时内使用了 3 次员工卡（"10:00"，"10:40"，"11:00"）。
</pre>

<p><strong>示例 2：</strong></p>

<pre><strong>输入：</strong>keyName = ["alice","alice","alice","bob","bob","bob","bob"], keyTime = ["12:01","12:00","18:00","21:00","21:20","21:30","23:00"]
<strong>输出：</strong>["bob"]
<strong>解释：</strong>"bob" 在一小时内使用了 3 次员工卡（"21:00"，"21:20"，"21:30"）。
</pre>

<p><strong>示例 3：</strong></p>

<pre><strong>输入：</strong>keyName = ["john","john","john"], keyTime = ["23:58","23:59","00:01"]
<strong>输出：</strong>[]
</pre>

<p><strong>示例 4：</strong></p>

<pre><strong>输入：</strong>keyName = ["leslie","leslie","leslie","clare","clare","clare","clare"], keyTime = ["13:00","13:20","14:00","18:00","18:51","19:30","19:49"]
<strong>输出：</strong>["clare","leslie"]
</pre>

<p>&nbsp;</p>

<p><strong>提示：</strong></p>

<ul>
	<li><code>1 &lt;= keyName.length, keyTime.length &lt;= 10<sup>5</sup></code></li>
	<li><code>keyName.length == keyTime.length</code></li>
	<li><code>keyTime</code> 格式为&nbsp;<strong>"HH:MM"&nbsp;</strong>。</li>
	<li>保证&nbsp;<code>[keyName[i], keyTime[i]]</code>&nbsp;形成的二元对&nbsp;<strong>互不相同&nbsp;</strong>。</li>
	<li><code>1 &lt;= keyName[i].length &lt;= 10</code></li>
	<li><code>keyName[i]</code>&nbsp;只包含小写英文字母。</li>
</ul>

class Solution {
private:
    int convertKeyTimeToMinutes(const string &t) {
        int hours = (t[0] - '0') * 10 + (t[1] - '0');
        int minutes = (t[3] - '0') * 10 + (t[4] - '0');
        return hours * 60 + minutes;
    }
public:
    vector<string> alertNames(vector<string>& keyName, vector<string>& keyTime) {
        set<string> names;
        unordered_map<string, vector<int>> record;
        int n = keyName.size();
        for (int i = 0; i < n; ++i) {
            if (record.find(keyName[i]) == record.end()) {
                record[keyName[i]] = make_pair(convertKeyTimeToMinutes(keyTime[i]), 1);   
            }
            else {
                int temp = convertKeyTimeToMinutes(keyTime[i]), pre = record[keyName[i]].first;
                if (abs(temp - pre) > 60) { //为不同的某一天 或者 两者的值
                    record[keyName[i]] = make_pair(temp, 1); 
                    continue;
                }
                record[keyName[i]].first = temp;
                ++record[keyName[i]].second;
                if (record[keyName[i]].second >= 3) names.insert(keyName[i]);
            }
        } 
        return vector<string>(names.begin(), names.end());
    }
};


 ---
# 给定行和列的和求可行矩阵
<p>给你两个非负整数数组&nbsp;<code>rowSum</code> 和&nbsp;<code>colSum</code>&nbsp;，其中&nbsp;<code>rowSum[i]</code>&nbsp;是二维矩阵中第 <code>i</code>&nbsp;行元素的和， <code>colSum[j]</code>&nbsp;是第 <code>j</code>&nbsp;列元素的和。换言之你不知道矩阵里的每个元素，但是你知道每一行和每一列的和。</p>

<p>请找到大小为&nbsp;<code>rowSum.length x colSum.length</code>&nbsp;的任意 <strong>非负整数</strong>&nbsp;矩阵，且该矩阵满足&nbsp;<code>rowSum</code> 和&nbsp;<code>colSum</code>&nbsp;的要求。</p>

<p>请你返回任意一个满足题目要求的二维矩阵，题目保证存在 <strong>至少一个</strong>&nbsp;可行矩阵。</p>

 

<p><strong>示例 1：</strong></p>

<pre><strong>输入：</strong>rowSum = [3,8], colSum = [4,7]
<strong>输出：</strong>[[3,0],
      [1,7]]
<strong>解释：</strong>
第 0 行：3 + 0 = 0 == rowSum[0]
第 1 行：1 + 7 = 8 == rowSum[1]
第 0 列：3 + 1 = 4 == colSum[0]
第 1 列：0 + 7 = 7 == colSum[1]
行和列的和都满足题目要求，且所有矩阵元素都是非负的。
另一个可行的矩阵为：[[1,2],
                  [3,5]]
</pre>

<p><strong>示例 2：</strong></p>

<pre><strong>输入：</strong>rowSum = [5,7,10], colSum = [8,6,8]
<strong>输出：</strong>[[0,5,0],
      [6,1,0],
      [2,0,8]]
</pre>

<p><strong>示例 3：</strong></p>

<pre><strong>输入：</strong>rowSum = [14,9], colSum = [6,9,8]
<strong>输出：</strong>[[0,9,5],
      [6,0,3]]
</pre>

<p><strong>示例 4：</strong></p>

<pre><strong>输入：</strong>rowSum = [1,0], colSum = [1]
<strong>输出：</strong>[[1],
      [0]]
</pre>

<p><strong>示例 5：</strong></p>

<pre><strong>输入：</strong>rowSum = [0], colSum = [0]
<strong>输出：</strong>[[0]]
</pre>
 
<p><strong>提示：</strong></p>

<ul>
	<li><code>1 &lt;= rowSum.length, colSum.length &lt;= 500</code></li>
	<li><code>0 &lt;= rowSum[i], colSum[i] &lt;= 10<sup>8</sup></code></li>
	<li><code>sum(rows) == sum(columns)</code></li>
</ul>

            
 
