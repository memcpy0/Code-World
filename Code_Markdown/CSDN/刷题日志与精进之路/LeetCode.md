第 208 场周赛

<p>每当用户执行变更文件夹操作时，LeetCode 文件系统都会保存一条日志记录。</p>

<p>下面给出对变更操作的说明：</p>

<ul>
	<li><code>"../"</code> ：移动到当前文件夹的父文件夹。如果已经在主文件夹下，则 <strong>继续停留在当前文件夹</strong> 。</li>
	<li><code>"./"</code> ：继续停留在当前文件夹<strong>。</strong></li>
	<li><code>"x/"</code> ：移动到名为 <code>x</code> 的子文件夹中。题目数据 <strong>保证总是存在文件夹 <code>x</code></strong> 。</li>
</ul>

<p>给你一个字符串列表 <code>logs</code> ，其中 <code>logs[i]</code> 是用户在 <code>i<sup>th</sup></code> 步执行的操作。</p>

<p>文件系统启动时位于主文件夹，然后执行 <code>logs</code> 中的操作。</p>

<p>执行完所有变更文件夹操作后，请你找出 <strong>返回主文件夹所需的最小步数</strong> 。</p>

<p>&nbsp;</p>

<p><strong>示例 1：</strong></p>

<p><img alt="" src="https://assets.leetcode-cn.com/aliyun-lc-upload/uploads/2020/09/26/sample_11_1957.png" style="height: 151px; width: 775px;"></p>

<pre><strong>输入：</strong>logs = ["d1/","d2/","../","d21/","./"]
<strong>输出：</strong>2
<strong>解释：</strong>执行 "../" 操作变更文件夹 2 次，即可回到主文件夹
</pre>

<p><strong>示例 2：</strong></p>

<p><img alt="" src="https://assets.leetcode-cn.com/aliyun-lc-upload/uploads/2020/09/26/sample_22_1957.png" style="height: 270px; width: 600px;"></p>

<pre><strong>输入：</strong>logs = ["d1/","d2/","./","d3/","../","d31/"]
<strong>输出：</strong>3
</pre>

<p><strong>示例 3：</strong></p>

<pre><strong>输入：</strong>logs = ["d1/","../","../","../"]
<strong>输出：</strong>0
</pre>

<p>&nbsp;</p>

<p><strong>提示：</strong></p>

<ul>
	<li><code>1 &lt;= logs.length &lt;= 10<sup>3</sup></code></li>
	<li><code>2 &lt;= logs[i].length &lt;= 10</code></li>
	<li><code>logs[i]</code> 包含小写英文字母，数字，<code>'.'</code> 和 <code>'/'</code></li>
	<li><code>logs[i]</code> 符合语句中描述的格式</li>
	<li>文件夹名称由小写英文字母和数字组成</li>
</ul>

```cpp
class Solution {
public:
    int minOperations(vector<string>& logs) {
        int level = 0, len = logs.size();
        for (int i = 0; i < len; ++i) {
            if (logs[i] == "./") continue;
            if (s == "../") {
                if (level == 0) continue;
                --level;
            } 
            else ++level;
        }
        return level;
    }
};
```

---
  <p>你正在经营一座摩天轮，该摩天轮共有 <strong>4 个座舱</strong> ，每个座舱<strong> 最多可以容纳 4 位游客</strong> 。你可以 <strong>逆时针</strong>&nbsp;轮转座舱，但每次轮转都需要支付一定的运行成本 <code>runningCost</code> 。摩天轮每次轮转都恰好转动 1 / 4 周。</p>

<p>给你一个长度为 <code>n</code> 的数组 <code>customers</code> ， <code>customers[i]</code> 是在第 <code>i</code> 次轮转（下标从 0 开始）之前到达的新游客的数量。这也意味着你必须在新游客到来前轮转 <code>i</code> 次。每位游客在登上离地面最近的座舱前都会支付登舱成本 <code>boardingCost</code> ，一旦该座舱再次抵达地面，他们就会离开座舱结束游玩。</p>

<p>你可以随时停下摩天轮，即便是 <strong>在服务所有游客之前</strong> 。如果你决定停止运营摩天轮，为了保证所有游客安全着陆，<strong>将免费进行</strong><strong>所有后续轮转</strong>&nbsp;。注意，如果有超过 4 位游客在等摩天轮，那么只有 4 位游客可以登上摩天轮，其余的需要等待 <strong>下一次轮转</strong> 。</p>

<p>返回最大化利润所需执行的 <strong>最小轮转次数</strong> 。 如果不存在利润为正的方案，则返回 <code>-1</code> 。</p>

<p>&nbsp;</p>

<p><strong>示例 1：</strong></p>

<p><img alt="" src="https://assets.leetcode-cn.com/aliyun-lc-upload/uploads/2020/09/26/wheeldiagram12.png" style="height: 291px; width: 906px;"></p>

<pre><strong>输入：</strong>customers = [8,3], boardingCost = 5, runningCost = 6
<strong>输出：</strong>3
<strong>解释：</strong>座舱上标注的数字是该座舱的当前游客数。
1. 8 位游客抵达，4 位登舱，4 位等待下一舱，摩天轮轮转。当前利润为 4 * $5 - 1 * $6 = $14 。
2. 3 位游客抵达，4 位在等待的游客登舱，其他 3 位等待，摩天轮轮转。当前利润为 8 * $5 - 2 * $6 = $28 。
3. 最后 3 位游客登舱，摩天轮轮转。当前利润为 11 * $5 - 3 * $6 = $37 。
轮转 3 次得到最大利润，最大利润为 $37 。</pre>

<p><strong>示例 2：</strong></p>

<pre><strong>输入：</strong>customers = [10,9,6], boardingCost = 6, runningCost = 4
<strong>输出：</strong>7
<strong>解释：</strong>
1. 10 位游客抵达，4 位登舱，6 位等待下一舱，摩天轮轮转。当前利润为 4 * $6 - 1 * $4 = $20 。
2. 9 位游客抵达，4 位登舱，11 位等待（2 位是先前就在等待的，9 位新加入等待的），摩天轮轮转。当前利润为 8 * $6 - 2 * $4 = $40 。
3. 最后 6 位游客抵达，4 位登舱，13 位等待，摩天轮轮转。当前利润为 12 * $6 - 3 * $4 = $60 。
4. 4 位登舱，9 位等待，摩天轮轮转。当前利润为 * $6 - 4 * $4 = $80 。
5. 4 位登舱，5 位等待，摩天轮轮转。当前利润为 20 * $6 - 5 * $4 = $100 。
6. 4 位登舱，1 位等待，摩天轮轮转。当前利润为 24 * $6 - 6 * $4 = $120 。
7. 4 位登舱，摩天轮轮转。当前利润为 25 * $6 - 7 * $4 = $122 。
轮转 7 次得到最大利润，最大利润为$122 。
</pre>

<p><strong>示例 3：</strong></p>

<pre><strong>输入：</strong>customers = [3,4,0,5,1], boardingCost = 1, runningCost = 92
<strong>输出：</strong>-1
<strong>解释：</strong>
1. 3 位游客抵达，3 位登舱，0 位等待，摩天轮轮转。当前利润为 3 * $1 - 1 * $92 = -$89 。
2. 4 位游客抵达，4 位登舱，0 位等待，摩天轮轮转。当前利润为 is 7 * $1 - 2 * $92 = -$177 。
3. 0 位游客抵达，0 位登舱，0 位等待，摩天轮轮转。当前利润为 7 * $1 - 3 * $92 = -$269 。
4. 5 位游客抵达，4 位登舱，1 位等待，摩天轮轮转。当前利润为 12 * $1 - 4 * $92 = -$356 。
5. 1 位游客抵达，2 位登舱，0 位等待，摩天轮轮转。当前利润为 13 * $1 - 5 * $92 = -$447 。
利润永不为正，所以返回 -1 。
</pre>

<p><strong>示例 4：</strong></p>

<pre><strong>输入：</strong>customers = [10,10,6,4,7], boardingCost = 3, runningCost = 8
<strong>输出：</strong>9
<strong>解释：</strong>
1. 10 位游客抵达，4 位登舱，6 位等待，摩天轮轮转。当前利润为 4 * $3 - 1 * $8 = $4 。
2. 10 位游客抵达，4 位登舱，12 位等待，摩天轮轮转。当前利润为 8 * $3 - 2 * $8 = $8 。
3. 6 位游客抵达，4 位登舱，14 位等待，摩天轮轮转。当前利润为 12 * $3 - 3 * $8 = $12 。
4. 4 位游客抵达，4 位登舱，14 位等待，摩天轮轮转。当前利润为 16 * $3 - 4 * $8 = $16 。
5. 7 位游客抵达，4 位登舱，17 位等待，摩天轮轮转。当前利润为 20 * $3 - 5 * $8 = $20 。
6. 4 位登舱，13 位等待，摩天轮轮转。当前利润为 24 * $3 - 6 * $8 = $24 。
7. 4 位登舱，9 位等待，摩天轮轮转。当前利润为 28 * $3 - 7 * $8 = $28 。
8. 4 位登舱，5 位等待，摩天轮轮转。当前利润为 32 * $3 - 8 * $8 = $32 。
​​​​​​​9. 4 位登舱，1 位等待，摩天轮轮转。当前利润为 36 * $3 - 9 * $8 = $36 。
​​​​​​​10. 1 位登舱，0 位等待，摩天轮轮转。当前利润为 37 * $3 - 10 * $8 = $31 。
轮转 9 次得到最大利润，最大利润为 $36 。
</pre>

<p>&nbsp;</p>

<p><strong>提示：</strong></p>

<ul>
	<li><code>n == customers.length</code></li>
	<li><code>1 &lt;= n &lt;= 10<sup>5</sup></code></li>
	<li><code>0 &lt;= customers[i] &lt;= 50</code></li>
	<li><code>1 &lt;= boardingCost, runningCost &lt;= 100</code></li>
</ul>

 ---- 
 <p>一个王国里住着国王、他的孩子们、他的孙子们等等。每一个时间点，这个家庭里有人出生也有人死亡。</p>

<p>这个王国有一个明确规定的皇位继承顺序，第一继承人总是国王自己。我们定义递归函数&nbsp;<code>Successor(x, curOrder)</code>&nbsp;，给定一个人&nbsp;<code>x</code>&nbsp;和当前的继承顺序，该函数返回 <code>x</code>&nbsp;的下一继承人。</p>

<pre>Successor(x, curOrder):
    如果 x 没有孩子或者所有 x 的孩子都在 curOrder 中：
        如果 x 是国王，那么返回 null
        否则，返回 Successor(x 的父亲, curOrder)
    否则，返回 x 不在 curOrder 中最年长的孩子
</pre>

<p>比方说，假设王国由国王，他的孩子&nbsp;Alice 和 Bob （Alice 比 Bob&nbsp;年长）和 Alice 的孩子&nbsp;Jack 组成。</p>

<ol>
	<li>一开始，&nbsp;<code>curOrder</code>&nbsp;为&nbsp;<code>["king"]</code>.</li>
	<li>调用&nbsp;<code>Successor(king, curOrder)</code>&nbsp;，返回 Alice ，所以我们将 Alice 放入 <code>curOrder</code>&nbsp;中，得到&nbsp;<code>["king", "Alice"]</code>&nbsp;。</li>
	<li>调用&nbsp;<code>Successor(Alice, curOrder)</code>&nbsp;，返回 Jack ，所以我们将 Jack 放入&nbsp;<code>curOrder</code>&nbsp;中，得到&nbsp;<code>["king", "Alice", "Jack"]</code>&nbsp;。</li>
	<li>调用&nbsp;<code>Successor(Jack, curOrder)</code>&nbsp;，返回 Bob ，所以我们将 Bob 放入&nbsp;<code>curOrder</code>&nbsp;中，得到&nbsp;<code>["king", "Alice", "Jack", "Bob"]</code>&nbsp;。</li>
	<li>调用&nbsp;<code>Successor(Bob, curOrder)</code>&nbsp;，返回&nbsp;<code>null</code>&nbsp;。最终得到继承顺序为&nbsp;<code>["king", "Alice", "Jack", "Bob"]</code>&nbsp;。</li>
</ol>

<p>通过以上的函数，我们总是能得到一个唯一的继承顺序。</p>

<p>请你实现&nbsp;<code>ThroneInheritance</code>&nbsp;类：</p>

<ul>
	<li><code>ThroneInheritance(string kingName)</code> 初始化一个&nbsp;<code>ThroneInheritance</code>&nbsp;类的对象。国王的名字作为构造函数的参数传入。</li>
	<li><code>void birth(string parentName, string childName)</code>&nbsp;表示&nbsp;<code>parentName</code>&nbsp;新拥有了一个名为&nbsp;<code>childName</code>&nbsp;的孩子。</li>
	<li><code>void death(string name)</code>&nbsp;表示名为&nbsp;<code>name</code>&nbsp;的人死亡。一个人的死亡不会影响&nbsp;<code>Successor</code>&nbsp;函数，也不会影响当前的继承顺序。你可以只将这个人标记为死亡状态。</li>
	<li><code>string[] getInheritanceOrder()</code>&nbsp;返回 <strong>除去</strong>&nbsp;死亡人员的当前继承顺序列表。</li>
</ul>

<p>&nbsp;</p>

<p><strong>示例：</strong></p>

<pre><strong>输入：</strong>
["ThroneInheritance", "birth", "birth", "birth", "birth", "birth", "birth", "getInheritanceOrder", "death", "getInheritanceOrder"]
[["king"], ["king", "andy"], ["king", "bob"], ["king", "catherine"], ["andy", "matthew"], ["bob", "alex"], ["bob", "asha"], [null], ["bob"], [null]]
<strong>输出：</strong>
[null, null, null, null, null, null, null, ["king", "andy", "matthew", "bob", "alex", "asha", "catherine"], null, ["king", "andy", "matthew", "alex", "asha", "catherine"]]

<strong>解释：</strong>
ThroneInheritance t= new ThroneInheritance("king"); // 继承顺序：<strong>king</strong>
t.birth("king", "andy"); // 继承顺序：king &gt; <strong>andy</strong>
t.birth("king", "bob"); // 继承顺序：king &gt; andy &gt; <strong>bob</strong>
t.birth("king", "catherine"); // 继承顺序：king &gt; andy &gt; bob &gt; <strong>catherine</strong>
t.birth("andy", "matthew"); // 继承顺序：king &gt; andy &gt; <strong>matthew</strong> &gt; bob &gt; catherine
t.birth("bob", "alex"); // 继承顺序：king &gt; andy &gt; matthew &gt; bob &gt; <strong>alex</strong> &gt; catherine
t.birth("bob", "asha"); // 继承顺序：king &gt; andy &gt; matthew &gt; bob &gt; alex &gt; <strong>asha</strong> &gt; catherine
t.getInheritanceOrder(); // 返回 ["king", "andy", "matthew", "bob", "alex", "asha", "catherine"]
t.death("bob"); // 继承顺序：king &gt; andy &gt; matthew &gt; <strong>bob（已经去世）</strong>&gt; alex &gt; asha &gt; catherine
t.getInheritanceOrder(); // 返回 ["king", "andy", "matthew", "alex", "asha", "catherine"]
</pre>

<p>&nbsp;</p>

<p><strong>提示：</strong></p>

<ul>
	<li><code>1 &lt;= kingName.length, parentName.length, childName.length, name.length &lt;= 15</code></li>
	<li><code>kingName</code>，<code>parentName</code>，&nbsp;<code>childName</code>&nbsp;和&nbsp;<code>name</code>&nbsp;仅包含小写英文字母。</li>
	<li>所有的参数&nbsp;<code>childName</code> 和&nbsp;<code>kingName</code>&nbsp;<strong>互不相同</strong>。</li>
	<li>所有&nbsp;<code>death</code>&nbsp;函数中的死亡名字 <code>name</code>&nbsp;要么是国王，要么是已经出生了的人员名字。</li>
	<li>每次调用 <code>birth(parentName, childName)</code> 时，测试用例都保证 <code>parentName</code> 对应的人员是活着的。</li>
	<li>最多调用&nbsp;<code>10<sup>5</sup></code>&nbsp;次<code>birth</code> 和&nbsp;<code>death</code>&nbsp;。</li>
	<li>最多调用&nbsp;<code>10</code>&nbsp;次&nbsp;<code>getInheritanceOrder</code>&nbsp;。</li>
</ul>
 

-----
LeetCode 第83场周赛 


## 830. 较大分组的位置
在一个由小写字母构成的字符串 S 中，包含由一些连续的相同字符所构成的分组。

例如，在字符串 S = "abbxxxxzyy" 中，就含有 "a", "bb", "xxxx", "z" 和 "yy" 这样的一些分组。

我们称所有包含大于或等于三个连续字符的分组为较大分组。找到每一个较大分组的起始和终止位置。

最终结果按照字典顺序输出。

示例 1:

输入: "abbxxxxzzy"
输出: [[3,6]]
解释: "xxxx" 是一个起始于 3 且终止于 6 的较大分组。

示例 2:

输入: "abc"
输出: []
解释: "a","b" 和 "c" 均不是符合要求的较大分组。

示例 3:

输入: "abcdddeeeeaabbbcd"
输出: [[3,5],[6,9],[12,14]]

说明:  1 <= S.length <= 1000

```cpp
class Solution {
public:
    vector<vector<int>> largeGroupPositions(string s) {
        vector<vector<int>> ans;
        int len = s.size(), countConsecutive = 0;
        for (int i = 0; i < len; ++i) {
            ++countConsecutive;
            if (i + 1 >= len || s[i] != s[i + 1]) {
                if (countConsecutive >= 3)
                    ans.push_back({i - countConsecutive + 1, i});
                countConsecutive = 0;            
            }
        }
        return ans;
    }
};
```

----
## 831. 隐藏个人信息
<p>给你一条个人信息字符串 <code>S</code>，它可能是一个 <strong>邮箱地址</strong> ，也可能是一串 <strong>电话号码</strong> 。</p>

<p>我们将隐藏它的隐私信息，通过如下规则:</p>

<p>&nbsp;</p>

<p><strong>1. 电子邮箱</strong></p>

<p>定义名称 <strong>name</strong> 是长度大于等于 2 （<code>length ≥ 2</code>），并且只包含小写字母 <code>a-z</code> 和大写字母 <code>A-Z</code> 的字符串。</p>

<p>电子邮箱地址由名称 <strong>name</strong> 开头，紧接着是符号 <code>'@'</code>，后面接着一个名称 <strong>name</strong>，再接着一个点号 <code>'.'</code>，然后是一个名称 <strong>name</strong>。</p>

<p>电子邮箱地址确定为有效的，并且格式是 <code>"name1@name2.name3"</code>。</p>

<p>为了隐藏电子邮箱，所有的名称 <strong>name</strong> 必须被转换成小写的，并且第一个名称&nbsp;<strong>name</strong> 的第一个字母和最后一个字母的中间的所有字母由 5 个 <code>'*'</code> 代替。</p>

<p>&nbsp;</p>

<p><strong>2. 电话号码</strong></p>

<p>电话号码是一串包括数字&nbsp;<code>0-9</code>，以及 <code>{'+', '-', '(', ')', '&nbsp;'}</code> 这几个字符的字符串。你可以假设电话号码包含 10 到 13 个数字。</p>

<p>电话号码的最后 10 个数字组成本地号码，在这之前的数字组成国际号码。注意，国际号码是可选的。我们只暴露最后 4 个数字并隐藏所有其他数字。</p>

<p>本地号码是有格式的，并且如 <code>"***-***-1111"</code> 这样显示，这里的 1 表示暴露的数字。</p>

<p>为了隐藏有国际号码的电话号码，像&nbsp;<code>"+111 111 111 1111"</code>，我们以 <code>"+***-***-***-1111"</code> 的格式来显示。在本地号码前面的 <code>'+'</code> 号和第一个 <code>'-'</code> 号仅当电话号码中包含国际号码时存在。例如，一个 12 位的电话号码应当以 <code>"+**-"</code> 开头进行显示。</p>

<p>注意：像 <code>"("，")"，" "</code> 这样的不相干的字符以及不符合上述格式的额外的减号或者加号都应当被删除。</p>

<p>&nbsp;</p>

<p>最后，将提供的信息正确隐藏后返回。</p>

<p>&nbsp;</p>

<p><strong>示例 1：</strong></p>

<pre><strong>输入: </strong>"LeetCode@LeetCode.com"
<strong>输出: </strong>"l*****e@leetcode.com"
<strong>解释： 
</strong>所有的名称转换成小写, 第一个名称的第一个字符和最后一个字符中间由 5 个星号代替。
因此，"leetcode" -&gt; "l*****e"。
</pre>

<p><strong>示例 2：</strong></p>

<pre><strong>输入: </strong>"AB@qq.com"
<strong>输出: </strong>"a*****b@qq.com"
<strong>解释:&nbsp;
</strong>第一个名称"ab"的第一个字符和最后一个字符的中间必须有 5 个星号
因此，"ab" -&gt; "a*****b"。
</pre>

<p><strong>示例 3：</strong></p>

<pre><strong>输入: </strong>"1(234)567-890"
<strong>输出: </strong>"***-***-7890"
<strong>解释:</strong>&nbsp;
10 个数字的电话号码，那意味着所有的数字都是本地号码。
</pre>

<p><strong>示例 4：</strong></p>

<pre><strong>输入: </strong>"86-(10)12345678"
<strong>输出: </strong>"+**-***-***-5678"
<strong>解释:</strong>&nbsp;
12 位数字，2 个数字是国际号码另外 10 个数字是本地号码 。
</pre>

<p>&nbsp;</p>

<p><strong>注意:</strong></p>

<ol>
	<li><code>S.length&nbsp;&lt;=&nbsp;40</code>。</li>
	<li>邮箱的长度至少是 8。</li>
	<li>电话号码的长度至少是 10。</li>
</ol>

```cpp
class Solution {
public:
    string maskPII(string S) {
        string ans;
        size_t idx = S.find('@'); 
        if (idx != string::npos) {
            for (char &c : S) c = tolower(c);
            ans.push_back(S[0]);
            ans.append(5, '*');
            ans.push_back(S[idx - 1]);
            ans.append(S.substr(idx));
        } else { 
            for (auto it = S.begin(); it != S.end(); ) 
                if (*it == '+' || *it == '-' || *it == '(' || *it == ')' || *it == ' ') it = S.erase(it); 
                else ++it;
            if (S.size() > 10) ans.push_back('+');
            cout << S << endl;
            for (int i = 0; i < S.size(); ++i) {
                if (ans[0] == '+' && i == S.size() - 10) ans.push_back('-');
                if (i == S.size() - 7 || i == S.size() - 4) ans.push_back('-');
                if (i < S.size() - 4) ans.push_back('*');
                else ans.push_back(S[i]);
            } 
        }
        return ans;
    }
};
```
---
### 829. 连续整数求和
 <p>给定一个正整数 <code>N</code>，试求有多少组连续正整数满足所有数字之和为 <code>N</code>?</p>

<p><strong>示</strong><strong>例 1:</strong></p>

<pre><strong>输入: </strong>5
<strong>输出: </strong>2
<strong>解释: </strong>5 = 5 = 2 + 3，共有两组连续整数([5],[2,3])求和后为 5。</pre>

<p><strong>示例 2:</strong></p>

<pre><strong>输入: </strong>9
<strong>输出: </strong>3
<strong>解释: </strong>9 = 9 = 4 + 5 = 2 + 3 + 4</pre>

<p><strong>示例 3:</strong></p>

<pre><strong>输入: </strong>15
<strong>输出: </strong>4
<strong>解释: </strong>15 = 15 = 8 + 7 = 4 + 5 + 6 = 1 + 2 + 3 + 4 + 5</pre>

<p><strong>说明:&nbsp;</strong><code>1 &lt;= N &lt;= 10 ^ 9</code></p>
 
首先看题目中给出的N的大小是10^9，由此可以看出O(n)的时间复杂度是肯定会超时的，那么最少也的是O(logn)才可以，分析这题题意，是让我们找到所有连续的序列相加和为n，第一眼看上去肯定是暴力，而暴力肯定是过不了的，换个思路想等差数列的求和公式，
（n1 + n2) * (n2 - n1 + 1)  = 2 * N，其中（n1 + n2） 和 （n2 - n1 + 1）一定要是2*N的因子，因此转化成求2 * N的因子的问题，而经过优化成功的将时间复杂度控制在了O(logn)。 

???

---
  <p>我们定义了一个函数 <code>countUniqueChars(s)</code> 来统计字符串 <code>s</code> 中的唯一字符，并返回唯一字符的个数。</p>

<p>例如：<code>s = "LEETCODE"</code> ，则其中 <code>"L"</code>, <code>"T"</code>,<code>"C"</code>,<code>"O"</code>,<code>"D"</code> 都是唯一字符，因为它们只出现一次，所以 <code>countUniqueChars(s) = 5</code> 。</p>

<p>本题将会给你一个字符串 <code>s</code> ，我们需要返回 <code>countUniqueChars(t)</code> 的总和，其中 <code>t</code> 是 <code>s</code> 的子字符串。注意，某些子字符串可能是重复的，但你统计时也必须算上这些重复的子字符串（也就是说，你必须统计 <code>s</code> 的所有子字符串中的唯一字符）。</p>

<p>由于答案可能非常大，请将结果 <strong>mod 10 ^ 9 + 7</strong> 后再返回。</p>

<p>&nbsp;</p>

<p><strong>示例 1：</strong></p>

<pre><strong>输入: </strong>"ABC"
<strong>输出: </strong>10
<strong>解释:</strong> 所有可能的子串为："A","B","C","AB","BC" 和 "ABC"。
     其中，每一个子串都由独特字符构成。
     所以其长度总和为：1 + 1 + 1 + 2 + 2 + 3 = 10
</pre>

<p><strong>示例 2：</strong></p>

<pre><strong>输入: </strong>"ABA"
<strong>输出: </strong>8
<strong>解释: </strong>除<code>了 countUniqueChars</code>("ABA") = 1 之外，其余与示例 1 相同。
</pre>

<p><strong>示例 3：</strong></p>

<pre><strong>输入：</strong>s = "LEETCODE"
<strong>输出：</strong>92
</pre>

<p>&nbsp;</p>

<p><strong>提示：</strong></p>

<ul>
	<li><code>0 &lt;= s.length &lt;= 10^4</code></li>
	<li><code>s</code> 只包含大写英文字符</li>
</ul>
这道题给了我们一个字符串S，要统计其所有的子串中不同字符的个数之和，这里的子串是允许重复的，而且说结果需要对一个超大数取余，这暗示了返回值可能会很大，这样的话对于纯暴力的解法，比如遍历所有可能的子串并统计不同字符的个数的这种解法肯定是不行的。这道题还真是一点没有辱没其 Hard 标签，确实是一道很有难度的题，不太容易想出正确解法。还好有 [李哥 lee215 的帖子](https://leetcode.com/problems/unique-letter-string/discuss/128952/One-pass-O(N)-Straight-Forward)，一个帖子的点赞数超过了整个第一页所有其他帖子的点赞数之和，简直是刷题界的 Faker，你李哥永远是你李哥。这里就按照李哥的帖子来讲解吧，首先来看一个字符串 CACACCAC，若想让第二个A成为子串中的唯一，那么必须要知道其前后两个相邻的A的位置，比如 CA(CACC)AC，括号中的子串 CACC 中A就是唯一的存在，同样，对于 CAC(AC)CAC，括号中的子串 AC 中A也是唯一的存在。这样就可以观察出来，只要左括号的位置在第一个A和第二个A之间（共有2个位置），右括号在第二个A和第三个A之间（共有3个位置），这样第二个A在6个子串中成为那个唯一的存在。换个角度来说，只有6个子串可以让第二个A作为单独的存在从而在结果中贡献。这是个很关键的转换思路，与其关注每个子串中的单独字符个数，不如换个角度，对于每个字符，统计其可以在多少个子串中成为单独的存在，同样可以得到正确的结果。这样的话，每个字母出现的位置就很重要了，由于上面的分析说了，只要知道三个位置，就可以求出中间的字母的贡献值，为了节省空间，只保留每个字母最近两次的出现位置，这样加上当前位置i，就可以知道前一个字母的贡献值了。这里使用一个长度为 26x2 的二维数组 idx，因为题目中限定了只有26个大写字母。这里只保留每个字母的前两个出现位置，均初始化为 -1。然后遍历S中每个字母，对于每个字符减去A，就是其对应位置，此时将前一个字母的贡献值累加到结果 res 中，假如当前字母是首次出现，也不用担心，前两个字母的出现位置都是 -1，相减后为0，所以累加值还是0。然后再更新 idx 数组的值。由于每次都是计算该字母前一个位置的贡献值，所以最后还需要一个 for 循环去计算每个字母最后出现位置的贡献值，此时由于身后没有该字母了，就用位置N来代替即可，参见代码如下：

解法一：

class Solution {
public:
    int uniqueLetterString(string S) {
        int res = 0, n = S.size(), M = 1e9 + 7;
        vector<vector<int>> idx(26, vector<int>(2, -1));
        for (int i = 0; i < n; ++i) {
        	int c = S[i] - 'A';
        	res = (res + (i - idx[c][1]) * (idx[c][1] - idx[c][0]) % M) % M;
        	idx[c][0] = idx[c][1];
        	idx[c][1] = i;
        }
        for (int c = 0; c < 26; ++c) {
        	res = (res + (n - idx[c][1]) * (idx[c][1] - idx[c][0]) % M) % M;
        }
        return res;
    }
};


我们也可以换一种解法，使得其更加简洁一些，思路稍微有些不同，这里参考了 [大神 meng789987 的帖子](https://leetcode.com/problems/unique-letter-string/discuss/158378/Concise-DP-O(n)-solution)。使用的是动态规划 Dynmaic Programming 的思想，用一个一维数组 dp，其中 dp[i] 表示以 S[i] 为结尾的所有子串中的单独字母个数之和，这样只要把 [0, n-1] 范围内所有的 dp[i] 累加起来就是最终的结果了。更新 dp[i] 的方法关键也是要看重复的位置，比如当前是 AB 的话，此时 dp[1]=3，因为以B结尾的子串是 B 和 AB，共有3个单独字母。若此时再后面加上个C的话，由于没有重复出现，则以C结尾的子串 C，BC，ABC 共有6个单独字母，即 dp[2]=6，怎么由 dp[1] 得到呢？首先新加的字母本身就是子串，所以一定是可以贡献1的，然后由于之前都没有C出现，则之前的每个子串中C都可以贡献1，而原本的A和B的贡献值也将保留，所以总共就是 dp[2] = 1+dp[1]+2 = 6。但若新加的字母是A的话，就比较 tricky 了，首先A本身也是子串，有稳定的贡献1，由于之前已经有A的出现了，所以只要知道了之前A的位置，那么中间部分是没有A的，即子串 B 中没有A，A可以贡献1，但是对于之前的有A的子串，比如 AB，此时新加的A不但不能贡献，反而还会伤害之前A的贡献值，即变成 ABA 了后，不但第二个A不能贡献，连第一个A之前的贡献值也要减去，此时 dp[2] = 1+dp[1]+(2-1)-(1-0) = 4。其中2是当前A的位置，1是前一个A的位置加1，0是再前一个A的位置加1。讲到这里应该就比较清楚了吧，这里还是要知道每个字符的前两次出现的位置，这里用两个数组 first 和 second，不过需要注意的是，这里保存的是位置加1。又因为每个 dp 值只跟其前一个 dp 值有关，所以为了节省空间，并不需要一个 dp 数组，而是只用一个变量 cur 进行累加即可，记得每次循环都要把 cur 存入结果 res 中。那么每次 cur 的更新方法就是前一个 cur 值加上1，再加上当前字母产生的贡献值，减去当前字母抵消的贡献值，参见代码如下：

解法二：

class Solution {
public:
    int uniqueLetterString(string S) {
        int res = 0, n = S.size(), cur = 0, M = 1e9 + 7;
        vector<int> first(26), second(26);
        for (int i = 0; i < n; ++i) {
        	int c = S[i] - 'A';
        	cur = cur + 1 + i - first[c] * 2 + second[c];
        	res = (res + cur) % M;
        	second[c] = first[c];
        	first[c] = i + 1;
        }
        return res;
    }
};

