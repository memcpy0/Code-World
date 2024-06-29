package stone; // 单词的抽象类

public abstract class Token {
	public static final Token EOF = new Token(-1) {}; // end of file，表示程序结束的单词
	public static final String EOL = "\n"; // end of line 换行符单词
	
	private int lineNumber; // 单词所在位置的行号
	
	protected Token(int line) { // 受保护的构造方法，目的在于允许子类访问，或者同一个包内的所有其他类访问
		lineNumber = line;
	}
	
	public int getLineNumber() { // 返回行号
		return lineNumber;
	}
	
	// 区分单词类型
	public boolean isIdentifier() { return false; }
	public boolean isNumber()     { return false; }
	public boolean isString()     { return false; }
	
	public int getNumber() { 
		throw new StoneException("not number token");
	}
	
	public String getText() { // 返回字符串类型的单词内容
		return "";
	}
}
