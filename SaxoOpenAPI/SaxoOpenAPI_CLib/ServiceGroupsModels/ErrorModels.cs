using Newtonsoft.Json;
using System;

namespace SaxoServiceGroupsModels
{

    public class ErrorModel
    {
        [JsonProperty("ErrorInfo")]
        public ErrorInfo ErrorInfo { get; set; }

        public void PrintError()
        {
            Console.WriteLine(" ############## ERROR ###############");
            Console.WriteLine($"ErrorCode: {this.ErrorInfo.ErrorCode}");
            Console.WriteLine($"Message: {this.ErrorInfo.Message}");
            Console.WriteLine(" ############# END ERROR ############");

        }

    }


    public struct ErrorInfo
    {
        [JsonProperty("ErrorCode")]
        public string ErrorCode { get; set; }
        [JsonProperty("Message")]
        public string Message { get; set; }
        //public ModelState ModelState { get; set; }

    }

    public struct ModelState
    {

        [JsonProperty("$skip")]
        public string skip { get; set; }
    }
}
