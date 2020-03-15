using System;
using System.Net.Http;
using System.Threading.Tasks;

namespace saxoOpenAPI_CLib
{
    public static class TradingProcessor
    {

        public static async Task<AccountInfoModel> LoadAccount()
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
                    throw new Exception(response.ReasonPhrase);
                }
            }
        }
    }
}