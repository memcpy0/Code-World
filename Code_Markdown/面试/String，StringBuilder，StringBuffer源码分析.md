**TODO：Arrays.sort底层TimSort！**
**TODO：String编码的问题**

**聊的是八股的文，干的是搬砖的活！**

面我的题开发都用不到，你为什么要问？可能这是大部分程序员求职时的经历，甚至也是大家讨厌和烦躁的点。明明给的是拧螺丝的钱、明明做的是写 CRUD 的事、明明担的是成工具的人！

明明... 有很多，可明明公司不会招 5 年开发做 3 年经验的事、明明公司也更喜
欢具有附加价值的研发。有些小公司不好说，但在一些互联网大厂中，我们都希望招聘到具有培养价值的，也更喜欢能快速打怪升级的，也更愿意让这样的人承担更大的职责。

但，你酸了！ 别人看源码你打游戏、别人学算法你刷某音、别人写博客你浪 98。所以，没有把时间用到个人成长上，就一直会被别人榨取。

# 1. 面试题
谢飞机，总感觉自己有技术瓶颈、有知识盲区，但是又不知道在哪。所以约面试官聊天，虽然也面不过去！
面试官：飞机，你又抱着大脸，来白嫖我了啦？
谢飞机：嘿嘿，我需要知识，我渴。
面试官：好，那今天聊聊最常用的 String 吧，你怎么初始化一个字符串类型。
谢飞机：String str = "abc";
面试官：还有吗？
谢飞机：还有？啊，这样 String str = new String("abc"); 😄
面试官：还有吗？
谢飞机：啊！？还有！不知道了！
面试官：你不懂 String，你没看过源码。还可以这样；new String(new char[]{'c','d'}); 回家再学学吧，下次记得给我买百事，我不喝可口。

# 2. StringBuilder 比 String 快吗？ 
老子代码一把梭，总有人絮叨这么搞不好，那 StringBuilder 到底那快了！
## 测试String, StringBuilder, StringBuffer
```java
@Test  
public void test_03() {  
    long startTime = System.currentTimeMillis();  
    String str = "";  
    for (int i = 0; i < 1000000; i++) {  
        str += i;  
    }  
    System.out.println("String 耗时：" + (System.currentTimeMillis() - startTime) + "毫秒");  
    // 100  1毫秒  
    // 1000  6毫秒  
    // 10000  389毫秒  
    // 50000  7267毫秒  
    // 10万 String 耗时：29350毫秒  
}
  
@Test  
public void test_04() {  
    long startTime = System.currentTimeMillis();  
    StringBuilder str = new StringBuilder();  
    for (int i = 0; i < 1000000; i++) {  
        str.append(i);  
    }  
    System.out.println("StringBuilder 耗时" + (System.currentTimeMillis() - startTime) + "毫秒");  
    // 100  0毫秒  
    // 1000  1毫秒  
    // 10000 2毫秒  
    // 50000 6毫秒  
    // 10万 StringBuilder 耗时12毫秒  
}  
  
@Test  
public void test_05() {  
    long startTime = System.currentTimeMillis();  
    StringBuffer str = new StringBuffer();  
    for (int i = 0; i < 1000000; i++) {  
        str.append(i);  
    }  
    System.out.println("StringBuffer 耗时" + (System.currentTimeMillis() - startTime) + "毫秒");  
    // 100  0毫秒  
    // 1000  1毫秒  
    // 10000 2毫秒  
    // 50000 6毫秒  
    // 10万 StringBuffer 耗时13毫秒  
}
```
综上，分别使用了 String、StringBuilder、StringBuffer，做字符串链接操作
(100 个、1000 个、1 万个、10 万个、100 万个)，记录每种方式的耗时。最终汇总图表如下；
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202305042308263.png)
从上图可以得出以下结论；
1. **String 字符串链接是耗时的**，尤其数据量大的时候，简直没法使用了。这是做实验，基本也不会有人这么干！
2. StringBuilder、StringBuffer，因为没有发生多线程竞争也就没有🔒锁升级，所以两个类耗时几乎相同，当然**在单线程下更推荐使用StringBuilder** 。

## StringBuilder 比 String 快， 为什么？
```java
String str = "";
for (int i = 0; i < 10000; i++) {
	str += i;
}
```
这段代码就是三种字符串拼接方式，最慢的一种。不是说这种+加的符号，会被
优化成 StringBuilder 吗，那怎么还慢？

**确实会被 JVM 编译期优化**，但优化成什么样子了呢，先看下字节码指令；
```bash
javap -c ApiTest.class
```
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202305042312704.png)
一看指令码，这不是在循环里 `if_icmpge` 给我 `new` 了 `StringBuilder` 了吗，
怎么还这么慢呢？再仔细看，其实你会发现，这 new 是在循环里吗呀，我们把这段代码写出来再看看；
```java
String str = "";
for (int i = 0; i < 10000; i++) {
	str = new StringBuilder().append(str).append(i).toString();
}
```
现在再看这段代码就很清晰了，**所有的字符串链接操作，都需要实例化一次
StringBuilder**，所以非常耗时。并且你可以验证，这样写代码耗时与字符串直接链接是一样的。 所以**把 StringBuilder 提到上一层 for 循环外更快**。

# 3. String源码分析
```java
public final class String  
    implements java.io.Serializable, Comparable<String>, CharSequence, Constable, ConstantDesc {  
  
    /**  
     * The value is used for character storage.     
     *     
     * @implNote This field is trusted by the VM, and is a subject to  
     * constant folding if String instance is constant. Overwriting this     
     * field after construction will cause problems.     
     *     
     * Additionally, it is marked with {@link Stable} to trust the contents  
     * of the array. No other facility in JDK provides this functionality (yet).     
     * {@link Stable} is safe here, because value is never null.  
     */    
    @Stable  
    private final byte[] value;  
  
    /**  
     * The identifier of the encoding used to encode the bytes in     * {@code value}. The supported values in this implementation are  
     *     
     * LATIN1     
     * UTF16     
     *     
     * @implNote This field is trusted by the VM, and is a subject to  
     * constant folding if String instance is constant. Overwriting this     
     * field after construction will cause problems.     
     */    
    private final byte coder;  
  
    /** Cache the hash code for the string */  
    private int hash; // Default to 0  
  
    /**  
     * Cache if the hash has been calculated as actually being zero, enabling     
     * us to avoid recalculating this.     
     */    
    private boolean hashIsZero; // Default to false;
```
## 3.1 初始化
在与 谢飞机 的面试题中，我们聊到了 String 初始化的问题，按照一般我们应
用的频次上，能想到的只有直接赋值，String str = "abc";，但因为 String 的
底层数据结构是数组 char value[]，所以它的初始化方式也会有很多跟数组相关
的，如下；
```java
String str_01 = "abc";
System.out.println("默认方式：" + str_01);
String str_02 = new String(new char[]{'a', 'b', 'c'});
System.out.println("char 方式：" + str_02);

String str_03 = new String(new int[]{0x61, 0x62, 0x63}, 0, 3);
System.out.println("int 方式：" + str_03);
String str_04 = new String(new byte[]{0x61, 0x62, 0x63});
System.out.println("byte 方式：" + str_04);
```
以上这些方式都可以初始化，并且最终的结果是一致的，abc。
### 空构造函数
```java
public String() {  
    this.value = "".value;  
    this.coder = "".coder;  
}
```
### 字符串构造
```java
@IntrinsicCandidate  
public String(String original) {  
    this.value = original.value;  
    this.coder = original.coder;  
    this.hash = original.hash;  
    this.hashIsZero = original.hashIsZero;  
}
```
### char数组 StringUTF16
COMPACT_STRINGS表示是否压缩存储：
- 为false，则bytes数组value中的bytes会被编码为UTF-16格式；
- 在性能敏感的地方，在相遇对coder编码器字段检查前，先要对COMPACT_STRINGS显式检查，如 `if (COMPACT_STRINGS && coder == LATIN1)`
```java
String(char[] value, int off, int len, Void sig) {  
    if (len == 0) {  
        this.value = "".value;  
        this.coder = "".coder;  
        return;    
    }  
    if (COMPACT_STRINGS) {  
        byte[] val = StringUTF16.compress(value, off, len);  
        if (val != null) {  
            this.value = val;  
            this.coder = LATIN1;  
            return;        
        }  
    }  
    this.coder = UTF16;  
    this.value = StringUTF16.toBytes(value, off, len);  
}
// 相关的压缩方法
public static byte[] compress(char[] val, int off, int len) {  
    byte[] ret = new byte[len];  
    if (compress(val, off, ret, 0, len) == len) {  
        return ret;  
    }  
    return null;  
}
// compressedCopy char[] -> byte[]  
@IntrinsicCandidate  
public static int compress(char[] src, int srcOff, byte[] dst, int dstOff, int len) {  
    for (int i = 0; i < len; i++) {
        char c = src[srcOff];  // 从srcOff开始，一个char(16位)
        if (c > 0xFF) { // 大于0xFF，不是ASCII码表中的
            len = 0;  // 就返回？
            break;        
        }  
        dst[dstOff] = (byte)c;  // 
        srcOff++;  
        dstOff++;  
    }  
    return len;  
}
```
上面的方法就是，**对于 `<= 0xFF` 的字符char，不让它存两个字节，只让他在byte数组中占一个格子**。能压缩存储的话就压缩，否则返回null

如果COMPACT_STRINGS为false，则最终调用下面的方法，将其转换为byte数组：即每次将当前位置 $i$ 的字符 `value[off]` 放到byte数组val的对应位置 `i * 2` ，`value[off]` 被存为两个字节
- 如果是大端序，则先存高8位比特，再存低8位；xy -> x y
- 如果是小端序，则相反 xy -> y x
```java
@IntrinsicCandidate  
public static byte[] toBytes(char[] value, int off, int len) {  
    byte[] val = newBytesFor(len);  
    for (int i = 0; i < len; i++) {  
        putChar(val, i, value[off]);  
        off++;  
    }  
    return val;  
}
@IntrinsicCandidate  
// intrinsic performs no bounds checks  
static void putChar(byte[] val, int index, int c) {  
    assert index >= 0 && index < length(val) : "Trusted caller missed bounds check";  
    index <<= 1;  
    val[index++] = (byte)(c >> HI_BYTE_SHIFT);  
    val[index]   = (byte)(c >> LO_BYTE_SHIFT);  
}

static final int HI_BYTE_SHIFT;  
static final int LO_BYTE_SHIFT;  
static {  
    if (isBigEndian()) {  
        HI_BYTE_SHIFT = 8;  
        LO_BYTE_SHIFT = 0;  
    } else {  
        HI_BYTE_SHIFT = 0;  
        LO_BYTE_SHIFT = 8;  
    }  
}
```
### byte数组
如果是byte数组，且COMPACT_STRINGS为true，则直接Arrays.copyOfRange；否则存储为UTF16（每个hibyte byte16位占两个byte数组的格子）：
```java
@Deprecated(since="1.1")  
public String(byte[] ascii, int hibyte, int offset, int count) {  
    checkBoundsOffCount(offset, count, ascii.length);  
    if (count == 0) {  
        this.value = "".value;  
        this.coder = "".coder;  
        return;    }  
    if (COMPACT_STRINGS && (byte)hibyte == 0) {  
        this.value = Arrays.copyOfRange(ascii, offset, offset + count);  
        this.coder = LATIN1;  
    } else {  
        hibyte <<= 8;  
        byte[] val = StringUTF16.newBytesFor(count);  
        for (int i = 0; i < count; i++) {  
            StringUTF16.putChar(val, i, hibyte | (ascii[offset++] & 0xff));  
        }  
        this.value = val;  
        this.coder = UTF16;  
    }  
}
```
### byte数组+指定编码 TODO
```java
/**  
 * Constructs a new {@code String} by decoding the specified subarray of  
 * bytes using the specified {@linkplain java.nio.charset.Charset charset}.  
 * The length of the new {@code String} is a function of the charset, and  
 * hence may not be equal to the length of the subarray. * * <p> This method always replaces malformed-input and unmappable-character  
 * sequences with this charset's default replacement string.  The {@link  
 * java.nio.charset.CharsetDecoder} class should be used when more control  
 * over the decoding process is required. * * @param  bytes  
 *         The bytes to be decoded into characters  
 * * @param  offset  
 *         The index of the first byte to decode  
 * * @param  length  
 *         The number of bytes to decode  
 * * @param  charset  
 *         The {@linkplain java.nio.charset.Charset charset} to be used to  
 *         decode the {@code bytes}  
 * * @throws  IndexOutOfBoundsException  
 *          If {@code offset} is negative, {@code length} is negative, or  
 *          {@code offset} is greater than {@code bytes.length - length}  
 * * @since  1.6  
 */@SuppressWarnings("removal")  
public String(byte[] bytes, int offset, int length, Charset charset) {  
    Objects.requireNonNull(charset);  
    checkBoundsOffCount(offset, length, bytes.length);  
    if (length == 0) {  
        this.value = "".value;  
        this.coder = "".coder;  
    } else if (charset == UTF_8.INSTANCE) {  
        if (COMPACT_STRINGS && !StringCoding.hasNegatives(bytes, offset, length)) {  
            this.value = Arrays.copyOfRange(bytes, offset, offset + length);  
            this.coder = LATIN1;  
        } else {  
            int sl = offset + length;  
            int dp = 0;  
            byte[] dst = null;  
            if (COMPACT_STRINGS) {  
                dst = new byte[length];  
                while (offset < sl) {  
                    int b1 = bytes[offset];  
                    if (b1 >= 0) {  
                        dst[dp++] = (byte)b1;  
                        offset++;  
                        continue;                    }  
                    if ((b1 == (byte)0xc2 || b1 == (byte)0xc3) &&  
                            offset + 1 < sl) {  
                        int b2 = bytes[offset + 1];  
                        if (!isNotContinuation(b2)) {  
                            dst[dp++] = (byte)decode2(b1, b2);  
                            offset += 2;  
                            continue;                        }  
                    }  
                    // anything not a latin1, including the repl  
                    // we have to go with the utf16                    break;  
                }  
                if (offset == sl) {  
                    if (dp != dst.length) {  
                        dst = Arrays.copyOf(dst, dp);  
                    }  
                    this.value = dst;  
                    this.coder = LATIN1;  
                    return;                }  
            }  
            if (dp == 0 || dst == null) {  
                dst = new byte[length << 1];  
            } else {  
                byte[] buf = new byte[length << 1];  
                StringLatin1.inflate(dst, 0, buf, 0, dp);  
                dst = buf;  
            }  
            dp = decodeUTF8_UTF16(bytes, offset, sl, dst, dp, true);  
            if (dp != length) {  
                dst = Arrays.copyOf(dst, dp << 1);  
            }  
            this.value = dst;  
            this.coder = UTF16;  
        }  
    } else if (charset == ISO_8859_1.INSTANCE) {  
        if (COMPACT_STRINGS) {  
            this.value = Arrays.copyOfRange(bytes, offset, offset + length);  
            this.coder = LATIN1;  
        } else {  
            this.value = StringLatin1.inflate(bytes, offset, length);  
            this.coder = UTF16;  
        }  
    } else if (charset == US_ASCII.INSTANCE) {  
        if (COMPACT_STRINGS && !StringCoding.hasNegatives(bytes, offset, length)) {  
            this.value = Arrays.copyOfRange(bytes, offset, offset + length);  
            this.coder = LATIN1;  
        } else {  
            byte[] dst = new byte[length << 1];  
            int dp = 0;  
            while (dp < length) {  
                int b = bytes[offset++];  
                StringUTF16.putChar(dst, dp++, (b >= 0) ? (char) b : REPL);  
            }  
            this.value = dst;  
            this.coder = UTF16;  
        }  
    } else {  
        // (1)We never cache the "external" cs, the only benefit of creating  
        // an additional StringDe/Encoder object to wrap it is to share the        // de/encode() method. These SD/E objects are short-lived, the young-gen        // gc should be able to take care of them well. But the best approach        // is still not to generate them if not really necessary.        // (2)The defensive copy of the input byte/char[] has a big performance        // impact, as well as the outgoing result byte/char[]. Need to do the        // optimization check of (sm==null && classLoader0==null) for both.        CharsetDecoder cd = charset.newDecoder();  
        // ArrayDecoder fastpaths  
        if (cd instanceof ArrayDecoder ad) {  
            // ascii  
            if (ad.isASCIICompatible() && !StringCoding.hasNegatives(bytes, offset, length)) {  
                if (COMPACT_STRINGS) {  
                    this.value = Arrays.copyOfRange(bytes, offset, offset + length);  
                    this.coder = LATIN1;  
                    return;                }  
                this.value = StringLatin1.inflate(bytes, offset, length);  
                this.coder = UTF16;  
                return;            }  
  
            // fastpath for always Latin1 decodable single byte  
            if (COMPACT_STRINGS && ad.isLatin1Decodable()) {  
                byte[] dst = new byte[length];  
                ad.decodeToLatin1(bytes, offset, length, dst);  
                this.value = dst;  
                this.coder = LATIN1;  
                return;            }  
  
            int en = scale(length, cd.maxCharsPerByte());  
            cd.onMalformedInput(CodingErrorAction.REPLACE)  
                    .onUnmappableCharacter(CodingErrorAction.REPLACE);  
            char[] ca = new char[en];  
            int clen = ad.decode(bytes, offset, length, ca);  
            if (COMPACT_STRINGS) {  
                byte[] bs = StringUTF16.compress(ca, 0, clen);  
                if (bs != null) {  
                    value = bs;  
                    coder = LATIN1;  
                    return;                }  
            }  
            coder = UTF16;  
            value = StringUTF16.toBytes(ca, 0, clen);  
            return;        }  
  
        // decode using CharsetDecoder  
        int en = scale(length, cd.maxCharsPerByte());  
        cd.onMalformedInput(CodingErrorAction.REPLACE)  
                .onUnmappableCharacter(CodingErrorAction.REPLACE);  
        char[] ca = new char[en];  
        if (charset.getClass().getClassLoader0() != null &&  
                System.getSecurityManager() != null) {  
            bytes = Arrays.copyOfRange(bytes, offset, offset + length);  
            offset = 0;  
        }  
  
        int caLen = decodeWithDecoder(cd, ca, bytes, offset, length);  
        if (COMPACT_STRINGS) {  
            byte[] bs = StringUTF16.compress(ca, 0, caLen);  
            if (bs != null) {  
                value = bs;  
                coder = LATIN1;  
                return;            }  
        }  
        coder = UTF16;  
        value = StringUTF16.toBytes(ca, 0, caLen);  
    }  
}
```

### int数组 StringUTF16, StringLatin1
需要转换为char数组
```java
public String(int[] codePoints, int offset, int count) {  
    checkBoundsOffCount(offset, count, codePoints.length);  
    if (count == 0) {  
        this.value = "".value;  
        this.coder = "".coder;  
        return;    
    }  
    if (COMPACT_STRINGS) {  
        byte[] val = StringLatin1.toBytes(codePoints, offset, count);  
        if (val != null) {  
            this.coder = LATIN1;  
            this.value = val;  
            return;        
        }  
    }  
    this.coder = UTF16;  
    this.value = StringUTF16.toBytes(codePoints, offset, count);  
}
```
能压缩存储的话就要用StringLatin1.toBytes方法，：
```java
public static byte[] toBytes(int[] val, int off, int len) {  
    byte[] ret = new byte[len];  
    for (int i = 0; i < len; i++) {  
        int cp = val[off++];  
        if (!canEncode(cp)) {  
            return null;  
        }  
        ret[i] = (byte)cp;  
    }  
    return ret;  
}
```
不能压缩存储的话，调用StringUTF16.toBytes将其转换为byte数组：
- 先准确计算需要多少个char，Basic Multilingual Plane (BMP)平面上的字符需要一个char，还是有效码点的话，就需要第2个char
- 然后分配2n大小的byte数组
- 接着遍历每个int字符，看情况调用putChar，要么只用put一个Char，要么要先将高位char放进去、再放低位char
```java
public static byte[] toBytes(int[] val, int index, int len) {  
    final int end = index + len;  
    // Pass 1: Compute precise size of char[]  
    int n = len;  
    for (int i = index; i < end; i++) {  
        int cp = val[i];  
        if (Character.isBmpCodePoint(cp))  
            continue;  
        else if (Character.isValidCodePoint(cp))  
            n++;  
        else throw new IllegalArgumentException(Integer.toString(cp));  
    }  
    // Pass 2: Allocate and fill in <high, low> pair  
    byte[] buf = newBytesFor(n);  
    for (int i = index, j = 0; i < end; i++, j++) {  
        int cp = val[i];  
        if (Character.isBmpCodePoint(cp)) {  
            putChar(buf, j, cp);  
        } else {  
            putChar(buf, j++, Character.highSurrogate(cp));  
            putChar(buf, j, Character.lowSurrogate(cp));  
        }  
    }  
    return buf;  
}
```


## 3.2 charAt
如果说初始化的方式没用让你感受到它是数据结构，那么 str_01.charAt(0);呢，只要你往源码里一点，就会发现它是 O(1) 的时间复杂度从数组中获取元素，所以效率也是非常高，源码如下； 
```java
/**  
 * Returns the {@code char} value at the  
 * specified index. An index ranges from {@code 0} to  
 * {@code length() - 1}. The first {@code char} value of the sequence  
 * is at index {@code 0}, the next at index {@code 1},  
 * and so on, as for array indexing. * 
 * <p>If the {@code char} value specified by the index is a  
 * <a href="Character.html#unicode">surrogate</a>, the surrogate  
 * value is returned. * 
 * @param      index   the index of the {@code char} value.  
 * @return     the {@code char} value at the specified index of this string.  
 *             The first {@code char} value is at index {@code 0}.  
 * @throws     IndexOutOfBoundsException  if the {@code index}  
 *             argument is negative or not less than the length of this *             string. 
 */
 public char charAt(int index) {  
    if (isLatin1()) {  
        return StringLatin1.charAt(value, index);  
    } else {  
        return StringUTF16.charAt(value, index);  
    }  
}
// 要压缩 & coder=LATIN1表示可以压缩为byte[]
boolean isLatin1() {  
    return COMPACT_STRINGS && coder == LATIN1;  
}

// 是Latin1就返回(char)
final class StringLatin1 {  
  
    public static char charAt(byte[] value, int index) {  
        checkIndex(index, value.length);  
        return (char)(value[index] & 0xff);  
    }
...
final class StringUTF16 {
	public static char charAt(byte[] value, int index) {  
	    checkIndex(index, value);  
	    return getChar(value, index);  
	}
	@IntrinsicCandidate  
	// intrinsic performs no bounds checks  
	static char getChar(byte[] val, int index) {  
	    assert index >= 0 && index < length(val) : "Trusted caller missed bounds check";  
	    index <<= 1;  
	    return (char)(((val[index++] & 0xff) << HI_BYTE_SHIFT) |  
	                  ((val[index]   & 0xff) << LO_BYTE_SHIFT));  
	}
```
如果是LATIN1，则直接索引byte数组的index位置，转为char。否则，将 `index * 2` 的byte作为高位、`index * 2 + 1` 的byte作为低位，合并为一个char返回。
## 3.3 不可变(final)
字符串创建后是不可变的，你看到的+加号连接操作，都是创建了新的对象把数据存放过去，通过源码就可以看到；
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202305050006282.png)
从源码中可以看到，String 的类和用于存放字符串的方法都用了 final 修饰，
也就是创建了以后，这些都是不可变的。

**举个例子**
```java
String str_01 = "abc";
String str_02 = "abc" + "def";
String str_03 = str_01 + "def";
```
不考虑其他情况，对于程序初始化。以上这些代码 str_01、str_02、str_03，都会初始化几个对象呢？其实这个初始化几个对象从侧面就是反应对象是否可变
性。

接下来我们把上面代码反编译，通过指令码看到底创建了几个对象。
```java
  public void test_00();
    Code:
       0: ldc           #2                  // String abc
       2: astore_1
       3: ldc           #3                  // String abcdef
       5: astore_2
       6: new           #4                  // class java/lang/StringBuilder
       9: dup
      10: invokespecial #5                  // Method java/lang/StringBuilder."<init>":()V
      13: aload_1
      14: invokevirtual #6                  // Method java/lang/StringBuilder.append:(Ljava/lang/String;)Ljava/lang/StringBuilder;
      17: ldc           #7                  // String def
      19: invokevirtual #6                  // Method java/lang/StringBuilder.append:(Ljava/lang/String;)Ljava/lang/StringBuilder;
      22: invokevirtual #8                  // Method java/lang/StringBuilder.toString:()Ljava/lang/String;
      25: astore_3
      26: aload_1
      27: invokevirtual #9                  // Method java/lang/String.intern:()Ljava/lang/String;
      30: astore        4
      32: getstatic     #10                 // Field java/lang/System.out:Ljava/io/PrintStream;
      35: aload_1
      36: aload         4
      38: if_acmpne     45
      41: iconst_1
      42: goto          46
      45: iconst_0
      46: invokevirtual #11                 // Method java/io/PrintStream.println:(Z)V
      49: return
```
1. str_01 = "abc"，指令码：0: ldc，创建了一个对象。
2. str_02 = "abc" + "def"，指令码：`3: ldc // String abcdef`，**得益于 JVM 编译期的优化，两个字符串会进行相连，创建一个对象存储**。
3. str_03 = str_01 + "def"，指令码：invokevirtual，这个就不一样了，它需要把两个字符串相连，**会创建 StringBuilder 对象**，直至最后 toString:()操作，**共创建了三个对象**。

所以，我们看到，字符串的创建是不能被修改的，相连操作会创建出新对象。
## 3.4 intern()
### 3.4.1 经典题目
```java
@Test  
public void test_000() {  
    String str_1 = new String("ab");  
    String str_2 = new String("ab");  
    String str_3 = "ab";  
  
    System.out.println(str_1 == str_2); // false  
    System.out.println(str_1 == str_2.intern()); // false  
  
    System.out.println(str_1.equals(str_2)); // true  
    System.out.println(str_1.intern() == str_2.intern()); // true  
  
    System.out.println(str_1 == str_3); // false  
    System.out.println(str_1.intern() == str_3); // true  
    System.out.println(str_2.intern() == str_3); // true  
  
    System.out.println(str_1.intern() == str_3.intern()); // true  
    System.out.println(str_2.intern() == str_3.intern()); // true  
}
```
这是一道经典的 String 字符串面试题，乍一看可能还会有点晕。

### 3.4.2 源码分析
看了答案有点感觉了吗，其实可能你了解方法 intern()，这里先看下它的源码；
```java
/**  
 * Returns a canonical representation for the string object. 
 * <p>  
 * A pool of strings, initially empty, is maintained privately by the  
 * class {@code String}.  
 * <p>  
 * When the intern method is invoked, if the pool already contains a  
 * string equal to this {@code String} object as determined by  
 * the {@link #equals(Object)} method, then the string from the pool is  
 * returned. Otherwise, this {@code String} object is added to the  
 * pool and a reference to this {@code String} object is returned.  
 * <p>  
 * It follows that for any two strings {@code s} and {@code t},  
 * {@code s.intern() == t.intern()} is {@code true}  
 * if and only if {@code s.equals(t)} is {@code true}.  
 * <p>  
 * All literal strings and string-valued constant expressions are  
 * interned. String literals are defined in section {@jls 3.10.5} of the  
 * <cite>The Java Language Specification</cite>.  
 * 
 * @return  a string that has the same contents as this string, but is  
 *          guaranteed to be from a pool of unique strings. 
 **/
public native String intern();
```
`s.equals(t) == true` 当且仅当 `s.intern() == t.intern()`
字面值字符串literal string和string-valued constant expressions被interned

这段代码和注释什么意思呢？
native，说明 intern() 是一个本地方法，底层通过 JNI 调用 C++语言编写的功
能。
```c
\openjdk8\jdk\src\share\native\java\lang\String.c
Java_java_lang_String_intern(JNIEnv *env, jobject this)
{
	return JVM_InternString(env, this);
}

oop result = StringTable::intern(string, CHECK_NULL);

oop StringTable::intern(Handle string_or_null, jchar* name,
int len, TRAPS) {
	unsigned int hashValue = java_lang_String::hash_string(name, len);
	int index = the_table()->hash_to_index(hashValue);
	oop string = the_table()->lookup(index, name, len, hashValue);
	if (string != NULL) return string;
	return the_table()->basic_add(index, string_or_null, name, len,
	hashValue, CHECK_NULL);
}
```
- 代码块有点长这里只截取了部分内容，源码可以学习开源 jdk 代码，连接：
https://codeload.github.com/abhijangda/OpenJDK8/zip/master
- C++这段代码**有点像 HashMap 的哈希桶+链表的数据结构**，用来存放字符串，所以如果哈希值冲突严重，就会导致链表过长。**这在我们讲解 hashMap 中已经介绍，可以回看 HashMap 源码**
- StringTable 是一个固定长度的数组 1009 个大小，jdk1.6 不可调、**jdk1.7 可以设置-XX:StringTableSize，按需调整**。
- 但是一旦缓存池中的字符串达到一定规模后，性能就会急剧下降，因此默认的缓存池大小也在不断的扩大中，从最初的1009到7u40的60013

## 3.4.3 问题图解
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202305050021563.png)
看图说话，如下；
1. 先说 `==` ，**基础类型比对的是值，引用类型比对的是地址**、基础类型与引用类型间对比（如和包装类型），会拆箱了对比。另外，equal 比对的是哈希值。
3. **两个 new 出来的对象，地址肯定不同**，所以是 false。
4. **intern()，直接把值推进了常量池**，所以两个对象都做了 intern() 操作后，比对是常量池里的值。
4. str_3 = "ab"，**赋值，JVM 编译器做了优化，不会重新创建对象，直接引用常量池里的值**。所以 str_1.intern() == str_3，比对结果是 true。

理解了这个结构，根本不需要死记硬背应对面试，让懂了就是真的懂，大脑也会跟着愉悦。
```java
@Test  
public void test_000() {  
    String str_1 = new String("ab");  
    String str_2 = new String("ab");  
    String str_3 = "ab";  
  
    System.out.println(str_1 == str_2); // false  
    System.out.println(str_1 == str_2.intern()); // false  
  
    System.out.println(str_1.equals(str_2)); // true  
    System.out.println(str_1.intern() == str_2.intern()); // true  
  
    System.out.println(str_1 == str_3); // false  
    System.out.println(str_1.intern() == str_3); // true  
    System.out.println(str_2.intern() == str_3); // true  
  
    System.out.println(str_1.intern() == str_3.intern()); // true  
    System.out.println(str_2.intern() == str_3.intern()); // true  
}
```


# 4. StringBuilder源码分析
## 4.1 StringBuilder和StringBuffer初始化
```java
new StringBuilder();
new StringBuilder(16);
new StringBuilder("abc");
```
这几种方式都可以初始化，你可以传一个初始化容量，也可以初始化一个默认的
字符串。它的源码如下；
```java
public StringBuilder() {
	super(16);
}
AbstractStringBuilder(int capacity) {
	value = new char[capacity];
}
```
定睛一看，这就是在初始化数组呀！那是不操作起来跟使用 ArrayList 似的呀！

实际上，==下面StringBuilder和StringBuffer的初始化方法都一模一样，都委托给**共同的抽象父类 `AbstractStringBuilder` 中的构造方法**==：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202305071626848.png)

```java
/**  
 * Constructs a string builder with no characters in it and an * initial capacity of 16 characters. */@IntrinsicCandidate  
public StringBuilder() {  
    super(16);  
}  
  
/**  
 * Constructs a string builder with no characters in it and an * initial capacity specified by the {@code capacity} argument.  
 * * @param      capacity  the initial capacity.  
 * @throws     NegativeArraySizeException  if the {@code capacity}  
 *               argument is less than {@code 0}.  
 */@IntrinsicCandidate  
public StringBuilder(int capacity) {  
    super(capacity);  
}  
  
/**  
 * Constructs a string builder initialized to the contents of the * specified string. The initial capacity of the string builder is * {@code 16} plus the length of the string argument.  
 * * @param   str   the initial contents of the buffer.  
 */@IntrinsicCandidate  
public StringBuilder(String str) {  
    super(str);  
}  
  
/**  
 * Constructs a string builder that contains the same characters * as the specified {@code CharSequence}. The initial capacity of  
 * the string builder is {@code 16} plus the length of the  
 * {@code CharSequence} argument.  
 * * @param      seq   the sequence to copy.  
 */public StringBuilder(CharSequence seq) {  
    super(seq);  
}
```

```java
/**  
 * Constructs a string buffer with no characters in it and an * initial capacity of 16 characters. */@IntrinsicCandidate  
public StringBuffer() {  
    super(16);  
}  
  
/**  
 * Constructs a string buffer with no characters in it and * the specified initial capacity. * * @param      capacity  the initial capacity.  
 * @throws     NegativeArraySizeException  if the {@code capacity}  
 *             argument is less than {@code 0}.  
 */@IntrinsicCandidate  
public StringBuffer(int capacity) {  
    super(capacity);  
}  
  
/**  
 * Constructs a string buffer initialized to the contents of the * specified string. The initial capacity of the string buffer is * {@code 16} plus the length of the string argument.  
 * * @param   str   the initial contents of the buffer.  
 */@IntrinsicCandidate  
public StringBuffer(String str) {  
    super(str);  
}  
  
/**  
 * Constructs a string buffer that contains the same characters * as the specified {@code CharSequence}. The initial capacity of  
 * the string buffer is {@code 16} plus the length of the  
 * {@code CharSequence} argument.  
 * * @param      seq   the sequence to copy.  
 * @since 1.5  
 */public StringBuffer(CharSequence seq) {  
    super(seq);  
}
```

 
### 4.2 添加元素
```java
stringBuilder.append("a");
stringBuilder.append("b");
stringBuilder.append("c");
```
添加元素的操作很简单，使用 append 即可，那么它是怎么往数组中存放的呢，需要扩容吗？我们看源码。
StringBuilder的append方法：
```java
@Override  
public StringBuilder append(Object obj) {  
    return append(String.valueOf(obj));  
}  
  
@Override  
@IntrinsicCandidate  
public StringBuilder append(String str) {  
    super.append(str);  
    return this;}  
  
/**  
 * Appends the specified {@code StringBuffer} to this sequence.  
 * <p>  
 * The characters of the {@code StringBuffer} argument are appended,  
 * in order, to this sequence, increasing the * length of this sequence by the length of the argument. * If {@code sb} is {@code null}, then the four characters  
 * {@code "null"} are appended to this sequence.  
 * <p>  
 * Let <i>n</i> be the length of this character sequence just prior to  
 * execution of the {@code append} method. Then the character at index  
 * <i>k</i> in the new character sequence is equal to the character at  
 * index <i>k</i> in the old character sequence, if <i>k</i> is less than  
 * <i>n</i>; otherwise, it is equal to the character at index <i>k-n</i>  
 * in the argument {@code sb}.  
 * * @param   sb   the {@code StringBuffer} to append.  
 * @return  a reference to this object.  
 */
public StringBuilder append(StringBuffer sb) {  
    super.append(sb);  
    return this;
}  
  
@Override  
public StringBuilder append(CharSequence s) {  
    super.append(s);  
    return this;}  
  
/**  
 * @throws     IndexOutOfBoundsException {@inheritDoc}  
 */@Override  
public StringBuilder append(CharSequence s, int start, int end) {  
    super.append(s, start, end);  
    return this;}  
  
@Override  
public StringBuilder append(char[] str) {  
    super.append(str);  
    return this;}  
  
/**  
 * @throws IndexOutOfBoundsException {@inheritDoc}  
 */@Override  
public StringBuilder append(char[] str, int offset, int len) {  
    super.append(str, offset, len);  
    return this;}  
  
@Override  
public StringBuilder append(boolean b) {  
    super.append(b);  
    return this;}  
  
@Override  
@IntrinsicCandidate  
public StringBuilder append(char c) {  
    super.append(c);  
    return this;}  
  
@Override  
@IntrinsicCandidate  
public StringBuilder append(int i) {  
    super.append(i);  
    return this;}  
  
@Override  
public StringBuilder append(long lng) {  
    super.append(lng);  
    return this;}  
  
@Override  
public StringBuilder append(float f) {  
    super.append(f);  
    return this;}  
  
@Override  
public StringBuilder append(double d) {  
    super.append(d);  
    return this;}
```
然后是StringBuffer的append方法，和StringBuilder的append方法差不多，都是委托给父类的append方法。==但是StringBuffer的append方法都有方法上的synchronized、还有toStringCache==——一个缓存值，缓存最后一次被toString方法返回的String值，当StringBuffer发生变化时清为null：
```java
@Override  
public synchronized StringBuffer append(Object obj) {  
    toStringCache = null;  
    super.append(String.valueOf(obj));  
    return this;}  
  
@Override  
@IntrinsicCandidate  
public synchronized StringBuffer append(String str) {  
    toStringCache = null;  
    super.append(str);  
    return this;}  
  
/**  
 * Appends the specified {@code StringBuffer} to this sequence.  
 * <p>  
 * The characters of the {@code StringBuffer} argument are appended,  
 * in order, to the contents of this {@code StringBuffer}, increasing the  
 * length of this {@code StringBuffer} by the length of the argument.  
 * If {@code sb} is {@code null}, then the four characters  
 * {@code "null"} are appended to this {@code StringBuffer}.  
 * <p>  
 * Let <i>n</i> be the length of the old character sequence, the one  
 * contained in the {@code StringBuffer} just prior to execution of the  
 * {@code append} method. Then the character at index <i>k</i> in  
 * the new character sequence is equal to the character at index <i>k</i>  
 * in the old character sequence, if <i>k</i> is less than <i>n</i>;  
 * otherwise, it is equal to the character at index <i>k-n</i> in the  
 * argument {@code sb}.  
 * <p>  
 * This method synchronizes on {@code this}, the destination  
 * object, but does not synchronize on the source ({@code sb}).  
 * * @param   sb   the {@code StringBuffer} to append.  
 * @return  a reference to this object.  
 * @since 1.4  
 */public synchronized StringBuffer append(StringBuffer sb) {  
    toStringCache = null;  
    super.append(sb);  
    return this;}  
  
/**  
 * @since 1.8  
 */@Override  
synchronized StringBuffer append(AbstractStringBuilder asb) {  
    toStringCache = null;  
    super.append(asb);  
    return this;}  
  
/**  
 * Appends the specified {@code CharSequence} to this  
 * sequence. * <p>  
 * The characters of the {@code CharSequence} argument are appended,  
 * in order, increasing the length of this sequence by the length of the * argument. * * <p>The result of this method is exactly the same as if it were an  
 * invocation of this.append(s, 0, s.length()); * * <p>This method synchronizes on {@code this}, the destination  
 * object, but does not synchronize on the source ({@code s}).  
 * * <p>If {@code s} is {@code null}, then the four characters  
 * {@code "null"} are appended.  
 * * @param   s the {@code CharSequence} to append.  
 * @return  a reference to this object.  
 * @since 1.5  
 */@Override  
public synchronized StringBuffer append(CharSequence s) {  
    toStringCache = null;  
    super.append(s);  
    return this;}  
  
/**  
 * @throws IndexOutOfBoundsException {@inheritDoc}  
 * @since      1.5  
 */@Override  
public synchronized StringBuffer append(CharSequence s, int start, int end)  
{  
    toStringCache = null;  
    super.append(s, start, end);  
    return this;}  
  
@Override  
public synchronized StringBuffer append(char[] str) {  
    toStringCache = null;  
    super.append(str);  
    return this;}  
  
/**  
 * @throws IndexOutOfBoundsException {@inheritDoc}  
 */@Override  
public synchronized StringBuffer append(char[] str, int offset, int len) {  
    toStringCache = null;  
    super.append(str, offset, len);  
    return this;}  
  
@Override  
public synchronized StringBuffer append(boolean b) {  
    toStringCache = null;  
    super.append(b);  
    return this;}  
  
@Override  
@IntrinsicCandidate  
public synchronized StringBuffer append(char c) {  
    toStringCache = null;  
    super.append(c);  
    return this;}  
  
@Override  
@IntrinsicCandidate  
public synchronized StringBuffer append(int i) {  
    toStringCache = null;  
    super.append(i);  
    return this;}  
  
/**  
 * @since 1.5  
 */@Override  
public synchronized StringBuffer appendCodePoint(int codePoint) {  
    toStringCache = null;  
    super.appendCodePoint(codePoint);  
    return this;}  
  
@Override  
public synchronized StringBuffer append(long lng) {  
    toStringCache = null;  
    super.append(lng);  
    return this;}  
  
@Override  
public synchronized StringBuffer append(float f) {  
    toStringCache = null;  
    super.append(f);  
    return this;}  
  
@Override  
public synchronized StringBuffer append(double d) {  
    toStringCache = null;  
    super.append(d);  
    return this;}
```
接下来我们看父类的方法。
## 4.2.1 入口方法
```java
public AbstractStringBuilder append(String str) {
	if (str == null)
		return appendNull();
	int len = str.length();
	ensureCapacityInternal(count + len);
	str.getChars(0, len, value, count);
	count += len;
	return this;
}
```
这个是 `public final class StringBuilder extends AbstractStringBuilder` ，StringBuffer也是这个父类，共用这个方法。这里包括了容量检测、元素拷贝、记录 count 数量。

本人看到的源码：一样特别处理了null的append，然后保证容量为count+len（count为字符数，在length()方法被返回；capacity()方法返回 value.length >> coder; 为可容纳的字符数量）。所以，str.length()为append的字符数，内部空间至少要扩容到count+str.count：
```java
/**  
 * Appends the specified string to this character sequence. * <p>  
 * The characters of the {@code String} argument are appended, in  
 * order, increasing the length of this sequence by the length of the 
 * argument. If {@code str} is {@code null}, then the four  
 * characters {@code "null"} are appended.  
 * <p>  
 * Let <i>n</i> be the length of this character sequence just prior to  
 * execution of the {@code append} method. Then the character at  
 * index <i>k</i> in the new character sequence is equal to the character  
 * at index <i>k</i> in the old character sequence, if <i>k</i> is less  
 * than <i>n</i>; otherwise, it is equal to the character at index  
 * <i>k-n</i> in the argument {@code str}.  
 * * @param   str   a string.  
 * @return  a reference to this object.  
 */public AbstractStringBuilder append(String str) {  
    if (str == null) {  
        return appendNull();  
    }  
    int len = str.length();  
    ensureCapacityInternal(count + len);  
    putStringAt(count, str); // 拷贝str中的字符到value数组的count下标及以后
    count += len;  // 新字符数为两个字符数之和
    return this;
}
```
### 4.2.2 扩容方法（父类的方法）
ensureCapacityInternal(count + len);
```java
	/**
	* This method has the same contract as ensureCapacity, but is
	* never synchronized.
	*/
	private void ensureCapacityInternal(int minimumCapacity) {
		// overflow-conscious code
		if (minimumCapacity - value.length > 0)
			expandCapacity(minimumCapacity);
	}
	/**
	* This implements the expansion semantics of ensureCapacity with no
	* size check or synchronization.
	*/
	void expandCapacity(int minimumCapacity) {
		int newCapacity = value.length * 2 + 2;
		if (newCapacity - minimumCapacity < 0)
			newCapacity = minimumCapacity;
		if (newCapacity < 0) {
			if (minimumCapacity < 0) // overflow
				throw new OutOfMemoryError();
			newCapacity = Integer.MAX_VALUE;
		}
		value = Arrays.copyOf(value, newCapacity);
	}
```
如上，StringBuilder，就跟操作数组的原理一样，都需要检测容量大小，按需扩容。扩容的容量是value数组的长度 n * 2 + 2，另外把原有元素拷贝到新新数组中。

但在我看到的源码中，**扩容时需要考虑coder字段**：LATIN1=0，UTF16=1，这两个可被赋给coder：
```java
/**  
 * Ensures that the capacity is at least equal to the specified minimum. * If the current capacity is less than the argument, then a new internal * array is allocated with greater capacity. The new capacity is the * larger of: * <ul>  
 * <li>The {@code minimumCapacity} argument.  
 * <li>Twice the old capacity, plus {@code 2}.  
 * </ul>  
 * If the {@code minimumCapacity} argument is nonpositive, this  
 * method takes no action and simply returns. * Note that subsequent operations on this object can reduce the * actual capacity below that requested here. * * @param   minimumCapacity   the minimum desired capacity.  
 */
public void ensureCapacity(int minimumCapacity) {  
    if (minimumCapacity > 0) {  
        ensureCapacityInternal(minimumCapacity);  
    }  
}  
  
/**  
 * For positive values of {@code minimumCapacity}, this method  
 * behaves like {@code ensureCapacity}, however it is never  
 * synchronized. * If {@code minimumCapacity} is non positive due to numeric  
 * overflow, this method throws {@code OutOfMemoryError}.  
 */
private void ensureCapacityInternal(int minimumCapacity) {  
    // overflow-conscious code  
    int oldCapacity = value.length >> coder;  // 旧的字符容量
    if (minimumCapacity - oldCapacity > 0) {  // 已经超过了旧的字符容量
        value = Arrays.copyOf(value,  
                newCapacity(minimumCapacity) << coder); // 按照新字符容量<<coder(编码)的大小，申请新value数组的长度
    }  
}  
  
/**  
 * Returns a capacity at least as large as the given minimum capacity. * Returns the current capacity increased by the current length + 2 if * that suffices. * Will not return a capacity greater than * {@code (SOFT_MAX_ARRAY_LENGTH >> coder)}  
 * unless the given minimum capacity is greater than that. * * @param  minCapacity the desired minimum capacity  
 * @throws OutOfMemoryError if minCapacity is less than zero or  
 *         greater than (Integer.MAX_VALUE >> coder) */
 private int newCapacity(int minCapacity) {  
    int oldLength = value.length;  
    int newLength = minCapacity << coder;  // 传进来的最小字符容量<<coder，就是新数组的长度
    int growth = newLength - oldLength;  
    int length = ArraysSupport.newLength(oldLength, growth, oldLength + (2 << coder));  // 还能申请更长，oldLength+oldLength+2<<coder)，最多为n*2+2
    if (length == Integer.MAX_VALUE) {  
        throw new OutOfMemoryError("Required length exceeds implementation limit");  
    }  
    return length >> coder;  
}
```
### 4.2.3 填充元素
str.getChars(0, len, value, count);
```java
public void getChars(int srcBegin, int srcEnd, char dst[], int dstBegin) {
	// ...
	System.arraycopy(value, srcBegin, dst, dstBegin, srcEnd - srcBegin);
}
```
**添加元素的方式是基于 System.arraycopy 拷贝操作进行的，这是一个本地方法**。

我看的源码：如果压缩字符串COMPACT_STRINGS为true（final值，一直为true）、但编码不一定是Latin1、且添加进来的编码coder不同，则需要inflate，当然如果编码现在不是Latin1就不用inflate、直接返回；否则将每个8位存储扩展为16位，并设置编码为UTF16：
```java
private void putStringAt(int index, String str) {  
    inflateIfNeededFor(str);  
    str.getBytes(value, index, coder);  
}
private void inflateIfNeededFor(String input) {  
    if (COMPACT_STRINGS && (coder != input.coder())) {  
        inflate();  
    }  
}
/**  
 * If the coder is "isLatin1", this inflates the internal 8-bit storage * to 16-bit <hi=0, low> pair storage.  
 */
 private void inflate() {  
    if (!isLatin1()) {  
        return;  
    }  
    byte[] buf = StringUTF16.newBytesFor(value.length);  
    StringLatin1.inflate(value, 0, buf, 0, count);  
    this.value = buf;  
    this.coder = UTF16;  
}

// String类中的getBytes方法
/**  
 * Copy character bytes from this string into dst starting at dstBegin. * This method doesn't perform any range checking. * * Invoker guarantees: dst is in UTF16 (inflate itself for asb), if two * coders are different, and dst is big enough (range check) * * @param dstBegin  the char index, not offset of byte[]  
 * @param coder     the coder of dst[]  
 */
void getBytes(byte[] dst, int dstBegin, byte coder) {  
    if (coder() == coder) {  
        System.arraycopy(value, 0, dst, dstBegin << coder, value.length);  
    } else {    // this.coder == LATIN && coder == UTF16  
        StringLatin1.inflate(value, 0, dst, dstBegin, value.length);  
    }  
}
/**  
 * Copy character bytes from this string into dst starting at dstBegin. * This method doesn't perform any range checking. * * Invoker guarantees: dst is in UTF16 (inflate itself for asb), if two * coders are different, and dst is big enough (range check) * * @param srcPos    the char index, not offset of byte[]  
 * @param dstBegin  the char index to start from  
 * @param coder     the coder of dst[]  
 * @param length    the amount of copied chars  
 */
void getBytes(byte[] dst, int srcPos, int dstBegin, byte coder, int length) {  
    if (coder() == coder) {  // 可能都是Latin1，或都是UTF16；
        System.arraycopy(value, srcPos << coder, dst, dstBegin << coder, length << coder);  
    } else {    // this.coder == LATIN && coder == UTF16  
        StringLatin1.inflate(value, srcPos, dst, dstBegin, length);  
    }  
}
```
### 4.3 toString()
既然 stringBuilder 是数组，那么它是怎么转换成字符串的呢？其实需要用到它是 String 字符串的时候，就是使用 String 的构造函数传递数组进行转换的，这个方法在我们上面讲解 String 的时候已经介绍过。
```java
@Override
public String toString() {
	// Create a copy, don't share the array
	return new String(value, 0, count);
}
```
我看到的StringBuilder源码中：
```java
@Override  
@IntrinsicCandidate  
public String toString() {  
    // Create a copy, don't share the array  
    return isLatin1() ? StringLatin1.newString(value, 0, count)  
                      : StringUTF16.newString(value, 0, count);  
}
```
我看到的StringBuffer源码中，用到了一个toStringCache缓存，当然还有编码的问题：
```java
@Override  
@IntrinsicCandidate  
public synchronized String toString() {  
    if (toStringCache == null) {  
        return toStringCache =  
                isLatin1() ? StringLatin1.newString(value, 0, count)  
                           : StringUTF16.newString(value, 0, count);  
    }  
    return new String(toStringCache);  
}
```

两个类序列化的方法可以看！TODO

# StringBuffer源码分析

StringBuffer 与 StringBuilder，API 的使用和底层实现上基本一致，不同的是 StringBuffer 加了 synchronized 🔒锁，所以它是线程安全的。源码如下；
```java
@Override  
@IntrinsicCandidate  
public synchronized StringBuffer append(String str) {  
    toStringCache = null;  
    super.append(str);  
    return this;
}
```
那么，**synchronized 不是重量级锁吗，JVM 对它有什么优化呢**？其实为了减少获得锁与释放锁带来的性能损耗，从而引入了**偏向锁、轻量级锁、重量级锁**来进行优化，进行一个锁升级，如下图(此图引自互联网用户：韭韭韭韭菜，画的非常优秀)；
1. 从无锁01状态开始，**当线程进入 synchronized 同步代码块，会检查对象头和栈帧内是否有当前线程 ID 编号**，无则使用 CAS 替换。
2. 解锁时，会使用 CAS 将 Displaced Mark Word 替换回到对象头，**如果成功，则表示竞争没有发生**，反之则表示**当前锁存在竞争锁就会升级成重量级锁**。
3. 另外，==大多数情况下锁🔒是不发生竞争的，基本由一个线程持有==。所以，==为了避免获得锁与释放锁带来的性能损耗，所以引入锁升级，升级后不能降级==。
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202305071953724.png)

# 常用API
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202305071957602.png)
本文介绍了 String、StringBuilder、StringBuffer，的数据结构和源码分析，更加透彻的理解后，也能更加准确的使用，不会被因为不懂而犯错误。

想把代码写好，至少要有这四面内容，包括；数据结构、算法、源码、设计模式，这四方面在加上业务经验与个人视野，才能真的把一个需求、一个大项目写的具备良好的扩展性和易维护性。




