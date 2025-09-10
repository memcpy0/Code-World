> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的[仓库](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

<p>There is a network of <code>n</code> servers, labeled from <code>0</code> to <code>n - 1</code>. You are given a 2D integer array <code>edges</code>, where <code>edges[i] = [u<sub>i</sub>, v<sub>i</sub>]</code> indicates there is a message channel between servers <code>u<sub>i</sub></code> and <code>v<sub>i</sub></code>, and they can pass <strong>any</strong> number of messages to <strong>each other</strong> directly in <strong>one</strong> second. You are also given a <strong>0-indexed</strong> integer array <code>patience</code> of length <code>n</code>.</p>

<p>All servers are <strong>connected</strong>, i.e., a message can be passed from one server to any other server(s) directly or indirectly through the message channels.</p>

<p>The server labeled <code>0</code> is the <strong>master</strong> server. The rest are <strong>data</strong> servers. Each data server needs to send its message to the master server for processing and wait for a reply. Messages move between servers <strong>optimally</strong>, so every message takes the <strong>least amount of time</strong> to arrive at the master server. The master server will process all newly arrived messages <strong>instantly</strong> and send a reply to the originating server via the <strong>reversed path</strong> the message had gone through.</p>

<p>At the beginning of second <code>0</code>, each data server sends its message to be processed. Starting from second <code>1</code>, at the <strong>beginning</strong> of <strong>every</strong> second, each data server will check if it has received a reply to the message it sent (including any newly arrived replies) from the master server:</p>

<ul>
	<li>If it has not, it will <strong>resend</strong> the message periodically. The data server <code>i</code> will resend the message every <code>patience[i]</code> second(s), i.e., the data server <code>i</code> will resend the message if <code>patience[i]</code> second(s) have <strong>elapsed</strong> since the <strong>last</strong> time the message was sent from this server.</li>
	<li>Otherwise, <strong>no more resending</strong> will occur from this server.</li>
</ul>

<p>The network becomes <strong>idle</strong> when there are <strong>no</strong> messages passing between servers or arriving at servers.</p>

<p>Return <em>the <strong>earliest second</strong> starting from which the network becomes <strong>idle</strong></em>.</p>

<p><strong>Example 1:</strong></p>
<img style="width: 750px; height: 384px;" src="https://assets.leetcode.com/uploads/2021/09/22/quiet-place-example1.png" alt="example 1">

```c
Input: edges = [[0,1],[1,2]], patience = [0,2,1]
Output: 8
Explanation:
At (the beginning of) second 0,
- Data server 1 sends its message (denoted 1A) to the master server.
- Data server 2 sends its message (denoted 2A) to the master server.

At second 1,
- Message 1A arrives at the master server. Master server processes message 1A instantly and sends a reply 1A back.
- Server 1 has not received any reply. 1 second (1 < patience[1] = 2) elapsed since this server has sent the message, therefore it does not resend the message.
- Server 2 has not received any reply. 1 second (1 == patience[2] = 1) elapsed since this server has sent the message, therefore it resends the message (denoted 2B).

At second 2,
- The reply 1A arrives at server 1. No more resending will occur from server 1.
- Message 2A arrives at the master server. Master server processes message 2A instantly and sends a reply 2A back.
- Server 2 resends the message (denoted 2C).
...
At second 4,
- The reply 2A arrives at server 2. No more resending will occur from server 2.
...
At second 7, reply 2D arrives at server 2.

Starting from the beginning of the second 8, there are no messages passing between servers or arriving at servers.
This is the time when the network becomes idle.
```
<p><strong>Example 2:</strong></p>
<img style="width: 100px; height: 85px;" src="https://assets.leetcode.com/uploads/2021/09/04/network_a_quiet_place_2.png" alt="example 2">

```c
Input: edges = [[0,1],[0,2],[1,2]], patience = [0,10,10]
Output: 3
Explanation: Data servers 1 and 2 receive a reply back at the beginning of second 2.
From the beginning of the second 3, the network becomes idle.
```
 
<p><strong>Constraints:</strong></p>

<ul>
	<li><code>n == patience.length</code></li>
	<li><code>2 &lt;= n &lt;= 10<sup>5</sup></code></li>
	<li><code>patience[0] == 0</code></li>
	<li><code>1 &lt;= patience[i] &lt;= 10<sup>5</sup></code> for <code>1 &lt;= i &lt; n</code></li>
	<li><code>1 &lt;= edges.length &lt;= min(10<sup>5</sup>, n * (n - 1) / 2)</code></li>
	<li><code>edges[i].length == 2</code></li>
	<li><code>0 &lt;= u<sub>i</sub>, v<sub>i</sub> &lt; n</code></li>
	<li><code>u<sub>i</sub> != v<sub>i</sub></code></li>
	<li>There are no duplicate edges.</li>
	<li>Each server can directly or indirectly reach another server.</li>
</ul>

题目：给出一个有 <code>n</code> 个服务器的相通的计算机网络，服务器编号为 <code>0</code> 到 <code>n - 1</code> 。同时给出一个二维整数数组 <code>edges</code> ，其中 <code>edges[i] = [u<sub>i</sub>, v<sub>i</sub>]</code> 表示服务器 <code>u<sub>i</sub></code> 和 <code>v<sub>i</sub></code><sub> </sub>之间有一条信息线路，在 <strong>一秒</strong> 内它们之间可以传输 <strong>任意</strong> 数目的信息。再给你一个长度为 <code>n</code> 且下标从 <strong>0</strong> 开始的整数数组 <code>patience</code> 。

<p>编号为 <code>0</code> 的服务器是 <strong>主</strong> 服务器，其他服务器为 <strong>数据</strong> 服务器。每个数据服务器都要向主服务器发送信息，并等待回复。信息在服务器之间按 <strong>最优</strong> 线路传输，也就是说每个信息都会以 <strong>最少时间</strong> 到达主服务器。主服务器会处理 <strong>所有</strong> 新到达的信息，并 <strong>立即</strong> 按照每条信息来时的路线 <strong>反方向</strong> 发送回复信息。</p>

<p>在 <code>0</code> 秒的开始，所有数据服务器都会发送各自需要处理的信息。从第 <code>1</code> 秒开始，<strong>每</strong> 一秒最 <strong>开始</strong> 时，每个数据服务器都会检查它是否收到了主服务器的回复信息（包括新发出信息的回复信息）：</p>

<ul>
	<li>如果还没收到任何回复信息，那么该服务器会周期性 <strong>重发</strong> 信息。数据服务器 <code>i</code> 每 <code>patience[i]</code> 秒都会重发一条信息，即数据服务器 <code>i</code> 在上一次发送信息给主服务器后的 <code>patience[i]</code> 秒 <strong>后</strong> 会重发一条信息给主服务器。</li>
	<li>否则，该数据服务器 <strong>不会重发</strong> 信息。</li>
</ul>

<p>当没有任何信息在线路上传输或者到达某服务器时，该计算机网络变为 <strong>空闲</strong> 状态。返回计算机网络变为 <strong>空闲</strong> 状态的 <strong>最早秒数</strong> 。 
 
---
### 解法 图+BFS
据题意可知，这是一个边权为 $1$ 的无向连通图，可以用邻接表建图，并用BFS预处理出 `dist[]` ，`dist[i]` 表示节点 $0$ 到节点 $i$ 的最短距离。一个数据服务器 $i$ 向主服务器发送消息，所消耗的时间为两个节点间的最短距离 `dist[i]` ，由于**主服务器处理所有消息、并立即回复消息**，从发送消息到收到回复所需的时间——即往返时间 `round-trip time, rtt` 等于 `2 * dist[i]` 。

另外还需注意，每个数据服务器从第一次发送消息开始，每隔 `patience[i]` 秒会重新发送一次消息，重发动作直到（第一次）收到主服务器的回复后才会停止。

因此，如果 `patience[i] >= rtt` ，数据服务器不会出现重发动作，该节点的活动停止时间点为 `rtt` ；如果 `patience[i] < rtt` ，数据服务器会出现重发动作（可能不止一次，我们只关注最后一次），且最后一个消息的发送时间为 `(rtt - 1) / patience[i] * patience[i]` ，这一发送时间加上该数据包的往返时间 `rtt` ，即为该节点的活动停止时间点。**所有节点的活动停止时间点的最大值再加一**即为答案。

```cpp
// C++ version
class Solution {
public:
    int networkBecomesIdle(vector<vector<int>>& edges, vector<int>& patience) {
        int n = patience.size();
        vector<vector<int>> g(patience.size());
        vector<int> dist(n, -1);
        // 建图
        for (const auto &e : edges) {
            g[e[0]].push_back(e[1]);
            g[e[1]].push_back(e[0]);
        }
        // BFS求结点0到其他结点的最短距离
        queue<int> q;
        q.push(0);
        dist[0] = 0;
        while (!q.empty()) {
            int u = q.front(); q.pop();
            for (const int &v : g[u]) 
                if (dist[v] == -1) { q.push(v); dist[v] = dist[u] + 1; }
        }
        int ans = 0;
        for (int i = 1; i < n; ++i) {
            int rtt = 2 * dist[i]; // patience[i] >= 2*dist[i] 时, 不会重发消息, 对应的lastSent = 0
            int lastSent = (rtt - 1) / patience[i] * patience[i];
            ans = max(ans, lastSent + rtt);
        }
        return ans + 1;
    }
};
// 执行用时：476 ms, 在所有 C++ 提交中击败了83.62% 的用户
// 内存消耗：184.3 MB, 在所有 C++ 提交中击败了66.38% 的用户
```


