
检查Java的包名、类名、变量名不仅要符合硬性规范，也要符合习惯写法和约束。下面的我写的一个检查器，用的是正则表达式。
```java
import java.util.Scanner;
import java.util.regex.*;

abstract class Identifier { // 标识符
	/*   
            * 所有的标识符都应该以字母(A-Z或者a-z)，美元符($)，或者下划线(_)开始 
	* 首字符之后可以是字母(A-Z或者a-z)，美元符($)，或者下划线(_)或数字的任何字符组合
	* 关键字不能用作标识符 
    */
	private static String regexPat = "^([A-Za-z_$][A-Za-z0-9_$]*)$";
	private static Pattern superPattern = Pattern.compile(regexPat);  
	private static String[] keywords = {
		"private", "protected", "public",  // 访问控制
		"abstract", "class", "extends", "final", "implements", "interface", "native", 
		"new", "static", "strictfp", "synchronized", "transient", "volatile", // 类、方法、变量修饰符
		"break", "case", "continue", "default", "do", "else", "for", // 程序控制语句
		"if", "instanceof", "return", "switch", "while", 
		"assert", "catch", "finally", "throw", "throws", "try", // 错误处理
		"import", "package",  // 包相关
		"boolean", "byte", "char", "double", "float", "int", "long", "short", // 基本类型
		"super", "this", "void",  // 变量引用
		"goto", "const", "null", // 保留关键字
		"true", "false"
	};
	public static boolean isIdentifier(String name) {
		for (String s : keywords) { // 检查是否与关键字和保留字冲突
			if (s.equals(name)) return false;
		}
		Matcher superMatcher = superPattern.matcher(name);
		return (superMatcher.find() && superMatcher.group(1).equals(name));
	}
	public static boolean isIdentifier(String name, Pattern subPattern) {  
		Matcher subMatcher = subPattern.matcher(name);
		return (isIdentifier(name) && (subMatcher.find() && subMatcher.group(1).equals(name))); 
	}
	public static boolean isPackageName(String name) { return false;}
	public static boolean isClassName(String name) { return false;}
	public static boolean isVariableName(String name) { return false;}
	public static boolean isConstantName(String name) { return false;}
	public static boolean isFunctionName(String name) { return false;}
}

class PackageName extends Identifier { // 包名一般是小写英文字母
	private static String packageRegexPat = "^([a-z]+)$";
	private static Pattern packagePattern = Pattern.compile(packageRegexPat);  
	public static boolean isPackageName(String name) {
		return isIdentifier(name, packagePattern);
	}
}

class ClassName extends Identifier { // 类名 : 符合驼式命名法，首字母大写，后面单词的首字母也应该大写 
	private static String classRegexPat = "^(([A-Z][a-z]*)+)$";
	private static Pattern classPattern = Pattern.compile(classRegexPat);  
	public static boolean isClassName(String name) { 
		return isIdentifier(name, classPattern);
	} 
}

class VariableName extends Identifier { // 变量名 : 符合驼式命名法，首字母小写，后面单词首字母大写
	private static String variableRegexPat = "^(([a-z][a-z0-9_$]*)([A-Z][a-z0-9_$]*)*)$";
	private static Pattern variablePattern = Pattern.compile(variableRegexPat);  
	public static boolean isVariableName(String name) { 
		return isIdentifier(name, variablePattern);
	} 
} 

class ConstantName extends Identifier { // 常量名 : 要求全部大写，_分割单词，结尾没有_
	private static String constantRegexPat = "^(([A-Z]+[_]?)+[^_])$";
	private static Pattern constantPattern = Pattern.compile(constantRegexPat); 
	public static boolean isConstantName(String name) {
		return isIdentifier(name, constantPattern);
	} 
}

class FunctionName extends Identifier { // 方法名 : 符合驼式命名法，首字母小写，后面单词首字母大写 
	private static VariableName tempJudger = new VariableName();
	public static boolean isFunctionName(String name) { 
		return tempJudger.isVariableName(name); // 和变量名要求一样
	} 
}

public class JavaNameIdentifier {
	public static void show(boolean tf) {
		System.out.println(tf ? "True" : "False");
	}
	public static void main(String[] args) {
		System.out.println("--------Java命名规则检查器--------------------------");
		System.out.println("0. 基础标识符检查\n1. 包名\n2. 类名\n3. 变量名\n4. 常量名\n5. 方法名\n6. 退出");
		Scanner input = new Scanner(System.in); 
		
		while (true) {
			System.out.println("请输入操作号:");
			int id = input.nextInt();
		    if (id == 6) {
		    	input.close(); return;
		    }
			System.out.println("请输入名称:");
			String name = input.next();
			switch (id) {
			case 0: show(Identifier.isIdentifier(name));     break;
			case 1: show(PackageName.isPackageName(name));   break;
			case 2: show(ClassName.isClassName(name));       break;
			case 3: show(VariableName.isVariableName(name)); break;
			case 4: show(ConstantName.isConstantName(name)); break;
			case 5: show(FunctionName.isFunctionName(name)); break;
			}
		} 
	}
}
```
