
#  Bit fields of the CCSDS header of the transfer frame for downlink (Telemetry)
# for a mission the trasnfer frame length (including source packeges) is constant

#name of the namespace
CCSDS

#name of the generated strucutre
DownlinkTFTrailer

#name     bits  comments

# clcw controls:     32    Uplink-Acks 
control             1              shall be 0
version             2	           shall be 0
status              3              content not defined! recommend: 0
commandOpProcedure  2              COP in effect recommend 01
virtualChanel       6
spare1              2              shall be 0
noRF                1              0 -> ok, 1-> no signal received
noBitLock           1              0 -> ok, 1-> signal to noisy
lockOut             1              0 -> ok, 1-> frame rejected
wait                1
retransmit          1
farmBCnt            2              current counter of framtes Type B
spare2              1
reportValue         8              next expected frame sequence countrer of frames Type A 

crc                 16    cyclic redundancy code in CCSDS called "FRAME ERROR CONTROL FIELD"



