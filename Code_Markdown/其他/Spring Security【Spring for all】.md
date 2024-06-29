> 现在要集中精力提高性能和安全性。

[Spring Security](https://spring.io/projects/spring-security)是一个专注于为Java应用程序提供身份**认证**（判断有没有登录）和**授权**（认证后进一步判断，有没有访问功能的权限）的框架，它的强大之处在于**它可以轻松扩展以满足自定义的需求**。

特征：
- 对身份的认证和授权提供全面的、可扩展的支持
- 防止各种功能，如会话固定攻击、点击劫持、CSRF攻击等
- 支持与Servlet API（即JavaEE）、**Spring MVC**等Web技术集成。

Spring MVC的核心组件是DispatcherServlet，它把请求分发给不同的控制器——一个应用中只有一个DispatcherServlet、但有多个控制器。在请求被DispatcherServlet分发给Controller之间，可以有多个拦截器拦截并处理请求。==**Controller和Interceptor是SpringMVC特有的组件**；而DispatcherServlet是满足Java EE Servlet接口规范的一个实现类==：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202304172006201.png)
Java EE还有一个核心组件和标准，即过滤器Filter（它和DispatcherServlet之间的关系，和Interceptor与Controller之间的关系是一样的，过滤器可以拦截对DispatcherServlet的访问，而拦截器可以拦截对Controller的访问。Filter也可以有多个，每个解决一方面的问题：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202304172012856.png)
Spring Security底层利用统一的机制处理系统的权限——它利用了JavaEE的规范，即Filter，大概有11个Filter。分别验证账号密码、退出、存储认证权限的……底层用Filter对请求拦截，做权限管理。

Spring Security对权限的判断很靠前。

Spring Securiy是Spring所有模块中最复杂的，它底层的机制很复杂。

### 1. 依赖导入
```xml
<dependency>
	<groupId>org.springframework.boot</groupId>
	<artifactId>spring-boot-starer-security</artifactId>
</dependency>
```
### 2. ???
导入依赖、重启项目后，发现没有登录，原来可以访问的页面就变为登录页面（自带的），还自行生成密码：![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202304172048902.png)
用户 -- 多对多 -- 角色 -- 多对多 -- 权限

之前的User的type：普通用户 版主 管理员。Security授权时要的是一个字符串，明确表达权限的含义。一个做法是，让User实体类实现UserDetail接口（Security标准的做法），一般都默认为true：
```java
// User类中
// true: 账号未过期
@Override
public boolean isAccountNonExpired() {
	return true; // 认证的账号不做过期处理
}
// true: 账号未锁定
@Override
public boolean isAccountNonLocked() {
	return true;
}
// true: 凭证未过期
@Override
public boolean isCredentialsNonExpired() {
	return true;
}
// true: 账号可用
@Override
public boolean isEnable() {
	return true; 
}
```
关键方法：
```java
// 每个用户可以有多个权限
// 现在调用这个方法就能知道该用户的权限
@Override
public Collection<? extends GrantedAuthority> getAuthorities() {
	List<GrantedAuthority> list = new ArrayList<>();
	list.add(new GrantedAuthority() {
		@Override
		public String getAuthority() {
			// 通过type判断，不同type返回不同权限名
			switch (type) {
			case 1: return "ADMIN"; // 管理员
			case 2: return "USER"; // 普通用户
			}
		}
	});
	return list; 
}
```
通常还让UserService实现接口UserDetailsService（的方法），security在检查登录时要用这个方法，即检查用户名是否匹配：
```java
@Override
public UserDetails loadUserByUsername(String username) throws UsernameNotFoundException {
	return this.findUserByName(username); // return userMapper.selectByName(username);
}
```
我们只需写一个配置类SpringConfig，就可解决所有的验证和授权需求：
- AuthenticationManager: 认证的核心接口
- AuthenticationManagerBuilder：用于构建AuthenticationManager对象的工具。
- ProviderManager: AuthenticationManager接口的默认实现类，是Builder默认构建的对象
  ProviderManager持有一组AuthenticationProvider,每个AuthenticationProvider负责一种认证 `private List<AuthenticationProvider> providers;`
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202304180016269.png)
- AuthenticationProvider：委托模式：ProviderManager将认证委托给AuthenticationProvider		![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202304180019617.png)

```java
@Configuration
public class SecurityConfig extends WebSecuriyConfigurerAdapter {
	@Autowired
	private UserService userService;
	@Override
	public void configure(WebSecurity web) throws Exception {
		// 忽略静态资源的访问，以提高性能
		web.ignoring().antMatches("/resources/**"); // 这些静态资源也可以写个HTTP请求拦截
	} 
	@Override
	protected void configure(AuthenticationManagerBuilder auth) throws Exception {
		// 内置的认证规则
		// auth.userDetailsService(userService).passwordEncoder(new Pbkdf2PasswordEncoder("12345"));

		// 自定义认证规则 
		auth.authenticationProvider(new AuthenticationProvider() {
			// Authentication: 用于封装认证信息的接口,不同的实现类代表不同类型的认证信息
			@Override
			public Authentication authenticate(Authentication authentication) throws AuthenticationException {
				String username = authentication.getName();
				String password = (String) authentication.getCredentials();
				User user = userService.findUserByName(username);
				if (user == null) {
					throw new UsernameNotFoundException("账号不存在！");
				}
				password = CommunityUtil.md5(password + user.getSalt());
				if (!user.getPassword().equals(password)) {
					throw new BadCredentialException("密码不正确！");
				}
				// principal: 主要信息; credentials: 证书; authorities: 权限
				return new UsernamePasswordAuthenticationToken(user, user.getPassword(), user.getAuthorities());
			}
			// 当前的AuthenticationProvider支持哪种类型的认证
			@Override
			public boolean supports(Class<?> aClass) {
				// UsernamePasswordAuthenticationToken: Authentication接口的常用的实现类
				return UsernamePasswordAuthenticationToken.class.equals(aClass);
			}
		});
	}

	@Override
	protected void configure(HttpSecurity http) throws Exception {
		// 登录相关配置
		http.formLogin()
			.loginPage("/loginpage")
			.loginProcessingUrl("/login")
			.successHandler(new AuthenticationSuccessHandler() {
				@Override
				public void onAuthenticationSuccess(HttpServletRequest request, HttpServletResponse response, Authentication auth) {
					response.sendRedirect(request.getContextPath() + "/index"); // 重定向到首页
				}
			})
			.failureHandler(new AuthenticationFailureHandler() {
				@Override
				public void onAuthenticationFailure(HttpServletRequest request, HttpServletResponse response, Authentication auth) {
					request.setAttribute("error", e.getMessage());
					request.getRequestDispatcher("/loginpage").forward(request, response));
				} // 请求失败后，这里不能重定向，因为重定向会重新开始一个新的请求；转发则会保持在一个请求中
			});
		// 退出相关配置
		http.logout()
			.logoutUrl("/logout")
			.logoutSuccessHandler(new LogoutSuccessHandler() {
				@Override
				public void onLogoutSuccess(HttpServletRequest request, HttpServletResponse response, Authentication auth) {
					response.sendRedirect(request.getContextPath() + "/index");
				}
			});
			
		// 授权配置
		http.authorizedRequests()
			.antMathcers("/letter").hasAnyAuthority("USER", "ADMIN")
			.antMatchers("/admin").hasAnyAuthority("ADMIN")
			.and().exceptionHandling().accessDeniedPage("/denied"); // 拒绝访问时的提示页面
		// 验证码要在账号密码前验证，验证码不对就不用检查账号密码了
		// 增加Filter,处理验证码
		http.addFilterBefore(new Filter() {
			@Override
			public void doFilter(ServiceRequest servletRequest, ServletResponse servletResponse, FilterChain filterChain) {
				HttpServletRequest request = (HttpServletReques)servletRequest;
				HttpServletResponse response = (HttpServletResponse) servletResponse;
				if (request.getServletPath().equals("/login")) {
					String verifyCode = request.getParameter("verifyCode");
					if (verifyCode == null || !verifyCode.equalsIgnoreCase("1234")) {
						request.setAttribute("error", "验证码错误！");
						request.getRequestDispatcher("/loginpage").forward(request, response);
						return;
					}
					// 让请求继续向下执行
					filterChain.doFilter(request, response);
				}
			}
		}, UsernamePasswordAuthenticationFilter.class); // 在处理账号密码的底层Filter前加过滤器
	});
	// 记住我
	http.rememberMe() // 记住我帮助绕过了登录认证的过程，相当于自动登录了
		.tokenRepository(new InMemoryTokenRepositoryImpl()) // 把用户信息存到内存里面,可自己实现接口tokenRepository
		.tokenValiditySeconds(3600 * 24) // 过期时间
		.userDetailsService(userService); // 下次登录后，从内存中根据凭证得到用户信息，然后去查询用户的完整信息，自动过认证
}
```
- 重定向要有两次请求，不好在两个组件间共用数据、不好传参，只能用跨组件的Cookie或Session这些；转发时，A可以把数据存到Request中，B可以从Request中取出数据，这样就可以在一次请求的不同组件间共享数据。
- 服务端两个组件之间有耦合，要共同完成一个请求，就用转发；没有耦合，就用重定向。
- 重定向会导致浏览器地址栏变化，转发不会。

![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202304180131475.png)

认证成功后，结果会通过SecurityContextHolder存入SecurityContext中
```java
@RequestMapping(path = "/index", method = RequestMethod.GET)
public String getIndexPage() {
	// 认证成功后，结果会通过SecurityContextHolder存入SecurityContext中
	Object obj = SecurityContextHolder.getContext().getAuthentication().getPrincipal();
	if (obj instanceof User) {
		model.addAttribute("loginUser", obj);
	}
	return "/index";
}
```

### 需求
