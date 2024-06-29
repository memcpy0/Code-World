package stone; // ���ʵĳ�����

public abstract class Token {
	public static final Token EOF = new Token(-1) {}; // end of file����ʾ��������ĵ���
	public static final String EOL = "\n"; // end of line ���з�����
	
	private int lineNumber; // ��������λ�õ��к�
	
	protected Token(int line) { // �ܱ����Ĺ��췽����Ŀ����������������ʣ�����ͬһ�����ڵ��������������
		lineNumber = line;
	}
	
	public int getLineNumber() { // �����к�
		return lineNumber;
	}
	
	// ���ֵ�������
	public boolean isIdentifier() { return false; }
	public boolean isNumber()     { return false; }
	public boolean isString()     { return false; }
	
	public int getNumber() { 
		throw new StoneException("not number token");
	}
	
	public String getText() { // �����ַ������͵ĵ�������
		return "";
	}
}
