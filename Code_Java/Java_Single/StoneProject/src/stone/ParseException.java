package stone;

import java.io.IOException;
// 解析异常
public class ParseException extends Exception { // 受查异常
	public ParseException(String msg) { // 异常信息
		super(msg);
	}
	public ParseException(IOException e) { // Exception(Throwable cause) 用异常对象构造ParseException
		super(e);
	}
	
	public ParseException(Token t) { // 仅使用Token对象构造ParseException时，没有异常信息
		this("", t);
	}
	public ParseException(String msg, Token t) { // 异常信息和Token对象构造ParseException
		super("syntax error around " + location(t) + ". " + msg); 
	} 
	private static String location(Token t) { // 给出字符串类型的Token位置
		if (t == Token.EOF) // Token是end of file
			return "the last line";
		else // 其他单词，返回形式: (”单词内容“ at line x)
			return "\"" + t.getText() + "\" at line " + t.getLineNumber();
	}
	

}
