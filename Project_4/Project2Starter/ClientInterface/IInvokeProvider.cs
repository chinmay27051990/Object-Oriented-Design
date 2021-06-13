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
namespace ClientInterface
{
    internal interface IInvokeProvider
    {
    }
}