#include "../interface/Ntuplizer.h"
#include "../interface/CandidateNtuplizer.h"
#include "../interface/JetsNtuplizer.h"
#include "../interface/GenJetsNtuplizer.h"
#include "../interface/MuonsNtuplizer.h"
#include "../interface/ElectronsNtuplizer.h"
#include "../interface/TausNtuplizer.h"
#include "../interface/METsNtuplizer.h"
#include "../interface/PileUpNtuplizer.h"
#include "../interface/GenEventNtuplizer.h"
#include "../interface/LHEEventNtuplizer.h"
#include "../interface/GenRunNtuplizer.h"
#include "../interface/GenParticlesNtuplizer.h"
#include "../interface/TriggersNtuplizer.h"
#include "../interface/VerticesNtuplizer.h"



///////////////////////////////////////////////////////////////////////////////////
Ntuplizer::Ntuplizer(const edm::ParameterSet& iConfig):

	geneventToken_        	    (consumes<GenEventInfoProduct>(iConfig.getParameter<edm::InputTag>("genEventInfo"))),     
	genrunToken_        	    (consumes<GenRunInfoProduct>(iConfig.getParameter<edm::InputTag>("genRunInfo"))),     
	lheToken_        	    (consumes<LHEEventProduct>(iConfig.getParameter<edm::InputTag>("LHEEventProduct"))),     

	genparticleToken_     	    (consumes<reco::GenParticleCollection>(iConfig.getParameter<edm::InputTag>("genparticles"))),
	genparticleTokenC_     	    (consumes<reco::CandidateView>(iConfig.getParameter<edm::InputTag>("genparticles"))),

    genJetToken_                (consumes<reco::GenJetCollection>(iConfig.getParameter<edm::InputTag>("genJets")))
	
{

	
  /*=======================================================================================*/
  edm::Service<TFileService> fs;
  TTree* tree = fs->make<TTree>( "tree", "tree" );
  
  std::map< std::string, bool > runFlags;
  runFlags["runOnMC"] = true; //iConfig.getParameter<bool>("runOnMC");
  runFlags["doGenParticles"] = iConfig.getParameter<bool>("doGenParticles");
  runFlags["doGenJets"] = iConfig.getParameter<bool>("doGenJets");
  runFlags["doGenEvent"] = iConfig.getParameter<bool>("doGenEvent");
  runFlags["doGenRun"] = iConfig.getParameter<bool>("doGenRun");

  nBranches_ = new NtupleBranches( runFlags, tree );
  
  /*=======================================================================================*/

  /*=======================================================================================*/    

  if (runFlags["doGenJets"]) 
    nTuplizers_["genJets"]   = new GenJetsNtuplizer   ( genJetToken_, nBranches_    );

  if (runFlags["doGenParticles"]) {
    std::vector<edm::EDGetTokenT<reco::GenParticleCollection>> genpTokens;
    genpTokens.push_back( genparticleToken_ );
//    genpTokens.push_back( genparticleTokenC_ );
   nTuplizers_["genParticles"] = new GenParticlesNtuplizer( genpTokens, genparticleTokenC_, nBranches_ );
  }

//  if (runFlags["doPileUp"]) {
//    std::vector<edm::EDGetTokenT< std::vector<PileupSummaryInfo> > > puTokens;
//    puTokens.push_back( puinfoToken_ );
//    nTuplizers_["PU"] = new PileUpNtuplizer( puTokens, nBranches_ );
//  }

  if (runFlags["doGenEvent"]) {
    std::vector<edm::EDGetTokenT< GenEventInfoProduct > > geneTokens;
    geneTokens.push_back( geneventToken_ );
    nTuplizers_["genEvent"] = new GenEventNtuplizer( geneTokens, nBranches_ );

    std::vector<edm::EDGetTokenT< LHEEventProduct > > lheTokens;
    lheTokens.push_back( lheToken_ );
    nTuplizers_["lheEvent"] = new LHEEventNtuplizer( lheTokens, nBranches_ );

 }

  if (runFlags["doGenRun"]) {
    std::vector<edm::EDGetTokenT< GenRunInfoProduct > > genrunTokens;
    genrunTokens.push_back( genrunToken_ );
    nTuplizers_["genRun"] = new GenRunNtuplizer( genrunTokens, nBranches_ );
  }

}

///////////////////////////////////////////////////////////////////////////////////
Ntuplizer::~Ntuplizer()
{
	  
   for( std::map<std::string,CandidateNtuplizer*>::iterator it = nTuplizers_.begin(); it != nTuplizers_.end(); ++it )
      delete it->second;
   
   nTuplizers_.clear();
   
   delete nBranches_;
   
}

///////////////////////////////////////////////////////////////////////////////////
void Ntuplizer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup){
  
  nBranches_->reset();

  nBranches_->EVENT_event     = iEvent.id().event();
     
  for( std::map<std::string,CandidateNtuplizer*>::iterator it = nTuplizers_.begin(); it != nTuplizers_.end(); ++it )
    (it->second)->fillBranches( iEvent, iSetup );
  
  nBranches_->fillTree();
  
  nBranches_->reset();    
  
}


///////////////////////////////////////////////////////////////////////////////////
void Ntuplizer::beginJob(){
}

///////////////////////////////////////////////////////////////////////////////////
void Ntuplizer::endJob() {
}

///////////////////////////////////////////////////////////////////////////////////
void Ntuplizer::beginRun(edm::Run const&, edm::EventSetup const&){
}


///////////////////////////////////////////////////////////////////////////////////
void Ntuplizer::endRun(edm::Run const&, edm::EventSetup const&){
}

///////////////////////////////////////////////////////////////////////////////////
void Ntuplizer::beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&){
}


///////////////////////////////////////////////////////////////////////////////////
void Ntuplizer::endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&){
}


///////////////////////////////////////////////////////////////////////////////////
void Ntuplizer::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(Ntuplizer);
