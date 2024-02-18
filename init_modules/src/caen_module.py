cmdvme = "cmdvme"
yes_txt = ["yes", "Yes", "y", "true", "True"]

def printx(addr) :
    print(format(addr, "#x"))


class caen_module :
    def __init__(self, name, haddr, geo, in_use):
        self.name=name
        self.haddr=haddr
        self.geo =geo
        self.in_use= in_use in yes_txt or in_use
        self.firmware_revision=0

    def print(self):
        if self.in_use :
            print("",self.name)
        else :
            print("",self.name, "(not used)")
        print("VME Base Address  =", format(self.haddr, "#x"))
        print("Geometry (GEO) or MID =", self.geo)
        print("Firmware Revision =", self.firmware_revision)
        print()

