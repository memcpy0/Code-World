package stone;

import java.io.IOException;
// �����쳣
public class ParseException extends Exception { // �ܲ��쳣
	public ParseException(String msg) { // �쳣��Ϣ
		super(msg);
	}
	public ParseException(IOException e) { // Exception(Throwable cause) ���쳣������ParseException
		super(e);
	}
	
	public ParseException(Token t) { // ��ʹ��Token������ParseExceptionʱ��û���쳣��Ϣ
		this("", t);
	}
	public ParseException(String msg, Token t) { // �쳣��Ϣ��Token������ParseException
		super("syntax error around " + location(t) + ". " + msg); 
	} 
	private static String location(Token t) { // �����ַ������͵�Tokenλ��
		if (t == Token.EOF) // Token��end of file
			return "the last line";
		else // �������ʣ�������ʽ: (���������ݡ� at line x)
			return "\"" + t.getText() + "\" at line " + t.getLineNumber();
	}
	

}
