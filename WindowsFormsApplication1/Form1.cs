using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

using System.Runtime.InteropServices;//Windows.Forms;

namespace WindowsFormsApplication1
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }

        //public

        // 图像灰度化
        private void button1_Click(object sender, EventArgs e)
        {
            //Console.WriteLine("hello.  ");
            bool op = false;
            if (op)
            {
                System.Windows.Forms.MessageBox.Show("button1_Click ", "帮助");
            }

            bool opt = true;//   false;// true;// false;
            if (opt)
            {
                TestCppDll tc = new TestCppDll();
                int res = TestCppDll.add_lk(1, 2);
                //string   str_v = 
                System.Windows.Forms.MessageBox.Show("res = " + res, "计算");

                //TestCppDll.add_lk(3, 5);
            }
        }

        // 图像灰度化
        private void toolStripLabel1_Click(object sender, EventArgs e)
        {
            System.Windows.Forms.MessageBox.Show("toolStripLabel1_Click  ", "提示");
        }

        private void Form1_Load(object sender, EventArgs e)
        {

        }
    }

    public class TestCppDll
    {
        public TestCppDll()
        {
        }

        //[DllImport("classLibOpencvBase.dll", CallingConvention = CallingConvention.cdecl)]
        //[DllImport(@"classLibOpencvBase.dll", CallingConvention = CallingConvention.cdecl)]
        //[DllImport(@"classLibOpencvBase.dll", EntryPoint = "add_lk")]
        [DllImport(@"classLibOpencvBase.dll")]
        public extern static int add_lk(int a, int b);
    }
}




