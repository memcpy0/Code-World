<p>Design a time-based key-value data structure that can store multiple values for the same key at different time stamps and retrieve the key's value at a certain timestamp.</p>

<p>Implement the <code>TimeMap</code> class:</p>

<ul>
	<li><code>TimeMap()</code> Initializes the object of the data structure.</li>
	<li><code>void set(String key, String value, int timestamp)</code> Stores the key <code>key</code> with the value <code>value </code>at the given time <code>timestamp</code>.</li>
	<li><code>String get(String key, int timestamp)</code> Returns a value such that <code>set</code> was called previously, with <code>timestamp_prev &lt;= timestamp</code>. If there are multiple such values, it returns the value associated with the largest <code>timestamp_prev</code>. If there are no values, it returns <code>""</code>.</li>
</ul>

 
<p><strong>Example 1:</strong></p>

```haskell
Input
["TimeMap", "set", "get", "get", "set", "get", "get"]
[[], ["foo", "bar", 1], ["foo", 1], ["foo", 3], ["foo", "bar2", 4], ["foo", 4], ["foo", 5]]
Output
[null, null, "bar", "bar", null, "bar2", "bar2"]

Explanation
TimeMap timeMap = new TimeMap();
timeMap.set("foo", "bar", 1);  // store the key "foo" and value "bar" along with timestamp = 1.
timeMap.get("foo", 1);         // return "bar"
timeMap.get("foo", 3);         // return "bar", since there is no value corresponding to foo at timestamp 3 and timestamp 2, then the only value is at timestamp 1 is "bar".
timeMap.set("foo", "bar2", 4); // store the key "foo" and value "ba2r" along with timestamp = 4.
timeMap.get("foo", 4);         // return "bar2"
timeMap.get("foo", 5);         // return "bar2" 
```
 
 
<p><strong>Constraints:</strong></p>

<ul>
	<li><code>1 &lt;= key.length, value.length &lt;= 100</code></li>
	<li><code>key</code> and <code>value</code> consist of lowercase English letters and digits.</li>
	<li><code>1 &lt;= timestamp &lt;= 10<sup>7</sup></code></li>
	<li>All the timestamps <code>timestamp</code> of <code>set</code> are strictly increasing.</li>
	<li>At most <code>2 * 10<sup>5</sup></code> calls will be made to <code>set</code> and <code>get</code>.</li>
</ul>

题意：创建一个基于时间的键值存储类&nbsp;<code>TimeMap</code>，它支持下面两个操作：
<p>1. <code>set(string key, string value, int timestamp)</code> ：存储键&nbsp;<code>key</code>、值&nbsp;<code>value</code>，以及给定的时间戳&nbsp;<code>timestamp</code>。 
<p>2. <code>get(string key, int timestamp)</code> ： </p>
<ul>
	<li>返回先前调用&nbsp;<code>set(key, value, timestamp_prev)</code>&nbsp;所存储的值，其中&nbsp;<code>timestamp_prev &lt;= timestamp</code>。</li>
	<li>如果有多个这样的值，则返回对应最大的&nbsp;&nbsp;<code>timestamp_prev</code>&nbsp;的那个值。</li>
	<li>如果没有值，则返回空字符串（<code>""</code>）。</li>
</ul>
 
---
遇到这样的题目，第一时间想到的就是映射（有序映射和哈希映射）和二分，不过具体怎样写还有待解决。

### 解法1 哈希映射+数组

### 解法2 哈希映射+有序映射
