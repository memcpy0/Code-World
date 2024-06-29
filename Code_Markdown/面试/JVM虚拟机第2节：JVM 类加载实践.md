学习，不知道从哪下手？

当学习一个新知识不知道从哪下手的时候，最有效的办法是梳理这个知识结构的脉络信息，汇总出一整张的思维导出。接下来就是按照思维导图的知识结构，一个个学习相应的知识点，并汇总记录。

就像 JVM 的学习，可以说它包括了非常多的内容，也是一个庞大的知识体系。例如：**类加载、加载器**、生命周期、性能优化、调优参数、调优工具、优化方案、内存区域、**虚拟机栈**、直接内存、内存溢出、**元空间**、**垃圾回收、可达性分析、标记清除、回收过程**等等。如果没有梳理的一头扎进去，东一榔头西一棒子，很容易造成学习恐惧感。如图 24-1 是 JVM 知识框架梳理，后续我们会按照这个结构陆续讲解每一块内容。
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202305250816889.png)

---
# 一、面试题
谢飞机，小记！，很多知识根本就是背背背，也没法操作，难学！
谢飞机：大哥，你问我两个 JVM 问题，我看看我自己还行不！
面试官：啊？嗯！往死了问还是？
谢飞机：就就就，都行！你看着来！
面试官：啊，那 JVM 加载过程都是什么步骤？
谢飞机：巴拉巴拉，加载、验证、准备、解析、初始化、使用、卸载！
面试官：嗯，背的挺好！我怀疑你没操作过！ 那加载的时候，JVM 规范规定从
第几位开始是解析常量池，以及数据类型是如何定义的，u1、u2、u4，是怎么个
玩意？
谢飞机：握草！算了，告诉我看啥吧！

![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202305250819398.png)
JVM 类加载过程分为，**加载、链接、初始化、使用和卸载**这四个阶段，在链接中又包括：验证、准备、解析。
- 加载：Java 虚拟机规范对 class 文件格式进行了严格的规则，但对于从哪里加载class 文件，却非常自由。Java 虚拟机实现可以从文件系统读取、从 JAR(或 ZIP)压缩包中提取 class 文件。除此之外也可以通过网络下载、数据库加载，甚至是运行时直接生成的 class 文件。
- 链接：包括了三个阶段；
    - **验证**，确保被加载类的正确性，验证字节流是否符合 class 文件规范，例如 魔数 0xCAFEBABE，以及版本号等。
    - **准备**，**为类的静态变量分配内存**并设置变量初始值等
    - **解析**，解析包括**解析出常量池数据和属性表信息**，这里会包括ConstantPool 结构体以及 AttributeInfo 接口等。
- 初始化：类加载完成的最后一步就是初始化，目的就是为标记常量值的字段赋值，以及执行 `<clinit>` 方法的过程。**JVM 虚拟机通过锁的方式确保 clinit 仅被执行一次**
- 使用：程序代码执行使用阶段。
- 卸载：程序代码退出、异常、结束等。
# 三、写个代码加载下
JVM 之所以不好掌握，主要是因为不好实操。虚拟机是 C++ 写的，很多 Java程序员根本就不会去读，或者读不懂。那么，也就没办法实实在在的体会到，到底是怎么加载的，加载的时候都干了啥。只有看到代码，我才觉得自己学会了！

所以，我们这里要手动写一下，JVM 虚拟机的部分代码，也就是类加载的过程。通过 Java 代码来实现 Java 虚拟机的部分功能，让开发 Java 代码的程序员更容易理解虚拟机的执行过程。
## 1. 案例工程
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202305250854979.png)

以上，工程结构就是按照 JVM 虚拟机规范，使用 Java 代码实现 JVM 中加载 class 文件部分内容。当然这部分还不包括解析，因为解析部分的代码非常庞大，我们先从**把 .class 文件加载**读取开始了解。
2. 代码讲解
2.1 定义类路径接口(Entry)
```java
public interface Entry {
byte[] readClass(String className) throws IOException;
static Entry create(String path) {
//File.pathSeparator；路径分隔符(win\linux)
if (path.contains(File.pathSeparator)) {
return new CompositeEntry(path);
}
if (path.endsWith("*")) {
return new WildcardEntry(path);
}
if (path.endsWith(".jar") || path.endsWith(".JAR") ||
path.endsWith(".zip") || path.endsWith(".ZIP")) {
return new ZipEntry(path);
}
return new DirEntry(path);
}
}
```
接口中提供了接口方法 readClass 和静态方法 create(String path)。

jdk1.8 是可以在接口中编写静态方法的，在设计上属于补全了抽象类的类似功能。这个静态方法主要是按照不同的路径地址类型，提供不同的解析方法。包括：CompositeEntry、WildcardEntry、ZipEntry、DirEntry，这四种。接下来分别看每一种的具体实现
### 2.2 目录形式路径(DirEntry)
```
public class DirEntry implements Entry {
private Path absolutePath;
public DirEntry(String path){
//获取绝对路径
this.absolutePath = Paths.get(path).toAbsolutePath();
}
@Override
public byte[] readClass(String className) throws IOException {
return Files.readAllBytes(absolutePath.resolve(className));
}
@Override
public String toString() {
return this.absolutePath.toString();
}
}
```

目录形式的通过读取绝对路径下的文件，通过 Files.readAllBytes 方式获
取字节码。
### 2.3 压缩包形式路径(ZipEntry)
```
public class ZipEntry implements Entry {
private Path absolutePath;
public ZipEntry(String path) {
//获取绝对路径
this.absolutePath = Paths.get(path).toAbsolutePath();
}
@Override
public byte[] readClass(String className) throws IOException {
try (FileSystem zipFs = FileSystems.newFileSystem(absolutePath, null)) {
return Files.readAllBytes(zipFs.getPath(className));
}
}
@Override
public String toString() {
return this.absolutePath.toString();
}
}
```
其实压缩包形式与目录形式，只有在文件读取上有包装差别而已。
FileSystems.newFileSystem
### 2.4 混合形式路径(CompositeEntry)
```
public class CompositeEntry implements Entry {
private final List<Entry> entryList = new ArrayList<>();
public CompositeEntry(String pathList) {
String[] paths = pathList.split(File.pathSeparator);
for (String path : paths) {
entryList.add(Entry.create(path));
}
}
@Override
public byte[] readClass(String className) throws IOException {
for (Entry entry : entryList) {
try {
return entry.readClass(className);
} catch (Exception ignored) {
//ignored
}
}
throw new IOException("class not found " + className);
}
@Override
public String toString() {
String[] strs = new String[entryList.size()];
for (int i = 0; i < entryList.size(); i++) {
strs[i] = entryList.get(i).toString();
}
return String.join(File.pathSeparator, strs);
}
}
```
File.pathSeparator，是一个分隔符属性，win/linux 有不同的类型，所以使
用这个方法进行分割路径。

分割后的路径装到 List 集合中，这个过程属于拆分路径。
### 2.5 通配符类型路径(WildcardEntry)
```java
public class WildcardEntry extends CompositeEntry {
public WildcardEntry(String path) {
super(toPathList(path));
}
private static String toPathList(String wildcardPath) {
String baseDir = wildcardPath.replace("*", ""); // remove *
try {
return Files.walk(Paths.get(baseDir))
.filter(Files::isRegularFile)
.map(Path::toString)
.filter(p -> p.endsWith(".jar") || p.endsWith(".JAR"))
.collect(Collectors.joining(File.pathSeparator));
} catch (IOException e) {
return "";
}
}
}
```

这个类属于混合形式路径处理类的子类，唯一提供的方法就是把类路径解析出来。
### 2.6 类路径解析(Classpath)
启动类路径、扩展类路径、用户类路径，熟悉吗？是不经常看到这几句话，那么时候
怎么实现的呢？
有了上面我们做的一些基础类的工作，接下来就是类解析的实际调用过程。代码
如下：
```java
public class Classpath {
private Entry bootstrapClasspath; //启动类路径
private Entry extensionClasspath; //扩展类路径
private Entry userClasspath; //用户类路径
public Classpath(String jreOption, String cpOption) {
//启动类&扩展类 "C:\Program Files\Java\jdk1.8.0_161\jre"
bootstrapAndExtensionClasspath(jreOption);
//用户类 F:\..\org\itstack\demo\test\HelloWorld
parseUserClasspath(cpOption);
}
private void bootstrapAndExtensionClasspath(String jreOption) {
String jreDir = getJreDir(jreOption);
//..jre/lib/*
String jreLibPath = Paths.get(jreDir, "lib") + File.separator + "*";
bootstrapClasspath = new WildcardEntry(jreLibPath);
//..jre/lib/ext/*
String jreExtPath = Paths.get(jreDir, "lib", "ext") + File.separator + "*";
extensionClasspath = new WildcardEntry(jreExtPath);
}
private static String getJreDir(String jreOption) {
if (jreOption != null && Files.exists(Paths.get(jreOption))) {
return jreOption;
}
if (Files.exists(Paths.get("./jre"))) {
return "./jre";
}
String jh = System.getenv("JAVA_HOME");
if (jh != null) {
return Paths.get(jh, "jre").toString();
}
throw new RuntimeException("Can not find JRE folder!");
}
private void parseUserClasspath(String cpOption) {
if (cpOption == null) {
cpOption = ".";
}
userClasspath = Entry.create(cpOption);
}
public byte[] readClass(String className) throws Exception {
className = className + ".class";
//[readClass]启动类路径
try {
return bootstrapClasspath.readClass(className);
} catch (Exception ignored) {
//ignored
}
//[readClass]扩展类路径
try {
return extensionClasspath.readClass(className);
} catch (Exception ignored) {
//ignored
}
//[readClass]用户类路径
return userClasspath.readClass(className);
}
}
```
启动类路径，bootstrapClasspath.readClass(className);

扩展类路径，extensionClasspath.readClass(className);

用户类路径，userClasspath.readClass(className);

这回就看到它们具体在哪使用了吧！有了具体的代码也就方便理解了
### 2.7 加载类测试验证
```java
private static void startJVM(Cmd cmd) {
Classpath cp = new Classpath(cmd.jre, cmd.classpath);
System.out.printf("classpath：%s class：%s args：%s\n", cp, cmd.getMainClass(),
cmd.getAppArgs());
//获取 className
String className = cmd.getMainClass().replace(".", "/");
try {
byte[] classData = cp.readClass(className);
System.out.println(Arrays.toString(classData));
} catch (Exception e) {
System.out.println("Could not find or load main class " + cmd.getMainClass(
));
e.printStackTrace();
}
}
```
这 段 就 是 使 用 Classpath 类 进 行 类 路 径 加 载 ， 这 里 我 们 测 试 加 载
java.lang.String 类。你可以加载其他的类，或者自己写的类

配置 IDEA，program arguments 参数：-Xjre "C:\Program
Files\Java\jdk1.8.0_161\jre" java.lang.String

另外这里读取出的 class 文件信息，打印的是 byte 类型信息。
测试结果
[-54, -2, -70, -66, 0, 0, 0, 52, 2, 28, 3, 0, 0, -40, 0, 3, 0, 0, -37, -
1, 3, 0, 0, -33, -
1, 3, 0, 1, 0, 0, 8, 0, 15, 8, 0, 61, 8, 0, 85, 8, 0, 88, 8, 0, 89, 8, 0, 112, 8, 0
, -81, 8, 0, -75, 8, 0, -47, 8, 0, -
45, 1, 0, 0, 1, 0, 3, 40, 41, 73, 1, 0, 20, 40, 41, 76, 106, 97, 118, 97, 47, 108,
97, 110, 103, 47, 79, 98, 106, 101, 99, 116, 59, 1, 0, 20, 40, 41, 76, 106, 97, 118
 360 / 417

, 97, 47, 108, 97, 110, 103, 47, 83, 116, 114, 105, 110, 103, 59, 1, 0, 3, 40, 41,
86, 1, 0, 3, 40, 41, 90, 1, 0, 4, 40, 41, 91, ...]
这块部分截取的程序运行打印结果，就是读取的 class 文件信息，只不过暂时
还不能看出什么。接下来我们再把它翻译过来！
四、解析字节码文件
JVM 在把 class 文件加载完成后，接下来就进入链接的过程，这个过程包括了
内容的校验、准备和解析，其实就是把 byte 类型 class 翻译过来，做相应的
操作。
整个这个过程内容相对较多，这里只做部分逻辑的实现和讲解。如果读者感兴趣
可以阅读小傅哥的《用 Java 实现 JVM》专栏。
1. 提取部分字节码
```
//取部分字节码：java.lang.String
private static byte[] classData = {
-54, -2, -70, -66, 0, 0, 0, 52, 2, 26, 3, 0, 0, -40, 0, 3, 0, 0, -37, -
1, 3, 0, 0, -33, -1, 3, 0, 1, 0, 0, 8, 0,
59, 8, 0, 83, 8, 0, 86, 8, 0, 87, 8, 0, 110, 8, 0, -83, 8, 0, -77, 8, 0, -
49, 8, 0, -47, 1, 0, 3, 40, 41, 73, 1,
0, 20, 40, 41, 76, 106, 97, 118, 97, 47, 108, 97, 110, 103, 47, 79, 98, 106
, 101, 99, 116, 59, 1, 0, 20, 40, 41,
76, 106, 97, 118, 97, 47, 108, 97, 110, 103, 47, 83, 116, 114, 105, 110, 10
3, 59, 1, 0, 3, 40, 41, 86, 1, 0, 3,
40, 41, 90, 1, 0, 4, 40, 41, 91, 66, 1, 0, 4, 40, 41, 91, 67, 1, 0, 4, 40,
67, 41, 67, 1, 0, 21, 40, 68, 41, 76,
106, 97, 118, 97, 47, 108, 97, 110, 103, 47, 83, 116, 114, 105, 110, 103, 5
9, 1, 0, 4, 40, 73, 41, 67, 1, 0, 4};
```

java.lang.String 解析出来的字节码内容较多，当然包括的内容也多，比如魔数、版
本、类、常量、方法等等。所以我们这里只截取部分进行进行解析。
2. 解析魔数并校验
很多文件格式都会规定满足该格式的文件必须以某几个固定字节开头，这几个字
节主要起到标识作用，叫作魔数(magic number)。
例如；
 361 / 417


PDF 文件以 4 字节“%PDF”(0x25、0x50、0x44、0x46)开头，

ZIP 文件以 2 字节“PK”(0x50、0x4B)开头

class 文件以 4 字节“0xCAFEBABE”开头
```
private static void readAndCheckMagic() {
System.out.println("\r\n------------ 校验魔数 ------------");
//从 class 字节码中读取前四位
byte[] magic_byte = new byte[4];
System.arraycopy(classData, 0, magic_byte, 0, 4);
//将 4 位 byte 字节转成 16 进制字符串
String magic_hex_str = new BigInteger(1, magic_byte).toString(16);
System.out.println("magic_hex_str：" + magic_hex_str);
//byte_magic_str 是 16 进制的字符串，cafebabe，因为 java 中没有无符号整型，所以如果想
要无符号只能放到更高位中
long magic_unsigned_int32 = Long.parseLong(magic_hex_str, 16);
System.out.println("magic_unsigned_int32：" + magic_unsigned_int32);
//魔数比对，一种通过字符串比对，另外一种使用假设的无符号 16 进制比较。如果使用无符号比较
需要将 0xCAFEBABE & 0x0FFFFFFFFL 与运算
System.out.println("0xCAFEBABE & 0x0FFFFFFFFL：" + (0xCAFEBABE & 0x0FFFFFFFFL));
if (magic_unsigned_int32 == (0xCAFEBABE & 0x0FFFFFFFFL)) {
System.out.println("class 字节码魔数无符号 16 进制数值一致校验通过");
} else {
System.out.println("class 字节码魔数无符号 16 进制数值一致校验拒绝");
}
}
```
读取字节码中的前四位，-54, -2, -70, -66，将这四位转换为 16 进制。

因为 java 中是没有无符号整型的，所以只能用更高位存放。

解析后就是魔数的对比，看是否与 CAFEBABE 一致。
测试结果
 362 / 417

```
------------ 校验魔数 ------------
magic_hex_str：cafebabe
magic_unsigned_int32：3405691582
0xCAFEBABE & 0x0FFFFFFFFL：3405691582
```
class 字节码魔数无符号 16 进制数值一致校验通过
3. 解析版本号信息
刚才我们已经读取了 4 位魔数信息，接下来再读取 2 位，是版本信息。
魔数之后是 class 文件的次版本号和主版本号，都是 u2 类型。假设某 class 文
件的主版本号是 M，次版本号是 m，那么完整的版本号可以表示成“M.m”的形式。
次版本号只在 J2SE 1.2 之前用过，从 1.2 开始基本上就没有什么用了(都是 0)。
主版本号在 J2SE 1.2 之前是 45，从 1.2 开始，每次有大版本的 Java 版本发布，
都会加 1｛45、46、47、48、49、50、51、52｝
```
private static void readAndCheckVersion() {
System.out.println("\r\n------------ 校验版本号 ------------");
//从 class 字节码第 4 位开始读取，读取 2 位
byte[] minor_byte = new byte[2];
System.arraycopy(classData, 4, minor_byte, 0, 2);
//将 2 位 byte 字节转成 16 进制字符串
String minor_hex_str = new BigInteger(1, minor_byte).toString(16);
System.out.println("minor_hex_str：" + minor_hex_str);
//minor_unsigned_int32 转成无符号 16 进制
int minor_unsigned_int32 = Integer.parseInt(minor_hex_str, 16);
System.out.println("minor_unsigned_int32：" + minor_unsigned_int32);
//从 class 字节码第 6 位开始读取，读取 2 位
byte[] major_byte = new byte[2];
System.arraycopy(classData, 6, major_byte, 0, 2);
//将 2 位 byte 字节转成 16 进制字符串
String major_hex_str = new BigInteger(1, major_byte).toString(16);
System.out.println("major_hex_str：" + major_hex_str);
//major_unsigned_int32 转成无符号 16 进制
int major_unsigned_int32 = Integer.parseInt(major_hex_str, 16);

System.out.println("major_unsigned_int32：" + major_unsigned_int32);
System.out.println("版本号：
" + major_unsigned_int32 + "." + minor_unsigned_int32);
}
```

这里有一个小技巧，class 文件解析出来是一整片的内容，JVM 需要按照虚拟机规
范，一段一段的解析出所有的信息。

同样这里我们需要把 2 位 byte 转换为 16 进制信息，并继续从第 6 位继续读取 2 位
信息。组合出来的才是版本信息。
测试结果
------------ 校验版本号 ------------
minor_hex_str：0
minor_unsigned_int32：0
major_hex_str：34
major_unsigned_int32：52
版本号：52.0
4. 解析全部内容对照
按照 JVM 的加载过程，其实远不止魔数和版本号信息，还有很多其他内容，这
里我们可以把测试结果展示出来，方便大家有一个学习结果的比对印象。
```
classpath：org.itstack.demo.jvm.classpath.Classpath@4bf558aa class：
java.lang.String args：null
version: 52.0
constants count：540
access flags：0x31
this class：java/lang/String
super class：java/lang/Object
interfaces：[java/io/Serializable, java/lang/Comparable, java/lang/CharSequence]
fields count：5
value [C
hash I
serialVersionUID J
serialPersistentFields [Ljava/io/ObjectStreamField;
CASE_INSENSITIVE_ORDER Ljava/util/Comparator;
methods count: 94
<init> ()V
<init> (Ljava/lang/String;)V 
<init> ([C)V
<init> ([CII)V
<init> ([III)V
<init> ([BIII)V
<init> ([BI)V
checkBounds ([BII)V
<init> ([BIILjava/lang/String;)V
<init> ([BIILjava/nio/charset/Charset;)V
<init> ([BLjava/lang/String;)V
<init> ([BLjava/nio/charset/Charset;)V
<init> ([BII)V
<init> ([B)V
<init> (Ljava/lang/StringBuffer;)V
<init> (Ljava/lang/StringBuilder;)V
<init> ([CZ)V
length ()I
isEmpty ()Z
charAt (I)C
codePointAt (I)I
codePointBefore (I)I
codePointCount (II)I
offsetByCodePoints (II)I
getChars ([CI)V
getChars (II[CI)V
getBytes (II[BI)V
getBytes (Ljava/lang/String;)[B
getBytes (Ljava/nio/charset/Charset;)[B
getBytes ()[B
equals (Ljava/lang/Object;)Z
contentEquals (Ljava/lang/StringBuffer;)Z
nonSyncContentEquals (Ljava/lang/AbstractStringBuilder;)Z
contentEquals (Ljava/lang/CharSequence;)Z
equalsIgnoreCase (Ljava/lang/String;)Z
compareTo (Ljava/lang/String;)I
compareToIgnoreCase (Ljava/lang/String;)I
regionMatches (ILjava/lang/String;II)Z
regionMatches (ZILjava/lang/String;II)Z
startsWith (Ljava/lang/String;I)Z
startsWith (Ljava/lang/String;)Z 
endsWith (Ljava/lang/String;)Z
hashCode ()I
indexOf (I)I
indexOf (II)I
indexOfSupplementary (II)I
lastIndexOf (I)I
lastIndexOf (II)I
lastIndexOfSupplementary (II)I
indexOf (Ljava/lang/String;)I
indexOf (Ljava/lang/String;I)I
indexOf ([CIILjava/lang/String;I)I
indexOf ([CII[CIII)I
lastIndexOf (Ljava/lang/String;)I
lastIndexOf (Ljava/lang/String;I)I
lastIndexOf ([CIILjava/lang/String;I)I
lastIndexOf ([CII[CIII)I
substring (I)Ljava/lang/String;
substring (II)Ljava/lang/String;
subSequence (II)Ljava/lang/CharSequence;
concat (Ljava/lang/String;)Ljava/lang/String;
replace (CC)Ljava/lang/String;
matches (Ljava/lang/String;)Z
contains (Ljava/lang/CharSequence;)Z
replaceFirst (Ljava/lang/String;Ljava/lang/String;)Ljava/lang/String;
replaceAll (Ljava/lang/String;Ljava/lang/String;)Ljava/lang/String;
replace (Ljava/lang/CharSequence;Ljava/lang/CharSequence;)Ljava/lang/String;
split (Ljava/lang/String;I)[Ljava/lang/String;
split (Ljava/lang/String;)[Ljava/lang/String;
join (Ljava/lang/CharSequence;[Ljava/lang/CharSequence;)Ljava/lang/String;
join (Ljava/lang/CharSequence;Ljava/lang/Iterable;)Ljava/lang/String;
toLowerCase (Ljava/util/Locale;)Ljava/lang/String;
toLowerCase ()Ljava/lang/String;
toUpperCase (Ljava/util/Locale;)Ljava/lang/String;
toUpperCase ()Ljava/lang/String;
trim ()Ljava/lang/String;
toString ()Ljava/lang/String;
toCharArray ()[C
format (Ljava/lang/String;[Ljava/lang/Object;)Ljava/lang/String;
format (Ljava/util/Locale;Ljava/lang/String;[Ljava/lang/Object;)Ljava/lang/String;
valueOf (Ljava/lang/Object;)Ljava/lang/String;
valueOf ([C)Ljava/lang/String;
valueOf ([CII)Ljava/lang/String;
copyValueOf ([CII)Ljava/lang/String;
copyValueOf ([C)Ljava/lang/String;
valueOf (Z)Ljava/lang/String;
valueOf (C)Ljava/lang/String;
valueOf (I)Ljava/lang/String;
valueOf (J)Ljava/lang/String;
valueOf (F)Ljava/lang/String;
valueOf (D)Ljava/lang/String;
intern ()Ljava/lang/String;
compareTo (Ljava/lang/Object;)I
<clinit> ()V
Process finished with exit code 0
```
如果大家对这部分验证、准备、解析，的实现过程感兴趣，可以参照这部分用 Java
实现的 JVM 源码：https://github.com/fuzhengwei/itstack-demo-jvm
# 五、总结
学习 JVM 最大的问题是不好实践，所以本文以案例实操的方式，学习 JVM 的加载解析过程。也让更多的对 JVM 感兴趣的研发，能更好的接触到 JVM 并深入的学习。

有了以上这段代码，大家可以参照 JVM 虚拟机规范，在调试 Java 版本的 JVM，这样就可以非常容易理解整个 JVM 的加载过程，都做了什么。