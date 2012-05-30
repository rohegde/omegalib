using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace MissionControl
{
    public partial class MacroWindow: Form
    {
        ///////////////////////////////////////////////////////////////////////////////////////////
        public MacroWindow()
        {
            InitializeComponent();
        }

        ///////////////////////////////////////////////////////////////////////////////////////////
        private void MacroWindow_Load(object sender, EventArgs e)
        {
            foreach(Macro m in MainWindow.Instance.Macros)
            {
                myMacroList.Items.Add(m);
            }
        }

        ///////////////////////////////////////////////////////////////////////////////////////////
        private void myMacroList_SelectedIndexChanged(object sender, EventArgs e)
        {
            Macro m = (Macro)myMacroList.SelectedItem;
            if(m != null)
            {
                myShortcutBox.Text = m.Shortcut;
                myScriptBox.Text = m.Script;

                mySaveButton.Enabled = false;
                myRunButton.Enabled = true;
                myDeleteButton.Enabled = true;
            }
        }

        ///////////////////////////////////////////////////////////////////////////////////////////
        private void myShortcutBox_TextChanged(object sender, EventArgs e)
        {
            mySaveButton.Enabled = true;
        }

        ///////////////////////////////////////////////////////////////////////////////////////////
        private void mySaveButton_Click(object sender, EventArgs e)
        {
            Macro newMacro = new Macro();
            newMacro.Script = myScriptBox.Text;
            newMacro.Shortcut = myShortcutBox.Text;

            MainWindow.Instance.Macros.Add(newMacro);
            myMacroList.Items.Add(newMacro);
        }

        ///////////////////////////////////////////////////////////////////////////////////////////
        private void myDeleteButton_Click(object sender, EventArgs e)
        {
            myShortcutBox.Text = "";
            myScriptBox.Text = "";

            Macro oldMacro = (Macro)myMacroList.SelectedItem;
            myMacroList.Items.Remove(oldMacro);
            MainWindow.Instance.Macros.Remove(oldMacro);

            mySaveButton.Enabled = false;
            myRunButton.Enabled = false;
            myDeleteButton.Enabled = false;
        }

        ///////////////////////////////////////////////////////////////////////////////////////////
        private void myRunButton_Click(object sender, EventArgs e)
        {
            Connection conn = MainWindow.Instance.Connection;
            conn.SendCommand(myScriptBox.Text);
        }
    }
}
