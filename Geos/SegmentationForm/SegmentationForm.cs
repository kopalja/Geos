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

namespace SegmentationForm
{

    enum SegmentationType{ Sharp, Color, Interactive }
    enum Pen { None, Foreground, Background };



    public partial class SegmentationForm : Form
    {

        [DllImport("Geos.dll", CallingConvention = CallingConvention.StdCall)]
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

       // [DllImport("Geos.dll", CallingConvention = CallingConvention.StdCall)]
       // static public extern void SegmentationProcess(String imagePath, int segmentationType, int[] labeling);

        private string inputImagePath;
        private Pen pen;
        private Bitmap originImage;
        private Bitmap opacityImage;
        private Bitmap resultImage;
        private Color backGroundColor;
        private string saveImagePath = "";
        private bool originImageVisible = true;

        // Parameters for segmentation
        private List<int> foregroundX = new List<int>();
        private List<int> foregroundY = new List<int>();

        private List<int> backgroundX = new List<int>();
        private List<int> backgroundY = new List<int>();

        int timeOptimalization;

        //

        public SegmentationForm()
        {
            InitializeComponent();
            this.FormBorderStyle = FormBorderStyle.FixedSingle;
            pictureBox.Top = 25;
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

        private void OriginPictureBox_MouseMove(object sender, MouseEventArgs e)
        {
            var position  = TranslateZoomMousePosition(new Point(e.Location.X, e.Location.Y));




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
            if ( saveImagePath != "")
                resultImage.Save(saveImagePath);

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
                        output.MakeTransparent(Color.White);
                        output.Save(stream, System.Drawing.Imaging.ImageFormat.Png);
                        saveImagePath = saveDialog.FileName;
                    }
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
            EditParams.segmentationType = EditParams.Segmentationtype.Sharp;
            TypeStrip.Text = "Type -> Sharp";
            SharpStrip.Checked = true;
            ColorStrip.Checked = false;

        }

        private void ColorStrip_Click(object sender, EventArgs e)
        {
            EditParams.segmentationType = EditParams.Segmentationtype.Color;
            TypeStrip.Text = "Type -> Color";
            SharpStrip.Checked = false;
            ColorStrip.Checked = true;
        }

        private void BlackStrip_Click(object sender, EventArgs e)
        {
            backGroundColor = Color.Black;
            BackgroundColorStrip.Text = "Background -> Black";
            BlackStrip.Checked = true;
            WhiteStrip.Checked = false;
            GreenStrip.Checked = false;
            TransparentStrip.Checked = false;
        }

        private void WhiteStrip_Click(object sender, EventArgs e)
        {
            backGroundColor = Color.White;
            BackgroundColorStrip.Text = "Background -> White";
            BlackStrip.Checked = false;
            WhiteStrip.Checked = true;
            GreenStrip.Checked = false;
            TransparentStrip.Checked = false;
        }

        private void GreenStrip_Click(object sender, EventArgs e)
        {
            backGroundColor = Color.Green;
            BackgroundColorStrip.Text = "Background -> Green";
            BlackStrip.Checked = false;
            WhiteStrip.Checked = false;
            GreenStrip.Checked = true;
            TransparentStrip.Checked = false;
        }

        private void TransparentStrip_Click(object sender, EventArgs e)
        {
            backGroundColor = Color.Transparent;
            BackgroundColorStrip.Text = "Background -> Transparent";
            BlackStrip.Checked = false;
            WhiteStrip.Checked = false;
            GreenStrip.Checked = false;
            TransparentStrip.Checked = true;
        }

        private void EditStrip_Click(object sender, EventArgs e)
        {
            EditForm editForm = new EditForm();
            editForm.ShowDialog(this);

            if (EditParams.segmentationType == EditParams.Segmentationtype.Sharp)
            {
                TypeStrip.Text = "Type -> Sharp";
                SharpStrip.Checked = true;
                ColorStrip.Checked = false;
            }
            else
            {
                TypeStrip.Text = "Type -> Color";
                SharpStrip.Checked = false;
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
            pictureBox.Image = opacityImage;
            progressBar1.Visible = true;
            Task.Factory.StartNew(Segment).ContinueWith(t => { progressBar1.Visible = false; pictureBox.Image = resultImage; SwitchStrip.Visible = true; originImageVisible = false; }, TaskScheduler.FromCurrentSynchronizationContext());
        }



        /*-------------------------------------------------------------------------------------------------------------------------------------------------------*/
        /*-------------------------------------------------------------------------------------------------------------------------------------------------------*/
        /*-------------------------------------------------------------------------------------------------------------------------------------------------------*/
        /*-------------------------------------------------------------------------------------------------------------------------------------------------------*/
        /*-------------------------------------------------------------------------------------------------------------------------------------------------------*/

        private void LoadImageDialog()
        {
            OpenFileDialog fileDialog = new OpenFileDialog();
            fileDialog.Filter = "Image files (*.jpg, *.jpeg, *.jpe) | *.jpg; *.jpeg; *.jpe";
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
                }
            }
        }

        private void SetSizes(int width, int height)
        {
            if (originImage.Width > 1000)
            {
                width = 1000;
                height = 1000 * height / originImage.Width;
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


            Console.WriteLine("match 1 " + foregroundX.Count.ToString() );

            if (foregroundX.Count == 0 || backgroundX.Count == 0)
                SegmentationProcess(
                    inputImagePath,
                    (int)EditParams.segmentationType,
                    (int)EditParams.timeOptimalization,
                    (int)EditParams.boundSmoothness,
                    (int)EditParams.colorRepresentation,
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
                    (int)EditParams.segmentationType,
                    (int)EditParams.timeOptimalization,
                    (int)EditParams.boundSmoothness,
                    (int)EditParams.colorRepresentation,
                    labeling,
                    foregroundX.ToArray(),
                    foregroundY.ToArray(),
                    backgroundX.ToArray(),
                    backgroundY.ToArray(),
                    foregroundX.Count,
                    backgroundX.Count
                    );


            for (int i = 0; i < resultImage.Width * resultImage.Height; i++)
            {
                if (labeling[i] < 0)
                    resultImage.SetPixel(i % resultImage.Width, i / resultImage.Width, backGroundColor);
            }
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





    }

    static class EditParams
    {
        public enum Segmentationtype { Sharp = 0, Color = 1, SharpAndColor = 2 }
        public static Segmentationtype segmentationType = Segmentationtype.SharpAndColor;

        public enum TimeOptimalization { Low = 0, Medium = 1, High = 2 }
        public static TimeOptimalization timeOptimalization = TimeOptimalization.Medium;

        public enum BoundSmoothness { Harsh = 0, Normal = 1, Smooth = 2 }
        public static BoundSmoothness boundSmoothness = BoundSmoothness.Normal;

        public enum ColorRepresentation { RGB = 0, LAB = 1 }
        public static ColorRepresentation colorRepresentation = ColorRepresentation.LAB;
    }
}
