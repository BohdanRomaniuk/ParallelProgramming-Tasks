using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Shapes;
using System.Threading;
using System.Threading.Tasks;


namespace _5__WPF_Threads
{
	/// <summary>
	/// Interaction logic for ThreadWindow.xaml
	/// </summary>
	public partial class ThreadWindow : Window
	{
		private double[,] FirstMatrix { get; set; }
		private double[,] SecondMatrix { get; set; }
		private uint Size { get; set; }
		private uint FromRow { get; set; }
		private uint ToRow { get; set; }
		public ThreadWindow()
		{
			InitializeComponent();
		}
		public ThreadWindow(double[,] firstMatrix, double[,] secondMatrix, uint size, uint fromRow, uint toRow)
		{
			InitializeComponent();
			singleThreadProgres.Maximum = (toRow-fromRow)+2*size;
			FirstMatrix = firstMatrix;
			SecondMatrix = secondMatrix;
			Size = size;
			FromRow = fromRow;
			ToRow = toRow;
		}
		public void StartCalculation()
		{
			for (uint i = FromRow; i < ToRow; ++i)
			{
				for (uint j = 0; j < Size; ++j)
				{
					for (uint k = 0; k < Size; ++k)
					{
						((MainWindow)Application.Current.Windows[0]).Matrix[i, j] += FirstMatrix[i, k] * SecondMatrix[k, j];
						++singleThreadProgres.Value;
					}
				}
			}
		}
	}
}
