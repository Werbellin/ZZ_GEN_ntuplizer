#include "../interface/GenParticlesNtuplizer.h"
 
//===================================================================================================================        
GenParticlesNtuplizer::GenParticlesNtuplizer( std::vector<edm::EDGetTokenT<reco::GenParticleCollection>> tokens, edm::EDGetTokenT<reco::CandidateView> tokensC, NtupleBranches* nBranches ) 
   : CandidateNtuplizer( nBranches )
   , genParticlesToken_( tokens[0] )
   , genParticlesTokenC_( tokensC) 
{

}

//===================================================================================================================        
GenParticlesNtuplizer::~GenParticlesNtuplizer( void )
{
}

const reco::Candidate* getMotherZpointer(const reco::CandidateView& genParticles,const vector<const reco::Candidate *>& cands, const reco::Candidate* particle, int& index) {
   // edm::LogError("") << "id: "<< particle->pdgId() << "  pt: " << particle->pt();
   if(particle->pdgId() == 23)  {
    index = std::distance( cands.begin(), find(cands.begin(), cands.end(), particle));
 //    edm::LogError("") << "found Z! at" << particle << "   index:" <<index;
     return &*particle; 
   }
   const reco::Candidate* mother = nullptr;
    
   if(particle->numberOfMothers() != 0) {
        mother = particle->mother(0);
        getMotherZpointer(genParticles, cands,  mother, index);
   }      
   return nullptr;
}

int getMotherZIndex(const reco::CandidateView& genParticles, const vector<const reco::Candidate *>& cands, const reco::Candidate* particle) {
    int index = -2;
    //const reco::Candidate* mother = 
    getMotherZpointer(genParticles, cands, particle, index);
//    edm::LogError("") << "mother: " << mother;
//    int index = -2;
//    if(mother == nullptr) { 
//        index = -1;
//    } else {
//        index = std::distance( cands.begin(), find(cands.begin(), cands.end(), mother));
//    }
    return index;
}


NewLorentzVector getFSRPhotonSum(const reco::CandidateView& genParticles, const vector<const reco::Candidate *>& cands, const reco::Candidate* postFSRLepton) {

    NewLorentzVector sumFSR = NewLorentzVector(0, 0, 0, 0);

    
    const reco::Candidate* preFSRLepton = postFSRLepton;
    while(preFSRLepton->numberOfMothers() != 0) {
        if(preFSRLepton->pdgId() == preFSRLepton->mother(0)->pdgId()) {
            preFSRLepton = preFSRLepton->mother(0);
        } else {
            break;
        }
    }
    //edm::LogError("") << "post FSR index:" << std::distance( cands.begin(), find(cands.begin(), cands.end(), postFSRLepton));
    //edm::LogError("") << "pre FSR index:" << std::distance( cands.begin(), find(cands.begin(), cands.end(), preFSRLepton));

    sumFSR = preFSRLepton->p4() - postFSRLepton->p4();

    return sumFSR;
}
      vector<NewLorentzVector> _mc_gen_ele_p4;

//===================================================================================================================        
  void GenParticlesNtuplizer::fillBranches( edm::Event const & event, const edm::EventSetup& iSetup ){
  
    event.getByToken(genParticlesToken_ , genParticles_); 
    event.getByToken(genParticlesTokenC_ , genParticlesC_); 

   /* here we want to save  gen particles info*/

    vector<const reco::Candidate *> cands;
    vector<const reco::Candidate *>::const_iterator found = cands.begin();
    for(reco::CandidateView::const_iterator p = genParticlesC_->begin();
        p != genParticlesC_->end(); ++ p) {
      cands.push_back(&*p);
    }

    std::vector<int> vDau ;
    std::vector<int> vMoth;
    std::vector<int> vMothIdx;
    int nMoth = 0;
    int nDau  = 0;  
    nBranches_->genParticle_N = genParticles_->size();
    for( unsigned p=0; p<genParticles_->size(); ++p ){
      if( (*genParticles_)[p].status() != 1 || abs((*genParticles_)[p].pdgId()) > 30 || abs((*genParticles_)[p].pdgId()) == 22 )  {
        if(abs((*genParticles_)[p].pdgId()) != 23)
            continue;
      }
      //edm::LogError("") << "Particle index: " << p;
        //int found = std::distance( genParticles_->begin(), find(genParticles_->begin(), genParticles_->end(), (*genParticles_)[p]));
        ////if(found != cands.end()) index = found - cands.begin() ; 
        //if ((*genParticles_)[p].pdgId() == 23 ) {
        //    edm::LogError("") << "Index: " << found;
//      //  vMoth.push_back( index );
        //}

      int motherZindex = getMotherZIndex(*genParticlesC_.product(), cands, &genParticlesC_->at(p));
      //edm::LogError("") << "Mother Z index: " << motherZindex;

      NewLorentzVector FSR_vector = getFSRPhotonSum(*genParticlesC_.product(), cands, &genParticlesC_->at(p));
      //edm::LogError("") << motherZindex << FSR_vector.E();
      vDau.clear(); vMoth.clear(); vMothIdx.clear();
      nDau = 0; nMoth = 0;
      nBranches_->genParticle_pt    .push_back((*genParticles_)[p].pt()     );
      nBranches_->genParticle_px    .push_back((*genParticles_)[p].px()     );
      nBranches_->genParticle_py    .push_back((*genParticles_)[p].py()     );
      nBranches_->genParticle_pz    .push_back((*genParticles_)[p].pz()     );
      nBranches_->genParticle_eta   .push_back((*genParticles_)[p].eta()    );
      nBranches_->genParticle_mass  .push_back((*genParticles_)[p].mass()   );
      nBranches_->genParticle_phi   .push_back((*genParticles_)[p].phi()    );
      nBranches_->genParticle_e     .push_back((*genParticles_)[p].energy() );
      nBranches_->genParticle_status.push_back((*genParticles_)[p].status() );
      nBranches_->genParticle_pdgId .push_back((*genParticles_)[p].pdgId()  );
      nBranches_->genParticle_motherZindex.push_back(motherZindex);
      nBranches_->genParticle_FSR.push_back(FSR_vector);
      for( unsigned int d=0; d<(*genParticles_)[p].numberOfDaughters(); ++d ){
        vDau.push_back( (*genParticles_)[p].daughter(d)->pdgId() );
	    nDau++;
      }
      for( unsigned int m=0; m<(*genParticles_)[p].numberOfMothers(); ++m ){
        vMoth.push_back( (*genParticles_)[p].mother(m)->pdgId() );
       
        //int index = -2;
        int found = std::distance( cands.begin(), find(cands.begin(), cands.end(), (*genParticles_)[p].mother(m)));
        //if(found != cands.end()) index = found - cands.begin() ; 
        //if ((*genParticles_)[p].mother(m)->pdgId() == 23 ) {
   //         edm::LogError("") << "Index: " << found;
        vMothIdx.push_back(found  );
        //}
    	nMoth++;
      }
      nBranches_->genParticle_nDau  .push_back( nDau  );
      nBranches_->genParticle_nMoth .push_back( nMoth );      
      nBranches_->genParticle_mother.push_back( vMoth );
      nBranches_->genParticle_mother_Idx.push_back( vMothIdx );
      nBranches_->genParticle_dau   .push_back( vDau  );      
    }

    nBranches_->lheV_pt = 0.;
    nBranches_->lheNj = 0;
    nBranches_->lheHT = 0.;
}

