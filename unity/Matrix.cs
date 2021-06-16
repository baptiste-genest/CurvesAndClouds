public class Matrix
{
	int w;
	int h;
	double[,] a;
 
	public Matrix(int _w, int _h)
	{
		this.w = _w;
		this.h = _w;
		a = new double[_w, _h];
	}
 
	//indices start from one
	public double this[int i, int j]
	{
		get { return a[i - 1, j - 1]; }
		set { a[i - 1, j - 1] = value; }
	}
 
	public int W { get { return w; } }
	public int H { get { return h; } }
 
	public static Matrix operator*(Matrix _a, Matrix b)
	{
		int n = _a.W;
		int m = b.H;
		int l = _a.H;
		Matrix result = new Matrix(_a.W, b.H);
		for(int i = 0; i < n; i++)
			for (int j = 0; j < m; j++)
			{
				double sum = 0.0;
				for (int k = 0; k < l; k++)
					sum += _a.a[i, k]*b.a[k, j];
				result.a[i, j] = sum;
			}
		return result;
	}
}