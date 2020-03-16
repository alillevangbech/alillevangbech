using System;

namespace SaxoServiceGroupsModels
{
    public struct AccountInfoModel
    {
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
}