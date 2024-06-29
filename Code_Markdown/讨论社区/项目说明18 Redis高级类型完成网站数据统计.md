### 1. 说明
HyperLogLog：
- 采用一种基数算法，用于完成**独立总数**的统计。比如一个人同一天多次访问算一个UV；或者7天重复访问算一个UV（将7天的7组数据合并，再统计合并后的重复数据的独立总数）
- 占据空间小，无论统计多少个数据，只占12K的内存空间。
- 不精确的统计算法，标准误差为0.81%

Bitmap：
- 不是一种独立的数据结构，实际上就是字符串
- 支持按位存取数据，可以看出是byte数组
- 适合存取大量的连续数据的布尔值，比如某个用户一年中每天的签到情况，一个0/1就可记录一天的签到情况

bitmap就是String！！！
```java
/**  
 * 统计20万个有重复数据的独立总数  
 */  
@Test  
public void testHyperLogLog() {  
    String redisKey = "test:hll:01";  
    for (int i = 1; i <= 100000; ++i) {  
        redisTemplate.opsForHyperLogLog().add(redisKey, i);  
    }  
    for (int i = 1; i <= 100000; ++i) {  
        int r = (int) (Math.random() * 100000 + 1);  
        redisTemplate.opsForHyperLogLog().add(redisKey, r);  
    }  
    System.out.println(redisTemplate.opsForHyperLogLog().size(redisKey));  
}
```
结果是99553。

```java
/**  
 * 合并多组独立总数的重复计数  
 */  
@Test  
public void testHyperLogLogUnion() {  
    String redisKey2 = "test:hll:02";  
    for (int i = 1; i <= 10000; ++i)  
        redisTemplate.opsForHyperLogLog().add(redisKey2, i);  
    String redisKey3 = "test:hll:03";  
    for (int i = 5001; i <= 15000; ++i)  
        redisTemplate.opsForHyperLogLog().add(redisKey3, i);  
    String redisKey4 = "test:hll:04";  
    for (int i = 10001; i <= 20000; ++i)  
        redisTemplate.opsForHyperLogLog().add(redisKey4, i);  
    String unionKey = "test:hll:union";  
    redisTemplate.opsForHyperLogLog().union(unionKey, redisKey2, redisKey3, redisKey4);  
    System.out.println(redisTemplate.opsForHyperLogLog().size(unionKey));  
}
```
结果是19833.

```java
/**  
 * 测试BitMap  
 */
@Test  
public void testBitMap() {  
    String redisKey = "test:bm:01";  
    // 记录  
    redisTemplate.opsForValue().setBit(redisKey, 1, true);  
    redisTemplate.opsForValue().setBit(redisKey, 4, true);  
    redisTemplate.opsForValue().setBit(redisKey, 7, true);  
    // 查询  
    System.out.println(redisTemplate.opsForValue().getBit(redisKey, 0));  
    System.out.println(redisTemplate.opsForValue().getBit(redisKey, 1));  
    System.out.println(redisTemplate.opsForValue().getBit(redisKey, 2));  
  
    // 统计  
    Object obj = redisTemplate.execute(new RedisCallback<Object>() {  
        @Override  
        public Object doInRedis(RedisConnection connection) throws DataAccessException {  
            return connection.bitCount(redisKey.getBytes());  
        }  
    });  
    System.out.println(obj);  
}
```
结果是：
```java
false
true
false
3
```
```java
/**  
 * 测试BitMap: 统计3组数据的布尔值，并对3组数据做OR运算  
 */  
@Test  
public void testBitMap2() {  
    String redisKey2 = "test:bm:02";  
    // 记录  
    redisTemplate.opsForValue().setBit(redisKey2, 0, true);  
    redisTemplate.opsForValue().setBit(redisKey2, 1, true);  
    redisTemplate.opsForValue().setBit(redisKey2, 2, true);  
    String redisKey3 = "test:bm:03";  
    // 记录  
    redisTemplate.opsForValue().setBit(redisKey3, 2, true);  
    redisTemplate.opsForValue().setBit(redisKey3, 3, true);  
    redisTemplate.opsForValue().setBit(redisKey3, 4, true);  
    String redisKey4 = "test:bm:04";  
    // 记录  
    redisTemplate.opsForValue().setBit(redisKey4, 4, true);  
    redisTemplate.opsForValue().setBit(redisKey4, 5, true);  
    redisTemplate.opsForValue().setBit(redisKey4, 6, true);  
  
    String redisKey = "test:bm:or";  
    // 统计  
    Object obj = redisTemplate.execute(new RedisCallback<Object>() {  
        @Override  
        public Object doInRedis(RedisConnection connection) throws DataAccessException {  
            connection.bitOp(RedisStringCommands.BitOperation.OR,  
                    redisKey.getBytes(), redisKey2.getBytes(), redisKey3.getBytes(), redisKey4.getBytes());  
            return connection.bitCount(redisKey.getBytes());  
        }  
    });  
    System.out.println(obj);  
}
```
结果为7.

### 1. 网站数据统计UV和DAU
UV：Unique Visitor——独立访客，需要通过用户IP去重统计数据。每次访问都要进行统计，无需用户登录。类似 浏览量 。使用HyperLogLog，性能好，存储空间小。

DAU：Daily Active User——日活跃用户，需要通过用户ID去重统计数据。每天访问过一次则认为其活跃。使用BitMap，性能好，可以统计精确的结果。（类似登录过后才能进行的功能……）
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202304171118172.png)

### 2. Redis代码
```java
private static final String PREFIX_UV = "uv"; // 独立访客  
private static final String PREFIX_DAU = "dau"; // 每日活跃用户

// 单日UV
public static String getUVKey(String date) {
	return PREFIX_UV + SPLIT + date;
}

// 区间UV
public static String getUVKey(String startDate, String endDate) {
	return PREFIX_UV + SPLIT + startDate + SPLIT + endDate;
}
    
// 单日活跃用户  
public static String getDAUKey(String date) {  
    return PREFIX_DAU + SPLIT + date;  
}  
  
// 区间活跃用户  
public static String getDAUKey(String startDate, String endDate) {  
    return PREFIX_DAU + SPLIT + startDate + SPLIT + endDate;  
}
```
### 3. 业务层代码 StatisticsService
```java
package com.nowcoder.community.service;  
  
import com.nowcoder.community.util.RedisKeyUtil;  
import org.springframework.beans.factory.annotation.Autowired;  
import org.springframework.dao.DataAccessException;  
import org.springframework.data.redis.connection.RedisConnection;  
import org.springframework.data.redis.connection.RedisStringCommands;  
import org.springframework.data.redis.core.RedisCallback;  
import org.springframework.data.redis.core.RedisTemplate;  
import org.springframework.stereotype.Service;  
  
import java.text.SimpleDateFormat;  
import java.util.ArrayList;  
import java.util.Calendar;  
import java.util.Date;  
import java.util.List;  
  
@Service  
public class StatisticsService {  
    @Autowired  
    private RedisTemplate redisTemplate;  
  
    private SimpleDateFormat df = new SimpleDateFormat("yyyyMMdd");  
  
    // 将指定的IP计入UV  
    public void recordUV(String ip) {  
        String redisKey = RedisKeyUtil.getDAUKey(df.format(new Date()));  
        redisTemplate.opsForHyperLogLog().add(redisKey, ip);  
    }  
  
    // 统计指定区间范围内的UV  
    public long calculateUV(Date start, Date end) {  
        if (start == null || end == null) throw new IllegalArgumentException("参数不能为空！");  
        // 整理该日期范围内的key  
        List<String> keyList = new ArrayList<>();  
        Calendar calendar = Calendar.getInstance();  
        calendar.setTime(start);  
        while (!calendar.getTime().after(end)) { // 以天为单位进行统计  
            String key = RedisKeyUtil.getUVKey(df.format(calendar.getTime()));  
            keyList.add(key);  
            calendar.add(Calendar.DATE, 1);  
        }  
        // 合并这些数据  
        String redisKey = RedisKeyUtil.getUVKey(df.format(start), df.format(end)); // 合并数据的UVKey  
        redisTemplate.opsForHyperLogLog().union(redisKey, keyList.toArray());  
        // 返回统计结果  
        return redisTemplate.opsForHyperLogLog().size(redisKey);  
    }  
  
    // 将指定用户计入DAU  
    public void recordDAU(int userId) {  
        String redisKey = RedisKeyUtil.getDAUKey(df.format(new Date()));  
        redisTemplate.opsForValue().setBit(redisKey, userId, true); // 某天某个用户登录了(就是活跃用户)  
    }  
  
    // 统计指定区间范围内的DAU,这段时间内活跃一天就是活跃用户  
    public long calculateDAU(Date start, Date end) {  
        if (start == null || end == null) throw new IllegalArgumentException("参数不能为空！");  
        // 整理该日期范围内的key  
        List<byte[]> keyList = new ArrayList<>();  
        Calendar calendar = Calendar.getInstance();  
        calendar.setTime(start);  
        while (!calendar.getTime().after(end)) { // 以天为单位进行统计  
            String key = RedisKeyUtil.getDAUKey(df.format(calendar.getTime()));  
            keyList.add(key.getBytes());  
            calendar.add(Calendar.DATE, 1);  
        }  
        System.out.println(keyList);  
        // 返回统计结果  
        // 进行OR运算  
        return (long) redisTemplate.execute(new RedisCallback() {  
            @Override  
            public Object doInRedis(RedisConnection connection) throws DataAccessException {  
                String redisKey = RedisKeyUtil.getDAUKey(df.format(start), df.format(end)); // 合并这些数据  
  
                connection.bitOp(RedisStringCommands.BitOperation.OR,  
                        redisKey.getBytes(), keyList.toArray(new byte[0][0]));  
                return connection.bitCount(redisKey.getBytes());  
            }  
        });  
    }  
}
```
由于是每次请求都需要记录数据，所以要用拦截器：实现拦截器：
```java
package com.nowcoder.community.interceptor;  
  
import com.nowcoder.community.entity.HostHolder;  
import com.nowcoder.community.entity.User;  
import com.nowcoder.community.service.StatisticsService;  
import org.springframework.beans.factory.annotation.Autowired;  
import org.springframework.stereotype.Component;  
import org.springframework.web.servlet.HandlerInterceptor;  
  
import javax.servlet.http.HttpServletRequest;  
import javax.servlet.http.HttpServletResponse;  
  
@Component  
public class StatisticsInterceptor implements HandlerInterceptor {  
    @Autowired  
    private StatisticsService statisticsService;  
  
    @Autowired  
    private HostHolder hostHolder;  
  
    @Override  
    public boolean preHandle(HttpServletRequest request, HttpServletResponse response, Object handler) throws Exception {  
        // 统计UV    
String ip = request.getRemoteHost();  
        statisticsService.recordUV(ip); // 不管是否登录都要记录  
        // 统计DAU    
User user = hostHolder.getUser();  
        if (user != null) { // 用户登录才统计    
statisticsService.recordDAU(user.getId());  
        }  
        return true;  
    }  
}
```
拦截器配置：
```java
registry.addInterceptor(statisticsInterceptor)  
        .excludePathPatterns("/**/*.css", "/**/*.js", "/**/*.png", "/**/*.jpg", "/**/*.jpeg");
```

---
### 4. 控制层代码
由于点击页面上的开始统计时，会传递两个参数——起始和终止日期，我们用POST请求：
```java
package com.nowcoder.community.controller;  
  
import com.nowcoder.community.service.StatisticsService;  
import org.springframework.beans.factory.annotation.Autowired;  
import org.springframework.format.annotation.DateTimeFormat;  
import org.springframework.stereotype.Controller;  
import org.springframework.web.bind.annotation.RequestMapping;  
import org.springframework.web.bind.annotation.RequestMethod;  
  
import java.time.LocalDate;  
  
@Controller  
public class StatisticsController {  
    @Autowired  
    private StatisticsService statisticsService;  
  
    // 统计页面  
    @RequestMapping(path = "/data", method = {RequestMethod.GET, RequestMethod.POST})  
    public String getDataPage() {  
        return "/site/admin/data";  
    }  
  
    // 统计页面上的网站UV  
    @RequestMapping(path = "/data/uv", method = RequestMethod.POST)  
    public String getUV(@DateTimeFormat(pattern = "yyyy-MM-dd") LocalDate start,  
                        @DateTimeFormat(pattern = "yyyy-MM-dd") LocalDate end, Model model) {  
        long uv = statisticsService.calculateUV(start, end);  
        model.addAttribute("uvResult", uv);  
        model.addAttribute("uvStartDate", start);  
        model.addAttribute("uvEndDate", end);  
        // return "/site/admin/data";  
        return "forward:/data"; // 只能处理一部分，还需要其他同级方法继续处理; 转发是在一个请求内完成的，转给其他方法时请求类型不变  
    }  
   
  
    // 统计页面上的网站UV  
    @RequestMapping(path = "/data/dau", method = RequestMethod.POST)  
    public String getDAU(@DateTimeFormat(pattern = "yyyy-MM-dd") LocalDate start,  
                        @DateTimeFormat(pattern = "yyyy-MM-dd") LocalDate end, Model model) {  
        long dau = statisticsService.calculateDAU(start, end);  
        model.addAttribute("dauResult", dau);  
        model.addAttribute("dauStartDate", start);  
        model.addAttribute("dauEndDate", end);  
        // return "/site/admin/data";  
        return "forward:/data"; // 只能处理一部分，还需要其他同级方法继续处理; 转发是在一个请求内完成的，转给其他方法时请求类型不变  
    }  
}
```
前端表单代码：
```html
<div class="container pl-5 pr-5 pt-3 pb-3 mt-3">  
   <h6 class="mt-3"><b class="square"></b> 网站 UV</h6>  
   <form class="form-inline mt-3" method="post" th:action="@{/data/uv}">  
      <input type="date" class="form-control" required name="start" th:value="${#temporals.format(uvStartDate, 'yyyy-MM-dd')}"/>  
      <input type="date" class="form-control ml-3" required name="end" th:value="${#temporals.format(uvEndDate, 'yyyy-MM-dd')}"/>  
      <button type="submit" class="btn btn-primary ml-3">开始统计</button>  
   </form>   <ul class="list-group mt-3 mb-3">  
      <li class="list-group-item d-flex justify-content-between align-items-center">  
         统计结果  
         <span class="badge badge-primary badge-danger font-size-14" th:text="${uvResult}">0</span>  
      </li>   </ul></div>  
<!-- 活跃用户 -->  
<div class="container pl-5 pr-5 pt-3 pb-3 mt-4">  
   <h6 class="mt-3"><b class="square"></b> 活跃用户</h6>  
   <form class="form-inline mt-3" method="post" th:action="@{/data/dau}">  
      <input type="date" class="form-control" required name="start" th:value="${#temporals.format(dauStartDate, 'yyyy-MM-dd')}"/>  
      <input type="date" class="form-control ml-3" required name="end" th:value="${#temporals.format(dauEndDate, 'yyyy-MM-dd')}"/>  
      <button type="submit" class="btn btn-primary ml-3">开始统计</button>  
   </form>   <ul class="list-group mt-3 mb-3">  
      <li class="list-group-item d-flex justify-content-between align-items-center">  
         统计结果  
         <span class="badge badge-primary badge-danger font-size-14" th:text="${dauResult}">0</span>  
      </li>   </ul></div>
```
权限现在：只有管理员能访问它。

SecurityConfig：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202304171600358.png)

```java
```