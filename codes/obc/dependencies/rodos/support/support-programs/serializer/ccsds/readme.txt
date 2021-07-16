

Warning: CCSDS Communication protocoll has to be implemented two times
once for uplink and once for downlink.
They are not the same!


Please first check the header definitions in
	header-definitions/*
Please compare each field with the ccsds documentation
different for downlink and uplink

then take a look to ccsds-envelop, to understand how to use
read frist
	readme-how-to-use.txt


To write this I used:
- CCSDS 133.0-B-1 Space Packet Protocol
- CCSDS 132.0-B-1 TM Space Data Link Protocol
- CCSDS 232.0-B-2 TC Space Data Link Protocol
- CCSDS 232.1-B-2 Communications Operation Procedure
- CCSDS A30.0-G-3 Space Data Systems Glossary


See 
	readme-how-to-use.txt

