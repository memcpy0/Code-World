package chap3;
import stone.*;

public class LexerRunner {
	public static void main(String[] args) throws ParseException {
		Lexer l = new Lexer(new CodeDialog()); // Lexer(Reader)
		for (Token t; (t = l.read()) != Token.EOF; ) 
			System.out.println("=> " + t.getText()); 
	}
}
