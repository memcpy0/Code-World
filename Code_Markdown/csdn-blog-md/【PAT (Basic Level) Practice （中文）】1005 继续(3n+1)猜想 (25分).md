<p>卡拉兹(Callatz)猜想已经在1001中给出了描述。在这个题目里，情况稍微有些复杂。</p>
<p>当我们验证卡拉兹猜想的时候，为了避免重复计算，可以记录下递推过程中遇到的每一个数。例如对 <span class="katex"><span class="katex-mathml"><math><mrow><mi>n</mi><mo>=</mo><mn>3</mn></mrow>n=3</math></span><span aria-hidden="true" class="katex-html"><span style="height:0.64444em;" class="strut"></span><span style="height:0.64444em;vertical-align:0em;" class="strut bottom"></span><span class="base textstyle uncramped"><span class="mord mathit">n</span><span class="mrel">=</span><span class="mord mathrm">3</span></span></span></span> 进行验证的时候，我们需要计算 3、5、8、4、2、1，则当我们对 <span class="katex"><span class="katex-mathml"><math><mrow><mi>n</mi><mo>=</mo><mn>5</mn></mrow>n=5</math></span><span aria-hidden="true" class="katex-html"><span style="height:0.64444em;" class="strut"></span><span style="height:0.64444em;vertical-align:0em;" class="strut bottom"></span><span class="base textstyle uncramped"><span class="mord mathit">n</span><span class="mrel">=</span><span class="mord mathrm">5</span></span></span></span>、8、4、2 进行验证的时候，就可以直接判定卡拉兹猜想的真伪，而不需要重复计算，因为这 4 个数已经在验证3的时候遇到过了，我们称 5、8、4、2 是被 3“覆盖”的数。我们称一个数列中的某个数 <span class="katex"><span class="katex-mathml"><math><mrow><mi>n</mi></mrow>n</math></span><span aria-hidden="true" class="katex-html"><span style="height:0.43056em;" class="strut"></span><span style="height:0.43056em;vertical-align:0em;" class="strut bottom"></span><span class="base textstyle uncramped"><span class="mord mathit">n</span></span></span></span> 为“关键数”，如果 <span class="katex"><span class="katex-mathml"><math><mrow><mi>n</mi></mrow>n</math></span><span aria-hidden="true" class="katex-html"><span style="height:0.43056em;" class="strut"></span><span style="height:0.43056em;vertical-align:0em;" class="strut bottom"></span><span class="base textstyle uncramped"><span class="mord mathit">n</span></span></span></span> 不能被数列中的其他数字所覆盖。</p>
<p>现在给定一系列待验证的数字，我们只需要验证其中的几个关键数，就可以不必再重复验证余下的数字。你的任务就是找出这些关键数字，并按从大到小的顺序输出它们。</p>
<h3 id="输入格式：">输入格式：</h3>
<p>每个测试输入包含 1 个测试用例，第 1 行给出一个正整数 <span class="katex"><span class="katex-mathml"><math><mrow><mi>K</mi></mrow>K</math></span><span aria-hidden="true" class="katex-html"><span style="height:0.68333em;" class="strut"></span><span style="height:0.68333em;vertical-align:0em;" class="strut bottom"></span><span class="base textstyle uncramped"><span style="margin-right:0.07153em;" class="mord mathit">K</span></span></span></span> (<span class="katex"><span class="katex-mathml"><math><mrow><mo>&lt;</mo><mn>1</mn><mn>0</mn><mn>0</mn></mrow>&lt;100</math></span><span aria-hidden="true" class="katex-html"><span style="height:0.64444em;" class="strut"></span><span style="height:0.68354em;vertical-align:-0.0391em;" class="strut bottom"></span><span class="base textstyle uncramped"><span class="mrel">&lt;</span><span class="mord mathrm">1</span><span class="mord mathrm">0</span><span class="mord mathrm">0</span></span></span></span>)，第 2 行给出 <span class="katex"><span class="katex-mathml"><math><mrow><mi>K</mi></mrow>K</math></span><span aria-hidden="true" class="katex-html"><span style="height:0.68333em;" class="strut"></span><span style="height:0.68333em;vertical-align:0em;" class="strut bottom"></span><span class="base textstyle uncramped"><span style="margin-right:0.07153em;" class="mord mathit">K</span></span></span></span> 个互不相同的待验证的正整数 <span class="katex"><span class="katex-mathml"><math><mrow><mi>n</mi></mrow>n</math></span><span aria-hidden="true" class="katex-html"><span style="height:0.43056em;" class="strut"></span><span style="height:0.43056em;vertical-align:0em;" class="strut bottom"></span><span class="base textstyle uncramped"><span class="mord mathit">n</span></span></span></span> (<span class="katex"><span class="katex-mathml"><math><mrow><mn>1</mn><mo>&lt;</mo><mi>n</mi><mo>≤</mo><mn>1</mn><mn>0</mn><mn>0</mn></mrow>1&lt;n\le 100</math></span><span aria-hidden="true" class="katex-html"><span style="height:0.64444em;" class="strut"></span><span style="height:0.78041em;vertical-align:-0.13597em;" class="strut bottom"></span><span class="base textstyle uncramped"><span class="mord mathrm">1</span><span class="mrel">&lt;</span><span class="mord mathit">n</span><span class="mrel">≤</span><span class="mord mathrm">1</span><span class="mord mathrm">0</span><span class="mord mathrm">0</span></span></span></span>)的值，数字间用空格隔开。</p>
<h3 id="输出格式：">输出格式：</h3>
<p>每个测试用例的输出占一行，按从大到小的顺序输出关键数字。数字间用 1 个空格隔开，但一行中最后一个数字后没有空格。</p>
<h3 id="输入样例：">输入样例：</h3>

```swift
6
3 5 6 7 8 11
```

<h3 id="输出样例：">输出样例：</h3>
 

```swift
7 6
```
---
以前的代码：
```cpp
#include <cstdio>
#include <algorithm>
using namespace std;
bool cmp(int a, int b) { return a > b; }
int hash[10500] = {0};
/* 如果n不能被数列中的其他数字所覆盖, 则n为“关键数”, 即数列中其他数计算时不会计算到它 */
void Callatz(int n) {
    while (n != 1) { //n不等于时循环
        if (n % 2) n = (3 * n + 1) / 2; //是奇数
        else n /= 2;  //是偶数
        hash[n] = 0; //在计算某一个数的卡拉兹数列时被覆盖
    }
}

int main() {
    int K, cnt = 0;
    scanf("%d", &K); 
    int a[K];
    for (int i = 0; i < K; i++) { scanf("%d", &a[i]); hash[a[i]] = 1; }
    sort(a, a + K, cmp); //排序
    for (int i = 0; i < K; i++) Callatz(a[i]); 
    for (int i = 0; i < K; i++) {
        if (hash[a[i]] != 0) { 
            if (cnt == 0) { printf("%d", a[i]); cnt++; }
            else printf(" %d", a[i]);
        }
    }
    return 0;
}
```
现在的代码：
```cpp
#include <bits/stdc++.h>
using namespace std;

int main() {
    int k, n, nums[110];
    scanf("%d", &k);
    bool hash[2500] = {false};
    for (int i = 0; i < k; ++i) {
        scanf("%d", &nums[i]);
        hash[nums[i]] = true;
    }
    vector<int> ans;
    for (int i = 0; i < k; ++i) {
        if (hash[nums[i]]) { //待验证的未被覆盖的正整数 
            int t = nums[i];
            while (t != 1) {
                t = (t & 1) ? (3 * t + 1) / 2 : t / 2;
                hash[t] = false; //t被覆盖
            }
        }
    } 
    for (int i = 100; i >= 2; --i) if (hash[i]) ans.push_back(i); //从大到小收集答案
    for (int i = 0; i < ans.size(); ++i) printf(" %d" + !i, ans[i]);
    return 0;
}
```
