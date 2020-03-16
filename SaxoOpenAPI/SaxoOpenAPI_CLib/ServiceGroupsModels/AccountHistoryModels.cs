using System;

namespace SaxoServiceGroupsModels
{
    public struct AccountInfoModel
    {
        /*
        GET
        URL: https://gateway.saxobank.com/sim/openapi/hist/v1/users/me
        */
        
        public string ClientKey {get; set;}
        public string Culture {get;set;}
        public string Language {get;set;}
        public string LastLoginStatus { get; set; }
        public string LastLoginTime {get;set;}
        public string[] LegalAssetTypes {get; set;}
        public string MarketDataViaOpenApiTermsAccepted {get;set;}
        public string Name {get; set;}
        public int TimeZoneId {get;set;}
        public string UserId { get; set; } 
        public string UserKey { get; set; }

    }

    public struct AccountValueModel
    {
        /*
        GET
        URL: https://gateway.saxobank.com/sim/openapi/hist/v3/accountValues/{ClientKey}/?MockDataId={MockDataId}
        Data:
        {
            AccountValueModel
        }
        */
        public double AccountValue{get;set;}
        public double AccountValueMonth{get;set;}
        public double AccountValueYear{get;set;}
        public string Key{get;set;}
        public string KeyType{get;set;}
    }
}