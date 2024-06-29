> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的[仓库](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

<p>Design an iterator that supports the <code>peek</code> operation on an existing iterator in addition to the <code>hasNext</code> and the <code>next</code> operations.</p>

<p>Implement the <code>PeekingIterator</code> class:</p>

<ul>
	<li><code>PeekingIterator(Iterator&lt;int&gt; nums)</code> Initializes the object with the given integer iterator <code>iterator</code>.</li>
	<li><code>int next()</code> Returns the next element in the array and moves the pointer to the next element.</li>
	<li><code>boolean hasNext()</code> Returns <code>true</code> if there are still elements in the array.</li>
	<li><code>int peek()</code> Returns the next element in the array <strong>without</strong> moving the pointer.</li>
</ul>

<p><strong>Note:</strong> Each language may have a different implementation of the constructor and <code>Iterator</code>, but they all support the <code>int next()</code> and <code>boolean hasNext()</code> functions.</p>
 
<p><strong>Example 1:</strong></p>

```cpp
Input
["PeekingIterator", "next", "peek", "next", "next", "hasNext"]
[[[1, 2, 3]], [], [], [], [], []]
Output
[null, 1, 2, 2, 3, false]

Explanation
PeekingIterator peekingIterator = new PeekingIterator([1, 2, 3]); // [1,2,3]
peekingIterator.next();    // return 1, the pointer moves to the next element [1,2,3].
peekingIterator.peek();    // return 2, the pointer does not move [1,2,3].
peekingIterator.next();    // return 2, the pointer moves to the next element [1,2,3]
peekingIterator.next();    // return 3, the pointer moves to the next element [1,2,3]
peekingIterator.hasNext(); // return False
```

 
<p><strong>Constraints:</strong></p>

<ul>
	<li><code>1 &lt;= nums.length &lt;= 1000</code></li>
	<li><code>1 &lt;= nums[i] &lt;= 1000</code></li>
	<li>All the calls to <code>next</code> and <code>peek</code> are valid.</li>
	<li>At most <code>1000</code> calls will be made to <code>next</code>, <code>hasNext</code>, and <code>peek</code>.</li>
</ul>
 
<strong>Follow up:</strong> How would you extend your design to be generic and work with all types, not just integer?

题意：设计一个迭代器 <code>PeekingIterator</code> 类，在集成现有迭代器拥有的&nbsp;<code>hasNext</code> 和 <code>next</code> 操作的基础上，还额外支持 <code>peek</code> 操作： 
<ul>
	<li><code>PeekingIterator(Iterator&lt;int&gt; nums)</code> 使用指定整数迭代器&nbsp;<code>nums</code> 初始化迭代器。</li>
	<li><code>int next()</code> 返回数组中的下一个元素，并将指针移动到下个元素处。</li>
	<li><code>bool hasNext()</code> 如果数组中存在下一个元素，返回 <code>true</code> ；否则，返回 <code>false</code> 。</li>
	<li><code>int peek()</code> 返回数组中的下一个元素，但 <strong>不</strong> 移动指针。</li>
</ul>
 
 
 进阶：如何拓展你的设计，使之变得通用化，从而适应所有的类型，而不只是整数型？
 
---
### 解法 设计+缓存
常见的迭代器 `Iterator`（或者说设计模式）只支持两种操作：
- `hasNext()` 操作：如果存在下一元素，返回 `true` ，否则返回 `false`  。具体实现上，就是判断「**游标是否到达结束位置**」；
- `next()` 操作：返回下一元素，当不存在下一元素时返回 `null` 。具体实现上，就是「**返回游标指向的元素且让游标后移**」。

本题中，还需要我们额外支持 `peek()` 操作，即「**在不移动游标的前提下，返回游标指向的元素**」。要做到这一点，关键在于「**缓存下一个元素**」。

即使了解了关键点，具体实现上也可以有不同。不仅是语言上的差异——C++通过继承 `Iterator` 类，实现 `PeekingIterator` ；Java通过实现 `Iterator` 接口，实现 `PeekingIterator` ……还有实际操作顺序的不同。

一种干净利落的做法如下，它与[Google's guava library source code](https://github.com/google/guava/blob/703ef758b8621cfbab16814f01ddcc5324bdea33/guava-gwt/src-super/com/google/common/collect/super/com/google/common/collect/Iterators.java#L1125)完全一致。**设置一个 `hasPeeked` 标志和一个缓存变量 `cache` ，在构造函数中只对其进行初始化**。
- 调用 `peek()` 时先判断 `hasPeeked` 是否为 `false` ，是则先调用父类的 `Iterator::next()` 方法，**缓存下个元素**，并将 `hasPeeked` 设置为 `true` 。最后都返回当前缓存的值 `cache` 。
- 调用 `next()` 时先判断 `hasPeeked` 是否为 `true` ，是则设置 `hasPeeked` 为 `false` ，表示**之前缓存的值失效**，同时返回之前缓存的值；否则直接调用 `Iterator::next()` ，**因为之前没有缓存过值**。
- 调用 `hasNext()` 时，只要 `hasPeeked` 或 `Iterator::next()` 任一为 `true` ，就表示**还有值**，返回 `true` 。

下面是C++代码。不过不一定要使用 `hasPeeked` 标志变量……还可以设置 `int *cache = nullptr` ，区分「**没有缓存值**」和「**已经缓存值**」这两种情况。
```cpp
//C++ version
class PeekingIterator : public Iterator {
    bool hasPeeked;
    int cache; // 第一次peek时缓存迭代的元素
public:
	PeekingIterator(const vector<int>& nums) : Iterator(nums), hasPeeked(false), cache(0) {
	    // Initialize any member here.
	    // **DO NOT** save a copy of nums and manipulate it directly.
	    // You should only use the Iterator interface methods.
	}
	
    // Returns the next element in the iteration without advancing the iterator.
	int peek() {
        if (!hasPeeked) {
            cache = Iterator::next();
            hasPeeked = true;
        }
        return cache;
	}
	
	// hasNext() and next() should behave the same as in the Iterator interface.
	// Override them if needed.
	int next() {
	    if (hasPeeked) {
            hasPeeked = false;
            return cache;
        }
        return Iterator::next();
	}
	
	bool hasNext() const {
	    return hasPeeked || Iterator::hasNext();
	}
};
//执行用时：0 ms, 在所有 C++ 提交中击败了100.00% 的用户
//内存消耗：7.3 MB, 在所有 C++ 提交中击败了58.28% 的用户
```
在进阶要求中，我们会看到另一种实现——即**在构造函数中就先缓存下一元素**。此时代码更加简单：
- 调用 `peek()` 时，由于已经缓存过值，直接返回 `cache` ；
- 调用 `next()` 时，先保存 `cache` 到一个临时变量，再缓存下一个值到 `cache` 中，没有下个值时 `cache = null` ；
- 调用 `hasNext()` 时，由于已经缓存过值，只需判断 `cache != null` 。

### 进阶要求
受益于Java的「泛型」设计，我们可以轻松支持任意类型，只需将 `Integer` 换成泛型标识符如 `E` 。
```java
// Java Iterator interface reference:
// https://docs.oracle.com/javase/8/docs/api/java/util/Iterator.html

class PeekingIterator implements Iterator<E> {
    private Iterator<Integer> iterator;
    private E cache = null;
	public PeekingIterator(Iterator<E> iterator) {
	    // initialize any member here.
	    this.iterator = iterator;
        if (iterator.hasNext())
            cache = iterator.next(); // 提前缓存
	}
	
    // Returns the next element in the iteration without advancing the iterator.
	public E peek() { // 已经缓存过值
        return cache;
	}
	
	// hasNext() and next() should behave the same as in the Iterator interface.
	// Override them if needed.
	@Override
	public E next() {
	    E ans = cache; // 返回已经缓存的值
        if (iterator.hasNext()) // 如果还有值,就再缓存下一个值
            cache = iterator.next();
        else
            cache = null;
        return ans;
	}
	
	@Override
	public boolean hasNext() {
	    return cache != null; // 由于提前缓存了值,只需判断cache是否为null
	}
}
```
Java泛型的实现原理是「**擦除法**」——即实际以 `Object` 顶层类型来存储，只是在编译期会自动增加强制类型转换的代码，（在增加了强制类型转换后）泛型信息就不被需要，（在编译过后）泛型信息被直接擦除、不会带到运行时。

对于C这种不支持泛型的语言，可采用类似思路来实现，保存一个 `void *` 的指针，指向要缓存的值，要实际使用时手动强转，再接收进来/返回出去。对于C++，则可使用模板参数等诸多手段。
