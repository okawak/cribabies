import subprocess
import caen_module

debug = True

def dec2hex(n) : 
    return format(n, "#x")

def call_cmdvme(haddr,subadd,val,val_name) :
    if not val and val is not 0 :
        cmd = caen_module.cmdvme+" -wr "+format(haddr+subadd, "#x")
    else: 
        cmd = caen_module.cmdvme+" -ww "+format(haddr+subadd, "#x")+ " " +  dec2hex(val)
    if debug :
            print (cmd + " (" + val_name + ")")

    return subprocess.check_output(cmd, shell=True, executable="/bin/bash")
    
# input_range
# 0 ->  4V
# 1 -> 10V
# 2 ->  8V
def set_input_range(haddr, val) :
    return call_cmdvme(haddr,0x6060, val,"Input range")

# input_range
# 0 -> 800ns conv.time
# 1 -> 1.6us conv.time
# 2 -> 3.2us conv. time
# 3 -> 6.4us conv.time
# 4 -> 8k, hires 12.5us conv.time

def set_resolution(haddr, val) :
    return call_cmdvme(haddr, 0x6042, val,"Resolution")

def get_firmware_revision(haddr) :
    return call_cmdvme(haddr,0x600e,"","Firmware Rev.")
 
def set_module_id(haddr, module_id) :
    return call_cmdvme(haddr,0x6004,module_id,"Module_id")
    

# def set_single_shot_reset(haddr, val) :
#     address_single_shot_reset = 0x1016
#     if debug :
#         print(caen_module.cmdvme+" -ww "+ dec2hex(haddr+address_single_shot_reset) +" "+  dec2hex(val), "(Single Shot Reset)")
#     subprocess.call(caen_module.cmdvme+" -ww "+ dec2hex(haddr+address_single_shot_reset) +" "+ dec2hex(val), shell=True, executable="/bin/bash")
# 
# def set_bit_clear2(haddr, val) :
#     address = 0x1034
#     if debug :
#         print(caen_module.cmdvme+" -ww "+ dec2hex(haddr+address) +" "+  dec2hex(val), "(Bit Clear 2)")
#     subprocess.call(caen_module.cmdvme+" -ww "+ dec2hex(haddr+address) +" "+  dec2hex(val), shell=True, executable="/bin/bash")
# 
# def set_bit_set2(haddr, val) :
#     address = 0x1032
#     if debug :
#         print(caen_module.cmdvme+" -ww "+ dec2hex(haddr+address) +" "+  dec2hex(val), "(Bit Set 2)")
#     subprocess.call(caen_module.cmdvme+" -ww "+ dec2hex(haddr+address) +" "+  dec2hex(val), shell=True, executable="/bin/bash")
# 

def set_thresholds(haddr, thresholds) :
    address = 0x4000
    for ch, th in enumerate(thresholds) :
        call_cmdvme(haddr,address+2*ch, th,"Threshold")


class madc32(caen_module.caen_module) :
    def __init__(self, conf) :
        super().__init__(conf["name"], conf["haddr"], conf["module_id"], conf["in use"])
        self.thresholds = conf["config"]["thresholds"]
        print (len(self.thresholds))
        if not len(self.thresholds) == 32 :
        	self.thresholds = [0] * 32
        self.firmware_revision = get_firmware_revision(self.haddr) #!
        self.input_range = conf["config"]["input_range"]
        self.resolution = conf["config"]["resolution"]

    def init(self) :
        if self.in_use :
            self.print()
            set_module_id(self.haddr, self.geo)
#           set_single_shot_reset(self.haddr, 1) # 1 is recommended
#            set_bit_clear2(self.haddr, 0x011c) # why this?
#            set_bit_set2(self.haddr, 0x0108) # why this?
            set_thresholds(self.haddr, self.thresholds)
            set_input_range(self.haddr, self.input_range)
            set_resolution(self.haddr, self.resolution)

            print(self.name, "initialized")
            print()
