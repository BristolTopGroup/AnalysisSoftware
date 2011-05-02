from __future__ import division
import sys

def triggerInfo(trigger, logString):
    logString = logString.replace('\n', '')
    #remove empty entries
    entries = [token for token in logString.split(' ') if not token == '']
    info = {}
    info['name'] = entries[-1]
    info['eventsTotal'] = entries[3]
    info['eventsPassed'] = entries[4]
    info['eventsFailed'] = entries[5]
    info['efficiency'] = entries[4] / entries[3]
    return info

if __name__ == "__main__":
    args = sys.argv
    if len(args) == 3:
        file = open(args[1])
        trigger = args[2]
        for line in file.readlines():
            if trigger in line:
                info = triggerInfo(trigger, line)
                print info
                break;
        file.close()
        
