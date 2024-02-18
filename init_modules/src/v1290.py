import subprocess
import caen_module
import re

debug = True

def dec2hex(n) : 
    return format(n, "#x")


def call_cmdvme(haddr,subadd,val,val_name) :
#    if not val and val is not 0 :
    if val == "" :
        cmd = caen_module.cmdvme+" -wr "+format(haddr+subadd, "#x")
    else: 
        cmd = caen_module.cmdvme+" -ww "+format(haddr+subadd, "#x")+ " " +  dec2hex(val)
    if debug :
            print (cmd + " (" + val_name + ")")

    return subprocess.check_output(cmd, shell=True, executable="/bin/bash")

def call_cmdvme_silent(haddr,subadd,val,val_name) :
    if val == "" :
        cmd = caen_module.cmdvme+" -wr "+format(haddr+subadd, "#x")
    else: 
        cmd = caen_module.cmdvme+" -ww "+format(haddr+subadd, "#x")+ " " +  dec2hex(val)

    return subprocess.check_output(cmd, shell=True, executable="/bin/bash")


def get_firmware_revision(haddr) :
    address = 0x1026
    if debug :
        print(caen_module.cmdvme+" -wr "+format(haddr+address, "#x"))
    #subprocess.check_outpu]t(caen_module.cmdvme+" -wr "+format(haddr+address_firmware_revision, "#x"))
    return subprocess.check_output(caen_module.cmdvme+" -wr "+format(haddr+address, "#x"), shell=True, executable="/bin/bash")

def set_geometry(haddr, geo) :
    call_cmdvme(haddr,0x100e,geo,"Geometry")

def set_control_register(haddr, val) :
    call_cmdvme(haddr,0x1000,val,"Control Register")

def set_module_reset(haddr, val) :
    call_cmdvme(haddr,0x1014,val,"Module reset")

def wait_write_ok(haddr) :
    ret = 0
    
    while ret == 0: 
        retbytes = call_cmdvme_silent(haddr,0x1030,"","Write_OK?")
        #Note: This method does not work for extracting hex numbers
        retstr=retbytes.decode()
        m=re.search("[\d]+",retstr)
        ret=int(m.group()) & 1

       # print (ret)
#        if not ret :
#            print("Waiting for Write_Ok.\n")

def wait_read_ok(haddr) :
    ret = 0
    
    while ret == 0: 
#        retbytes = call_cmdvme_silent(haddr,0x1030,"","Read_OK?")
        retbytes = call_cmdvme(haddr,0x1030,"","Read_OK?")
        retstr=retbytes.decode()
        m=re.search("[\d]+",retstr)
        ret=int(m.group()) & 2
       # print (ret)
#        if not ret :
#            print("Waiting for Write_Ok.\n")

        
def set_micro_register(haddr,opcode) :
    
    #opcode=(op_command<<8) + op_object
    #print (opcode)
    call_cmdvme(haddr,0x102E,opcode,"Micro Register: "+format(opcode, "#x"))

def read_micro_register(haddr) :
    retbytes = call_cmdvme(haddr,0x102E,"","Reading MR")
    retstr=retbytes.decode()
    print (retstr)
    #m=re.search("[\d]+",retstr)
    #ret=int(m.group())  
    #print (ret)


def wait_set_micro_register(haddr,opcode) :
    wait_write_ok(haddr)
    set_micro_register(haddr,opcode)


class v1290(caen_module.caen_module) :
    def __init__(self, conf) :
        super().__init__(conf["name"], conf["haddr"], conf["geometry"], conf["in use"])
        print()
        self.firmware_revision = get_firmware_revision(self.haddr)
        self.control_register = conf["control_register"]

        self.print()

    def init(self) :
        if self.in_use :
            set_module_reset(self.haddr, 1)
            set_geometry(self.haddr, self.geo)
            set_control_register(self.haddr, self.control_register)

            #Oct. 29,2021 HY not sure if the following microcodes are working...
            wait_set_micro_register(self.haddr, 0x2400) #LSB 
            set_micro_register(self.haddr, 0x2) #(default: 0x02, 100ps )
#            set_micro_register(self.haddr, 0x0) #(0x00, 800ps )

            wait_set_micro_register(self.haddr, 0x2600) #Read resolution
#            wait_read_ok(self.haddr)
            read_micro_register(self.haddr) #Read value 
            print ("If this is 2...100ps\n")


#            wait_set_micro_register(self.haddr, 0x0100) #Continuous Storage mode

# Dec. 16 2021, HY don't know why, but only one command may fail to set the mode.   
#  Dec. 17 comment: Most likely this was wrong usage of above resolution setting.  We need only one command now.

            wait_set_micro_register(self.haddr, 0x0000) #Trigger Matching mode
#            wait_set_micro_register(self.haddr, 0x0100) #Continuous Storage mode

            wait_set_micro_register(self.haddr, 0x0200) #Read acquisition mode
#            wait_read_ok(self.haddr)
            read_micro_register(self.haddr) #Read value 
            print ("If this is 1...trigger matching mode\n")




            wait_set_micro_register(self.haddr, 0x3000) #TDC header ON
#            wait_set_micro_register(self.haddr, 0x3100) #TDC header OFF (default)

            wait_set_micro_register(self.haddr, 0x3200) #Read header status
            wait_read_ok(self.haddr)
            read_micro_register(self.haddr) #Read value
            print ("If this is 1...header enabled\n")


 #           wait_write_ok(self.haddr)
            wait_set_micro_register(self.haddr, 0x1000) #window width to be set 
#            set_micro_register(self.haddr, 0x0050) #0x50..window width 2us
            set_micro_register(self.haddr, 0x0028) #0x28..window width 1us
#           set_micro_register(self.haddr, 0x0014) #0x14..window width 500ns

            wait_set_micro_register(self.haddr, 0x1100) #window offset to be set
            set_micro_register(self.haddr, 0x0FEC) #0xFEC...window offset -500ns  
#            set_micro_register(self.haddr, 0x0FD8) #0xFD8...window offset -1us 
#            set_micro_register(self.haddr, 0x000) #0x000...window offset 0ns  

            wait_set_micro_register(self.haddr, 0x2200) #set edge detection
            set_micro_register(self.haddr, 0x02) #0x2..only leading
#            set_micro_register(self.haddr, 0x03) #0x3..leading and trailing

            wait_set_micro_register(self.haddr, 0x1400) #Enable subtraction of trigger time
#            wait_set_micro_register(self.haddr, 0x1500) #Disable subtraction of trigger time

            wait_set_micro_register(self.haddr, 0x1600) #Read subtraction mode
#            wait_read_ok(self.haddr)
            read_micro_register(self.haddr) #Read value 
            read_micro_register(self.haddr) #Read value 
            read_micro_register(self.haddr) #Read value
            read_micro_register(self.haddr) #Read value 
            read_micro_register(self.haddr) #Read value 
            print ("Width, offset, extra search window, reject margin, trig time sub (1..enabled)")



            print(self.name, "initialized")
            print()

