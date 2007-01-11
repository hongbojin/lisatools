// EMRI.i, required by swig
// requires GSL
// SVN_ID = "$Id$"

%module EMRI
%{
#include "AKWaveform.hh"
%}

%include numpy_typemaps.i
%typemap(in) (double *hPlus ,long hPlusLength ) = (double *numpyarray,long numpyarraysize);
%typemap(in) (double *hCross,long hCrossLength) = (double *numpyarray,long numpyarraysize);

%include "cpointer.i"
%pointer_class(double, doublep);
%pointer_class(float, floatp);

%include "AKWaveform.hh"

%pythoncode %{
import lisaxml
import numpy
import math
import sys

SourceClassModules = {}
SourceClassModules['ExtremeMassRatioInspiral'] = 'EMRI'
lisaxml.SourceClassModules['ExtremeMassRatioInspiral'] = 'EMRI'

class XMLobject(object):
    def __init__(self):
        self.__dict__['parameters'] = []

    def __setattr__(self,attr,value):
        self.__dict__[attr] = value

        if (not attr in self.parameters) and (not '_Unit' in attr):
            self.parameters.append(attr)

class Source(XMLobject):
    def __init__(self,sourcetype,name=''):
        super(Source,self).__init__()

        # avoid calling setattr
        self.__dict__['xmltype'] = sourcetype
        self.__dict__['name'] = name

    def parstr(self,attr):
        value = getattr(self,attr)

        try:
            unit = getattr(self,attr+'_Unit')
        except AttributeError:
            unit = 'default'

        return "%s (%s)" % (value,unit)



class ExtremeMassRatioInspiral(lisaxml.Source):
    """Returns a MLDC source that models the waveform emitted by the
    extreme mass ratio nspiral."""

    # this is the list of parameters that will be recorded in the lisaXML SourceData sections
    # give ParameterName, DefaultUnit, DefaultValue (a string), Description


    outputlist = (('EclipticLatitude',                 'Radian',        None, 'standard ecliptic latitude'),
                  ('EclipticLongitude',                'Radian',        None, 'standard ecliptic longitude'),
                  ('Polarization',                     'Radian',        None, 'standard source polarization'),
                  ('PolarAngleOfSpin',                 'Radian',        None, 'polar angle of MBH spin'),
                  ('AzimuthalAngleOfSpin',             'Radian',        None, 'azimuthal direction of MBH spin'),
                  ('Spin',                             'MassSquared',   None, 'magnitude of (specific) spin (S) of MBH'),
                  ('MassOfCompactObject',              'SolarMass',     None, 'mass of the compact object'),
                  ('MassOfSMBH',                       'SolarMass',     None, 'mass of the MBH'),
                  ('PlungeTime', 		       'Seconds',       None, 'time of plunge'),
                  ('PlungeAzimuthalOrbitalPhase',      'Radian',        None, 'azimuthal orbital phase at plunge'),
                  ('EccentricityAtPlunge',             'Unit',          None, 'orbital eccentricity at plunge'),
                  ('PlungeTildeGamma',                 'Radian',        None, 'position of pericenter, as angle between LxS and pericenter at plunge'),
                  ('PlungeAlphaAngle',                 'Radian',        None, 'azimuthal direction of L (in the orbital plane) at plunge'),
                  ('LambdaAngle',                      'Radian',        None, 'angle between L and S'),
                  ('Distance',                         'Parsec',        None, 'standard source distance')
            )
    e0 = 0.0;
    nu0 = 0.0;
    gam0 = 0.0;
    phi0 = 0.0;
    al0 = 0.0;

    def  __init__(self,name=''):
        super(ExtremeMassRatioInspiral, self).__init__('ExtremeMassRatioInspiral',name)
    
    def waveforms(self,samples,deltat,inittime):
      
       duration = self.PlungeTime - inittime
       if (duration > deltat*(samples-1)):
           print "You have requested EMRI plunging beyond observation time. This is not implemented yet. Please contact S.Babak: stba@aei.mpg.de"
	   sys.exit(1)
	   
       emri = AKWaveform(self.Spin, self.MassOfCompactObject, self.MassOfSMBH, duration, deltat)

       emri.SetSourceLocation(self.EclipticLatitude, self.EclipticLongitude, self.PolarAngleOfSpin, \
                              self.AzimuthalAngleOfSpin, self.Distance)
			      
       emri.EvolveOrbit(inittime,  self.EccentricityAtPlunge, self.PlungeTildeGamma,\
                        self.PlungeAzimuthalOrbitalPhase, self.PlungeAlphaAngle, self.LambdaAngle)

       hp = numpy.empty(samples,'d')
       hc = numpy.empty(samples,'d')
       wavelen = emri.GetWaveform(self.Polarization, hp, hc)

       hp[wavelen:] = 0.0
       hc[wavelen:] = 0.0 
       
       nut = doublep()
       et = doublep()
       gamt = doublep()
       pht = doublep()
       alt = doublep()
       emri.GetOrbitalParams(0.0, nut, et, gamt, pht, alt)

       [self.nu0, self.e0, self.gam0, self.phi0, self.al0] = [nut.value(), et.value(), gamt.value(), pht.value(), alt.value()]


       return (hp,hc)
       
    def GetOrbitalParameters(self):
       
       	return [self.nu0, self.e0, self.gam0, self.phi0, self.al0]
       
# utility function

def EMRIEstimateInitialOrbit(spin,mu,MBHmass,Tend,e_lso,nu_lso): 
   """Estimates the initial eccentricity and orbital frequency for the orbit
specified at the plunge by e_lso and nu_lso, of approximate duration Tend."""
       
   ak = AKWaveform(spin, mu, MBHmass, 3.e8, 15.)

   ein = floatp()
   nuin = floatp()
   ak.EstimateInitialParams(Tend,e_lso,nu_lso,ein,nuin)

   return[ein.value(), nuin.value()]

%}
