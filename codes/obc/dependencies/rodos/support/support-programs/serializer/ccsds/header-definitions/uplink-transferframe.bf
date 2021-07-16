
#  Bit fields of the CCSDS header of the transfer frame for Uplink (commands and data upload)

#name of the namespace
CCSDS

#name of the generated strucutre
UplinkTFHeader

### first 2 bytes sync =  0xeb90

#name                 bits  comments
version               2     set to 0
bypassFlag            1     0->A: normal (check SeqNr),  1->B: Bypass, accept all cmds
controllCommandFlag   1     0->D (data upload protocoll) 1 -> C (Command protocoll)
spare1                2     set to 0
spacecraftID         10
virtualChannelID      6   
frameLength          10     frameLength = real_len - 1
frameSequenceNr       8

# segment header
sequenceFlags         2     set to 0x3 
multiplexAceessPoint  6     set to 0x3 , eg to select target computer


### at the end of transferrame 4 bytes tail sequence pattern =  0xc5c5c5c5c5c5c5c5


