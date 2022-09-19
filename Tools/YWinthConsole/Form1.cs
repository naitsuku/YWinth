using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace YWinthConsole {
    public partial class Form1 : Form {

        string rcvdata;

        public Form1() {
            InitializeComponent();
        }

        private void tabPage4_Click(object sender, EventArgs e) {

        }

        private void button1_Click(object sender, EventArgs e) {
            string port;
            if (serialPort1.IsOpen == true) {

            } else {

                port = portSelectcomboBox.Text;

                serialPort1.PortName = port;
                serialPort1.BaudRate = 19200;

                try {
                    /* 接続する！ */
                    serialPort1.Open();
                    MessageBox.Show(port + "にYWinthが接続されました。");
                    serEventTextBox.Text = "YWinth Connected in " + port;

                } catch (Exception ex) {
                    MessageBox.Show("接続できません");
                }
            }
            
        }

        private void rmtCtrl(char schr) {
            if (serialPort1.IsOpen == true) {
                serialPort1.WriteLine("@rm " + schr + ";");
            } else {
                MessageBox.Show("接続されていません.");
            }
        }

        private void rmtPlusButton_Click(object sender, EventArgs e) {
            rmtCtrl('U');
        }

        private void rmtMinusButton_Click(object sender, EventArgs e) {
            rmtCtrl('D');
        }

        private void rmtNextButton_Click(object sender, EventArgs e) {
            rmtCtrl('N');
        }

        private void rmtPrevButton_Click(object sender, EventArgs e) {
            rmtCtrl('P');
        }

        private void button2_Click(object sender, EventArgs e) {
            if (serialPort1.IsOpen == true) {
                serialPort1.Close();
                MessageBox.Show("YWinthを切断しました。");
                serEventTextBox.Text = "YWinth Disconnected.";
            }
        }

        private void Form1_Load(object sender, EventArgs e) {
            portSelectcomboBox.SelectedIndex = 0;
        }

        private void serialPort1_DataReceived(object sender, System.IO.Ports.SerialDataReceivedEventArgs e) {
            rcvdata +=  serialPort1.ReadExisting();
            
        }

        private void timer1_Tick(object sender, EventArgs e) {
            //serEventTextBox.Text = rcvdata;
        }
    }
}
