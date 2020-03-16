using System;
using System.Net.Http;
using System.Threading.Tasks;
using SaxoServiceGroupsModels;

namespace saxoOpenAPI_CLib
{
    public static class TradingProcessor
    {
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