####### Process initialization ##########

import FWCore.ParameterSet.Config as cms

process = cms.Process("Ntuple")

#process.load("Configuration.StandardSequences.MagneticField_cff")
#process.load('Configuration.Geometry.GeometryRecoDB_cff')
process.load("FWCore.MessageService.MessageLogger_cfi")

process.options   = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )

process.MessageLogger = cms.Service(
    "MessageLogger",
    destinations = cms.untracked.vstring(
        'LOG',
    ),
    LOG = cms.untracked.PSet(
        threshold  = cms.untracked.string('WARNING'), # DEBUG 
        filename  = cms.untracked.string('log.log')
    ),
    debugModules = cms.untracked.vstring('*'),
       
    statistics     = cms.untracked.vstring('STAT'),
        STAT = cms.untracked.PSet(
            threshold = cms.untracked.string('WARNING'),
            filename  = cms.untracked.string('stats.log')
    )
)


process.TFileService = cms.Service("TFileService",
                                    fileName = cms.string('flat_tree.root')
                                   )

from ZZjj_Analyzer.Ntuplizer.ntuplizerOptions_MC_cfi import config
				   
####### Config parser ##########

import FWCore.ParameterSet.VarParsing as VarParsing

options = VarParsing.VarParsing ('analysis')

options.maxEvents = -1

import glob
files = glob.glob('/home/llr/cms/pigard/data/Pythia/*.root')

file_list = []
for f in files :
    f = 'file:' + f
    file_list.append(f)

#print file_list
options.inputFiles =  file_list#
#options.inputFiles = "file:/home/llr/cms/pigard/data/MG_testrun/pythia_testrun/QCD_40_LHE_pythia_34.root"

#file_list#'file:/home/llr/cms/pigard/gridpack-generation/genproductions/bin/MadGraph5_aMCatNLO/ZZjj_ewk/ZZjj_ewk_gridpack/ZZjj_ewk_testrun_pyhtia_0.root'
#options.inputFiles = 'file:vbf.root'#68791C0A-3013-E511-88FD-D4AE5269F5FF.root'
#'/store/mc/RunIISpring15DR74/VBF_HToZZTo4L_M125_13TeV_powheg_JHUgen_pythia8/MINIAODSIM/Asympt25ns_MCRUN2_74_V9-v1/00000/68791C0A-3013-E511-88FD-D4AE5269F5FF.root' 
#'/store/mc/RunIISpring15DR74/VBF_HToZZTo4L_M125_13TeV_powheg_JHUgen_pythia8/AODSIM/Asympt25ns_MCRUN2_74_V9-v1/00000/9A2EDBC1-2713-E511-BA83-001EC9B0AD32.root'
#'/store/mc/RunIIFall15DR76/VBF_HToZZTo4L_M125_13TeV_powheg2_JHUgenV6_pythia8/AODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/00000/08D8604E-ECA6-E511-B19B-001E67397751.root'
options.parseArguments()

process.options  = cms.untracked.PSet( 
                     wantSummary = cms.untracked.bool(False),
                     #SkipEvent = cms.untracked.vstring('ProductNotFound'),
                     )

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(options.maxEvents) )

process.source = cms.Source("PoolSource",
                            fileNames = cms.untracked.vstring(options.inputFiles),
#                            firstEvent = cms.untracked.uint32(103),
#                            eventsToProcess = cms.untracked.VEventRange('1:103')
                           )
                                                 
process.source.duplicateCheckMode = cms.untracked.string('noDuplicateCheck')

################## Ntuplizer ###################
process.ntuplizer = cms.EDAnalyzer("Ntuplizer",
    doGenParticles    = cms.bool(True),
    doGenJets	      = cms.bool(True),
    doGenEvent	      = cms.bool(True),
    doGenRun	      = cms.bool(False),

#    genJets = cms.InputTag("slimmedGenJets"),
#    genparticles = cms.InputTag("prunedGenParticles"),
    LHEEventProduct = cms.InputTag("externalLHEProducer"), 
    genJets = cms.InputTag("ak4GenJets"),
    genparticles = cms.InputTag("genParticles"),
    genEventInfo = cms.InputTag("generator"),
    genRunInfo = cms.InputTag("generator")
)

#process.dump=cms.EDAnalyzer('EventContentAnalyzer')
#process.load("SimGeneral.HepPDTESSource.pythiapdt_cfi")
#process.printTree = cms.EDAnalyzer("ParticleListDrawer",
#  maxEventsToPrint = cms.untracked.int32(1),
#  printVertex = cms.untracked.bool(False),
#  printOnlyHardInteraction = cms.untracked.bool(False), # Print only status=3 particles. This will not work for Pythia8, which does not have any such particles.
#  src = cms.InputTag("genParticles")
#)
####### Final path ##########
process.p = cms.Path()
#process.p += process.printTree
#process.p += process.dump
process.p += process.ntuplizer
