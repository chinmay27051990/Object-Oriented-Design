using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
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

using System.IO;
using System.Windows.Forms;
using KeyEventArgs = System.Windows.Input.KeyEventArgs;
using Path = System.IO.Path;

namespace WPF
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        private string working_directory { get; set; }
        private string Patterns { get; set; }
        private string Regex_ { get; set; }

        public MainWindow()
        {
            InitializeComponent();
        }

        private void MainWindow_OnLoaded(object sender, RoutedEventArgs e)
        {
            working_directory = "../../../test";
            Patterns = "*.h *.cpp";
            Regex_ = "[A-C](.*)";

            working_directory = Path.GetFullPath(working_directory);
            txtPath.Text = working_directory;
            txtPatterns.Text = Patterns;
            txtRegex.Text = Regex_;
        }

        private void BtnBrowse_Click(object sender, RoutedEventArgs e)
        {
            System.Windows.Forms.FolderBrowserDialog DialogResult = new System.Windows.Forms.FolderBrowserDialog();
            DialogResult = new System.Windows.Forms.FolderBrowserDialog();

            if (DialogResult.ShowDialog() == System.Windows.Forms.DialogResult.OK)
            {
                working_directory = DialogResult.SelectedPath;
                txtPath.Text = working_directory;
            }
        }

        private async void BtnPublish_OnClick(object sender, RoutedEventArgs e)
        {
            string html = "Converter.cpp.html";
            var p = System.Diagnostics.Process.Start(html);
            txtProgress.Text = "Waiting to close";
            while (!p.HasExited)
                await Task.Delay(500);
            txtProgress.Text = "Closed";
        }

        private void TxtPath_OnTextChanged(object sender, TextChangedEventArgs e)
        {
            lstFiles.Items.Clear();
            lstFiles.Items.Add("[..]");

            List<string> cmdargs = new List<string>();

            cmdargs.Add("");
            cmdargs.Add(working_directory);

            var dirs = Directory.GetDirectories(working_directory);

            foreach (var dir in dirs)
            {
                string dirName = "[" + Path.GetFileName(dir) + "]";
                lstFiles.Items.Add(dirName);
            }
            List<string> filesMatchingPatterns = new List<string>();

            if (Patterns != null)
            {
                var patterns = Patterns.Split(' ');

                foreach (var patt in patterns)
                {
                    filesMatchingPatterns.AddRange(Directory.GetFiles(working_directory, patt));

                }

            }
            else
            {
                filesMatchingPatterns = Directory.GetFiles(working_directory).ToList();
            }
            foreach (var file in filesMatchingPatterns) lstFiles.Items.Add(Path.GetFileName(file));

        }

        private void LstFiles_OnMouseDoubleClick(object sender, MouseButtonEventArgs e)
        {
            if (lstFiles.SelectedIndex == -1) return;

            string SelectedItem = lstFiles.SelectedItem.ToString();
            SelectedItem = SelectedItem.Substring(1, SelectedItem.Length - 2);
            SelectedItem = Path.Combine(working_directory, SelectedItem);
            SelectedItem = Path.GetFullPath(SelectedItem);

            if (!Directory.Exists(SelectedItem)) return;

            working_directory = SelectedItem;
            txtPath.Text = SelectedItem;
        }

        void usefulFunction()
        {
            List<string> args = new List<string>();
            args.Add("WPF.exe");
            args.Add(working_directory);
            if (cbRecursive.IsChecked.HasValue) args.Add("/s");
            args.AddRange(Patterns.Split(' '));
            args.AddRange(Regex_.Split(' '));
        }

        private void TxtPatterns_OnKeyUp(object sender, KeyEventArgs e)
        {
            if (e.Key == Key.Enter)
            {
                Patterns = txtPatterns.Text;
                TxtPath_OnTextChanged(this, null);
            }
        }

    }
}

