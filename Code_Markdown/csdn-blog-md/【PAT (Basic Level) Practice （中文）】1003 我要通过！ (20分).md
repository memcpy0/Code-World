<p>“<strong>答案正确</strong>”是自动判题系统给出的最令人欢喜的回复。本题属于 PAT 的“<strong>答案正确</strong>”大派送 —— 只要读入的字符串满足下列条件，系统就输出“<strong>答案正确</strong>”，否则输出“<strong>答案错误</strong>”。</p>
<p>得到“<strong>答案正确</strong>”的条件是：</p>
<ol>
<li>字符串中必须仅有 <code>P</code>、 <code>A</code>、 <code>T</code>这三种字符，不可以包含其它字符；</li>
<li>任意形如 <code>xPATx</code> 的字符串都可以获得“<strong>答案正确</strong>”，其中 <code>x</code> 或者是空字符串，或者是仅由字母 <code>A</code> 组成的字符串；</li>
<li>如果 <code>aPbTc</code> 是正确的，那么 <code>aPbATca</code> 也是正确的，其中 <code>a</code>、 <code>b</code>、 <code>c</code> 均或者是空字符串，或者是仅由字母 <code>A</code> 组成的字符串。</li>
</ol>
<p>现在就请你为 PAT 写一个自动裁判程序，判定哪些字符串是可以获得“<strong>答案正确</strong>”的。</p>
<h3 id="输入格式：">输入格式：</h3>
<p>每个测试输入包含 1 个测试用例。第 1 行给出一个正整数 <span class="katex"><span class="katex-mathml"><math><mrow><mi>n</mi></mrow>n</math></span><span aria-hidden="true" class="katex-html"><span style="height:0.43056em;" class="strut"></span><span style="height:0.43056em;vertical-align:0em;" class="strut bottom"></span><span class="base textstyle uncramped"><span class="mord mathit">n</span></span></span></span> (<span class="katex"><span class="katex-mathml"><math><mrow><mo>&lt;</mo><mn>1</mn><mn>0</mn></mrow>&lt;10</math></span><span aria-hidden="true" class="katex-html"><span style="height:0.64444em;" class="strut"></span><span style="height:0.68354em;vertical-align:-0.0391em;" class="strut bottom"></span><span class="base textstyle uncramped"><span class="mrel">&lt;</span><span class="mord mathrm">1</span><span class="mord mathrm">0</span></span></span></span>)，是需要检测的字符串个数。接下来每个字符串占一行，字符串长度不超过 100，且不包含空格。</p>
<h3 id="输出格式：">输出格式：</h3>
<p>每个字符串的检测结果占一行，如果该字符串可以获得“<strong>答案正确</strong>”，则输出 <code>YES</code>，否则输出 <code>NO</code>。</p>
<h3 id="输入样例：">输入样例：</h3>

```swift
8
PAT
PAAT
AAPATAA
AAPAATAAAA
xPATx
PT
Whatever
APAAATAA
```

<h3 id="输出样例：">输出样例：</h3>
 

```swift
YES
YES
YES
YES
NO
NO
NO
NO
```
---
以前的代码：
```cpp
#include <cstdio>
#include <cstring>
int main()
{
    int T;
    scanf("%d", &T);
    while (T--) {
        char s[120];
        scanf("%s", s);
        int len = strlen(s);
        int num_p = 0, num_t = 0, other = 0; //代表P的个数、T的个数、除PAT外字符的个数
        int loc_p = -1, loc_t = -1; //分别代表P的位置、T的位置
        for (int i = 0; i < len; i++) {
            if (s[i] == 'P') { //当前字符为P
                num_p++;   //个数+1
                loc_p = i; //位置变为i
            } else if (s[i] == 'T') { //当前字符为T
                num_t++;   //个数+1
                loc_t = i; //位置变为i
            } else if (s[i] != 'A') other++; //除PAT外字符的个数+1
        }
        //如果P的个数不为1; 或者T的个数不为1; 或者存在除PAT之外的字符; 或者P和T之间没有字符
        if (num_p != 1 || num_t != 1 || other != 0 || loc_t - loc_p <= 1) {
            printf("NO\n"); continue;
        } 
        int x = loc_p, y = loc_t - loc_p - 1, z = len - loc_t - 1;
        if (z - x * (y - 1) == x) { //条件2成立的思路
            printf("YES\n");
        } else printf("NO\n");
    }
    return 0;
}
```
