﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Net.Sockets;

namespace MissionControl
{
    public class Connection
    {
        ///////////////////////////////////////////////////////////////////////////////////////////
        public bool Connect(string address, int port)
        {
            if(myClient == null)
            {
                myClient = new TcpClient();
                myBufferSize = 1024;
                myBuffer = new byte[myBufferSize];
            }
            try
            {
                myClient.Connect(address, port);
            }
            catch(SocketException e)
            {
                System.Windows.Forms.MessageBox.Show(
                    e.Message, "Connection Error", System.Windows.Forms.MessageBoxButtons.OK, System.Windows.Forms.MessageBoxIcon.Warning);
                return false;
            }
            return true;
        }

        ///////////////////////////////////////////////////////////////////////////////////////////
        public void Poll()
        {
            // See if some data is available.
            if(myClient.Available != 0)
            {
                // Read message header.
                myClient.GetStream().Read(myBuffer, 0, 4);

                String header = Encoding.UTF8.GetString(myBuffer, 0, 4);

                // Read message payload
                myClient.GetStream().Read(myBuffer, 0, 4);
                int size = BitConverter.ToInt32(myBuffer, 0);
                // Resize the buffer if needed.
                if(myBufferSize < size)
                {
                    myBufferSize = size;
                    myBuffer = new byte[myBufferSize];
                }
                myClient.GetStream().Read(myBuffer, 0, size);

                // Handle some default message types
                // smsg: log message, just print it to the console.
                if(header == "smsg")
                {
                    String message = Encoding.UTF8.GetString(myBuffer, 0, size);
                    message += System.Environment.NewLine;
                    MainWindow.Instance.PrintMessage(message);
                }
            }
        }

        ///////////////////////////////////////////////////////////////////////////////////////////
        public void SendCommand(string cmd)
        {
            if(myClient != null && myClient.Connected)
            {
                byte[] header = Encoding.UTF8.GetBytes("scmd");
                myClient.GetStream().Write(header, 0, 4);
                byte[] cmdBytes = Encoding.UTF8.GetBytes(cmd);
                byte[] cmdLengthBytes = BitConverter.GetBytes(cmdBytes.Length);
                myClient.GetStream().Write(cmdLengthBytes, 0, cmdLengthBytes.Length);
                myClient.GetStream().Write(cmdBytes, 0, cmdBytes.Length);
            }
        }

        ///////////////////////////////////////////////////////////////////////////////////////////
        public bool Connected
        {
            get
            {
                return (myClient != null && myClient.Connected);
            }
        }

        ///////////////////////////////////////////////////////////////////////////////////////////
        private TcpClient myClient;
        private byte[] myBuffer;
        private int myBufferSize;
    }
}
