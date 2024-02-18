import subprocess
import caen_module
import re

debug = True

def dec2hex(n) : 
    return format(n, "#x")

# str -> str
def hex2bin(val, n_bit) :
    return bin(int(val, 16))[2:].zfill(n_bit)

def call_cmdvme(haddr, subadd, val, val_name, mode="wr") :
    if not val and val is not 0 :
        cmd = caen_module.cmdvme+" -"+mode+" "+format(haddr+subadd, "#x")
    else: 
        cmd = caen_module.cmdvme+" -"+mode+" "+format(haddr+subadd, "#x")+ " " +  dec2hex(val)
    if debug :
        print (cmd + " (" + val_name + ")")
    return subprocess.check_output(cmd, shell=True, executable="/bin/bash")

def get_firmware_revision(haddr) :
    result = call_cmdvme(haddr, 0x1026, val, "Status Register")
    return result

def set_control_register(haddr, val) :
    call_cmdvme(haddr, 0x1000, val, "Control Register")

def get_status_register(haddr) :
    result = call_cmdvme(haddr, 0x100e, val, "Status Register")
    if debug :
        print()


def wait_write_ok(haddr) :
    ret = 0
    
    while ret == 0: 
        retbytes = call_cmdvme(haddr,0x1030,"","Write_OK?")
        retstr=retbytes.decode()
        m=re.search("[\d]+",retstr)
        ret=int(m.group()) & 1
       # print (ret)
        if not ret :
            print("Waiting for Write_Ok.\n")

def set_micro_register(haddr,op_command,op_object) :
    opcode=(op_command<<8) + op_object
    #print (opcode)
    call_cmdvme(haddr,0x102E,opcode,"Micro Register")
