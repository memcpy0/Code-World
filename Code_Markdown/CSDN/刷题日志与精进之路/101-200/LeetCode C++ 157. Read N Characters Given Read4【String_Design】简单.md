<p>Given a file and assume that you can only read the file using a given method&nbsp;<code>read4</code>, implement a method to read <em>n</em> characters.</p>

<p><b>Method read4: </b></p>

<p>The API&nbsp;<code>read4</code> reads 4 consecutive characters from the file, then writes those characters into the buffer array <code>buf4</code>.</p>

<p>The return value is the number of actual characters read.</p>

<p>Note that&nbsp;<code>read4()</code> has its own file pointer, much like <code>FILE *fp</code> in C.</p>

<p><b>Definition of read4:</b></p>

```haskell
Parameter:  char[] buf4
Returns:    int

Note: buf4[] is destination not source, the results from read4 will be copied to buf4[]
```

<p>Below is a high level example of how <code>read4</code> works:</p>

<p><img style="width: 600px; height: 403px;" src="https://assets.leetcode.com/uploads/2020/07/01/157_example.png" alt=""></p>

```haskell
File file("abcde"); // File is "abcde", initially file pointer (fp) points to 'a'
char[] buf4 = new char[4]; // Create buffer with enough space to store characters
read4(buf4); // read4 returns 4. Now buf4 = "abcd", fp points to 'e'
read4(buf4); // read4 returns 1. Now buf4 = "e", fp points to end of file
read4(buf4); // read4 returns 0. Now buf4 = "", fp points to end of file
```
<p><strong>Method read:</strong></p>

<p>By using the <code>read4</code> method, implement the method&nbsp;<code>read</code> that reads <i>n</i> characters from the file and store it in the&nbsp;buffer array&nbsp;<code>buf</code>. Consider that you <strong>cannot</strong> manipulate the file directly.</p>

<p>The return value is the number of actual characters read.</p>

<p><b>Definition of read: </b></p>

```haskell
Parameters:	char[] buf, int n
Returns:	int

Note: buf[] is destination not source, you will need to write the results to buf[] 
```

<p><strong>Example 1:</strong></p>

```haskell
Input: file = "abc", n = 4
Output: 3
Explanation: After calling your read method, buf should contain "abc". We read 
a total of 3 characters from the file, so return 3. Note that "abc" is the file's 
content, not buf. buf is the destination buffer that you will have to write the results to.
```

 
<p><strong>Example 2:</strong></p>

```haskell
Input: file = "abcde", n = 5
Output: 5
Explanation: After calling your read method, buf should contain "abcde". 
We read a total of 5 characters from the file, so return 5.
```

<p><strong>Example 3:</strong></p>

```haskell
Input: file = "abcdABCD1234", n = 12
Output: 12
Explanation: After calling your read method, buf should contain "abcdABCD1234". 
We read a total of 12 characters from the file, so return 12.
```

 
<p><strong>Example 4:</strong></p>

```haskell
Input: file = "leetcode", n = 5
Output: 5
Explanation: After calling your read method, buf should contain "leetc". 
We read a total of 5 characters from the file, so return 5. 
```

 
<p><strong>Note:</strong></p>

<ul>
	<li>Consider that you <strong>cannot</strong> manipulate the file directly, the file is only accesible for <code>read4</code> but&nbsp;<strong>not</strong> for <code>read</code>.</li>
	<li>The <code>read</code> function will only be called once for each test case.</li>
	<li>You may assume the destination buffer array,&nbsp;<code>buf</code>,&nbsp;is guaranteed to have enough&nbsp;space for storing&nbsp;<em>n</em>&nbsp;characters.</li>
</ul>

题意：给出一个文件，并且该文件只能通过给定的 `read4` 方法来读取，请实现一个方法使其能够读取 `n` 个字符到 `buf` 数组中。

---
### 解法 读懂题意
这道题的关键是读懂题意。题目要求我们设计一个函数 `read(char *buf, int n)` ，从文件中读取 `n` 个字符到 `buf` 字符数组中。注意，我们不能直接操作文件，而是必须通过 `read4(char *buf)` 来间接读取文件，`read4` 总是试图读取文件中的 `4` 个连续字符并且写到 `buf` 字符数组中，此时返回 `4` ；不足 `4` 个字符则读取到文件末尾，返回实际读取的字符个数。

因此，我们应该保证 `read` 不会写入 `buf` 超过 `n` 个字符。同时，如果某次读取已经 `EOF(End Of File)` 就应该结束读取，减少IO消耗。代码的整体逻辑如下：
- 如果文件大小 `< n` ，则读取整个文件，返回值为文件大小，此时会遇到 `EOF`；
- 否则返回前 `n` 个字符，返回值为 `n` 。

具体代码如下：
```cpp
/**
 * The read4 API is defined in the parent class Reader4.
 *     int read4(char *buf4);
 */

class Solution {
public:
    /**
     * @param buf Destination buffer
     * @param n   Number of characters to read
     * @return    The number of actual characters read
     */
    int read(char *buf, int n) {
        int needBytes = n, curIdx = 0;
        char hideBuf[4];
        bool endOfFile = false;
        while (true) {
            int bytes = read4(hideBuf); //读取4个连续的字符,返回实际读取的字符个数
            endOfFile = bytes < 4; //此时到达文件末尾,文件大小<n
            int idx = 0;
            while (needBytes > 0 && idx < bytes) { //写入到buf中
                buf[curIdx++] = hideBuf[idx++];
                --needBytes;
            }
            if (endOfFile || needBytes == 0) break;
        }
        return curIdx;
    }
};
```
运行效率如下：
```cpp
执行用时：0 ms, 在所有 C++ 提交中击败了100.00% 的用户
内存消耗：6.4 MB, 在所有 C++ 提交中击败了30.49% 的用户
```
更简单的写法如下：
```cpp
class Solution {
public:
    /**
     * @param buf Destination buffer
     * @param n   Number of characters to read
     * @return    The number of actual characters read
     */
    int read(char *buf, int n) {
        int res = 0;
        for (int i = 1; i <= n; i += 4) {
            res += read4(buf);
            buf += 4;
        }
        return n < res ? n : res;
    }
};
```
