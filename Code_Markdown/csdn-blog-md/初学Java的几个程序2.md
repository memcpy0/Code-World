冒泡排序：
```java
import java.util.Scanner;

public class BubbleSort {
	public static void SortAscend(int array[]) { // 升序排列
		for (int i = 0; i < array.length - 1; i++) {
			for (int j = array.length - 1; j > 0; j--) {
				if (array[j] < array[j - 1]) {
					int temp = array[j];
					array[j] = array[j - 1];
					array[j - 1] = temp;
				}
			}
		}
	}
	public static void SortDescend(int array[]) { // 降序排列
		SortAscend(array);
		for (int i = 0, j = array.length - 1; i < j; i++, j--) {
			int temp = array[i];
			array[i] = array[j];
			array[j] = temp;
		}
	}
	public static void main(String[] args) {
		System.out.println("输入10个数进行排序：");
		Scanner input = new Scanner(System.in);
		int size = 10;
		int[] array = new int[size];
		for (int i = 0; i <= 9; i++) {
			array[i] = input.nextInt();
		}
		SortAscend(array);
		for (int i = 0; i < array.length; i++) 
			System.out.print(array[i] + " ");
		SortDescend(array);
		System.out.println();
		for (int i = 0; i < array.length; i++) 
			System.out.print(array[i] + " ");
	}
}
```

二维矩阵加法和乘法的实现：
```java
import java.util.Scanner;

public class Matrix {
	private int[][] matrix; // 二维数组
	private int row; // 行
	private int col; // 列
	public Matrix(int n, int m) {
		row = n;
		col = m;
		matrix = new int[n][m];
	}
	public int RowNum() {
		return row;
	}
	public int ColNum() {
		return col;
	}
	public int GetXY(int x, int y) throws IndexOutOfBoundsException { // 取值 
		if (x < 0 || x >= row || y < 0 || y >= col) { // 范围检查
			System.out.println("ERROR!");
			throw new IndexOutOfBoundsException();
		} else return matrix[x][y];
	}
	public void SetXY(int x, int y, int data) throws IndexOutOfBoundsException { // 置值
		if (x < 0 || x >= row || y < 0 || y >= col) { // 范围检查
			System.out.println("ERROR!");
			throw new IndexOutOfBoundsException();
		} else matrix[x][y] = data;
	}
	public void AddXY(int x, int y, int data) throws IndexOutOfBoundsException { 
		if (x < 0 || x >= row || y < 0 || y >= col) { // 范围检查
			System.out.println("ERROR!");
			throw new IndexOutOfBoundsException();
		} else matrix[x][y] += data;
	}
	public static Matrix Add(Matrix m1, Matrix m2) { // 矩阵加法
		int c1 = m1.ColNum(), r1 = m1.RowNum();
		int c2 = m2.ColNum(), r2 = m2.RowNum();
		Matrix result = new Matrix(r1, c1);
		if (c1 != c2 || r1 != r2) {
			System.out.println("ERROR!");
		} else {
			for (int i = 0; i < r1; i++) {
				for (int j = 0; j < c1; j++) {
					result.SetXY(i, j, m1.GetXY(i, j) + m2.GetXY(i, j));
				}
			}
		}
		return result;
	} 
	public static Matrix Multi(Matrix m1, Matrix m2) { // 矩阵乘法
		int r = m1.RowNum();
		int c1 = m1.ColNum(), r1 = m1.RowNum();
		int c2 = m2.ColNum(), r2 = m2.RowNum();
		Matrix result = new Matrix(r1, c2);
		if (c1 != r2) {
			System.out.println("ERROR!");
		} else {
			for (int i = 0; i < r1; i++) {
				for (int j = 0; j < c2; j++) {
					for (int k = 0; k < c1; k++) {
						int t = m1.GetXY(i, k), t2 = m2.GetXY(k, j);
						result.AddXY(i, j, m1.GetXY(i, k) * m2.GetXY(k, j));
					} 
				}
			}
		}
		return result;
	}
	public void Input() { // 输入矩阵
		Scanner input = new Scanner(System.in);
		for (int i = 0; i < row; i++) {
			for (int j = 0; j < col; j++) {
				matrix[i][j] = input.nextInt(); 
			}
		}  
	}
	public void Show() { // 打印矩阵
		for (int i = 0; i < row; i++) {
			for (int j = 0; j < col; j++) {
				System.out.print(matrix[i][j] + " ");
			}
			System.out.println();
		}
	}
	public static void main(String[] args) {
		Matrix m1 = new Matrix(3, 4), m2 = new Matrix(3, 4), m3 = new Matrix(4, 2);
		System.out.println("请输入矩阵1(3行4列)的数据：");
		m1.Input();
		System.out.println("请输入矩阵2(3行4列)的数据：");
		m2.Input();
		Matrix result = Matrix.Add(m1, m2);
		System.out.println("矩阵1+矩阵2为 ：");
		result.Show();
		
		System.out.println("请输入矩阵3(4行2列)的数据：");
		m3.Input();
		Matrix result2 = Matrix.Multi(m2, m3);
		System.out.println("矩阵2*矩阵3为 ：");
		result2.Show();
	}
}
```
正则表达式的使用：
```java
import java.util.regex.*;

public class UniversityNameGetter {
	public static void main(String[] args) {
		String[] content = new String[4];
		content[0] = "我去北京大学上学!";
		content[1] = "我在武汉大学上课！"; 
		content[2] = "我在中山大学上课！"; 
		content[3] = "我去武汉学院游玩！";
		String reg = ".*(.{2}(大学|学院)).*";
		Pattern p = Pattern.compile(reg);
		for (int i = 0; i < content.length; i++) {
			Matcher m = p.matcher(content[i]);
			if (m.find()) {
				System.out.println("Found : " + m.group(1));
			}
		}
	}
}

```
 
