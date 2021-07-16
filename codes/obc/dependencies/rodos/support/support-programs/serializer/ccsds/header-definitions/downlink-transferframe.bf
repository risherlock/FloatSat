
#  Bit fields of the CCSDS header of the transfer frame for downlink (Telemetry)
# for a mission the trasnfer frame length (including source packeges) is constant

#name of the namespace
CCSDS

#name of the generated strucutre
DownlinkTFHeader

# Warning, eventaully first 4 bytes of sync: 0x1acffc1d

#field name      number or bits     Commenst

# primary header

version             2              set to 0
spaceCraftId       10
virtualChanId       3              0: Real time, 1 history, 7 idle
opControlFlag       1              1 -> at the end of the TF is a 32-bit CLCW command status word
masterChanFrameCnt  8
virtualChanFrameCnt 8
secondHeaderFlag    1              1-> there is a second. header (I recomend: NO secondary header)
synchFlag           1              set to 0
packetOrderFlag     1              set to 0
segmentLenId        2              set to 11
firstHeaderPtr     11              normaly set to 0 , for idle frames set to 11111111110 (0x7fe)

# secondary header
#secondaryVersion    2              set to 0
#secondHeaderLen     6              warning: secondHeaderLen = header_real_len - 1


