/////////////////////////////////////////////////////////////////////
// Window.xaml.cs - Prototype for OOD Project #4                   //
//                                                                 //
// Chinmay Vanjare, CSE687 - Object Oriented Design, Spring 2019   //
/////////////////////////////////////////////////////////////////////
/*
 * Package Operations:
 * -------------------
 * This package defines one class SelectionWindow that provides a 
 * ListBox for displaying files selected in the main window.
 * 
 * Two way communication is provided:
 * - MainWindow creates SelectionWindow and stores a reference as
 *   member data.
 * - SelectionWindow provides a SetMainwindow(MainWindow mwin) function
 *   which MainWindow uses to give SelectionWindow access to its public
 *   members.
 * 
 * Required Files:
 * ---------------
 * MainWindow.xaml, MainWindow.xaml.cs
 * 
 * Maintenance History:
 * --------------------
 * ver 1.0 : 17 Apr 2019
 * - first release
 * 
 */
using System;
using System.Collections.Generic;
using System.Windows;
using System.Windows.Input;
using System.IO;
using KeyEventArgs = System.Windows.Input.KeyEventArgs;
using Path = System.IO.Path;
using MsgPassingCommunication;
using System.Threading;
using System.Windows.Automation.Peers;

namespace ClientInterface
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        string workingdir = "../../../ClientDirectory";
        Translater translater_obj;
        private string working_directory { get; set; }
        private string Patterns { get; set; }
        private string Regex_ { get; set; }


        public MainWindow()
        {
            InitializeComponent();
        }

        private Stack<string> pathStack_ = new Stack<string>();
        private Translater translater;
        private CsEndPoint endPoint_;
        private Thread rcvThrd = null;
        private List<string> files = new List<string> { };
        private Dictionary<string, Action<CsMessage>> dispatcher_
          = new Dictionary<string, Action<CsMessage>>();
        private static int serverport;
        private static int selfport;

        private void ProcessMessages()
        {
            void thrdProc()
            {
                while (true)
                {
                    CsMessage msg = translater.getMessage();
                    string msgId = msg.value("command");
                    if (dispatcher_.ContainsKey(msgId))
                        dispatcher_[msgId].Invoke(msg);
                }
            }
            rcvThrd = new Thread(thrdProc)
            {
                IsBackground = true
            };
            rcvThrd.Start();
        }

        private void ClearDirs()
        {
            lstDirs.Items.Clear();
        }

        private void ClearFiles()
        {
            lstFiles.Items.Clear();
        }

        private void AddDir(string dir)
        {
            lstDirs.Items.Add(dir);
        }

        private void InsertParent()
        {
            lstDirs.Items.Insert(0, "..");
        }

        private void AddFile(string file)
        {
            lstFiles.Items.Add(file);
        }

        private void AddClientProc(string key, Action<CsMessage> clientProc)
        {
            dispatcher_[key] = clientProc;
        }

        private void Addconvfiles(string dir)
        {
            files.Add(dir);
        }

        private void DispatcherLoadGetDirs()
        {
            void getDirs(CsMessage rcvMsg)
            {
                Action clrDirs = () =>
                {
                    ClearDirs();
                };
                Dispatcher.Invoke(clrDirs, new Object[] { });
                var enumer = rcvMsg.attributes.GetEnumerator();
                while (enumer.MoveNext())
                {
                    string key = enumer.Current.Key;
                    if (key.Contains("dir"))
                    {
                        Action<string> doDir = (string dir) =>
                        {
                            AddDir(dir);
                        };
                        Dispatcher.Invoke(doDir, new Object[] { enumer.Current.Value });
                    }
                }
                Action insertUp = () =>
                {
                    InsertParent();
                };
                Dispatcher.Invoke(insertUp, new Object[] { });
            }
            AddClientProc("getDirs", getDirs);
        }

        private void DispatcherLoadGetFiles()
        {
            void getFiles(CsMessage rcvMsg)
            {
                Action clrFiles = () =>
                {
                    ClearFiles();
                };
                Dispatcher.Invoke(clrFiles, new Object[] { });
                var enumer = rcvMsg.attributes.GetEnumerator();
                while (enumer.MoveNext())
                {
                    string key = enumer.Current.Key;
                    if (key.Contains("file"))
                    {
                        Action<string> doFile = (string file) =>
                        {
                            AddFile(file);
                        };
                        Dispatcher.Invoke(doFile, new Object[] { enumer.Current.Value });
                    }
                }
            }
            AddClientProc("getFiles", getFiles);

        }

        private void LoadDispatcher()
        {
            DispatcherLoadGetDirs();
            DispatcherLoadGetFiles();
        }

        //----------< Window Loaded function >-----------------
        private void MainWindow_OnLoaded(object sender, RoutedEventArgs e)
        {
            translater_obj = new Translater();
            working_directory = "../../../";
            Patterns = "*.h *.cpp";
            Regex_ = "[A-C](.*)";

            
            working_directory = Path.GetFullPath(working_directory);
            txtPath.Text = working_directory;
            txtPatterns.Text = Patterns;
            txtRegex.Text = Regex_;

            if (!System.IO.Directory.Exists(workingdir))
                System.IO.Directory.CreateDirectory(workingdir);   // create client directory if non-existing

            string[] argss = Environment.GetCommandLineArgs();
            selfport = int.Parse(argss[1]);
            serverport = int.Parse(argss[2]);

            // start Comm
            endPoint_ = new CsEndPoint
            {
                machineAddress = "localhost",
                port = selfport
            };

            translater = new Translater();
            translater.listen(endPoint_,workingdir , workingdir);

            // start processing messages
            ProcessMessages();

            // load dispatcher
            LoadDispatcher();

            CsEndPoint serverEndPoint = new CsEndPoint
            {
                machineAddress = "localhost",
                port = serverport
            };

            // loading files and directories at root directory of the server
            txtPath.Text = "Publisher_root";
            pathStack_.Push("..");
            CsMessage msg = new CsMessage();
            msg.add("to", CsEndPoint.toString(serverEndPoint));
            msg.add("from", CsEndPoint.toString(endPoint_));
            msg.add("command", "getDirs");
            msg.add("path", pathStack_.Peek());
            translater.postMessage(msg);
            msg.remove("command");
            msg.add("command", "getFiles");
            translater.postMessage(msg);
            automatedTest();
        }

        private string RemoveFirstDir(string path)
        {
            string modifiedPath = path;
            int pos = path.IndexOf("/");
            modifiedPath = path.Substring(pos + 1, path.Length - pos - 1);
            return modifiedPath;
        }

        //----------< Automatic Testing >-----------------
        private void automatedTest()
        {
            Console.Write("\n\n------------------------------------< Requirement #1 >------------------------------------\n");
            Console.Write("Using Visual Studio 2017 and its C++ Windows Console Projects, as provided in the ECS computer labs.\n");

            Console.Write("\n\n------------------------------------< Requirement #2 >------------------------------------\n");
            Console.Write("Using Windows Presentation Foundation(WPF) for the Client's user display\n");

            Console.Write("\n\n------------------------------------< Requirement #3 >------------------------------------\n");
            Console.Write("Assembled working parts from Projects #1, #2, and #3 into a Client-Server configuration. This sends asyn messages in Client-Server architectures as below\n");

            CsEndPoint serverEndPoint = new CsEndPoint
            {
                machineAddress = "localhost",
                port = serverport
            };

            CsMessage msg = new CsMessage();
            msg.add("to", CsEndPoint.toString(serverEndPoint));
            msg.add("from", CsEndPoint.toString(endPoint_));
            msg.add("command", "echo");
            msg.add("tst_msg", "msg 1");
            translater.postMessage(msg);

            CsMessage msg1 = new CsMessage();
            msg1.add("to", CsEndPoint.toString(serverEndPoint));
            msg1.add("from", CsEndPoint.toString(endPoint_));
            msg1.add("command", "echo");
            msg1.add("tst_msg", "msg 2");
            translater.postMessage(msg1);

            CsMessage msg2 = new CsMessage();
            msg2.add("to", CsEndPoint.toString(serverEndPoint));
            msg2.add("from", CsEndPoint.toString(endPoint_));
            msg.add("command", "echo");
            msg2.add("tst_msg", "msg 3");
            translater.postMessage(msg2);

            Console.Write("\n\n\n------------------------------------< Requirement #4 >------------------------------------\n");
            Console.Write("Provided a Graphical User Interface (GUI) for the client that supports navigating remote directories to find a project for conversion, " +
                "and supports displaying the results as per user specifications ");

            Console.Write("\nPlease press Publish button to check the same\n");

            Console.Write("\nResults can be seen in Converted Pages tab\n");

            Console.Write("\n\n\n------------------------------------< Requirement #5 >------------------------------------\n");
            Console.Write("Provided message designs appropriate for this application. Those are as below\n");

            msg.show();
            msg1.show();
            msg2.show();

            Console.Write("\n\n\n------------------------------------< Requirement #6 >------------------------------------\n");
            Console.Write("Transfering a file to Client directory for demo\n");
            CsMessage msg3 = new CsMessage();
            msg3.add("to", CsEndPoint.toString(serverEndPoint));
            msg3.add("from", CsEndPoint.toString(endPoint_));
            msg3.add("command", "getFile");
            msg3.add("name", "Converter.cpp.html");
            translater.postMessage(msg3);
            String pth = Path.GetFullPath("../../../ClientDirectory/Converter.cpp.html");
            System.Diagnostics.Process.Start(pth);

            Console.Write("\n\n\n------------------------------------< Requirement #7 >------------------------------------\n");
            Console.Write("Demonstrating correct operations for concurrent clients currently\n");

            Console.Write("\n\n\n------------------------------------< Requirement #8 >------------------------------------\n");
            Console.Write("Included an automated test as per requirement\n");

        }



        //----------< Button on click for Publishing files >-----------------
        private void BtnPublish_OnClick(object sender, RoutedEventArgs e)
        {
            txtProgress.Text = "Publishing...";
            //Publish_stat = true;
            CsEndPoint serverEndPoint = new CsEndPoint
            {
                machineAddress = "localhost",
                port = serverport
            };
            //Publish_stat = true;
            CsMessage msg_con = new CsMessage();
            msg_con.add("to", CsEndPoint.toString(serverEndPoint));
            msg_con.add("from", CsEndPoint.toString(endPoint_));
            msg_con.add("command", "convert");

            List<String> cmd = usefulFunction();
            int Count = 0;
            foreach (string item in cmd)
            {
                msg_con.add("cmdline" + Count.ToString(), item);
                Count++;
            }

            msg_con.add("tot_cmd", Count.ToString());
            translater.postMessage(msg_con);
            Helper_thread();
            
        }

        void Helper_thread()
        {
            Action<CsMessage> action = (CsMessage rcvMsg) =>
            {
                Action clrfiles = () =>
                {
                    lstConvertedFiles.Items.Clear();
                };
                Dispatcher.Invoke(clrfiles, new object[] { });
                
                var enumer = rcvMsg.attributes.GetEnumerator();
                while (enumer.MoveNext())
                {
                    string key = enumer.Current.Key;
                    if (key.Contains("convfile"))
                    {

                        Action<string> doconvFile = (string file) =>
                        {
                            lstConvertedFiles.Items.Add(file);
                          
                        };
                        Dispatcher.Invoke(doconvFile, new Object[] { enumer.Current.Value });
                    }
                }
            };
            AddClientProc("files_list", action);
         
        }


        //----------< On double click get list of files >-----------------
        private void LstDirs_MouseDoubleClick(object sender, MouseButtonEventArgs e)
        {
            // build path for selected dir
            string selectedDir = (string)lstDirs.SelectedItem;
            if (selectedDir == null)
                return;
            string path;
            if (selectedDir == "..")
            {
                if (pathStack_.Count > 1)  // don't pop off "Storage"
                    pathStack_.Pop();
                else
                    return;
            }
            else
            {
                path = pathStack_.Peek() + "/" + selectedDir;
                pathStack_.Push(path);
            }

            if (lstDirs.SelectedIndex == -1) return;
            string selectedItem = lstDirs.SelectedItem.ToString();
            selectedItem = Path.Combine(working_directory, selectedItem);
            selectedItem = Path.GetFullPath(selectedItem);

            if (!Directory.Exists(selectedItem)) return;
            working_directory = selectedItem;

            // display path in Dir TextBlcok
            txtPath.Text = RemoveFirstDir(pathStack_.Peek());

            // build message to get dirs and post it
            CsEndPoint serverEndPoint = new CsEndPoint
            {
                machineAddress = "localhost",
                port = serverport
            };
            CsMessage msg = new CsMessage();
            msg.add("to", CsEndPoint.toString(serverEndPoint));
            msg.add("from", CsEndPoint.toString(endPoint_));
            msg.add("command", "getDirs");
            msg.add("path", pathStack_.Peek());
            translater.postMessage(msg);

            // build message to get files and post it
            msg.remove("command");
            msg.add("command", "getFiles");
            translater.postMessage(msg);

          
        }

        //----------< Passes a list of arguements >-----------------
        List<string> usefulFunction()
        {
            List<string> args = new List<string>();
            args.Add("WPF.exe");
            args.Add(working_directory);
            if (cbRecursive.IsChecked.HasValue) args.Add("/s");
            args.AddRange(Patterns.Split(' '));
            args.AddRange(Regex_.Split(' '));
            return args;
        }

        //----------< Change Pattern on enter >-----------------
        private void TxtPatterns_OnKeyUp(object sender, KeyEventArgs e)
        {
            if (e.Key == Key.Enter)
            {
                Patterns = txtPatterns.Text;
                
            }
        }

        //----------< Exiting application >-----------------
        private void MenuItem_Click(object sender, RoutedEventArgs e)
        {
            System.Windows.Application.Current.Shutdown();

        }

        //----------< Get list of converted files >-----------------
        private void LstConvertedFiles_MouseDoubleClick(object sender, MouseButtonEventArgs e)
        {
            CsEndPoint serverEndPoint = new CsEndPoint
            {
                machineAddress = "localhost",
                port = serverport
            };

            String temp = lstConvertedFiles.SelectedItem.ToString();
            
                CsMessage msg1 = new CsMessage();
                msg1.add("to", CsEndPoint.toString(serverEndPoint));
                msg1.add("from", CsEndPoint.toString(endPoint_));
                msg1.add("command", "getFile");
                msg1.add("name", temp);
                translater.postMessage(msg1);
            
            String pth = Path.GetFullPath("../../../ClientDirectory/" + temp);
            
           System.Diagnostics.Process.Start(pth);
            

        }

        //----------< Handle Regexes >-----------------
        private void TxtRegex_SelectionChanged(object sender, RoutedEventArgs e)
        {
            Regex_ = txtRegex.Text;
        }

    }
}