import os
import subprocess
def Settings(**kwargs):
#    os.chdir("rodos-core")
#    subprocess.Popen(["source make/rodosenvs"], shell=True)
    final_flags = [ "-Werror", "-Wno-long-long", "-Wno-cpp",  
            "-pedantic" ,"-fno-exceptions", "-ferror-limit=100000","-x", "c++", "-std=c++11", 
            "-I", ".", "-I"]
    final_flags.append(os.environ["RODOS_API"])

    final_flags.append("-I")
    my_srcs = os.environ["RODOS_ROOT"] + "/src/bare-metal-generic"
    final_flags.append(my_srcs)

    final_flags.append("-I")
    my_hw_srcs = os.environ["RODOS_ROOT"] + "/src/bare-metal/linux-x86"
    final_flags.append(my_hw_srcs)

    final_flags.append("-I")
    final_flags.append(os.environ["RODOS_SUPPORT_LIBS"])
    flags = {"flags" : final_flags}

    return flags
