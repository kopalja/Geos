using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.IO;
using System.Runtime.InteropServices;
using System.Diagnostics;



namespace SegmentationForm
{
    public partial class HelloForm : Form
    {
        public static bool nextForm = false;

        public HelloForm()
        {
            InitializeComponent();

            InviteLabel.Font = new Font(FontFamily.GenericSansSerif, 14);
            InviteLabel.Text = "Choose image to segment";


            InviteLabel.Location = new Point(this.Width / 2 - InviteLabel.Width / 2, 100);
            StartButton.Location = new Point(this.Width / 2 - StartButton.Width / 2, InviteLabel.Location.Y + InviteLabel.Height + 20);
        }






        private void HelloForm_Load(object sender, EventArgs e)
        {
        }

        private void StartButton_Click(object sender, EventArgs e)
        {
            nextForm = true;
            this.Close();
        }

        private void HelloForm_Load_1(object sender, EventArgs e)
        {

        }
    }
}
