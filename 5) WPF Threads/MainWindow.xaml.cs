using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using System.ComponentModel;

namespace _5__WPF_Threads
{
	/// <summary>
	/// Interaction logic for MainWindow.xaml
	/// </summary>
	public partial class MainWindow : Window, INotifyPropertyChanged
	{
		private uint currentProgres=0;
		public uint CurrentProgress
		{
			get
			{
				return currentProgres;
			}
			set
			{
				currentProgres = value;
				OnPropertyChanged("CurrentProgress");
			}
		}
		private double[,] FirstMatrix { get; set; }
		private double[,] SecondMatrix { get; set; }
		private uint Size { get; set; }
		public double[,] Matrix { get; set; }
		public MainWindow()
		{
			InitializeComponent();
			DataContext = this;
			Random randomizer = new Random();
			Size = 100;
			FirstMatrix = new double[Size, Size];
			SecondMatrix = new double[Size, Size];
			Matrix = new double[Size, Size];
			for (uint i = 0; i < Size; ++i)
			{
				for (uint j = 0; j < Size; ++j)
				{
					FirstMatrix[i, j] = randomizer.Next(0, 1000);
					SecondMatrix[i, j] = randomizer.Next(0, 2000);
				}
			}
		}

		private void createThreads_Click(object sender, RoutedEventArgs e)
		{
			uint threadsQuantity = Convert.ToUInt32(threadsCount.Text);
			ThreadWindow[] threadsWindows = new ThreadWindow[threadsQuantity];
			uint fromRow = 0;
			uint threadStep = Size / threadsQuantity;
			uint toRow = threadStep;
			threadsProgress.Maximum = Size;
			for(uint i=0; i<threadsQuantity; ++i)
			{
				threadsWindows[i] = new ThreadWindow(FirstMatrix, SecondMatrix, Size, fromRow, toRow);
				threadsWindows[i].Show();
				fromRow += threadStep;
				toRow += threadStep;
			}
			for(uint i=0; i<threadsQuantity; ++i)
			{
				threadsWindows[i].StartCalculation();
				CurrentProgress += threadStep;
			}
		}

		private void dropThreads_Click(object sender, RoutedEventArgs e)
		{
			foreach (Window item in Application.Current.Windows)
			{
				if (!(item is MainWindow))
				{
					item.Close();
				}
			}
		}

		public event PropertyChangedEventHandler PropertyChanged;
		protected virtual void OnPropertyChanged(string propertyName = null)
		{
			PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(propertyName));
		}
	}
}
