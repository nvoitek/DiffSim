using System;
using System.ComponentModel;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;
using System.Windows;
using System.Windows.Forms;

namespace DiffSimApp
{
    /// <summary>
    /// Logika interakcji dla klasy MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window, INotifyPropertyChanged
    {
        [DllImport("DiffSimDll.dll", CallingConvention = CallingConvention.StdCall)]
        public static extern int StartDiffractionSimulation([In, Out] WorkProgressInteropNegotiator negotiator, string input, string output);

        private BackgroundWorker bgWorker;
        private string input;
        private string output;
        private int progress;

        #region Flags

        private bool isInputLoaded;

        public bool IsInputLoaded
        {
            get
            {
                return isInputLoaded;
            }
            set
            {
                if (value != isInputLoaded)
                {
                    isInputLoaded = value;
                    NotifyPropertyChanged();
                    NotifyPropertyChanged("CanStart");
                }
            }
        }

        public bool CanStart
        {
            get
            {
                return IsInputLoaded && !InProgress;
            }
        }

        private bool inProgress;

        public bool InProgress
        {
            get
            {
                return inProgress;
            }
            set
            {
                if (value != inProgress)
                {
                    inProgress = value;
                    NotifyPropertyChanged();
                    NotifyPropertyChanged("CanStart");
                }
            }
        }

        #endregion

        #region BindableProperties

        public string Input
        {
            get { return input; }
            set
            {
                if (value != this.input)
                {
                    this.input = value;
                    NotifyPropertyChanged();
                    if (string.IsNullOrEmpty(input))
                        IsInputLoaded = false;
                    else
                        IsInputLoaded = true;
                }
            }
        }

        public string Output
        {
            get { return output; }
            set
            {
                if (value != this.output)
                {
                    this.output = value;
                    NotifyPropertyChanged();
                }
            }
        }

        public int Progress
        {
            get { return progress; }
            set
            {
                if (value != this.progress)
                {
                    this.progress = value;
                    NotifyPropertyChanged();
                }
            }
        }

        #endregion

        public MainWindow()
        {
            bgWorker = new BackgroundWorker();
            bgWorker.DoWork += bgWorker_DoWork;
            bgWorker.ProgressChanged += bgWorker_ProgressChanged;
            bgWorker.RunWorkerCompleted += bgWorker_RunWorkerCompleted;
            bgWorker.WorkerReportsProgress = true;
            bgWorker.WorkerSupportsCancellation = true;

            input = string.Empty;
            output = string.Empty;

            InProgress = false;
            IsInputLoaded = false;

            this.DataContext = this;

            InitializeComponent();
        }

        private void Quit_Click(object sender, RoutedEventArgs e)
        {
            App.Current.Shutdown();
        }

        private void StartSimulation_Click(object sender, RoutedEventArgs e)
        {
            SaveFileDialog sfDialog = new SaveFileDialog();
            sfDialog.Filter = "Data files (.dat)|*.dat|Text files (.txt)|*.txt|All files (.*)|*.*";
            DialogResult result = sfDialog.ShowDialog();

            if (result == System.Windows.Forms.DialogResult.OK)
            {
                Output = sfDialog.FileName;
                bgWorker.RunWorkerAsync();
                InProgress = true;
            }
        }

        private void StopSimulation_Click(object sender, RoutedEventArgs e)
        {
            Stop = true;
        }

        private void LoadSample_Click(object sender, RoutedEventArgs e)
        {
            OpenFileDialog ofDialog = new OpenFileDialog();
            DialogResult result = ofDialog.ShowDialog();

            if (result == System.Windows.Forms.DialogResult.OK)
            {
                Input = ofDialog.FileName;
            }
        }

        void bgWorker_ProgressChanged(object sender, ProgressChangedEventArgs e)
        {
            Progress = e.ProgressPercentage;
            System.Console.WriteLine(progress.ToString());
        }

        void bgWorker_DoWork(object sender, DoWorkEventArgs e)
        {
            WorkProgressInteropNegotiator negotiator = new WorkProgressInteropNegotiator();
            negotiator.reportProgress = new ReportProgressCallback(ReportProgress);
            negotiator.cancellationPending = new CancellationPendingCallback(() => { return Stop; });

            try
            {
                StartDiffractionSimulation(negotiator, Input, Output);
            }
            catch(Exception ex)
            {
                System.Console.WriteLine("Problem with loading dll");
                System.Console.WriteLine(ex.Message);
            }
        }

        private void ReportProgress(int progressPercentage)
        {
            bgWorker.ReportProgress(progressPercentage);
        }

        void bgWorker_RunWorkerCompleted(object sender, RunWorkerCompletedEventArgs e)
        {
            if (e.Cancelled)
            {
                System.Windows.MessageBox.Show("Error during simulation");
            }
            else if (!Stop)
            {
                System.Windows.MessageBox.Show("Simulation completed. Output File is: " + Output);
            }
            else
            {
                System.Windows.MessageBox.Show("Simulation was Cancelled. Partial Output File is: " + Output);
            }
            Progress = 0;
            InProgress = false;
        }

        volatile bool _stop = false;
        public bool Stop
        {
            set
            {
                _stop = value;
            }
            get
            {
                return _stop;
            }
        }

        public delegate void ReportProgressCallback(int progressPercentage);

        public delegate bool CancellationPendingCallback();

        [StructLayout(LayoutKind.Sequential)]
        public class WorkProgressInteropNegotiator
        {
            public ReportProgressCallback reportProgress;

            public CancellationPendingCallback cancellationPending;

#pragma warning disable 0649
            // C# does not see this member is set up in native code, we disable warning to avoid it.
            public bool cancel;
#pragma warning restore 0649
        }

        public event PropertyChangedEventHandler PropertyChanged;

        private void NotifyPropertyChanged([CallerMemberName] String propertyName = "")
        {
            if (PropertyChanged != null)
                PropertyChanged(this, new PropertyChangedEventArgs(propertyName));
        }

        private void Window_Closing(object sender, CancelEventArgs e)
        {
            App.Current.Shutdown();
        }
    }
}
