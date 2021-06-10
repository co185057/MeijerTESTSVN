// MigrateReportingDatabase.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#import "c:\Program Files\Common Files\system\ado\msadox.dll" //no_namespace

#import "c:\Program Files\Common Files\System\ADO\msado15.dll" \
    no_namespace rename("EOF", "EndOfFile")
//#include <msado15.tlh>
#import "c:\Program Files\Common Files\Microsoft Shared\dao\dao360.dll"  \
     rename("EOF", "EndOfFile")
#include "MigrateReportingDatabase.h"
#include "FLReportTextFile.h"
#include <msjro.tlh>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#define DEFAULT_CONNECT_STRING _T("Provider=Microsoft.Jet.OLEDB.4.0; Data Source = %s\\scot\\Report\\database\\FLReports.mdb; User ID=admin;Password=;")
#define DATABASE_NAME _T("%s\\scot\\Report\\database\\FLReports.mdb")
#define MDB_CANTOPEN -1
#define MDB_CANTINITIALIZE -2

// This variable should be set to the new "latest" version
#define CURRENT_VERSION 14

// This should be a list that contain the versions of each new feature
#define PICK_LIST_VERSION 2
#define PATCH32A_UPDATES_VERSION 3
#define REL_40_VERSION 4
#define REL_40_VERSION_2 5
#define REL_40_VERSION_3 6
#define REL_40_VERSION_4 7
#define REL_32D_ATTENDANTMODE 8
#define REL_41_VERSION_4 9		//FL4.1 REQUIREMENT
#define REL_41_VERSION_5 10
#define REL_41_VERSION_6 11
#define REL_41_VERSION_7 12  //FL4.2 REQUIRMENT (turkish, swedish and finnish)
#define REL_42_VERSION_1 13  //FL4.2 Enterprise Reporting Enhancements
#define REL_42_VERSION_2 14

/////////////////////////////////////////////////////////////////////////////
// Query Updates
#define TODAYS_HOURLY_ASSISTANCE_REPORT_LANE_QUERY _T("SELECT Store.BusinessDate, Transactions.TransStartDateTime, Transactions.StationID, 0 AS Trans, 0 AS RAPIntervention, Count(Interventions.InterventionID) AS LaneIntervention, Store.Language \
FROM Store INNER JOIN (Transactions INNER JOIN Interventions ON (Transactions.StationID = Interventions.StationID) AND (Transactions.TransactionID = Interventions.TransactionID)) ON Store.BusinessDate = Transactions.BusinessDate \
WHERE (((Interventions.ClearStationID)<>\"RAP\") AND ((Interventions.OperatorID)<>\"\" And (Interventions.OperatorID)<>\"0000\") AND ((Transactions.TransAssistedFlag)=True) AND ((Interventions.InTransaction)=True)) \
GROUP BY Store.BusinessDate, Transactions.TransStartDateTime, Transactions.StationID, Store.Language;")
#define TODAYS_HOURLY_ASSISTANCE_REPORT_LANE _T("TodaysHourlyAssistanceReportLane")

#define TODAYS_HOURLY_ASSISTANCE_REPORT_TRANS_QUERY _T("SELECT Store.BusinessDate, Transactions.TransStartDateTime, Transactions.StationID, Count(Transactions.TransactionID) AS Trans, 0 AS RAPIntervention, 0 AS LaneIntervention, Store.Language \
FROM Store INNER JOIN (Transactions INNER JOIN Interventions ON (Transactions.StationID = Interventions.StationID) AND (Transactions.TransactionID = Interventions.TransactionID)) ON Store.BusinessDate = Transactions.BusinessDate \
GROUP BY Store.BusinessDate, Transactions.TransStartDateTime, Transactions.StationID, Store.Language, Interventions.OperatorID \
HAVING (((Interventions.OperatorID)<>\"\" And (Interventions.OperatorID)<>\"0000\"));")
#define TODAYS_HOURLY_ASSISTANCE_REPORT_TRANS _T("TodaysHourlyAssistanceReportTrans")

#define YESTERDAYS_HOURLY_ASSISTANCE_REPORT_LANE_QUERY _T("SELECT Store.BusinessDate, Transactions.TransStartDateTime, Transactions.StationID, 0 AS Trans, 0 AS RAPIntervention, Count(Interventions.InterventionID) AS LaneIntervention, Store.Language \
FROM (Transactions INNER JOIN Interventions ON (Transactions.TransactionID = Interventions.TransactionID) AND (Transactions.StationID = Interventions.StationID)) INNER JOIN Store ON Transactions.BusinessDate = Store.YesterdayBusinessDate \
WHERE (((Interventions.ClearStationID)<>\"RAP\") AND ((Interventions.OperatorID)<>\"\" And (Interventions.OperatorID)<>\"0000\") AND ((Transactions.TransAssistedFlag)=True) AND ((Interventions.InTransaction)=True)) \
GROUP BY Store.BusinessDate, Transactions.TransStartDateTime, Transactions.StationID, Store.Language;")
#define YESTERDAYS_HOURLY_ASSISTANCE_REPORT_LANE _T("YesterdayHourlyAssistanceReportLane")

#define YESTERDAYS_HOURLY_ASSISTANCE_REPORT_TRANS_QUERY _T("SELECT Store.BusinessDate, Transactions.TransStartDateTime, Transactions.StationID, Count(Transactions.TransactionID) AS Trans, 0 AS RAPIntervention, 0 AS LaneIntervention, Store.Language \
FROM (Transactions INNER JOIN Interventions ON (Transactions.TransactionID = Interventions.TransactionID) AND (Transactions.StationID = Interventions.StationID)) INNER JOIN Store ON Transactions.BusinessDate = Store.YesterdayBusinessDate \
WHERE (((Interventions.OperatorID)<>\"\" And (Interventions.OperatorID)<>\"0000\")) \
GROUP BY Store.BusinessDate, Transactions.TransStartDateTime, Transactions.StationID, Store.Language;")
#define YESTERDAYS_HOURLY_ASSISTANCE_REPORT_TRANS _T("YesterdayHourlyAssistanceReportTrans")

#define ASSISTANCE_REPORT_ONE_DAY_SUMMARY_ASSISTS_QUERY _T("SELECT Count(Transactions.TransactionID) AS NumAssists, 0 AS NumTrans, Store.Language \
FROM Store INNER JOIN Transactions ON Store.BusinessDate = Transactions.BusinessDate \
WHERE (((Transactions.TransAssistedFlag)=True) AND ((Transactions.TotalNumberOfItems)<>0)) OR (((Transactions.TotalNumberOfItems)<>0) AND ((Transactions.TransRAPAssistedFlag)=True)) \
GROUP BY 0, Store.Language; ")
#define ASSISTANCE_REPORT_ONE_DAY_SUMMARY_ASSISTS _T("AssistanceReportOneDaySummaryAssists")

#define ASSISTANCE_REPORT_YESTERDAY_SUMMARY_ASSISTS_QUERY _T("SELECT Count(Transactions.TransactionID) AS NumAssists, 0 AS NumTrans, Store.Language \
FROM Transactions INNER JOIN Store ON Transactions.BusinessDate = Store.YesterdayBusinessDate \
WHERE (((Transactions.TransAssistedFlag)=True) AND ((Transactions.TotalNumberOfItems)<>0)) OR (((Transactions.TotalNumberOfItems)<>0) AND ((Transactions.TransRAPAssistedFlag)=True)) \
GROUP BY 0, Store.Language; ")
#define ASSISTANCE_REPORT_YESTERDAY_SUMMARY_ASSISTS _T("AssistanceReportYesterdaySummaryAssists")

#define ASSISTANCE_REPORT_ONE_DAY_SUMMARY_TRANS_QUERY _T("SELECT 0 AS NumAssists, Count(Transactions.TransactionID) AS NumTrans, Store.Language \
FROM Store INNER JOIN Transactions ON Store.BusinessDate = Transactions.BusinessDate \
WHERE (((Transactions.TotalNumberOfItems)<>0)) \
GROUP BY 0, Store.Language; ")
#define ASSISTANCE_REPORT_ONE_DAY_SUMMARY_TRANS _T("AssistanceReportOneDaySummaryTrans")

#define ASSISTANCE_REPORT_YESTERDAY_SUMMARY_TRANS_QUERY _T("SELECT 0 AS NumAssists, Count(Transactions.TransactionID) AS NumTrans, Store.Language \
FROM Store INNER JOIN Transactions ON Transactions.BusinessDate = Store.YesterdayBusinessDate \
WHERE (((Transactions.TotalNumberOfItems)<>0)) \
GROUP BY 0, Store.Language; ")
#define ASSISTANCE_REPORT_YESTERDAY_SUMMARY_TRANS _T("AssistanceReportYesterdaySummaryTrans")

#define ASSISTANCE_REPORT_ONE_DAY_SUMMARY_LANE_QUERY _T("SELECT Interventions.InterventionClass, 0 AS NumRAPInterventions, Count(Interventions.StationID & Interventions.TransactionID & Interventions.InterventionID) AS NumLaneInterventions, 0 AS NumPager, Store.Language \
FROM Store INNER JOIN (Transactions INNER JOIN Interventions ON (Transactions.TransactionID = Interventions.TransactionID) AND (Transactions.StationID = Interventions.StationID)) ON Store.BusinessDate = Transactions.BusinessDate \
WHERE (((Interventions.ClearStationID)<>\"RAP\") AND ((Transactions.TransAssistedFlag)=True) AND ((Interventions.InTransaction)=True) AND ((Interventions.OperatorID)<>\"\" And (Interventions.OperatorID)<>\"0000\")) \
GROUP BY Interventions.InterventionClass, 0, 0, Store.Language; ") 
#define ASSISTANCE_REPORT_ONE_DAY_SUMMARY_LANE _T("AssistanceReportOneDaySummaryLane")

#define ASSISTANCE_REPORT_YESTERDAY_SUMMARY_LANE_QUERY _T("SELECT Interventions.InterventionClass, 0 AS NumRAPInterventions, Count(Interventions.StationID & Interventions.TransactionID & Interventions.InterventionID) AS NumLaneInterventions, 0 AS NumPager, Store.Language \
FROM Store INNER JOIN (Transactions INNER JOIN Interventions ON (Transactions.TransactionID = Interventions.TransactionID) AND (Transactions.StationID = Interventions.StationID)) ON Store.YesterdayBusinessDate = Transactions.BusinessDate \
WHERE (((Interventions.ClearStationID)<>\"RAP\") AND ((Transactions.TransAssistedFlag)=True) AND ((Interventions.InTransaction)=True) AND ((Interventions.OperatorID)<>\"\" And (Interventions.OperatorID)<>\"0000\")) \
GROUP BY Interventions.InterventionClass, 0, 0, Store.Language; ") 
#define ASSISTANCE_REPORT_YESTERDAY_SUMMARY_LANE _T("AssistanceReportYesterdaySummaryLane")

#define ASSISTANCE_REPORT_ONE_DAY_SUMMARY_ASSISTS_BY_LANE_QUERY _T("SELECT Count(Transactions.TransactionID) AS NumAssists, 0 AS NumTrans, Transactions.StationID, Store.Language \
FROM Store INNER JOIN Transactions ON Store.BusinessDate = Transactions.BusinessDate \
WHERE (((Transactions.TransAssistedFlag)=True) AND ((Transactions.TotalNumberOfItems)<>0)) OR (((Transactions.TotalNumberOfItems)<>0) AND ((Transactions.TransRAPAssistedFlag)=True)) \
GROUP BY 0, Transactions.StationID, Store.Language; ")
#define ASSISTANCE_REPORT_ONE_DAY_SUMMARY_ASSISTS_BY_LANE _T("AssistanceReportOneDaySummaryAssistsByLane")

#define ASSISTANCE_REPORT_YESTERDAY_SUMMARY_ASSISTS_BY_LANE_QUERY _T("SELECT Count(Transactions.TransactionID) AS NumAssists, 0 AS NumTrans, Transactions.StationID, Store.Language \
FROM Transactions INNER JOIN Store ON Transactions.BusinessDate = Store.YesterdayBusinessDate \
WHERE (((Transactions.TransAssistedFlag)=True) AND ((Transactions.TotalNumberOfItems)<>0)) OR (((Transactions.TotalNumberOfItems)<>0) AND ((Transactions.TransRAPAssistedFlag)=True)) \
GROUP BY 0, Transactions.StationID, Store.Language; ")
#define ASSISTANCE_REPORT_YESTERDAY_SUMMARY_ASSISTS_BY_LANE _T("AssistanceReportYesterdaySummaryAssistsByLane")

#define ASSISTANCE_REPORT_ONE_DAY_SUMMARY_TRANS_BY_LANE_QUERY _T("SELECT 0 AS NumAssists, Count(Transactions.TransactionID) AS NumTrans, Transactions.StationID, Store.Language \
FROM Store INNER JOIN Transactions ON Store.BusinessDate = Transactions.BusinessDate \
WHERE (((Transactions.TotalNumberOfItems)<>0)) \
GROUP BY 0, Transactions.StationID, Store.Language; ")
#define ASSISTANCE_REPORT_ONE_DAY_SUMMARY_TRANS_BY_LANE _T("AssistanceReportOneDaySummaryTransByLane")

#define ASSISTANCE_REPORT_YESTERDAY_SUMMARY_TRANS_BY_LANE_QUERY _T("SELECT 0 AS NumAssists, Count(Transactions.TransactionID) AS NumTrans, Transactions.StationID, Store.Language \
FROM Transactions INNER JOIN Store ON Transactions.BusinessDate = Store.YesterdayBusinessDate \
WHERE (((Transactions.TotalNumberOfItems)<>0)) \
GROUP BY 0, Transactions.StationID, Store.Language; ")
#define ASSISTANCE_REPORT_YESTERDAY_SUMMARY_TRANS_BY_LANE _T("AssistanceReportYesterdaySummaryTransByLane")

#define ASSISTANCE_REPORT_ONE_DAY_SUMMARY_LANE_BY_LANE_QUERY _T("SELECT Interventions.StationID, Interventions.InterventionClass, 0 AS NumRAPInterventions, Count(Interventions.StationID & Interventions.TransactionID & Interventions.InterventionID) AS NumLaneInterventions, 0 AS NumPager, Store.Language \
FROM (Transactions INNER JOIN Interventions ON (Transactions.StationID = Interventions.StationID) AND (Transactions.TransactionID = Interventions.TransactionID)) INNER JOIN Store ON Transactions.BusinessDate = Store.BusinessDate \
WHERE (((Transactions.TransAssistedFlag)=True) AND ((Interventions.InTransaction)=True) AND ((Interventions.OperatorID)<>\"\" And (Interventions.OperatorID)<>\"0000\")) \
GROUP BY Interventions.StationID, Interventions.InterventionClass, Store.Language, Interventions.ClearStationID \
HAVING (((Interventions.ClearStationID)<>\"RAP\")); ")
#define ASSISTANCE_REPORT_ONE_DAY_SUMMARY_LANE_BY_LANE _T("AssistanceReportOneDaySummaryLaneByLane")

#define ASSISTANCE_REPORT_YESTERDAY_SUMMARY_LANE_BY_LANE_QUERY _T("SELECT Interventions.StationID, Interventions.InterventionClass, 0 AS NumRAPInterventions, Count(Interventions.StationID & Interventions.TransactionID & Interventions.InterventionID) AS NumLaneInterventions, 0 AS NumPager, Store.Language \
FROM (Transactions INNER JOIN Interventions ON (Transactions.TransactionID = Interventions.TransactionID) AND (Transactions.StationID = Interventions.StationID)) INNER JOIN Store ON Transactions.BusinessDate = Store.YesterdayBusinessDate \
WHERE (((Transactions.TransAssistedFlag)=True) AND ((Interventions.InTransaction)=True) AND ((Interventions.OperatorID)<>\"\" And (Interventions.OperatorID)<>\"0000\")) \
GROUP BY Interventions.StationID, Interventions.InterventionClass, Store.Language, Interventions.ClearStationID \
HAVING (((Interventions.ClearStationID)<>\"RAP\")); ")
#define ASSISTANCE_REPORT_YESTERDAY_SUMMARY_LANE_BY_LANE _T("AssistanceReportYesterdaySummaryLaneByLane")

#define ASSISTANCE_REPORT_ONE_DAY_BY_CASHIER_LANE_QUERY _T("SELECT Interventions.OperatorID, Interventions.InterventionClass, 0 AS RAPIntervention, Count(Interventions.InterventionID) AS LaneIntervention, 0 AS Pager, Store.Language, Avg(DateDiff(\"s\",Interventions.InterventionStartDateTime,Interventions.InterventionEndDateTime)) AS LaneResponseTime, 0 AS RAPResponseTime \
FROM (Transactions INNER JOIN Interventions ON (Transactions.StationID = Interventions.StationID) AND (Transactions.TransactionID = Interventions.TransactionID)) INNER JOIN Store ON Transactions.BusinessDate = Store.BusinessDate \
WHERE (((Interventions.ClearStationID)<>\"RAP\") AND ((Interventions.InTransaction)=True) AND ((Interventions.OperatorID)<>\"\" And (Interventions.OperatorID)<>\"0000\")) \
GROUP BY Interventions.OperatorID, Interventions.InterventionClass, 0, 0, Store.Language; ") 
#define ASSISTANCE_REPORT_ONE_DAY_BY_CASHIER_LANE _T("AssistanceReportOneDayByCashierLane")

#define ASSISTANCE_REPORT_YESTERDAY_BY_CASHIER_LANE_QUERY _T("SELECT Interventions.OperatorID, Interventions.InterventionClass, 0 AS RAPIntervention, Count(Interventions.InterventionID) AS LaneIntervention, 0 AS Pager, Store.Language, Avg(DateDiff(\"s\",Interventions.InterventionStartDateTime,Interventions.InterventionEndDateTime)) AS LaneResponseTime, 0 AS RAPResponseTime \
FROM Store INNER JOIN (Transactions INNER JOIN Interventions ON (Transactions.TransactionID = Interventions.TransactionID) AND (Transactions.StationID = Interventions.StationID)) ON Store.YesterdayBusinessDate = Transactions.BusinessDate \
WHERE (((Interventions.ClearStationID)<>\"RAP\") AND ((Interventions.InTransaction)=True) AND ((Interventions.OperatorID)<>\"\" And (Interventions.OperatorID)<>\"0000\")) \
GROUP BY Interventions.OperatorID, Interventions.InterventionClass, 0, 0, Store.Language; ")
#define ASSISTANCE_REPORT_YESTERDAY_BY_CASHIER_LANE _T("AssistanceReportYesterdayByCashierLane")

#define WEEKLY_ASSISTANCE_SUMMARY_BY_CASHIER_LANE_QUERY _T("SELECT Interventions.OperatorID, Interventions.InterventionClass, 0 AS RAPIntervention, 0 AS AvgRAPTime, Count(Interventions.InterventionID) AS LaneIntervention, Avg(Format(Interventions.InterventionEndDateTime-Interventions.InterventionStartDateTime,\"ss\")) AS AvgLaneTime, Store.Language \
FROM Store, Transactions INNER JOIN Interventions ON (Transactions.TransactionID = Interventions.TransactionID) AND (Transactions.StationID = Interventions.StationID) \
WHERE (((Transactions.BusinessDate)>=[Store].[SevenDayDate]) AND ((Interventions.ClearStationID)<>\"RAP\") AND ((Interventions.OperatorID)<>\"\" And (Interventions.OperatorID)<>\"0000\") AND ((Interventions.InTransaction)=True)) \
GROUP BY Interventions.OperatorID, Interventions.InterventionClass, Store.Language;")
#define WEEKLY_ASSISTANCE_SUMMARY_BY_CASHIER_LANE _T("WeeklyAssistanceSummaryByCashierLane")

#define WEEKLY_ASSISTANCE_SUMMARY_BY_CASHIER_RAP_QUERY _T("SELECT Interventions.OperatorID, Interventions.InterventionClass, Count(Interventions.InterventionID) AS RAPIntervention, Avg(Format(Interventions.InterventionEndDateTime-Interventions.InterventionStartDateTime,\"ss\")) AS AvgRAPTime, 0 AS LaneIntervention, 0 AS AvgLaneTime, Store.Language \
FROM Store, Transactions INNER JOIN Interventions ON (Transactions.TransactionID = Interventions.TransactionID) AND (Transactions.StationID = Interventions.StationID) \
WHERE (((Transactions.BusinessDate)>=[Store].[SevenDayDate]) AND ((Interventions.ClearStationID)=\"RAP\") AND ((Transactions.TransRAPAssistedFlag)=True) AND ((Interventions.InTransaction)=True)) \
GROUP BY Interventions.OperatorID, Interventions.InterventionClass, Store.Language;")
#define WEEKLY_ASSISTANCE_SUMMARY_BY_CASHIER_RAP _T("WeeklyAssistanceSummaryByCashierRAP")

#define WEEKLY_ASSISTANCE_SUMMARY_BY_CASHIER_BY_DATE_LANE_QUERY _T("SELECT Interventions.OperatorID, Transactions.BusinessDate, 0 AS RAPIntervention, 0 AS AvgRAPTime, Count(Interventions.InterventionID) AS LaneIntervention, Avg(Format(Interventions.InterventionEndDateTime-Interventions.InterventionStartDateTime,\"ss\")) AS AvgLaneTime, 0 AS PagerAlert, Store.Language \
FROM Store, Transactions INNER JOIN Interventions ON (Transactions.TransactionID = Interventions.TransactionID) AND (Transactions.StationID = Interventions.StationID) \
WHERE (((Transactions.BusinessDate)>=[Store].[SevenDayDate]) AND ((Interventions.ClearStationID)<>\"RAP\") AND ((Interventions.OperatorID)<>\"\" And (Interventions.OperatorID)<>\"0000\") AND ((Interventions.InTransaction)=True)) \
GROUP BY Interventions.OperatorID, Transactions.BusinessDate, Store.Language; ")
#define WEEKLY_ASSISTANCE_SUMMARY_BY_CASHIER_BY_DATE_LANE _T("WeeklyAssistanceSummaryByCashierByDateLane")

#define WEEKLY_ASSISTANCE_TOTALS_TRANS_QUERY _T("SELECT Count(Transactions.TransactionID) AS Trans, 0 AS Assist, 0 AS RAPIntervention, 0 AS PagerAlert, 0 AS AssistTrans, 0 AS RAPAssistTrans, Store.Language \
FROM Store, Transactions \
WHERE (((Transactions.BusinessDate)>=[Store].[SevenDayDate]) AND ((Transactions.TotalNumberOfItems)<>0)) OR (((Transactions.BusinessDate)>=[Store].[SevenDayDate]) AND ((Transactions.TransAssistedFlag)=True)) OR (((Transactions.BusinessDate)>=[Store].[SevenDayDate]) AND ((Transactions.TransRAPAssistedFlag)=True)) \
GROUP BY Store.Language; ") 
#define WEEKLY_ASSISTANCE_TOTALS_TRANS _T("WeeklyAssistanceTotalsTrans")

#define WEEKLY_ASSISTANCE_TOTALS_ASSISTS_QUERY _T("SELECT 0 AS Trans, Count(Transactions.TransactionID) AS Assist, 0 AS RAPIntervention, 0 AS PagerAlert, 0 AS AssistTrans, 0 AS RAPAssistTrans, Store.Language \
FROM Store, Transactions INNER JOIN Interventions ON (Transactions.TransactionID = Interventions.TransactionID) AND (Transactions.StationID = Interventions.StationID) \
WHERE (((Transactions.BusinessDate)>=[Store].[SevenDayDate]) AND ((Interventions.InTransaction)=True) AND ((Transactions.TotalNumberOfItems)<>0)) \
GROUP BY Store.Language; ") 
#define WEEKLY_ASSISTANCE_TOTALS_ASSISTS _T("WeeklyAssistanceTotalsAssists")

#define WEEKLY_ASSISTANCE_TOTALS_ASSISTED_TRANS_QUERY _T("SELECT 0 AS Trans, 0 AS Assist, 0 AS RAPIntervention, 0 AS PagerAlert, Count(WeeklyAssistanceTotalsAssistedTransCount.CountOfTransactionID) AS AssistTrans, 0 AS RAPAssistTrans, Store.Language \
FROM Store, WeeklyAssistanceTotalsAssistedTransCount \
GROUP BY Store.Language; ") 
#define WEEKLY_ASSISTANCE_TOTALS_ASSISTED_TRANS _T("WeeklyAssistanceTotalsAssistedTrans")

#define WEEKLY_ASSISTANCE_TOTALS_ASSISTED_TRANS_COUNT_QUERY _T("SELECT Count(Interventions.TransactionID) AS CountOfTransactionID, Store.Language \
FROM Store, Transactions INNER JOIN Interventions ON (Transactions.TransactionID = Interventions.TransactionID) AND (Transactions.StationID = Interventions.StationID) \
WHERE (((Transactions.BusinessDate)>=[Store].[SevenDayDate]) AND ((Interventions.InTransaction)=True) AND ((Interventions.OperatorID)<>\"\") AND ((Interventions.OperatorID)<>\"0000\")) \
GROUP BY Store.Language, Interventions.TransactionID;") 
#define WEEKLY_ASSISTANCE_TOTALS_ASSISTED_TRANS_COUNT _T("WeeklyAssistanceTotalsAssistedTransCount")

#define WEEKLY_ATTENDANT_TRANS_REPORT_LANE_QUERY _T("SELECT Transactions.StationID, Count(Transactions.TransactionID) AS NumTran, 0 AS Att_Trans, 0 AS Att_Entries, Store.Language \
FROM Store INNER JOIN Transactions ON Transactions.BusinessDate >=Store.SevenDayDate \
WHERE (((Transactions.TotalNumberOfItems)<>0)) \
GROUP BY Transactions.StationID, Store.Language \
UNION SELECT Transactions.StationID, 0 AS NumTrans, Count(Transactions.TransactionID) AS Att_Trans, 0 AS Att_Entries, Store.Language \
FROM Store INNER JOIN Transactions ON Transactions.BusinessDate >=Store.SevenDayDate \
WHERE (Transactions.TransAttendantFlag)=True AND (Transactions.TotalNumberOfItems)<>0 \
GROUP BY Transactions.StationID, Store.Language;")  
#define WEEKLY_ATTENDANT_TRANS_REPORT_LANE _T("WeeklyAttendantTransReportByLane")

#define WEEKLY_ATTENDANT_ENTRIES_REPORT_LANE_QUERY _T("SELECT ApplicationMode.StationID, 0 AS NumTrans, 0 AS Att_Trans, Count(ApplicationMode.TransactionID) AS Att_Entries, Store.Language \
FROM Store INNER JOIN ApplicationMode ON ApplicationMode.ModeChangeTime >=Store.SevenDayDate \
WHERE (ApplicationMode.ModeName)=\"ATTENDANT\" And (ApplicationMode.ModeChangeType)=\"ENTER\" \
GROUP BY ApplicationMode.StationID, Store.Language;")  
#define WEEKLY_ATTENDANT_ENTRIES_REPORT_LANE _T("WeeklyAttendantEntriesReportByLane")

#define WEEKLY_ATTENDANT_MODE_REPORT_LANE_QUERY _T("SELECT * FROM WeeklyAttendantTransReportByLane UNION SELECT * FROM WeeklyAttendantEntriesReportByLane; ") 
#define WEEKLY_ATTENDANT_MODE_REPORT_LANE _T("WeeklyAttendantModeReportByLane")

#define ASSISTANCE_REPORT_WEEK_SUMMARY_ASSIST_QUERY _T("SELECT Interventions.InterventionClass, Count(Interventions.InterventionID) AS TotalAssist, 0 AS RAPIntervention, 0 AS LaneIntervention, 0 AS AssistTran, 0 AS AvgLaneTime, 0 AS AvgRAPTime, Store.Language \
FROM Store, Transactions INNER JOIN Interventions ON (Transactions.TransactionID = Interventions.TransactionID) AND (Transactions.StationID = Interventions.StationID) \
WHERE (((Transactions.BusinessDate)>=[Store].[SevenDayDate]) AND ((Interventions.InTransaction)=True) AND ((Interventions.OperatorID)<>\"\") AND ((Interventions.OperatorID)<>\"0000\")) \
GROUP BY Interventions.InterventionClass, Store.Language; ") 
#define ASSISTANCE_REPORT_WEEK_SUMMARY_ASSIST _T("AssistanceReportWeekSummaryAssist")

#define ASSISTANCE_REPORT_WEEK_SUMMARY_LANE_QUERY _T("SELECT Interventions.InterventionClass, 0 AS TotalAssist, 0 AS RAPIntervention, Count(Interventions.InterventionID) AS LaneIntervention, 0 AS AssistTran, 0 AS AvgLaneTime, 0 AS AvgRAPTime, Store.Language \
FROM Store, Transactions INNER JOIN Interventions ON (Transactions.TransactionID = Interventions.TransactionID) AND (Transactions.StationID = Interventions.StationID) \
WHERE (((Transactions.BusinessDate)>=[Store].[SevenDayDate]) AND ((Interventions.ClearStationID)<>\"RAP\") AND ((Transactions.TransAssistedFlag)=True) AND ((Interventions.InTransaction)=True) AND ((Interventions.OperatorID)<>\"\") AND ((Interventions.OperatorID)<>\"0000\")) \
GROUP BY Interventions.InterventionClass, Store.Language; ") 
#define ASSISTANCE_REPORT_WEEK_SUMMARY_LANE _T("AssistanceReportWeekSummaryLane")

#define ASSIST_REPORT_WEEK_SUMMARY_ASSIST_TRANS_QUERY _T("SELECT Interventions.InterventionClass, Count(Interventions.TransactionID) AS CountOfTransactionID, Store.Language \
FROM Store, Transactions INNER JOIN Interventions ON (Transactions.StationID = Interventions.StationID) AND (Transactions.TransactionID = Interventions.TransactionID) \
WHERE (((Transactions.BusinessDate)>=[Store].[SevenDayDate]) AND ((Interventions.InTransaction)=True) AND ((Interventions.OperatorID)<>\"\") AND ((Interventions.OperatorID)<>\"0000\")) \
GROUP BY Interventions.InterventionClass, Store.Language, Interventions.TransactionID; ") 
#define ASSIST_REPORT_WEEK_SUMMARY_ASSIST_TRANS _T("AssistReportWeekSummaryAssistTrans")

#define ASSIST_REPORT_WEEK_AVG_LANE_QUERY _T("SELECT Interventions.InterventionClass, 0 AS TotalAssist, 0 AS RAPIntervention, 0 AS LaneIntervention, 0 AS AssistTran, Avg(DateDiff(\"s\",Interventions.InterventionStartDateTime,Interventions.InterventionEndDateTime)) AS AvgLaneTime, 0 AS AvgRAPTime, Store.Language \
FROM Store, Transactions INNER JOIN Interventions ON (Transactions.StationID = Interventions.StationID) AND (Transactions.TransactionID = Interventions.TransactionID) \
WHERE (((Transactions.BusinessDate)>=[Store].[SevenDayDate]) AND ((Interventions.InTransaction)=True) AND ((Interventions.ClearStationID)<>\"RAP\") AND ((Interventions.OperatorID)<>\"\") AND ((Interventions.OperatorID)<>\"0000\")) \
GROUP BY Interventions.InterventionClass, Store.Language; ") 
#define ASSIST_REPORT_WEEK_AVG_LANE _T("AssistReportWeekSummaryAvgLane")

#define YESTERDAYS_RAP_TILL_QUERY _T("SELECT * FROM YesterdaysRAPTender UNION SELECT * FROM YesterdaysRAPChange;") 
#define YESTERDAYS_RAP_TILL _T("YesterdaysRAPTill")

#define YESTERDAYS_RAP_CHANGE_QUERY _T("SELECT Store.BusinessDate, RAPChangeDispensed.TenderOperatorID, RAPChangeDispensed.TenderType, RAPChangeDispensed.StationID, Count(RAPChangeDispensed.ChangeAmount) AS NoRapTrans, Sum(RAPChangeDispensed.ChangeAmount) AS RAPAmt, Store.Language \
FROM (RAPChangeDispensed INNER JOIN Transactions ON (RAPChangeDispensed.TransactionID = Transactions.TransactionID) AND (RAPChangeDispensed.StationID = Transactions.StationID)) INNER JOIN Store ON Transactions.BusinessDate = Store.YesterdayBusinessDate \
GROUP BY Store.BusinessDate, RAPChangeDispensed.TenderOperatorID, RAPChangeDispensed.TenderType, RAPChangeDispensed.StationID, 0, Store.Language;") 
#define YESTERDAYS_RAP_CHANGE _T("YesterdaysRAPChange")

#define YESTERDAYS_RAP_TENDER_QUERY _T("SELECT Store.BusinessDate, Tenders.TenderOperatorID, Tenders.TenderType, Tenders.StationID, Count(Tenders.TenderID) AS NoRapTrans, Sum(Tenders.TenderTotal) AS RAPAmt, Store.Language \
FROM Store INNER JOIN (Transactions INNER JOIN Tenders ON (Transactions.StationID = Tenders.StationID) AND (Transactions.TransactionID = Tenders.TransactionID)) ON Store.YesterdayBusinessDate = Transactions.BusinessDate \
WHERE (((Tenders.TenderReceiveStationID)=\"RAP\")) \
GROUP BY Store.BusinessDate, Tenders.TenderOperatorID, Tenders.TenderType, Tenders.StationID, Store.Language;")
#define YESTERDAYS_RAP_TENDER _T("YesterdaysRAPTender")

#define YESTERDAYS_RAP_TILL_REPORT_QUERY _T("SELECT YesterdaysRAPTill.BusinessDate, YesterdaysRAPTill.TenderOperatorID, YesterdaysRAPTill.TenderType, YesterdaysRAPTill.StationID, Sum(YesterdaysRAPTill.NoRapTrans) AS SumOfNoRapTrans, Sum(YesterdaysRAPTill.RAPAmt) AS SumOfRAPAmt, YesterdaysRAPTill.Language \
FROM YesterdaysRAPTill \
GROUP BY YesterdaysRAPTill.BusinessDate, YesterdaysRAPTill.TenderOperatorID, YesterdaysRAPTill.TenderType, YesterdaysRAPTill.StationID, YesterdaysRAPTill.Language;")
#define YESTERDAYS_RAP_TILL_REPORT _T("YesterdaysRAPTillReport")

#define TODAYS_RAP_TILL_QUERY _T("SELECT * FROM TodaysRAPTender UNION SELECT * FROM TodaysRAPChange;") 
#define TODAYS_RAP_TILL _T("TodaysRAPTill")

#define TODAYS_RAP_CHANGE_QUERY _T("SELECT Store.BusinessDate, RAPChangeDispensed.TenderOperatorID, RAPChangeDispensed.TenderType, RAPChangeDispensed.StationID, Count(RAPChangeDispensed.ChangeAmount) AS NoRapTrans, Sum(RAPChangeDispensed.ChangeAmount) AS RAPAmt, Store.Language \
FROM (RAPChangeDispensed INNER JOIN Transactions ON (RAPChangeDispensed.TransactionID = Transactions.TransactionID) AND (RAPChangeDispensed.StationID = Transactions.StationID)) INNER JOIN Store ON Transactions.BusinessDate = Store.BusinessDate \
GROUP BY Store.BusinessDate, RAPChangeDispensed.TenderOperatorID, RAPChangeDispensed.TenderType, RAPChangeDispensed.StationID, 0, Store.Language;") 
#define TODAYS_RAP_CHANGE _T("TodaysRAPChange")

#define TODAYS_RAP_TENDER_QUERY _T("SELECT Store.BusinessDate, Tenders.TenderOperatorID, Tenders.TenderType, Tenders.StationID, Count(Tenders.TenderID) AS NoRapTrans, Sum(Tenders.TenderTotal) AS RAPAmt, Store.Language \
FROM Store INNER JOIN (Transactions INNER JOIN Tenders ON (Transactions.StationID = Tenders.StationID) AND (Transactions.TransactionID = Tenders.TransactionID)) ON Store.BusinessDate = Transactions.BusinessDate \
WHERE (((Tenders.TenderReceiveStationID)=\"RAP\")) \
GROUP BY Store.BusinessDate, Tenders.TenderOperatorID, Tenders.TenderType, Tenders.StationID, Store.Language;")
#define TODAYS_RAP_TENDER _T("TodaysRAPTender")

#define TODAYS_RAP_TILL_REPORT_QUERY _T("SELECT TodaysRAPTill.BusinessDate, TodaysRAPTill.TenderOperatorID, TodaysRAPTill.TenderType, TodaysRAPTill.StationID, Sum(TodaysRAPTill.NoRapTrans) AS SumOfNoRapTrans, Sum(TodaysRAPTill.RAPAmt) AS SumOfRAPAmt, TodaysRAPTill.Language \
FROM TodaysRAPTill \
GROUP BY TodaysRAPTill.BusinessDate, TodaysRAPTill.TenderOperatorID, TodaysRAPTill.TenderType, TodaysRAPTill.StationID, TodaysRAPTill.Language;")
#define TODAYS_RAP_TILL_REPORT _T("TodaysRAPTillReport")

// TAR 286056 start
#define ASSISTANCE_REPORT_WEEK_SUMMARY_RAP_QUERY _T("SELECT Interventions.InterventionClass, 0 AS TotalAssist, Count(Interventions.InterventionID) AS RAPIntervention, 0 AS LaneIntervention, 0 AS AssistTran, 0 AS AvgLaneTime, 0 AS AvgRAPTime, Store.Language \
FROM Store, Transactions INNER JOIN Interventions ON (Transactions.TransactionID = Interventions.TransactionID) AND (Transactions.StationID = Interventions.StationID) \
WHERE (((Transactions.BusinessDate)>=[Store].[SevenDayDate]) AND ((Interventions.ClearStationID)=\"RAP\") AND ((Transactions.TransRAPAssistedFlag)=True) AND ((Interventions.InTransaction)=True) AND ((Interventions.OperatorID)<>\"\") AND ((Interventions.OperatorID)<>\"0000\")) \
GROUP BY Interventions.InterventionClass, Store.Language; ")
#define ASSISTANCE_REPORT_WEEK_SUMMARY_RAP _T("AssistanceReportWeekSummaryRAP")

#define WEEKLY_ASSISTANCE_TOTALS_RAP_QUERY _T("SELECT 0 AS Trans, 0 AS Assist, Count(Interventions.InterventionID) AS RAPIntervention, 0 AS PagerAlert, 0 AS AssistTrans, 0 AS RAPAssistTrans, Store.Language \
FROM Store, Transactions INNER JOIN Interventions ON (Transactions.StationID = Interventions.StationID) AND (Transactions.TransactionID = Interventions.TransactionID) \
WHERE (((Transactions.BusinessDate)>=[Store].[SevenDayDate]) AND ((Interventions.ClearStationID)=\"RAP\") AND ((Transactions.TransRAPAssistedFlag)=True) AND ((Interventions.InTransaction)=True) AND ((Interventions.OperatorID)<>\"\") AND ((Interventions.OperatorID)<>\"0000\")) \
GROUP BY Store.Language; ")
#define WEEKLY_ASSISTANCE_TOTALS_RAP _T("WeeklyAssistanceTotalsRAP")

#define ASSISTANCE_REPORT_ONE_DAY_SUMMARY_PAGER_BY_LANE_QUERY _T("SELECT Interventions.StationID, Interventions.InterventionClass, 0 AS NumRAPInterventions, 0 AS NumLaneInterventions, Count(Interventions.PagerEvent) AS CountOfPagerEvent, Store.Language \
FROM (Transactions INNER JOIN Interventions ON (Transactions.TransactionID = Interventions.TransactionID) AND (Transactions.StationID = Interventions.StationID)) INNER JOIN Store ON Transactions.BusinessDate = Store.BusinessDate \
WHERE (((Interventions.PagerEvent)=True) AND ((Interventions.InTransaction)=True)) \
GROUP BY Interventions.StationID, Interventions.InterventionClass, Store.Language; ")
#define ASSISTANCE_REPORT_ONE_DAY_SUMMARY_PAGER_BY_LANE _T("AssistanceReportOneDaySummaryPagerByLane")

#define ASSISTANCE_REPORT_YESTERDAY_SUMMARY_PAGER_BY_LANE_QUERY _T("SELECT Interventions.StationID, Interventions.InterventionClass, 0 AS NumRAPInterventions, 0 AS NumLaneInterventions, Count(Interventions.PagerEvent) AS CountOfPagerEvent, Store.Language \
FROM (Transactions INNER JOIN Interventions ON (Transactions.TransactionID = Interventions.TransactionID) AND (Transactions.StationID = Interventions.StationID)) INNER JOIN Store ON Transactions.BusinessDate = Store.YesterdayBusinessDate \
WHERE (((Interventions.PagerEvent)=True) AND ((Interventions.InTransaction)=True)) \
GROUP BY Interventions.StationID, Interventions.InterventionClass, Store.Language; ")
#define ASSISTANCE_REPORT_YESTERDAY_SUMMARY_PAGER_BY_LANE _T("AssistanceReportYesterdaySummaryPagerByLane")
// TAR 286056 end

#define SIGNON_ENTRIES_QUERY _T("SELECT OperatorLogins.ActivityTime, OperatorLogins.StationID, OperatorLogins.OperatorID, OperatorLogins.SignOn, OperatorLogins.Application, Store.Language \
FROM OperatorLogins INNER JOIN Store ON OperatorLogins.ActivityTime >= Store.SevenDayDate \
GROUP BY OperatorLogins.ActivityTime, OperatorLogins.StationID, OperatorLogins.OperatorID, OperatorLogins.SignOn, OperatorLogins.Application, Store.Language;")
#define SIGNON_ENTRIES _T("SignOnEntries")
/////////////////////////////////////////////////////////////////////////////
// The one and only application object

CWinApp theApp;
_ConnectionPtr g_pConnection;
_RecordsetPtr g_pRecordset;
DAO::DatabasePtr g_pDAODatabase;
DAO::WorkspacePtr g_pDAOWorkSpace;
DAO::_DBEnginePtr g_pDAOEngine;

int g_nRetCode = 0;

using namespace std;
bool UpdateSignatureTable()
{
    bool bRetCode = false;
    cout << _T("    Updating Signature Table") << endl;
    try{
            _bstr_t strSQLCreateReceiptType(_T("ALTER TABLE Signatures ADD COLUMN ReceiptType TEXT(50) DEFAULT SIGNATURE"));
            g_pConnection->Execute(strSQLCreateReceiptType, NULL, adExecuteNoRecords);
            _bstr_t strSQLSetSignature(_T("UPDATE Signatures SET ReceiptType = 'SIGNATURE'"));
            g_pConnection->Execute(strSQLSetSignature, NULL, adExecuteNoRecords);

            cout << _T("    Updated Signature Table Successfully") << endl;
            bRetCode = true;
    }
    catch (...)
    {
        bRetCode = false;                   
    }
    return bRetCode;
 

}
bool UpdateTransactionsTable()
{
    bool bRetCode = false;
    cout << _T("    Updating Transactions Table") << endl;
    try{
            _bstr_t strSQLCreateAttendantFlag(_T("ALTER TABLE Transactions ADD COLUMN TransAttendantFlag YESNO"));
            g_pConnection->Execute(strSQLCreateAttendantFlag, NULL, adExecuteNoRecords);

            cout << _T("    Updated Transactions Table Successfully") << endl;
            bRetCode = true;
    }
    catch (...)
    {
        bRetCode = false;                   
    }
    return bRetCode;
 

}
bool UpdateTransactionsTable2()
{
    bool bRetCode = false;
    cout << _T("    Updating Transactions Table") << endl;
    try{
           _bstr_t strSQLCreateRAPTransaction(_T("ALTER TABLE Transactions ADD COLUMN TransRAPTransaction YESNO"));
           g_pConnection->Execute(strSQLCreateRAPTransaction, NULL, adExecuteNoRecords);

		    _bstr_t strSQLCreateLanguageColumn(_T("ALTER TABLE Transactions ADD COLUMN TransLanguage TEXT(50)"));
            g_pConnection->Execute(strSQLCreateLanguageColumn, NULL, adExecuteNoRecords);
            cout << _T("    Updated Transactions Table Successfully") << endl;
            bRetCode = true;
    }
    catch (...)
    {
        bRetCode = false;                   
    }
    return bRetCode;
 

}
// This routine updates the report text fields.  It assumes all the columns 
// referenced have already been built.
bool UpdateReportTextFromFile()
{
    long rc;
    long rc2;
    CString csLanguage;
    CString csColumn;
    CString csValue;
    CString csResult;
    bool brc = true;
    FLReportTextFile *pRTFile;
    _bstr_t strSQLUpdateTable;
    CString csSQLUpdateTable;

	// 5-partition mods.
	TCHAR szAppDrive[_MAX_DRIVE+1];
	DWORD dwRet;
	CString csReportTextConfig;

	dwRet = ExpandEnvironmentStrings(_T("%APP_DRIVE%"), szAppDrive, _MAX_DRIVE+1);
	if(dwRet == 0 || dwRet > _MAX_DRIVE+1)
	{
		_tcsncpy(szAppDrive, _T("C:"), _MAX_DRIVE+1);	// Start with sane values.
	}
	// 5-partition mods.


    cout << _T("    Updating Report Text Table") << endl;
    try
    {
        for (int i=0; i < 2; i++)
        {
            pRTFile = new FLReportTextFile;
            if (i == 0)
            {
				csReportTextConfig.Format(_T("%s\\scot\\config\\ReportTextConfig.xml"),
						(LPCTSTR)szAppDrive);
                rc = pRTFile->Open(csReportTextConfig);
            }
            else
            {
				csReportTextConfig.Format(_T("%s\\scot\\config\\ReportTextConfig.000"),
						(LPCTSTR)szAppDrive);
                rc = pRTFile->Open(csReportTextConfig);
            }
            if (rc == 0)
            {
                rc = 1;
                while (rc == 1)
                {
                    rc2=1;
                    while (rc2 == 1)
                    {
                        rc2 = pRTFile->GetNextEntry(csLanguage, csColumn, csValue);
                        if (rc2 == 1)
                        {
                            csSQLUpdateTable.Format(_T("UPDATE ReportText SET ReportText.[%s] = \"%s\" WHERE Language = \"%s\""),
                                (LPCTSTR) csColumn, (LPCTSTR) csValue, (LPCTSTR) csLanguage);
                            strSQLUpdateTable = (LPCTSTR)csSQLUpdateTable;
                            g_pConnection->Execute(strSQLUpdateTable, NULL, adExecuteNoRecords);                
                        }
                    }
                    rc = pRTFile->NextLanguage();
                }
            }
            delete pRTFile;
        }
    }
    catch (...)
    {
        cerr << _T("ERROR Exception caught while updating report text") << endl;
        brc = false;                   
    }
    return brc;
}
bool CreateApplicationModeTable()
{
	_RecordsetPtr pApplicationModeRecordset;
	HRESULT hr;
	_bstr_t bstrTableName (_T("ApplicationMode"));
	COleVariant vOrd, vVal;
	COleSafeArray saField, saVal;
	FieldPtr pField;
	_variant_t vtParm;
    bool bRetCode = false;
	_bstr_t strSQLUpdateTable;
    try
    {
		hr = pApplicationModeRecordset.CreateInstance(__uuidof(Recordset));
        if (FAILED(hr))
		{

		}
        else
        {
            try
            {
		        hr = pApplicationModeRecordset->Open(bstrTableName, _variant_t((IDispatch *)g_pConnection, true),
			        adOpenKeyset, adLockOptimistic, adCmdTable);

            }
            catch (_com_error e)
            {
                // Table doesn't exist go create it
                // Create the Version table with one field
                cout << _T("    Creating ApplicationMode Table") << endl;
                _bstr_t strSQLCreateTable(_T("CREATE TABLE ApplicationMode \
                    (TableKey AUTOINCREMENT (1, 1) CONSTRAINT PrimaryKey PRIMARY KEY, \
                     StationID TEXT(50), TransactionID TEXT(50), \
                     ModeName TEXT(50), ModeChangeType TEXT(50), \
					 ModeChangeTime DATETIME, OperatorID TEXT(50), \
					 InTransaction YESNO)"));
                g_pConnection->Execute(strSQLCreateTable, NULL, adExecuteNoRecords);
                _bstr_t strSQLCreateIndex(_T("CREATE INDEX TableKey ON ApplicationMode \
                    (TableKey ASC)"));
                g_pConnection->Execute(strSQLCreateIndex, NULL, adExecuteNoRecords);
                // Create new column in ReportText table
				
				strSQLUpdateTable = (_T("ALTER TABLE ReportText \
                    ADD COLUMN WeeklyAttendantEntry text(100)"));
                g_pConnection->Execute(strSQLUpdateTable, NULL, adExecuteNoRecords);
				strSQLUpdateTable = (_T("ALTER TABLE ReportText \
                    ADD COLUMN AttendantTrans text(100)"));
                g_pConnection->Execute(strSQLUpdateTable, NULL, adExecuteNoRecords);
				strSQLUpdateTable = (_T("ALTER TABLE ReportText \
                    ADD COLUMN AttendantEntries text(100)"));
                g_pConnection->Execute(strSQLUpdateTable, NULL, adExecuteNoRecords);
 
                cout << _T("    Created ApplicationMode Table Successfully") << endl;
            }
            bRetCode = true;

        }
    }
    catch (...)
    {
        bRetCode = false;                   
    }
    return bRetCode;
}
bool CreateDeviceErrorTable()
{
	_RecordsetPtr pDeviceErrorRecordset;
	HRESULT hr;
	_bstr_t bstrTableName (_T("DeviceErrors"));
	COleVariant vOrd, vVal;
	COleSafeArray saField, saVal;
	FieldPtr pField;
	_variant_t vtParm;
    bool bRetCode = false;
    try
    {
		hr = pDeviceErrorRecordset.CreateInstance(__uuidof(Recordset));
        if (FAILED(hr))
		{

		}
        else
        {
            try
            {
		        hr = pDeviceErrorRecordset->Open(bstrTableName, _variant_t((IDispatch *)g_pConnection, true),
			        adOpenKeyset, adLockOptimistic, adCmdTable);

            }
            catch (_com_error e)
            {
                // Table doesn't exist go create it
                // Create the Version table with one field
                cout << _T("    Creating DeviceErrors Table") << endl;
                _bstr_t strSQLCreateTable(_T("CREATE TABLE DeviceErrors \
                    (TableKey AUTOINCREMENT (1, 1) CONSTRAINT PrimaryKey PRIMARY KEY, \
                     StationID TEXT(50), TransactionID TEXT(50), \
                     ErrorID LONG, Device TEXT(50), \
                     ErrorType TEXT(50), TimeOfError DATETIME)"));
                g_pConnection->Execute(strSQLCreateTable, NULL, adExecuteNoRecords);
                _bstr_t strSQLCreateIndex(_T("CREATE INDEX TableKey ON DeviceErrors \
                    (TableKey ASC)"));
                g_pConnection->Execute(strSQLCreateIndex, NULL, adExecuteNoRecords);
                // Create new column in table
                _bstr_t strSQLUpdateTable = (_T("ALTER TABLE ReportText \
                    ADD COLUMN SevenDayDeviceErrors text(100)"));
                g_pConnection->Execute(strSQLUpdateTable, NULL, adExecuteNoRecords);
 
                cout << _T("    Created DeviceErrors Table Successfully") << endl;
            }
            bRetCode = true;

        }
    }
    catch (...)
    {
        bRetCode = false;                   
    }
    return bRetCode;
}
bool CreateRAPChangeDispensedTable()
{
	_RecordsetPtr pRAPChangeRecordset;
	HRESULT hr;
	_bstr_t bstrTableName (_T("RAPChangeDispensed"));
	COleVariant vOrd, vVal;
	COleSafeArray saField, saVal;
	FieldPtr pField;
	_variant_t vtParm;
    bool bRetCode = false;
    try
    {
		hr = pRAPChangeRecordset.CreateInstance(__uuidof(Recordset));
        if (FAILED(hr))
		{

		}
        else
        {
            try
            {
		        hr = pRAPChangeRecordset->Open(bstrTableName, _variant_t((IDispatch *)g_pConnection, true),
			        adOpenKeyset, adLockOptimistic, adCmdTable);

            }
            catch (_com_error e)
            {
                // Table doesn't exist go create it
                // Create the Version table with one field
                cout << _T("    Creating RAPChangeDispensed Table") << endl;
                _bstr_t strSQLCreateTable(_T("CREATE TABLE RAPChangeDispensed \
                    (TableKey AUTOINCREMENT (1, 1) CONSTRAINT PrimaryKey PRIMARY KEY, \
                     StationID TEXT(50), TransactionID TEXT(50), \
                     ChangeAmount CURRENCY, TimeReturned DATETIME, \
                     TenderOperatorID TEXT(50), TenderType TEXT(50))"));
                g_pConnection->Execute(strSQLCreateTable, NULL, adExecuteNoRecords);
                _bstr_t strSQLCreateIndex(_T("CREATE INDEX TableKey ON RAPChangeDispensed \
                    (TableKey ASC)"));
                g_pConnection->Execute(strSQLCreateIndex, NULL, adExecuteNoRecords);
                cout << _T("    Created RAPChangeDispensed Table Successfully") << endl;
            }
            bRetCode = true;

        }
    }
    catch (...)
    {
        bRetCode = false;                   
    }
    return bRetCode;
}

bool UpdateCashManagementTable()
{
  _RecordsetPtr pCashManagementRecordset;
  HRESULT hr;
  bool bRetCode = false;
  try
  {
    hr = pCashManagementRecordset.CreateInstance(__uuidof(Recordset));
    if (FAILED(hr))
    {

    }
    else
    {
      try
      {
        cout << _T("    Updating CurrencyCashManagement Table CashierID field type to text") << endl;


        //set CashierID type to text
        //if type is already text, then executing this query is still ok
        _bstr_t strSQLUpdateTable(_T("ALTER TABLE CurrencyCashManagement ALTER CashierID Text(50)"));
        g_pConnection->Execute(strSQLUpdateTable, NULL, adExecuteNoRecords);      
        bRetCode = true;
      }
      catch (_com_error e)
      {
        cout << _T("    Failed to modify type of CashierID to text") << endl;
      }
    }
  }
  catch (...)
  {
      bRetCode = false;                   
  }

  return bRetCode;
}

//+pw500001
bool CreateCashManagementTable()
{
	_RecordsetPtr pCashManagementRecordset;
	HRESULT hr;
	_bstr_t bstrTableName (_T("CurrencyCashManagement"));
	COleVariant vOrd, vVal;
	COleSafeArray saField, saVal;
	FieldPtr pField;
	_variant_t vtParm;
    bool bRetCode = false;
    try
    {
		hr = pCashManagementRecordset.CreateInstance(__uuidof(Recordset));
        if (FAILED(hr))
		{

		}
        else
        {
            try
            {
		        hr = pCashManagementRecordset->Open(bstrTableName, _variant_t((IDispatch *)g_pConnection, true),
			        adOpenKeyset, adLockOptimistic, adCmdTable);

            }
            catch (_com_error e)
            {
                cout << _T("    Creating CurrencyCashManagement Table") << endl;

                _bstr_t strSQLCreateTable(_T("CREATE TABLE CurrencyCashManagement \
                    (TableKey AUTOINCREMENT (1, 1) CONSTRAINT PrimaryKey PRIMARY KEY, \
                     StationID TEXT(50), TransactionID TEXT(50), \
                     CashManagementID LONG, \
                     Denomination TEXT(50), ChangeInCount LONG, \
                     CashManagementTime TIME, UserID LONG, CashierID TEXT(50), DeviceType LONG, \
                     BalanceInCount LONG)"));


                g_pConnection->Execute(strSQLCreateTable, NULL, adExecuteNoRecords);
                _bstr_t strSQLCreateIndex(_T("CREATE INDEX TableKey ON CurrencyCashManagement \
                    (TableKey ASC)"));
                g_pConnection->Execute(strSQLCreateIndex, NULL, adExecuteNoRecords);
                
                //PW500001 - Add to report text
                // Create new column in table
                _bstr_t strSQLUpdateTable = (_T("ALTER TABLE ReportText \
                    ADD COLUMN CurrencyCashManagementSummary TEXT(100)"));
                g_pConnection->Execute(strSQLUpdateTable, NULL, adExecuteNoRecords);

                //Cash Management Details Report
                strSQLUpdateTable = (_T("ALTER TABLE ReportText \
                    ADD COLUMN CurrencyCashManagementDetails TEXT(100)"));
                g_pConnection->Execute(strSQLUpdateTable, NULL, adExecuteNoRecords);

                //Lock Box word
                strSQLUpdateTable = (_T("ALTER TABLE ReportText \
                    ADD COLUMN CurrencyLockBox TEXT(100)"));
                g_pConnection->Execute(strSQLUpdateTable, NULL, adExecuteNoRecords);

                //Bins word
                strSQLUpdateTable = (_T("ALTER TABLE ReportText \
                    ADD COLUMN Bins TEXT(100)"));
                g_pConnection->Execute(strSQLUpdateTable, NULL, adExecuteNoRecords);

                //ChangeInCount word
                strSQLUpdateTable = (_T("ALTER TABLE ReportText \
                    ADD COLUMN ChangeInCount TEXT(100)"));
                g_pConnection->Execute(strSQLUpdateTable, NULL, adExecuteNoRecords);

                //ChangeInValue word
                strSQLUpdateTable = (_T("ALTER TABLE ReportText \
                    ADD COLUMN ChangeInValue TEXT(100)"));
                g_pConnection->Execute(strSQLUpdateTable, NULL, adExecuteNoRecords);
 
                //Terminal Number word
                strSQLUpdateTable = (_T("ALTER TABLE ReportText \
                    ADD COLUMN TerminalNumber TEXT(100)"));
                g_pConnection->Execute(strSQLUpdateTable, NULL, adExecuteNoRecords);

                //UserID word
                strSQLUpdateTable = (_T("ALTER TABLE ReportText \
                    ADD COLUMN UserID TEXT(100)"));
                g_pConnection->Execute(strSQLUpdateTable, NULL, adExecuteNoRecords);

                //CashierID Number word
                strSQLUpdateTable = (_T("ALTER TABLE ReportText \
                    ADD COLUMN CashierID TEXT(100)"));
                g_pConnection->Execute(strSQLUpdateTable, NULL, adExecuteNoRecords);

                //BalanceInCount word
                strSQLUpdateTable = (_T("ALTER TABLE ReportText \
                    ADD COLUMN BalanceInCount TEXT(100)"));
                g_pConnection->Execute(strSQLUpdateTable, NULL, adExecuteNoRecords);

                //BalanceInValue word
                strSQLUpdateTable = (_T("ALTER TABLE ReportText \
                    ADD COLUMN BalanceInValue TEXT(100)"));
                g_pConnection->Execute(strSQLUpdateTable, NULL, adExecuteNoRecords);

                //BillAcceptor word
                strSQLUpdateTable = (_T("ALTER TABLE ReportText \
                    ADD COLUMN BillAcceptor TEXT(100)"));
                g_pConnection->Execute(strSQLUpdateTable, NULL, adExecuteNoRecords);

                //CoinAcceptor word
                strSQLUpdateTable = (_T("ALTER TABLE ReportText \
                    ADD COLUMN CoinAcceptor TEXT(100)"));
                g_pConnection->Execute(strSQLUpdateTable, NULL, adExecuteNoRecords);

                //BillDispenser word
                strSQLUpdateTable = (_T("ALTER TABLE ReportText \
                    ADD COLUMN BillDispenser TEXT(100)"));
                g_pConnection->Execute(strSQLUpdateTable, NULL, adExecuteNoRecords);

                //CoinDispenser word
                strSQLUpdateTable = (_T("ALTER TABLE ReportText \
                    ADD COLUMN CoinDispenser TEXT(100)"));
                g_pConnection->Execute(strSQLUpdateTable, NULL, adExecuteNoRecords);

                //TotalCount word
                strSQLUpdateTable = (_T("ALTER TABLE ReportText \
                    ADD COLUMN TotalCount TEXT(100)"));
                g_pConnection->Execute(strSQLUpdateTable, NULL, adExecuteNoRecords);

                //TotalValue word
                strSQLUpdateTable = (_T("ALTER TABLE ReportText \
                    ADD COLUMN TotalValue TEXT(100)"));
                g_pConnection->Execute(strSQLUpdateTable, NULL, adExecuteNoRecords);

                //BOD word
                strSQLUpdateTable = (_T("ALTER TABLE ReportText \
                    ADD COLUMN BOD TEXT(100)"));
                g_pConnection->Execute(strSQLUpdateTable, NULL, adExecuteNoRecords);

                /*Accepted word
                strSQLUpdateTable = (_T("ALTER TABLE ReportText \
                    ADD COLUMN BAccepted TEXT(100)"));
                g_pConnection->Execute(strSQLUpdateTable, NULL, adExecuteNoRecords);
                */
                //Pickup word
                strSQLUpdateTable = (_T("ALTER TABLE ReportText \
                    ADD COLUMN Pickup TEXT(100)"));
                g_pConnection->Execute(strSQLUpdateTable, NULL, adExecuteNoRecords);

                //EOD word
                strSQLUpdateTable = (_T("ALTER TABLE ReportText \
                    ADD COLUMN EOD TEXT(100)"));
                g_pConnection->Execute(strSQLUpdateTable, NULL, adExecuteNoRecords);

                /*Hour word
                strSQLUpdateTable = (_T("ALTER TABLE ReportText \
                    ADD COLUMN Hour TEXT(100)"));
                g_pConnection->Execute(strSQLUpdateTable, NULL, adExecuteNoRecords);
                */
                //Den word
                strSQLUpdateTable = (_T("ALTER TABLE ReportText \
                    ADD COLUMN Den TEXT(100)"));
                g_pConnection->Execute(strSQLUpdateTable, NULL, adExecuteNoRecords);

                //NumberDispended word
                strSQLUpdateTable = (_T("ALTER TABLE ReportText \
                    ADD COLUMN NumberDispensed TEXT(100)"));
                g_pConnection->Execute(strSQLUpdateTable, NULL, adExecuteNoRecords);

                //NumberAccepted word
                strSQLUpdateTable = (_T("ALTER TABLE ReportText \
                    ADD COLUMN NumberAccepted TEXT(100)"));
                g_pConnection->Execute(strSQLUpdateTable, NULL, adExecuteNoRecords);

                cout << _T("    Created CurrencyCashManagement Table Successfully") << endl;

           
            }
            bRetCode = true;

        }
    }
    catch (...)
    {
        bRetCode = false;                   
    }
    return bRetCode;
}
//-pw500001

bool CreatePickListTable()
{
	_RecordsetPtr pPickListRecordset;
	HRESULT hr;
	_bstr_t bstrTableName (_T("PickListItemsSold"));
	COleVariant vOrd, vVal;
	COleSafeArray saField, saVal;
	FieldPtr pField;
	_variant_t vtParm;
    bool bRetCode = false;
    try
    {
		hr = pPickListRecordset.CreateInstance(__uuidof(Recordset));
        if (FAILED(hr))
		{

		}
        else
        {
            try
            {
		        hr = pPickListRecordset->Open(bstrTableName, _variant_t((IDispatch *)g_pConnection, true),
			        adOpenKeyset, adLockOptimistic, adCmdTable);

            }
            catch (_com_error e)
            {
                // Table doesn't exist go create it
                // Create the Version table with one field
                cout << _T("    Creating PickListItemsSold Table") << endl;
                _bstr_t strSQLCreateTable(_T("CREATE TABLE PickListItemsSold \
                    (ItemUPC TEXT(50) CONSTRAINT PrimaryKey PRIMARY KEY, \
                     TimesSold LONG, TimesSoldTime DATETIME, \
                     TimesSoldHistory TEXT(150))"));
                g_pConnection->Execute(strSQLCreateTable, NULL, adExecuteNoRecords);
                _bstr_t strSQLCreateIndex(_T("CREATE INDEX ItemUPC ON PickListItemsSold \
                    (ItemUPC ASC)"));
                g_pConnection->Execute(strSQLCreateIndex, NULL, adExecuteNoRecords);
                cout << _T("    Created PickListItemsSold Table Successfully") << endl;
            }
            bRetCode = true;

        }
    }
    catch (...)
    {
        bRetCode = false;                   
    }
    return bRetCode;
}

bool Patch32ATableUpdates()
{
	_RecordsetPtr pPatch32ARecordset;
	_bstr_t bstrTableName (_T("PickListItemsSold"));
	COleVariant vOrd, vVal;
	COleSafeArray saField, saVal;
	FieldPtr pField;
	_variant_t vtParm;
    bool bRetCode = false;
    try
    {
        cout << _T("    Performing Patch 3.2A Updates") << endl;
        // Create new column in table
        bstr_t strSQLUpdateTable = (_T("ALTER TABLE ReportText \
            ADD COLUMN SevenDaySaleSummaryByLane text(100)"));
        g_pConnection->Execute(strSQLUpdateTable, NULL, adExecuteNoRecords);
        

 
        cout << _T("    Patch 3.2A Updates Completed Successfully") << endl;
        bRetCode = true;

    }
    catch (...)
    {
        // Table doesn't exist go create it
        // Create the Version table with one field
        cout << _T("    Patch 3.2A Updates Failed!!!") << endl;
        bRetCode = false;                   
    }
    return bRetCode;
}

//REL_42_VERSION_1
bool Release42Version1TableUpdates()
{

    bool bRetCode = false;
    try
    {
        cout << _T("    Performing Patch 4.2A Updates") << endl;
        // Create new column in table
		bstr_t strSQLUpdateTable = (_T("ALTER TABLE ReportText \
			ADD COLUMN WeeklySignOnOffReport text(100)"));
		g_pConnection->Execute(strSQLUpdateTable, NULL, adExecuteNoRecords);
        // Create new column in table
		strSQLUpdateTable = (_T("ALTER TABLE ReportText \
			ADD COLUMN OnText text(100)"));
		g_pConnection->Execute(strSQLUpdateTable, NULL, adExecuteNoRecords);
        // Create new column in table
		strSQLUpdateTable = (_T("ALTER TABLE ReportText \
			ADD COLUMN OffText text(100)"));
		g_pConnection->Execute(strSQLUpdateTable, NULL, adExecuteNoRecords);
        // Create new column in table
		strSQLUpdateTable = (_T("ALTER TABLE ReportText \
			ADD COLUMN Utility text(100)"));
		g_pConnection->Execute(strSQLUpdateTable, NULL, adExecuteNoRecords);
        // Create new column in table
		strSQLUpdateTable = (_T("ALTER TABLE ReportText \
			ADD COLUMN App text(100)"));
		g_pConnection->Execute(strSQLUpdateTable, NULL, adExecuteNoRecords);
        cout << _T("    Patch 4.2A Updates Completed Successfully") << endl;
        bRetCode = true;

    }
    catch (...)
    {
        // Table doesn't exist go create it
        // Create the Version table with one field
        cout << _T("    Patch 4.2A Updates Failed!!!") << endl;
        bRetCode = false;                   
    }
    return bRetCode;
}

bool UpdateVersionTable()
{
	_RecordsetPtr pVersionRecordset;
	HRESULT hr;
	_bstr_t bstrTableName (_T("DBVersion"));
	COleVariant vOrd, vVal;
	COleSafeArray saField, saVal;
	FieldPtr pField;
	long rgIndices[1];
	_variant_t vtParm;
    bool bRetCode = false;
    try
    {
		hr = pVersionRecordset.CreateInstance(__uuidof(Recordset));
        if (FAILED(hr))
		{
            cerr << _T("ERROR Failure of CreateInstance of Recordset") << endl;

		}
        else
        {
            cout << _T("    Updating Version Table") << endl;
		    hr = pVersionRecordset->Open(bstrTableName, _variant_t((IDispatch *)g_pConnection, true),
			    adOpenKeyset, adLockOptimistic, adCmdTable);
			saField.CreateOneDim(VT_VARIANT, (unsigned long) 1);
			saVal.CreateOneDim(VT_VARIANT, (unsigned long) 1);

			vOrd = _T("Version");
			vVal = (long) CURRENT_VERSION;
			rgIndices[0] = 0;
			saField.PutElement(rgIndices, COleVariant(&vOrd));
			saVal.PutElement(rgIndices, COleVariant(&vVal));
            bRetCode = true;

			// Update the record
			if (VARIANT_TRUE == pVersionRecordset->EndOfFile)
			{
				hr = pVersionRecordset->AddNew(saField, saVal);
			}
			else
			{
				hr = pVersionRecordset->Update(saField, saVal);
			}

			if (FAILED(hr))
			{
                bRetCode = false;
			}
			
			pVersionRecordset->Close();
        }
    }
    catch (...)
    {
        cerr << _T("ERROR Exception caught") << endl;
        bRetCode = false;                   
    }
    return bRetCode;

}
bool CreateNewEntry( const CString& csLanguageCode )  //FL4.1 REQUIREMENT
{
	_RecordsetPtr pVersionRecordset;
	HRESULT hr;
	_bstr_t bstrTableName (_T("ReportText"));
	COleVariant vOrd, vVal;
	COleSafeArray saField, saVal;
	FieldPtr pField;
	long rgIndices[1];
	_variant_t vtParm;
    bool bRetCode = false;
    try
    {
		hr = pVersionRecordset.CreateInstance(__uuidof(Recordset));
        if (FAILED(hr))
		{
            cerr << _T("ERROR Failure of CreateInstance of Recordset") << endl;

		}
        else
        {
			_TCHAR *tLanguageCode = new _TCHAR[ csLanguageCode.GetLength() + 1 ];
			_tcscpy( tLanguageCode, csLanguageCode );
            wcout << _T("    Adding ");
			wcout << tLanguageCode ;
			wcout << _T(" Column to ReportText Table") << endl;

		    hr = pVersionRecordset->Open(bstrTableName, _variant_t((IDispatch *)g_pConnection, true),
			    adOpenKeyset, adLockOptimistic, adCmdTable);
			saField.CreateOneDim(VT_VARIANT, (unsigned long) 1);
			saVal.CreateOneDim(VT_VARIANT, (unsigned long) 1);

			vOrd = _T("Language");
			vVal = tLanguageCode ;
			rgIndices[0] = 0;
			saField.PutElement(rgIndices, COleVariant(&vOrd));
			saVal.PutElement(rgIndices, COleVariant(&vVal));
            bRetCode = true;

			hr = pVersionRecordset->AddNew(saField, saVal);

			if (FAILED(hr))
			{
                bRetCode = false;
			}
			
			pVersionRecordset->Close();

			delete tLanguageCode ;
			tLanguageCode = NULL;

        }
    }
    catch (...)
    {
        // In this case an exception may be thrown because the column may already exist.  
        // This is still an acceptable result
        bRetCode = true;                   
    }
    return bRetCode;

}
bool CreateCastillianEntry()
{
	_RecordsetPtr pVersionRecordset;
	HRESULT hr;
	_bstr_t bstrTableName (_T("ReportText"));
	COleVariant vOrd, vVal;
	COleSafeArray saField, saVal;
	FieldPtr pField;
	long rgIndices[1];
	_variant_t vtParm;
    bool bRetCode = false;
    try
    {
		hr = pVersionRecordset.CreateInstance(__uuidof(Recordset));
        if (FAILED(hr))
		{
            cerr << _T("ERROR Failure of CreateInstance of Recordset") << endl;

		}
        else
        {
            cout << _T("    Adding Castillian Column to ReportText Table") << endl;
		    hr = pVersionRecordset->Open(bstrTableName, _variant_t((IDispatch *)g_pConnection, true),
			    adOpenKeyset, adLockOptimistic, adCmdTable);
			saField.CreateOneDim(VT_VARIANT, (unsigned long) 1);
			saVal.CreateOneDim(VT_VARIANT, (unsigned long) 1);

			vOrd = _T("Language");
			vVal = _T("0C0A");
			rgIndices[0] = 0;
			saField.PutElement(rgIndices, COleVariant(&vOrd));
			saVal.PutElement(rgIndices, COleVariant(&vVal));
            bRetCode = true;

			hr = pVersionRecordset->AddNew(saField, saVal);

			if (FAILED(hr))
			{
                bRetCode = false;
			}
			
			pVersionRecordset->Close();
        }
    }
    catch (...)
    {
        // In this case an exception may be thrown because the column may already exist.  
        // This is still an acceptable result
        bRetCode = true;                   
    }
    return bRetCode;

}
bool CreateVersionTable()
{
    try
    {
        // Create the Version table with one field
        cout << _T("    Creating Version Table") << endl;
        _bstr_t strSQLCreateTable(_T("CREATE TABLE DBVersion (Version INTEGER)"));
        g_pConnection->Execute(strSQLCreateTable, NULL, adExecuteNoRecords);
        cout << _T("    Created Version Table Successfully") << endl;
        return true;
    }
    catch (...)
    {
        cerr << _T("ERROR Exception caught") << endl;
        return false;                   
    }

}
bool GetDBVersion(int &iDBVersion)
{
	_RecordsetPtr pVersionRecordset;
	HRESULT hr;
	_bstr_t bstrTableName (_T("DBVersion"));
	COleVariant vOrd, vVal;
	COleSafeArray saField, saVal;
	FieldPtr pField;
	_variant_t vtParm;
    bool bRetCode = false;
    try
    {
		hr = pVersionRecordset.CreateInstance(__uuidof(Recordset));
        if (FAILED(hr))
		{
            cerr << _T("ERROR CreateInstance of recordset failed") << endl;
		}
        else
        {
            try
            {
                cout << _T("    Opening Version Table") << endl;
		        hr = pVersionRecordset->Open(bstrTableName, _variant_t((IDispatch *)g_pConnection, true),
			        adOpenKeyset, adLockOptimistic, adCmdTable);

            }
            catch (_com_error e)
            {
                // Table doesn't exist go create it
                if (CreateVersionTable())
                {
                    iDBVersion = 1;
                    return true;

                }
            }
            cout << _T("    Retrieving Version ") << endl;
            pField = pVersionRecordset->GetFields()->GetItem(_T("Version"));
			vtParm = pField->GetValue();
            iDBVersion = vtParm.intVal;
            bRetCode = true;

        }
    }
    catch (...)
    {
        cerr << _T("ERROR Exception caught") << endl;
        bRetCode = false;                   
    }
    return bRetCode;
}
bool UpdateQuery(_bstr_t bstrQueryName, _bstr_t bstrNewSQL)
{
    bool bRetCode = false;
    DAO::_QueryDefPtr pQueryDef;
    _bstr_t bstrSQL;
    try
    {
        pQueryDef = g_pDAODatabase->GetQueryDefs()->GetItem(bstrQueryName);
        bstrSQL = pQueryDef->SQL;
        pQueryDef->PutSQL(bstrNewSQL);
        bRetCode = true;
        return bRetCode;
    }
    catch (...)
    {
    }
    try
    {
        pQueryDef = g_pDAODatabase->CreateQueryDef(bstrQueryName, bstrNewSQL);
        bRetCode = true;
    }
    catch (...)
    {
        cerr << _T("ERROR Exception caught") << endl;
        bRetCode = false;                   
    }
    return bRetCode;
}
bool CloseDatabase()
{
    g_pDAODatabase->Close();
    g_pDAODatabase = NULL;
    g_pConnection->Close();
    g_pConnection = NULL;
    return true;
}
bool OpenDatabase()
{
    HRESULT hr;
	g_pConnection = NULL;
	g_pRecordset = NULL;
    bool bRetStat = false;

	// 5-partition mods.
	TCHAR szDataDrive[_MAX_DRIVE+1];
	DWORD dwRet;
	CString csDefaultConnectString;
	CString csDatabaseName;

	dwRet = ExpandEnvironmentStrings(_T("%DATA_DRIVE%"), szDataDrive, _MAX_DRIVE+1);
	if(dwRet == 0 || dwRet > _MAX_DRIVE+1)
	{
		_tcsncpy(szDataDrive, _T("C:"), _MAX_DRIVE+1);	// Start with sane values.
	}

	csDefaultConnectString.Format(DEFAULT_CONNECT_STRING, (LPCTSTR)szDataDrive);
    _bstr_t bstrConnectString = csDefaultConnectString;
	// 5-partition mods.

    g_pDAOEngine.CreateInstance(__uuidof(DAO::DBEngine));
    g_pDAOWorkSpace = g_pDAOEngine->_30_CreateWorkspace (_T(""), _T("admin"), _T(""));

	csDatabaseName.Format(DATABASE_NAME, (LPCTSTR)szDataDrive);	// 5-partition mods.
    g_pDAODatabase = g_pDAOWorkSpace->OpenDatabase((LPCTSTR)csDatabaseName);
	hr = g_pConnection.CreateInstance(__uuidof(Connection));
    if (FAILED(hr))
	{

	}
    else
    {

        // Error on the open at this point results in a COM exception, which is why no check follows it
		g_pConnection->Open(bstrConnectString, _bstr_t(_T("Admin")), _bstr_t(_T("")), adConnectUnspecified); //lint !e534 Error causes exception
        cout << _T("    Opened Database") << endl;
        bRetStat = true;

    }
    return bRetStat;

}

//REL_42_VERSION_2
bool CreateLaneConfigurationsTable()
{
	_RecordsetPtr pLaneConfigurationsRecordset;
	HRESULT hr;
	_bstr_t bstrTableName (_T("LaneConfigurations"));
	COleVariant vOrd, vVal;
	COleSafeArray saField, saVal;
	FieldPtr pField;
	_variant_t vtParm;
    bool bRetCode = false;
	_bstr_t strSQLUpdateTable;
    try
    {
		hr = pLaneConfigurationsRecordset.CreateInstance(__uuidof(Recordset));
        if (FAILED(hr))
		{

		}
        else
        {
            try
            {
		        hr = pLaneConfigurationsRecordset->Open(bstrTableName, _variant_t((IDispatch *)g_pConnection, true),
			        adOpenKeyset, adLockOptimistic, adCmdTable);

            }
            catch (_com_error e)
            {
                // Table doesn't exist go create it
                cout << _T("    Creating LaneConfigurations Table") << endl;
                _bstr_t strSQLCreateTable(_T("CREATE TABLE LaneConfigurations \
                    (TableKey AUTOINCREMENT (1, 1) CONSTRAINT PrimaryKey PRIMARY KEY, \
                     StationID TEXT(50), TransactionID TEXT(50), \
                     HardwareVersion TEXT(50), HardwareConfiguration TEXT(50))"));
                g_pConnection->Execute(strSQLCreateTable, NULL, adExecuteNoRecords);
                _bstr_t strSQLCreateIndex(_T("CREATE INDEX StationID ON LaneConfigurations \
                    (StationID ASC)"));
                g_pConnection->Execute(strSQLCreateIndex, NULL, adExecuteNoRecords);
               
 
                cout << _T("    Created LaneConfigurations Table Successfully") << endl;
            }
            bRetCode = true;

        }
    }
    catch (...)
    {
        bRetCode = false;                   
    }
    return bRetCode;
}


//REL_42_VERSION_2
bool CreateOperatorLoginsTable()
{
	_RecordsetPtr pOperatorLoginsRecordset;
	HRESULT hr;
	_bstr_t bstrTableName (_T("OperatorLogins"));
	COleVariant vOrd, vVal;
	COleSafeArray saField, saVal;
	FieldPtr pField;
	_variant_t vtParm;
    bool bRetCode = false;
	_bstr_t strSQLUpdateTable;
    try
    {
		hr = pOperatorLoginsRecordset.CreateInstance(__uuidof(Recordset));
        if (FAILED(hr))
		{

		}
        else
        {
            try
            {
		        hr = pOperatorLoginsRecordset->Open(bstrTableName, _variant_t((IDispatch *)g_pConnection, true),
			        adOpenKeyset, adLockOptimistic, adCmdTable);

            }
            catch (_com_error e)
            {
                // Table doesn't exist go create it
                cout << _T("    Creating OperatorLogins Table") << endl;
                _bstr_t strSQLCreateTable(_T("CREATE TABLE OperatorLogins \
                    (TableKey AUTOINCREMENT (1, 1) CONSTRAINT PrimaryKey PRIMARY KEY, \
                     StationID TEXT(50), TransactionID TEXT(50), \
                     OperatorID TEXT(50), \
					 ActivityTime DATETIME, SignOn YESNO, \
					 Application TEXT(50))"));
                g_pConnection->Execute(strSQLCreateTable, NULL, adExecuteNoRecords);
            
				_bstr_t strSQLCreateIndex(_T("CREATE INDEX TableKey ON OperatorLogins \
                    (TableKey ASC)"));
                g_pConnection->Execute(strSQLCreateIndex, NULL, adExecuteNoRecords);
               			 
                cout << _T("    Created OperatorLogins Table Successfully") << endl;
            }
            bRetCode = true;

        }
    }
    catch (...)
    {
        bRetCode = false;                   
    }
    return bRetCode;
}

/* BEGIN MEIJER CUSTOM CHANGES */
bool CreateMCouponTable()
{
	_RecordsetPtr pMCouponRecordset;
	HRESULT hr;
	_bstr_t bstrTableName (_T("MCoupons"));
	COleVariant vOrd, vVal;
	COleSafeArray saField, saVal;
	FieldPtr pField;
	_variant_t vtParm;
    bool bRetCode = false;
	_bstr_t strSQLUpdateTable;
    try
    {
		hr = pMCouponRecordset.CreateInstance(__uuidof(Recordset));
        if (FAILED(hr))
		{

		}
        else
        {
            try
            {
		        hr = pMCouponRecordset->Open(bstrTableName, _variant_t((IDispatch *)g_pConnection, true),
			        adOpenKeyset, adLockOptimistic, adCmdTable);

            }
            catch (_com_error e)
            {
                // Table doesn't exist go create it
                cout << _T("    Creating MCoupons Table") << endl;
                _bstr_t strSQLCreateTable(_T("CREATE TABLE MCoupons \
                    (TableKey AUTOINCREMENT (1, 1) CONSTRAINT PrimaryKey PRIMARY KEY, \
                     StationID TEXT(50), TransactionID TEXT(50), \
                     CouponCount LONG, \
					 CouponAmount CURRENCY)"));
                g_pConnection->Execute(strSQLCreateTable, NULL, adExecuteNoRecords);
            
				_bstr_t strSQLCreateIndex(_T("CREATE INDEX TableKey ON MCoupons \
                    (TableKey ASC)"));
                g_pConnection->Execute(strSQLCreateIndex, NULL, adExecuteNoRecords);
               			 
                cout << _T("    Created MCoupons Table Successfully") << endl;
            }
            bRetCode = true;

        }
    }
    catch (...)
    {
        bRetCode = false;                   
    }
    return bRetCode;
}
bool CreateMReportTextTable()
{
	_RecordsetPtr pMReportTextRecordset;
	HRESULT hr;
	_bstr_t bstrTableName (_T("MReportText"));
	COleVariant vOrd, vVal;
	COleSafeArray saField, saVal;
	FieldPtr pField;
	_variant_t vtParm;
    bool bRetCode = false;
	_bstr_t strSQLUpdateTable;
    try
    {
		hr = pMReportTextRecordset.CreateInstance(__uuidof(Recordset));
        if (FAILED(hr))
		{

		}
        else
        {
            try
            {
		        hr = pMReportTextRecordset->Open(bstrTableName, _variant_t((IDispatch *)g_pConnection, true),
			        adOpenKeyset, adLockOptimistic, adCmdTable);

            }
            catch (_com_error e)
            {
                // Table doesn't exist go create it
                cout << _T("    Creating MReportText Table") << endl;
                _bstr_t strSQLCreateTable(_T("CREATE TABLE MReportText \
                    (MLanguage TEXT(50) CONSTRAINT PrimaryKey PRIMARY KEY, \
                     Coupons TEXT(50), \
					 ChangeDispensed TEXT(50))"));
                g_pConnection->Execute(strSQLCreateTable, NULL, adExecuteNoRecords);
            
				_bstr_t strSQLCreateIndex(_T("CREATE INDEX MLanguage ON MReportText \
                    (MLanguage ASC)"));
                g_pConnection->Execute(strSQLCreateIndex, NULL, adExecuteNoRecords);
               			 
                cout << _T("    Created MReportText Table Successfully") << endl;
            }
            bRetCode = true;

        }
    }
    catch (...)
    {
        bRetCode = false;                   
    }
    return bRetCode;
}

bool UpdateMReportText()
{
    bool brc = true;
    _bstr_t strSQLUpdateTable;
    CString csSQLUpdateTable;

    cout << _T("    Updating MReport Text Table") << endl;
    try
    {
        csSQLUpdateTable.Format(_T("UPDATE MReportText SET MReportText.[Coupons] = \"Coupons\" WHERE MLanguage = \"0409\""));
        strSQLUpdateTable = (LPCTSTR)csSQLUpdateTable;
        g_pConnection->Execute(strSQLUpdateTable, NULL, adExecuteNoRecords);                
        csSQLUpdateTable.Format(_T("UPDATE MReportText SET MReportText.[ChangeDispensed] = \"Change Dispensed\" WHERE MLanguage = \"0409\""));
        strSQLUpdateTable = (LPCTSTR)csSQLUpdateTable;
        g_pConnection->Execute(strSQLUpdateTable, NULL, adExecuteNoRecords);                
    }
    catch (...)
    {
        cerr << _T("ERROR Exception caught while updating report text") << endl;
        brc = false;                   
    }
    return brc;
}
bool UpdateMCoupons()
{
    bool bRetCode = false;
    _bstr_t strSQLUpdateTable;
    CString csSQLUpdateTable;

    try
    {
        cout << _T("    Updating MJR Coupon Table") << endl;		
        // Create new column in table
		bstr_t strSQLUpdateTable = (_T("ALTER TABLE MCoupons \
			ADD COLUMN MediaType text(100)"));
		g_pConnection->Execute(strSQLUpdateTable, NULL, adExecuteNoRecords);		
        csSQLUpdateTable.Format(_T("UPDATE MCoupons SET MCoupons.[MediaType] = \"Coupons\" "));
        strSQLUpdateTable = (LPCTSTR)csSQLUpdateTable;
        g_pConnection->Execute(strSQLUpdateTable, NULL, adExecuteNoRecords);                
        cout << _T("    MJR Coupon Table Updates Completed Successfully") << endl;
        bRetCode = true;

    }
    catch (...)
    {
        // Table doesn't exist go create it
        // Create the Version table with one field
        cout << _T("    MJR Coupon Table Updates Failed!!!") << endl;
        bRetCode = false;                   
    }
    return bRetCode;
}

bool CreateMReportTextEntry()
{
	_RecordsetPtr pVersionRecordset;
	HRESULT hr;
	_bstr_t bstrTableName (_T("MReportText"));
	COleVariant vOrd, vVal;
	COleSafeArray saField, saVal;
	FieldPtr pField;
	long rgIndices[1];
	_variant_t vtParm;
    bool bRetCode = false;
    try
    {
		hr = pVersionRecordset.CreateInstance(__uuidof(Recordset));
        if (FAILED(hr))
		{
            cerr << _T("ERROR Failure of CreateInstance of Recordset") << endl;

		}
        else
        {
            cout << _T("    Adding English Column to MReportText Table") << endl;
		    hr = pVersionRecordset->Open(bstrTableName, _variant_t((IDispatch *)g_pConnection, true),
			    adOpenKeyset, adLockOptimistic, adCmdTable);
			saField.CreateOneDim(VT_VARIANT, (unsigned long) 1);
			saVal.CreateOneDim(VT_VARIANT, (unsigned long) 1);

			vOrd = _T("MLanguage");
			vVal = _T("0409");
			rgIndices[0] = 0;
			saField.PutElement(rgIndices, COleVariant(&vOrd));
			saVal.PutElement(rgIndices, COleVariant(&vVal));
            bRetCode = true;

			hr = pVersionRecordset->AddNew(saField, saVal);

			if (FAILED(hr))
			{
                bRetCode = false;
			}
			
			pVersionRecordset->Close();
        }
    }
    catch (...)
    {
        // In this case an exception may be thrown because the column may already exist.  
        // This is still an acceptable result
        bRetCode = true;                   
    }
    return bRetCode;

}

/* END MEIJER CUSTOM CHANGES */

bool PerformQueryUpdates()
{
    bool bRetCode = true;
    bool bTempCode;
    _bstr_t bstrQuery;
    _bstr_t bstrQueryName;
    cout << _T("    Performing Query Updates") << endl;
    bstrQuery = TODAYS_HOURLY_ASSISTANCE_REPORT_LANE_QUERY;
    bstrQueryName = TODAYS_HOURLY_ASSISTANCE_REPORT_LANE;
    bRetCode = UpdateQuery(bstrQueryName, bstrQuery);	
    bstrQuery = TODAYS_HOURLY_ASSISTANCE_REPORT_TRANS_QUERY;
    bstrQueryName = TODAYS_HOURLY_ASSISTANCE_REPORT_TRANS;
    bTempCode = UpdateQuery(bstrQueryName, bstrQuery);
    bRetCode &= bTempCode;
    bstrQuery = YESTERDAYS_HOURLY_ASSISTANCE_REPORT_LANE_QUERY;
    bstrQueryName = YESTERDAYS_HOURLY_ASSISTANCE_REPORT_LANE;
    bTempCode = UpdateQuery(bstrQueryName, bstrQuery);
    bRetCode &= bTempCode;
    bstrQuery = YESTERDAYS_HOURLY_ASSISTANCE_REPORT_TRANS_QUERY;
    bstrQueryName = YESTERDAYS_HOURLY_ASSISTANCE_REPORT_TRANS;
    bTempCode = UpdateQuery(bstrQueryName, bstrQuery);
    bRetCode &= bTempCode;
    bstrQuery = ASSISTANCE_REPORT_ONE_DAY_SUMMARY_ASSISTS_QUERY;
    bstrQueryName = ASSISTANCE_REPORT_ONE_DAY_SUMMARY_ASSISTS;
    bTempCode = UpdateQuery(bstrQueryName, bstrQuery);
    bRetCode &= bTempCode;
    bstrQuery = ASSISTANCE_REPORT_YESTERDAY_SUMMARY_ASSISTS_QUERY;
    bstrQueryName = ASSISTANCE_REPORT_YESTERDAY_SUMMARY_ASSISTS;
    bTempCode = UpdateQuery(bstrQueryName, bstrQuery);
    bRetCode &= bTempCode;
    bstrQuery = ASSISTANCE_REPORT_ONE_DAY_SUMMARY_TRANS_QUERY;
    bstrQueryName = ASSISTANCE_REPORT_ONE_DAY_SUMMARY_TRANS;
    bTempCode = UpdateQuery(bstrQueryName, bstrQuery);
    bRetCode &= bTempCode;
    bstrQuery = ASSISTANCE_REPORT_YESTERDAY_SUMMARY_TRANS_QUERY;
    bstrQueryName = ASSISTANCE_REPORT_YESTERDAY_SUMMARY_TRANS;
    bTempCode = UpdateQuery(bstrQueryName, bstrQuery);
    bRetCode &= bTempCode;
    bstrQuery = ASSISTANCE_REPORT_ONE_DAY_SUMMARY_LANE_QUERY;
    bstrQueryName = ASSISTANCE_REPORT_ONE_DAY_SUMMARY_LANE;
    bTempCode = UpdateQuery(bstrQueryName, bstrQuery);
    bRetCode &= bTempCode;
    bstrQuery = ASSISTANCE_REPORT_YESTERDAY_SUMMARY_LANE_QUERY;
    bstrQueryName = ASSISTANCE_REPORT_YESTERDAY_SUMMARY_LANE;
    bTempCode = UpdateQuery(bstrQueryName, bstrQuery);
    bRetCode &= bTempCode;
    bstrQuery = ASSISTANCE_REPORT_ONE_DAY_SUMMARY_ASSISTS_BY_LANE_QUERY;
    bstrQueryName = ASSISTANCE_REPORT_ONE_DAY_SUMMARY_ASSISTS_BY_LANE;
    bTempCode = UpdateQuery(bstrQueryName, bstrQuery);
    bRetCode &= bTempCode;
    bstrQuery = ASSISTANCE_REPORT_YESTERDAY_SUMMARY_ASSISTS_BY_LANE_QUERY;
    bstrQueryName = ASSISTANCE_REPORT_YESTERDAY_SUMMARY_ASSISTS_BY_LANE;
    bTempCode = UpdateQuery(bstrQueryName, bstrQuery);
    bRetCode &= bTempCode;
    bstrQuery = ASSISTANCE_REPORT_ONE_DAY_SUMMARY_TRANS_BY_LANE_QUERY;
    bstrQueryName = ASSISTANCE_REPORT_ONE_DAY_SUMMARY_TRANS_BY_LANE;
    bTempCode = UpdateQuery(bstrQueryName, bstrQuery);
    bRetCode &= bTempCode;
    bstrQuery = ASSISTANCE_REPORT_YESTERDAY_SUMMARY_TRANS_BY_LANE_QUERY;
    bstrQueryName = ASSISTANCE_REPORT_YESTERDAY_SUMMARY_TRANS_BY_LANE;
    bTempCode = UpdateQuery(bstrQueryName, bstrQuery);
    bRetCode &= bTempCode;
    bstrQuery = ASSISTANCE_REPORT_ONE_DAY_SUMMARY_LANE_BY_LANE_QUERY;
    bstrQueryName = ASSISTANCE_REPORT_ONE_DAY_SUMMARY_LANE_BY_LANE;
    bTempCode = UpdateQuery(bstrQueryName, bstrQuery);
    bRetCode &= bTempCode;
    bstrQuery = ASSISTANCE_REPORT_YESTERDAY_SUMMARY_LANE_BY_LANE_QUERY;
    bstrQueryName = ASSISTANCE_REPORT_YESTERDAY_SUMMARY_LANE_BY_LANE;
    bTempCode = UpdateQuery(bstrQueryName, bstrQuery);
    bRetCode &= bTempCode;
    bstrQuery = ASSISTANCE_REPORT_ONE_DAY_BY_CASHIER_LANE_QUERY;
    bstrQueryName = ASSISTANCE_REPORT_ONE_DAY_BY_CASHIER_LANE;
    bTempCode = UpdateQuery(bstrQueryName, bstrQuery);
    bRetCode &= bTempCode;
    bstrQuery = ASSISTANCE_REPORT_YESTERDAY_BY_CASHIER_LANE_QUERY;
    bstrQueryName = ASSISTANCE_REPORT_YESTERDAY_BY_CASHIER_LANE;
    bTempCode = UpdateQuery(bstrQueryName, bstrQuery);
    bRetCode &= bTempCode;
    bstrQuery = WEEKLY_ASSISTANCE_TOTALS_TRANS_QUERY;
    bstrQueryName = WEEKLY_ASSISTANCE_TOTALS_TRANS;
    bTempCode = UpdateQuery(bstrQueryName, bstrQuery);
    bRetCode &= bTempCode;
    bstrQuery = WEEKLY_ASSISTANCE_TOTALS_ASSISTS_QUERY;
    bstrQueryName = WEEKLY_ASSISTANCE_TOTALS_ASSISTS;
    bTempCode = UpdateQuery(bstrQueryName, bstrQuery);
    bRetCode &= bTempCode;
    bstrQuery = WEEKLY_ASSISTANCE_TOTALS_ASSISTED_TRANS_COUNT_QUERY;
    bstrQueryName = WEEKLY_ASSISTANCE_TOTALS_ASSISTED_TRANS_COUNT;
    bTempCode = UpdateQuery(bstrQueryName, bstrQuery);
    bRetCode &= bTempCode;
    bstrQuery = WEEKLY_ASSISTANCE_TOTALS_ASSISTED_TRANS_QUERY;
    bstrQueryName = WEEKLY_ASSISTANCE_TOTALS_ASSISTED_TRANS;
    bTempCode = UpdateQuery(bstrQueryName, bstrQuery);
    bRetCode &= bTempCode;
	bstrQuery = WEEKLY_ATTENDANT_TRANS_REPORT_LANE_QUERY;
    bstrQueryName = WEEKLY_ATTENDANT_TRANS_REPORT_LANE;
    bTempCode = UpdateQuery(bstrQueryName, bstrQuery);
    bRetCode &= bTempCode;
	bstrQuery = WEEKLY_ATTENDANT_ENTRIES_REPORT_LANE_QUERY;
    bstrQueryName = WEEKLY_ATTENDANT_ENTRIES_REPORT_LANE;
    bTempCode = UpdateQuery(bstrQueryName, bstrQuery);
    bRetCode &= bTempCode;
	bstrQuery = WEEKLY_ATTENDANT_MODE_REPORT_LANE_QUERY;
    bstrQueryName = WEEKLY_ATTENDANT_MODE_REPORT_LANE;
    bTempCode = UpdateQuery(bstrQueryName, bstrQuery);
    bRetCode &= bTempCode;
    bstrQuery = ASSISTANCE_REPORT_WEEK_SUMMARY_ASSIST_QUERY;
    bstrQueryName = ASSISTANCE_REPORT_WEEK_SUMMARY_ASSIST;
    bTempCode = UpdateQuery(bstrQueryName, bstrQuery);
    bRetCode &= bTempCode;
    bstrQuery = ASSISTANCE_REPORT_WEEK_SUMMARY_LANE_QUERY;
    bstrQueryName = ASSISTANCE_REPORT_WEEK_SUMMARY_LANE;
    bTempCode = UpdateQuery(bstrQueryName, bstrQuery);
    bRetCode &= bTempCode;
    bstrQuery = ASSIST_REPORT_WEEK_SUMMARY_ASSIST_TRANS_QUERY;
    bstrQueryName = ASSIST_REPORT_WEEK_SUMMARY_ASSIST_TRANS;
    bTempCode = UpdateQuery(bstrQueryName, bstrQuery);
	bRetCode &= bTempCode;    
    bstrQuery = ASSIST_REPORT_WEEK_AVG_LANE_QUERY;
    bstrQueryName = ASSIST_REPORT_WEEK_AVG_LANE;
    bTempCode = UpdateQuery(bstrQueryName, bstrQuery);
    bRetCode &= bTempCode;

    bstrQuery = YESTERDAYS_RAP_TILL_QUERY;
    bstrQueryName = YESTERDAYS_RAP_TILL;
    bTempCode = UpdateQuery(bstrQueryName, bstrQuery);
    bRetCode &= bTempCode;
    bstrQuery = YESTERDAYS_RAP_CHANGE_QUERY;
    bstrQueryName = YESTERDAYS_RAP_CHANGE;
    bTempCode = UpdateQuery(bstrQueryName, bstrQuery);
    bRetCode &= bTempCode;
    bstrQuery = YESTERDAYS_RAP_TENDER_QUERY;
    bstrQueryName = YESTERDAYS_RAP_TENDER;
    bTempCode = UpdateQuery(bstrQueryName, bstrQuery);
    bRetCode &= bTempCode;
    bstrQuery = YESTERDAYS_RAP_TILL_REPORT_QUERY;
    bstrQueryName = YESTERDAYS_RAP_TILL_REPORT;
    bTempCode = UpdateQuery(bstrQueryName, bstrQuery);
    bRetCode &= bTempCode;
    bstrQuery = TODAYS_RAP_TILL_QUERY;
    bstrQueryName = TODAYS_RAP_TILL;
    bTempCode = UpdateQuery(bstrQueryName, bstrQuery);
    bRetCode &= bTempCode;
    bstrQuery = TODAYS_RAP_CHANGE_QUERY;
    bstrQueryName = TODAYS_RAP_CHANGE;
    bTempCode = UpdateQuery(bstrQueryName, bstrQuery);	
    bRetCode &= bTempCode;
    bstrQuery = TODAYS_RAP_TENDER_QUERY;
    bstrQueryName = TODAYS_RAP_TENDER;
    bTempCode = UpdateQuery(bstrQueryName, bstrQuery);
    bRetCode &= bTempCode;
    bstrQuery = TODAYS_RAP_TILL_REPORT_QUERY;
    bstrQueryName = TODAYS_RAP_TILL_REPORT;
    bTempCode = UpdateQuery(bstrQueryName, bstrQuery);
    bRetCode &= bTempCode;
    bstrQuery = WEEKLY_ASSISTANCE_SUMMARY_BY_CASHIER_LANE_QUERY;
    bstrQueryName = WEEKLY_ASSISTANCE_SUMMARY_BY_CASHIER_LANE;
    bTempCode = UpdateQuery(bstrQueryName, bstrQuery);
    bRetCode &= bTempCode;
    bstrQuery = WEEKLY_ASSISTANCE_SUMMARY_BY_CASHIER_RAP_QUERY;
    bstrQueryName = WEEKLY_ASSISTANCE_SUMMARY_BY_CASHIER_RAP;
    bTempCode = UpdateQuery(bstrQueryName, bstrQuery);
    bRetCode &= bTempCode;
    bstrQuery = WEEKLY_ASSISTANCE_SUMMARY_BY_CASHIER_BY_DATE_LANE_QUERY;
    bstrQueryName = WEEKLY_ASSISTANCE_SUMMARY_BY_CASHIER_BY_DATE_LANE;
    bTempCode = UpdateQuery(bstrQueryName, bstrQuery);
    bRetCode &= bTempCode;

    // TAR 286056 start
    bstrQuery = ASSISTANCE_REPORT_WEEK_SUMMARY_RAP_QUERY;
    bstrQueryName = ASSISTANCE_REPORT_WEEK_SUMMARY_RAP;
    bTempCode = UpdateQuery(bstrQueryName, bstrQuery);
    bRetCode &= bTempCode;
    bstrQuery = WEEKLY_ASSISTANCE_TOTALS_RAP_QUERY;
    bstrQueryName = WEEKLY_ASSISTANCE_TOTALS_RAP;
    bTempCode = UpdateQuery(bstrQueryName, bstrQuery);
    bRetCode &= bTempCode;
    bstrQuery = ASSISTANCE_REPORT_ONE_DAY_SUMMARY_PAGER_BY_LANE_QUERY;
    bstrQueryName = ASSISTANCE_REPORT_ONE_DAY_SUMMARY_PAGER_BY_LANE;
    bTempCode = UpdateQuery(bstrQueryName, bstrQuery);
    bRetCode &= bTempCode;
    bstrQuery = ASSISTANCE_REPORT_YESTERDAY_SUMMARY_PAGER_BY_LANE_QUERY;
    bstrQueryName = ASSISTANCE_REPORT_YESTERDAY_SUMMARY_PAGER_BY_LANE;
    bTempCode = UpdateQuery(bstrQueryName, bstrQuery);
    bRetCode &= bTempCode;
    // TAR 286056 end
	bstrQuery = SIGNON_ENTRIES_QUERY;
    bstrQueryName = SIGNON_ENTRIES;
    bTempCode = UpdateQuery(bstrQueryName, bstrQuery);
    bRetCode &= bTempCode;
    return bRetCode;

}
bool MigrateDatabase()
{
    int iDBVersion;
    CString csMessage;
    bool bRetCode = true;

    if (GetDBVersion(iDBVersion))
    {
        csMessage.Format(_T("    Current Version %d"), iDBVersion);
        cout << (LPCTSTR)csMessage << endl;

        // Here is where all calls to the update routines should be called

        bRetCode = PerformQueryUpdates();

        if ((iDBVersion < PICK_LIST_VERSION) && bRetCode)
        {
            bRetCode = CreatePickListTable();
        }
        if ((iDBVersion < REL_40_VERSION) )
        {
            bRetCode = CreateRAPChangeDispensedTable();
        }
        if ((iDBVersion < REL_40_VERSION_2) )
        {
            bRetCode = UpdateSignatureTable();
        }
        if ((iDBVersion < REL_40_VERSION_3) )
        {
            bRetCode = CreateDeviceErrorTable();
        }
        if ((iDBVersion < REL_40_VERSION_4) )
        {
            bRetCode = CreateCastillianEntry();
        }
		if ((iDBVersion < REL_32D_ATTENDANTMODE) )
        {
            bRetCode = CreateApplicationModeTable();
			bRetCode = UpdateTransactionsTable();
        }
		if ((iDBVersion < REL_41_VERSION_4) ) //FL4.1 REQUIREMENT
		{
			bRetCode = CreateNewEntry( _T("0415") );
			bRetCode = CreateNewEntry( _T("0412") );
		}
		if ((iDBVersion < REL_41_VERSION_7) ) //FL4.2 REQUIREMENT (finnish, swedish and turkish)
		{
			bRetCode = CreateNewEntry( _T("040b") );	//finnish
			bRetCode = CreateNewEntry( _T("041d") );	//swedish
			bRetCode = CreateNewEntry( _T("041f") );	//turkish
		} 
		if ((iDBVersion < REL_42_VERSION_1) ) //FL4.2 VERSION 1 REQUIREMENTS
		{
			Release42Version1TableUpdates();
		} 

		if ((iDBVersion < REL_42_VERSION_2) ) //FL4.2 Enterprise Reporting Enhancements
		{
			bRetCode = CreateOperatorLoginsTable();
			bRetCode = CreateLaneConfigurationsTable();
			bRetCode = UpdateTransactionsTable2();
		} 
        //+pw500001 cashmanagement
        if ((iDBVersion < REL_41_VERSION_5) )
        {
            bRetCode = CreateCashManagementTable();
        }
        //-pw500001
        if ((iDBVersion < REL_41_VERSION_6) )
        {
            bRetCode = UpdateCashManagementTable();
        }
        if ((iDBVersion < PATCH32A_UPDATES_VERSION))
        {
            bRetCode = Patch32ATableUpdates();
        }

        bRetCode = UpdateReportTextFromFile();
        CreateMCouponTable();
        CreateMReportTextTable();
        CreateMReportTextEntry();
        UpdateMReportText();
        UpdateMCoupons();
    }
    else
    {
        bRetCode = false;
    }
    return bRetCode;
}
int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{

	// initialize MFC and print and error on failure
	if (!AfxWinInit(::GetModuleHandle(NULL), NULL, ::GetCommandLine(), 0))
	{
		// TODO: change error code to suit your needs
		cerr << _T("Fatal Error: MFC initialization failed") << endl;
		g_nRetCode = 1;
	}
	else
	{
		cout << _T("----------  Migrating FLReports.mdb  ------------") << endl;
        if (!SUCCEEDED( CoInitialize(NULL)))
        {
            g_nRetCode = MDB_CANTINITIALIZE;
            cerr << _T("ERROR Unable to Initialize") << endl;
        }
        else
        {
            if (OpenDatabase())
            {
                // If we opened the table successfully, update it
                if (MigrateDatabase())
                {
                    // After all the updates have been complete, we will update
                    // the version table entry to the new version

					//TAR 393644: We will leave Version table update to Base Patch G
					//   UpdateVersionTable();
                }
                CloseDatabase();
            }
            else
            {
                g_nRetCode = MDB_CANTOPEN;
                cerr << _T("ERROR Unable to Open Database") << endl;
            }

        }
	}
	cout << _T("----------  Completed Migration  ------------") << endl;

	return g_nRetCode;
}


