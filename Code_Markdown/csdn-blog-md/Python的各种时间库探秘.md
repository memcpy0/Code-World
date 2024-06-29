# Python的时间库探秘
我们一定会有许多的时间想要去追踪，去显示，为此要掌握Python的时间库。
## time文档
```python
In [2]: dir(time)
Out[2]:
['_STRUCT_TM_ITEMS', '__doc__', '__loader__', 
'__name__', '__package__', '__spec__', 
# 变量
'altzone', 'timezone', 'asctime', 'daylight', 'tzname', 
# 下面是要注意的函数
'clock', 'ctime', 'get_clock_info', 'gmtime', 
'localtime', 'mktime', 'monotonic', 'perf_counter', 
'process_time', 'sleep', 'strftime', 
'strptime', 'struct_time', 'time']
```
```python
In [3]: help(time)
Help on built-in module time:
NAME
    time - This module provides various functions to 
    manipulate time values. #（控制时间值）

DESCRIPTION
    There are two standard representations of time. #（两种时间标准）  
    One is the number of seconds since 
	the Epoch, in UTC (a.k.a. GMT).  
    It may be an integer or a floating point number 
    (to represent fractions of seconds). #（UTC时间/秒）
    The Epoch is system-defined;  #（Unix系统定义）
    on Unix, it is generally January 1st, 1970. 
    The actual value can be retrieved by calling gmtime(0).
	# 也就是说Epoch新纪元的值可以通过time.gmtime(0)恢复成实际值

    The other representation is a tuple of 9 integers 
    giving local time.  #（9个整数的元组，给出当地时间）
    The tuple items are:
      year (including century, e.g. 1998)
      month (1-12)
      day (1-31)
      hours (0-23)
      minutes (0-59)
      seconds (0-59)
      weekday (0-6, Monday is 0)
      Julian day (day in the year, 1-366)
	  DST (Daylight Savings Time) flag (-1, 0 or 1)
    If the DST flag is 0, the time is given 
    in the regular time zone;
    if it is 1, the time is given in the DST time zone;
    if it is -1, mktime() should guess based on 
    the date and time.
```

### time库时间表示方法
上面是**关于time库中表示时间的方法**，官方给出了2种：

1. 从`1970-01-01 00:00:00 UTC(GMT)`(格林尼治时间)开始到现在所经历的时间，以浮点数的`"秒"`来表示。
```
# 调用函数
 time(...)
     time() -> floating point number
     # 返回一个浮点数，代表着从Epoch基准开始到现在为止的(用秒表示)的时间
     # 也就代表现在的时间
     Return the current time in seconds since the Epoch.
	 Fractions of a second may be present if the system clock  
	 provides them.
```
```
 In [4]: import time
 In [5]: time.time()
 Out[5]: 1525085955.0463219
```
> UNIX及Linux的时间系统是由「新纪元时间」Epoch开始计算起，单位为秒，Epoch则是**指定为1970年一月一日凌晨零点零分零秒**，格林威治时间。  

关于更多Unix时间戳的事，看[这里](https://blog.csdn.net/lanmolei814/article/details/34115781)。
2. 用结构化的时间组（year,month,day,hours,minutes,seconds....）来表示从1970-01-01 00:00:00 UTC开始到现在所经历的时间.
> tm_sec表「秒」数，在[0,61]之间，多出来的两秒是用来处理跳秒问题用的
> tm_min表「分」数，在[0,59]之间
> tm_hour表「时」数，在[0,23]之间
> tm_mday表「本月第几日」，在[1,31]之间
> tm_mon表「本年第几月」，在[0,11]之间
> tm_year要加1900表示那一年
> tm_wday表「本年第几日」，在[0,6]之间
> tm_yday表「本年第几日」，在[0,365]之间，闰年有366日
> tm_isdst表是否为「日光节约时间」
```
# 调用函数
gmtime(...) 
     gmtime([seconds]) # 一个可选参数 -> 
     (tm_year, tm_mon, tm_mday, tm_hour, tm_min, 
     m_sec, tm_wday, tm_yday, tm_isdst)
     # 将从Epoch开始的秒值转换成代表着UTC时间的元组
     Convert seconds since the Epoch to 
     a time tuple expressing UTC (a.k.a.GMT).
     # 没有传递参数，就转换现在的时间为相应元组  
     When 'seconds' is not passed in, 
     convert the current time instead.

     If the platform supports the tm_gmtoff and tm_zone, 
     they are available as attributes only.
```
```
In [7]: time.gmtime(0)
Out[7]: time.struct_time
(tm_year=1970, tm_mon=1, tm_mday=1, tm_hour=0, 
tm_min=0, tm_sec=0, tm_wday=3, tm_yday=1, tm_isdst=0)

In [8]: time.gmtime(time.time())
Out[8]: time.struct_time
(tm_year=2018, tm_mon=4, tm_mday=30, tm_hour=11, 
tm_min=7, tm_sec=46, tm_wday=0, tm_yday=120, tm_isdst=0)

# 等价于：(年、月、日、时、分、秒……)
In [10]: time.gmtime()
Out[10]: time.struct_time
(tm_year=2018, tm_mon=4, tm_mday=30, tm_hour=11, 
tm_min=13, tm_sec=43, tm_wday=0, tm_yday=120, tm_isdst=0)
```
### time库变量
```
Variables:

  timezone -- difference in seconds 
    between UTC and local standard time
	
  altzone -- difference in  seconds 
	between UTC and local DST time
	
  daylight -- whether local time should reflect DST

  tzname -- tuple of (standard time zone name, 
	DST time zone name)
```
### time库函数
```python
Functions:

# 已介绍  time() 
-- return current time in seconds since the Epoch as a float
    
# 已介绍  gmtime() 
-- convert seconds since Epoch to UTC tuple
  
#  asctime() -- convert time tuple to string
#  ctime() -- convert time in seconds to string
 
#  clock() -- return CPU time since process start as a float
#  sleep() -- delay for a number of seconds given as a float
  
#  localtime() -- convert seconds since Epoch 
                 to local time tuple

#  mktime() -- convert local time tuple to 
              seconds since Epoch

#  strftime() -- convert time tuple to string 
	            according to format specification
#  strptime() -- parse string to time tuple 
                according to format specification

  tzset() -- change the local timezone ？？无该函数的文档
  
```
1. 第三种时间显示方法：更直观。
```python
asctime(...)
   asctime([tuple]) -> string

   Convert a time tuple to a string, 
   e.g. 'Sat Jun 06 16:26:11 1998'.
   # time()和gmtime()都代表着UTC时间
   # asctime()转换时间为字符串
   # 参数可选，无时就使用localtime()传回的时间元组
   When the time tuple is not present, 
   current time as returned by localtime() is used.
```
```python
In [11]: tup = time.gmtime()
In [12]: time.asctime(tup)
Out[12]: 'Mon Apr 30 11:32:18 2018'

In [13]: time.asctime()
Out[13]: 'Mon Apr 30 19:39:49 2018'
```
2.将时间直接转换为当地时间，比上面的进了一步。但同样是**用字符串string类型表示时间**。
```python
ctime(...)
   ctime(seconds) -> string

   Convert a time in seconds since the Epoch 
   to a string in local time.
   This is equivalent to asctime(localtime(seconds)). 
   When the time tuple is not present, 
   current time as returned by localtime() is used.
```
```python
In [5]: time.ctime()
Out[5]: 'Mon Apr 30 19:54:31 2018'
```
3.用结构化的时间组，表示本地时间，比起`gmtime()`进了一步。
```python
localtime(...)
    localtime([seconds]) -> 
        (tm_year,tm_mon,tm_mday,tm_hour,tm_min,
         tm_sec,tm_wday,tm_yday,tm_isdst)

     Convert seconds since the Epoch to 
     a time tuple expressing local time.
     When 'seconds' is not passed in, 
     convert the current time instead.
```
```python
In [4]: time.localtime()
Out[4]: time.struct_time
(tm_year=2018, tm_mon=4, tm_mday=30, 
tm_hour=19, tm_min=53, tm_sec=11, 
tm_wday=0, tm_yday=120, tm_isdst=0)

In [6]: type(time.localtime())
Out[6]: time.struct_time
```
4.`time.clock()`返回**程序运行**的**整个时间段中CPU运行的时间**。
```python
clock(...)
    clock() -> floating point number

    Return the CPU time or real time 
    since the start of the process or 
    since the first call to clock().  
    This has as much precision as the system records.
```
```python
def run2():
    start = time.clock()
    for i in range(1000):
        j = i * 2 
        for k in range(j):
            t = k
            print(t)
    end = time.clock()
    print('CPU执行时间: ',end - start)
```
```python
……
1993
1994
1995
1996
1997
CPU执行时间: 272.87826451604906
```
5.`time.sleep()`爬虫中常用，让程序暂停执行指定的秒数，如`time.sleep(2)`
```python
sleep(...)
    sleep(seconds)

    Delay execution for a given number of seconds.  
    The argument may be a floating point number 
    for subsecond precision.
```
6.`time.mktime()`将**本地时间列表转化为浮点数的秒**来表示
```python
mktime(...)
    mktime(tuple) -> floating point number

    Convert a time tuple in local time to seconds since the Epoch.
    Note that mktime(gmtime(0)) will not generally return zero for most
    time zones; 
    instead the returned value will either be equal to that
    of the timezone or altzone attributes on the time module.
```
```python
In [18]: time.mktime(time.localtime())
Out[18]: 1525090743.0
```
7.`time.strftime()`将时间组时间转化为**指定格式**的String类，格式必须指定！
```python
strftime(...)
    strftime(format[, tuple]) -> string

    Convert a time tuple to a string according 
    to a format specification.
    See the library reference manual for formatting codes. 
    When the time tuple is not present, current time 
    as returned by localtime() is used.

    Commonly used format codes:

      %Y  Year with century as a decimal number.
      %m  Month as a decimal number [01,12].
      %d  Day of the month as a decimal number [01,31].
      %H  Hour (24-hour clock) as a decimal number [00,23].
      %M  Minute as a decimal number [00,59].
      %S  Second as a decimal number [00,61].
      %z  Time zone offset from UTC.
      %a  Locale's abbreviated weekday name.
      %A  Locale's full weekday name.
      %b  Locale's abbreviated month name.
      %B  Locale's full month name.
      %c  Locale's appropriate date and time representation.
      %I  Hour (12-hour clock) as a decimal number [01,12].
      %p  Locale's equivalent of either AM or PM.

      Other codes may be available on your platform.  
      See documentation for the C library strftime function.
```
```python
In [15]: time.strftime('%Y-%m-%d %H:%M:%S', time.localtime())
Out[15]: '2018-04-30 20:14:15'
```

8.`time.strptime()`将String类时间**转化为时间组格式**。可以自己设置格式，有多种。
```python
strptime(...)
    strptime(string, format) -> struct_time

    Parse a string to a time tuple 
    according to a format specification.
    See the library reference manual for 
    formatting codes (same as strftime()).

    Commonly used format codes:

      %Y  Year with century as a decimal number.
      %m  Month as a decimal number [01,12].
      %d  Day of the month as a decimal number [01,31].
      %H  Hour (24-hour clock) as a decimal number [00,23].
      %M  Minute as a decimal number [00,59].
      %S  Second as a decimal number [00,61].
      %z  Time zone offset from UTC.
      %a  Locale's abbreviated weekday name.
      %A  Locale's full weekday name.
      %b  Locale's abbreviated month name.
      %B  Locale's full month name.
      %c  Locale's appropriate date and time representation.
      %I  Hour (12-hour clock) as a decimal number [01,12].
      %p  Locale's equivalent of either AM or PM.

      Other codes may be available on your platform.  
      See documentation for the C library strftime function.
```
```python
In [14]: time.strptime(time.ctime())
Out[14]: time.struct_time
(tm_year=2018, tm_mon=4, tm_mday=30, tm_hour=20, 
tm_min=10, tm_sec=10, tm_wday=0, tm_yday=120, tm_isdst=-1)
```

此外，还有一些函数，可以自己去学习和运用。

## datetime文档
```python
In [2]: dir(datetime)
Out[2]:
['MAXYEAR', 'MINYEAR',
 '__builtins__', '__cached__', '__doc__', '__file__',
 '__loader__', '__name__', '__package__', '__spec__',
 '_divide_and_round',
 'date', 'datetime', 'datetime_CAPI', 'time', 
 'timedelta', 'timezone', 'tzinfo']
```
### datetime示例
```python
In [7]: from datetime import datetime
In [8]: now = datetime.now()
In [9]: print(now)    # print out the current date and time 
2018-05-05 08:34:12.248013              

In [10]: print(now.year,now.month, now.day)
2018 5 5

# we need to print today's date in the following format
# mm-dd-yyyy
In [11]: print('%02d-%02d-%04d' % (now.day, now.month, now.year))
05-05-2018
# %02d pads the numbers with zeros to two places
In [12]: print('%2d-%2d-%4d' % (now.day, now.month, now.year))
 5- 5-2018

# hh:mm:ss
In [13]: print('%02d:%02d:%2d' % (now.hour, now.minute, now.second))
08:34:12

In [15]: print('%02d/%02d/%04d %2d:%2d:%2d' % \
(now.month, now.day, now.year, now.hour, now.minute, now.second))
05/05/2018  8:34:12
```
```python
# datetime.datetime.now()的文档
now(tz=None) from builtins.type
      Returns new datetime object representing current time local to tz.
	  tz
         Timezone object.
	  If no tz is specified, uses local timezone.
```
datetime比起time库来说更加复杂，姑且先学到这里吧。

