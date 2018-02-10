namespace SegmentationForm
{
    partial class SegmentationForm
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(SegmentationForm));
            this.pictureBox = new System.Windows.Forms.PictureBox();
            this.menuStrip1 = new System.Windows.Forms.MenuStrip();
            this.fileToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.NewFileStrip = new System.Windows.Forms.ToolStripMenuItem();
            this.SaveStrip = new System.Windows.Forms.ToolStripMenuItem();
            this.SaveAsStrip = new System.Windows.Forms.ToolStripMenuItem();
            this.QuitStrip = new System.Windows.Forms.ToolStripMenuItem();
            this.ParamsStrip = new System.Windows.Forms.ToolStripMenuItem();
            this.TypeStrip = new System.Windows.Forms.ToolStripMenuItem();
            this.SharpStrip = new System.Windows.Forms.ToolStripMenuItem();
            this.ColorStrip = new System.Windows.Forms.ToolStripMenuItem();
            this.BackgroundColorStrip = new System.Windows.Forms.ToolStripMenuItem();
            this.BlackStrip = new System.Windows.Forms.ToolStripMenuItem();
            this.WhiteStrip = new System.Windows.Forms.ToolStripMenuItem();
            this.GreenStrip = new System.Windows.Forms.ToolStripMenuItem();
            this.TransparentStrip = new System.Windows.Forms.ToolStripMenuItem();
            this.EditStrip = new System.Windows.Forms.ToolStripMenuItem();
            this.SelectStrip = new System.Windows.Forms.ToolStripMenuItem();
            this.ForegroundStrip = new System.Windows.Forms.ToolStripMenuItem();
            this.BackgroundStrip = new System.Windows.Forms.ToolStripMenuItem();
            this.StartStrip = new System.Windows.Forms.ToolStripMenuItem();
            this.SwitchStrip = new System.Windows.Forms.ToolStripMenuItem();
            this.progressBar1 = new System.Windows.Forms.ProgressBar();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox)).BeginInit();
            this.menuStrip1.SuspendLayout();
            this.SuspendLayout();
            // 
            // pictureBox
            // 
            this.pictureBox.Location = new System.Drawing.Point(0, 28);
            this.pictureBox.Margin = new System.Windows.Forms.Padding(0);
            this.pictureBox.Name = "pictureBox";
            this.pictureBox.Size = new System.Drawing.Size(819, 633);
            this.pictureBox.TabIndex = 0;
            this.pictureBox.TabStop = false;
            this.pictureBox.MouseMove += new System.Windows.Forms.MouseEventHandler(this.OriginPictureBox_MouseMove);
            // 
            // menuStrip1
            // 
            this.menuStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.fileToolStripMenuItem,
            this.ParamsStrip,
            this.SelectStrip,
            this.StartStrip,
            this.SwitchStrip});
            this.menuStrip1.Location = new System.Drawing.Point(0, 0);
            this.menuStrip1.Name = "menuStrip1";
            this.menuStrip1.Size = new System.Drawing.Size(980, 28);
            this.menuStrip1.TabIndex = 2;
            this.menuStrip1.Text = "menuStrip1";
            // 
            // fileToolStripMenuItem
            // 
            this.fileToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.NewFileStrip,
            this.SaveStrip,
            this.SaveAsStrip,
            this.QuitStrip});
            this.fileToolStripMenuItem.Name = "fileToolStripMenuItem";
            this.fileToolStripMenuItem.Size = new System.Drawing.Size(44, 24);
            this.fileToolStripMenuItem.Text = "File";
            // 
            // NewFileStrip
            // 
            this.NewFileStrip.Image = ((System.Drawing.Image)(resources.GetObject("NewFileStrip.Image")));
            this.NewFileStrip.Name = "NewFileStrip";
            this.NewFileStrip.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.N)));
            this.NewFileStrip.Size = new System.Drawing.Size(217, 24);
            this.NewFileStrip.Text = "New File";
            this.NewFileStrip.Click += new System.EventHandler(this.NewFileStrip_Click);
            // 
            // SaveStrip
            // 
            this.SaveStrip.Image = ((System.Drawing.Image)(resources.GetObject("SaveStrip.Image")));
            this.SaveStrip.Name = "SaveStrip";
            this.SaveStrip.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.S)));
            this.SaveStrip.Size = new System.Drawing.Size(217, 24);
            this.SaveStrip.Text = "Save";
            this.SaveStrip.Click += new System.EventHandler(this.SaveStrip_Click);
            // 
            // SaveAsStrip
            // 
            this.SaveAsStrip.Image = ((System.Drawing.Image)(resources.GetObject("SaveAsStrip.Image")));
            this.SaveAsStrip.Name = "SaveAsStrip";
            this.SaveAsStrip.ShortcutKeys = ((System.Windows.Forms.Keys)(((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.Shift) 
            | System.Windows.Forms.Keys.S)));
            this.SaveAsStrip.Size = new System.Drawing.Size(217, 24);
            this.SaveAsStrip.Text = "Save as";
            this.SaveAsStrip.Click += new System.EventHandler(this.SaveAsStrip_Click);
            // 
            // QuitStrip
            // 
            this.QuitStrip.Name = "QuitStrip";
            this.QuitStrip.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.Q)));
            this.QuitStrip.Size = new System.Drawing.Size(217, 24);
            this.QuitStrip.Text = "Quit";
            this.QuitStrip.Click += new System.EventHandler(this.QuitStrip_Click);
            // 
            // ParamsStrip
            // 
            this.ParamsStrip.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.TypeStrip,
            this.BackgroundColorStrip,
            this.EditStrip});
            this.ParamsStrip.Name = "ParamsStrip";
            this.ParamsStrip.Size = new System.Drawing.Size(94, 24);
            this.ParamsStrip.Text = "Parameters";
            // 
            // TypeStrip
            // 
            this.TypeStrip.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.SharpStrip,
            this.ColorStrip});
            this.TypeStrip.Name = "TypeStrip";
            this.TypeStrip.Size = new System.Drawing.Size(217, 24);
            this.TypeStrip.Text = "Type -> Sharp";
            // 
            // SharpStrip
            // 
            this.SharpStrip.Checked = true;
            this.SharpStrip.CheckState = System.Windows.Forms.CheckState.Checked;
            this.SharpStrip.Name = "SharpStrip";
            this.SharpStrip.Size = new System.Drawing.Size(116, 24);
            this.SharpStrip.Text = "Sharp";
            this.SharpStrip.Click += new System.EventHandler(this.SharpStrip_Click);
            // 
            // ColorStrip
            // 
            this.ColorStrip.Name = "ColorStrip";
            this.ColorStrip.Size = new System.Drawing.Size(116, 24);
            this.ColorStrip.Text = "Color";
            this.ColorStrip.Click += new System.EventHandler(this.ColorStrip_Click);
            // 
            // BackgroundColorStrip
            // 
            this.BackgroundColorStrip.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.BlackStrip,
            this.WhiteStrip,
            this.GreenStrip,
            this.TransparentStrip});
            this.BackgroundColorStrip.Name = "BackgroundColorStrip";
            this.BackgroundColorStrip.Size = new System.Drawing.Size(217, 24);
            this.BackgroundColorStrip.Text = "BackGround -> Black";
            // 
            // BlackStrip
            // 
            this.BlackStrip.Checked = true;
            this.BlackStrip.CheckState = System.Windows.Forms.CheckState.Checked;
            this.BlackStrip.Name = "BlackStrip";
            this.BlackStrip.Size = new System.Drawing.Size(155, 24);
            this.BlackStrip.Text = "Black";
            this.BlackStrip.Click += new System.EventHandler(this.BlackStrip_Click);
            // 
            // WhiteStrip
            // 
            this.WhiteStrip.Name = "WhiteStrip";
            this.WhiteStrip.Size = new System.Drawing.Size(155, 24);
            this.WhiteStrip.Text = "White";
            this.WhiteStrip.Click += new System.EventHandler(this.WhiteStrip_Click);
            // 
            // GreenStrip
            // 
            this.GreenStrip.Name = "GreenStrip";
            this.GreenStrip.Size = new System.Drawing.Size(155, 24);
            this.GreenStrip.Text = "Green";
            this.GreenStrip.Click += new System.EventHandler(this.GreenStrip_Click);
            // 
            // TransparentStrip
            // 
            this.TransparentStrip.Name = "TransparentStrip";
            this.TransparentStrip.Size = new System.Drawing.Size(155, 24);
            this.TransparentStrip.Text = "Transparent";
            this.TransparentStrip.Click += new System.EventHandler(this.TransparentStrip_Click);
            // 
            // EditStrip
            // 
            this.EditStrip.Image = ((System.Drawing.Image)(resources.GetObject("EditStrip.Image")));
            this.EditStrip.Name = "EditStrip";
            this.EditStrip.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.E)));
            this.EditStrip.Size = new System.Drawing.Size(217, 24);
            this.EditStrip.Text = "Edit...";
            this.EditStrip.Click += new System.EventHandler(this.EditStrip_Click);
            // 
            // SelectStrip
            // 
            this.SelectStrip.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.ForegroundStrip,
            this.BackgroundStrip});
            this.SelectStrip.Name = "SelectStrip";
            this.SelectStrip.Size = new System.Drawing.Size(162, 24);
            this.SelectStrip.Text = "Select -> Foreground";
            // 
            // ForegroundStrip
            // 
            this.ForegroundStrip.Checked = true;
            this.ForegroundStrip.CheckState = System.Windows.Forms.CheckState.Checked;
            this.ForegroundStrip.Name = "ForegroundStrip";
            this.ForegroundStrip.Size = new System.Drawing.Size(157, 24);
            this.ForegroundStrip.Text = "Foreground";
            this.ForegroundStrip.Click += new System.EventHandler(this.ForegroundStrip_Click);
            // 
            // BackgroundStrip
            // 
            this.BackgroundStrip.Name = "BackgroundStrip";
            this.BackgroundStrip.Size = new System.Drawing.Size(157, 24);
            this.BackgroundStrip.Text = "Background";
            this.BackgroundStrip.Click += new System.EventHandler(this.BackgroundStrip_Click);
            // 
            // StartStrip
            // 
            this.StartStrip.Image = ((System.Drawing.Image)(resources.GetObject("StartStrip.Image")));
            this.StartStrip.Name = "StartStrip";
            this.StartStrip.ShortcutKeys = System.Windows.Forms.Keys.F5;
            this.StartStrip.Size = new System.Drawing.Size(95, 24);
            this.StartStrip.Text = "Start   F5";
            this.StartStrip.Click += new System.EventHandler(this.StartStrip_Click);
            // 
            // SwitchStrip
            // 
            this.SwitchStrip.Name = "SwitchStrip";
            this.SwitchStrip.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.Tab)));
            this.SwitchStrip.Size = new System.Drawing.Size(128, 24);
            this.SwitchStrip.Text = "Switch  Ctrl+Tab";
            this.SwitchStrip.Click += new System.EventHandler(this.SwitchStrip_Click);
            // 
            // progressBar1
            // 
            this.progressBar1.Anchor = System.Windows.Forms.AnchorStyles.Top;
            this.progressBar1.Location = new System.Drawing.Point(-25, 176);
            this.progressBar1.Margin = new System.Windows.Forms.Padding(30);
            this.progressBar1.MarqueeAnimationSpeed = 50;
            this.progressBar1.Name = "progressBar1";
            this.progressBar1.Size = new System.Drawing.Size(395, 94);
            this.progressBar1.Style = System.Windows.Forms.ProgressBarStyle.Marquee;
            this.progressBar1.TabIndex = 4;
            // 
            // SegmentationForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 16F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.BackColor = System.Drawing.Color.Black;
            this.ClientSize = new System.Drawing.Size(980, 808);
            this.Controls.Add(this.progressBar1);
            this.Controls.Add(this.pictureBox);
            this.Controls.Add(this.menuStrip1);
            this.MainMenuStrip = this.menuStrip1;
            this.Name = "SegmentationForm";
            this.Text = "SegmentationForm";
            this.Load += new System.EventHandler(this.SegmentationForm_Load);
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox)).EndInit();
            this.menuStrip1.ResumeLayout(false);
            this.menuStrip1.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.PictureBox pictureBox;
        private System.Windows.Forms.MenuStrip menuStrip1;
        private System.Windows.Forms.ToolStripMenuItem fileToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem NewFileStrip;
        private System.Windows.Forms.ToolStripMenuItem SaveStrip;
        private System.Windows.Forms.ToolStripMenuItem SaveAsStrip;
        private System.Windows.Forms.ToolStripMenuItem QuitStrip;
        private System.Windows.Forms.ToolStripMenuItem ParamsStrip;
        private System.Windows.Forms.ProgressBar progressBar1;
        private System.Windows.Forms.ToolStripMenuItem TypeStrip;
        private System.Windows.Forms.ToolStripMenuItem SharpStrip;
        private System.Windows.Forms.ToolStripMenuItem ColorStrip;
        private System.Windows.Forms.ToolStripMenuItem BackgroundColorStrip;
        private System.Windows.Forms.ToolStripMenuItem BlackStrip;
        private System.Windows.Forms.ToolStripMenuItem WhiteStrip;
        private System.Windows.Forms.ToolStripMenuItem GreenStrip;
        private System.Windows.Forms.ToolStripMenuItem TransparentStrip;
        private System.Windows.Forms.ToolStripMenuItem SelectStrip;
        private System.Windows.Forms.ToolStripMenuItem ForegroundStrip;
        private System.Windows.Forms.ToolStripMenuItem BackgroundStrip;
        private System.Windows.Forms.ToolStripMenuItem StartStrip;
        private System.Windows.Forms.ToolStripMenuItem EditStrip;
        private System.Windows.Forms.ToolStripMenuItem SwitchStrip;
    }
}