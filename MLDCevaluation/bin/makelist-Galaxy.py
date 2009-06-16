#!/usr/bin/env python

__version__='$Id: $'

import lisaxml2 as lisaxml

from optparse import OptionParser

parser = OptionParser(usage="usage: %prog [options] INPUT.txt OUTPUT.dat",
                      version="$Id: $")

parser.add_option("-c", "--addCount",
                  action="store_true", dest="addCount", default=False,
                  help="add a src count at the beginning of each line (off by default)")

parser.add_option("-N", "--firstN",
                  type="int", dest="firstN", default=None,
                  help="limit total number of lines printed")

(options, args) = parser.parse_args()

if len(args) != 2:
    parser.error("You must specify the input lisaXML file and the output text file!")

lisaxml.Table.DisableRemoteTableLoad = True
    
srcfile = lisaxml.lisaXML(args[0],'r')
table   = srcfile.SourceData[0]

outfile = open(args[1],'w')

count = 0
for s in table.makeTableSources():
    if options.addCount:    
        print >> outfile, count,
    print >> outfile, s.Frequency, s.EclipticLatitude, s.EclipticLongitude, s.Amplitude,
    print >> outfile, s.Inclination, s.Polarization, s.InitialPhase, s.FrequencyDerivative
    
    count = count + 1
    if count == options.firstN:
        break

outfile.close()