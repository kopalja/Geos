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
        }

        private void EditForm_Load(object sender, EventArgs e)
        {
            SegmentationBox.SetItemChecked((int)EditParams.segmentationType, true);
            TimeBox.SetItemCheckState((int)EditParams.timeOptimalization, CheckState.Checked);
            TetaBox.SetItemChecked((int)EditParams.boundSmoothness, true);
            ColorBox.SetItemChecked((int)EditParams.colorRepresentation, true);
        }


        private void SegmentationBox_SelectedIndexChanged(object sender, EventArgs e)
        {
            SetNewState(SegmentationBox);
            if (SegmentationBox.GetItemChecked(0))
                EditParams.segmentationType = EditParams.Segmentationtype.Sharp;
            else if (SegmentationBox.GetItemChecked(1))
                EditParams.segmentationType = EditParams.Segmentationtype.Color;
            else if (SegmentationBox.GetItemChecked(2))
                EditParams.segmentationType = EditParams.Segmentationtype.SharpAndColor;
        }

        private void TimeBox_SelectedIndexChanged(object sender, EventArgs e)
        {
            SetNewState(TimeBox);
            if (TimeBox.GetItemChecked(0))
                EditParams.timeOptimalization = EditParams.TimeOptimalization.Low;
            else if (TimeBox.GetItemChecked(1))
                EditParams.timeOptimalization = EditParams.TimeOptimalization.Medium;
            else if (TimeBox.GetItemChecked(2))
                EditParams.timeOptimalization = EditParams.TimeOptimalization.High;
        }

        private void TetaBox_SelectedIndexChanged(object sender, EventArgs e)
        {
            SetNewState(TetaBox);
            if (TetaBox.GetItemChecked(0))
                EditParams.boundSmoothness = EditParams.BoundSmoothness.Harsh;
            else if (TetaBox.GetItemChecked(1))
                EditParams.boundSmoothness = EditParams.BoundSmoothness.Normal;
            else if (TetaBox.GetItemChecked(2))
                EditParams.boundSmoothness = EditParams.BoundSmoothness.Smooth;
        }

        private void ColorBox_SelectedIndexChanged(object sender, EventArgs e)
        {
            SetNewState(ColorBox);
            if (ColorBox.GetItemChecked(0))
                EditParams.colorRepresentation = EditParams.ColorRepresentation.RGB;
            else if (ColorBox.GetItemChecked(1))
                EditParams.colorRepresentation = EditParams.ColorRepresentation.LAB;
        }


        private void SetNewState(CheckedListBox checkBox)
        {
            int iSelectedIndex = checkBox.SelectedIndex;
            if (iSelectedIndex == -1) return;
            for (int iIndex = 0; iIndex < checkBox.Items.Count; iIndex++)
                checkBox.SetItemCheckState(iIndex, CheckState.Unchecked);
            checkBox.SetItemCheckState(iSelectedIndex, CheckState.Checked);
        }




    }
}
