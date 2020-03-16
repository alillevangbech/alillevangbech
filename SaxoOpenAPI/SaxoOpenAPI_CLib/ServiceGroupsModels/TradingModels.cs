using System;
using Newtonsoft.Json;

namespace SaxoServiceGroupsModels
{
    public struct Order
    {
        /*
        POST
        URL: https://gateway.saxobank.com/sim/openapi/trade/v2/orders
        */

        public string OrderId { get; set; } 
    }

    public struct OrderParameters
    {
        [JsonProperty("Uic")]
        public int Uic { get; set; }

        [JsonProperty("BuySell")]
        public string BuySell { get; set; }

        [JsonProperty("AssetType")]
        public string AssetType { get; set; }

        [JsonProperty("Amount")]
        public int Amount { get; set; }

        [JsonProperty("OrderPrice")]
        public double OrderPrice { get; set; }

        [JsonProperty("OrderType")]
        public string OrderType { get; set; }

        [JsonProperty("OrderRelation")]
        public string OrderRelation { get; set; }

        [JsonProperty("ManuelOrder")]
        public bool ManuelOrder { get; set; }

        [JsonProperty("OrderDuration")]
        public OrderDuration OrderDuration { get; set; }

        [JsonProperty("AccountKey")]
        public string AccountKey { get; set; }
    }

    public struct OrderDuration
    {
        [JsonProperty("DurationType")]
        public string DurationType { get; set; }
    }
}