using System;
using System.Net.Http;
using System.Text;
using System.Threading.Tasks;
using Newtonsoft.Json;
using SaxoServiceGroupsModels;
using System.Collections.Generic;
using System.Reflection;

namespace saxoOpenAPI_CLib
{
    public static class TradingProcessor
    {
        public static async Task<T> GetRequest<T>(string url) where T : struct
        {
            using (HttpResponseMessage response = await ApiHelper.ApiClient.GetAsync(url))
            {
                if (response.IsSuccessStatusCode)
                {
                    T data = await response.Content.ReadAsAsync<T>();
                    return data;
                }
                else
                {
                    ErrorModel error_response = await response.Content.ReadAsAsync<ErrorModel>();
                    error_response.PrintError();
                    throw new Exception(response.ReasonPhrase);
                }
            } 

        }






        public static async Task<AccountInfoModel> GET_AccountInfo()
        {
            string url = "https://gateway.saxobank.com/sim/openapi/port/v1/users/me";
            using (HttpResponseMessage response = await ApiHelper.ApiClient.GetAsync(url))
            {
                if (response.IsSuccessStatusCode)
                {
                    AccountInfoModel account = await response.Content.ReadAsAsync<AccountInfoModel>();
                    return account;
                }
                else
                {
                    ErrorModel error_response = await response.Content.ReadAsAsync<ErrorModel>();
                    error_response.PrintError();
                    throw new Exception(response.ReasonPhrase);
                }
            }
        }

        public static async Task<Order> POST_Order()
        {
            string url = "https://gateway.saxobank.com/sim/openapi/trade/v2/orders";

            var content = new OrderParameters { 
                Uic = 16,
                BuySell = "Buy",
                AssetType = "FxSpot",
                Amount = 500,
                OrderPrice = 7,
                OrderType = "Limit",
                OrderRelation = "StandAlone",
                ManuelOrder = true,
                OrderDuration = new OrderDuration { 
                    DurationType = "GoodTillCancel"
                },
                AccountKey = "GvKzTZwRymwaKZCZkl3|1g=="
            };

            // Serialize our concrete class into a JSON String
            var stringContent = JsonConvert.SerializeObject(content);
            var httpContent = new StringContent(stringContent, Encoding.UTF8, "application/json");


            //ApiHelper.ApiContent = new StringContent


            using (HttpResponseMessage response = await ApiHelper.ApiClient.PostAsync(url, httpContent))
            {

                if (response.IsSuccessStatusCode)
                {
                        Order order_response = await response.Content.ReadAsAsync<Order>();
                        return order_response;
                }
                else
                {
                    ErrorModel error_response = await response.Content.ReadAsAsync<ErrorModel>();
                    error_response.PrintError();
                    throw new Exception(response.ReasonPhrase);
                }


            }
        }




        public static async Task<AccountInfoModel> GET_AccountValue()
        {
            string url = "https://gateway.saxobank.com/sim/openapi/hist/v3/accountValues/{ClientKey}/?MockDataId={MockDataId}";
            using (HttpResponseMessage response = await ApiHelper.ApiClient.GetAsync(url))
            {
                if (response.IsSuccessStatusCode)
                {
                    AccountInfoModel account = await response.Content.ReadAsAsync<AccountInfoModel>();
                    return account;
                }
                else
                {
                    throw new Exception(response.ReasonPhrase);
                }
            }
        }
    }
}