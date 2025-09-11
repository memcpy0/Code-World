<p>
Given a start IP address <code>ip</code> and a number of ips we need to cover <code>n</code>, return a representation of the range as a list (of smallest possible length) of CIDR blocks.
</p><p>
A CIDR block is a string consisting of an IP, followed by a slash, and then the prefix length.  For example: "123.45.67.89/20".  That prefix length "20" represents the number of common prefix bits in the specified range.
</p>

**Example 1:** 

```css
Input: ip = "255.0.0.7", n = 10
Output: ["255.0.0.7/32","255.0.0.8/29","255.0.0.16/32"]
Explanation:
The initial ip address, when converted to binary, looks like this (spaces added for clarity):
255.0.0.7 -> 11111111 00000000 00000000 00000111
The address "255.0.0.7/32" specifies all addresses with a common prefix of 32 bits to the given address,
ie. just this one address.

The address "255.0.0.8/29" specifies all addresses with a common prefix of 29 bits to the given address:
255.0.0.8 -> 11111111 00000000 00000000 00001000
Addresses with common prefix of 29 bits are:
11111111 00000000 00000000 00001000
11111111 00000000 00000000 00001001
11111111 00000000 00000000 00001010
11111111 00000000 00000000 00001011
11111111 00000000 00000000 00001100
11111111 00000000 00000000 00001101
11111111 00000000 00000000 00001110
11111111 00000000 00000000 00001111

The address "255.0.0.16/32" specifies all addresses with a common prefix of 32 bits to the given address,
ie. just 11111111 00000000 00000000 00010000.

In total, the answer specifies the range of 10 ips starting with the address 255.0.0.7 .

There were other representations, such as:
["255.0.0.7/32","255.0.0.8/30", "255.0.0.12/30", "255.0.0.16/32"],
but our answer was the shortest possible.

Also note that a representation beginning with say, "255.0.0.7/30" would be incorrect,
because it includes addresses like 255.0.0.4 = 11111111 00000000 00000000 00000100 
that are outside the specified range.
```
<b>Note:</b><br>
 <ol>
<li><code>ip</code> will be a valid IPv4 address.</li>
<li>Every implied address <code>ip + x</code> (for <code>x &lt; n</code>) will be a valid IPv4 address.</li>
<li><code>n</code> will be an integer in the range <code>[1, 1000]</code>.</li>
</ol> 

题意：给定一个起始IP地址 `ip` 和一个我们需要包含的IP的数量 `n`，返回用列表（最小可能的长度）表示的 `CIDR` 块的范围。 

`CIDR` 块是包含IP的字符串，后接斜杠和固定长度。例如：`123.45.67.89/20` 。固定长度 `20` 表示在特定的范围中公共前缀位的长度。
 
