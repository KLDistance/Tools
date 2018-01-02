package review1;

public class WAlgorithm {
	
	public static void generate(double[][] mat, double initValue)
	{
		for(int i = 0; i < mat.length; i++)
		{
			for(int j = 0; j < mat[0].length; j++)
			{
				mat[i][j] = initValue;
			}
		}
	}
	
	public static void copy(double[][] desMatrix, double[][] srcMatrix)
	{
		for(int i = 0; i < srcMatrix.length; i++)
		{
			for(int j = 0; j < srcMatrix[0].length; j++)
			{
				desMatrix[i][j] = srcMatrix[i][j];
			}
		}
	}
	
	public static void add(double[][] result, double[][] input1, double[][] input2)
	{
		for(int i = 0; i < input1.length; i++)
		{
			for(int j = 0; j < input1[0].length; j++)
			{
				result[i][j] = input1[i][j] + input2[i][j];
			}
		}
	}
	
	public static void partialCopy(double[][] result, double[][] input, int startRow, int startCol)
	{
		for(int i = 0; i < result.length; i++)
		{
			for(int j = 0; j < result.length; j++)
			{
				result[i][j] = input[i + startRow][j + startCol];
			}
		}
	}
	
	public static void partialAdd(double[][] result, double[][] input, int startRow, int startCol)
	{
		for(int i = 0; i < result.length; i++)
		{
			for(int j = 0; j < result.length; j++)
			{
				result[i][j] += input[i + startRow][j + startCol];
			}
		}
	}
	
	public static void sub(double[][] result, double[][] input1, double[][] input2)
	{
		for(int i = 0; i < input1.length; i++)
		{
			for(int j = 0; j < input1[0].length; j++)
			{
				result[i][j] = input1[i][j] - input2[i][j];
			}
		}
	}
	
	public static void partialSub(double[][] result, double[][] input, int startRow, int startCol)
	{
		for(int i = 0; i < result.length; i++)
		{
			for(int j = 0; j < result.length; j++)
			{
				result[i][j] -= input[i + startRow][j + startCol];
			}
		}
	}
	
	public static void merge(double[][] C, double[][] C11, double[][] C12, double[][] C21, double[][] C22)
	{
		if(C.length == 1)
		{
			C[0][0] = C11[0][0];
		}
		else
		{
			for(int i = 0; i < C.length; i++)
			{
				for(int j = 0; j < C[0].length; j++)
				{
					if(i < C.length/2 && j < C.length/2)
					{
						C[i][j] = C11[i][j];
					}
					else if(i >= C.length/2 && j < C.length / 2)
					{
						C[i][j] = C21[i - C.length/2][j];
					}
					else if(i < C.length/2 && j >= C.length/2)
					{
						C[i][j] = C12[i][j - C.length/2];
					}
					else
					{
						C[i][j] = C22[i - C.length/2][j - C.length/2];
					}
				}
			}
		}
	}
	
	public static void multiply(double[][] A, double[][] B, double[][] C)
	{
		if(A.length == 1 || B.length == 1)
		{
			C[0][0] = A[0][0] * B[0][0];
		}
		else
		{
			int Ahalf = A.length / 2;
			int Bhalf = B.length / 2;
			//S1 high row 
			double[][] S1 = new double[Ahalf][Ahalf];
			double[][] S2 = new double[Ahalf][Ahalf];
			double[][] S3 = new double[Ahalf][Ahalf];
			double[][] S4 = new double[Ahalf][Ahalf];
			
			double[][] T1 = new double[Bhalf][Bhalf];
			double[][] T2 = new double[Bhalf][Bhalf];
			double[][] T3 = new double[Bhalf][Bhalf];
			double[][] T4 = new double[Bhalf][Bhalf];
			
			double[][] M1 = new double[Ahalf][Ahalf];
			double[][] M2 = new double[Ahalf][Ahalf];
			double[][] M3 = new double[Ahalf][Ahalf];
			double[][] M4 = new double[Ahalf][Ahalf];
			double[][] M5 = new double[Ahalf][Ahalf];
			double[][] M6 = new double[Ahalf][Ahalf];
			double[][] M7 = new double[Ahalf][Ahalf];
			
			double[][] U1 = new double[Ahalf][Ahalf];
			double[][] U2 = new double[Ahalf][Ahalf];
			double[][] U3 = new double[Ahalf][Ahalf];
			double[][] U4 = new double[Ahalf][Ahalf];
			double[][] U5 = new double[Ahalf][Ahalf];
			double[][] U6 = new double[Ahalf][Ahalf];
			double[][] U7 = new double[Ahalf][Ahalf];
			
			double[][] A11 = new double[Ahalf][Ahalf];
			double[][] A12 = new double[Ahalf][Ahalf];
			double[][] A21 = new double[Ahalf][Ahalf];
			double[][] A22 = new double[Ahalf][Ahalf];
			
			double[][] B11 = new double[Bhalf][Bhalf];
			double[][] B12 = new double[Bhalf][Bhalf];
			double[][] B21 = new double[Bhalf][Bhalf];
			double[][] B22 = new double[Bhalf][Bhalf];
			
			partialCopy(A11, A, 0, 0);
			partialCopy(A12, A, 0, Ahalf - 1);
			partialCopy(A21, A, Ahalf - 1, 0);
			partialCopy(A22, A, Ahalf - 1, Ahalf - 1);
			
			partialCopy(B11, B, 0, 0);
			partialCopy(B12, B, 0, Bhalf - 1);
			partialCopy(B21, B, Bhalf - 1, 0);
			partialCopy(B22, B, Bhalf - 1, Bhalf - 1);
			
			add(S1, A21, A22);
			sub(S2, S1, A11);
			sub(S3, A11, A21);
			sub(S4, A12, S2);
			
			sub(T1, B21, B11);
			sub(T2, B22, T1);
			sub(T3, B22, B12);
			sub(T4, T2, B21);
			
			multiply(A11, B11, M1);
			multiply(A12, B21, M2);
			multiply(S4, B22, M3);
			multiply(A22, T4, M4);
			multiply(S1, T1, M5);
			multiply(S2, T2, M6);
			multiply(S3, T3, M7);
			
			add(U1, M1, M2);
			add(U2, M1, M6);
			add(U3, U2, M7);
			add(U4, U2, M5);
			add(U5, U4, M3);
			sub(U6, U3, M4);
			add(U7, U3, M5);
			
			merge(C, U1, U5, U6, U7);
		}
	}
	
	public static void indexPartialAdd(double[][] result, double[][] input1, int rowStart1, int colStart1, double[][] input2, int rowStart2, int colStart2)
	{
		for(int i = 0; i < result.length; i++)
		{
			for(int j = 0; j < result[0].length; j++)
			{
				result[i][j] = input1[rowStart1 + i][colStart1 + j] + input2[rowStart2 + i][colStart2 + j];
			}
		}
	}
	
	public static void indexPartialAddOnce(double[][] result, double[][] input1, int rowStart1, int colStart1, int rowStart2, int colStart2)
	{
		for(int i = 0; i < result.length; i++)
		{
			for(int j = 0; j < result[0].length; j++)
			{
				result[i][j] = input1[rowStart1 + i][colStart1 + j] + input1[rowStart2 + i][colStart2 + j];
			}
		}
	}
	
	public static void indexPartialSub(double[][] result, double[][] input1, int rowStart1, int colStart1, double[][] input2, int rowStart2, int colStart2)
	{
		for(int i = 0; i < result.length; i++)
		{
			for(int j = 0; j < result[0].length; j++)
			{
				result[i][j] = input1[rowStart1 + i][colStart1 + j] - input2[rowStart2 + i][colStart2 + j];
			}
		}
	}
	
	public static void indexPartialSubOnce(double[][] result, double[][] input1, int rowStart1, int colStart1, int rowStart2, int colStart2)
	{
		for(int i = 0; i < result.length; i++)
		{
			for(int j = 0; j < result[0].length; j++)
			{
				result[i][j] = input1[rowStart1 + i][colStart1 + j] - input1[rowStart2 + i][colStart2 + j];
			}
		}
	}
	
	public static void multiply2(double[][] A, int ARowStart, int AColStart, int Alength,  
			double[][] B, int BRowStart, int BColStart, int Blength, double[][] C)
	{
		if(Alength == 1 || Blength == 1)
		{
			C[0][0] = A[0][0] * B[0][0];
		}
		else
		{
			int Ahalf = Alength / 2;
			int Bhalf = Blength / 2;
			//S1 high row 
			double[][] S1 = new double[Ahalf][Ahalf];
			double[][] S2 = new double[Ahalf][Ahalf];
			double[][] S3 = new double[Ahalf][Ahalf];
			double[][] S4 = new double[Ahalf][Ahalf];
			
			double[][] T1 = new double[Bhalf][Bhalf];
			double[][] T2 = new double[Bhalf][Bhalf];
			double[][] T3 = new double[Bhalf][Bhalf];
			double[][] T4 = new double[Bhalf][Bhalf];
			
			double[][] M1 = new double[Ahalf][Ahalf];
			double[][] M2 = new double[Ahalf][Ahalf];
			double[][] M3 = new double[Ahalf][Ahalf];
			double[][] M4 = new double[Ahalf][Ahalf];
			double[][] M5 = new double[Ahalf][Ahalf];
			double[][] M6 = new double[Ahalf][Ahalf];
			double[][] M7 = new double[Ahalf][Ahalf];
			
			double[][] U1 = new double[Ahalf][Ahalf];
			double[][] U2 = new double[Ahalf][Ahalf];
			double[][] U3 = new double[Ahalf][Ahalf];
			double[][] U4 = new double[Ahalf][Ahalf];
			double[][] U5 = new double[Ahalf][Ahalf];
			double[][] U6 = new double[Ahalf][Ahalf];
			double[][] U7 = new double[Ahalf][Ahalf];
			
			indexPartialAddOnce(S1, A, ARowStart + Ahalf, AColStart, ARowStart + Ahalf, AColStart + Ahalf);
			indexPartialAdd(S2, S1, 0, 0, A, ARowStart, AColStart);
			indexPartialSubOnce(S3, A, ARowStart, AColStart, ARowStart + Ahalf, AColStart);
			indexPartialSub(S4, A, ARowStart, AColStart + Ahalf, S2, 0, 0);
			
			indexPartialSubOnce(T1, B, BRowStart + Bhalf, BColStart, BRowStart, BColStart);
			indexPartialSub(T2, B, BRowStart + Bhalf, BColStart + Bhalf, T1, 0, 0);
			indexPartialSubOnce(T3, B, BRowStart + Bhalf, BColStart + Bhalf, BRowStart, BColStart + Bhalf);
			indexPartialSub(T4, T2, 0, 0, B, BRowStart + Bhalf, BColStart);
			
			multiply2(A, ARowStart, AColStart, Ahalf, B, BRowStart, BColStart, Bhalf, M1);
			multiply2(A, ARowStart, AColStart + Ahalf, Ahalf, B, BRowStart + Bhalf, BColStart, Bhalf, M2);
			multiply2(S4, 0, 0, S4.length, B, BRowStart + Bhalf, BColStart + Bhalf, Bhalf, M3);
			multiply2(A, ARowStart + Ahalf, AColStart + Ahalf, Ahalf, T4, 0, 0, T4.length, M4);
			multiply2(S1, 0, 0, S1.length, T1, 0, 0, T1.length, M5);
			multiply2(S2, 0, 0, S2.length, T2, 0, 0, T2.length, M6);
			multiply2(S3, 0, 0, S3.length, T3, 0, 0, T3.length, M7);
			
			add(U1, M1, M2);
			add(U2, M1, M6);
			add(U3, U2, M7);
			add(U4, U2, M5);
			add(U5, U4, M3);
			sub(U6, U3, M4);
			add(U7, U3, M5);
			
			merge(C, U1, U5, U6, U7);
		}
	}
	
	public static void multiply_naive(double[][] input1, double[][] input2, double[][] result)
	{
		for(int i = 0; i < input1.length; i++)
		{
			for(int j = 0; j < input2[0].length; j++)
			{
				for(int k = 0; k < input1[0].length; k++)
				{
					result[i][j] = input1[i][k] * input2[k][j];
				}
			}
		}
	}
	
	public static void main(String[]args)
	{
		int mat1_rowNum = 4096;
		int mat2_rowNum = 4096;
		
		double [][] mat1 = new double[mat1_rowNum][mat1_rowNum];
		double [][] mat2 = new double[mat2_rowNum][mat2_rowNum];
		double [][] result = new double[mat1_rowNum][mat1_rowNum];
		
		generate(mat1, 10.3);
		generate(mat2, 423.42);
		
		long time = System.currentTimeMillis();
		multiply2(mat1, 0, 0, mat1.length, mat2, 0, 0, mat2.length, result);
//		multiply_naive(mat1, mat2, result);
		System.out.printf("Time elapsed: %fs\n", ((double)(System.currentTimeMillis() - time)) / 1000);
//		for(int i = 0; i < result.length; i++)
//		{
//			for(int j = 0; j < result[0].length; j++)
//			{
//				System.out.printf("%f\t", result[i][j]);
//			}
//			System.out.println();
//		}
	}
}







