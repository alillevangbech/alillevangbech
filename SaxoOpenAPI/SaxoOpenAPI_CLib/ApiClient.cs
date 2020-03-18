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
            string token = "eyJhbGciOiJFUzI1NiIsIng1dCI6IjhGQzE5Qjc0MzFCNjNFNTVCNjc0M0QwQTc5MjMzNjZCREZGOEI4NTAifQ.eyJvYWEiOiI3Nzc3NyIsImlzcyI6Im9hIiwiYWlkIjoiMTA5IiwidWlkIjoiR3ZLelRad1J5bXdhS1pDWmtsM3wxZz09IiwiY2lkIjoiR3ZLelRad1J5bXdhS1pDWmtsM3wxZz09IiwiaXNhIjoiRmFsc2UiLCJ0aWQiOiIyMDAyIiwic2lkIjoiNzg1ZmNmNGUyNjViNGFhMGIwNzNmZTE2MWVjY2RkMDgiLCJkZ2kiOiI4NCIsImV4cCI6IjE1ODQ1NjY2NTgifQ.Zt2QsRDhS17Q3guEwOSqiFQ-Dp_iCMbeVDPAM_z4eItPTlDjNdyKvyaucSE4s6l2itacpgrKDBz2wF-iR9pvmw";
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