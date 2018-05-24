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
using System.Drawing.Imaging;
using System.Threading;

namespace SegmentationForm
{

    enum SegmentationType{ Sharp, Color, Interactive }
    enum Pen { None, Foreground, Background };



    public partial class SegmentationForm : Form
    {

        [DllImport("Geos.dll", CallingConvention = CallingConvention.Cdecl)]
        static public extern void SegmentationProcess(
            String imagePath,
            int segmentationType,
            int timeOptimalization,
            int boundSmoothness,
            int colorRepresentation,
            int[] labeling,
            int[] foregroundX,
            int[] foregroundY,
            int[] backgroundX,
            int[] backgroundY,
            int foregroundSize,
            int backgroundSize
            );


        private const int maxWidth = 1000;
        private const int maxHeight = 700;

        private string inputImagePath;
        private Pen pen;
        private Bitmap originImage;
        private Bitmap opacityImage;
        private Bitmap resultImage;
        private Color backGroundColor;
        private string saveImagePath = "";
        private bool originImageVisible = true;
        private bool imageSelected = false;
        private CancellationTokenSource cancelToken = new CancellationTokenSource();

        // Parameters for segmentation
        private List<int> foregroundX = new List<int>();
        private List<int> foregroundY = new List<int>();

        private List<int> backgroundX = new List<int>();
        private List<int> backgroundY = new List<int>();


        //

        public SegmentationForm()
        {
            InitializeComponent();
            this.FormBorderStyle = FormBorderStyle.FixedSingle;
            pictureBox.Top = menuStrip1.Height;
            pictureBox.Left = 0;
            pictureBox.SizeMode = PictureBoxSizeMode.StretchImage;
            progressBar1.Visible = false;
            pen = Pen.None;
        }


        /*-----------Events--------------------------------------------------------------------------------------------------------------------------------------*/
        /*-------------------------------------------------------------------------------------------------------------------------------------------------------*/
        /*-------------------------------------------------------------------------------------------------------------------------------------------------------*/

        private void SegmentationForm_Load(object sender, EventArgs e)
        {
            LoadImageDialog();
        }

        private void UnlockButtons()
        {
            ParamsStrip.Enabled = true;
            SelectStrip.Enabled = true;
            StartStrip.Enabled = true;

        }

        private void OriginPictureBox_MouseMove(object sender, MouseEventArgs e)
        {
            if (imageSelected)
            {
                var position = TranslateZoomMousePosition(new Point(e.Location.X, e.Location.Y));
                Graphics g = Graphics.FromImage(originImage);
                if (pen != Pen.None && e.Button == System.Windows.Forms.MouseButtons.Left)
                {
                    if (pen == Pen.Foreground)
                    {
                        g.FillEllipse(Brushes.Blue, position.X - 10, position.Y - 10, 20, 20);

                        foregroundX.Add(position.X);
                        foregroundY.Add(position.Y);
                    }
                    else
                    {
                        g.FillEllipse(Brushes.Red, position.X - 10, position.Y - 10, 20, 20);

                        backgroundX.Add(position.X);
                        backgroundY.Add(position.Y);
                    }
                    pictureBox.Image = originImage;
                }
            }
        }




        /* File Strip Actions */
        private void NewFileStrip_Click(object sender, EventArgs e)
        {
            LoadImageDialog();
            foregroundX = new List<int>();
            foregroundY = new List<int>();
            backgroundX = new List<int>();
            backgroundY = new List<int>();
        }

        private void SaveStrip_Click(object sender, EventArgs e)
        {
            if (saveImagePath != "")
                resultImage.Save(saveImagePath);
            else
                SaveAsStrip_Click(sender, e);

        }

        private void SaveAsStrip_Click(object sender, EventArgs e)
        {
            Stream stream;
            SaveFileDialog saveDialog = new SaveFileDialog();
            saveDialog.Filter = "JPeg Image|*.jpg|Bitmap Image|*.bmp|Png Image|*.png";
            if (saveDialog.ShowDialog(this) == DialogResult.OK)
            {
                if ((stream = saveDialog.OpenFile()) != null)
                {
                    // Code to write the stream goes here.
                    if (saveDialog.FilterIndex == 1)
                        resultImage.Save(stream, System.Drawing.Imaging.ImageFormat.Jpeg);
                    else if (saveDialog.FilterIndex == 2)
                        resultImage.Save(stream, System.Drawing.Imaging.ImageFormat.Bmp);
                    else if (saveDialog.FilterIndex == 3)
                    {
                        Rectangle r = new Rectangle(0, 0, resultImage.Width, resultImage.Height);

                        Bitmap output = resultImage.Clone(r, System.Drawing.Imaging.PixelFormat.Format32bppArgb);
                        output.MakeTransparent(backGroundColor);
                        output.Save(stream, System.Drawing.Imaging.ImageFormat.Png);
                    }
                    saveImagePath = saveDialog.FileName;
                    stream.Close();
                }
            }
        }

        private void SwitchStrip_Click(object sender, EventArgs e)
        {
            if (SwitchStrip.Visible)
            {
                if (originImageVisible)
                {
                    pictureBox.Image = resultImage;
                    originImageVisible = false;
                }
                else
                {
                    pictureBox.Image = originImage;
                    originImageVisible = true;
                }
            }
        }

        private void QuitStrip_Click(object sender, EventArgs e)
        {
            this.Close();
        }









        /* Parameters Strip Actions */
        private void SharpStrip_Click(object sender, EventArgs e)
        {
            SegmentationParams.segmentationType = SegmentationParams.Segmentationtype.Sharp;
            TypeStrip.Text = "Type -> Sharp";
            SharpStrip.Checked = true;
            SharpAndColorStrip.Checked = false;
            ColorStrip.Checked = false;

        }

        private void SharpAndColorStrip_Click(object sender, EventArgs e)
        {
            SegmentationParams.segmentationType = SegmentationParams.Segmentationtype.SharpAndColor;
            TypeStrip.Text = "Type -> Sharp&Color";
            SharpStrip.Checked = false;
            SharpAndColorStrip.Checked = true;
            ColorStrip.Checked = false;
        }

        private void ColorStrip_Click(object sender, EventArgs e)
        {
            SegmentationParams.segmentationType = SegmentationParams.Segmentationtype.Color;
            TypeStrip.Text = "Type -> Color";
            SharpStrip.Checked = false;
            SharpAndColorStrip.Checked = false;
            ColorStrip.Checked = true;
        }

        private void BlackStrip_Click(object sender, EventArgs e)
        {
            backGroundColor = Color.Black;
            BackgroundColorStrip.Text = "Background -> Black";
            BlackStrip.Checked = true;
            WhiteStrip.Checked = false;
            GreenStrip.Checked = false;
        }

        private void WhiteStrip_Click(object sender, EventArgs e)
        {
            backGroundColor = Color.White;
            BackgroundColorStrip.Text = "Background -> White";
            BlackStrip.Checked = false;
            WhiteStrip.Checked = true;
            GreenStrip.Checked = false;
        }

        private void GreenStrip_Click(object sender, EventArgs e)
        {
            backGroundColor = Color.Green;
            BackgroundColorStrip.Text = "Background -> Green";
            BlackStrip.Checked = false;
            WhiteStrip.Checked = false;
            GreenStrip.Checked = true;
        }

        private void EditStrip_Click(object sender, EventArgs e)
        {
            EditForm editForm = new EditForm();
            editForm.ShowDialog(this);

            if (SegmentationParams.segmentationType == SegmentationParams.Segmentationtype.Sharp)
            {
                TypeStrip.Text = "Type -> Sharp";
                SharpStrip.Checked = true;
                SharpAndColorStrip.Checked = false;
                ColorStrip.Checked = false;
            }
            else if (SegmentationParams.segmentationType == SegmentationParams.Segmentationtype.SharpAndColor)
            {
                TypeStrip.Text = "Type -> SharpAndColor";
                SharpStrip.Checked = false;
                SharpAndColorStrip.Checked = true;
                ColorStrip.Checked = false;
            }
            else
            {
                TypeStrip.Text = "Type -> Color";
                SharpStrip.Checked = false;
                SharpAndColorStrip.Checked = false;
                ColorStrip.Checked = true;
            }
        }






        /* Select Strip Actions */
        private void NoneStrip_Click(object sender, EventArgs e)
        {
            pen = Pen.None;
            SelectStrip.Text = "Select";
            NoneStrip.Checked = true;
            ForegroundStrip.Checked = false;
            BackgroundStrip.Checked = false;
        }

        private void ForegroundStrip_Click(object sender, EventArgs e)
        {
            pen = Pen.Foreground;
            SelectStrip.Text = "Select -> Foreground";
            NoneStrip.Checked = false;
            ForegroundStrip.Checked = true;
            BackgroundStrip.Checked = false;
        }

        private void BackgroundStrip_Click(object sender, EventArgs e)
        {
            pen = Pen.Background;
            SelectStrip.Text = "Select -> Background";
            NoneStrip.Checked = false;
            ForegroundStrip.Checked = false;
            BackgroundStrip.Checked = true;
        }






        /* Start Strip Actions */
        private void StartStrip_Click(object sender, EventArgs e)
        {
            /* If in process */
            if (progressBar1.Visible)
            {
                cancelToken.Cancel();
                SegmentationEnd(succes: false);
            }
            else
            {
                if (SegmentationStart())
                {
                    Task.Factory.StartNew(Segment).ContinueWith(t =>
                    {
                        SegmentationEnd(succes: true);
                    }, cancelToken.Token, TaskContinuationOptions.None, TaskScheduler.FromCurrentSynchronizationContext());
                }
            }
        }

        private bool SegmentationStart()
        {
            if ((foregroundX.Count != 0 && backgroundX.Count == 0) || (foregroundX.Count == 0 && backgroundX.Count != 0))
            {
                MessageBox.Show("Background and foreground have to be specified");
                return false;
            }

            pictureBox.Image = opacityImage;
            progressBar1.Visible = true;
            SwitchStrip.Visible = false;

            fileToolStripMenuItem.Enabled = false;
            ParamsStrip.Enabled = false;
            SelectStrip.Enabled = false;

            StartStrip.Text = "Stop F5";
            StartStrip.Image = Properties.Resources.stopIcon3;
            return true;
        }

        private void SegmentationEnd(bool succes)
        {
            cancelToken = new CancellationTokenSource();
            progressBar1.Visible = false;
            pictureBox.Image = resultImage;
            if (succes)
                SwitchStrip.Visible = true;
            originImageVisible = false;
            SetSaveButtonsVisibility(succes);

            fileToolStripMenuItem.Enabled = true;
            ParamsStrip.Enabled = true;
            SelectStrip.Enabled = true;

            StartStrip.Text = "Sart F5";
            StartStrip.Image = Properties.Resources.startIcon;
        }


        /*-------------------------------------------------------------------------------------------------------------------------------------------------------*/
        /*-------------------------------------------------------------------------------------------------------------------------------------------------------*/
        /*-------------------------------------------------------------------------------------------------------------------------------------------------------*/
        /*-------------------------------------------------------------------------------------------------------------------------------------------------------*/
        /*-------------------------------------------------------------------------------------------------------------------------------------------------------*/

        private void LoadImageDialog()
        {
            OpenFileDialog fileDialog = new OpenFileDialog();
            fileDialog.Filter = "Image files (*.jpg ) (*.tif) | *.jpg; *.tif; ";
            fileDialog.RestoreDirectory = true;

            if (fileDialog.ShowDialog(this) == DialogResult.OK)
            {
                if (fileDialog.OpenFile() != null)
                {
                    originImage = new Bitmap(fileDialog.FileName);
                    SetSizes(originImage.Width, originImage.Height);

                    pictureBox.Image = originImage;
                    inputImagePath = fileDialog.FileName.ToString();
                    CreateOpacityImage();
                    SwitchStrip.Visible = false;
                    imageSelected = true;
                    SetSaveButtonsVisibility(false);
                    NoneStrip_Click(null, null);

                }
            }
            if (imageSelected)
                UnlockButtons();
        }

        private void SetSaveButtonsVisibility(bool enabled)
        {
            SaveStrip.Enabled = enabled;
            SaveAsStrip.Enabled = enabled;
        }

        private void SetSizes(int width, int height)
        {
            if (originImage.Width > maxWidth)
            {
                width = maxWidth;
                height = (int)Math.Round((double)maxWidth * (double)originImage.Height / (double)originImage.Width);
            }
            if (height > maxHeight)
            {
                width = (int)Math.Round((double)maxHeight * (double)width / (double)height);
                height = maxHeight;
            }
            pictureBox.Width = width;
            pictureBox.Height = height;
            this.Width = width;
            this.Height = height + 65;
            progressBar1.Top = this.Height / 2 - 10;
            progressBar1.Left = this.Width / 2 - 100;
            progressBar1.Width = 200;
            progressBar1.Height = 20;
        }


        private void CreateOpacityImage()
        {
            opacityImage = new Bitmap(originImage.Width, originImage.Height);
            float opacity = 0.4f;
            Graphics g = Graphics.FromImage(opacityImage);
            //create a color matrix object  
            ColorMatrix matrix = new ColorMatrix();      

            //set the opacity  
            matrix.Matrix33 = opacity;  

            //create image attributes  
            ImageAttributes attributes = new ImageAttributes();      

            //set the color(opacity) of the image  
            attributes.SetColorMatrix(matrix, ColorMatrixFlag.Default, ColorAdjustType.Bitmap);    

            //now draw the image  
            g.DrawImage(originImage, new Rectangle(0, 0, opacityImage.Width, opacityImage.Height), 0, 0, opacityImage.Width, opacityImage.Height, GraphicsUnit.Pixel, attributes);
        }

        private void Segment()
        {
            resultImage = new Bitmap(inputImagePath);
            //result
            int[] labeling = new int[resultImage.Width * resultImage.Height];


            if (foregroundX.Count == 0 || backgroundX.Count == 0)
                SegmentationProcess(
                    inputImagePath,
                    (int)SegmentationParams.segmentationType,
                    (int)SegmentationParams.timeOptimalization,
                    (int)SegmentationParams.boundSmoothness,
                    (int)SegmentationParams.colorRepresentation,
                    labeling,
                    null,
                    null,
                    null,
                    null,
                    0,
                    0
                    );
            else
                SegmentationProcess(
                    inputImagePath,
                    (int)SegmentationParams.segmentationType,
                    (int)SegmentationParams.timeOptimalization,
                    (int)SegmentationParams.boundSmoothness,
                    (int)SegmentationParams.colorRepresentation,
                    labeling,
                    foregroundX.ToArray(),
                    foregroundY.ToArray(),
                    backgroundX.ToArray(),
                    backgroundY.ToArray(),
                    foregroundX.Count,
                    backgroundX.Count
                    );

            Console.WriteLine("Dll finished");
            SetBitmap(labeling);
           // Compare(labeling);
            //Compare2();

            //for (int i = 0; i < resultImage.Width * resultImage.Height; i++)
            //{
            //    if (labeling[i] < 0)
            //        resultImage.SetPixel(i % resultImage.Width, i / resultImage.Width, backGroundColor);
            //}
        }

        private void Compare2()
        {
            System.IO.StreamWriter file = new StreamWriter("C:\\xps15\\TestyBc\\geos\\score.txt");
            double sum = 0;
            for (int i = 1; i <= 31; i++)
            {
                string adobePath = "C:\\xps15\\TestyBc\\geos\\sample" + i.ToString() + ".jpg";
                string groundPath = "C:\\xps15\\TestyBc\\groundtruth\\sample" + i.ToString() + ".jpg";

                Bitmap b1 = new Bitmap(adobePath);
                Bitmap b2 = new Bitmap(groundPath);

                int match = 0;
                for (int y = 0; y < b1.Height; y++)
                {
                    for (int x = 0; x < b1.Width; x++)
                    {
                        var temp1 = b1.GetPixel(x, y);
                        var temp2 = b2.GetPixel(x, y);

                        if (temp1.R > 100 && temp1.G > 100 && temp1.B > 100 && temp2.R > 100 && temp2.G > 100 && temp2.B > 100)
                            match++;
                        else if (temp1.R < 100 && temp1.G < 100 && temp1.B < 100 && temp2.R < 100 && temp2.G < 100 && temp2.B < 100)
                            match++;
                    }
                }
                double ans = (double)match / (double)(b1.Width * b1.Height) * 100.0;
                sum += Math.Round(ans, 2);
                file.WriteLine(i.ToString() + '\t' + Math.Round(ans, 2));
            }
            sum /= 31;
            sum = Math.Round(sum, 2);
            file.WriteLine("======");
            file.WriteLine("mean" + '\t' + sum.ToString());
            file.Close();
        }

        protected Point TranslateZoomMousePosition(Point coordinates)
        {
            int Width = pictureBox.Width;
            int Height = pictureBox.Height;

            // Make sure our control width and height are not 0
            if (Width == 0 || Height == 0) return coordinates;
            // First, get the ratio (image to control) the height and width
            float ratioWidth = (float)originImage.Width / Width;
            float ratioHeight = (float)originImage.Height / Height;
            // Scale the points by our ratio
            float newX = coordinates.X;
            float newY = coordinates.Y;
            newX *= ratioWidth;
            newY *= ratioHeight;
            return new Point((int)newX, (int)newY);
        }

        unsafe private void SetBitmap(int[] labeling)
        {
            BitmapData imageData = resultImage.LockBits(new Rectangle(0, 0, resultImage.Width, resultImage.Height), ImageLockMode.ReadWrite, PixelFormat.Format24bppRgb);
            int bytesPerPixel = 3;

            byte* scan0 = (byte*)imageData.Scan0.ToPointer();
            int stride = imageData.Stride;




            int i = 0;
            for (int y = 0; y < imageData.Height; y++)
            {
                byte* row = scan0 + (y * stride);

                for (int x = 0; x < imageData.Width; x++)
                {
                    // Watch out for actual order (BGR)!
                    if (labeling[i] < 0)
                    {
                        int bIndex = x * bytesPerPixel;
                        int gIndex = bIndex + 1;
                        int rIndex = bIndex + 2;

                        row[rIndex] = backGroundColor.R;
                        row[gIndex] = backGroundColor.G;
                        row[bIndex] = backGroundColor.B;
                    }
                    i++;
                }
            }

            resultImage.UnlockBits(imageData);

        }

        private void Compare(int[] labeling)
        {
            string from = "C:\\Users\\kopi\\Downloads\\groundtruth\\sample6.jpg";
            Bitmap toSave = new Bitmap(originImage.Width, originImage.Height);
            Bitmap ground = new Bitmap(from);
            int i = 0;
            int match = 0;
            for (int y = 0; y < ground.Height; y++)
            {
                for (int x = 0; x < ground.Width; x++)
                {
                    var p = ground.GetPixel(x, y);
                    if (labeling[i] >= 0 && p.R > 100 && p.G > 100 && p.B > 100)
                        match++;
                    else if (labeling[i] < 0 && p.R == 0 && p.G == 0 && p.B == 0)
                        match++;

                    if (labeling[i] >= 0)
                        toSave.SetPixel(x, y, Color.White);
                    else
                        toSave.SetPixel(x, y, Color.Black);
                    i++;
                }
            }
            toSave.Save(from.Replace("groundtruth", "geos"));
            Console.WriteLine( "match " + (double)match / (double)(ground.Width * ground.Height) * 100.0 + " %");
        }
    }


    static class SegmentationParams
    {
        public enum Segmentationtype { Sharp = 0, Color = 1, SharpAndColor = 2 }
        public static Segmentationtype segmentationType = Segmentationtype.Sharp;

        public enum TimeOptimalization { Low = 0, Medium = 1, High = 2 }
        public static TimeOptimalization timeOptimalization = TimeOptimalization.Medium;

        public enum BoundSmoothness { Harsh = 0, Normal = 1, Smooth = 2 }
        public static BoundSmoothness boundSmoothness = BoundSmoothness.Normal;

        public enum ColorRepresentation { RGB = 0, LAB = 1 }
        public static ColorRepresentation colorRepresentation = ColorRepresentation.RGB;
    }
}
