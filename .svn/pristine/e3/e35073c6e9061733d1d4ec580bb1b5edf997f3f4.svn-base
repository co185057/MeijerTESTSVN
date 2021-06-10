/*
-------------------------------------------------------------------------------

 Module Name     : DPSElements.h
 Type            : Include file for application to use

 Language        : MSVC++
 Operating System: Win32
 Author/Location : SCOT 1.0 Team, RSG-Atlanta

 Copyright (C) NCR Corporation 1997, 1998

-------------------------------------------------------------------------------
 Date    | Changes Made                                      | Version | By
-------------------------------------------------------------------------------
 05/30/97  Original version.                                   01.00.00  KSo
 07/23/98  Now notify the application of events even when      01.00.04  KSo
           DPS handles them internally.  This is useful
           when the application wants to monitor user's
           inactivity.
-------------------------------------------------------------------------------

 This include file defines the DPS standard element IDs.  These IDs are fixed
 and shall not be changed (analogous to IDs of window messages, such as 
 WM_PAINT etc or standard control IDs such as IDCOK IDCCANCEL).

-------------------------------------------------------------------------------
$Archive: /Fastlane/4.0 Patch B/Core/Source/Include/DPSElements.h $
$Revision: 1 $
$Date: 1/26/05 1:32p $
-------------------------------------------------------------------------------
*/

#ifndef _DPSELEMENT_H_
#define _DPSELEMENT_H_

#ifndef _EXCL_ELEMENTIDS_
/*
** Standard Template IDs
*/
#define  IDD_REWARDDATA                  160
#define  IDD_RECEIPTWITHREWARDDATA       161
#define  IDD_CMFRAME          200
#define  IDD_CMRECEIPT        201
#define  IDD_CMLOOKUP         202
#define  IDD_CMHELP           203
#define  IDD_CMLIST           204
#define  IDD_CMTOTAL          205
#define  IDD_CMLISTNOSCROLL   209
#define  IDD_CMLIVEVIDEO      210

#define  IDD_SMFRAME          300
#define  IDD_SMRECEIPT        301
#define  IDD_SMLOOKUP         302
#define  IDD_SMHELP           303
#define  IDD_SMLIST           304
#define  IDD_SMTOTAL          305
#define  IDD_SMMAINMENU       306
#define  IDD_SMINFOLIST       307
#define  IDD_SMINFOBOX        308

/*
** Standard Element IDs
*/
         /* Passive Elements */
#define  IDC_BACKGROUND       1000        /* background of the entrie frame */
#define  IDC_LEADTHRUTEXT     1001        /* title of the screen */
#define  IDC_MESSAGE          1002        /* message box that does not touch the ButtonX */
#define  IDC_MESSAGEMODAL     1003        /* message box that touches the ButtonX */
#define  IDC_PROMPT1          1004
#define  IDC_PROMPT2          1005        /* reserved */
#define  IDC_AMOUNT1          1006
#define  IDC_AMOUNT2          1007
#define  IDC_TAXTEXT          1008
#define  IDC_TOTALTEXT        1009
#define  IDC_TAXAMOUNT        1010
#define  IDC_TOTALAMOUNT      1011
#define  IDC_WEIGHT           1012
#define  IDC_TITLE            1013
#define  IDC_HELPLIST         1014
#define  IDC_LIST             1015
#define  IDC_IMAGE            1016
#define  IDC_LISTSORTED       1017
#define  IDC_AMOUNT1LABEL     2004
#define  IDC_AMOUNT2LABEL     1010
#define  IDC_AMOUNT1          1006
#define  IDC_AMOUNT2          1007

#define  IDC_SCREENTEXT       1017
#define	 IDC_IMAGE2			  1019

#define  IDC_RECEIPTCOL1      1020        /* variable number cols */
#define  IDC_RECEIPTCOL2      1021        /* reserve 10 for this */
#define  IDC_RECEIPTCOL3      1022
#define  IDC_RECEIPTCOL4      1023
#define  IDC_RECEIPTCOL5      1024
#define  IDC_RECEIPTCOL6      1025
#define  IDC_RECEIPTCOL7      1026
#define  IDC_RECEIPTCOL8      1027
#define  IDC_RECEIPTCOL9      1028
#define  IDC_RECEIPTCOL10     1029
#define  IDC_TITLEPIC         1030
#define  IDC_LISTCOL1         1031
#define  IDC_LISTCOL2         1032
#define  IDC_LISTCOL3         1033
#define  IDC_LISTCOL4         1034
#define  IDC_MESSAGE2         1035
#define  IDC_MESSAGE12        1036
#define  IDC_MESSAGE13        1037
#define  IDC_MESSAGE14        1038
#define  IDC_MESSAGE23        1039
#define  IDC_MESSAGE24        1040
#define  IDC_MESSAGEMODAL2    1041
#define  IDC_MESSAGEMODAL12   1042
#define  IDC_MESSAGEMODAL13   1043
#define  IDC_MESSAGEMODAL14   1044
#define  IDC_MESSAGEMODAL23   1045
#define  IDC_MESSAGEMODAL24   1046
#define  IDC_LEADTHRUTEXT2    1047
#define  IDC_TITLE2           1048
#define  IDC_MESSAGE3         1049
#define  IDC_MESSAGE34        1050
#define  IDC_MESSAGEMODAL3    1051
#define  IDC_MESSAGEMODAL34   1052
#define  IDC_LIVEVIDEO        1053
#define  IDC_LIVEVIDEO2       1054
#define  IDC_LIVEVIDEO3       1055
#define  IDC_LIVEVIDEO4       1056
#define  IDC_MESSAGEMODALWIDE34 1057
#define  IDC_BUTTONGOBACK     1058
#define  IDC_BUTTONX2         1059
#define  IDC_CUSTOMERMESSAGE             1060
#define  IDC_AMOUNTSAVEDLABEL            1061
#define  IDC_AMOUNTSAVED                 1062
#define  IDC_POINTEARNEDLABEL            1063
#define  IDC_POINTEARNED                 1064

         /* Interactive Elements */
#define  IDC_ECHO             1100        /* the one and only Echo for each frame */
#define  IDC_RECEIPT          1101
#define  IDC_ECHO2            1102
#define  IDC_ECHO1            1103
#define  IDC_ECHO3            1104
#define  IDC_ECHO4            1105
#define  IDC_ECHO5            1106
#define  IDC_ECHO6            1107
#define  IDC_ECHO7            1108


         /* Interactive Buttons */
#define  IDC_BUTTONHELP       1200
#define  IDC_BUTTONUP         1201        /* does not generate events in SCOT */
#define  IDC_BUTTONDOWN       1202        /* does not generate events in SCOT */
#define  IDC_RECEIPTUP        1203
#define  IDC_RECEIPTDOWN      1204
#define  IDC_BUTTONBACKUP     1205
#define  IDC_BUTTONMAINMENU   1206
#define  IDC_BUTTONTOGGLE     2009
#define  IDC_BUTTONLANESTATUS 2010

#define  IDC_BUTTONX          1209        /* These are ButtonX text */
#define  IDC_BUTTON1          1210
#define  IDC_BUTTON2          1211
#define  IDC_BUTTON3          1212
#define  IDC_BUTTON4          1213
#define  IDC_BUTTON5          1214
#define  IDC_BUTTON6          1215
#define  IDC_BUTTON7          1216
#define  IDC_BUTTON8          1217
#define  IDC_BUTTON9          1218
#define  IDC_BUTTON10         1219
#define  IDC_BUTTON11         1220
#define  IDC_BUTTON12         1221
#define  IDC_BUTTON13         1222
#define  IDC_BUTTON14         1223
#define  IDC_BUTTON15         1224
#define  IDC_BUTTON16         1225
#define  IDC_BUTTON17         1226
#define  IDC_BUTTON18         1227
#define  IDC_BUTTON19         1228
#define  IDC_BUTTON20         1229

#define  IDC_BUTTON1PIC       1240        /* ButtonX foreground pictures */
#define  IDC_BUTTON2PIC       1241
#define  IDC_BUTTON3PIC       1242
#define  IDC_BUTTON4PIC       1243
#define  IDC_BUTTON5PIC       1244
#define  IDC_BUTTON6PIC       1245
#define  IDC_BUTTON7PIC       1246
#define  IDC_BUTTON8PIC       1247
#define  IDC_BUTTON9PIC       1248
#define  IDC_BUTTON10PIC      1249
#define  IDC_BUTTON11PIC      1250
#define  IDC_BUTTON12PIC      1251
#define  IDC_BUTTON13PIC      1252
#define  IDC_BUTTON14PIC      1253
#define  IDC_BUTTON15PIC      1254
#define  IDC_BUTTON16PIC      1255
#define  IDC_BUTTON17PIC      1256
#define  IDC_BUTTON18PIC      1257
#define  IDC_BUTTON19PIC      1258
#define  IDC_BUTTON20PIC      1259

#define  IDC_BUTTONXBG        1260        /* ButtonX Background picture */
#define  IDC_ALPHALIST        1270
#define  IDC_HELPTOPICS       1271        /* Help topics */

#define  IDC_BUTTON1S         1280
#define  IDC_BUTTON2S         1281
#define  IDC_BUTTON3S         1282
#define  IDC_BUTTON4S         1283
#define  IDC_BUTTON5S         1284
#define  IDC_BUTTON6S         1285
#define  IDC_BUTTON7S         1286
#define  IDC_BUTTON8S         1287
#define  IDC_BUTTON9S         1288
#define  IDC_BUTTON10S        1289
#define  IDC_BUTTON11S        1290
#define  IDC_BUTTON12S        1291
#define  IDC_BUTTON13S        1292
#define  IDC_BUTTON14S        1293
#define  IDC_BUTTON15S        1294
#define  IDC_BUTTON16S        1295
#define  IDC_BUTTON17S        1296
#define  IDC_BUTTON18S        1297
#define  IDC_BUTTON19S        1298
#define  IDC_BUTTON20S        1299

         /* Extra elements for StoreMode */
#define  IDC_STATUS           1310
#define  IDC_STATUS1          1311
#define  IDC_STATUS2          1312
#define  IDC_STATUS3          1313
#define  IDC_STATUS4          1314
#define  IDC_STATUS5          1315
#define  IDC_STATUS6          1316
#define  IDC_STATUS7          1317
#define  IDC_STATUS8          1318
#define  IDC_STATUS9          1319
#define  IDC_STATUS10         1320
#define  IDC_STATUS11         1321
#define  IDC_STATUS12         1322

#define  IDC_MENU1            1340
#define  IDC_MENU2            1341
#define  IDC_MENU3            1342
#define  IDC_MENU4            1343
#define  IDC_MENU5            1344
#define  IDC_MENU6            1345
#define  IDC_MENU7            1346
#define  IDC_MENU8            1347
#define  IDC_MENU9            1348
#define  IDC_MENU10           1349
#define  IDC_MENU11           1350
#define  IDC_MENU12           1351
#define  IDC_MENU13           1352
#define  IDC_MENU14           1353
#define  IDC_MENU15           1354

#define IDC_KEYBOARD          1440
#define IDC_KEYBOARDP1        1441
#define IDC_KEYBOARDP2        1442
#define IDC_KEYBOARDP3        1443
#define IDC_VLIST             1460

#define IDC_KEY1              1500
#define IDC_KEY2              1501
#define IDC_KEY3              1502
#define IDC_KEY4              1503
#define IDC_KEY5              1504
#define IDC_KEY6              1505
#define IDC_KEY7              1506
#define IDC_KEY8              1507
#define IDC_KEY9              1508
#define IDC_KEY10             1509
#define IDC_KEY11             1510
#define IDC_KEY12             1511
#define IDC_KEY13             1512
#define IDC_KEY14             1513
#define IDC_KEY15             1514
#define IDC_KEY16             1515
#define IDC_KEY17             1516
#define IDC_KEY18             1517
#define IDC_KEY19             1518
#define IDC_KEY20             1519
#define IDC_KEY21             1520
#define IDC_KEY22             1521
#define IDC_KEY23             1522
#define IDC_KEY24             1523
#define IDC_KEY25             1524
#define IDC_KEY26             1525
#define IDC_KEY27             1526
#define IDC_KEY28             1527
#define IDC_KEY29             1528
#define IDC_KEY30             1529
#define IDC_KEY31             1530
#define IDC_KEY32             1531
#define IDC_KEY33             1532
#define IDC_KEY34             1533
#define IDC_KEY35             1534
#define IDC_KEY36             1535
#define IDC_KEY37             1536
#define IDC_KEY38             1537
#define IDC_KEY39             1538
#define IDC_KEY40             1539
#define IDC_KEY41             1540
#define IDC_KEY42             1541
#define IDC_KEY43             1542
#define IDC_KEY44             1543
#define IDC_KEY45             1544
#define IDC_KEY46             1545
#define IDC_KEY47             1546
#define IDC_KEY48             1547
#define IDC_KEY49             1548
#define IDC_KEY50             1549
#define IDC_KEY51             1550
#define IDC_KEY52             1551
#define IDC_KEY53             1552
#define IDC_KEY54             1553
#define IDC_KEY55             1554
#define IDC_KEY56             1555
#define IDC_KEY57             1556
#define IDC_KEY58             1557
#define IDC_KEY59             1558
#define IDC_KEY60             1559
#define IDC_KEY61             1560
#define IDC_KEY62             1561
#define IDC_KEY63             1562
#define IDC_KEY64             1563
#define IDC_KEY65             1564
#define IDC_KEY66             1565
#define IDC_KEY67             1566
#define IDC_KEY68             1567
#define IDC_KEY69             1568
#define IDC_KEY70             1569
#define IDC_KEY71             1570
#define IDC_KEY72             1571
#define IDC_KEY73             1572
#define IDC_KEY74             1573
#define IDC_KEY75             1574
#define IDC_KEY76             1575
#define IDC_KEY77             1576
#define IDC_KEY78             1577
#define IDC_KEY79             1578
#define IDC_KEY80             1579
#define IDC_KEY81             1580
#define IDC_KEY82             1581
#define IDC_KEY83             1582
#define IDC_KEY84             1583
#define IDC_KEY85             1584
#define IDC_KEY86             1585
#define IDC_KEY87             1586
#define IDC_KEY88             1587
#define IDC_KEY89             1588
#define IDC_KEY90             1589
#define IDC_KEY91             1590
#define IDC_KEY92             1591
#define IDC_KEY93             1592
#define IDC_KEY94             1593
#define IDC_KEY95             1594
#define IDC_KEY96             1595
#define IDC_KEY97             1596
#define IDC_KEY98             1597
#define IDC_KEY99             1598
#define IDC_KEY100             1599
#define IDC_KEY101             1600
#define IDC_KEY102             1601
#define IDC_KEY103             1602
#define IDC_KEY104             1603
#define IDC_KEY105             1604
#define IDC_KEY106             1605
#define IDC_KEY107             1606
#define IDC_KEY108             1607
#define IDC_KEY109             1608
#define IDC_KEY110             1609
#define IDC_KEY111             1610
#define IDC_KEY112             1611
#define IDC_KEY113             1612
#define IDC_KEY114             1613
#define IDC_KEY115             1614
#define IDC_KEY116             1616
#define IDC_KEY117             1616
#define IDC_KEY118             1617
#define IDC_KEY119             1618
#define IDC_KEY120             1619
#define IDC_KEY121             1620
#define IDC_KEY122             1621
#define IDC_KEY123             1622
#define IDC_KEY124             1623
#define IDC_KEY125             1624
#define IDC_KEY126             1625
#define IDC_KEY127             1626
#define IDC_KEY128             1627
#define IDC_KEY129             1628
#define IDC_KEY130             1629
#define IDC_KEY131             1630
#define IDC_KEY132             1631
#define IDC_KEY133             1632
#define IDC_KEY134             1633
#define IDC_KEY135             1634
#define IDC_KEY136             1635
#define IDC_KEY137             1636
#define IDC_KEY138             1637
#define IDC_KEY139             1638
#define IDC_KEY140             1639
#define IDC_KEY141             1640
#define IDC_KEY142             1641
#define IDC_KEY143             1642
#define IDC_KEY144             1643
#define IDC_KEY145             1644
#define IDC_KEY146             1645
#define IDC_KEY147             1646
#define IDC_KEY148             1647
#define IDC_KEY149             1648
#define IDC_KEY150             1649
#define IDC_KEY151             1650
#define IDC_KEY152             1651
#define IDC_KEY153             1652
#define IDC_KEY154             1653
#define IDC_KEY155             1654
#define IDC_KEY156             1655
#define IDC_KEY157             1656
#define IDC_KEY158             1657
#define IDC_KEY159             1658
#define IDC_KEY160             1659
#define IDC_KEY161             1660
#define IDC_KEY162             1661
#define IDC_KEY163             1662
#define IDC_KEY164             1663
#define IDC_KEY165             1664
#define IDC_KEY166             1665
#define IDC_KEY167             1666
#define IDC_KEY168             1667
#define IDC_KEY169             1668
#define IDC_KEY170             1669
#define IDC_KEY171             1670
#define IDC_KEY172             1671
#define IDC_KEY173             1672
#define IDC_KEY174             1673
#define IDC_KEY175             1674
#define IDC_KEY176             1675
#define IDC_KEY177             1676
#define IDC_KEY178             1677
#define IDC_KEY179             1678
#define IDC_KEY180             1679
#define IDC_KEY181             1680
#define IDC_KEY182             1681
#define IDC_KEY183             1682
#define IDC_KEY184             1683
#define IDC_KEY185             1684
#define IDC_KEY186             1685
#define IDC_KEY187             1686
#define IDC_KEY188             1687
#define IDC_KEY189             1688
#define IDC_KEY190             1689
#define IDC_KEY191             1690
#define IDC_KEY192             1691
#define IDC_KEY193             1692
#define IDC_KEY194             1693
#define IDC_KEY195             1694
#define IDC_KEY196             1695
#define IDC_KEY197             1696
#define IDC_KEY198             1697
#define IDC_KEY199             1698
#define IDC_KEY200             1699

#define IDC_KEYBOARDP4			2003
#define IDC_PRODUCTIMAGECTRL1	2005
#define IDC_TENDERIMAGECTRL     2008
#define IDC_REMOTEBUTTON1		2013
#define IDC_REMOTEBUTTON2		2014
#define IDC_REMOTEBUTTON3		2015
#define IDC_REMOTEBUTTON4		2016


#endif   / *_EXCL_ELEMENTIDS_*/

/*
** Notification Messages
*/
#define  DPSN_BUTTON_PRESSED     100
#define  DPSN_BUTTON_RELEASED    101
#define  DPSN_LIST_SELCHANGE     200
#define  DPSN_KEYDOWN            300
#define  DPSN_KEYUP              301
#define  DPSN_VIDEO_DONE         302
#define  DPSN_TOUCH              400 /* General touch event for user activity monitor */
#define	DPSN_REMOTE_DISCONNECT  500 /* Event for remote connections.  The other side has disconnected */
#define  DPSN_REMOTE_CONNECT     501 /* Event for remote connect */

// For reference, the following notification define is in SCOTAuxDisp.h
// #define DPSN_AUXDISP_SPOOF_NUMERIC   777

/*
** Window styles
*/
const int STYLE_FRAME    = 0;
const int STYLE_POPUPTX  = 1;
const int STYLE_POPUPPIC = 2;

#endif