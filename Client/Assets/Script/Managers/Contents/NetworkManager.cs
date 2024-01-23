using System.Collections;
using System.Collections.Generic;
using System;
using UnityEngine;
using System.Net.Sockets;
using System.Text;
using Google.Protobuf;

public class NetworkManager : MonoBehaviour
{
    private TcpClient tcpClient;
    private NetworkStream stream;
    private byte[] receiveBuffer = new byte[4096];

    private bool isConnected = false;

    private void Start()
    {
        Debug.Log("hey!");
        ConnectToServer("127.0.0.1", 7777);
    }

    private void Update()
    {
        if (!isConnected) return;

        if (tcpClient == null || !tcpClient.Connected)
        {
            Debug.Log("Not connected to the server.");
            return;
        }

        if (stream.DataAvailable)
        {
            int bytesRead = stream.Read(receiveBuffer, 0, receiveBuffer.Length);
            string receivedMessage = Encoding.UTF8.GetString(receiveBuffer, 0, bytesRead);
            Debug.Log($"Received from server: {receivedMessage}");
        }
    }

    private void ConnectToServer(string ipAddress, int port)
    {
        try
        {
            tcpClient = new TcpClient(ipAddress, port);
            stream = tcpClient.GetStream();
            Debug.Log("Connected to the server.");
            isConnected = true;
        }
        catch (Exception e)
        {
            Debug.LogError($"Connected Server Error: {e.Message}");
        }
    }

    public void SendMessageToServer(string message)
    {
        if (tcpClient == null || !tcpClient.Connected)
        {
            Debug.LogError("Not connected to the server.");
            return;
        }

        byte[] data = Encoding.UTF8.GetBytes(message);
        stream.Write(data, 0, data.Length);
        Debug.Log($"Sent to server: {message}");
    }

    private void OnDestroy()
    {
        if (tcpClient != null)
        {
            tcpClient.Close();
            Debug.Log("Disconnected from the server.");
        }
    }
}
