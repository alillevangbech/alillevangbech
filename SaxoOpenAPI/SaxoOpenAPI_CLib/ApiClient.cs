using System;
using System.Net.Http;
using System.Net.Http.Headers;

namespace saxoOpenAPI_CLib
{
    public class ApiHelper
    {
        public static HttpClient ApiClient {get;set;}

        public static void Initialize()
        {
            string token = "eyJhbGciOiJFUzI1NiIsIng1dCI6IjhGQzE5Qjc0MzFCNjNFNTVCNjc0M0QwQTc5MjMzNjZCREZGOEI4NTAifQ.eyJvYWEiOiI3Nzc3NyIsImlzcyI6Im9hIiwiYWlkIjoiMTA5IiwidWlkIjoiR3ZLelRad1J5bXdhS1pDWmtsM3wxZz09IiwiY2lkIjoiR3ZLelRad1J5bXdhS1pDWmtsM3wxZz09IiwiaXNhIjoiRmFsc2UiLCJ0aWQiOiIyMDAyIiwic2lkIjoiOWExNjRlMmUzODljNDlmMWE4YjBlYzYxZTIyNjkwNmQiLCJkZ2kiOiI4NCIsImV4cCI6IjE1ODQzODU3NDgifQ.O-5cyUAgDi1PL4FFUsgwRqQIScPmQBaNjbIk0tPpu6yv5TTZZNTzdogAIhmbUE85Iz6NLZWUxcN97_AbBlzLDg";
            ApiClient = new HttpClient();
            ApiClient.DefaultRequestHeaders.Accept.Clear();
            ApiClient.DefaultRequestHeaders.Accept.Add( new MediaTypeWithQualityHeaderValue("application/json"));
            ApiClient.DefaultRequestHeaders.Authorization = new AuthenticationHeaderValue("BEARER",token);
            //ProductHeaderValue product = new ProductHeaderValue("Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.11 (KHTML, like Gecko) Chrome/23.0.1271.64 Safari/537.11",System.Reflection.Assembly.GetExecutingAssembly().GetName().Version.ToString());
            //ProductInfoHeaderValue UAgent = new ProductInfoHeaderValue(product);
            //ApiClient.DefaultRequestHeaders.UserAgent.ParseAdd("Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.11 (KHTML, like Gecko) Chrome/23.0.1271.64 Safari/537.11");
            ApiClient.DefaultRequestHeaders.UserAgent.Add(new System.Net.Http.Headers.ProductInfoHeaderValue("appname", System.Reflection.Assembly.GetExecutingAssembly().GetName().Version.ToString()));
        }
    }
}