
#  Bit fields of the CCSDS header of the source packes for downlink (Telemetry)

#name of the namespace
CCSDS

#name of the generated strucutre
DownlinkSPHeader

#field name      number or bits     Commenst

# Warning, eventaully first 4 bytes of sync: 0x1acffc1d

# primary header  allways 6 bytes

version             3              just ignore! set to 0
typeId              1              shall be 0!
secHeaderFlag       1              1 -> secondary header follows
applicationId      11              normaly called APID, for idle set to 11111111111 = 0x7ff
groupingFlags       2              00: cont packet, 01: first one, 10: last one, 11: no groupping (11 Recommended!)
sourceSeqCnt       14              please count only not idle packeges
dataPackLen        16              Warning! = len(secondaryheader + userdata) - 1 , in our case here 10 + userdatalen - 1


# sondary header: Warning: not exactly defined in ccsds, in this case 10 bytes

secondaryFlag       1              shall be 0
pusVersion          3              ignore! set to 1
spare               4              shall be 0

service             8              3  = normal bus telemetry
subservice          8              25 = Automatic HKD
destination         8              0 for Groundstation (normal case) ; WARNING: Some expect here source (not dest)

timeStampSeconds   32              UTC in seconds since 1.1.2000  0:00, set by Housekeeper
timeStampFraction  16              (1/2^16) of the second

