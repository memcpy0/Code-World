<p>读入 <span class="katex"><span class="katex-mathml"><math><mrow><mi>n</mi></mrow>n</math></span><span aria-hidden="true" class="katex-html"><span style="height:0.43056em;" class="strut"></span><span style="height:0.43056em;vertical-align:0em;" class="strut bottom"></span><span class="base textstyle uncramped"><span class="mord mathit">n</span></span></span></span>（<span class="katex"><span class="katex-mathml"><math><mrow><mo>&gt;</mo><mn>0</mn></mrow>&gt;0</math></span><span aria-hidden="true" class="katex-html"><span style="height:0.64444em;" class="strut"></span><span style="height:0.68354em;vertical-align:-0.0391em;" class="strut bottom"></span><span class="base textstyle uncramped"><span class="mrel">&gt;</span><span class="mord mathrm">0</span></span></span></span>）名学生的姓名、学号、成绩，分别输出成绩最高和成绩最低学生的姓名和学号。</p>
<h3 id="输入格式：">输入格式：</h3>
<p>每个测试输入包含 1 个测试用例，格式为</p>
<pre class="pre_2GaNN"><code class="hljs angelscript">第 <span class="hljs-number">1</span> 行：正整数 n
第 <span class="hljs-number">2</span> 行：第 <span class="hljs-number">1</span> 个学生的姓名 学号 成绩
第 <span class="hljs-number">3</span> 行：第 <span class="hljs-number">2</span> 个学生的姓名 学号 成绩
  ... ... ...
第 n+<span class="hljs-number">1</span> 行：第 n 个学生的姓名 学号 成绩</code><div><div class="pc-icon clipboardIcon_2q_Ks pc-active-primary inline" aria-label="点击复制"><use xlink:href="#pat-clippy"></use></svg></div></div></pre>
<p>其中<code>姓名</code>和<code>学号</code>均为不超过 10 个字符的字符串，成绩为 0 到 100 之间的一个整数，这里保证在一组测试用例中没有两个学生的成绩是相同的。</p>
<h3 id="输出格式：">输出格式：</h3>
<p>对每个测试用例输出 2 行，第 1 行是成绩最高学生的姓名和学号，第 2 行是成绩最低学生的姓名和学号，字符串间有 1 空格。</p>
<h3 id="输入样例：">输入样例：</h3>
 

```swift
3
Joe Math990112 89
Mike CS991301 100
Mary EE990830 95
```

<h3 id="输出样例：">输出样例：</h3>
 

```swift
Mike CS991301
Joe Math990112
```
---
以前的代码：
```cpp
#include <cstdio>
struct student {
    char name[15];
    char id[15];
    int score;
}stu, max, min;

int main()
{
    int n;
    scanf("%d", &n);
    scanf("%s %s %d", stu.name, stu.id, &stu.score);
    max = min = stu;
    for (int i = 1; i < n; i++) {
        scanf("%s %s %d", stu.name, stu.id, &stu.score);
        if (stu.score > max.score) max = stu;
        if (stu.score < min.score) min = stu;
    }
    printf("%s %s\n%s %s\n", max.name, max.id, min.name, min.id);
    
    return 0;
}
```
现在的代码：
```cpp
#include <bits/stdc++.h>
using namespace std;
struct stu {
    char name[12], id[12];
    int score;
} s, e, t;
int main() {
    int n;
    scanf("%d%s%s%d", &n, t.name, t.id, &t.score);
    s = e = t;
    for (int i = 1; i < n; ++i) {
        scanf("%s%s%d", t.name, t.id, &t.score);
        if (t.score > s.score) s = t;
        if (t.score < e.score) e = t;
    }
    printf("%s %s\n%s %s", s.name, s.id, e.name, e.id);
    return 0;
}
```
