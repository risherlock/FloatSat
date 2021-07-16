

Definition of the bit fields of CCSDS headers.
To generate code use
	bitFieldsSerializer
	bitFieldsSerializer.cpp
see examples in
	bitFieldsSerializer-example.bf

ccsds-envelop/*
	different for downlink and uplink

TF = tf = TransferFrame
SP = sp = Source package


How to use: See examples in
../ccsds-test-envelop/
    ccsds-print-headers.cpp
    ccsds-example-downlink-receive.cpp
    ccsds-example-downlink-sender.cpp
    ccsds-example-uplink-receive.cpp
    ccsds-example-uplink-sender.cpp


How to use:
To make it easier to you, concentrate first only on donwlink.
When you understand downlink then see uplink.
same procedure, but die ccsds fields differs (Idea of CCSDS-people!!!)


use %meld to compare uplink and downlink or spacesegment and gounrsegment
they shall be very similar


/**
 *---------------- to Send data ---------------
 *
 *Ground: (Sends commands)
 *        uplink-transferframe.bf
 *SpaceCraft:(send Telemetry)
 *        downlink-transferframe.bf
 *
 * initDefaultTFHeaderAndTrailer();
 * initDefaultSPHeader();
 *
 *loop {
 *        beginNewTF
 *        loop {
 *                maxlen = beginNewSP();
 *                if(maxlen < whatINeed) break;
 *                wirte data to userDataBuf[0 ..  lenOfCurrentUserData
 *                lenOfCurrentUserData = myLen;
 *                set required sp-header parameter
 *                commitSP();
 *        }
 *        set required ft-header parameter
 *        commitTF();
 *        send (buf, tflen);
 *}
 **/



/**
 *------------- to recieve  data----------------
 *
 *Ground:  (receives Telemetry)
 *       downlink-transferframe.bf 
 *SpaceCraft:  (recieves commands)
 *        uplink-transferframe.bf 
 *
 *loop {
 *        checkoutTF();
 *        use some tf-header parameter
 *        loop {
 *                ok = checkoutSP();
 *                if(!ok) break;
 *                use some sp-header parameter
 *                read data from userDataBuf[0 ..  lenOfCurrentUserData
 *        }
 *}
 **/



