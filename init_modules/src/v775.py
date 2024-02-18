import subprocess
import caen_module

debug = True


def dec2hex(n):
    return format(n, "#x")


def get_firmware_rivision(haddr):
    address_firmware_revision = 0x1000
    if debug:
        print(
            caen_module.cmdvme + " -wr " + dec2hex(haddr + address_firmware_revision),
            "(Getting Firmware Rivision)",
        )
    return subprocess.check_output(
        caen_module.cmdvme + " -wr " + format(haddr + address_firmware_revision, "#x"),
        shell=True,
        executable="/bin/bash",
    )


def set_full_scale_range(haddr, val):
    address = 0x1060
    if debug:
        print(
            caen_module.cmdvme
            + " -ww "
            + dec2hex(haddr + address)
            + " "
            + dec2hex(val),
            "(Full Scale Range)",
        )
    subprocess.call(
        caen_module.cmdvme + " -ww " + dec2hex(haddr + address) + " " + dec2hex(val),
        shell=True,
        executable="/bin/bash",
    )


def set_geometry(haddr, geo):
    address_set_geometry = 0x1002
    if debug:
        print(
            caen_module.cmdvme
            + " -ww "
            + dec2hex(haddr + address_set_geometry)
            + " "
            + dec2hex(geo),
            "(Setting geo)",
        )
    subprocess.call(
        caen_module.cmdvme
        + " -ww "
        + format(haddr + address_set_geometry, "#x")
        + " "
        + dec2hex(geo),
        shell=True,
        executable="/bin/bash",
    )


def set_single_shot_reset(haddr, val):
    address_single_shot_reset = 0x1016
    if debug:
        print(
            caen_module.cmdvme
            + " -ww "
            + dec2hex(haddr + address_single_shot_reset)
            + " "
            + dec2hex(val),
            "(Single Shot Reset",
        )
    subprocess.call(
        caen_module.cmdvme
        + " -ww "
        + dec2hex(haddr + address_single_shot_reset)
        + " "
        + dec2hex(val),
        shell=True,
        executable="/bin/bash",
    )


def set_bit_clear2(haddr, val):
    address = 0x1034
    if debug:
        print(
            caen_module.cmdvme
            + " -ww "
            + dec2hex(haddr + address)
            + " "
            + dec2hex(val),
            "(Bit Clear 2)",
        )
    subprocess.call(
        caen_module.cmdvme + " -ww " + dec2hex(haddr + address) + " " + dec2hex(val),
        shell=True,
        executable="/bin/bash",
    )


def set_bit_set2(haddr, val):
    address = 0x1032
    if debug:
        print(
            caen_module.cmdvme
            + " -ww "
            + dec2hex(haddr + address)
            + " "
            + dec2hex(val),
            "(Bit Set 2)",
        )
    subprocess.call(
        caen_module.cmdvme + " -ww " + dec2hex(haddr + address) + " " + dec2hex(val),
        shell=True,
        executable="/bin/bash",
    )


def set_zero_supression(haddr, thresholds):
    address = 0x1080
    for ch, th in enumerate(thresholds):
        if debug:
            print(
                caen_module.cmdvme
                + " -ww "
                + dec2hex(haddr + address + 2 * ch)
                + " "
                + dec2hex(th),
                "(Threshold)",
            )
        subprocess.call(
            caen_module.cmdvme
            + " -ww "
            + dec2hex(haddr + address + 2 * ch)
            + " "
            + dec2hex(th),
            shell=True,
            executable="/bin/bash",
        )


class v775(caen_module.caen_module):
    def __init__(self, conf):
        super().__init__(conf["name"], conf["haddr"], conf["geometry"], conf["in use"])
        self.thresholds = conf["config"]["thresholds"]
        self.fsr = conf["config"]["FSR"]
        if not len(self.thresholds) == 32:
            self.thresholds = [0] * 32
        self.firmware_revision = get_firmware_rivision(self.haddr)  #!

    def init(self):
        if self.in_use:
            self.print()
            set_geometry(self.haddr, self.geo)
            set_single_shot_reset(self.haddr, 1)  # 1 is recommended
            set_bit_clear2(self.haddr, 0x011C)  # why this?
            #            set_bit_set2(self.haddr, 0x0108) # why this?
            set_bit_set2(self.haddr, 0x1108)  # above + enable header even with no hit
            set_full_scale_range(self.haddr, self.fsr)
            set_zero_supression(self.haddr, self.thresholds)
            print(self.name, "initialized")
            print()
