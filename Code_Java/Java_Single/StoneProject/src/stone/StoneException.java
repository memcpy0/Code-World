package stone;

public class StoneException extends RuntimeException { // 继承自RuntimeException， 为非受查异常
	public StoneException(String message) {
		super(message);
	}
}
