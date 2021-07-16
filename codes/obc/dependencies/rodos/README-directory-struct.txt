.
├── api
│   ├── gateway
│   └── hal
├── doc
├── scripts
│   ├── build-scripts
│   └── help-scrips
│       └── git-aware-prompt
├── src
│   ├── bare-metal
│   │   ├── linux-makecontext
│   │   │   └── hal -> ../linux-x86/hal/
│   │   ├── linux-x86
│   │   │   └── hal
│   │   ├── stm32f4
│   │   │   ├── CMSIS
│   │   │   │   ├── Device
│   │   │   │   │   └── ST
│   │   │   │   │       └── STM32F4xx
│   │   │   │   │           └── Include
│   │   │   │   └── Include
│   │   │   ├── examples
│   │   │   ├── hal
│   │   │   ├── scripts
│   │   │   ├── startup
│   │   │   └── STM32F4xx_StdPeriph_Driver
│   │   │       ├── inc
│   │   │       └── src
│   │   └── template
│   │       └── README-how-to-port
│   ├── bare-metal-generic
│   │   └── ip
│   │       └── lwip
│   │           ├── core
│   │           ├── include
│   │           │   ├── arch
│   │           │   ├── lwip
│   │           │   └── netif
│   │           └── netif
│   ├── independent
│   │   └── gateway
│   ├── on-posix
│   │   └── hal -> ../bare-metal/linux-x86/hal
│   └── on-posix-mac
│       └── hal
├── support
│   ├── support-libs
│   │   └── ccsds -> ../support-programs/serializer/ccsds
│   ├── support-programs
│   │   ├── maskgenerator
│   │   │   ├── examples
│   │   │   └── qt-example
│   │   │       └── qtdisplay
│   │   ├── middleware-standalone
│   │   │   ├── api
│   │   │   │   ├── gateway
│   │   │   │   └── hal
│   │   │   ├── examples
│   │   │   │   ├── on-linux
│   │   │   │   └── on-rodos
│   │   │   └── src
│   │   │       ├── independent
│   │   │       ├── linux
│   │   │       │   └── hal
│   │   │       └── stm32f4
│   │   │           └── hal
│   │   ├── serializer
│   │   │   ├── ccsds
│   │   │   │   ├── generated
│   │   │   │   └── header-definitions
│   │   │   ├── ccsds-envelop-tutorial
│   │   │   ├── example-minimal-standalone
│   │   │   │   └── include
│   │   │   ├── example-on-rodos
│   │   │   │   └── include
│   │   │   ├── generator
│   │   │   │   └── parser
│   │   │   │       └── src
│   │   │   ├── test
│   │   │   │   └── include
│   │   │   └── test_with-virtual_call-for-gateway
│   │   │       └── include
│   │   ├── telecommand-generator
│   │   │   └── generated-examples
│   │   ├── timing-display-octave
│   │   └── udpinterface
│   └── tools
├── tmp
└── tutorials
    ├── 10-first-steps
    │   └── gps
    ├── 20-core
    │   ├── shared-stack-activity
    │   └── tutorials-with-acceptable-warnings
    ├── 30-communication-and-bbs
    │   ├── 10-alice-bob-charly
    │   ├── 20-middleware
    │   ├── 30-middleware-collector
    │   ├── 40-middleware-distributed
    │   ├── 50-middleware-miscs
    │   ├── 60-middleware-ping
    │   └── 80-gateway
    │       ├── uart
    │       └── uart-can-udp
    ├── 40-support-lib
    │   └── matlib
    │       ├── geodetic
    │       ├── lowpassfilter
    │       ├── rotations
    │       └── trust-kalmanfilter-for-informatics
    ├── 50-time-related
    │   ├── 1pps-getriggered-scheduling
    │   ├── network-time-sync
    │   │   ├── client
    │   │   └── server
    │   └── time-triegged-protocoll
    │       ├── client
    │       └── server
    ├── 60-tutorial-big-application
    │   ├── app01
    │   ├── app02
    │   ├── app03
    │   ├── app04
    │   │   ├── subdir1
    │   │   ├── subdir2
    │   │   └── subdir3
    │   └── common
    └── 70-network-and-io
        ├── 10-udp
        │   └── udp-linux-standalone
        ├── 20-tcp
        │   └── tcp-linux-standalone
        └── 30-interrupts

