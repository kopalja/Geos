using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace SegmentationForm
{
    public partial class EditForm : Form
    {
        public EditForm()
        {
            InitializeComponent();
            this.FormBorderStyle = FormBorderStyle.FixedSingle; 
            InitCheck();
        }

        private void InitCheck()
        {
            switch (SegmentationParams.segmentationType)
            {
                case SegmentationParams.Segmentationtype.Sharp:
                    {
                        TypeButton1.Checked = true;
                        break;
                    }
                case SegmentationParams.Segmentationtype.Color:
                    {
                        TypeButton3.Checked = true;
                        break;
                    }
                case SegmentationParams.Segmentationtype.SharpAndColor:
                    {
                        TypeButton2.Checked = true;
                        break;
                    }
            }
            switch (SegmentationParams.timeOptimalization)
            {
                case SegmentationParams.TimeOptimalization.Low:
                    {
                        TimeButton1.Checked = true;
                        break;
                    }
                case SegmentationParams.TimeOptimalization.Medium:
                    {
                        TimeButton2.Checked = true;
                        break;
                    }
                case SegmentationParams.TimeOptimalization.High:
                    {
                        TimeButton3.Checked = true;
                        break;
                    }
            }
            switch (SegmentationParams.boundSmoothness)
            {
                case SegmentationParams.BoundSmoothness.Harsh:
                    {
                        BoundButton1.Checked = true;
                        break;
                    }
                case SegmentationParams.BoundSmoothness.Normal:
                    {
                        BoundButton2.Checked = true;
                        break;
                    }
                case SegmentationParams.BoundSmoothness.Smooth:
                    {
                        BoundButton3.Checked = true;
                        break;
                    }
            }
            switch (SegmentationParams.colorRepresentation)
            {
                case SegmentationParams.ColorRepresentation.RGB:
                    {
                        ColorButton1.Checked = true;
                        break;
                    }
                case SegmentationParams.ColorRepresentation.LAB:
                    {
                        ColorButton2.Checked = true;
                        break;
                    }
            }

        }

        private void EditForm_Load(object sender, EventArgs e)
        {

        }

        private void TypeButton1_CheckedChanged(object sender, EventArgs e)
        {
            SegmentationParams.segmentationType = SegmentationParams.Segmentationtype.Sharp;
        }

        private void TypeButton2_CheckedChanged(object sender, EventArgs e)
        {
            SegmentationParams.segmentationType = SegmentationParams.Segmentationtype.SharpAndColor;
        }

        private void TypeButton3_CheckedChanged(object sender, EventArgs e)
        {
            SegmentationParams.segmentationType = SegmentationParams.Segmentationtype.Color;
        }

        private void TimeButton1_CheckedChanged(object sender, EventArgs e)
        {
            SegmentationParams.timeOptimalization = SegmentationParams.TimeOptimalization.Low;
        }

        private void TimeButton2_CheckedChanged(object sender, EventArgs e)
        {
            SegmentationParams.timeOptimalization = SegmentationParams.TimeOptimalization.Medium;
        }

        private void TimeButton3_CheckedChanged(object sender, EventArgs e)
        {
            SegmentationParams.timeOptimalization = SegmentationParams.TimeOptimalization.High;
        }

        private void BoundButton1_CheckedChanged(object sender, EventArgs e)
        {
            SegmentationParams.boundSmoothness = SegmentationParams.BoundSmoothness.Harsh;
        }

        private void BoundButton2_CheckedChanged(object sender, EventArgs e)
        {
            SegmentationParams.boundSmoothness = SegmentationParams.BoundSmoothness.Normal;
        }

        private void BoundButton3_CheckedChanged(object sender, EventArgs e)
        {
            SegmentationParams.boundSmoothness = SegmentationParams.BoundSmoothness.Smooth;
        }

        private void ColorButton1_CheckedChanged(object sender, EventArgs e)
        {
            SegmentationParams.colorRepresentation = SegmentationParams.ColorRepresentation.RGB;
        }

        private void ColorButton2_CheckedChanged(object sender, EventArgs e)
        {
            SegmentationParams.colorRepresentation = SegmentationParams.ColorRepresentation.LAB;
        }
    }
}
