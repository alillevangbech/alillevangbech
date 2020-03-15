using System;

namespace saxoOpenAPI_CLib
{
    public class AccountInfoModel
    {
        public string UserKey { get; set; }
        public string Name {get; set;}
        public string LastLoginStatus { get; set; }
        public string UserId { get; set; } 
        public string[] LegalAssetTypes {get; set;}

    }
}