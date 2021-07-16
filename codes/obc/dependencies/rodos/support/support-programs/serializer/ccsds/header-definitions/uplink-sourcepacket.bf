
#  Bit fields of the CCSDS header of the source packes for uplink (Commands and data upload)

#name of the namespace
CCSDS

#name of the generated strucutre
UplinkSPHeader

#field name        bits   Commenst
version              3    set to 0
type                 1    1 -> telecommand
secondaryHeaderFlag  1    set to 1 (We have a PUS header)
applicationId       11  
sequenceFlags        2    set 3 for standalone
sequenceCounter     14
length	            16    real_len - 1

# Secondary (PUS) Header, secondary header
pusSecondaryHeaderFlag 1    set to 0
pusVersion             3    set to 1
ackType                4    4x Bool: (completion, execution, start, acceptance)
serviceType            8
serviceSubtype         8
sourceID               8    set to  0

# secondary PUS header (Warning: optional, and free definition!)
#timeSeconds        32
#timeFraction       16
#destinationId       8
#commandCounter      8





