@[toc]

# 1. VS Code配置Ruby语言环境
首先看一下这篇文章，配置好学习Ruby的语言环境。

下面将进入Ruby基础知识的学习，做好准备吧！

Ruby的官网http://www.ruby-lang.org/
# 2. Ruby Shell的交互式体验
## 2.1 相关命令 `irb, ruby` 及参数
Python的交互式控制台和解释器命令都是 `python` ，而Ruby的交互式命令是 `irb` ，解释器命令（用于执行程序文件）则是 `ruby` ：
```cpp
> irb -h
Usage:  irb.rb [options] [programfile] [arguments]
  -f                Suppress read of ~/.irbrc
  -d                Set $DEBUG to true (same as `ruby -d')
  -r load-module    Same as `ruby -r'
  -I path           Specify $LOAD_PATH directory
  -U                Same as `ruby -U`
  -E enc            Same as `ruby -E`
  -w                Same as `ruby -w`
  -W[level=2]       Same as `ruby -W`
  --context-mode n  Set n[0-4] to method to create Binding Object,
                    when new workspace was created
  --echo            Show result(default)
  --noecho          Don't show result
  --inspect         Use `inspect' for output
  --noinspect       Don't use inspect for output
  --multiline       Use multiline editor module
  --nomultiline     Don't use multiline editor module
  --singleline      Use singleline editor module
  --nosingleline    Don't use singleline editor module
  --colorize        Use colorization
  --nocolorize      Don't use colorization
  --prompt prompt-mode/--prompt-mode prompt-mode
                    Switch prompt mode. Pre-defined prompt modes are
                    `default', `simple', `xmp' and `inf-ruby'
  --inf-ruby-mode   Use prompt appropriate for inf-ruby-mode on emacs.
                    Suppresses --multiline and --singleline.
  --sample-book-mode/--simple-prompt
                    Simple prompt mode
  --noprompt        No prompt mode
  --single-irb      Share self with sub-irb.
  --tracer          Display trace for each execution of commands.
  --back-trace-limit n
                    Display backtrace top n and tail n. The default
                    value is 16.
  --verbose         Show details
  --noverbose       Don't show details
  -v, --version     Print the version of irb
  -h, --help        Print help
  --                Separate options of irb from the list of command-line args

> ruby -h
Usage: ruby [switches] [--] [programfile] [arguments]
  -0[octal]       specify record separator (\0, if no argument)
  -a              autosplit mode with -n or -p (splits $_ into $F)
  -c              check syntax only
  -Cdirectory     cd to directory before executing your script
  -d              set debugging flags (set $DEBUG to true)
  -e 'command'    one line of script. Several -e's allowed. Omit [programfile]
  -Eex[:in]       specify the default external and internal character encodings
  -Fpattern       split() pattern for autosplit (-a)
  -i[extension]   edit ARGV files in place (make backup if extension supplied)
  -Idirectory     specify $LOAD_PATH directory (may be used more than once)
  -l              enable line ending processing
  -n              assume 'while gets(); ... end' loop around your script
  -p              assume loop like -n but print line also like sed
  -rlibrary       require the library before executing your script
  -s              enable some switch parsing for switches after script name
  -S              look for the script using PATH environment variable
  -v              print the version number, then turn on verbose mode
  -w              turn warnings on for your script
  -W[level=2|:category]     set warning level; 0=silence, 1=medium, 2=verbose
  -x[directory]   strip off text before #!ruby line and perhaps cd to directory
  --jit           enable JIT with default options (experimental)
  --jit-[option]  enable JIT with an option (experimental)
  -h              show this message, --help for more info
```
如果只执行 `irb` 命令，那么提示符会是 `irb(main):001:0>` ，所以要执行 `irb --simple-prompt` 命令，提示符变为 `>>` 。本章出现 `>>` 时默认在与IRB交互，结果显示在 `=>` 提示符之后。
```ruby
>> 1 + 2
=> 3
>> 'hello world'.length
=> 11
>> x = 2
=> 2
>> y = 3
=> 3
>> z = x + y
=> 5
>> x * y * z
=> 30
```
## 2.2 IRB如何显示对象、自定义 `#inspect` 方法
IRB是如何显示一个对象的呢？此处可以看了第5节后再返回来看。**当IRB需要显示一个对象时，会向这个对象发送 `inspect` 消息，这个对象就返回自身的字符串表示**。Ruby中所有对象都默认拥有对 `#inspect` 方法的合理实现。通过自己提供定义，可以控制如何在命令行显示对象：
```ruby
>> o = Object.new
=> #<Object:0x00000229682581e8>
?> def o.inspect
?>   '[my object]'
>> end
=> :inspect
>> o
=> [my object]
```


# 3. 不同类型的值
Ruby是**面向表达式**的语言，**每一段有效的代码执行后都要产生一个值**。
## 3.1 基本类型值
Ruby支持布尔型 `Boolean`（支持 `&&, ||, !` 等逻辑运算）、数值型 `number`（支持 `+, -, *, /, ()` 等常规运算）、字符串 `string`（支持 `+` 连接运算）。
```ruby
>> (true && false) || true
=> true
>> (3 + 3) * (14 / 2)
=> 42
>> 'hello' + ' world' # 字符串连接
=> "hello world"
>> 'hello world'.slice(6) # 取字符
=> "w"
```
此外很重要的是Ruby符号 `symbol` ——一个**轻量**、**不可变**的名字值，作为字符串的**简单化**、**非内存密集化** `less memory-intensive` 的替代。符号在Ruby中被广泛使用，特别是作为哈希表的键。**符号字面量**以 `:` 开头：
```ruby
>> :my_symbol
=> :my_symbol
>> :my_symbol == :my_symbol
=> true
>> :my_symbol == :another_symbol
=> false
```
特殊值 `nil` 用来表示**不存在任何有用的值**：
```ruby
>> 'hello world'.slice(12)
=> nil
```

### 3.1.1 字符串插值
对于字符串，可以使用单引号也可以使用双引号，只是**双引号中的字符串会自动用表达式的结果替换 `#{ 表达式 }`** ，执行字符串插值操作。示例如下：
```ruby
>> "hello #{'dlrow'.reverse}"
=> "hello world"
```


如果**被插入的表达式**返回的值**不是一个字符串类型的对象**，那么这个对象会自动收到一个 `to_s` 消息、然后对象执行 `#to_s` 方法以返回能够替代其位置的字符串。由此，我们可以控制**被插入的表达式返回的对象的表示方式**：
```ruby
>> x = 3
=> 3
>> "x is equal to #{x}" # 调用x.to_s,发送给x对象to_s消息,x对象调用#to_s方法响应
=> "x is equal to 3"
>> o = Object.new
=> #<Object:0x000002296875ac18>
?> def o.to_s
?>   'a new object'
>> end
=> :to_s
>> "here is an #{o}" # 调用o.to_s,发送给o对象to_s消息,o对象调用#to_s方法响应
=> "here is an a new object"
>> "here is #{o}"
=> "here is a new object"
```
### 3.1.2 字符串打印
`#puts` 方法对每个Ruby对象包括 `main` 都可用，用于向标准输出打印字符串：
```ruby
>> x = 128
=> 128
?> while x < 1000
?>   puts "x is #{x}"
?>   x = x * 2
>> end
x is 128
x is 256
x is 512
=> nil
```




## 3.2 数据结构
Ruby的**数组字面量**形式如下，**用方括号括起来、以逗号分隔值**。这种数据结构在Python中叫做列表 `list` ：
```ruby
>> numbers = ['zero', 'one', 'two']
=> ["zero", "one", "two"]
>> numbers[1] # 支持[]取值
=> "one"
>> numbers.slice(1) # 也支持slice方法
=> "one"
>> numbers[3] # 访问不存在的值
=> nil
>> numbers.push('three', 'four') # 在数组末尾添加值
=> ["zero", "one", "two", "three", "four"]
>> numbers.drop(2) # 丢弃指定位置的值
=> ["two", "three", "four"]
```
**范围 `range`** 表示最小值和最大值之间的值集合，**范围字面量**的写法是**两个值之间加两个点**：
```ruby
>> ages = 1..35
=> 1..35
>> ages.entries
=> [1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35] 
>> ages.include?(36)
=> false
>> ages.include?(25)
=> true
```
**散列表 `hash`** 表示一个集合，其中每个键都对应一个值。这种数据结构在Python中叫做字典 `dict` ，其他语言中还有映射 `map` 、关联数组 `associative array` 等称呼。**散列表字面量**的写法是**大括号括起来、里面用逗号分隔的多个“键=>值”对**：
```ruby
>> fruit = {'a' => 'apple', 'b' => 'banana', 'c' => 'coconut'}
=> {"a"=>"apple", "b"=>"banana", "c"=>"coconut"}
>> fruit['b']
=> "banana"
>> fruit['d' ] = 'date'
=> "date"
>> fruit
=> {"a"=>"apple", "b"=>"banana", "c"=>"coconut", "d"=>"date"}
>> fruit['e'] # 查询不存在的值
=> nil
```
散列表也可以用**符号**作为键，此时Ruby提供了另一种书写键值对的语法，更为紧凑，类似于用于JavaScript对象的JSON格式，此时**散列表字面量中的符号前不加冒号**：
```ruby
>> dimensions = {width: 1000, height: 2250, depth: 250}
=> {:width=>1000, :height=>2250, :depth=>250}
>> dimensions[:height] # 查询时符号前加:
=> 2250
```
## 3.3 `proc`
一个 `proc` 是一段未经求值的代码，可以根据需要进行传递或者求值。也被称呼为**匿名函数**或者lambda函数。**`proc` 字面量**有多种写法，最紧凑的一种是 `-> 参数 { 函数体 }` 语法，调用时可以用 `proc.call(参数)` 或者 `proc[参数]` 语法：
```ruby
>> multiply = -> x, y { x * y }
=> #<Proc:0x0000022967ef2698 (irb):38 (lambda)>
>> multiply.call(6, 9) # .call语法调用
=> 54
>> multiply.call(2, 3) 
=> 6
>> multiply[3, 4] # 方括号调用
=> 12
```
# 4. 控制流
Ruby也有判断、分支、循环等结构，表现为 `if, case, while` 表达式，注意写法的不同，但都以通常的方式工作：
```ruby
?> if 2 < 3
?>   'less'
?> else
?>   'more'
>> end
=> "less"

?> quantify =
?> -> number {
?>   case number
?>   when 1
?>     'one'
?>   when 2
?>     'a couple'
?>   else
?>     'many'
?>   end
>> }
=> #<Proc:0x0000022968232290 (irb):53 (lambda)> 
>> quantify.call(2)
=> "a couple"
>> quantify[10]
=> "many"

>> x = 1
=> 1
?> while x < 1000
?>   x = x * 2
>> end
=> nil
>> x
=> 1024
```
# 5. 对象和方法
Ruby无疑很像一门动态语言，也是一门完全的面向对象语言——**每个值都是一个对象**，对象之间通过**发送消息**进行通信（这种风格来自于Smalltalk，对Ruby影响很深）；**每个对象都有自己的方法集合**，方法决定了它如何响应特定的消息。与此同时，**每个消息都有一个名字**，根据需要还可以有一些**参数**。一个对象收到一个消息时，**对应的方法**使用**消息的参数**作为**自己的参数**来执行。

可以毫不夸张的说，**Ruby完成全部工作的所有秘密都在此处**！比如 `1 + 2` 意味着使用参数 `2` 给对象 `1` 发送一个叫 `+` 的消息，而对象 `1` 处理这个消息的对应方法是 `#+` ，即理解为 `1.operator+(2)` 。

我们可以**向特殊内建对象 `Object` 发送 `new` 消息来新建一个 `Object` 类的对象**；创建新对象之后，**使用关键字 `def` 在对象上定义一个叫做 `#add` 的方法**—— `#add` 方法把它的两个参数相加并返回结果，**由于一个方法会自动返回最后执行的表达式的值，所以不需要一个显式的 `return` 语句**。接着使用 `2` 和 `3` 作为（消息的）参数向新对象发送 `add` 消息，然后 `#add` 方法使用消息的参数 `2` 和 `3` 执行，以响应 `add` 消息，最后就得到了想要的结果：
```ruby
>> o = Object.new 
=> #<Object:0x00000229687633b8>
?> def o.add(x, y)
?>   x + y
>> end
=> :add
>> o.add(2, 3)
=> 5
```
**一般来说**，发送消息时要写上**接收对象**比如 `o` 和**消息名** `add` 并用圆点分隔，有参数还要写出参数（例如 `o.add(2, 3)` ）。但是凡事皆有例外——Ruby会一直追踪作用域下的**当前对象** `self` ，**由于当前对象总是接收消息、执行对应方法的对象，在向当前对象发送消息时就只用写一个消息名，不必显式写出接收对象**。一个实际例子是，在一个特定对象的方法定义内部，在这个作用域内，向当前对象发送其他消息时只需要写一个消息名，接收对象不必显式提及。如果要向其他对象发送消息，就又回到**一般情况**了。
```ruby
>> o = Object.new
=> #<Object:0x00000229682b99e8> 
?> def o.add(x, y)
?>   x + y
>> end
=> :add
?> def o.add_twice(x, y)
?>   add(x, y) + add(x, y) 
?>   # 方法内部向o发送add消息时不用写成o.add(x,y),因为o是接收add_twice消息的对象
>> end
=> :add_twice
>> o.add_twice(2, 3)
=> 10
```
上述种种都是**发送消息**时的情况，那么定义消息时呢？一般来说，定义消息和方法时要以 `def` 开头、写上相关对象比如 `o` 和消息名 `add` 并用 `.` 分隔、有形式参数还要再加上括号和用 `,` 分隔的各个参数（例如 `def o.add(x, y)` ）。这样用关键字 `def` 就在对象 `o` 上定义了消息 `add` 和对应的方法 `#add` 。不过特殊的是，Ruby语言中有类似C函数的方法定义，不需要指明相关的对象——这又是哪跟哪儿啊？

在所有的方法定义之外，**当前对象是一个名叫 `main` 的特殊顶层对象**。可以说，**任何没有指明对象的消息与方法定义，都指向了 `main` ，都可以通过 `main` 使用**。不难注意到，此处的方法使用也不需要指明接收对象——或者说，由于处在当前对象 `main` 的作用域之中，**任何没有指明接收对象的消息都会默认发送给它**。
```ruby
?> def multiply(a, b)
?>   a * b
>> end
=> :multiply
>> multiply(2, 3)
=> 6
```
# 6. 类和模块
在Ruby中，通过把方法定义放到一个类中，然后通过给那个类发送 `new` 消息来新建对象，获得的对象是**包括方法在内**的这个类的实例。注意此处会把方法加到类的实例中，而非 `main` 里面。
```ruby
?> class Calculator
?>   def divide(x, y)
?>     x/y
?>   end
>> end
=> :divide
>> c = Calculator.new # 给Calculator类发送new消息
=> #<Calculator:0x0000022968250f60>
>> c.class
=> Calculator
>> c.divide(10, 2)
=> 5
>> divide(10, 2)
(irb):3:in `<main>': undefined method `divide' for main:Object (NoMethodError)
        from D:/Ruby30-x64/lib/ruby/gems/3.0.0/gems/irb-1.3.6/exe/irb:11:in `<top (required)>'
        from D:/Ruby30-x64/bin/irb:23:in `load'
        from D:/Ruby30-x64/bin/irb:23:in `<main>'
```
一个类可以**继承**来引入另一个类中的方法定义：
```ruby
?> class MultiplyCalculator < Calculator
?>   def multiply(x, y)
?>     x * y
?>   end
>> end
=> :multiply
>> mc = MultiplyCalculator.new
=> #<MultiplyCalculator:0x000001da7b32b250>
>> mc.class
=> MultiplyCalculator
>> mc.class.superclass # 查看一个类的超类
=> Calculator
>> mc.multiply(2, 10)
=> 20
>> mc.divide(10, 2)
=> 5
```
**子类中的方法**，通过 `super` 关键字可以调用**超类中的同名方法**。即子类方法中，如果要使用未被覆盖的超类方法，可以直接使用方法名调用；否则要通过使用 `super` 与否，区分是调用超类方法还是子类方法？？：
```ruby
?> class BinaryMultiplyCalculator < MultiplyCalculator
?>   def multiply(x, y)
?>     result = super(x, y)
?>     result.to_s(2)
?>   end
>> end
=> :multiply
>> bmc = BinaryMultiplyCalculator.new
=> #<BinaryMultiplyCalculator:0x000001da7b2f1a78>
>> bmc.multiply(10, 2)
=> "10100"
```
**如果在模块 `module` 中声明了方法定义，就能够被任意类包括进去**，从而共享方法定义：
```ruby
?> module Addition
?>   def add(x, y)
?>     x + y
?>   end
>> end
=> :add
?> class AddingCalculator
?>   include Addition
>> end
=> AddingCalculator
>> ac = AddingCalculator.new
=> #<AddingCalculator:0x000001da7b342a90>
>> ac.add(10, 2)
=> 12
```
## 局部变量和赋值
Ruby仅允许通过赋值声明局部变量。此外还可以**通过数组一次性给多个变量并行赋值**：
```ruby
>> greeting = 'hello'
=> "hello"
>> greeting
=> "hello"
>> width, height, depth = [1000, 2250, 250]
=> [1000, 2250, 250]
>> height
=> 2250
```
## 可变参数方法 `variadic method`
在定义方法时使用 `*` 运算符，支持数目可变的参数：
```ruby

```
注意，**一个方法定义只能有一个可变参数**，常规参数可以放到可变参数的前后：
```ruby

```

---
除了本文总结的两本书的知识外，

计算的本质：深入剖析程序和计算机
七周七语言：理解多种编程范型

如果想要学习更多关于Ruby的知识，可以看O'Reilly的《Ruby编程语言》`The Ruby Programming Language` 。
