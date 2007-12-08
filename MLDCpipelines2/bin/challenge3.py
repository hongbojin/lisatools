#!/usr/bin/env python

import os
import sys
import random
from random import randint
from optparse import OptionParser

def run(command):
    commandline = command % globals()
    print "----> %s" % commandline
    
    try:
        assert(os.system(commandline) == 0)
    except:
        print 'Script %s failed at command "%s".' % (sys.argv[0],commandline)
        sys.exit(1)

mydir = os.path.dirname(os.path.abspath(sys.argv[0]))

parser = OptionParser(usage="usage: %prog [options] SEED...",
                      version="$Id: $")

parser.add_option("-P", "--nProc",
                  type="int", dest="nproc", default=1,
                  help="run in parallel on nproc CPUs [default 1]")

(options, args) = parser.parse_args()

nproc = options.nproc
seed  = int(args[0])

def twoseeds():
    return randint(1,10000000), randint(1,10000000)

seed31,seed31t = twoseeds()
seed32,seed32t = twoseeds()
seed33,seed33t = twoseeds()
seed34,seed34t = twoseeds()
seed35,seed35t = twoseeds()

run('%(mydir)s/challenge.py -P %(nproc)s            --seed=%(seed31)s  challenge3.1' % globals())
run('%(mydir)s/challenge.py -P %(nproc)s --training --seed=%(seed31t)s challenge3.1' % globals())
                                                                      
run('%(mydir)s/challenge.py -P %(nproc)s            --seed=%(seed32)s  challenge3.2' % globals())
run('%(mydir)s/challenge.py -P %(nproc)s --training --seed=%(seed32t)s challenge3.2' % globals())
                                                                      
run('%(mydir)s/challenge.py -P %(nproc)s            --seed=%(seed33)s  challenge3.3' % globals())
run('%(mydir)s/challenge.py -P %(nproc)s --training --seed=%(seed33t)s challenge3.3' % globals())

options34 = '--synthlisa --rawMeasurements --randomizeNoise=0.2 --laserNoise=10'
run('%(mydir)s/challenge.py -P %(nproc)s            --duration=2097152 --timeStep=1 --seed=%(seed34)s  %(options34)s challenge3.4' % globals())
run('%(mydir)s/challenge.py -P %(nproc)s --training --duration=2097152 --timeStep=1 --seed=%(seed34t)s %(options34)s challenge3.4' % globals())

run('%(mydir)s/challenge.py -P %(nproc)s            --duration=2097152 --timeStep=1 --seed=%(seed35)s  %(options34)s challenge3.5' % globals())
run('%(mydir)s/challenge.py -P %(nproc)s --training --duration=2097152 --timeStep=1 --seed=%(seed35t)s %(options34)s challenge3.5' % globals())