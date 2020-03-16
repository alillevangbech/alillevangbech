﻿using System;
using System.Net;
using saxoOpenAPI_CLib;
using System.Threading.Tasks;


/*
        public string UserKey { get; set; }
        public string Name {get; set;}
        public string LastLoginStatus { get; set; }
        public string UserId { get; set; } 
        public string[] LegalAssetTypes {get; set;}
*/

namespace SaxoOpenAPI_UI
{
    class Program
    {
        private static async void LoadAccountInfo()
        {
            var acc = await TradingProcessor.GET_AccountInfo();

            System.Console.WriteLine($"UserKey: {acc.UserKey}");
            System.Console.WriteLine($"ClientKey: {acc.ClientKey}");
            System.Console.WriteLine($"Name: {acc.Name}");
            System.Console.WriteLine($"LastLoginStatus: {acc.LastLoginStatus}");
            System.Console.WriteLine($"UserId: {acc.UserId}");
            
            System.Console.WriteLine("#######");
            foreach (var item in acc.LegalAssetTypes)
            {
                System.Console.WriteLine($"LegalAssetType: {item}");
            }
            System.Console.WriteLine("#######");
        }

        private static async void PlaceOrder()
        {
            var ord = await TradingProcessor.POST_Order();

            System.Console.WriteLine($"Order id: {ord.OrderId}");
        }

        static void Main(string[] args)
        {
            ApiHelper.Initialize();
            LoadAccountInfo();
            PlaceOrder();
            System.Console.Read();
        }
    }
}