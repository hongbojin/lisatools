// $Id: LISACODE-ConfigSim.h,v 1.11 2007/04/20 12:24:41 ruiloba Exp $
/*
 *  LISACODE-ConfigSim.h
 *  V 1.4
 *  LISACode
 *
 *  Description :
 *  -------------
 *  Description of model
 *
 *
 *  Created on 30/06/05 by A.Petiteau (APC)
 *  Last modification on 06/06/06 by A.Petiteau, E. Plagnol (APC)
 *
 */

#ifndef __CONFIGSIM_H
#define __CONFIGSIM_H


#include <iostream.h>
#include <stdexcept>
#include <math.h>
#include <fstream>
#include <sstream>
#include <string>
#include <iomanip.h>
#include "ezxml.h"
#include "LISACODE-LISAConstants.h"
#include "LISACODE-Tools.h"
#include "LISACODE-Serie.h"
#include "LISACODE-GW.h"
#include "LISACODE-GWMono.h"
#include "LISACODE-GWBinary.h"
#include "LISACODE-GWNewton2.h"
#include "LISACODE-GWFile.h"
#include "LISACODE-GWFile2.h"
#include "LISACODE-GWPeriGate.h"
#include "LISACODE-GWSto.h"
#include "LISACODE-GWFastSpinBBH.h"
#include "LISACODE-GWCusp.h"
#include "LISACODE-GWNew.h"
#include "LISACODE-Background.h"
#include "LISACODE-BackgroundGalactic.h"
#include "LISACODE-Noise.h"
#include "LISACODE-NoiseWhite.h"
#include "LISACODE-NoiseFilter.h"
#include "LISACODE-NoiseFShape.h"
#include "LISACODE-NoiseOof.h"
#include "LISACODE-NoiseTwoFilter.h"
#include "LISACODE-NoiseFile.h"
#include "LISACODE-USOClock.h"
#include "LISACODE-Filter.h"
#include "LISACODE-Geometry.h"
#include "LISACODE-GeometryAnalytic.h"
#include "LISACODE-GeometryMLDC.h"
#include "LISACODE-GeometryFile.h"
/*!\defgroup input_data Input data (directory Input_data)
 *\{
 */
/*!\brief Description of model
 */


using namespace std;
/*!\brief Noise specification structure
 */


struct NoiseSpec{
	/*!\brief Type of Noise.
	 *
	 * The possible values for the nois type are:
	 * \arg 0: No noise.
	 * \arg 1: White noise.
	 * \arg 2: Noise read from a file.
	 * \arg 3: Noise generated by filtering a white noise (undefined filter : parameters are given). 
	 * \arg 4: Noise generated by filtering a white noise (filter parameters are computed for 1/frequency noise; only noise level is given).
	 * \arg 5: Noise generated by filtering a white noise (filter parameters are computed for frequency noise; only noise level is given).
	 * \arg 6: Noise generated by filtering a white noise (filter parameters are computed for frequency noise; only noise level is given; noise is proportional to arms length and square root power too ).
	 */
	int NType; // 0: No noise, 1 : White, 2 : File, 3 : Filter, 4 : 1of, 5 : f, 6 : fLosP
	/*!\brief Noise level (used if #NType=4,5 or 6) */
	double NVal0, NVal01;
	//double Slope_ep,Tsample_ep,Fknee_ep,Fmin_ep;
	/*!\brief \f$ \alpha \f$ recursive coefficients (used if #NType=3) */
	vector<double> NVal1;
	/*!\brief \f$ \beta \f$ recursive coefficients (used if #NType=3) */
	vector<double> NVal2;
	/*!\brief Filename (used if #NType=2) */
	char NStr[256];
};

/*! \brief Class to configure LISA simulation, that is, LISACode execution.
 */
class ConfigSim
	{
	private:
		Tools * MT;
		/*! \brief File name of simulation parameters. It is called 
		 *	configuration file.
		 */
		char * ConfigFileName;
		char * SystemEncoding;
		int SystemEncoding_int;
		string Author,GenerationDate,GenerationType,TDIParamName,TDIParamNameType;
		string SC1ParamName,SC2ParamName,SC3ParamName,Simulator;
		char XmlOutputFile[256] ;
		ofstream FichXML;
		// All the next variables represents a time parameter.*/ 
		/*!\var tStepPhy
		 *\brief Physical time step.
		 * 
		 * It is the smaller time step of the simulation. 
		 * It is used to simulate continuous phenomena. 
		 */
		/*!\var tMax
		 *\brief It is the maximal simulation duration time.
		 */ 
		/*!\var tStepMes
		 *\brief It is the time step between two phasemeter measures.
		 *
		 * It corresponds to the time step between two output data samples.
		 */
		/*!\var tMemNoiseFirst
		 *\brief time shift between noise computation time and current time.
		 *
		 * Noises are computed tMemNoiseFirst second(s) before current time.
		 */
		/*!\var tMemNoiseLast
		 *\brief time shift between time of last computed noise and current time.
		 *
		 * later noises are eleminated
		 */ 
		/*! \var tMemSig
		 * \brief Duration of satellite memory for signal storage.
		 */
		/*! \var tDisplay
		 * \brief Time step for screen display. 
		 */
		/*! \var tDeltaTDIDelay
		 * \brief Inaccuracy on wave time propagation with a length of a LISA arm.
		 *
		 * It is an error added to the exact time propagation before to its use by TDI.  
		 */
		
		double tStepPhy, tStepMes;
		double tOffset, tMax, tDur;
		double tMemNoiseFirst, tMemNoiseLast, tMemSig;
		double tDeltaTDIDelay;
		double tDisplay;
		
		
		int GlobalRandomSeed;
		int Endian ;
		bool BigEndian ;
		
		/*!\brief TDI interpolator type */
		INTERP TDIInterp;
		/*!\brief Value used for TDI interpolation */
		double TDIInterpVal;
		//double Slope_ep,Tsample_ep,Fknee_ep,Fmin_ep;
		
		/*!\brief Nominal LISA arm length. */
		double Armlength;
		
		/*!\brief  Orbits start time.
		 * 
		 * This parameter allows to start simulation with a satellites position different
		 * to base configuration. In the base configuration satellite 1 is on \f$ x \f$ axis
		 * under the ecliptic plan. Satellites 2 and 3 are over the ecliptic plan, 2 in \f$ y<0 \f$
		 * and 3 in  \f$ y>0 \f$ .
		 */
		double OrbStartTime;
		/*!\brief Angle (radians) giving rotation of the satellites triangle 
		 * (from the basical position) at initial time (t=0).
		 *    
		 * Basical position (OrbInitRot=0) is a triangle pointing to the bottom with 
		 * satellite 1 on the bottom, satellite 2 on negative Y axis (on the right hand side 
		 * seeing on X axis) and satelite 3 on Y positive axis (on the left hand side seeing 
		 * on X axis).\n
		 * OrbInitRot not null gives rotation angle to obtain the new starting position of
		 * the satellites. 
		 *
		 */
		double OrbInitRot;
		
		
		/*!\brief
		 * Satellites motion.
		 *
		 * It indicates the type of orbits :
		 * \arg 0  : Analytical LISACode standard orbits (from gr-qc_0410093).
		 * \arg 1  : MLDC orbits (from PRD_71_022001).
		 * \arg 2  : ESA orbits read in file (default 'Orbit_ESA.input.txt').
		 
		 */
		int OrbType;
		
		/*!\brief File describing satellite motion */
		char OrbFile[512];
		
		/*! \brief  Order for time propagation computing.
		 *
		 * This parameter specifies the order to compute the round-trip time of photons
		 * between two satellites.
		 * The possible order values are : 
		 * \arg 0  : classical computing
		 * \arg 1  : time propagation is computed considering Sagnac effect.
		 * \arg 2  : time propagation is computed using general relativity.
		 */
		int OrbOrder;
		
		/*! \brief  Orbits approximation.
		 *
		 * The possible order values are : 
		 * \arg 0  : static, no motion of satellites
		 * \arg 1  : rigid.
		 * \arg 2  : eccentric.
		 */
		int OrbApprox;
		
		/*!\brief Approximated TDI delays computation or not.  */
		bool TDIDelayApprox;
		/*!\brief  Laser Power in Watts.
		 *
		 * It specifies the beam power at the laser output. 
		 */
		double LaserPower;
		/*! \brief Phasemeter Filter (On, Off).
		 *
		 * It specifies if a low pass filter is applied to the phasemeters signals.\n
		 * If variable is set to 1 the signals are filtered. 
		 */
		bool PhaMetFilterON;
		/*!\brief Phasemeter Filter Parameters
		 *
		 *  Phasemeter Filter Parameters :
		 *  \arg attenuation [dB]
		 *  \arg oscillations in bandwidth [dB]
		 *  \arg low transition frequency in factor of frequency of measurment 
		 *  \arg high transition frequency in factor of frequency of measurment
		 */
		vector<double> PhaMetFilterParam; // Parameters of filter : attenuation [dB], oscillations in bandwidth [dB], low transition frequency in factor of frequency of measurment ,  high transition frequency in factor of frequency of measurment
		bool InternalPhasemeters;
		/*!\brief  Gravitational Waves Parameters  */
		vector< GW *> GWs;
		/*!\brief #Background signals */
		Background * GWB;
		/*!\brief noise specifications*/
		vector < vector <NoiseSpec> > NoisesData;
		/*!\brief satellites noise */
		vector <Noise *> Noises;
		/*!\brief  satellites USO time*/
		vector<USOClock> USOs;
		/*! \var FileNameSigSC1
		 * \brief FileName for spacecraft 1 Signal.
		 *
		 * 4 phasemeters data are stored in this file.
		 */ 
		/*! \var FileNameSigSC2
		 * \brief FileName for spacecraft 2 Signal.
		 *
		 * 4 phasemeters data are stored in this file.
		 */ 
		/*! \var FileNameSigSC3
		 * \brief FileName for spacecraft 3 Signal.
		 *
		 * 4 phasemeters data are stored in this file.
		 */ 
		/*! \var FileNameTDI
		 * \brief FileName for TDI generators.
		 *
		 * All TDI generators data
		 */ 
		/*! \var FileNameDelays
		 * \brief FileName for Delays.
		 *
		 * 6 time delays between satellites are stored in this file.
		 */ 
		/*! \var FileNamePositions
		 * \brief FileName for Positions.
		 */ 
		char FileNameSigSC1[256], FileNameSigSC2[256], FileNameSigSC3[256], FileNameTDI[256], FileNameDelays[256], FileNamePositions[256];
		/*! \brief It returns  #FileName SigSCn attribute.*/
		/*! \var FileEncodindingSigSC1
		 * \brief FileEncodinding for spacecraft 1 Signal.
		 *
		 * 0 for ASCII , 1 for binary
		 */ 
		/*! \var FileEncodindingSigSC2
		 * \brief FileEncodinding for spacecraft 2 Signal.
		 *
		 * 0 for ASCII , 1 for binary
		 */ 
		/*! \var FileEncodindingSigSC3
		 * \brief FileEncodinding for spacecraft 3 Signal.
		 *
		 * 0 for ASCII , 1 for binary
		 */ 
		/*! \var FileEncodindingTDI
		 * \brief FileEncodinding for TDI generators.
		 *
		 * * 0 for ASCII , 1 for binary
		 */ 
		/*! \var FileEncodindingDelays
		 * \brief FileEncodinding for Delays.
		 *
		 * 0 for ASCII , 1 for binary
		 */ 
		/*! \var FileEncodindingPositions
		 * \brief FileEncodinding for Positions.
		 */ 
		int FileEncodingSC1, FileEncodingSC2, FileEncodingSC3, FileEncodingTDI, FileEncodingDelays, FileEncodingPos ;
		/*!\brief Name of TDI. */
		vector<string> TDIsName;
		/*!\brief Vector of TDI data. */
		vector< vector<int> > TDIsPacks;
		/*!\brief Vector of TDI data. */
		vector< vector<double> > TDIsPacksFact;
		/*!\brief Maximum Delays Number. */
		int NbMaxDelays;
		
	public:
		/* Constructor */
		/* Documentation in .cpp*/
		ConfigSim();
		ConfigSim(char * NameReadFile_n);
		
		/*  Destructor */
		/* Documentation in .cpp*/
		~ConfigSim();
		
		/* Access methods */
		void DefaultConfig(char * NameConfigFile_n);
		/*! \brief It returns the value of #GlobalRandomSeed attribute.*/
		int getGlobalRandomSeed() {return(GlobalRandomSeed);};
		/*! \brief It returns physical time step, that is the value of #tStepPhy attribute.*/
		double gettStepPhy() {return(tStepPhy);};
		/*! \brief It returns maximal time, that is the value of #tMax attribute.*/
		double gettMax() {return(tMax);};
		/*! \brief Return tie offset.*/
		double gettOffset() {return(tOffset);};
		/*! \brief It returns duration.*/
		double gettDur() {return(tDur);};
		/*! \brief It returns  #tStepMes attribute.*/
		double gettStepMes() {return(tStepMes);};
		/*! \brief It returns  #tMemNoiseFirst attribute.*/
		double gettMemNoiseFirst() {return(tMemNoiseFirst);};
		/*! \brief It returns  #tMemNoiseLast attribute.*/
		double gettMemNoiseLast() {return(tMemNoiseLast);};
		/*! \brief It returns  #tMemSig attribute.*/
		double gettMemSig() {return(tMemSig);};
		/*! \brief It returns is the value of #tDisplay attribute.*/
		double gettDisplay() {return(tDisplay);};
		/*! \brief It returns  #tDeltaTDIDelay attribute.*/
		double gettDeltaTDIDelay() {return(tDeltaTDIDelay);};
		/*! \brief It returns  #TDIInterp attribute.*/
		INTERP getTDIInterp() {return(TDIInterp);};
		/*! \brief It returns  #TDIInterpVal attribute.*/
		double getTDIInterpVal() {return(TDIInterpVal);};
		/*! \brief It returns the value of #Armlength attribute.*/
		double getArmlength() {return(Armlength);};
		/*! \brief It returns  #OrbStartTime attribute.*/
		double getOrbStartTime() {return(OrbStartTime);};
		/*! \brief It returns  #OrbInitRot attribute.*/
		double getOrbInitRot() {return(OrbInitRot);};
		/*! \brief It returns  #TDIDelayApprox attribute.*/
		bool getTDIDelayApprox() {return(TDIDelayApprox);};
		/*! \brief It returns the value of #OrbType attribute.*/
		int getOrbType() {return(OrbType);};
		/*! \brief It returns the value of #OrbOrder attribute.*/
		int getOrbOrder() {return(OrbOrder);};
		/*! \brief It returns the value of #OrbApprox attribute.*/
		int getOrbApprox() {return(OrbApprox);};
		/* Documentation in .cpp*/
		void getGeometry(Geometry * & SCPos);
		/*! \brief It returns the laser power, that is the value of #LaserPower attribute.*/
		double getLaserPower() {return(LaserPower);};
		/*! \brief It returns 1 if a filter is applied in phasemeters,
		 * that is the value of #PhaMetFilterON attribute.*/
		bool getPhaMetFilter() {return(PhaMetFilterON);};
		/*! \brief It returns  #PhaMetFilterParam attribute.*/
		vector<double> getPhaMetFilterParam() {return(PhaMetFilterParam);};
		/*! \brief It returns  #InternalPhasemeters attribute.*/
		bool getInternalPhasemeters() {return(InternalPhasemeters);};
		/*! \brief It returns the first element of the vector #GWs.*/
		GW * getGW() {return(GWs[0]);};
		/*! \brief It returns a pointer to the #GWs attribute.*/
		vector<GW *> * getGWs() {return(&GWs);};
		/* Documentation in .cpp*/
		char * getFileNameSig(int iSC);
		/*! \brief It returns  #FileNameTDI attribute.*/
		char * getFileNameTDI() {return(FileNameTDI);};
		/*! \brief It returns  SystemEncoding , BigEndian or LittleEndian.*/
		char * getSystemEncoding() {return(SystemEncoding);};
		/*! \brief It returns  #FileNameDelays attribute.*/
		char * getFileNameDelays() {return(FileNameDelays);};
		/*! \brief It returns  #FileNamePositions attribute.*/
		char * getFileNamePositions() {return(FileNamePositions);};
		/* Documentation in .cpp*/
		int getFileEncodingSig(int iSC);
		/*! \brief It returns  #FileEncodingTDI attribute.*/
		int getFileEncodingTDI() {return(FileEncodingTDI);};
		/*! \brief It returns  #FileEncodingDelays attribute.*/
		int getFileEncodingDelays() {return(FileEncodingDelays);};
		/*! \brief It returns  #FileEncodingPositions attribute.*/
		int getFileEncodingPositions() {return(FileEncodingPos);};
		/*! \brief It returns  #Noises attribute.*/
		vector<Noise *> getNoises() {return(Noises);};
		/*! \brief It returns  #GWB attribute.*/
		Background * getGWB() {return(GWB);};
		/*! \brief It returns  #USOs attribute.*/
		vector<USOClock> getUSOs() {return(USOs);};
		/*! \brief It returns  #TDIsName attribute.*/
		int NbGenTDI() {return(TDIsName.size());};
		/* Documentation in .cpp*/
		char * getNameGenTDI(int iGen);
		/* Documentation in .cpp*/
		vector<int> getGenTDIPacks(int iGen);
		/* Documentation in .cpp*/
		vector<double> getGenTDIPacksFact(int iGen);
		/*! \brief It returns  #NbMaxDelays attribute plus 1.*/
		int getNbMaxDelays(){return(NbMaxDelays+1);};
		char * getXmlOutputFile() {return(XmlOutputFile);};
		string getAuthor() {return(Author);};
		string getGenerationDate() {return(GenerationDate);};
		string getGenerationType() {return(GenerationType);};
		string getTDIParamName() {return(TDIParamName);};
		string getTDIParamNameType() {return(TDIParamNameType);};
		string getSC1ParamName() {return(SC1ParamName);};
		string getSC2ParamName() {return(SC2ParamName);};
		string getSC3ParamName() {return(SC3ParamName);};
		string getSimulator() {return(Simulator);};
		char * getFileNameSigSC1() {return(FileNameSigSC1);};
		char * getFileNameSigSC2() {return(FileNameSigSC2);};
		char * getFileNameSigSC3() {return(FileNameSigSC3);};
		
		/*! \brief Set maximal time #tMax and duration #tDur according to offset time (should be define BEFORE) */
		void settMax(double tMax_n);
		/*! \brief Set duration #tDur andmaximal time #tMax according to offset time (should be define BEFORE) */
		void settDur(double tDur_n);
		
		// **  Others methods ** //	
		/*! \brief Read the configuration file. */
		void ReadFile();
		/*! \brief  Read the ASCII configuration file. */
		void ReadASCIIFile();
		/*! \brief Read the ASCII configuration file. */
		void ReadXMLFile();
		/*! \brief Creates Xml Output File as header of binary file */
		void CreateXmlOutputFile();
		/*! \brief Write in XML output file the bloc corresponding to an output data file */
		void AddTimeSeriesInXMLOutput(ofstream * FichXML, string ind1, string ObsDescr, double tOffset, int NRec, int DataFileEncoding, char * DataFileName);
		/*! \brief Return unit */
		char * gXMLUnit(const char In[], double & Fact);
		/*! \brief Return a time value read in XML file. Unit is Second. */
		double gXMLTime(ezxml_t param);
		/*! \brief Return an angle value read in XML file. Unit is Radian. */
		double gXMLAngle(ezxml_t param);
		/*! \brief Return an frequency value read in XML file. Unit is Hertz. */
		double gXMLFrequency(ezxml_t param);
		/*! \brief Return an astronomic mass value read in XML file. Unit is SolarMass. */
		double gXMLAstroMass(ezxml_t param);
		/*! \brief Return an astronomic distance value read in XML file. Unit is KiloParsec. */
		double gXMLAstroDistance(ezxml_t param);
		/*! \brief Return the first word read in bloc XML file */
		char * gXMLWord(ezxml_t param);
		/*! \brief Return the real value read in bloc XML file */
		double gXMLdouble(ezxml_t param);
		/*! \brief Return the string read in bloc XML file */
		string gXMLstring(ezxml_t param);
		/*! \brief Return the integer in read in bloc XML file */
		int gXMLint(ezxml_t param);
		//char * gXMLTimeSeries(ezxml_t series, const char * type, const char * encoding, int & length, int & record); // Return timeseries parameters for an input file.
		/*! \brief Return timeseries parameters for an input file.  */
		char * gXMLTimeSeries(ezxml_t series, int & type, int & encoding, int & length, int & record); 
		/*! \brief Place a noise in the noises' list */
		void NoisePlace(NoiseSpec tmp_noise, int iSC, int IndDir, int InstrumentIndex);
		/*! \brief Creation of noise's object. */
		void NoisesCreation();
		/*! \brief Maximal time travel for one delay. */
		double tMaxDelay();
		/*! \brief Minimal time travel for one delay. */
		double tMinDelay();
		/*! \brief Return the time the memory time during which data must be saved for apply TDI interpolation */
		double tMemNecInterpTDI();
		/*! \brief Offset between the start of data reading and the start of TDI application */
		double gettTDIShift() { return(2.0*MAX(0.0, tMemNecInterpTDI() - tMinDelay()) ); };
		/*! \brief Required time storage in phasemeters */
		double gettMemPhasemeters() { return(gettTDIShift() + tMemNecInterpTDI() + tMaxDelay()); };
		/*! \brief Reauired time storage in TDI */
		double gettMemTDI() { return(gettTDIShift() + tMemNecInterpTDI() + getNbMaxDelays() * tMaxDelay()); };
		/*! \brief Starting time for phasemeters */
		double gettStartPhasemeters() { return(-1.0 * ( gettMemPhasemeters() + gettMemTDI() + gettTDIShift() ) ); };
		/*! \brief Return true if there are no noises */
		bool getNoNoise();
		/*! \brief Return true if internal phasemeter must be used */
		bool UseInternalPhasemeter();
		int testbyteorder();
		/*! \brief Convert word in uppercase word */ 
		char * uppercase(const char *);
		/*! \brief Convert letter in lower case in uppercase letter */ 
		void majuscule(char &);
		string upS(string Str);
		bool scmp(const string str1, const string str2);
		/*! \brief Makes a copy of the string "orig", stripping all whitespace-type characters  */
		char * stripcopy(const char *orig);
		/*! \brief Find if generatorname is known */
		bool FindTDIName(const char * generatorname, vector<int> & tmp_TDIPacks, vector<double> & tmp_TDIPacksFact);
	};

/*\}*/
#endif // __CONFIGSIM_H

// end of LISACODE-ConfigSim.h

