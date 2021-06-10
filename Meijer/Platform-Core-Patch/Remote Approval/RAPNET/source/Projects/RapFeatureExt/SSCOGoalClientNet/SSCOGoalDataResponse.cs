using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Diagnostics;
using Newtonsoft.Json;
using Newtonsoft.Json.Serialization;
using Newtonsoft.Json.Converters;


namespace SSCOGoalClientNet
{
    /* These classes are not exposed outside of the assembly, since they are subject to change 
         * in the future. They are only used to send service requests and receive service responses */

    class CustomDateTimeConverter : IsoDateTimeConverter
    {
        public CustomDateTimeConverter()
        {
            base.DateTimeFormat = "yyyy-MM-dd";
        }
    }

    class CustomIsoDateTimeConverter : IsoDateTimeConverter
    {
        public CustomIsoDateTimeConverter()
        {
            base.DateTimeFormat = "yyyy-MM-ddTHH:mm:ss.fffZ";
        }
    }

    [JsonObject]
    class SSCOGoalRegisterData
    {
        public SSCOGoalRegisterData(string sscoGoalValue, string sscoActualValue)
        {
            SSCOGoalValue = sscoGoalValue;
            SSCOActualValue = sscoActualValue;
        }

        private string _sscoGoalValue;
        private string _sscoActualValue;


        [JsonProperty(PropertyName = "registerSCOGoal")]
        public string SSCOGoalValue
        {
            get { return _sscoGoalValue; }
            set { _sscoGoalValue = value; }
        }

        [JsonProperty(PropertyName = "registerSCOActual")]
        public string SSCOActualValue
        {
            get { return _sscoActualValue; }
            set { _sscoActualValue = value; }
        }
    }

    [JsonObject]
    class SSCOGoalDataResponse
    {
        public SSCOGoalDataResponse()
        {

        }

        public SSCOGoalDataResponse(string storeNumber, DateTime? reportDate, DateTime? retrievalDate, string goal, string actual)
        {
            DataResponseStoreNumber = storeNumber;
            DataResponseReportDate = reportDate;
            DataResponseRetrievalDate = retrievalDate;
            DataResponseSCOGoal = goal;
            DataResponseSCOActual = actual;
        }

        private string _drStoreNumber;
        private DateTime? _drReportDate;
        private DateTime? _drRetrievalDate;
        private string _drSCOGoal;
        private string _drSCOActual;
        private IDictionary<int, SSCOGoalRegisterData> _drRegisters = new System.Collections.Generic.Dictionary<int, SSCOGoalRegisterData>();

        [JsonProperty(PropertyName = "storeNbr")]
        public string DataResponseStoreNumber
        {
            get { return _drStoreNumber; }
            set { _drStoreNumber = value; }
        }

        [JsonProperty(PropertyName = "reportDate")]
        [JsonConverter(typeof(CustomDateTimeConverter))]
        public DateTime? DataResponseReportDate
        {
            get { return _drReportDate; }
            set { _drReportDate = value; }
        }

        [JsonProperty(PropertyName = "storeSCOGoal")]
        public string DataResponseSCOGoal
        {
            get { return _drSCOGoal; }
            set { _drSCOGoal = value; }
        }

        [JsonProperty(PropertyName = "storeSCOActual")]
        public string DataResponseSCOActual
        {
            get { return _drSCOActual; }
            set { _drSCOActual = value; }
        }

        [JsonProperty(PropertyName = "scoreSCORegisterDataList")]
        public IDictionary<int, SSCOGoalRegisterData> SSCOGoalRegisters
        {
            get { return _drRegisters; }
            set { _drRegisters = value; }
        }

        [JsonProperty(PropertyName = "dateTimeDataRetrieved")]
        [JsonConverter(typeof(CustomIsoDateTimeConverter))]
        public DateTime? DataResponseRetrievalDate
        {
            get { return _drRetrievalDate; }
            set { _drRetrievalDate = value; }
        }
    }

    [JsonObject]
    class SSCODataResponse
    {
        public SSCODataResponse(SSCOGoalDataResponse scoDataResponse)
        {
            Body = scoDataResponse;
        }

        private SSCOGoalDataResponse _scoDataResponse;

        [JsonProperty(PropertyName = "getSCODataResponse")]
        public SSCOGoalDataResponse Body
        {
            get { return _scoDataResponse; }
            set { _scoDataResponse = value; }
        }
    }
}
