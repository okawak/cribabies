import sys
import yaml

import v775
import v785
import v1290
import madc32


if __name__ == '__main__':
    args = sys.argv
    module_list = args[1]

    with open(module_list, 'r') as yml:
        conf = yaml.safe_load(yml)
    for each in conf : 
        if each["type"] == "v785" : 
            module = v785.v785(each)
            module.init()
        if each["type"] == "v775" : 
            module = v775.v775(each)
            module.init()
        if each["type"] in ["v1190", "v1290"] : 
            module = v1290.v1290(each)
            module.init()
        if each["type"] == "madc32" : 
            module = madc32.madc32(each)
            module.init()
