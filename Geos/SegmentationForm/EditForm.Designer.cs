namespace SegmentationForm
{
    partial class EditForm
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
            this.SegmentationLabel = new System.Windows.Forms.Label();
            this.tableLayoutPanel1 = new System.Windows.Forms.TableLayoutPanel();
            this.ColorLabel = new System.Windows.Forms.Label();
            this.ColorBox = new System.Windows.Forms.CheckedListBox();
            this.TetaLabel = new System.Windows.Forms.Label();
            this.TimeBox = new System.Windows.Forms.CheckedListBox();
            this.SegmentationBox = new System.Windows.Forms.CheckedListBox();
            this.TimeLabel = new System.Windows.Forms.Label();
            this.TetaBox = new System.Windows.Forms.CheckedListBox();
            this.tableLayoutPanel1.SuspendLayout();
            this.SuspendLayout();
            // 
            // SegmentationLabel
            // 
            this.SegmentationLabel.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.SegmentationLabel.AutoSize = true;
            this.SegmentationLabel.Font = new System.Drawing.Font("Microsoft Sans Serif", 10F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(238)));
            this.SegmentationLabel.Location = new System.Drawing.Point(50, 20);
            this.SegmentationLabel.Margin = new System.Windows.Forms.Padding(50, 20, 50, 10);
            this.SegmentationLabel.Name = "SegmentationLabel";
            this.SegmentationLabel.Size = new System.Drawing.Size(286, 70);
            this.SegmentationLabel.TabIndex = 0;
            this.SegmentationLabel.Text = "SegmentationType";
            this.SegmentationLabel.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // tableLayoutPanel1
            // 
            this.tableLayoutPanel1.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.tableLayoutPanel1.ColumnCount = 2;
            this.tableLayoutPanel1.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 50F));
            this.tableLayoutPanel1.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 50F));
            this.tableLayoutPanel1.Controls.Add(this.ColorLabel, 1, 2);
            this.tableLayoutPanel1.Controls.Add(this.ColorBox, 0, 3);
            this.tableLayoutPanel1.Controls.Add(this.TetaLabel, 0, 2);
            this.tableLayoutPanel1.Controls.Add(this.TimeBox, 1, 1);
            this.tableLayoutPanel1.Controls.Add(this.SegmentationBox, 0, 1);
            this.tableLayoutPanel1.Controls.Add(this.TimeLabel, 1, 0);
            this.tableLayoutPanel1.Controls.Add(this.SegmentationLabel, 0, 0);
            this.tableLayoutPanel1.Controls.Add(this.TetaBox, 0, 3);
            this.tableLayoutPanel1.Location = new System.Drawing.Point(12, 12);
            this.tableLayoutPanel1.Name = "tableLayoutPanel1";
            this.tableLayoutPanel1.RowCount = 4;
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 100F));
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 100F));
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 100F));
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 100F));
            this.tableLayoutPanel1.Size = new System.Drawing.Size(773, 400);
            this.tableLayoutPanel1.TabIndex = 1;
            // 
            // ColorLabel
            // 
            this.ColorLabel.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.ColorLabel.AutoSize = true;
            this.ColorLabel.Font = new System.Drawing.Font("Microsoft Sans Serif", 10F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(238)));
            this.ColorLabel.Location = new System.Drawing.Point(436, 220);
            this.ColorLabel.Margin = new System.Windows.Forms.Padding(50, 20, 50, 10);
            this.ColorLabel.Name = "ColorLabel";
            this.ColorLabel.Size = new System.Drawing.Size(287, 70);
            this.ColorLabel.TabIndex = 10;
            this.ColorLabel.Text = "Color Representation";
            this.ColorLabel.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // ColorBox
            // 
            this.ColorBox.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.ColorBox.CheckOnClick = true;
            this.ColorBox.Font = new System.Drawing.Font("Microsoft Sans Serif", 10F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(238)));
            this.ColorBox.FormattingEnabled = true;
            this.ColorBox.Items.AddRange(new object[] {
            "RGB",
            "LAB"});
            this.ColorBox.Location = new System.Drawing.Point(436, 310);
            this.ColorBox.Margin = new System.Windows.Forms.Padding(50, 10, 50, 10);
            this.ColorBox.Name = "ColorBox";
            this.ColorBox.Size = new System.Drawing.Size(287, 67);
            this.ColorBox.TabIndex = 9;
            this.ColorBox.SelectedIndexChanged += new System.EventHandler(this.ColorBox_SelectedIndexChanged);
            // 
            // TetaLabel
            // 
            this.TetaLabel.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.TetaLabel.AutoSize = true;
            this.TetaLabel.Font = new System.Drawing.Font("Microsoft Sans Serif", 10F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(238)));
            this.TetaLabel.Location = new System.Drawing.Point(50, 220);
            this.TetaLabel.Margin = new System.Windows.Forms.Padding(50, 20, 50, 10);
            this.TetaLabel.Name = "TetaLabel";
            this.TetaLabel.Size = new System.Drawing.Size(286, 70);
            this.TetaLabel.TabIndex = 7;
            this.TetaLabel.Text = "Bound smoothness";
            this.TetaLabel.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // TimeBox
            // 
            this.TimeBox.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.TimeBox.CheckOnClick = true;
            this.TimeBox.Font = new System.Drawing.Font("Microsoft Sans Serif", 10F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(238)));
            this.TimeBox.FormattingEnabled = true;
            this.TimeBox.Items.AddRange(new object[] {
            "Low",
            "Medium",
            "High"});
            this.TimeBox.Location = new System.Drawing.Point(436, 110);
            this.TimeBox.Margin = new System.Windows.Forms.Padding(50, 10, 50, 10);
            this.TimeBox.Name = "TimeBox";
            this.TimeBox.Size = new System.Drawing.Size(287, 67);
            this.TimeBox.TabIndex = 6;
            this.TimeBox.SelectedIndexChanged += new System.EventHandler(this.TimeBox_SelectedIndexChanged);
            // 
            // SegmentationBox
            // 
            this.SegmentationBox.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.SegmentationBox.CheckOnClick = true;
            this.SegmentationBox.Font = new System.Drawing.Font("Microsoft Sans Serif", 10F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(238)));
            this.SegmentationBox.FormattingEnabled = true;
            this.SegmentationBox.Items.AddRange(new object[] {
            "Sharp",
            "Color"});
            this.SegmentationBox.Location = new System.Drawing.Point(50, 110);
            this.SegmentationBox.Margin = new System.Windows.Forms.Padding(50, 10, 50, 10);
            this.SegmentationBox.Name = "SegmentationBox";
            this.SegmentationBox.Size = new System.Drawing.Size(286, 67);
            this.SegmentationBox.TabIndex = 5;
            this.SegmentationBox.SelectedIndexChanged += new System.EventHandler(this.SegmentationBox_SelectedIndexChanged);
            // 
            // TimeLabel
            // 
            this.TimeLabel.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.TimeLabel.AutoSize = true;
            this.TimeLabel.Font = new System.Drawing.Font("Microsoft Sans Serif", 10F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(238)));
            this.TimeLabel.Location = new System.Drawing.Point(436, 20);
            this.TimeLabel.Margin = new System.Windows.Forms.Padding(50, 20, 50, 10);
            this.TimeLabel.Name = "TimeLabel";
            this.TimeLabel.Size = new System.Drawing.Size(287, 70);
            this.TimeLabel.TabIndex = 1;
            this.TimeLabel.Text = "Time Optimalization";
            this.TimeLabel.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // TetaBox
            // 
            this.TetaBox.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.TetaBox.CheckOnClick = true;
            this.TetaBox.Font = new System.Drawing.Font("Microsoft Sans Serif", 10F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(238)));
            this.TetaBox.FormattingEnabled = true;
            this.TetaBox.Items.AddRange(new object[] {
            "Harsh",
            "Normal",
            "Smooth"});
            this.TetaBox.Location = new System.Drawing.Point(50, 310);
            this.TetaBox.Margin = new System.Windows.Forms.Padding(50, 10, 50, 10);
            this.TetaBox.Name = "TetaBox";
            this.TetaBox.Size = new System.Drawing.Size(286, 67);
            this.TetaBox.TabIndex = 8;
            this.TetaBox.SelectedIndexChanged += new System.EventHandler(this.TetaBox_SelectedIndexChanged);
            // 
            // EditForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 16F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(797, 453);
            this.Controls.Add(this.tableLayoutPanel1);
            this.Name = "EditForm";
            this.Text = "EditForm";
            this.Load += new System.EventHandler(this.EditForm_Load);
            this.tableLayoutPanel1.ResumeLayout(false);
            this.tableLayoutPanel1.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Label SegmentationLabel;
        private System.Windows.Forms.TableLayoutPanel tableLayoutPanel1;
        private System.Windows.Forms.Label TimeLabel;
        private System.Windows.Forms.CheckedListBox SegmentationBox;
        private System.Windows.Forms.CheckedListBox TimeBox;
        private System.Windows.Forms.Label ColorLabel;
        private System.Windows.Forms.CheckedListBox ColorBox;
        private System.Windows.Forms.Label TetaLabel;
        private System.Windows.Forms.CheckedListBox TetaBox;
    }
}