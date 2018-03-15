namespace SegmentationForm
{
    partial class HelloForm
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.InviteLabel = new System.Windows.Forms.Label();
            this.StartButton = new System.Windows.Forms.Button();
            this.SuspendLayout();
            // 
            // InviteLabel
            // 
            this.InviteLabel.AutoSize = true;
            this.InviteLabel.Location = new System.Drawing.Point(261, 141);
            this.InviteLabel.Name = "InviteLabel";
            this.InviteLabel.Size = new System.Drawing.Size(46, 17);
            this.InviteLabel.TabIndex = 0;
            this.InviteLabel.Text = "label1";
            // 
            // StartButton
            // 
            this.StartButton.Location = new System.Drawing.Point(232, 254);
            this.StartButton.Name = "StartButton";
            this.StartButton.Size = new System.Drawing.Size(75, 23);
            this.StartButton.TabIndex = 1;
            this.StartButton.Text = "button1";
            this.StartButton.UseVisualStyleBackColor = true;
            this.StartButton.Click += new System.EventHandler(this.StartButton_Click);
            // 
            // HelloForm
            // 
            this.ClientSize = new System.Drawing.Size(789, 616);
            this.Controls.Add(this.StartButton);
            this.Controls.Add(this.InviteLabel);
            this.Name = "HelloForm";
            this.Load += new System.EventHandler(this.HelloForm_Load_1);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.IO.FileSystemWatcher fileSystemWatcher1;
        private System.Windows.Forms.Label InviteLabel;
        private System.Windows.Forms.Button StartButton;
    }
}

