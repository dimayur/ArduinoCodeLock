using Microsoft.AspNetCore.Mvc.ViewFeatures;

namespace Arduino_API.Model
{
    public class UserModule
    {
        public int Id { get; set; }
        public string User { get; set; }
        public string Password { get; set; }
        public string RFIDData { get; set; }
    }
}
